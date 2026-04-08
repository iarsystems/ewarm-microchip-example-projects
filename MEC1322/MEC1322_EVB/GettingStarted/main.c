/*****************************************************************************
* � 2014 Microchip Technology Inc. and its subsidiaries.
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
/** @file  main.c
 ******************************************************************************
 *  main.c This is the main file for GettingStarted (MEC1322_Blink) Sample Code
 ******************************************************************************
 *  Revision history (latest first):
 *  #1  2014/07/31 c21674  initial revision supporting MEC1322
 *******************************************************************************
 *  This file is the main program for an ARM-based demo that :
 *    - initializes MEC1322 EVB ASSY 6703 REV A
 *    - configures timer interrupts
 *    - configures LEDs on GPIO signals
 *    - blinks LEDs
 *******************************************************************************
*/
#include "mchp.h"
#include "uart.h"
#include <string.h>
#include <intrinsics.h>

//#define TRACE_FIFO_DEBUG_BOARD
#define UART_115200_8_N_1SB

void timer_init(void);
void timer_delay_60ms(void);
void board_init(void);
void system_enable_32k_clock(UINT8);

void banner(void);

volatile UINT8 TIMER0_TIMEOUT = TIMER_60MS;
volatile UINT8 ISR_COUNT = 0;
volatile UINT8 T_DONE = 0;

char uart_tx_buff[TX_BYTE_SIZE];

/** This is the main module for the MEC1322 Blink LED demo
*/
int main(void)
{
UINT32 i;

  board_init();

  timer_delay_60ms();                       //adjust timeout value to system h/w and/or f/w requirements

  REG32( GPIO_PIN_CONTROL(0154) ) = (1<<9) | (1<<10);     // LED0 is on GPIO154, set for output
  REG32( GPIO_PIN_CONTROL(0155) ) = (1<<9) | (1<<10);     // LED1 is on GPIO155, set for output
  REG32( GPIO_OUTPUT3 ) |= LED1;            //GPIO154/LED1 off
  REG32( GPIO_OUTPUT3 ) |= LED2;            //GPIO155/LED2 off

#ifdef TRACE_FIFO_DEBUG_BOARD
  TRACE0(1, INIT, 0, "Let's blink some LEDS ...");
#endif
  uart_printf ("Let's blink LEDs....");

  while(1)
  {
    for (i = 0; i < 16; i++)  //wait a second
      timer_delay_60ms();

    REG32( GPIO_OUTPUT3 ) ^= (1<<LED2);       //GPIO155 toggle

    uart_printf( "LED2 toggle");
#ifdef TRACE_FIFO_DEBUG_BOARD
    TRACE0(2, INIT, 0, "LED2 toggle");
#endif

    for (i = 0; i < 16; i++)  //wait a second
      timer_delay_60ms();

    REG32( GPIO_OUTPUT3 ) ^= (1<<LED1);       //GPIO154 toggle
    uart_printf( "LED1 toggle");
#ifdef TRACE_FIFO_DEBUG_BOARD
    TRACE0(3, INIT, 0, "LED1 toggle");
#endif
  }
}


/********************************************************************
* Utilities                                                         *
*********************************************************************/

/**  IRQ23_IRQHandler
* @note  timer0 interrupt handler fires 250탎 intervals
*/
void IRQ23_IRQHandler(void)
{
  if (REG32(IRQ23_ENABLE) & 0x01)
  {   //fire every 250 탎
    REG32(IRQ23_SOURCE) |= 0x01;                //write clear this instance

    ISR_COUNT++;
    if (ISR_COUNT >= TIMER0_TIMEOUT)            //waiting for 1 or 60 msec
    {
      ISR_COUNT = 0;
      T_DONE = 1;
    }
  }
}

/**  timer_delay_60ms
* @note  clear timer irq flags and wait 60 msec before returning
*/
void timer_delay_60ms()
{

  TIMER0_TIMEOUT = TIMER_60MS;                  //250 탎ec * 240 =  60 ms

  T_DONE = 0;
  ISR_COUNT = 0;

  while (!T_DONE);                              //wait here for the timer to finish

}


/**  Initializes timer 0 for basic functionality necessary for this demo
* @note  Initializes timer 0 to interrupt at 250 탎ec intervals, enables interrupts
*/
void timer_init()
{
  unsigned short i;

  REG32(EC_INT_CNTL) &= ~1;                   //clear the EC only interrupt control bit

  REG32(IRQ23_SOURCE) = 1;                    //clear timer 0 irq
  REG32(IRQ23_ENABLE_CLEAR) = 1;
  REG32(IRQ23_ENABLE) = 1;                    //enable timer 0 irq
  REG32(IRQ23_BLOCK_EN) = (1<<23);            //enable irq23

  REG32(NVIC_ENABLE2) |= (1<<8);              //set bit 8 for NVIC irq 72 (72 - 64 = 8)
  __enable_interrupt();

  REG32(TIMER_CONTROL) = 0x10;                //soft reset timer

  for(i = 0; i < 100; i++)                    //soft delay
  {
    __no_operation();
  }

  REG32(TIMER_PRELOAD) = 250;                 //250 탎
  REG32(TIMER_INTEN) = 1;                     //enable interrupt
  REG32(TIMER_CONTROL) = (0x29 | TIMER_PRE_SCL_DIV_48);     //start auto count-down timer w/48MHz Pre-scale

}


/**  Initializes for basic functionality necessary for this demo
* @note  Initializes Trace Debugger, inits timer0 int
*/
void board_init(void)
{
#ifdef FOR_MECC_MEC1322_ONLY
  REG32( GPIO_PIN_CONTROL(0143) ) = 0;                    // GPIO143 signal is defaulting to PP, output holding PCH in reset. Set to input
#endif

//  system_enable_32k_clock(XOSEL_SINGLE | BIT_32K_EN);   //OSC single-ended, 32K enabled
  system_enable_32k_clock(XOSEL_DUAL | BIT_32K_EN);       //Crystal oscillator double-ended

  while( (REG32(MMCR_OSC_LOCK_REG) & 0x100) == 0);

 #ifdef TRACE_FIFO_DEBUG_BOARD
/*-------------------------------------------------------------------------
   * Enable Trace FIFO Debug Port(TFDP)
   * 1. Config GPIO164 = TF_RST#,  Mux0=GPIO,   Out=Lo, Push-Pull, Dis.INT
   * 2. Config GPIO103 = TFDP_DTA, Mux1=TFDP_DTA, Out=Hi, Push-Pull, Dis.INT
   * 3. Config GPIO104 = TFDP_CLK, Mux1=TFDP_CLK, Out=Hi, Push-Pull, Dis.INT
   * 4. Set GPIO164(TF_RST#) High to complete Pegasus board reset.
   * 5. Enable Trace FIFO Debug Port
   * Note: Also see the GPIO Interface of MEC1322/SSC710x spec for more details.
   *-----------------------------------------------------------------------*/

  REG32( GPIO_PIN_CONTROL(0164) ) = 0x000240UL;         // TF_RST#, low
  REG32( GPIO_PIN_CONTROL(0103) ) = 0x011240UL;         // TFDP_DTA
  REG32( GPIO_PIN_CONTROL(0104) ) = 0x011240UL;         // TFDP_CLK
  REG32( GPIO_PIN_CONTROL(0164) ) = 0x010240UL;         // TF_RST#, high
  REG8(  TFDP_CONTROL ) = 0x01;                         // Enable TFDP func.

  /*-------------------------------------------------------------------------
   * Print the information of application to Pegasus (TraceFIFO).
   *-----------------------------------------------------------------------*/

  TRACE0(4, INIT, 0, ".................................." );
  TRACE0(5, INIT, 0, "MEC1322 Firmware Sample (ARM-based) v1.0");
  TRACE0(6, INIT, 0, "device_initialization" );
#endif

#ifdef UART_115200_8_N_1SB
  uart_init();                          //output to HyperTerm thru UART

  banner();
#endif

  timer_init();                         //Initialize Timer
}


/*-------------------------------------------------------------------------
 * Print the information of application.
 *-----------------------------------------------------------------------*/
void banner()
{
  uart_printf( "_______________________________________________________");
  uart_printf( "|                                                      |");
  uart_printf( "|    M  E  C  1  3  2  2  Blink LED Demo v1.0          |");
  uart_printf( "|______________________________________________________|");
  uart_printf( "|                                                      |");
  uart_printf( "|    PLATFORM:   MEC1322    DEMO BOARD ASSY6703 REV A  |");
  uart_printf( "|______________________________________________________|");
}


void uart_init(void)
{
unsigned char lcr_val = (WORD_LENGTH_8 | ONE_STOP_BIT );
unsigned char mcr_val = MCR_OUT2;
unsigned char cs_val =  0;

  /* UART_TX */
  MMCR_GPIO165_PIN_CONTROL = GPIO_MUX_FUNC1;

  uart_config_cs(cs_val);
  uart_set_baudrate(RATE_115200);
  uart_config_lcr(lcr_val);

  uart_config_mcr(mcr_val);

  ACTIVATE_UART();

/* End uart_init() */
}


void uart_printf(char *mychar)  //output <CR, LF>
{
unsigned char ctr;

  ctr = strlen(mychar);
  strcpy(uart_tx_buff,mychar);
  uart_tx_buff[ctr] = '\n';
  uart_tx_buff[ctr+1] = '\r';
  uart_tx_buff[ctr+2] = '\0';
  uart_output();
}


void uart_output()
{
unsigned char cnt, i;

  //without interrupts
  MMCR_M16C550A_UART_LINE_CONTROL &= ~0x80;
  cnt = strlen(uart_tx_buff);
  for (i=0; i < cnt; )
  {
    while ( (MMCR_M16C550A_UART_LINE_STATUS & 0x20) == 0 );    //wait for Tx empty

    MMCR_M16C550A_UART_TRANSMIT_BUFFER = uart_tx_buff[i++];    //output a character
  }
}


/******************************************************************************/
/** uart_config_mcr
* This will configure Config Select Register.
* @param UINT8 cs
* @return void
*******************************************************************************/
void uart_config_cs(unsigned char cs)
{
  CONFIG_SEL_REGISTER(cs);
} /* End uart_config_cs() */


/******************************************************************************/
/** uart_set_baudrate
* This will set the desired baud rate.
* @param BAUD_RATE baud_rate
* @return void
*******************************************************************************/
void uart_set_baudrate(BAUD_RATE baud_rate)
{
  /* Set DLAB bit to access BRG */
  SET_DLAB();
  SET_BAUD_RATE(baud_rate);
} /* End uart_set_baudrate() */


/******************************************************************************/
/** uart_config_lcr
* This will configure Line Control Register.
* @param UINT8 lcr
* @return void
*******************************************************************************/
void uart_config_lcr(unsigned char lcr)
{
  RESET_DLAB();
  CONFIG_LCR(lcr);
} /* End uart_config_lcr() */


/******************************************************************************/
/** uart_config_mcr
* This will configure Modem Control Register.
* @param UINT8 mcr
* @return void
*******************************************************************************/
void uart_config_mcr(unsigned char mcr)
{
  CONFIG_MCR(mcr);
} /* End uart_config_mcr() */


/******************************************************************************/
/** system_enable_32k_clock
* Enable 32KHz crystal/OSC clock
* @param void
* @return void
*******************************************************************************/
void system_enable_32k_clock(UINT8 val)
{
UINT16 i, x= 0;

  REG8(MMCR_VBAT_CLOCK_ENABLE) = (val & 3);

  /* delay a little while until 32KHz stable, for crystal only */
  for(i = 0; i < 100; i++)
  {
     x++;
  }
} /* End system_enable_32k_clock() */
