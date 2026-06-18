;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Part one of the system initialization code,
;; contains low-level
;; initialization.
;;
;; Copyright 2006 IAR Systems. All rights reserved.
;;
;; $Revision: 10 $
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
        ldr  pc,[pc,#+24]             ;; Reset
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
        ldr PC,[PC,#-0xF20]           ;; IRQ
__fiq_handler:
        ldr  pc,[pc,#+24]             ;; FIQ

        DC32  __iar_program_start
        DC32  __und_handler
        DC32  __swi_handler
        DC32  __prefetch_handler
        DC32  __data_handler
        B .
        B .
        DC32  __fiq_handler


;---------------------------------------------------------------
; ?BOOT
;---------------------------------------------------------------

    SECTION FIQ_STACK:DATA:NOROOT(3)
    SECTION IRQ_STACK:DATA:NOROOT(3)
    SECTION SVC_STACK:DATA:NOROOT(3)
    SECTION ABT_STACK:DATA:NOROOT(3)
    SECTION UND_STACK:DATA:NOROOT(3)
    SECTION CSTACK:DATA:NOROOT(3)

		SECTION	text:CODE:NOROOT(2)
    REQUIRE __vector
    EXTERN ?main
		PUBLIC __iar_program_start

    ARM
__iar_program_start:

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
