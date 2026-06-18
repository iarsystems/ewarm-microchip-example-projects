;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Part one of the system initialization code,
;; contains low-level
;; initialization.
;;
;; Copyright 2007-2017 IAR Systems AB.
;;
;; $Revision$
;;

        MODULE  ?cstartup

        ;; Forward declaration of sections.

        ; Make sure that any stack which the application depends on is set up
        ; properly in the linker configuration.

        SECTION STACK:DATA:NOROOT(3)
        SECTION ROM:CODE:NOROOT(2)
;
; The module in this file is included in the libraries, and may be
; replaced by any user-defined modules that define the PUBLIC symbol
; __iar_program_start or a user defined start symbol.
;
; To override the cstartup defined in the library, simply add your
; modified version to the workbench project.

#include "hardware.h"
#include "mon_macros.h"

        SECTION .intvec:CODE:NOROOT(2)

        PUBLIC  __vector
        PUBLIC  __iar_program_start
        PUBLIC  _go
        EXTERN  Undefined_Handler
        EXTERN  SWI_Handler
        EXTERN  Prefetch_Handler
        EXTERN  Abort_Handler
        EXTERN  IRQ_Handler
        EXTERN  FIQ_Handler
//        EXTERN  Hyp_Handler

        DATA

__iar_init$$done:               ; The vector table is not needed
                                ; until after copy initialization is done

__vector:                       ; Make this a DATA label, so that stack usage
                                ; analysis doesn't consider it an uncalled fun

        ARM

        ; All default exception handlers (except reset) are
        ; defined as weak symbol definitions.
        ; If a handler is defined by the application it will take precedence.
        LDR     PC,Reset_Addr           ; Reset
        LDR     PC,Undefined_Addr       ; Undefined instructions
        LDR     PC,SWI_Addr             ; Software interrupt (SWI/SVC)
        LDR     PC,Prefetch_Addr        ; Prefetch abort
        LDR     PC,Abort_Addr           ; Data abort
        DCD     SIZEOF(ROM)             ;
        LDR     PC,IRQ_Addr             ; IRQ
        LDR     PC,FIQ_Addr             ; FIQ

        DATA

Reset_Addr:     DCD   __iar_program_start
Undefined_Addr: DCD   Undefined_Handler
SWI_Addr:       DCD   SWI_Handler
Prefetch_Addr:  DCD   Prefetch_Handler
Abort_Addr:     DCD   Abort_Handler
IRQ_Addr:       DCD   IRQ_Handler
FIQ_Addr:       DCD   FIQ_Handler
//Hyp_Addr:       DCD   Hyp_Handler


; --------------------------------------------------
; ?cstartup -- low-level system initialization code.
;
; After a reset execution starts here, the mode is ARM, supervisor
; with interrupts disabled.
;



        SECTION .text:CODE:NOROOT(2)

        EXTERN  __cmain
        REQUIRE __vector
        EXTWEAK __iar_init_core
        EXTWEAK __iar_init_vfp


        ARM

__iar_program_start:
?cstartup:

;
; Add initialization needed before setup of stackpointers here.
;

;
; Initialize the stack pointers.
; The pattern below can be used for any of the exception stacks:
; FIQ, IRQ, SVC, ABT, UND, SYS.
; The USR mode uses the same stack as SYS.
; The stack segments must be defined in the linker command file,
; and be declared above.
;


; --------------------
; Mode is bits 0-3 in CPSR, bit 4 is always 1.

#define MODE_MSK 0x1F            ; Bit mask for mode bits in CPSR

#define USR_MODE 0x10            ; User mode
#define FIQ_MODE 0x11            ; Fast Interrupt Request mode
#define IRQ_MODE 0x12            ; Interrupt Request mode
#define SVC_MODE 0x13            ; Supervisor mode
#define MON_MODE 0x16            ; Monitor mode
#define ABT_MODE 0x17            ; Abort mode
#define HYP_MODE 0x1A            ; Hypervisor mode
#define UND_MODE 0x1B            ; Undefined Instruction mode
#define SYS_MODE 0x1F            ; System mode

#if defined(CONFIG_CPU_V7)
        /*
        * We must configure VBAR for the reset vectors to be at
        * the start of SRAM (CONFIG_LINK_ADDR)
        */
        MRC     p15, 0, r2, c12, c0, 0 /* Read VBAR into R2 */
        LDR	r2,=__vector
        MCR	p15, 0, r2, c12, c0, 0
#endif /* CONFIG_CPU_V7 */

#ifdef BACKUP_REGISTER_BOOT_MODE_R4
        LDR	r1, =BACKUP_REGISTER_BOOT_MODE_R4
        STR	r4, [r1]
#endif

/* Init the stack */
        LDR     r1, =SFE(STACK)        ; End of CSTACK
        BIC     sp,r1,#0x7              ; Make sure SP is 8 aligned

/* Save BootROM supplied boot source information to stack */
        PUSH	{r4}

;;;
;;; Add more initialization here
;;;

#ifdef CONFIG_FLASH
/*
 * When running from NOR, we must relocate to SRAM prior to resetting
 * the clocks and SMC timings.
 */
_relocate_to_sram:

#if 0
/* relocation is slow, disable the watchdog or it will trigger */
        LDR	r1, =0xFFFFFD44
        MOV	r2, #0x00008000
        STR	r2, [r1]
#endif

        MOV	r1, #0
        LDR	r3, =SFB(ROM)
        LDR	r4, =SFE(ROM)
loop1:
        CMP     r3, r4
        LDRCC   r2, [r1], #4
        STRCC   r2, [r3], #4
        bcc     loop1
#endif /* CONFIG_FLASH */

#if defined(CONFIG_PMC_COMMON)
        EXTERN  lowlevel_clock_init
        LDR     r4, =lowlevel_clock_init
        MOV     lr, pc
        BX      r4
#endif /* CONFIG_PMC */

#if defined(CONFIG_CPU_V7)
/* Clear Abort condition if it is pending with help of the abort handler */
        LDR	r1, =(CPSR_A_BIT)
        MRS	r0, cpsr
        EOR	r0, r0, r1
        MSR	cpsr_x, r0
        NOP
        EOR	r0, r0, r1
        MSR	cpsr_x, r0
#endif

;;; Continue to __cmain for C-level initialization.       
        FUNCALL __iar_program_start, __cmain
        B       __cmain

/* Branch to the application at the end of the bootstrap init */
_go:
      	LDR	r1, =(SFE(STACK) - 4)
      	LDR	r4, [r1]
      	LDR r1, =MACH_TYPE
      	MOV lr, pc

/* Notifiy the debugger by setting a breakpoint */
#ifdef CONFIG_BKPT_NOTIFY_DONE
        BKPT    0xAB
#endif

/* we endless loop here and not jump to any application
 * if we are configured to not load anything
 */
#if defined(CONFIG_ENTER_NWD)
loop:
        B	loop
#endif

        BX r0

/*#ifdef CONFIG_THUMB*/

        SECTION .text:CODE:NOROOT(2)
        PUBLIC  set_cp15

set_cp15:
        MCR p15, 0, r0, c1, c0, 0
        BX  lr

        SECTION .text:CODE:NOROOT(2)
        PUBLIC  get_cp15

get_cp15:
        MRC p15, 0, r0, c1, c0, 0
        BX  lr

        SECTION .text:CODE:NOROOT(2)
        PUBLIC  disable_irq

disable_irq:
        MRS	r0, cpsr
        ORR r0, r0, #0xc0
        MSR	cpsr_c, r0
        BX	lr

        SECTION .text:CODE:NOROOT(2)
        PUBLIC  get_cpsr

get_cpsr:
        MRS r0, cpsr
        BX	lr

        SECTION .text:CODE:NOROOT(2)
        PUBLIC  set_cpsr

set_cpsr:
        MSR cpsr_c, r0
        BX	lr

        SECTION .text:CODE:NOROOT(2)
        PUBLIC  disable_icache

disable_icache:
        MRC p15, 0, r0, c1, c0, 0
        MVN	r1, #(1 << 12)
        AND r0, r0, r1
        MCR	p15, 0, r0, c1, c0, 0
        BX	lr

        SECTION .text:CODE:NOROOT(2)
        PUBLIC  disable_dcache

disable_dcache:
        MRC p15, 0, r0, c1, c0, 0
        MVN	r1, #(1 << 2)
        AND r0, r0, r1
        MCR	p15, 0, r0, c1, c0, 0
        BX	lr

        SECTION .text:CODE:NOROOT(2)
        PUBLIC  flush_idcache

flush_idcache:
        MOV	r0, #0
        MCR p15, 0, r0, c7, c7, 0
        BX	lr

/*#endif*/

#if defined(CONFIG_MMU)
        ARM
        
        SECTION .text:CODE:NOROOT(2)
        PUBLIC  cp15_read_sctlr
        
cp15_read_sctlr:
        MRC	p15, 0, r0, c1, c0, 0
        BX	lr

        SECTION .text:CODE:NOROOT(2)
        PUBLIC  cp15_write_sctlr
        
cp15_write_sctlr:
        MCR	p15, 0, r0, c1, c0, 0
        BX	lr

        SECTION .text:CODE:NOROOT(2)
        PUBLIC  cp15_write_ttbr0

cp15_write_ttbr0:
        MCR	p15, 0, r0, c2, c0, 0
        BX	lr

        SECTION .text:CODE:NOROOT(2)
        PUBLIC  cp15_write_dacr
        
cp15_write_dacr:
        MCR	p15, 0, r0, c3, c0, 0
        BX	lr

        SECTION .text:CODE:NOROOT(2)
        PUBLIC  cp15_icache_invalidate
        
cp15_icache_invalidate:
        MCR	p15, 0, r0, c7, c5, 0
        BX	lr

        SECTION .text:CODE:NOROOT(2)
        PUBLIC  cp15_dcache_invalidate_setway
        
cp15_dcache_invalidate_setway:
        MCR	p15, 0, r0, c7, c6, 2
        BX	lr

        SECTION .text:CODE:NOROOT(2)
        PUBLIC  cp15_dcache_clean_setway
        
cp15_dcache_clean_setway:
        MCR	p15, 0, r0, c7, c10, 2
        BX	lr

        SECTION .text:CODE:NOROOT(2)
        PUBLIC  cp15_dcache_invalidate_mva
        
cp15_dcache_invalidate_mva:
        MCR	p15, 0, r0, c7, c6, 1
        BX	lr

        SECTION .text:CODE:NOROOT(2)
        PUBLIC  dsb

dsb:
        MCR	p15, 0, r0, c7, c10, 4
        BX	lr
#endif
        END
