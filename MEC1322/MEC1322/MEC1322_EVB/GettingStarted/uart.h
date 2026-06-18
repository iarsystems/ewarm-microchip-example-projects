/*****************************************************************************
* © 2014 Microchip Technology Inc. and its subsidiaries.
* You may use this software and any derivatives exclusively with
* Microchip products.
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".
* NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
* INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
* AND FITNESS FOR A PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP
* PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.
* TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL
* CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF
* FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
* MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE
* OF THESE TERMS.
*****************************************************************************/
/*  FILE:     $File: uart.h $
 *  REVISION: $Revision: #1 $
 *  DATETIME: $DateTime: 2015/01/23 16:42:36 $
 *  AUTHOR:   CPG Applications Team
 *_____________________________________________________________________________
 *****************************************************************************/
typedef volatile unsigned char      VUINT8;
typedef volatile unsigned long int  VUINT32;


#define WORD_LENGTH_8    0x03
#define ONE_STOP_BIT     0x00
#define MCR_OUT2         0x08

#define	UART_MAX         64

#define FALSE            0
#define TRUE             1

#define TX_BYTE_SIZE     80

#define ADDR_GPIO165_PIN_CONTROL                                 0x400811D4
#define MMCR_GPIO165_PIN_CONTROL                                 (*(VUINT32 *)(ADDR_GPIO165_PIN_CONTROL))
#define ADDR_CLOCK_ENABLE                                        0x4000A408
#define MMCR_CLOCK_ENABLE                                        (*(VUINT32 *)(ADDR_CLOCK_ENABLE))

/***************************************************************
*                            UART
***************************************************************/
#define ADDR_M16C550A_UART_ACTIVATE                              0x400F1F30
#define MMCR_M16C550A_UART_ACTIVATE                              (*(VUINT8 *)(ADDR_M16C550A_UART_ACTIVATE))

#define ADDR_M16C550A_UART_CONFIG_SELECT                         0x400F1FF0
#define MMCR_M16C550A_UART_CONFIG_SELECT                         (*(VUINT8 *)(ADDR_M16C550A_UART_CONFIG_SELECT))

#define ADDR_M16C550A_UART_PROGRAMMABLE_BAUD_RATE_GENERATOR_LSB  0x400F1C00
#define MMCR_M16C550A_UART_PROGRAMMABLE_BAUD_RATE_GENERATOR_LSB  (*(VUINT8 *)(ADDR_M16C550A_UART_PROGRAMMABLE_BAUD_RATE_GENERATOR_LSB))

#define ADDR_M16C550A_UART_RECEIVE_BUFFER                        0x400F1C00
#define MMCR_M16C550A_UART_RECEIVE_BUFFER                        (*(VUINT8 *)(ADDR_M16C550A_UART_RECEIVE_BUFFER))

#define ADDR_M16C550A_UART_TRANSMIT_BUFFER                       0x400F1C00
#define MMCR_M16C550A_UART_TRANSMIT_BUFFER                       (*(VUINT8 *)(ADDR_M16C550A_UART_TRANSMIT_BUFFER))

#define ADDR_M16C550A_UART_PROGRAMMABLE_BAUD_RATE_GENERATOR_MSB  0x400F1C01
#define MMCR_M16C550A_UART_PROGRAMMABLE_BAUD_RATE_GENERATOR_MSB  (*(VUINT8 *)(ADDR_M16C550A_UART_PROGRAMMABLE_BAUD_RATE_GENERATOR_MSB))

#define ADDR_M16C550A_UART_INTERRUPT_ENABLE                      0x400F1C01
#define MMCR_M16C550A_UART_INTERRUPT_ENABLE                      (*(VUINT8 *)(ADDR_M16C550A_UART_INTERRUPT_ENABLE))

#define ADDR_M16C550A_UART_FIFO_CONTROL                          0x400F1C02
#define MMCR_M16C550A_UART_FIFO_CONTROL                          (*(VUINT8 *)(ADDR_M16C550A_UART_FIFO_CONTROL))

#define ADDR_M16C550A_UART_INTERRUPT_IDENTIFICATION              0x400F1C02
#define MMCR_M16C550A_UART_INTERRUPT_IDENTIFICATION              (*(VUINT8 *)(ADDR_M16C550A_UART_INTERRUPT_IDENTIFICATION))

#define ADDR_M16C550A_UART_LINE_CONTROL                          0x400F1C03
#define MMCR_M16C550A_UART_LINE_CONTROL                          (*(VUINT8 *)(ADDR_M16C550A_UART_LINE_CONTROL))

#define ADDR_M16C550A_UART_MODEM_CONTROL                         0x400F1C04
#define MMCR_M16C550A_UART_MODEM_CONTROL                         (*(VUINT8 *)(ADDR_M16C550A_UART_MODEM_CONTROL))

#define ADDR_M16C550A_UART_LINE_STATUS                           0x400F1C05
#define MMCR_M16C550A_UART_LINE_STATUS                           (*(VUINT8 *)(ADDR_M16C550A_UART_LINE_STATUS))

#define ADDR_M16C550A_UART_MODEM_STATUS                          0x400F1C06
#define MMCR_M16C550A_UART_MODEM_STATUS                          (*(VUINT8 *)(ADDR_M16C550A_UART_MODEM_STATUS))

#define ADDR_M16C550A_UART_SCRATCHPAD                            0x400F1C07
#define MMCR_M16C550A_UART_SCRATCHPAD                            (*(VUINT8 *)(ADDR_M16C550A_UART_SCRATCHPAD))



#define CONFIG_SEL_REGISTER(VALUE)          MMCR_M16C550A_UART_CONFIG_SELECT = VALUE
#define ACTIVATE_UART()                     MMCR_M16C550A_UART_ACTIVATE = 0x01
#define DEACTIVATE_UART()                   MMCR_M16C550A_UART_ACTIVATE = 0
#define CONFIG_SELECT_REGISTER(VALUE)       MMCR_M16C550A_UART_CONFIG_SELECT = VALUE
#define CONFIG_IER(VALUE)                   MMCR_M16C550A_UART_INTERRUPT_ENABLE = VALUE
#define CONFIG_LCR(VALUE)                   MMCR_M16C550A_UART_LINE_CONTROL = VALUE
#define CONFIG_MCR(VALUE)                   MMCR_M16C550A_UART_MODEM_CONTROL = VALUE
#define SET_DLAB()                          MMCR_M16C550A_UART_LINE_CONTROL |= (1<<7)
#define RESET_DLAB()                        MMCR_M16C550A_UART_LINE_CONTROL &= ~(1<<7)
#define WRITE_TB(VALUE)                     MMCR_M16C550A_UART_TRANSMIT_BUFFER = VALUE
#define READ_RB(VALUE)                      VALUE = MMCR_M16C550A_UART_RECEIVE_BUFFER
#define SET_BAUD_RATE(VALUE)                MMCR_M16C550A_UART_PROGRAMMABLE_BAUD_RATE_GENERATOR_LSB = VALUE
#define GET_UART_INT_NUM()                  MMCR_M16C550A_UART_INTERRUPT_IDENTIFICATION & 0x0F
#define GET_LSR()                           MMCR_M16C550A_UART_LINE_STATUS



typedef enum
{
    RATE_134 = 857,
    RATE_1200 = 96,
    RATE_3600 = 32,
    RATE_9600 = 12,
    RATE_19200 = 6,
    RATE_115200 = 1
}BAUD_RATE;


/* public function prototypes */
void uart_init(void);
void uart_printf(char *mychar);
void uart_config_lcr(unsigned char lcr);
void uart_config_mcr(unsigned char mcr);
void uart_config_cs(unsigned char cs);
void uart_set_baudrate(BAUD_RATE baud_rate);
void uart_output(void);
