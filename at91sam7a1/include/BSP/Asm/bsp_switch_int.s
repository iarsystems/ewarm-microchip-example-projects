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
; Processor   : easyCAN3
; File Name   : BSP_SWITCH_INT.S
; Description : Interrupt handler Declaration for the on-board 3 switches
; Version     : 1.01
;
;       +----- (NEW | MODify | ADD | DELete)                                 
;       |                                                                    
;  No   |   when       who                what               
;-----+---+----------+------------------+--------------------------------------
; 000  NEW  06/07/01   David LEWIN        Creation from scratch
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
   EXPORT  BSP_AsmTIOB1InterruptHandler		; Assembler Interrupt Handler
   EXPORT  BSP_AsmTIOA2InterruptHandler		; Assembler Interrupt Handler
   EXPORT  BSP_AsmIRQ0InterruptHandler		   ; Assembler Interrupt Handler
   IMPORT  BSP_SWITCHTIOB1InterruptHandler 	; C Interrupt function
   IMPORT  BSP_SWITCHTIOA2InterruptHandler 	; C Interrupt function
   IMPORT  BSP_SWITCHIRQ0InterruptHandler 	; C Interrupt function


;******************************************************************************
;* ST0CH0 Handler
;******************************************************************************
BSP_AsmTIOB1InterruptHandler
   IRQ_ENTRY
   bl    BSP_SWITCHTIOB1InterruptHandler
   IRQ_EXIT
   
;******************************************************************************
;* ST0CH0 Handler
;******************************************************************************
BSP_AsmTIOA2InterruptHandler
   IRQ_ENTRY
   bl    BSP_SWITCHTIOA2InterruptHandler
   IRQ_EXIT

;******************************************************************************
;* ST0CH0 Handler
;******************************************************************************
BSP_AsmIRQ0InterruptHandler
   IRQ_ENTRY
   bl    BSP_SWITCHIRQ0InterruptHandler
   IRQ_EXIT

;******************************************************************************
   END


