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
; File Name       : csp_spurious.s
; Description     : Default Function for Spurious Interrupt Handler
; Version         : 2.00
; Compiler Tools  : ADS / ASPEX / METAWARE
;
;       +----- (NEW | MODify | ADD | DELete)                                 
;       |                                                                    
;  No   |   when       who                what               
;-----+---+----------+------------------+--------------------------------------
; 000  NEW  01/05/99   Patrice VILCHEZ    Creation
; 001  MOD  01/04/01   Olivier MAZUYER    Clean up
; 002  MOD  14/01/02   Christophe GARDIN  Clean up
;------------------------------------------------------------------------------
         
;******************************************************************************
;* AREA Definition
;******************************************************************************
   AREA INTERRUPT, CODE, READONLY, INTERWORK
   CODE32


;******************************************************************************
;* Include Directive
;******************************************************************************
   INCLUDE   csp_arm.inc   
   INCLUDE   csp_gic.inc   


;******************************************************************************
;* Export/Import Directive
;******************************************************************************
   EXPORT  CSP_ASM_GICSpuriousIntHandler
   IMPORT  CSP_GICUndefinedIntHandler


;******************************************************************************
;* Spurious Handler
;******************************************************************************
CSP_ASM_GICSpuriousIntHandler

   IRQ_ENTRY
   BL     CSP_GICUndefinedIntHandler
   IRQ_EXIT
  
;******************************************************************************
   END


