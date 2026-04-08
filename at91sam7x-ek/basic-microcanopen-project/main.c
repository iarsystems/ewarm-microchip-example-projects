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
/// \dir "Basic MicroCANOpen project"
///
/// !!!Purpose
/// This demo is based on Open Source library MicrocanOpen.
/// MicroCANopen is a minimal CANopen implementation targeted at embedded
/// networking applications. 
/// The MicroCANopen protocol stack implements all mandatory functionality of 
/// the CiA (CAN in Automation user’s and manufacturer’s group).
///
/// !!!See
/// <a href="http://www.microcanopen.com">MicrocanOpen</a>
/// <a href="../../../external_libs/microcanopen/Doc/MicroCANopen Manual Edu.pdf"> MICROCANOPEN USER MANUAL </a>
///
/// !!!Requirements
///
/// This package can be used with all Atmel evaluation kits that have CAN
/// interface
///
/// !!!Description
///
/// Open HyperTerminal before running this program, use SAM-BA to download this program to 
/// SRAM or SDRAM, make the program run, the HyperTerminal will give out the test results.
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
/// -# Upon startup, the application will output the following lines on the DBGU.
///     \code
///     -- Basic Microcanopen Project xxx --
///     -- AT91xxxxxx-xx
///     -- Compiled: xxx xx xxxx xx:xx:xx --
///     \endcode
///
//------------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//         Headers
//-----------------------------------------------------------------------------
#include <board.h>
#include <pio/pio.h>
#include <pio/pio_it.h>
#include <irq/irq.h>
#include <dbgu/dbgu.h>
#include <usart/usart.h>
#include <string.h>
#include <dbgu/dbgu.h>
#include <utility/trace.h>
#include <utility/assert.h>
#include <can/can.h>

#include "mco.h"
#include "r_atmel_io.h"
#include "procimg.h"

// external declaration for the process image array
extern UNSIGNED8 gProcImg[];

//-----------------------------------------------------------------------------
/// main function
//-----------------------------------------------------------------------------
int main(void)
{
    PIO_InitializeInterrupts(0);
    TRACE_CONFIGURE(DBGU_STANDARD, 115200, BOARD_MCK);
    printf("-- Basic Microcanopen Project %s --\n\r", SOFTPACK_VERSION);
    printf("-- %s\n\r", BOARD_NAME);
    printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);

    // Reset/Initialize CANopen communication
    MCOUSER_ResetCommunication();

    // foreground loop
    for(;;) {
        // Update process data
        // First digital inputs are real I/O
        gProcImg[IN_digi_1] = read_dip_switches();

        // output first digital outputs to LEDs
        switch_leds(gProcImg[OUT_digi_1]);

        // echo all other I/O values from input to output
        // digital
        gProcImg[IN_digi_2] = gProcImg[OUT_digi_2];
        gProcImg[IN_digi_3] = gProcImg[OUT_digi_3];
        gProcImg[IN_digi_4] = gProcImg[OUT_digi_4];

        // analog
        gProcImg[IN_ana_1]   = gProcImg[OUT_ana_1];
        gProcImg[IN_ana_1+1] = gProcImg[OUT_ana_1+1];
        gProcImg[IN_ana_2]   = gProcImg[OUT_ana_2];
        gProcImg[IN_ana_2+1] = gProcImg[OUT_ana_2+1];

        // Operate on CANopen protocol stack
        MCO_ProcessStack();
    } // end of for

    return 0;
} // end of main


