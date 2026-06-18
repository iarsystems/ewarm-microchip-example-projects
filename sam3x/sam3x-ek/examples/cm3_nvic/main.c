/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support
 * ----------------------------------------------------------------------------
 * Copyright (c) 2010, Atmel Corporation
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Atmel's name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ----------------------------------------------------------------------------
 */

/**
 * \page cm3_nvic Cortex-M3 NVIC Example
 *
 * \section Purpose
 *
 * This example shows how to use the NVIC especially nested and priority
 * management.
 *
 * \section Requirements
 *
 * This example can be used with SAM3X evaluation kits.
 *
 * \section Description
 *
 * In this example, two interrupt sources will be configured, one is from
 * User Button, the other is from PIO output. We call them INT1 and INT2.
 * INT1 is triggered by press user button #1, and INT2 is triggered by
 * press user button #2.\n
 * \n
 * In INT1 handler:
 * <ul>
 * <li> At entrance, an enter message will be printed and LED1 will be
 * turned on.
 * <li> Then delay for a while.
 * <li> At the exit, an exit message will be printed and LED1 will be turned off.
 * </ul>
 * In INT2 handler:
 * <ul>
 * <li> At the entrance, an enter message will be printed and LED2 will be
 * turned on.
 * <li> Then delay for a while.
 * <li> At the exit, an exit message will be printed and LED2 will be turned off.
 * </ul>
 * Users can change priority of INT1 and INT2, nest and priority features can be
 * watched in terminal window or LEDs.
 *
 * \section Usage
 *
 * -# Build the program and download it inside the evaluation board. Please
 *    refer to the
 *    <a href="http://www.atmel.com/dyn/resources/prod_documents/doc6224.pdf">
 *    SAM-BA User Guide</a>, the
 *    <a href="http://www.atmel.com/dyn/resources/prod_documents/doc6310.pdf">
 *    GNU-Based Software Development</a> application note or to the
 *    <a href="ftp://ftp.iar.se/WWWfiles/arm/Guides/EWARM_UserGuide.ENU.pdf">
 *    IAR EWARM User Guide</a>, depending on your chosen solution.
 * -# On the computer, open and configure a terminal application
 *    (e.g. HyperTerminal on Microsoft Windows) with these settings:
 *   - 115200 bauds
 *   - 8 bits of data
 *   - No parity
 *   - 1 stop bit
 *   - No flow control
 * -# In the terminal window, the following text should appear:
 *    \code
 *     -- Cortex-M3 NVIC Example 1.0 --
 *     -- xxxxxx-xx
 *     -- Compiled: xxx xx xxxx xx:xx:xx --
 *     Set INT2's priority higher than INT1.
 *     Please connect PB0 to PB1 on the SAM3N-EK board for INT2 trigger.
 *
 *     Menu:
 *     ===================================================
 *     1: Set INT2's priority higher than INT1.
 *     2: Set INT1's priority higher than INT2.
 *     h: Show this menu.
 *
 *     ===================================================
 *     Press button USR-LEFT to trigger the interrupts.
 *    \endcode
 * -# Choose option in the above menu to set intterrupt priority.
 * -# Press button to trigger the interrupts. \n
 *    If INT2's priority higher than INT1, the following text should appear
 *    In the terminal window:
 *    \code
 *    Enter _Int1Handler.
 *        Enter _Int2Handler.
 *        Exit _Int2Handler.
 *    Exit _Int1Handler.
 *    \endcode
 *    If INT1's priority higher than INT2, the following text should appear
 *    In the terminal window:
 *    \code
 *    Enter _Int1Handler.
 *    Exit _Int1Handler.
 *        Enter _Int2Handler.
 *        Exit _Int2Handler.
 *    \endcode
 *    The result is also shown by LEDs.
 *
 * \section References
 * - cm3_nvic/main.c
 * - pio.h
 * - pio_it.h
 * - led.h
 */

/**
 * \file
 * This file contains all the specific code for the cm3_nvic.
 */

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

#include "board.h"

#include <stdint.h>
#include <stdio.h>

/*----------------------------------------------------------------------------
 *        Local definitions
 *----------------------------------------------------------------------------*/
/* Define interrupt priority (0-15). A higher level corresponds to a lower
 * priority, so level 0 is the highest interrupt priority.
 */
#define INT_PRIOR_HIGH    4
#define INT_PRIOR_LOW     6

/*----------------------------------------------------------------------------
 *        Local variables
 *----------------------------------------------------------------------------*/

/* INT1 pin instance. */
const Pin pinInt1 = PIN_PB_LEFT_CLICK ;

/* INT2 pin instance. */
const Pin pinInt2 = PIN_PB_RIGHT_CLICK ;

/*----------------------------------------------------------------------------
 *        Local functions
 *----------------------------------------------------------------------------*/

/**
 * \brief Delay for a while.
 */
static void _DelayMs( volatile uint32_t dwDelayMs )
{
    volatile uint32_t dwDelayTick=(dwDelayMs * (BOARD_MCK / 18000));

    while ( dwDelayTick-- );
}

/**
 * \brief Handler for INT1, rising edge interrupt. In INT1, it will trigger
 *        INT2.
 */
static void _Int1Handler( const Pin* pPin )
{
    PIO_PinDisableIt( pPin ) ;
    /* Enter INT1 */
    LED_Set(0);
    printf("Enter _Int1Handler.\n\r");

    /* Trigger INT2 & Delay for a while */
    _DelayMs( 1000 );

    /* Exit INT1 */
    LED_Clear(0);
    printf("Exit _Int1Handler.\n\r");

    PIO_PinEnableIt( pPin ) ;
}

/**
 * \brief Handler for INT2, rising edge interrupt.
 */
static void _Int2Handler( const Pin* pPin )
{
    PIO_PinDisableIt( pPin ) ;

    /* Enter INT2 */
    LED_Set( 1 ) ;
    printf( "    Enter _Int2Handler.\n\r" ) ;

    /* Delay for a while */
    _DelayMs( 1000 ) ;

    /* Exit INT2 */
    LED_Clear( 1 ) ;
    printf( "    Exit _Int2Handler.\n\r" ) ;

    PIO_PinEnableIt( pPin ) ;
}

/**
 * \brief Configure PIOs for intterrupt.
 * Steps:
 * 1. Configure pios as inputs
 * 2. Adjust pio debounce filter patameters
 * 3. Initialize pios interrupt handlers
 * 4. Enable PIO controller IRQs
 * 5. Enable PIO line interrupts
 */
static void _ConfigureIntPio( void )
{
    /* ============ Configure INT1 pin ============ */
    /* Configure pios as inputs. */
    PIO_PinConfigure( &pinInt1, 1 ) ;

    /* Adjust pio debounce filter patameters, uses 1 Hz filter. */
    PIO_PinSetDebounceFilter( &pinInt1, 1 ) ;

    /* Initialize pios interrupt handlers. */
    PIO_PinConfigureIt( &pinInt1, _Int1Handler ) ;

    /* Enable PIO controller IRQs. */
    NVIC_EnableIRQ( (IRQn_Type)pinInt1.id ) ;

    /* Enable PIO line interrupts. */
    PIO_PinEnableIt( &pinInt1 ) ;

    /* ============ Configure INT2 pin ============ */
    /* Configure pios as inputs. */
    PIO_PinConfigure( &pinInt2, 1 ) ;

    /* Adjust pio debounce filter patameters, uses 1 Hz filter. */
    PIO_PinSetDebounceFilter( &pinInt2, 1 ) ;

    /* Initialize pios interrupt handlers. */
    PIO_PinConfigureIt( &pinInt2, _Int2Handler ) ;

    /* Enable PIO controller IRQs. */
    NVIC_EnableIRQ( (IRQn_Type)pinInt2.id ) ;

    /* Enable PIO line interrupts. */
    PIO_PinEnableIt( &pinInt2 ) ;
}

/**
 * \brief Set interrupt priority of INT1 and INT2.
 *
 * \param int1Prior priority of interrupt 1
 * \param int2Prior priority of interrupt 2
 *
 * Step to change the IRQ priority
 * 1. Disable the interrupt service handler
 * 2. Clear the pending interrupt service handler
 * 3. Set the new priority
 * 4. Enable the interrupt service handler
 */
static void _SetIntPriority(uint8_t int1Prior, uint8_t int2Prior)
{
    /* Set INT1 priority */
    NVIC_DisableIRQ( (IRQn_Type)pinInt1.id ) ;
    NVIC_ClearPendingIRQ( (IRQn_Type)pinInt1.id ) ;
    NVIC_SetPriority( (IRQn_Type)pinInt1.id, int1Prior) ;
    NVIC_EnableIRQ( (IRQn_Type)pinInt1.id ) ;

    /* Set INT2 priority */
    NVIC_DisableIRQ( (IRQn_Type)pinInt2.id ) ;
    NVIC_ClearPendingIRQ( (IRQn_Type)pinInt2.id ) ;
    NVIC_SetPriority( (IRQn_Type)pinInt2.id, int2Prior ) ;
    NVIC_EnableIRQ( (IRQn_Type)pinInt2.id ) ;
}

/**
 * \brief Display menu.
 */
static void _DisplayMenu( void )
{
    printf( "\n\r" ) ;
    printf( "Menu:\n\r" ) ;
    printf( "===================================================\n\r" ) ;
    printf( "1: Set INT2's priority higher than INT1.\n\r" ) ;
    printf( "2: Set INT1's priority higher than INT2.\n\r" ) ;
    printf( "h: Show this menu.\n\r" ) ;
    printf( "===================================================\n\r" ) ;
    printf( "Press button USR-LEFT/USR-RIGHT to trigger the interrupts.\n\r" ) ;
    printf( "\n\r" ) ;
}

/*----------------------------------------------------------------------------
 *        Exported functions
 *----------------------------------------------------------------------------*/

/**
 * \brief Application entry point.
 *
 * \return Unused (ANSI-C compatibility).
 */
int main( void )
{
    uint8_t ucKey;

    /* Disable watchdog */
    WDT_Disable( WDT ) ;

    /* Output example information */
    printf("-- Cortex-M3 NVIC Example %s --\n\r", SOFTPACK_VERSION ) ;
    printf("-- %s\n\r", BOARD_NAME);
    printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);

    /* PIO configuration for LEDs and Buttons. */
    PIO_InitializeInterrupts( 0 ) ;

    printf( "Configure LED PIOs.\n\r" ) ;
    LED_Configure( 0 ) ;
    LED_Configure( 1 ) ;

    printf( "Configure interrupt PIOs.\n\r" ) ;
    _ConfigureIntPio() ;

    /* Set interrupt priority */
    printf( "Set INT2's priority higher than INT1.\n\r" ) ;
    _SetIntPriority(INT_PRIOR_LOW, INT_PRIOR_HIGH);

    _DisplayMenu() ;
    while ( 1 )
    {
        ucKey = UART_GetChar() ;
        switch ( ucKey )
        {
            case '1' :
                _SetIntPriority(INT_PRIOR_LOW, INT_PRIOR_HIGH);
                printf( "Set INT2's priority higher than INT1.\n\r\n\r" ) ;
                break;
            case '2' :
                _SetIntPriority(INT_PRIOR_HIGH, INT_PRIOR_LOW);
                printf( "Set INT1's priority higher than INT2.\n\r\n\r" ) ;
                break;
            case 'h':
                _DisplayMenu() ;
                break;
            default:
                printf("Invalid input.\n\r");
                break ;
        }
    }
}

