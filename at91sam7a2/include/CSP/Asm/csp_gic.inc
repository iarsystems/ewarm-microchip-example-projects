;------------------------------------------------------------------------------
;   EUROPE TECHNOLOGIES Software Support
;------------------------------------------------------------------------------
; The software is delivered "AS IS" without warranty or condition of any
; kind, either express, implied or statutory. This includes without
; limitation any warranty or condition with respect to merchantability or
; fitness for any particular purpose, or against the infringements of
; intellectual property rights of others.
;------------------------------------------------------------------------------
;
; Processor       : ARM7TDMI
; File Name       : csp_gic.inc
; Description     : Advanced Interrupt Controller Definition File
;                   Exception Entry and Exit Macro
; Version         : 2.00
; Compiler Tools  : ADS / ASPEX / METAWARE
;
;       +----- (NEW | MODify | ADD | DELete)                                 
;       |                                                                    
;  No   |   when       who                what               
;-----+---+----------+------------------+--------------------------------------
; 000  NEW  01/05/99   Patrice VILCHEZ    Creation
; 001  ADD  13/04/00   Patrice VILCHEZ    Clean up
; 002  MOD  01/04/01   Olivier MAZUYER    Clean up
; 003  MOD  14/01/02   Christophe GARDIN  Clean up
;------------------------------------------------------------------------------

;******************************************************************************
;* AREA Definition
;******************************************************************************
   CODE32


;******************************************************************************
;************************ GIC Structure Definition ****************************
;******************************************************************************
GIC_BASE   	EQU   0xFFFFF000        ; Address of controller

GIC_SMR     EQU   GIC_BASE          ; Source Mode Register
GIC_SVR     EQU   GIC_SMR   + 32*4  ; Source Vector Register
GIC_IVR     EQU   GIC_SVR   + 32*4  ; IRQ Vector Register
GIC_FVR     EQU   GIC_IVR   + 4     ; FIQ Vector Register
GIC_ISR     EQU   GIC_FVR   + 4     ; Interrupt Status Register
GIC_IPR     EQU   GIC_ISR   + 4     ; Interrupt Pending Register
GIC_IMR     EQU   GIC_IPR   + 4     ; Interrupt Mask Register
GIC_CISR    EQU   GIC_IMR   + 4     ; Core Interrupt Status Register
GIC_IECR    EQU   GIC_CISR  + 3*4   ; Interrupt Enable Command Register
GIC_IDCR    EQU   GIC_IECR  + 4     ; Interrupt Disable Command Register
GIC_ICCR    EQU   GIC_IDCR  + 4     ; Interrupt Clear Command Register
GIC_ISCR    EQU   GIC_ICCR  + 4     ; Interrupt Set Command Register
GIC_EOICR   EQU   GIC_ISCR  + 4     ; End of Interrupt Command Register
GIC_SPU     EQU   GIC_EOICR + 4     ; Spurious Vector Register


;******************************************************************************
;************************* GIC Registers Definition ***************************
;******************************************************************************

;******************************************************************************
;* SMR : GIC Source Mode Register
;******************************************************************************
GIC_PRIOR                  EQU   0x07     ; Priority

GIC_SRCTYPE                EQU   0x60     ; Source Type Definition
GIC_SRCTYPE_LOW_LEVEL      EQU   0x00     ; Low Level Sensitive
GIC_SRCTYPE_NEGATIVE_EDGE  EQU   0x40     ; Negative Edge
GIC_SRCTYPE_HIGH_LEVEL     EQU   0x80     ; High Level
GIC_SRCTYPE_POSITIVE_EDGE  EQU   0x60     ; Positive Edge

;******************************************************************************
;* ISR : Interrupt Status Register
;******************************************************************************
GIC_IRQID   EQU   0x1F              ; Current Source Interrupt

;******************************************************************************
;* CISR : Interrupt Core Status Register
;******************************************************************************
GIC_NFIQ    EQU   0x01              ; Core FIQ Status
GIC_NIRQ    EQU   0x02              ; Core IRQ Status


;******************************************************************************
;*************************** GIC Macros Definition ****************************
;******************************************************************************

;******************************************************************************
;* IRQ Entry
;******************************************************************************
   MACRO
   IRQ_ENTRY

; Adjust and save LR_irq in IRQ stack
   sub   r14, r14, #4
   stmfd sp!, {r14}

; Save SPSR and r0 in IRQ stack
   mrs   r14, SPSR
   stmfd sp!, {r0, r14}

; Enable Interrupt and Switch in SYS Mode
   mrs   r0, CPSR
   bic   r0, r0, #I_BIT
   orr   r0, r0, #ARM_MODE_SYS
   msr   CPSR_c, r0

; Save scratch/used registers and LR in User Stack
   stmfd sp!, { r1-r3, r12, r14}

   MEND

;******************************************************************************
;* IRQ Exit
;******************************************************************************
   MACRO
   IRQ_EXIT

; Restore scratch/used registers and LR from User Stack
   ldmia sp!, { r1-r3, r12, r14}

; Disable Interrupt and switch back in IRQ mode
   mrs   r0, CPSR
   bic   r0, r0, #ARM_MODE_SYS
   orr   r0, r0, #I_BIT:OR:ARM_MODE_IRQ
   msr   CPSR_c, r0

; Mark the End of Interrupt on the GIC
   ldr   r0, =GIC_EOICR
   str   r0, [r0]

; Restore SPSR_irq and r0 from IRQ stack
   ldmia sp!, {r0, r14}
   msr   SPSR_cxsf, r14

; Restore adjusted  LR_irq from IRQ stack directly in the PC
   ldmia sp!, {pc}^

   MEND

;******************************************************************************
;* FIQ Entry
;******************************************************************************
   MACRO
   FIQ_ENTRY

; Adjust and save LR_irq in FIQ stack
   sub   r14, r14, #4
   stmfd sp!, {r14}

; Save SPSR and r0 in FIQ stack
   mrs   r14, SPSR
   stmfd sp!, {r0, r14}

; Enable Interrupt and Switch in SYS Mode
   mrs   r0, CPSR
   bic   r0, r0, #F_BIT
   orr   r0, r0, #ARM_MODE_SYS
   msr   CPSR_c, r0

; Save scratch/used registers and LR in User Stack
   stmfd sp!, { r1-r3, r12, r14}

   MEND

;******************************************************************************
;* FIQ Exit
;******************************************************************************
   MACRO
   FIQ_EXIT

; Restore scratch/used registers and LR from User Stack
   ldmia sp!, { r1-r3, r12, r14}

; Disable Interrupt and switch back in FIQ mode
   mrs   r0, CPSR
   bic   r0, r0, #ARM_MODE_SYS
   orr   r0, r0, #F_BIT:OR:ARM_MODE_FIQ
   msr   CPSR_c, r0

; Mark the End of Interrupt on the GIC
   ldr   r0, =GIC_EOICR
   str   r0, [r0]

; Restore SPSR_irq and r0 from IRQ stack
   ldmia sp!, {r0, r14}
   msr   SPSR_cxsf, r14

; Restore adjusted  LR_irq from FIQ stack directly in the PC
   ldmia sp!, {pc}^

   MEND


;******************************************************************************
   END
