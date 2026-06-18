;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Part one of the system initialization code,
;; contains low-level
;; initialization.
;;
;; Copyright 2006 IAR Systems. All rights reserved.
;;
;; $Revision: 5221 $
;;

;
; Naming covention of labels in this file:
;
;  ?xxx   - External labels only accessed from assembler.
;  __xxx  - External labels accessed from or defined in C.
;  xxx    - Labels local to one module (note: this file contains
;           several modules).
;  main   - The starting point of the user program.
;


;---------------------------------------------------------------
; Macros and definitions for the whole file
;---------------------------------------------------------------

; Mode, correspords to bits 0-5 in CPSR
MODE_BITS DEFINE  0x1F    ; Bit mask for mode bits in CPSR
USR_MODE  DEFINE  0x10    ; User mode
FIQ_MODE  DEFINE  0x11    ; Fast Interrupt Request mode
IRQ_MODE  DEFINE  0x12    ; Interrupt Request mode
SVC_MODE  DEFINE  0x13    ; Supervisor mode
ABT_MODE  DEFINE  0x17    ; Abort mode
UND_MODE  DEFINE  0x1B    ; Undefined Instruction mode
SYS_MODE  DEFINE  0x1F    ; System mode

;---------------------------------------------------------------
; Reset Vector.
; Normally, segment INTVEC is linked at address 0.
; For debugging purposes, INTVEC may be placed at other
; addresses.
; A debugger that honors the entry point will start the
; program in a normal way even if INTVEC is not at address 0.
;---------------------------------------------------------------


        SECTION .intvec:CODE:NOROOT(2)
        PUBLIC  __vector
        PUBLIC  __iar_program_start

        ARM
__vector:
        B   __iar_program_start             ;; Reset
__und_handler:
        ldr  pc,[pc,#+24]             ;; Undefined instructions
__swi_handler:
        ldr  pc,[pc,#+24]             ;; Software interrupt (SWI/SVC)
__prefetch_handler:
        ldr  pc,[pc,#+24]             ;; Prefetch abort
__data_handler:
        ldr  pc,[pc,#+24]             ;; Data abort
        DC32  0xFFFFFFFF              ;; RESERVED
__irq_handler:
        ldr  pc,[pc,#+24]             ;; IRQ
__fiq_handler:
        ldr  pc,[pc,#+24]             ;; FIQ

        DC32  __iar_program_start
        DC32  __und_handler
        DC32  __swi_handler
        DC32  __prefetch_handler
        DC32  __data_handler
        B .
        DC32  __irq_handler
        DC32  __fiq_handler


;---------------------------------------------------------------
; ?BOOT
;---------------------------------------------------------------
		SECTION	text:CODE:NOROOT(2)
    REQUIRE __vector
		ARM
		PUBLIC __iar_program_start
__iar_program_start:
#if AT91_REMAP
                ; The memory controller is initialized immediately before the remap
                ldr     r10, =EBI_init_table ; EBI register initialization table
                ; If pc > 0x100000
                movs    r0, pc, LSR #20
                ; Mask the 12 highest bits of the address
                moveq   r10, r10, LSL #12
                moveq   r10, r10, LSR #12

                ; Load the address where to jump
                ldr     r12, =after_remap ; get the real jump address ( after remap )

                ; Copy chip select register image to memory controller and command remap
                ldmia   r10!, {r0-r9,r11}       ; load the complete image and the EBI base
                stmia   r11!, {r0-r9}           ; store the complete image with the remap command

                ; jump to ROM at its new address
                ; this instruction was loaded into the pipeline before the remap was done
                mov         pc, r12             ; jump and break the pipeline

                ; Put constant table here.
                LTORG

                ; EBI initialization table
                ; 32,768 MHz master clock assumed for timing
EBI_init_table:
                dc32        0x0100252d  ; Flash at 0x01000000, 16MB, 2 hold, 16 bits, 4 WS
                dc32        0x02002121  ; RAM   at 0x02000000,  1MB, 0 hold, 16 bits, 1 WS
                dc32        0x20000000  ; unused
                dc32        0x30000000  ; unused
                dc32        0x40000000  ; unused
                dc32        0x50000000  ; unused
                dc32        0x60000000  ; unused
                dc32        0x70000000  ; unused
                dc32        0x00000001  ; REMAP command
                dc32        0x00000006  ; standard read
                dc32        __EBI_CSR0  ; EBI Base address

after_remap:
#endif
                ; Execute C startup code.
                b ?cstartup


;---------------------------------------------------------------
; ?CSTARTUP
;---------------------------------------------------------------

    SECTION FIQ_STACK:DATA:NOROOT(3)
    SECTION IRQ_STACK:DATA:NOROOT(3)
    SECTION SVC_STACK:DATA:NOROOT(3)
    SECTION ABT_STACK:DATA:NOROOT(3)
    SECTION UND_STACK:DATA:NOROOT(3)
    SECTION CSTACK:DATA:NOROOT(3)
		SECTION text:CODE:NOROOT(2)
		EXTERN	?main
		ARM
?cstartup
; Execution starts here.
; After a reset, the mode is ARM, Supervisor, interrupts disabled.

; Add initialization nedded before setup of stackpointers here

; Initialize the stack pointers.
; The pattern below can be used for any of the exception stacks:
; FIQ, IRQ, SVC, ABT, UND, SYS.
; The USR mode uses the same stack as SYS.
; The stack segments must be defined in the linker command file,
; and be declared above.

                mrs     r0,cpsr                             ; Original PSR value
                bic     r0,r0,#MODE_BITS                    ; Clear the mode bits
                orr     r0,r0,#SVC_MODE                     ; Set SVC mode bits
                msr     cpsr_c,r0                           ; Change the mode
                ldr     sp,=SFE(SVC_STACK)                  ; End of SVC_STACK

                bic     r0,r0,#MODE_BITS                    ; Clear the mode bits
                orr     r0,r0,#UND_MODE                     ; Set UND mode bits
                msr     cpsr_c,r0                           ; Change the mode
                ldr     sp,=SFE(UND_STACK)                  ; End of UND_STACK

                bic     r0,r0,#MODE_BITS                    ; Clear the mode bits
                orr     r0,r0,#ABT_MODE                     ; Set ABT mode bits
                msr     cpsr_c,r0                           ; Change the mode
                ldr     sp,=SFE(ABT_STACK)                  ; End of ABT_STACK

                bic     r0,r0,#MODE_BITS                    ; Clear the mode bits
                orr     r0,r0,#FIQ_MODE                     ; Set FIQ mode bits
                msr     cpsr_c,r0                           ; Change the mode
                ldr     sp,=SFE(FIQ_STACK)                  ; End of FIQ_STACK

                bic     r0,r0,#MODE_BITS                    ; Clear the mode bits
                orr     r0,r0,#IRQ_MODE                     ; Set IRQ mode bits
                msr     cpsr_c,r0                           ; Change the mode
                ldr     sp,=SFE(IRQ_STACK)                  ; End of IRQ_STACK

                bic     r0,r0,#MODE_BITS                    ; Clear the mode bits
                orr     r0,r0,#SYS_MODE                     ; Set System mode bits
                msr     cpsr_c,r0                           ; Change the mode
                ldr     sp,=SFE(CSTACK)                     ; End of CSTACK

#ifdef __ARMVFP__
; Enable the VFP coprocessor.
                mov     r0, #0x40000000                 ; Set EN bit in VFP
                fmxr    fpexc, r0                       ; FPEXC, clear others.

; Disable underflow exceptions by setting flush to zero mode.
; For full IEEE 754 underflow compliance this code should be removed
; and the appropriate exception handler installed.
                mov     r0, #0x01000000		        ; Set FZ bit in VFP
                fmxr    fpscr, r0                       ; FPSCR, clear others.
#endif

; Add more initialization here


; Continue to ?main for more IAR specific system startup

                ldr     r0,=?main
                bx      r0

                END
