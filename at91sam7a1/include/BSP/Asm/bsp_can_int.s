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
; File Name       : bsp_can_int.s (hardware dependent)
; Description     : Function declarations for CAN Interrupt Handler
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
   EXPORT  TestCAN_AsmCAN0InterruptHandler	; Assembler Interrupt Handler
   IMPORT  TestCAN_CAN0InterruptHandler 	   ; C Interrupt function


;******************************************************************************
;* CAN0 Handler
;******************************************************************************
TestCAN_AsmCAN0InterruptHandler
   IRQ_ENTRY
   bl    TestCAN_CAN0InterruptHandler
   IRQ_EXIT

;******************************************************************************
   END


