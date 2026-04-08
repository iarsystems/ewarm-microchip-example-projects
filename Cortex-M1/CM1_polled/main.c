/*******************************************************************************
 * (c) Copyright 2008 Actel Corporation.  All rights reserved.
 *
 * CoreUARTapb polled transmit and  receive example.
 *
 * This simple example demonstrates how to use the CoreUARTapb driver to
 * transmit and receive data in polled mode.
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
#include "core_uart_apb.h"

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
"\n\r\n\r\n\rCoreUARTapb polled transmitter and receiver example. \n\rAll characters typed \
will be echoed back.\n\r";

/******************************************************************************
 * Cortex-M1 interrupt handler for external interrupt 0.
 * This function is called when the Cortex-M1 IRQ0 signal is asserted.
 *****************************************************************************/ 
/*
void
isr_0( void )
{
}
*/
/******************************************************************************
 * main function.
 *****************************************************************************/ 
int main( void )
{
    uint8_t rx_data[MAX_RX_DATA_SIZE];
    size_t rx_size;
    
	/**************************************************************************
	 * Initialize CoreUARTapb with its base address, baud value, and line
     * configuration.
	 *************************************************************************/	
    UART_init( &g_uart, COREUARTAPB0_BASE_ADDR, BAUD_VALUE_57600, (DATA_8_BITS | NO_PARITY) );

	/**************************************************************************
	 * Send the instructions message.
	 *************************************************************************/		
    UART_send( &g_uart,	g_message, sizeof(g_message) );
    
	/**************************************************************************
	 * Infinite Loop.
	 *************************************************************************/		
    while(1)
    {
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
}
