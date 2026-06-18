;------------------------------------------------------------------------------
;-         ATMEL Microcontroller Software Support  -  ROUSSET  -
;------------------------------------------------------------------------------
; The software is delivered "AS IS" without warranty or condition of any
; kind, either express, implied or statutory. This includes without
; limitation any warranty or condition with respect to merchantability or
; fitness for any particular purpose, or against the infringements of
; intellectual property rights of others.
;-----------------------------------------------------------------------------
;- File source          : Cstartup.s79
;- Object               : Generic CStartup for IAR Use REMAP
;- Compilation flag     : Flash_DEBUG for Flash and Bin generation
;-			  none for RAM debug area
;- 1.0 22/Jun/05 JPP    : Creation
;------------------------------------------------------------------------------

;------------------------------------------------------------------------------
;- Area Definition
;------------------------------------------------------------------------------

;---------------------------------------------------------------
; ?RESET
; Reset Vector.
; Normally, segment INTVEC is linked at address 0.
; For debugging purposes, INTVEC may be placed at other
; addresses.
; A debugger that honors the entry point will start the
; program in a normal way even if INTVEC is not at address 0.
;-------------------------------------------------------------


		SECTION	.intvec:CODE:ROOT(2)
        PUBLIC  __vector
        PUBLIC  __iar_program_start


AT91C_BASE_GIC		EQU     0xFFFFF000
GIC_FVR                 EQU     (260)
GIC_IVR                 EQU     (256)
GIC_EOICR               EQU     (304)
AT91C_AMC_RCB           EQU      0x01

;------------------------------------------------------------------------------
;- Exception vectors
;--------------------
;- These vectors can be read at address 0 or at RAM address
;- They ABSOLUTELY requires to be in relative addresssing mode in order to
;- guarantee a valid jump. For the moment, all are just looping.
;- If an exception occurs before remap, this would result in an infinite loop.
;- To ensure if a exeption occurs before start application to infinite loop.
;------------------------------------------------------------------------------
		  ARM	; Always ARM mode after reset	

__vector:
                B           __iar_program_start ; 0x00 Reset handler
undefvec:
                B           undefvec            ; 0x04 Undefined Instruction
swivec:
                B           swivec              ; 0x08 Software Interrupt
pabtvec:
                B           pabtvec             ; 0x0C Prefetch Abort
dabtvec:
                B           dabtvec             ; 0x10 Data Abort
rsvdvec:
                B           rsvdvec             ; 0x14 reserved
irqvec:         ldr         pc, [pc,#-0xF20]    ; 0x18 IRQ

fiqvec:                                         ; 0x1C FIQ
;------------------------------------------------------------------------------
;- Function             : asm_FIQ_Handlesr
;- Treatments           : FIQ Controller Interrupt Handler.
;- Called Functions     : GIC_FVR[interrupt]
;------------------------------------------------------------------------------
                mov         r9,r0
;- Switch in SVC/User Mode to allow User Stack access for C code
; because the FIQ is not yet acknowledged

;- Save and r0 in FIQ_Register
 	          ldr         r0 , [r8, #GIC_FVR]
            msr         CPSR_c,#I_BIT | F_BIT | ARM_MODE_SVC
;- Save scratch/used registers and LR in User Stack
            stmfd       sp!, { r1-r3, r12, lr}

;- Branch to the routine pointed by the GIC_FVR
            mov         r14, pc
            bx          r0
;- Restore scratch/used registers and LR from User Stack
            ldmia       sp!, { r1-r3, r12, lr}

;- Leave Interrupts disabled and switch back in FIQ mode
            msr         CPSR_c, #I_BIT | F_BIT | ARM_MODE_FIQ

;- Restore the R0 ARM_MODE_SVC register
            mov         r0,r9

;- Restore the Program Counter using the LR_fiq directly in the PC
            subs        pc,lr,#4
End_Vector:
__iar_program_start:
;------------------------------------------------------------------------------
;- Low level Init (PMC, GIC, ? ....) by C function AT91F_LowLevelInit
;------------------------------------------------------------------------------
            EXTERN  AT91F_LowLevelInit
            EXTERN  iRAM_start_before_remap
            EXTERN  iRAM_end_before_remap
            EXTERN  iRAM_remap
            EXTERN  iRAM_code_start

#define  __iramend_remap 	iRAM_end_before_remap
#define  __iramSart_remap	iRAM_start_before_remap
#define  __icodeStart     iRAM_code_start

;- For Flash definition
#ifdef  Flash_DEBUG
MaskkAdd          	EQU   0x00FFFFFF
#else
;- For Ice definition
MaskkAdd	        EQU   0xFFFFFFFF
#endif

;- minumum C initialization
;- call  AT91F_LowLevelInit( void)
	    ldr	    r4,=AT91F_LowLevelInit
	    ldr	    r1,=MaskkAdd
	    and     r4,r4,r1

      ldr     r13,=__iramend_remap     ; temporary stack in internal RAM
      ldr     r0,=__icodeStart
      ldr     r1,=__iramSart_remap
      ldr     r2,= End_Vector-__vector

;--Call Low level init function in ABSOLUTE through the Interworking
      mov     lr, pc
	    bx	    r4	

;--------------------------------------------
;- Remap Command and jump on ABSOLUTE address
;--------------------------------------------
            ldr     r12, PtInitRemap        ; Get the real jump address ( after remap )
            mov     r1,#AT91C_AMC_RCB       ; Get the REMAP value
            str     r1, [r0]                ; Store the complete image with the remap command

;- Jump to LINK address at its absolute address
            mov     pc, r12                 ; Jump and break the pipeline

PtInitRemap
            DCD     InitRemap               ; Address where to jump after REMAP
;------------------------------------------------------------------------------
;- The Reset Handler after Remap
;-------------------------------
;- From here, the code is executed from its link address, ie. 0x100 0000.
;------------------------------------------------------------------------------
InitRemap
	
;------------------------------------------------------------------------------
;- Stack Sizes Definition
;------------------------
;- Interrupt Stack requires 3 words x 8 priority level x 4 bytes when using
;- the vectoring. This assume that the IRQ management.
;- The Interrupt Stack must be adjusted depending on the interrupt handlers.
;- Fast Interrupt not requires stack If in your application it required you must
;- be definehere.
;- The System stack size is not defined and is limited by the free internal
;- SRAM.
;------------------------------------------------------------------------------

;------------------------------------------------------------------------------
;- Top of Stack Definition
;-------------------------
;- Interrupt and Supervisor Stack are located at the top of internal memory in
;- order to speed the exception handling context saving and restoring.
;- ARM_MODE_SVC (Application, C) Stack is located at the top of the external memory.
;------------------------------------------------------------------------------

IRQ_STACK_SIZE          EQU     (3*8*4)     ; 3 words per interrupt priority level

ARM_MODE_FIQ            EQU     0x11
ARM_MODE_IRQ            EQU     0x12
ARM_MODE_SVC            EQU     0x13

I_BIT                   EQU     0x80
F_BIT                   EQU     0x40

;------------------------------------------------------------------------------
;- Setup the stack for each mode
;-------------------------------
#define  __iramend 	iRAM_remap

                ldr     r0, =__iramend
;- Set up Fast Interrupt Mode and set FIQ Mode Stack
                msr     CPSR_c, #ARM_MODE_FIQ | I_BIT | F_BIT
;- Init the FIQ register
            	  ldr     r8, =AT91C_BASE_GIC

;- Set up Interrupt Mode and set IRQ Mode Stack
                msr     CPSR_c, #ARM_MODE_IRQ | I_BIT | F_BIT
                mov     r13, r0                     ; Init stack IRQ
                sub     r0, r0, #IRQ_STACK_SIZE

;- Enable interrupt & Set up Supervisor Mode and set Supervisor Mode Stack
                msr     CPSR_c, #ARM_MODE_SVC
                mov     r13, r0

;---------------------------------------------------------------
; ?CSTARTUP
;---------------------------------------------------------------
		EXTERN	?main
                ldr     r0,=?main
                bx      r0

;---------------------------------------------------------------
; ?EXEPTION_VECTOR
; This module is only linked if needed for closing files.
;---------------------------------------------------------------
		PUBLIC	AT91F_Default_FIQ_handler
		PUBLIC	AT91F_Default_IRQ_handler
		PUBLIC	AT91F_Spurious_handler

		ARM	; Always ARM mode after exeption	

AT91F_Default_FIQ_handler
            b     AT91F_Default_FIQ_handler

AT91F_Default_IRQ_handler
            b     AT91F_Default_IRQ_handler

AT91F_Spurious_handler
            b     AT91F_Spurious_handler

	END

