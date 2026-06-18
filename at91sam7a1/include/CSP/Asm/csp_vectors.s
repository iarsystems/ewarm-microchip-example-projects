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
; File Name       : csp_vectors.s
; Description     : Exception Vectors Definition File
; Version         : 2.00
; Compiler Tools  : ADS / ASPEX / METAWARE 
;
;       +----- (NEW | MODify | ADD | DELete)                                 
;       |                                                                    
;  No   |   when       who                what               
;-----+---+----------+------------------+--------------------------------------
; 000  NEW  01/05/99   Patrice VILCHEZ    Creation
; 001  ADD  13/04/01   Patrice VILCHEZ    Clean up
; 002  MOD  01/04/01   Olivier MAZUYER    Clean up
; 003  MOD  14/01/02   Christophe GARDIN  Clean up
;------------------------------------------------------------------------------

;******************************************************************************
;* AREA Definition
;******************************************************************************
   AREA VECTORS, CODE, READONLY
   CODE32
   

;******************************************************************************
;* Include Directive
;******************************************************************************
   INCLUDE csp_arm.inc


;******************************************************************************
;* Exception Vectors
;******************************************************************************
   LDR   PC,   Reset_Addr
   LDR   PC,   Undefined_Addr
   LDR   PC,   SWI_Addr
   LDR   PC,   Prefetch_Addr
   LDR   PC,   Abort_Addr
   NOP                             
   LDR   PC,   [PC,#-0xF20] 
   LDR   PC,   [PC,#-0xF20]

;Import Directive
   IMPORT  Reset_Handler      
   IMPORT  Undefined_Handler
   IMPORT  SWI_Handler
   IMPORT  Prefetch_Handler
   IMPORT  Abort_Handler
   IMPORT  IRQ_Handler
   IMPORT  FIQ_Handler
 
Reset_Addr
   DCD   Reset_Handler      
Undefined_Addr
   DCD   Undefined_Handler
SWI_Addr
   DCD   SWI_Handler
Prefetch_Addr
   DCD   Prefetch_Handler
Abort_Addr
   DCD   Abort_Handler
IRQ_Addr
   DCD   IRQ_Handler
FIQ_Addr
   DCD   FIQ_Handler

;******************************************************************************
   END  
