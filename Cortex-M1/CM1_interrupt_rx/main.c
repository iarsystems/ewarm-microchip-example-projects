/*******************************************************************************
 * (c) Copyright 2008 Actel Corporation.  All rights reserved.
 *
 * CoreUARTapb interrupt driven receive example.
 *
 * This simple example demonstrates how to use interrupt driven UART receive.
 * It is meant to be used with the Cortex-M1 baseline design running on a board
 * connected via a serail cable to a host PC running HyperTerminal.
 * HyperTerminal must be configure for 57600 baud, 8 bits, 1 stop bit,no parity,
 * no flow control.
 * This program displays a message on HyperTerminal then echoed back characters
 * typed in HyperTerminal.
 *
 * SVN $Revision: 5 $
 * SVN $Date: 2013-12-23 13:52:47 +0100 (mån, 23 dec 2013) $
 */
#include "hal.h"
#include "platform.h"
#include "cortex_nvic.h"
#include "core_uart_apb.h"
#include "core_irq_ctrl.h"

/******************************************************************************
 * Baud value to achieve a 57600 baud rate with a 24MHz system clock.
 * This value is calculated using the following equation:
 *      BAUD_VALUE = CLOCK / ( (16 * BAUD_RATE) - 1 )
 *****************************************************************************/ 
#define BAUD_VALUE_57600    26

/******************************************************************************
 * Maximum receiver buffer size.
 *****************************************************************************/ 
#define MAX_RX_DATA_SIZE    256

/******************************************************************************
 * CoreUARTapb instance data.
 *****************************************************************************/ 
UART_instance_t g_uart;

/******************************************************************************
 * Instruction message. This message will be transmited over the UART to
 * HyperTerminal when the program starts.
 *****************************************************************************/ 
uint8_t g_message[] = 
"\n\r\n\r\n\rCoreUARTapb interrupt driven receiver example. All characters typed \
will be echoed back.\n\r";

/******************************************************************************
 * Cortex-M1 interrupt handler for external interrupt 0.
 * This function is called when the Cortex-M1 IRQ0 signal is asserted.
 *****************************************************************************/ 
void cortex_irq_0_isr( void )
{
	/**********************************************************************
	 * Call the CoreInterrupt driver to determine the source of the
	 * interrupt and call the relevant interrupt service routine.
	 *********************************************************************/		
    CIC_irq_handler();
	
	/**********************************************************************
	 * Clear the interrupt in the Cortex-M1 NVIC.
	 *********************************************************************/		
    NVIC_clear_interrupt( NVIC_IRQ_0 );
}

/******************************************************************************
 * UART receiver interrupt service routine.
 * This function is called by the CoreInterrupt driver when the CoreInterrupt
 * irqSource2 signal is asserted.
 *****************************************************************************/ 
void uart_rx_isr( void )
{
    uint8_t rx_data[MAX_RX_DATA_SIZE];
    size_t rx_size;
    
	/**********************************************************************
	 * Read data received by the UART.
	 *********************************************************************/		
    rx_size = UART_get_rx( &g_uart,	rx_data, sizeof(rx_data) );
    
	/**********************************************************************
	 * Echo back data received, if any.
	 *********************************************************************/		
    if ( rx_size > 0 )
    {
        UART_send( &g_uart,	rx_data, rx_size );   
    }
}

/******************************************************************************
 * main function.
 *****************************************************************************/ 
int main( void )
{
	/**************************************************************************
	 * Configure Interrupts
	 * 	 1.  Initialize CoreInterrupt Controller with its base address.
     *   2.  Initialize Cortex-M1 NVIC.
	 *   3.  Configure CoreInterrupt Controller with the address of the UART
     *       receiver interrupt service routine.
	 *************************************************************************/	
    CIC_init( COREINTERRUPT_BASE_ADDR );
    NVIC_init();
    CIC_set_irq_handler( UART0_RXRDY_IRQ_NB, uart_rx_isr );
    
	/**************************************************************************
	 * Initialize CoreUARTapb with its base address, baud value, and line
     * configuration.
	 *************************************************************************/	
    UART_init( &g_uart, COREUARTAPB0_BASE_ADDR, BAUD_VALUE_57600, (DATA_8_BITS | NO_PARITY) );

	/**************************************************************************
	 * Enable interrupts at the processor level
	 *************************************************************************/	
    NVIC_enable_interrupt( NVIC_IRQ_0 );
    
	/**************************************************************************
	 * Send the instructions message.
	 *************************************************************************/		
    UART_send( &g_uart,	g_message, sizeof(g_message) );
    
	/**************************************************************************
	 * Enable UART RXRDY interrupt at the CoreInterrupt level.
	 *************************************************************************/		
    CIC_enable_irq( UART0_RXRDY_IRQ_NB );

	/**************************************************************************
	 * Infinite Loop.
     * This loop will be interrupted by calls to uart_rx_isr() when the UART's
     * RXRDY signal is asserted upon characters being received.
	 *************************************************************************/		
    while(1)
    {
        ;
    }
}
