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
/// \dir "Basic Parallel LCD project"
///
/// !!!Purpose
///
/// Demonstrates basic usage of the parallel LCD.
///
/// !!!Requirements
///
/// This package can be used with all Atmel evaluation kits that have 
/// parallel LCD interface.
///
/// !!!Description
///
/// Display red, green and blue color block on the LCD periodically.
///
/// !!!Usage
///
/// -# Build the program and download it inside the evaluation board. Please
///    refer to the <a href="http://www.atmel.com/dyn/resources/prod_documents/doc6224.pdf">SAM-BA User Guide</a>,
///    the <a href="http://www.atmel.com/dyn/resources/prod_documents/doc6310.pdf">GNU-Based Software Development</a>
///    application note or to the <a href="ftp://ftp.iar.se/WWWfiles/arm/Guides/EWARM_UserGuide.ENU.pdf">IAR EWARM User Guide</a>,
///    depending on your chosen solution.
/// -# On the computer, open and configure a terminal application
///    (e.g. HyperTerminal on Microsoft Windows) with these settings:
///   - 115200 bauds
///   - 8 bits of data
///   - No parity
///   - 1 stop bit
///   - No flow control
/// -# Display red, green and blue color block on the LCD periodically.
///    In addition, the following message should appear on the DBGU:
///     \code
///     -- Basic Parallel LCD Project xxx --
///     -- AT91xxxxxx-xx
///     -- Compiled: xxx xx xxxx xx:xx:xx --
///     \endcode
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// \unit
///
/// !Purpose
///
/// This file contains all the specific code for the basic-parallel-lcd-project.
///
/// !Contents
/// The code can be roughly broken down as follows:
///    - Interrupt handlers
///    - Utility functions
///    - The main() function, which implements the program behavior
///
/// Please refer to the list of functions in the #Overview# tab of this unit
/// for more detailed information.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//         Headers
//------------------------------------------------------------------------------

#include <board.h>
#include <stdio.h>
#include <utility/trace.h>
#include <pio/pio.h>
#include <irq/irq.h>
#include <lcd/color.h>
#include <lcd/lcdd.h>
#include <lcd/draw.h>
#if defined(cortexm3)
#include <systick/systick.h>
#endif

//------------------------------------------------------------------------------
//         Local definitions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//         Local variables
//------------------------------------------------------------------------------
/// Global timestamp in milliseconds since start of application.
volatile unsigned int timestamp = 0;

//------------------------------------------------------------------------------
//         Local functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Handler for SysTick interrupt. Increments the timestamp counter.
//------------------------------------------------------------------------------
void SysTick_Handler(void)
{
    timestamp++;
}

//------------------------------------------------------------------------------
//         Exported functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Delay millisecond. Use SysTick interrupt for function implement, so ensure
/// the SysTick interrupt is enabled before using this function.
/// \param ms   millisecond to be delay
//------------------------------------------------------------------------------
void DelayMS(unsigned int ms)
{
    unsigned int st = timestamp;

    while (timestamp - st < ms);
}

//------------------------------------------------------------------------------
/// Application entry point.
/// \return Unused (ANSI-C compatibility).
//------------------------------------------------------------------------------
int main(void)
{
    void *pLcdBase = (void *)BOARD_LCD_BASE;

    // DBGU configuration
    TRACE_CONFIGURE(DBGU_STANDARD, 115200, BOARD_MCK);
    printf("-- Basic Parallel LCD Project %s --\n\r", SOFTPACK_VERSION);
    printf("-- %s\n\r", BOARD_NAME);
    printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);

    // 1ms tick
    SysTick_Configure(1, BOARD_MCK/1000, SysTick_Handler);

    // Initialize LCD
    LCDD_Initialize();
    LCDD_Fill((void *)BOARD_LCD_BASE, COLOR_WHITE);
    LCDD_Start();

    // Main loop
    while (1) {

        LCDD_DrawString(pLcdBase, 1, 20, "Display red color:", COLOR_BLACK);
        LCDD_DrawRectangle(pLcdBase, 20, 50, 80, 20, COLOR_RED);
        DelayMS(1000);

        LCDD_DrawString(pLcdBase, 1, 100, "Display green color:", COLOR_BLACK);
        LCDD_DrawRectangle(pLcdBase, 20, 130, 80, 20, COLOR_GREEN);
        DelayMS(1000);

        LCDD_DrawString(pLcdBase, 1, 180, "Display blue color:", COLOR_BLACK);
        LCDD_DrawRectangle(pLcdBase, 20, 210, 80, 20, COLOR_BLUE);
        DelayMS(2000);

        LCDD_Fill((void *)BOARD_LCD_BASE, COLOR_WHITE);
    }
}

