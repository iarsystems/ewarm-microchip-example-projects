;------------------------------------------------------------------------------
;-         ATMEL Microcontroller Software Support  -  ROUSSET  -
;------------------------------------------------------------------------------
; The software is delivered "AS IS" without warranty or condition of any
; kind, either express, implied or statutory. This includes without
; limitation any warranty or condition with respect to merchantability or
; fitness for any particular purpose, or against the infringements of
; intellectual property rights of others.
;-----------------------------------------------------------------------------
;- File source          : swirq0_Handler.s79
;- Object               : Generic IRQ Handler
;- Compilation flag     : None
;- 1.0 22/Jun/05 JPP    : Creation
;------------------------------------------------------------------------------


;------------------------------------------------------------------------------
;- Area Definition
;------------------------------------------------------------------------------

;---------------------------------------------------------------
; ?asm_SWIRQ0_Handler_Entry
; Reset Vector.
; Normally, segment INTVEC is linked at address 0.
; For debugging purposes, INTVEC may be placed at other
; addresses.
; A debugger that honors the entry point will start the
; program in a normal way even if INTVEC is not at address 0.
;-------------------------------------------------------------

		PROGRAM	?asm_SWIRQ0_Handler_Entry

		SECTION	text:CODE:ROOT(2)
		ARM	; Always ARM mode after reset	

AT91C_BASE_GIC		EQU     0xFFFFF000
GIC_IVR                 EQU     (256)
GIC_EOICR               EQU     (304)
ARM_MODE_IRQ            EQU     0x12
ARM_MODE_SVC            EQU     0x13
I_BIT                   EQU     0x80
;------------------------------------------------------------------------------
;- Manage exception
;---------------
;- This module The exception must be ensure in ARM mode
;------------------------------------------------------------------------------
;------------------------------------------------------------------------------
;- Function             : asm_SWIRQ0_Handler_Entry
;- Treatments           : IRQ Controller Interrupt Handler.
;------------------------------------------------------------------------------
	PUBLIC	asm_SWIRQ0_Handler_Entry
  EXTERN  c_software_interrupt
asm_SWIRQ0_Handler_Entry:

;- Manage Exception Entry
;- Adjust and save LR_irq in IRQ stack
            sub         lr, lr, #4
            stmfd       sp!, {lr}

;- Save SPSR need to be saved for nested interrupt
            mrs         r14, SPSR
            stmfd       sp!, {r14}

;- Save and r0 in IRQ stack
            stmfd       sp!, {r0}

;- De-assert the NIRQ
            ldr         r14, =AT91C_BASE_GIC
	          str         r14, [r14, #GIC_IVR]

;- Enable Interrupt and Switch in Supervisor Mode
            msr         CPSR_c, #ARM_MODE_SVC

;- Save scratch/used registers and LR in User Stack
            stmfd       sp!, { r1-r3, r12, r14}

;- Branch to the routine pointed by the GIC_IVR
	    ldr		r0, = c_software_interrupt
            mov         r14, pc
            bx          r0

;- Restore scratch/used registers and LR from User Stack
            ldmia       sp!, { r1-r3, r12, r14}

;- Disable Interrupt and switch back in IRQ mode
            msr         CPSR_c, #I_BIT | ARM_MODE_IRQ

;- Mark the End of Interrupt on the GIC
            ldr         r14, =AT91C_BASE_GIC
            str         r14, [r14, #GIC_EOICR]

;- Restore R0
            ldmia       sp!, {r0}

;- Restore SPSR_irq and r0 from IRQ stack
            ldmia       sp!, {r14}
            msr         SPSR_cxsf, r14

;- Restore adjusted  LR_irq from IRQ stack directly in the PC
            ldmia       sp!, {pc}^

	END

