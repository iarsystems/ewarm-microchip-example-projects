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
; File Name       : bsp_st_int.s (hardware dependent)
; Description     : Function declarations for ST Interrupt Handler
; Library Version : 1.00
;
;       +----- (NEW | MODify | ADD | DELete)                                 
;       |                                                                    
;  No   |   When       Who                What               
;-----+---+----------+------------------+--------------------------------------
; 000  NEW  08/08/01   Frederic SAMSON    Creation 
; 001  MOD  25/03/02   Christophe GARDIN  Clean up
;------------------------------------------------------------------------------

;******************************************************************************
;* AREA Definition
;******************************************************************************
   AREA INTERRUPT, CODE, READONLY
   CODE32
  

;******************************************************************************
;* Include Directive
;******************************************************************************
   INCLUDE ../../CSP/asm/csp_arm.inc
   INCLUDE ../../CSP/asm/csp_gic.inc


;******************************************************************************
;* Export/Import Directive
;******************************************************************************
   EXPORT  ST0CH0_AsmInterruptHandler		; Assembler Interrupt Handler
   EXPORT  ST0CH1_AsmInterruptHandler		; Assembler Interrupt Handler
   EXPORT  ST1CH0_AsmInterruptHandler		; Assembler Interrupt Handler
   EXPORT  ST1CH1_AsmInterruptHandler		; Assembler Interrupt Handler
   IMPORT  ST0CH0_InterruptHandler 	      ; C Interrupt function
   IMPORT  ST0CH1_InterruptHandler 	      ; C Interrupt function
   IMPORT  ST1CH0_InterruptHandler 	      ; C Interrupt function
   IMPORT  ST1CH1_InterruptHandler 	      ; C Interrupt function


;******************************************************************************
;* ST0CH0 Handler
;******************************************************************************
ST0CH0_AsmInterruptHandler
   IRQ_ENTRY
   bl    ST0CH0_InterruptHandler
   IRQ_EXIT
    
;******************************************************************************
;* ST0CH1 Handler
;******************************************************************************
ST0CH1_AsmInterruptHandler
   IRQ_ENTRY
   bl    ST0CH1_InterruptHandler
   IRQ_EXIT

;******************************************************************************
;* ST1CH0 Handler
;******************************************************************************
ST1CH0_AsmInterruptHandler
   IRQ_ENTRY
   bl    ST1CH0_InterruptHandler
   IRQ_EXIT

;******************************************************************************
;* ST1CH1 Handler
;******************************************************************************
ST1CH1_AsmInterruptHandler
   IRQ_ENTRY
   bl    ST1CH1_InterruptHandler
   IRQ_EXIT

;******************************************************************************
   END


