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
; File Name       : bsp_adc_int.s (hardware dependent)
; Description     : Function declarations for ADC Interrupt Handler
; Library Version : 1.00
;
;       +----- (NEW | MODify | ADD | DELete)                                 
;       |                                                                    
;  No   |   When       Who                What               
;-----+---+----------+------------------+--------------------------------------
; 000  NEW  06/12/99   Patrice Vilchez    Creation 
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
   EXPORT  ADC0_AsmHandler		; Assembler Interrupt Handler
   IMPORT  ADC0_Handler 	   ; C Interrupt function


;******************************************************************************
;* ADC0 Handler
;******************************************************************************
ADC0_AsmHandler
  IRQ_ENTRY
  bl     ADC0_Handler
  IRQ_EXIT
    
;******************************************************************************
   END


