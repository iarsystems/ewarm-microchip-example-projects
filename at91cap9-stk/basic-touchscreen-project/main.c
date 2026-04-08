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
//         Headers
//------------------------------------------------------------------------------

#include <board.h>
#include <board_memories.h>
#include <pio/pio.h>
#include <dbgu/dbgu.h>
#include <tsd/tsd.h>
#include <lcd/lcdd.h>
#include <lcd/draw.h>
#include <lcd/color.h>
#include <stdio.h>
#include <string.h>
#include <utility/trace.h>
#include <irq/irq.h>
#if defined(cortexm3)
#include <systick/systick.h>
#endif

//------------------------------------------------------------------------------
/// \dir "Basic-touchscreen-project"
///
/// !!!Purpose
///
/// The Basic-touchscreen-project will help you to get familiar with touchscreen
/// controller on AT91SAM microcontrollers. It can also help you to get familiar
/// with the touchscreen configurations and usage, which can be used for fast
/// implementation of your own touchscreen driver and other applications related
///
/// You can find following information depends on your needs:
/// - Touchscreen controller configurations
/// - PIT module configurations for SAM7/SAM9 or System tick configurations fo SAM3
/// - LCD initialize sequence
/// - Sample code of drawing LCD
/// - Sample code of calibrating Touchscreen
/// - The method to sample data from touchscreen interface
/// - Sample code to calculate slopes
///
/// !See
/// - lcd : lcd peripheral driver.
/// - tsadcc : touchscreen controller driver for AT91SAM peripheral
/// - ads7843 : touchscreen controller driver for ADS7843
///
/// !!!Requirements
///
/// This package can be used with all Atmel evaluation kits that have
/// touchscreen interface.
///
/// !!!Description
///
/// Open HyperTerminal before running this program, use SAM-BA to download this
/// program to the evaluation board, make the program run, the HyperTerminal 
/// will give out the test results.
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
/// -# Start the application
/// -# In HyperTerminal, it will show something like
///     \code
///     -- Basic Touchscreen Project xxx --
///     -- AT91xxxxxx-xx
///     -- Compiled: xxx xx xxxx xx:xx:xx --
///     -I- Calibration successful !
///     \endcode
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// \unit
///
/// !Purpose
///
/// This file contains all the specific code for the basic-touchscreen-project
///
/// !Contents
/// The basic-touchscreen-project application can be roughly broken down as 
/// follows:
///    - SysTick handlers and Delay function for SAM3
///       - SysTick_Handler
///       - DelayMS
///    - Callback functions for touchsreeen
///       - TSD_PenPressed
///       - TSD_PenMoved
///       - TSD_PenReleased
///    - The main function, which implements the program behavior
///       - I/O and DBGU configuration
///       - Configure SysTick for SAM3
///       - Initialize the LCD
///       - Initialize the touchscreen
///       - Calibration with touchscreen input
///
/// Please refer to the list of functions in the #Overview# tab of this unit
/// for more detailed information.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//         Local definition
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//         Local types
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//         Local variables
//------------------------------------------------------------------------------

 /// LCD buffer.
#ifdef BOARD_LCD_HX8347
static unsigned char *pLcdBuffer = (unsigned char *) (BOARD_LCD_BASE);
#elif defined(at91sam9g45ek)
static unsigned char *pLcdBuffer = (unsigned char *) (AT91C_DDR2 + 0x00100000);
#else
static unsigned char *pLcdBuffer = (unsigned char *) (AT91C_EBI_SDRAM + 0x00100000);
#endif

#if defined(cortexm3)
/// Global timestamp in milliseconds since start of application.
volatile unsigned int timestamp = 0;
#endif

//------------------------------------------------------------------------------
//         Global functions
//------------------------------------------------------------------------------
#if defined(cortexm3)
//------------------------------------------------------------------------------
/// Handler for SysTick interrupt. Increments the timestamp counter.
//------------------------------------------------------------------------------
void SysTick_Handler(void)
{
    timestamp++;

    // Call TSD_TimerHandler per 10ms
    if ((timestamp % 10) == 0) {
        
        TSD_TimerHandler();
    }
}

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
#endif

//------------------------------------------------------------------------------
/// Callback called when the pen is pressed on the touchscreen
/// \param x horizontal position (in pixel if ts calibrated)
/// \param y vertical position (in pixel if ts calibrated)
//------------------------------------------------------------------------------
void TSD_PenPressed(unsigned int x, unsigned int y)
{
    printf("Pen pressed at  (%03u, %03u)\n\r", x, y);
}

//------------------------------------------------------------------------------
/// Callback called when the pen is moved on the touchscreen
/// \param x horizontal position (in pixel if ts calibrated)
/// \param y vertical position (in pixel if ts calibrated)
//------------------------------------------------------------------------------
void TSD_PenMoved(unsigned int x, unsigned int y)
{
    printf("Pen moved at    (%03u, %03u)\n\r", x, y);
}

//------------------------------------------------------------------------------
/// Callback called when the touchscreen is released on the touchscreen
/// \param x horizontal position (in pixel if ts calibrated)
/// \param y vertical position (in pixel if ts calibrated)
//------------------------------------------------------------------------------
void TSD_PenReleased(unsigned int x, unsigned int y)
{
    printf("Pen released at (%03u, %03u)\n\r", x, y);
}

//------------------------------------------------------------------------------
/// Initializes the touchscreen and outputs measurements on the DBGU.
//------------------------------------------------------------------------------
int main(void)
{
    unsigned int bResult;

    TRACE_CONFIGURE(DBGU_STANDARD, 115200, BOARD_MCK);
    printf("-- Basic Touchscreen Project %s --\n\r", SOFTPACK_VERSION);
    printf("-- %s\n\r", BOARD_NAME);
    printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);

#if (!defined(sdram) && !defined(cortexm3))
    BOARD_ConfigureSdram(BOARD_SDRAM_BUSWIDTH);
#endif

#if (!defined(ddram) && defined(AT91C_DDR2))
    BOARD_ConfigureDdram(DDR_MICRON_MT47H64M8, BOARD_DDRAM_BUSWIDTH);
#endif

#if defined(cortexm3)
    // Configuration 1ms tick
    SysTick_Configure(1, BOARD_MCK/1000, SysTick_Handler);
#endif

    // Initialize LCD
    LCDD_Initialize();
    LCDD_Fill(pLcdBuffer, COLOR_WHITE);
#ifdef BOARD_LCD_HX8347   
    LCDD_Start();
#else    // peripheral LCDC
    LCDD_DisplayBuffer(pLcdBuffer);
#endif

    // Initialize touchscreen without calibration
    TSD_Initialize(0);

    while(1) {
        bResult = TSD_Calibrate(pLcdBuffer);
        if(bResult) {
            printf("-I- Calibration successful !\n\r");
            break;
        }
        else {
            printf("-E- Error too big ! Retry...\n\r");
        }
    }

    // Infinite loop
    while (1);
}

