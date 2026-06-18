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
; Processor       : SmartCAN1
; File Name       : csp_amc.inc
; Description     : Advanced Memory Controller Definition File
; Version         : 2.00
; Compiler Tools  : ADS / ASPEX / METAWARE
;
;       +----- (NEW | MODify | ADD | DELete)                                 
;       |                                                                    
;  No   |   when       who                what               
;-----+---+----------+------------------+--------------------------------------
; 000  NEW  01/05/99   Patrice VILCHEZ    Creation
; 001  MOD  01/04/01   Olivier MAZUYER    Clean up
; 002  MOD  22/04/01   Tristan BONHOMME   Final Version
; 003  ADD  19/06/01   Frederic SAMSON    Add the number of Chip Select : NB_CS
; 004  MOD  14/01/02   Christophe GARDIN  Clean up
;------------------------------------------------------------------------------

;******************************************************************************
;* AREA Definition
;******************************************************************************
   CODE32


;******************************************************************************
;****************************** AMC Definition ********************************
;******************************************************************************

;******************************************************************************
;* Number of Chips Select
;******************************************************************************
NB_CS    EQU   0x2         ; 2 Chips Select


;******************************************************************************
;* Chip Select Definition
;******************************************************************************
CS0_VAL  EQU   0x400030A9  ; FLASH : Base Address = 0x40000000
                           ; CS Enable, Byte Select access, 0 tdf, 4MB, 3 waitstates, 16 bits
CS1_VAL  EQU   0x48003081  ; RAM  : Base Address = 0x48000000 
                           ; CS Enable, Byte Select access, 0 tdf, 4MB, 0 waitstates, 16 bits
CS2_VAL  EQU   0x50000000  ; Base Address = 0x50000000
                           ; CS Disable
CS3_VAL  EQU   0x58000000  ; Base Address = 0x58000000
                           ; CS Disable
CS4_VAL  EQU   0x60000000  ; Base Address = 0x60000000
                           ; CS Disable
CS5_VAL  EQU   0x68000000  ; Base Address = 0x68000000
                           ; CS Disable
CS6_VAL  EQU   0x70000000  ; Base Address = 0x70000000 
                           ; CS Disable
CS7_VAL  EQU   0x78000000  ; Base Address = 0x78000000 
                           ; CS Disable
MCR_VAL  EQU   0x00000017  ; Memory Control Register Value
                           ; Early Read Protocol, All chip selects valid

;******************************************************************************
;* AMC Structure Definition
;******************************************************************************
AMC_BASE_ADDRESS  EQU   0xFFE00000     ; AMC Base Address

AMC_CSR0          EQU   0x00           ; Chip Select Register 0 offset 
AMC_CSR1          EQU   0x04           ; Chip Select Register 1 offset 
AMC_CSR2          EQU   0x08           ; Chip Select Register 2 offset 
AMC_CSR3          EQU   0x0C           ; Chip Select Register 3 offset 
AMC_CSR4          EQU   0x10           ; Chip Select Register 4 offset 
AMC_CSR5          EQU   0x14           ; Chip Select Register 5 offset 
AMC_CSR6          EQU   0x18           ; Chip Select Register 6 offset 
AMC_CSR7          EQU   0x1C           ; Chip Select Register 7 offset 

AMC_RCR           EQU   0x20           ; Remap Control Register offset 
AMC_MCR           EQU   0x24           ; Memory Control Register offset 


;******************************************************************************
   END

