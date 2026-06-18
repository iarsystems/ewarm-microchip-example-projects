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
; Processor       : easyCAN4
; File Name       : csp_stack.inc
; Description     : Stack Definition File
; Version         : 2.00
; Compiler Tools  : ADS / ASPEX / METAWARE
;
;       +----- (NEW | MODify | ADD | DELete)                                 
;       |                                                                    
;  No   |   when       who                what               
;-----+---+----------+------------------+--------------------------------------
; 000  NEW  23/06/01   Frederic SAMSON    Creation
; 001  MOD  14/01/02   Christophe GARDIN  Clean up
;------------------------------------------------------------------------------

;******************************************************************************
;* AREA Definition
;******************************************************************************
   CODE32


;******************************************************************************
;* Internal RAM Definition
;******************************************************************************
INT_RAM_BASE   EQU   0x00000000
INT_RAM_SIZE   EQU   0x00004000                       ; 16 KBytes
INT_RAM_LIMIT  EQU   INT_RAM_BASE + INT_RAM_SIZE


;******************************************************************************
;* Stacks Sizes Definition
;******************************************************************************
UNDEF_STACK_SIZE  EQU   3*4      ; UNDEF Stack Size : 3 words saved
ABORT_STACK_SIZE  EQU   3*4      ; ABORT Stack Size : 3 words saved
FIQ_STACK_SIZE    EQU   3*4      ; FIQ Stack Size   : 3 words saved
IRQ_STACK_SIZE    EQU   3*8*4    ; IRQ Stack Size   : 3 words saved per priority level
SVC_STACK_SIZE    EQU   3*4      ; SUPERVISOR Stack Size : 3 words saved


;******************************************************************************
;* Stacks Base Address Definition
;******************************************************************************
UNDEF_STACK EQU   INT_RAM_LIMIT                    ; Undefined instruction stack at top of memory
ABORT_STACK EQU   UNDEF_STACK - UNDEF_STACK_SIZE   ; followed by ABORT Stack 
FIQ_STACK   EQU   ABORT_STACK - ABORT_STACK_SIZE   ; followed by FIQ Stack
IRQ_STACK   EQU   FIQ_STACK   - FIQ_STACK_SIZE     ; followed by IRQ Stack
SVC_STACK   EQU   IRQ_STACK   - IRQ_STACK_SIZE     ; followed by SUPERVISOR Stack
USR_STACK   EQU   SVC_STACK   - SVC_STACK_SIZE     ; followed by USER Stack


;******************************************************************************
   END
