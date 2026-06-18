/*******************************************************************************
 * (c) Copyright 2008 Actel Corporation.  All rights reserved.
 *
 * CoreGPIO example program.
 * 
 * This program reads the state of the GPIO inputs and writes the input state back
 * to the outputs. This would result in a change in the state of input switches
 * (buttons or banked switches) to be reflected in the state of LEDs.
 * This program is meant to be used with the Cortex-M1 baseline design.
 * 
 * SVN $Revision: 5 $
 * SVN $Date: 2013-12-23 13:52:47 +0100 (mån, 23 dec 2013) $
 */
#include "hal.h"
#include "platform.h"
#include "core_gpio.h"

#define GPOUT_INIT_STATE		0x00000000

gpio_instance_t g_gpio;

/******************************************************************************
 * main function.
 *****************************************************************************/ 
int main( void )
{
	uint32_t io_state;
	
	/**************************************************************************
	 * Initialize the CoreGPIO driver with the base address of the CoreGPIO
	 * instance to use and the initial state of the outputs. 
	 *************************************************************************/	
    GPIO_init( &g_gpio,	COREGPIO_BASE_ADDR, GPOUT_INIT_STATE );
    
	/**************************************************************************
	 * Infinite Loop.
	 *************************************************************************/		
    while(1)
    {
        /**********************************************************************
	     * Read inputs.
	     *********************************************************************/
        io_state = GPIO_get_input( &g_gpio );
        
        /**********************************************************************
	     * Write state of inputs back to the outputs.
	     *********************************************************************/
        GPIO_set_output( &g_gpio, io_state );	
    }
}
