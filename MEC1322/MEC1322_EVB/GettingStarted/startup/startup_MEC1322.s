;
; The vector table is normally located at address 0.
; When debugging in RAM, it can be located in RAM, aligned to at least 2^6.
; The name "__vector_table" has special meaning for C-SPY:
; it is where the SP start value is found, and the NVIC vector
; table register (VTOR) is initialized to this address if != 0.
;
; Cortex-M version
;

        MODULE  ?cstartup

        ;; Forward declaration of sections.
        SECTION CSTACK:DATA:NOROOT(3)

        SECTION .intvec:CODE:NOROOT(2)

        EXTERN  __iar_program_start
        PUBLIC  __vector_table

        DATA
__vector_table
        DCD     sfe(CSTACK)
        DCD     Reset_Handler           ; Reset Handler
        DCD     NMI_Handler             ; NMI Handler
        DCD     HardFault_Handler       ; Hard Fault Handler
        DCD     MemManage_Handler       ; MPU Fault Handler
        DCD     BusFault_Handler        ; Bus Fault Handler
        DCD     UsageFault_Handler      ; Usage Fault Handler
        DCD     0                       ; Reserved
        DCD     0                       ; Reserved
        DCD     0                       ; Reserved
        DCD     0                       ; Reserved
        DCD     SVC_Handler             ; SVCall Handler
        DCD     DebugMon_Handler        ; Debug Monitor Handler
        DCD     0                       ; Reserved
        DCD     PendSV_Handler          ; PendSV Handler
        DCD     SysTick_Handler         ; SysTick Handler

         ; External Interrupts
        DCD     I2C_0_IRQHandler        ; 0:
        DCD     I2C_1_IRQHandler        ; 1:
        DCD     I2C_2_IRQHandler        ; 2:
        DCD     I2C_3_IRQHandler        ; 3:
        DCD     DMA0_IRQHandler         ; 4:                      (ch0)
        DCD     DMA1_IRQHandler         ; 5:                      (ch1)
        DCD     DMA2_IRQHandler         ; 6:                      (ch2)
        DCD     DMA3_IRQHandler         ; 7:                      (ch3)
        DCD     DMA4_IRQHandler         ; 8:                      (ch4)
        DCD     DMA5_IRQHandler         ; 9:                      (ch5)
        DCD     DMA6_IRQHandler         ; 10:                     (ch6)
        DCD     DMA7_IRQHandler         ; 11:                     (ch7)
        DCD     LPC_IRQHandler          ; 12:
        DCD     M16550_0_IRQHandler     ; 13:
        DCD     IMAP_IRQHandler         ; 14:
        DCD     ACPIEC0_0_IRQHandler    ; 15:                     (IBF)
        DCD     ACPIEC0_1_IRQHandler    ; 16:                     (OBF)
        DCD     ACPIEC1_0_IRQHandler    ; 17:                     (IBF)
        DCD     ACPIEC1_1_IRQHandler    ; 18:                     (OBF)
        DCD     ACPIPM1_0_IRQHandler    ; 19:                     (CTL)
        DCD     ACPIPM1_1_IRQHandler    ; 20:                     (EN)
        DCD     ACPIPM1_2_IRQHandler    ; 21:                     (STS)
        DCD     MIF8042_0_IRQHandler    ; 22:                     (OBF)
        DCD     MIF8042_1_IRQHandler    ; 23:                     (IBF)
        DCD     MAILBOX_IRQHandler      ; 24:
        DCD     PECIHOST_IRQHandler     ; 25:
        DCD     TACH_0_IRQHandler       ; 26:
        DCD     TACH_1_IRQHandler       ; 27:
        DCD     ADC_INT0_IRQHandler     ; 28:                     (SNGL)
        DCD     ADC_INT1_IRQHandler     ; 29:                     (RPT)
        DCD     V2P_INT0_IRQHandler     ; 30:
        DCD     V2P_INT1_IRQHandler     ; 31:
        DCD     PS2_0_IRQHandler        ; 32:
        DCD     PS2_1_IRQHandler        ; 33:
        DCD     PS2_2_IRQHandler        ; 34:
        DCD     PS2_3_IRQHandler        ; 35:
        DCD     SPI0_INT0_IRQHandler    ; 36:                     (TX)
        DCD     SPI0_INT1_IRQHandler    ; 37:                     (RX)
        DCD     HIB_TMR_IRQHandler      ; 38:
        DCD     KEY_INT0_IRQHandler     ; 39:                     (INT)
        DCD     KEY_INT1_IRQHandler     ; 40:                     (WAKE)
        DCD     RPM_INT0_IRQHandler     ; 41:                     (STALL)
        DCD     RPM_INT1_IRQHandler     ; 42:                     (SPIN)
        DCD     VBAT_INT_IRQHandler     ; 43:
        DCD     LED0_IRQHandler         ; 44:
        DCD     LED1_IRQHandler         ; 45:
        DCD     LED2_IRQHandler         ; 46:
        DCD     MBC_INT0_IRQHandler     ; 47:                     (ERR)
        DCD     MBC_INT1_IRQHandler     ; 48:                     (BUSY)
        DCD     TIMER_0_IRQHandler      ; 49:
        DCD     TIMER_1_IRQHandler      ; 50:
        DCD     TIMER_2_IRQHandler      ; 51:
        DCD     TIMER_3_IRQHandler      ; 52:
        DCD     TIMER_4_IRQHandler      ; 53:
        DCD     TIMER_5_IRQHandler      ; 54:
        DCD     SPI1_INT0_IRQHandler    ; 55:                     (TX)
        DCD     SPI1_INT1_IRQHandler    ; 56:                     (RX)
        DCD     IRQ8_IRQHandler         ; 57: Aggregated IRQ08
        DCD     IRQ9_IRQHandler         ; 58: Aggregated IRQ09
        DCD     IRQ10_IRQHandler        ; 59: Aggregated IRQ10
        DCD     IRQ11_IRQHandler        ; 60: Aggregated IRQ11
        DCD     IRQ12_IRQHandler        ; 61: Aggregated IRQ12
        DCD     IRQ13_IRQHandler        ; 62: Aggregated IRQ13
        DCD     IRQ14_IRQHandler        ; 63: Aggregated IRQ14
        DCD     IRQ15_IRQHandler        ; 64: Aggregated IRQ15
        DCD     IRQ16_IRQHandler        ; 65: Aggregated IRQ16
        DCD     IRQ17_IRQHandler        ; 66: Aggregated IRQ17
        DCD     IRQ18_IRQHandler        ; 67: Aggregated IRQ18
        DCD     IRQ19_IRQHandler        ; 68: Aggregated IRQ19
        DCD     IRQ20_IRQHandler        ; 69: Aggregated IRQ20
        DCD     IRQ21_IRQHandler        ; 70: Aggregated IRQ21
        DCD     IRQ22_IRQHandler        ; 71: Aggregated IRQ22
        DCD     IRQ23_IRQHandler        ; 72: Aggregated IRQ23

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Default interrupt handlers.
;;
        THUMB

        PUBWEAK Reset_Handler
        SECTION .text:CODE:NOROOT:REORDER(2)
Reset_Handler

        ; Remap vector table
        LDR     R0, =__vector_table
        LDR     R1, =0xE000ED08          ;NVIC Vector Table Offset Register
        STR     R0,  [R1]

        CPSID   i

        LDR     SP, =sfe(CSTACK)

        LDR     R0, =__iar_program_start
        BX      R0

        PUBWEAK NMI_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
NMI_Handler
        B NMI_Handler

        PUBWEAK HardFault_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
HardFault_Handler
        B HardFault_Handler

        PUBWEAK MemManage_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
MemManage_Handler
        B MemManage_Handler

        PUBWEAK BusFault_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
BusFault_Handler
        B BusFault_Handler

        PUBWEAK UsageFault_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
UsageFault_Handler
        B UsageFault_Handler

        PUBWEAK SVC_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
SVC_Handler
        B SVC_Handler

        PUBWEAK DebugMon_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
DebugMon_Handler
        B DebugMon_Handler

        PUBWEAK PendSV_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
PendSV_Handler
        B PendSV_Handler

        PUBWEAK SysTick_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
SysTick_Handler
        B SysTick_Handler

        PUBWEAK I2C_0_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
I2C_0_IRQHandler
        B I2C_0_IRQHandler

        PUBWEAK I2C_1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
I2C_1_IRQHandler
        B I2C_1_IRQHandler

        PUBWEAK I2C_2_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
I2C_2_IRQHandler
        B I2C_2_IRQHandler

        PUBWEAK I2C_3_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
I2C_3_IRQHandler
        B I2C_3_IRQHandler

        PUBWEAK DMA0_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
DMA0_IRQHandler
        B DMA0_IRQHandler

        PUBWEAK DMA1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
DMA1_IRQHandler
        B DMA1_IRQHandler

        PUBWEAK DMA2_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
DMA2_IRQHandler
        B DMA2_IRQHandler

        PUBWEAK DMA3_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
DMA3_IRQHandler
        B DMA3_IRQHandler

        PUBWEAK DMA4_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
DMA4_IRQHandler
        B DMA4_IRQHandler

        PUBWEAK DMA5_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
DMA5_IRQHandler
        B DMA5_IRQHandler

        PUBWEAK DMA6_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
DMA6_IRQHandler
        B DMA6_IRQHandler

        PUBWEAK DMA7_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
DMA7_IRQHandler
        B DMA7_IRQHandler

        PUBWEAK LPC_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
LPC_IRQHandler
        B LPC_IRQHandler

        PUBWEAK M16550_0_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
M16550_0_IRQHandler
        B M16550_0_IRQHandler

        PUBWEAK IMAP_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
IMAP_IRQHandler
        B IMAP_IRQHandler

        PUBWEAK ACPIEC0_0_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
ACPIEC0_0_IRQHandler
        B ACPIEC0_0_IRQHandler

        PUBWEAK ACPIEC0_1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
ACPIEC0_1_IRQHandler
        B ACPIEC0_1_IRQHandler

        PUBWEAK ACPIEC1_0_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
ACPIEC1_0_IRQHandler
        B ACPIEC1_0_IRQHandler

        PUBWEAK ACPIEC1_1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
ACPIEC1_1_IRQHandler
        B ACPIEC1_1_IRQHandler

        PUBWEAK ACPIPM1_0_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
ACPIPM1_0_IRQHandler
        B ACPIPM1_0_IRQHandler

        PUBWEAK ACPIPM1_1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
ACPIPM1_1_IRQHandler
        B ACPIPM1_1_IRQHandler

        PUBWEAK ACPIPM1_2_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
ACPIPM1_2_IRQHandler
        B ACPIPM1_2_IRQHandler

        PUBWEAK MIF8042_0_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
MIF8042_0_IRQHandler
        B MIF8042_0_IRQHandler

        PUBWEAK MIF8042_1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
MIF8042_1_IRQHandler
        B MIF8042_1_IRQHandler

        PUBWEAK MAILBOX_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
MAILBOX_IRQHandler
        B MAILBOX_IRQHandler

        PUBWEAK PECIHOST_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
PECIHOST_IRQHandler
        B PECIHOST_IRQHandler

        PUBWEAK TACH_0_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TACH_0_IRQHandler
        B TACH_0_IRQHandler

        PUBWEAK TACH_1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TACH_1_IRQHandler
        B TACH_1_IRQHandler

        PUBWEAK ADC_INT0_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
ADC_INT0_IRQHandler
        B ADC_INT0_IRQHandler

        PUBWEAK ADC_INT1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
ADC_INT1_IRQHandler
        B ADC_INT1_IRQHandler

        PUBWEAK V2P_INT0_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
V2P_INT0_IRQHandler
        B V2P_INT0_IRQHandler

        PUBWEAK V2P_INT1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
V2P_INT1_IRQHandler
        B V2P_INT1_IRQHandler

        PUBWEAK PS2_0_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
PS2_0_IRQHandler
        B PS2_0_IRQHandler

        PUBWEAK PS2_1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
PS2_1_IRQHandler
        B PS2_1_IRQHandler

        PUBWEAK PS2_2_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
PS2_2_IRQHandler
        B PS2_2_IRQHandler

        PUBWEAK PS2_3_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
PS2_3_IRQHandler
        B PS2_3_IRQHandler

        PUBWEAK SPI0_INT0_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
SPI0_INT0_IRQHandler
        B SPI0_INT0_IRQHandler

        PUBWEAK SPI0_INT1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
SPI0_INT1_IRQHandler
        B SPI0_INT1_IRQHandler

        PUBWEAK HIB_TMR_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
HIB_TMR_IRQHandler
        B HIB_TMR_IRQHandler

        PUBWEAK KEY_INT0_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
KEY_INT0_IRQHandler
        B KEY_INT0_IRQHandler

        PUBWEAK KEY_INT1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
KEY_INT1_IRQHandler
        B KEY_INT1_IRQHandler

        PUBWEAK RPM_INT0_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
RPM_INT0_IRQHandler
        B RPM_INT0_IRQHandler

        PUBWEAK RPM_INT1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
RPM_INT1_IRQHandler
        B RPM_INT1_IRQHandler

        PUBWEAK VBAT_INT_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
VBAT_INT_IRQHandler
        B VBAT_INT_IRQHandler

        PUBWEAK LED0_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
LED0_IRQHandler
        B LED0_IRQHandler

        PUBWEAK LED1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
LED1_IRQHandler
        B LED1_IRQHandler

        PUBWEAK LED2_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
LED2_IRQHandler
        B LED2_IRQHandler

        PUBWEAK MBC_INT0_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
MBC_INT0_IRQHandler
        B MBC_INT0_IRQHandler

        PUBWEAK MBC_INT1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
MBC_INT1_IRQHandler
        B MBC_INT1_IRQHandler

        PUBWEAK TIMER_0_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TIMER_0_IRQHandler
        B TIMER_0_IRQHandler

        PUBWEAK TIMER_1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TIMER_1_IRQHandler
        B TIMER_1_IRQHandler

        PUBWEAK TIMER_2_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TIMER_2_IRQHandler
        B TIMER_2_IRQHandler

        PUBWEAK TIMER_3_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TIMER_3_IRQHandler
        B TIMER_3_IRQHandler

        PUBWEAK TIMER_4_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TIMER_4_IRQHandler
        B TIMER_4_IRQHandler

        PUBWEAK TIMER_5_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TIMER_5_IRQHandler
        B TIMER_5_IRQHandler

        PUBWEAK SPI1_INT0_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
SPI1_INT0_IRQHandler
        B SPI1_INT0_IRQHandler

        PUBWEAK SPI1_INT1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
SPI1_INT1_IRQHandler
        B SPI1_INT1_IRQHandler

        PUBWEAK IRQ8_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
IRQ8_IRQHandler
        B IRQ8_IRQHandler

        PUBWEAK IRQ9_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
IRQ9_IRQHandler
        B IRQ9_IRQHandler

        PUBWEAK IRQ10_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
IRQ10_IRQHandler
        B IRQ10_IRQHandler

        PUBWEAK IRQ11_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
IRQ11_IRQHandler
        B IRQ11_IRQHandler

        PUBWEAK IRQ12_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
IRQ12_IRQHandler
        B IRQ12_IRQHandler

        PUBWEAK IRQ13_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
IRQ13_IRQHandler
        B IRQ13_IRQHandler

        PUBWEAK IRQ14_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
IRQ14_IRQHandler
        B IRQ14_IRQHandler

        PUBWEAK IRQ15_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
IRQ15_IRQHandler
        B IRQ15_IRQHandler

        PUBWEAK IRQ16_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
IRQ16_IRQHandler
        B IRQ16_IRQHandler

        PUBWEAK IRQ17_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
IRQ17_IRQHandler
        B IRQ17_IRQHandler

        PUBWEAK IRQ18_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
IRQ18_IRQHandler
        B IRQ18_IRQHandler

        PUBWEAK IRQ19_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
IRQ19_IRQHandler
        B IRQ19_IRQHandler

        PUBWEAK IRQ20_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
IRQ20_IRQHandler
        B IRQ20_IRQHandler

        PUBWEAK IRQ21_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
IRQ21_IRQHandler
        B IRQ21_IRQHandler

        PUBWEAK IRQ22_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
IRQ22_IRQHandler
        B IRQ22_IRQHandler

        PUBWEAK IRQ23_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
IRQ23_IRQHandler
        B IRQ23_IRQHandler

        END
