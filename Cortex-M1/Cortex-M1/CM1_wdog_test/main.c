/*******************************************************************************
 * (c) Copyright 2008 Actel Corporation.  All rights reserved.
 * 
 * This simple CoreWatchdog example echoes back the state of GPIO inputs to the
 * GPIO outputs. The state of the LEDs is changed every time a key is pressed.
 * Keeping a key pressed for more than 5 seconds causes the watchdog to fire and
 * reset the system. All LEDs are turned on after a reset allowing to easily
 * seeing that the system is after coming out of reset.
 *
 * SVN $Revision: 5 $
 * SVN $Date: 2013-12-23 13:52:47 +0100 (mån, 23 dec 2013) $
 */
#include "hal.h"
#include "platform.h"
#include "core_gpio.h"
#include "core_watchdog.h"

/******************************************************************************
 * GPIO initial state is all LEDs switched on.
 *****************************************************************************/ 
#define GPIO_INITIAL_STATE  0xFFFFFFFFUL

/******************************************************************************
 * Prescaler and load value for the watchdog. These should result in a watchdog
 * timout after 5 seconds. These assume a 12.5MHz system frequency.
 *****************************************************************************/ 
#define WD_PRESCALE         WD_PRESCALER_DIV_1024
#define WD_LOAD_VALUE       0xEE6B

/******************************************************************************
 * Value retuned by function GPIO_get_input() when no keys are pressed.
 *****************************************************************************/ 
#define NO_KEY_PRESSED      0

/******************************************************************************
 * main function.
 *****************************************************************************/ 
int main()
{
    static gpio_instance_t the_gpio;
    static wd_instance_t the_wdog;
    uint32_t gpio_state;
    
	/**************************************************************************
	 * Initialize CoreGPIO with its base address and switch all LEDs on.
	 *************************************************************************/	
    GPIO_init( &the_gpio, COREGPIO_BASE_ADDR, GPIO_INITIAL_STATE );
    
	/**************************************************************************
	 * Initialize CoreWatchdog with its base address and configure prescaler and
     * load values used to specify the watchdog timeout delay.
	 *************************************************************************/	
    WD_init( &the_wdog,	COREWATCHDOG_BASE_ADDR,	WD_LOAD_VALUE, WD_PRESCALE );
    
	/**************************************************************************
	 * Wait for all keys to be released.
	 *************************************************************************/	
    do {
        gpio_state = GPIO_get_input( &the_gpio );
    } while ( gpio_state != NO_KEY_PRESSED);
    
	/**************************************************************************
	 * Enable the CoreWatchdog timer.
	 *************************************************************************/	
    WD_enable( &the_wdog );
    
	/**************************************************************************
	 * Infinite loop.
	 *************************************************************************/	
    while(1)
    {
        uint32_t gpio_state;
        
        /**********************************************************************
	     * Read the GPIO inputs state.
	     *********************************************************************/		
        gpio_state = GPIO_get_input( &the_gpio );
    
        /**********************************************************************
	     * Only do something if at least one key is pressed.
	     *********************************************************************/		
        if ( gpio_state != NO_KEY_PRESSED )
        {
            /******************************************************************
             * Set the GPIO output state to the input state. This causes the 
             * pressed keys to be echoed back on the LEDs.
             *****************************************************************/	
            GPIO_set_output( &the_gpio, gpio_state );

            /******************************************************************
             * Wait for all keys to be released.
             *****************************************************************/	
            do {
                gpio_state = GPIO_get_input( &the_gpio );
            } while ( gpio_state != NO_KEY_PRESSED);
        }

        /**********************************************************************
	     * Reload the watchdog timer load value.
	     *********************************************************************/		
        WD_reload( &the_wdog );
    }
    
    return 0;
}
