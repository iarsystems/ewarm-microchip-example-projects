/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support  -  ROUSSET  -
 * ----------------------------------------------------------------------------
 * Copyright (c) 2007, Atmel Corporation

 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaiimer below.
 *
 * - Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the disclaimer below in the documentation and/or
 * other materials provided with the distribution.
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
/// \dir "Basic slow clock project"
///
/// !!!Purpose
///
/// This example allows to switch the slow clock from the internal RC
/// oscillator to the external 32.768kHz crystal, and inversely. 
/// User can get the slow clock frequency thanks to the Main Oscillator
/// sampling.
///
/// !See
/// - slck: Slow Clock driver
///
/// !!!Description
///
/// When launched, this program displays the current Slow Clock selection 
/// at start and a menu on the DBGU, enabling the user to choose between 
/// several options: 
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
/// -# Start the application.
/// -# In the terminal window, the following text should appear:
///    \code
///     -- Basic Slow Clock Project xxx --
///     -- AT91xxxxxx-xx
///     -- Compiled: xxx xx xxxx xx:xx:xx --
///     -I- The slow clock is the 32k/RC oscillator\n\r");
///     Menu:
///      1 - 32k to RC
///      2 - RC to 32k    
///      3 - Get Slow clock frequency      
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
/// This file contains all the specific code for the basic-slowclock-project
///
/// !Contents
///
/// !See also
/// - slck: Slow Clock driver
///
/// Please refer to the list of functions in the #Overview# tab of this unit
/// for more detailed information.
//-----------------------------------------------------------------------------

//------------------------------------------------------------------------------
//         Headers
//------------------------------------------------------------------------------

#include <board.h>
#include <board_lowlevel.h>
#include <pio/pio.h>
#include <dbgu/dbgu.h>
#include <slck/slck.h>
#include <utility/trace.h>

#include <stdio.h>

//------------------------------------------------------------------------------
//         Local functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Configure PCK
//------------------------------------------------------------------------------
static void ConfigurePCK(void)
{
#if defined(PIN_PCK0) && defined(PIN_PCK1) && defined(PIN_PCK2)
    // Configure pios
    static const Pin pck_pins[] = {PIN_PCK0, PIN_PCK1, PIN_PCK2};
    PIO_Configure(pck_pins, PIO_LISTSIZE(pck_pins));
    
    AT91C_BASE_PMC->PMC_PCKR[0] = AT91C_PMC_CSS_SLOW_CLK | AT91C_PMC_PRES_CLK;
    AT91C_BASE_PMC->PMC_PCKR[1] = AT91C_PMC_CSS_MAIN_CLK | AT91C_PMC_PRES_CLK_8;
    AT91C_BASE_PMC->PMC_PCKR[2] = AT91C_PMC_CSS_PLLA_CLK | AT91C_PMC_PRES_CLK_64; 
    
    AT91C_BASE_PMC->PMC_SCER = AT91C_PMC_PCK0 | AT91C_PMC_PCK1 | AT91C_PMC_PCK2;
#endif
}

//------------------------------------------------------------------------------
//         Exported functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Main function
//------------------------------------------------------------------------------
int main()
{
    unsigned char key;  
    unsigned int freq;
  
    // DBGU configuration
    TRACE_CONFIGURE(DBGU_STANDARD, 115200, BOARD_MCK);
    printf("-- Basic Slow Clock project %s --\n\r", SOFTPACK_VERSION);
    printf("-- %s\n\r", BOARD_NAME);
    printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);
  
    if( SLCK_Is32k() ) {
        printf("-I- The slow clock is the external 32.768kHz crystal\n\r");
    } else {
        printf("-I- The slow clock is the internal RC oscillator\n\r");      
    }

    ConfigurePCK();
          
    // Main loop
    while (1) {
        printf("\n\r");
        printf("Menu:\n\r");       
        printf(" 1 - 32k to RC\n\r");
        printf(" 2 - RC to 32k\n\r");    
        printf(" 3 - Get Slow clock frequency\n\r");            
            
        key = DBGU_GetChar();
        printf("Press '%c'\n\r", key);
        printf("\n\r");
        switch(key) {
            case '1' : 
            {
                SLCK_32ktoRC();
            }
            break;
            case '2' : 
            {
                SLCK_RCto32k();
            }
            break;
            case '3' : 
            {
                freq = SLCK_UtilGetFreq();
                printf("Slow Clock Frequency = %u Hz\n\r", freq);               
            }
            break;           
            default:     
            printf("Unknown menu!\n\r");
        }
    }

    return 0;
}

