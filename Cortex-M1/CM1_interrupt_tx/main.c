/*******************************************************************************
 * (c) Copyright 2007-2008 Actel Corporation.  All rights reserved.
 *
 * CoreUARTapb interrupt driven transmit example.
 *
 * This example demonstrates how to use the CoreUARTapb driver to perform
 * interrupt driven transmit.
 * It is meant to be used with the Cortex-M1 baseline design running on a board
 * connected via a serail cable to a host PC running HyperTerminal.
 * HyperTerminal must be configure for 57600 baud, 8 bits, 1 stop bit,no parity,
 * no flow control.
 * This program will continuously display a message on HyperTerminal.
 *
 * SVN $Revision: 5 $
 * SVN $Date: 2013-12-23 13:52:47 +0100 (mån, 23 dec 2013) $
 */
#include "hal.h"
#include "platform.h"
#include "cortex_nvic.h"
#include "core_uart_apb.h"
#include "core_irq_ctrl.h"

#define TX_IN_PROGRESS	0
#define TX_COMPLETE		1

/******************************************************************************
 * Baud value to achieve a 57600 baud rate with a 24MHz system clock.
 * This value is calculated using the following equation:
 *      BAUD_VALUE = (CLOCK / (16 * BAUD_RATE)) - 1 
 *****************************************************************************/ 
#define BAUD_VALUE_57600    26

/******************************************************************************
 * CoreUARTapb instance data.
 *****************************************************************************/ 
UART_instance_t g_uart;

/******************************************************************************
 * Global variable used to hold a pointer to the next block of data to transmit.
 *****************************************************************************/ 
uint8_t * g_tx_buffer = 0;

/******************************************************************************
 * Global variable used to hold the number of characters left to be sent in the
 * data buffer pointed to by g_tx_buffer.
 *****************************************************************************/ 
volatile size_t g_tx_size = 0;

/******************************************************************************
 * This message will be transmited over the UART to HyperTerminal.
 *****************************************************************************/ 
uint8_t g_message[] = 
"\n\r\n\r\n\rHello world using interrupt generated on assertion of the UART TXRDY signal. \
This message has to be longer than 256 characters in order to ensure that at \
least some part of this text gets loaded into the UART FIFO as a result of an \
interrupt. Please remember that the CoreUARTapb transmitter FIFO is 256 bytes long \
when enabled. Have a nice day and please remember: Power Matters!";

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
 * UART transmitter interrupt service routine.
 * This function is called by the CoreInterrupt driver when the CoreInterrupt
 * irqSource1 signal is asserted.
 *****************************************************************************/ 
void uart_tx_isr( void )
{
    size_t size_in_fifo;
    
    if ( g_tx_size > 0 )
    {
        size_in_fifo = UART_fill_tx_fifo( &g_uart, g_tx_buffer, g_tx_size );
        if ( size_in_fifo < g_tx_size )
        {
            g_tx_buffer = &g_tx_buffer[size_in_fifo];
            g_tx_size = g_tx_size - size_in_fifo;
        }
        else
        {
            g_tx_buffer = 0;
            g_tx_size = 0;
            CIC_disable_irq( UART0_TXRDY_IRQ_NB );
        }
    }
    else
    {
        CIC_disable_irq( UART0_TXRDY_IRQ_NB );
    }
}

/******************************************************************************
 * is_tx_complete() is used to check if interrupt driven transmition is complete.
 *****************************************************************************/ 
int is_tx_complete( void )
{
    int complete = TX_COMPLETE;
    
    if ( g_tx_size > 0 )
    {
        complete = TX_IN_PROGRESS;
    }
    
    return complete;
}

/******************************************************************************
 * send_using_interrupt() is used to initiate interrupt-driven transmition.
 * It fills the UART FIFO and enable the TXRDY interrupt if there is data
 * remaining to be sent.
 *****************************************************************************/ 
void send_using_interrupt
(
    uint8_t * pbuff,
    size_t tx_size
)
{
    size_t size_in_fifo;
    
    size_in_fifo = UART_fill_tx_fifo( &g_uart, pbuff, tx_size );
    if ( size_in_fifo < tx_size )
    {
        g_tx_buffer = &pbuff[size_in_fifo];
        g_tx_size = tx_size - size_in_fifo;
        CIC_enable_irq( UART0_TXRDY_IRQ_NB );
    }
    else
    {
        g_tx_buffer = 0;
        g_tx_size = 0;
        CIC_disable_irq( UART0_TXRDY_IRQ_NB );
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
     *       transmitter interrupt service routine.
	 *************************************************************************/	
    CIC_init( COREINTERRUPT_BASE_ADDR );
    NVIC_init();
    CIC_set_irq_handler( UART0_TXRDY_IRQ_NB, uart_tx_isr );
    
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
	 * Infinite Loop.
	 *************************************************************************/		
    while(1)
    {
        /**********************************************************************
	     * Initiate message transmit.
	     *********************************************************************/		
        send_using_interrupt( g_message, sizeof(g_message) );
        
        /**********************************************************************
	     * Wait for full message to be sent by the interrupt service routine.
	     *********************************************************************/		
        while( is_tx_complete() == TX_IN_PROGRESS )
        {
            ;
        }
    }
}
