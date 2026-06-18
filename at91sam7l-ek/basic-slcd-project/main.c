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

//------------------------------------------------------------------------------
/// \dir "Segment LCD Project"
///
/// !!!Purpose
///
/// This example shows how to use the Segment LCD Controller (SLCDC) to display 
/// information on the LCD screen. 
///
/// !See
/// - slcdc: Segment LCD controller driver
/// - pit: Periodic interval timer driver
/// - s7leklcd: APIs for segment LCD display of AT91SAM7L-EK board
///
/// !!!Requirements
///
/// This package can be used with all Atmel evaluation kits that have
/// SLCDC interface.
///
/// !!!Description
/// The program displays the following information on the SLCD: 
/// -# BasicSLCD 1.4 string 
/// -# Atmel logo 
/// -# Current time 
/// -# Dot matrix animation 
///
/// !!!Usage
///
/// -# Build the program and download it inside the evaluation board. 
/// Please refer to the
///    <a href="http://www.atmel.com/dyn/resources/prod_documents/doc6224.pdf">
///    SAM-BA User Guide</a>, the 
///    <a href="http://www.atmel.com/dyn/resources/prod_documents/doc6310.pdf">
///    GNU-Based Software Development</a>, application note or to the 
///    <a href="http://www.iar.se/website1/1.0.1.0/78/1/index.php?">
///    IAR EWARM User Guide</a>, depending on your chosen solution. 
/// -# #Optionally#, on the computer, open the DBGU port with a terminal 
/// application (e.g. HyperTerminal on Microsoft Windows). 
///       - Settings: 115200 bauds, 8 bits, 1 stop bit, no parity, no flow control. 
/// -# Start the application. 
/// -# The program will output a startup message on the DBGU:
///     \code 
///     -- Basic SLCD Project xxx --
///     -- AT91xxxxxx-xx
///     -- Compiled: xxx xx xxxx xx:xx:xx --
///     \endcode
/// In addition, the SLCD will display the aforementioned information. 
///
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// \unit
///
/// !Purpose
///
/// This file contains all the specific code for the 
/// basic-slcd-project.
///
/// !Contents
///
/// The code can be roughly broken down as follows:
///    - PIT configuration functions and Interrupt handlers
///    - The main function, which implements the program behavior
///       - Initialize SLCD
///       - Switch on the Atmel logo and the clock
///       - Display "Basic SLCD 1.4"
///       - Update time and Dot matrix animation 
///
/// Please refer to the list of functions in the #Overview# tab of this unit
/// for more detailed information.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//         Headers
//------------------------------------------------------------------------------

#include <board.h>
#include <irq/irq.h>
#include <pit/pit.h>
#include <slcdc/slcdc.h>
#include <supc/supc.h>
#include <utility/trace.h>
#include <utility/assert.h>
#include <components/slcd/s7leklcd/s7leklcd.h>

#include <stdio.h>

//------------------------------------------------------------------------------
//         Local Constants
//------------------------------------------------------------------------------

/// PIT period value in clock ticks.
#define PIT_PERIOD        1000

//------------------------------------------------------------------------------
//         Local variables
//------------------------------------------------------------------------------

 /// List of the ten SLCD clock numbers
static const unsigned short pClockNumbers[] = {

    S7LEKLCD_C0,
    S7LEKLCD_C1,
    S7LEKLCD_C2,
    S7LEKLCD_C3,
    S7LEKLCD_C4,
    S7LEKLCD_C5,
    S7LEKLCD_C6,
    S7LEKLCD_C7,
    S7LEKLCD_C8,
    S7LEKLCD_C9
};

//------------------------------------------------------------------------------
//         Local functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Updates the time display on the LCD.
/// \param seconds  Number of elapsed seconds since startup.
/// \param milliseconds  Number of elapsed milliseconds since last second.
//------------------------------------------------------------------------------
static void UpdateTime(unsigned int seconds, unsigned int milliseconds)
{
    unsigned int minutes = seconds / 60;

    // Display center dots if necessary
    if (milliseconds >= 500) {

        S7LEKLCD_Symbol(S7LEKLCD_COL4, 1);
    }
    else {

        S7LEKLCD_Symbol(S7LEKLCD_COL4, 0);
    }

    // Display seconds
    seconds -= minutes * 60;
    S7LEKLCD_ClockNumber(pClockNumbers[(seconds % 10)], 3);
    seconds /= 10;
    S7LEKLCD_ClockNumber(pClockNumbers[(seconds % 10)], 2);

    // Display minutes
    S7LEKLCD_ClockNumber(pClockNumbers[(minutes % 10)], 1);
    minutes /= 10;
    S7LEKLCD_ClockNumber(pClockNumbers[(minutes % 10)], 0);
}

//------------------------------------------------------------------------------
/// Updates the pixel display on the LCD depending on the current time.
/// \param seconds  Number of elapsed seconds since startup.
/// \param milliseconds  Number of elapsed milliseconds since the last second.
//------------------------------------------------------------------------------
static void UpdatePixels(unsigned int seconds, unsigned int milliseconds)
{
    signed char x, y;
    unsigned char xs, xe;
    unsigned char ys, ye;
    unsigned char pixels;

    // Compute the number of pixels to turn on
    pixels = (seconds % 60) * 2 + (milliseconds / 500);

    // Switch pixels on/off in a spiral shape
    xs = 0;
    xe = 12;
    ys = 0;
    ye = 10;
    while (ys != ye) {

        // Top line
        for (x = xs; x < xe; x++) {

            if (pixels > 0) {
            
                S7LEKLCD_Pixel(x, ys, 1);
                pixels--;
            }
            else {

                S7LEKLCD_Pixel(x, ys, 0);
            }
        }

        // Right line
        for (y = ys+1; y < ye; y++) {

            if (pixels > 0) {
            
                S7LEKLCD_Pixel(xe-1, y, 1);
                pixels--;
            }
            else {

                S7LEKLCD_Pixel(xe-1, y, 0);
            }
        }

        // Bottom line
        for (x = xe-2; x >= xs; x--) {

            if (pixels > 0) {
            
                S7LEKLCD_Pixel(x, ye-1, 1);
                pixels--;
            }
            else {

                S7LEKLCD_Pixel(x, ye-1, 0);
            }
        }

        // Left line
        for (y = ye-2; y > ys; y--) {

            if (pixels > 0) {
            
                S7LEKLCD_Pixel(xs, y, 1);
                pixels--;
            }
            else {

                S7LEKLCD_Pixel(xs, y, 0);
            }
        }

        xs++;
        xe--;
        ys++;
        ye--;
    }
}

//------------------------------------------------------------------------------
/// PITC interrupt handler. Increment the jiffies counter.
//------------------------------------------------------------------------------
static void ISR_Pit(void)
{
    static unsigned int seconds = 0;
    static unsigned int milliseconds = 0;
    unsigned int value;
    unsigned int status;

    // Read the PIT status register
    status = PIT_GetStatus();

    // Check if the PIT counter has been incremented
    if ((status & AT91C_PITC_PITS) != 0) {

        // Acknowledge the interrupt by reading the PIVR register
        value = PIT_GetPIVR() >> 20;

        // Increment milliseconds counter
        milliseconds += value;

        // If the milliseconds counter crossed 500ms, update time
        if ((milliseconds >= 500) && ((milliseconds - value) < 500)) {

            UpdateTime(seconds, milliseconds);
            UpdatePixels(seconds, milliseconds);
        }
        // Increment seconds counter and update time
        else if (milliseconds >= 1000) {

            milliseconds -= 1000;
            seconds++;
            UpdateTime(seconds, milliseconds);
            UpdatePixels(seconds, milliseconds);
        }
    }
}

//------------------------------------------------------------------------------
/// Configure the periodic interval timer to trigger an interrupt each ms.
//------------------------------------------------------------------------------
static void ConfigurePit(void)
{
    // Initialize and enable the PIT
    PIT_Init(PIT_PERIOD, BOARD_MCK / 1000000);

    // Disable the interrupt on the interrupt controller
    IRQ_DisableIT(AT91C_ID_SYS);

    // Configure the AIC for PIT interrupts
    IRQ_ConfigureIT(AT91C_ID_SYS, 0, ISR_Pit);

    // Enable the interrupt on the interrupt controller
    IRQ_EnableIT(AT91C_ID_SYS);

    // Enable the interrupt on the pit
    PIT_EnableIT();

    // Enable the pit
    PIT_Enable();
}

//------------------------------------------------------------------------------
/// Initializes the SLCD controller with the board parameters.
//------------------------------------------------------------------------------
static void InitializeSlcd(void)
{
    // Enable SLCD 3V power supply
    SUPC_EnableSlcd(AT91C_SUPC_LCDMODE_INTERNAL);
    SUPC_SetSlcdVoltage(0x9);
    
    // Disable SLCD
    SLCDC_Disable();
    
    //  Define the number of COM and SEG, Buffer driving time; select the bias
    SLCDC_Configure(S7LEKLCD_NUM_COMMONS,
                    S7LEKLCD_NUM_SEGMENTS,
                    AT91C_SLCDC_BIAS_1_3,
                    AT91C_SLCDC_BUFTIME_2_Tsclk);
    
    // Set frame rate 30 Hz
    SLCDC_SetFrameFreq(AT91C_SLCDC_PRESC_SCLK_32, AT91C_SLCDC_DIV_1);
    
    //  Enable SLCD
    SLCDC_Enable();
}

//------------------------------------------------------------------------------
//         Global functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Main function
//------------------------------------------------------------------------------
int main(void)
{   
    TRACE_CONFIGURE(DBGU_STANDARD, 115200, BOARD_MCK);
    printf("-- Basic Segment LCD Project %s --\n\r", SOFTPACK_VERSION);
    printf("-- %s\n\r", BOARD_NAME);
    printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);

    // Initialize SLCD
    InitializeSlcd();
    SLCDC_Clear();

    // Switch on the Atmel logo and the clock
    S7LEKLCD_Symbol(S7LEKLCD_ATMEL, 1);
    S7LEKLCD_Symbol(S7LEKLCD_CLOCK, 1);

    // Display "Basic SLCD 1.4"
    S7LEKLCD_Char(S7LEKLCD_B, 0);
    S7LEKLCD_Char(S7LEKLCD_a, 1);
    S7LEKLCD_Char(S7LEKLCD_s, 2);
    S7LEKLCD_Char(S7LEKLCD_i, 3);
    S7LEKLCD_Char(S7LEKLCD_c, 4);
    S7LEKLCD_Char(S7LEKLCD_S, 5);
    S7LEKLCD_Char(S7LEKLCD_L, 6);
    S7LEKLCD_Char(S7LEKLCD_C, 7);
    S7LEKLCD_Char(S7LEKLCD_D, 8);
    S7LEKLCD_Char(S7LEKLCD_NONE, 9);
    S7LEKLCD_Char(S7LEKLCD_1 | S7LEKLCD_DOT, 10);
    S7LEKLCD_Char(S7LEKLCD_4, 11);

    // Configure the PIT to start displaying time
    ConfigurePit();

    // Infinite loop
    while (1);

    return 0;
}
