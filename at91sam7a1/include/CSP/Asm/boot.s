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
; File Name       : boot.s
; Description     : Boot File
; Version         : 2.00
; Compiler Tools  : ADS
;
;       +----- (NEW | MODify | ADD | DELete)                                 
;       |                                                                    
;  No   |   when       who                what               
;-----+---+----------+------------------+--------------------------------------
; 000  NEW  01/05/99   Patrice VILCHEZ    Creation
; 001  MOD  01/04/01   Olivier MAZUYER    Clean up
; 002  MOD  22/04/01   Tristan BONHOMME   Final Version
; 003  ADD  19/06/01   Frederic SAMSON    Add possibility to configure up to 8 CS
;                                         Add MCR_reg label 
; 004  ADD  23/06/01   Frederic SAMSON    Add include of csp_stack.inc
; 005  MOD  01/10/01   Olivier Mazuyer    Replace interrupt vectors and data 
;                                         remmapping during boot by the branch on
;                                         Metaware _initcopy function
; 006  MOD  14/01/02   Christophe GARDIN  Clean up
;------------------------------------------------------------------------------

;******************************************************************************
;* AREA Definition
;******************************************************************************
   AREA  BOOT, CODE, READONLY
   CODE32


;******************************************************************************
;* Include Directive
;******************************************************************************
   INCLUDE csp_arm.inc
   INCLUDE csp_amc.inc
   INCLUDE csp_stack.inc


;******************************************************************************
;* Export/Import Directive
;******************************************************************************
   EXPORT  BOOT_START
   IMPORT  CSP_GICInit
   IMPORT  CSP_CInit
   IMPORT  main
 

;******************************************************************************
;* Define the entry point
;******************************************************************************
   ENTRY

;******************************************************************************
;* CS Configuration
;******************************************************************************
BOOT_START
   ldr   r1, =NB_CS
   ldr   r4, =CS0_VAL            ; Flash register value
   ldr   r3, =AMC_BASE_ADDRESS 
   str   r4, [r3, #AMC_CSR0]     ; set the Flash register (CS0)
   sub   r1, r1, #1
   cmp   r1, #0
   beq   MCR_reg                 ; go to MCR_reg flag if 1 CS to configure

   ldr   r4, =CS1_VAL            ; SRAM register value
   str   r4, [r3, #AMC_CSR1]     ; set the SRAM register (CS1)
   sub   r1, r1, #1
   cmp   r1, #0
   beq   MCR_reg                 ; go to MCR_reg flag if 2 CS to configure

   ldr   r4, =CS2_VAL            ; CS2 register value
   str   r4, [r3, #AMC_CSR2]     ; set the CS2 register
   sub   r1, r1, #1
   cmp   r1, #0
   beq   MCR_reg                 ; go to MCR_reg flag if 3 CS to configure

   ldr   r4, =CS3_VAL            ; CS3 register value
   str   r4, [r3, #AMC_CSR3]     ; set the CS3 register
   sub   r1, r1, #1
   cmp   r1, #0
   beq   MCR_reg                 ; go to MCR_reg flag if 4 CS to configure

   ldr   r4, =CS4_VAL            ; CS4 register value
   str   r4, [r3, #AMC_CSR4]     ; set the CS4 register
   sub   r1, r1, #1
   cmp   r1, #0
   beq   MCR_reg                 ; go to MCR_reg flag if 5 CS to configure

   ldr   r4, =CS5_VAL            ; CS5 register value
   str   r4, [r3, #AMC_CSR5]     ; set the CS5 register
   sub   r1, r1, #1
   cmp   r1, #0
   beq   MCR_reg                 ; go to MCR_reg flag if 6 CS to configure

   ldr   r4, =CS6_VAL            ; CS6 register value
   str   r4, [r3, #AMC_CSR6]     ; set the CS6 register
   sub   r1, r1, #1
   cmp   r1, #0
   beq   MCR_reg                 ; go to MCR_reg flag if 7 CS to configure

   ldr   r4, =CS7_VAL            ; CS7 register value
   str   r4, [r3, #AMC_CSR7]     ; set the CS7 register

MCR_reg
   ldr   r4, =MCR_VAL            ; Memory Controller Register value
   str   r4, [r3, #AMC_MCR]      ; set the MCR register
   
;******************************************************************************
;* Remap base address of Internal RAM and external memory on NCS0
;******************************************************************************
Remap
   ldr   r12, PtJumpAddress      ; load the address where to jump
   ldr   r3, =AMC_BASE_ADDRESS   ; point at the AMC base address
   mov   r4, #1                  ; value to write in MC_RCR
   str   r4, [r3, #AMC_RCR]      ; remap command
; Before the remapping takes place, jump to Flash at its new address
   mov   pc, r12                 ; jump and break the pipeline 
  
; Address where to jump after remapping definition 
PtJumpAddress
   DCD   JumpAddress
  
; Code after remapping
JumpAddress

;******************************************************************************
;* Stack Configuration
;******************************************************************************
; During initialization, IRQ and FIQ must be masked

; Set up SUPERVISOR mode
; define SUPERVISOR mode
   mov   r3, #ARM_MODE_SVC | I_BIT | F_BIT  
   msr   CPSR_c, r3              ; Copy to CPSR           
   ldr   r13, =SVC_STACK         ; Set up SUPERVISOR mode stack      

; Set up UNDEF mode
; define UNDEF mode
   mov   r3, #ARM_MODE_UNDEF | I_BIT | F_BIT 
   msr   CPSR_c, r3              ; Copy to CPSR                  
   ldr   r13, =UNDEF_STACK    
  
; Set up ABORT mode
; define ABORT mode
   mov   r3, #ARM_MODE_ABORT | I_BIT | F_BIT   
   msr   CPSR_c, r3              ; Copy to CPSR           
   ldr   r13, =ABORT_STACK       ; Set up ABORT mode stack    

; Set up FIQ mode
; define FIQ mode
   mov   r3, #ARM_MODE_FIQ | I_BIT | F_BIT 
   msr   CPSR_c, r3              ; Copy to CPSR            
   ldr   r13, =FIQ_STACK         ; Set up FIQ mode stack

; Set up IRQ mode
; define IRQ mode
   mov   r3, #ARM_MODE_IRQ | I_BIT | F_BIT 
   msr   CPSR_c, r3              ; Copy to CPSR           
   ldr   r13, =IRQ_STACK         ; Set up IRQ mode stack      

; Set up SUPERVISOR mode
; define SUPERVISOR mode
   mov   r3, #ARM_MODE_SVC | I_BIT | F_BIT  
   msr   CPSR_c, r3              ; Copy to CPSR           
   ldr   r13, =SVC_STACK         ; Set up SUPERVISOR mode stack      

;******************************************************************************
;* Remap (copy) code and data in RAM
;******************************************************************************
; Remap interrupt vectors to 0x00000000 (Internal RAM) and Relocate [copy]
; data from ROM
   bl     CSP_CInit

;******************************************************************************
;* Branch to C code entry
;******************************************************************************
; GIC Initialisation Before Enabling Interrupts
   stmfd sp!, {r0-r3}            ; Store the registers  
   bl    CSP_GICInit
   ldmfd sp!, {r0-r3}            ; Restore the registers and return

; Switch in User Mode and Enable Interrupts
; define USER mode
   mov   r3, #ARM_MODE_USER    
   msr   CPSR_c, r3              ; Copy to CPSR
   ldr   sp, =USR_STACK          ; Set up user stack
   
; Branch to C code entry
   ldr   r0, =main
   bx    r0    


;******************************************************************************
   END







