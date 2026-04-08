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
; File Name       : csp_arm.inc
; Description     : ARM Definition File
; Version         : 2.00
; Compiler Tools  : ADS / ASPEX / METAWARE
;
;       +----- (NEW | MODify | ADD | DELete)                                 
;       |                                                                    
;  No   |   when       who                what               
;-----+---+----------+------------------+--------------------------------------
; 000  NEW  01/05/99   Patrice VILCHEZ    Creation
; 001  MOD  01/04/01   Olivier MAZUYER    Clean up
; 002  MOD  22/05/01   Tristan BONHOMME   Final Version
; 003  MOD  14/01/02   Christophe GARDIN  Clean up
;------------------------------------------------------------------------------

;******************************************************************************
;* AREA Definition
;******************************************************************************
   CODE32


;******************************************************************************
;* CPSR : Current Program Status Register
;******************************************************************************

; Mode bits
ARM_MODE_USER     EQU   0x10
ARM_MODE_FIQ      EQU   0x11
ARM_MODE_IRQ      EQU   0x12
ARM_MODE_SVC      EQU   0x13
ARM_MODE_ABORT    EQU   0x17
ARM_MODE_UNDEF    EQU   0x1B
ARM_MODE_SYS      EQU   0x1F

; Control bits
I_BIT             EQU   0x80
F_BIT             EQU   0x40
T_BIT             EQU   0x20


;******************************************************************************
   END
