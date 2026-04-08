/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support
 * ----------------------------------------------------------------------------
 * Copyright (c) 2008, Atmel Corporation
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

//-----------------------------------------------------------------------------
/// \dir "Shutdown Controller"
///
/// !!!Purpose
///
/// This example demonstrates Shutdown Controller (SHDWC) provided on
/// AT91 microcontrollers. The Shutdown Controller controls the power supplies
/// VDDIO and VDDCORE and the wake-up detection on debounced input lines. It enables
/// the user to power off VDDIO and VDDCORE and to power on VDDIO and VDDCORE by
/// pressing the Wake Up button or by configuring an RTT alarm.
///
/// !See
/// - shdwc: Shutdown Controller driver
/// - rtt: Real Timer Timer driver
///
/// !!!Requirements
///
/// This package can be used with all Atmel evaluation kits. The package runs at 
/// SRAM or SDRAM, so SDRAM device is needed if you want to run this package in SDRAM.
/// The jumper "Force Power On " has to be opened before.
///
/// !!!Description
///
/// The demonstration program shutdowns the board and allows one of the wake up
/// possibility (RTT alarm or Wake Up button), or both, depending on the user selection.
/// The RTT alarm duration and the Wake up signal charateristic can be 
/// configured also by user.
///
/// !!!Usage
///
/// -# Build the program and download it inside the evaluation board. Please
///    refer to the
///    <a href="http://www.atmel.com/dyn/resources/prod_documents/doc6224.pdf">
///    SAM-BA User Guide</a>, the
///    <a href="http://www.atmel.com/dyn/resources/prod_documents/doc6310.pdf">
///    GNU-Based Software Development</a> application note or to the
///    <a href="ftp://ftp.iar.se/WWWfiles/arm/Guides/EWARM_UserGuide.ENU.pdf">
///    IAR EWARM User Guide</a>, depending on your chosen solution.
/// -# On the computer, open and configure a terminal application
///    (e.g. HyperTerminal on Microsoft Windows) with these settings:
///   - 115200 bauds
///   - 8 bits of data
///   - No parity
///   - 1 stop bit
///   - No flow control
/// -# Set a battery to supply the backup power
/// -# Open the Force Power On jumper
/// -# Start the application.
/// -# In the terminal window, the following text should appear:
///    \code
///    -- Basic Shutdown Project xxx --
///    -- AT91xxxxxx-xx
///    -- Compiled: xxx xx xxxx xx:xx:xx --
///    Config:
///     a - RTT alarm     : 5 seconds
///     m - WK0 mode      : 3 Low2High(1) High2Low(2), Both(3)
///     d - WK0 debounce  : 100 (x 16 slow clock)
///    Action:
///     1 - WK0 enable & RTT alarm disable
///     2 - WK0 disable & RTT alarm enable
///     3 - WK0 enable & RTT alarm enable
///     4 - WK0 disable & RTT alarm disable
///    Debug:
///     r - Display registers
///    \endcode
///
/// The user can then choose any of the available options to perform the described action.
///
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/// \unit
///
/// !Purpose
///
/// This file contains all the specific code for the basic-shutdown-project
///
/// !Contents
///
/// !See also
/// - shdwc: Shutdown Controller driver
/// - rtt: Real Timer Timer driver
///
/// Please refer to the list of functions in the #Overview# tab of this unit
/// for more detailed information.
//-----------------------------------------------------------------------------

//------------------------------------------------------------------------------
//         Headers
//------------------------------------------------------------------------------
#include <board.h>
#include <rtt/rtt.h>
#include <shdwc/shdwc.h>
#include <irq/irq.h>
#include <utility/trace.h>
#include <stdio.h>

//------------------------------------------------------------------------------
//         Local definitions
//------------------------------------------------------------------------------
unsigned int rtt_alarm    = 5;
unsigned int wk0_mode     = 3;
unsigned int wk0_debounce = 100;

//------------------------------------------------------------------------------
//         Global functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// SetRttAlarm
//------------------------------------------------------------------------------
static void SetRttAlarm(unsigned int value)
{
    unsigned int previousTime;

    // Configure RTT for a 1 second tick interrupt
    RTT_SetPrescaler(AT91C_BASE_RTTC, 32768);
    previousTime = RTT_GetTime(AT91C_BASE_RTTC);
    while (previousTime == RTT_GetTime(AT91C_BASE_RTTC));

    // reset previous alarm
    RTT_GetStatus(AT91C_BASE_RTTC);

    // set new alarm value
    RTT_SetAlarm(AT91C_BASE_RTTC, value);
}

//------------------------------------------------------------------------------
/// DisplayRegister
//------------------------------------------------------------------------------
static void DisplayRegister(void)
{
    printf("\n\r");
    printf("RTTC_RTMR  %08x\n\r", AT91C_BASE_RTTC->RTTC_RTMR);
    printf("RTTC_RTSR  %08x\n\r", AT91C_BASE_RTTC->RTTC_RTSR);
    printf("RTTC_RTAR  %08x\n\r", AT91C_BASE_RTTC->RTTC_RTAR);
    printf("RTTC_RTVR  %08x\n\r", AT91C_BASE_RTTC->RTTC_RTVR);

    printf("SHDWC_SHMR %08x\n\r", AT91C_BASE_SHDWC->SHDWC_SHMR);
    printf("SHDWC_SHSR %08x\n\r", AT91C_BASE_SHDWC->SHDWC_SHSR);
}

//------------------------------------------------------------------------------
/// Main
//------------------------------------------------------------------------------
int main(void)
{
    unsigned char key;
    unsigned int status;

    TRACE_CONFIGURE(DBGU_STANDARD, 115200, BOARD_MCK);
    printf("-- Basic Shutdown Project %s --\n\r", SOFTPACK_VERSION);
    printf("-- %s\n\r", BOARD_NAME);
    printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);

    // Display the shutdown status
    printf("\n\r");
    printf("Reminber:\n\r");
    printf(" - a battery have to supply the backup power.\n\r");
    printf(" - the force power on jumper have to be opened.\n\r");

    // Display the shutdown status
    printf("\n\r");
    printf("Shutdown status:\n\r");
    status = SHDWC_GetStatus();
    if(status & AT91C_SHDWC_WAKEUP0) {
        printf("Wake-up event occurred on the wake-up input\n\r");
    }
    if(status & AT91C_SHDWC_RTTWK) {
        printf("Wake-up alarm from the RTT\n\r");
    }
    printf("\n\r");

    while(1) {

        // Display menu
        printf("\n\r");
        printf("Config:\n\r");
        printf(" a - RTT alarm     : %d seconds\n\r", rtt_alarm);
        printf(" m - WK0 mode      : %d Low2High(1) High2Low(2), Both(3)\n\r", wk0_mode);
        printf(" d - WK0 debounce  : %d (x 16 slow clock)\n\r", wk0_debounce);
        printf("Action:\n\r");
        printf(" 1 - WK0 enable & RTT alarm disable\n\r");
        printf(" 2 - WK0 disable & RTT alarm enable\n\r");
        printf(" 3 - WK0 enable & RTT alarm enable\n\r");
        printf(" 4 - WK0 disable & RTT alarm disable\n\r");
        printf("Debug:\n\r");
        printf(" r - Display registers\n\r");
        printf("GPBR:\n\r");
        printf(" w - GPBR0 : %08x\n\r", *(AT91C_SYS_GPBR+0));
        printf(" x - GPBR1 : %08x\n\r", *(AT91C_SYS_GPBR+1));
        printf(" y - GPBR2 : %08x\n\r", *(AT91C_SYS_GPBR+2));
        printf(" z - GPBR3 : %08x\n\r", *(AT91C_SYS_GPBR+3));
                                
        // Wait for user input
        key = DBGU_GetChar();
        printf("\n\rPress '%c'\n\r", key);

        // Treat user input
        switch(key) {
            // config
            case 'a':
            {
                unsigned int value;
                printf("Enter RTT alarm: ");
                if( TRACE_GetInteger(&value) ) {
                    rtt_alarm = value;
                }
            }
            break;
            case 'm':
            {
                unsigned int value;
                printf("Enter WK0 mode: ");
                if( TRACE_GetIntegerMinMax(&value,1,3) ) {
                    wk0_mode = value;
                }
            }
            break;
            case 'd':
            {
                unsigned int value;
                printf("Enter WK0 debounce: ");
                if( TRACE_GetInteger(&value) ) {
                    wk0_debounce = value;
                }
            }
            break;
            // action
            case '1':
            {
                SHDWC_DoShutDown(0, wk0_mode, wk0_debounce);
                while(1);
            }
            break;
            case '2':
            {
                SetRttAlarm(rtt_alarm);
                SHDWC_DoShutDown(1, 0, 0);
                while(1);
            }
            break;
            case '3':
            {
                SetRttAlarm(rtt_alarm);
                SHDWC_DoShutDown(1, wk0_mode, wk0_debounce);
                while(1);
            }
            break;
            case '4':
            {
                SHDWC_DoShutDown(0, 0, 0);
                while(1);
            }
            break;
            // debug
            case 'r':
            {
                DisplayRegister();
            }
            break;
            
            case 'w':
            {
                unsigned int value;
                printf("Enter a value: ");
                if( TRACE_GetHexa32(&value) ) {
                    *(AT91C_SYS_GPBR+0) = value;
                }
            }
            break;            
            case 'x':
            {
                unsigned int value;
                printf("Enter a value: ");
                if( TRACE_GetHexa32(&value) ) {
                    *(AT91C_SYS_GPBR+1) = value;
                }
            }
            break; 
            case 'y':
            {
                unsigned int value;
                printf("Enter a value: ");
                if( TRACE_GetHexa32(&value) ) {
                    *(AT91C_SYS_GPBR+2) = value;
                }
            }
            break; 
            case 'z':
            {
                unsigned int value;
                printf("Enter a value: ");
                if( TRACE_GetHexa32(&value) ) {
                    *(AT91C_SYS_GPBR+3) = value;
                }
            }
            break;                                        
            default:
                printf("Unknown menu!\n\r");
        }
    }
}

