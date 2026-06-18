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
 * \page rtt RTT Example
 *
 * \section Purpose
 *
 * This example demonstrates the Real-Time Timer (RTT) provided on
 * SAM3X microcontrollers. It enables the user to set an alarm and watch
 * it being triggered when the timer reaches the corresponding value.
 *
 * You can configure the \ref rtt_module "RTT" by following steps
 * - SetPrescaler the RTT to 1s (32768)
 * - Initialize the ISR routine which refesh it when 1s is counted down
 * - Enable the RTT Interrtup of the vector
 *
 * \section Requirements
 *
 * This package can be used with SAM3X evaluation kits.
 *
 * \section Description
 *
 * When launched, this program displays a timer count and a menu on the terminal,
 * enabling the user to choose between several options.
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
 * -# Start the application.
 * -# In the terminal window, the following text should appear:
 *    \code
 *     -- RTT Example xxx --
 *     -- xxxxxx-xx
 *     -- Compiled: xxx xx xxxx xx:xx:xx --
 *     Time: 0
 *     Menu:
 *     r - Reset timer
 *     s - Set alarm
 *     Choice?
 *    \endcode
 *
 * The user can then choose any of the available options to perform
 * the described action.
 *
 * \section References
 * - rtt/main.c
 * - rtt.c
 * - rtt.h
 */

/**
 * \file
 *
 * This file contains all the specific code for the rtt example.
 *
 */

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

#include "board.h"

/* These headers were introduced in C99 by working group ISO/IEC JTC1/SC22/WG14. */
#include <stdint.h>
#include <stdio.h>

/*----------------------------------------------------------------------------
 *        Local types
 *----------------------------------------------------------------------------*/

typedef enum _EStates
{
  STATE_MENU=0,
  STATE_SET_ALARM,
  STATE_ALARM,
  STATE_NONE
} EStates ;

/*----------------------------------------------------------------------------
 *        Local variables
 *----------------------------------------------------------------------------*/

/** Current device state. */
volatile EStates eRTTStatus ;

/** New alarm time being currently entered. */
volatile uint32_t dwNewAlarm ;

/** Indicates if an alarm has occured but has not been cleared. */
volatile uint32_t dwAlarmed ;

/*----------------------------------------------------------------------------
 *        Local definitions
 *----------------------------------------------------------------------------*/
/** Macro for backspace key */
#define KEY_BACKSPACE_CODE           (8)
/** Macro for Enter key */
#define KEY_ENTER_CODE               (13)

/*----------------------------------------------------------------------------
 *        Local functions
 *----------------------------------------------------------------------------*/


/**
 * \brief Refresh display on terminal.
 *
 * Updates the terminal display to show the current menu and the current time
 * depending on the device state.
 */
static void _RefreshDisplay( void )
{
    printf( "%c[2J\r", 27 ) ;
    printf( "Time: %u\n\r", (uint32_t)RTT_GetTime( RTT ) ) ;

    switch ( eRTTStatus )
    {
        case STATE_MENU :
          printf( "Menu:\n\r" ) ;
          printf( " r - Reset timer\n\r" ) ;
          printf( " s - Set alarm\n\r" ) ;
          printf( "\n\rChoice? " ) ;
          break;

        case STATE_ALARM :
          printf( "!!! ALARM !!!\n\r" ) ;
          printf( " c - Press 'c' to clear alarm notification\n\r" ) ;
        break;

        case STATE_SET_ALARM :
          printf( "Enter alarm time: " ) ;

          if ( dwNewAlarm != 0 )
          {
            printf( "%u", dwNewAlarm ) ;
          }
        break;

        default :
          printf( "Menu:\n\r" ) ;
          printf( " r - Reset timer\n\r" ) ;
          printf( " s - Set alarm\n\r" ) ;
          printf( "\n\rChoice? " ) ;
        break ;
    }
}

/**
 * \brief RTT configuration function.
 *
 * Configures the RTT to generate a one second tick, which triggers the RTTINC
 * interrupt.
 */
static void _ConfigureRtt( void )
{
    uint32_t previousTime ;

    /* Configure RTT for a 1 second tick interrupt */
    RTT_SetPrescaler( RTT, 32768 ) ;
    previousTime = RTT_GetTime( RTT ) ;
    while ( previousTime == RTT_GetTime( RTT ) ) ;

    /* Enable RTT interrupt */
    NVIC_DisableIRQ( RTT_IRQn ) ;
    NVIC_ClearPendingIRQ( RTT_IRQn ) ;
    NVIC_SetPriority( RTT_IRQn, 0 ) ;
    NVIC_EnableIRQ( RTT_IRQn ) ;
    RTT_EnableIT( RTT, RTT_MR_RTTINCIEN ) ;
}

/**
 * \brief Interrupt handler for the RTT.
 *
 * Displays the current time on the terminal.
 */
void RTT_IrqHandler( void )
{
    uint32_t dwStatus ;

    /* Get RTT status */
    dwStatus = RTT_GetStatus( RTT ) ;

    /* Time has changed, refresh display */
    if ( (dwStatus & RTT_SR_RTTINC) == RTT_SR_RTTINC )
    {
       _RefreshDisplay() ;
    }

    /* Alarm */
    if ( (dwStatus & RTT_SR_ALMS) == RTT_SR_ALMS )
    {
        eRTTStatus = STATE_ALARM ;
        _RefreshDisplay() ;
    }
}

/*----------------------------------------------------------------------------
 *         Global functions
 *----------------------------------------------------------------------------*/

/**
 * \brief Application entry point for RTT example.
 *
 * Initializes the RTT, displays the current time and allows the user to
 * perform several actions: clear the timer, set an alarm, etc.
 *
 * \return Unused (ANSI-C compatibility).
 */
extern int main( void )
{
    uint8_t ucKey ;

    /* Disable watchdog */
    WDT_Disable( WDT ) ;

    /* Output example information */
    printf( "-- RTT Example %s --\n\r", SOFTPACK_VERSION ) ;
    printf( "-- %s\n\r", BOARD_NAME ) ;
    printf( "-- Compiled: %s %s --\n\r", __DATE__, __TIME__ ) ;

    /* Configure RTT */
    _ConfigureRtt() ;

    /* Initialize state machine */
    eRTTStatus = STATE_MENU ;
    dwAlarmed = 0;
    _RefreshDisplay() ;

    /* User input loop */
    while ( 1 )
    {
        /* Wait for user input */
        ucKey = UART_GetChar() ;

        switch ( ucKey )
        {
            case 'r':
                _ConfigureRtt() ;
                dwNewAlarm = 0;
                eRTTStatus = STATE_MENU ;
            break ;

            case 's' :
                dwNewAlarm = 0 ;
                eRTTStatus = STATE_SET_ALARM ;
            break ;

            case KEY_ENTER_CODE :
                /* Avoid dwNewAlarm = 0 case */
                if ( dwNewAlarm != 0 )
                {
                    RTT_SetAlarm( RTT, dwNewAlarm ) ;
                }
                eRTTStatus = STATE_MENU ;
            break ;

            case 'c' :
                dwNewAlarm = 0 ;
                eRTTStatus = STATE_MENU ;
            break ;

            case '0' :
            case '1' :
            case '2' :
            case '3' :
            case '4' :
            case '5' :
            case '6' :
            case '7' :
            case '8' :
            case '9' :
                if ( eRTTStatus == STATE_SET_ALARM )
                {
                    dwNewAlarm = dwNewAlarm * 10 + ucKey - '0' ;
                }
            break ;

            case KEY_BACKSPACE_CODE :
                if ( eRTTStatus == STATE_SET_ALARM )
                {
                    dwNewAlarm /= 10 ;
                }
            break ;

            default :
            break ;
        }

        _RefreshDisplay() ;
    }
}

