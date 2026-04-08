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
; File Name       : csp_irq.s
; Description     : ARM7TDMI Core Exception Vectors
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
   AREA INTERRUPT, CODE, READONLY
   CODE32


;******************************************************************************
;* Export/Import Directive
;******************************************************************************
   EXPORT   Reset_Handler
   EXPORT   Undefined_Handler
   EXPORT   SWI_Handler
   EXPORT   Prefetch_Handler
   EXPORT   Abort_Handler
   EXPORT   IRQ_Handler
   EXPORT   FIQ_Handler


;******************************************************************************
;* Reset Exception Handler
;******************************************************************************
Reset_Handler
   B     Reset_Handler

;******************************************************************************
;* Undefine Exception Handler
;******************************************************************************
Undefined_Handler
   B     Undefined_Handler

;******************************************************************************
;* Software Interrupt Exception Handler
;******************************************************************************
SWI_Handler
   B     SWI_Handler

;******************************************************************************
;* Prefecht Abort Exception Handler
;******************************************************************************
Prefetch_Handler
   B     Prefetch_Handler

;******************************************************************************
;* Abort Exception Handler
;******************************************************************************
Abort_Handler
   B     Abort_Handler

;******************************************************************************
;* IRQ Exception Handler
;******************************************************************************
IRQ_Handler
   B     IRQ_Handler

;******************************************************************************
;* FIQ Exception Handler
;******************************************************************************
FIQ_Handler
   B     FIQ_Handler

;******************************************************************************
   END
