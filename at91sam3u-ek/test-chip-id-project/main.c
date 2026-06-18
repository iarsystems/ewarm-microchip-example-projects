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
/// \dir "Test Chip ID with AT91SAM Microcontrollers"
///
/// !!!Purpose
///
/// The Test-chip-id project will help new users get familiar with how to read
/// Atmel's AT91 family of microcontrollers' Id. 
///
/// !!!Requirements
///
/// This package can be used with all Atmel evaluation kits.
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
/// -# Start the application.
/// -# In the terminal window, the following text should appear 
///     (values depend on the board and chip used):
///    \code
///     -- Test Chip ID Project xxx --
///     -- AT91xxxxxx-xx
///     -- Compiled: xxx xx xxxx xx:xx:xx --
///    \endcode
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// \unit
///
/// !Purpose
///
/// This file contains all the specific code for the test-chip-id-project.
///
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//         Headers
//------------------------------------------------------------------------------

#include <board.h>
#include <utility/trace.h>
#include <stdio.h>
#include <chipid/chipid.h>

//------------------------------------------------------------------------------
//         Local definitions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//         Local variables
//------------------------------------------------------------------------------
ChipId gChipId;

//------------------------------------------------------------------------------
//         Local functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//         Exported functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Application entry point. 
/// \return Unused (ANSI-C compatibility).
//------------------------------------------------------------------------------
int main(void)
{
    // DBGU output configuration
    TRACE_CONFIGURE(DBGU_STANDARD, 115200, BOARD_MCK);
    printf("-- Test Chip ID Project %s --\n\r", SOFTPACK_VERSION);
    printf("-- %s\n\r", BOARD_NAME);
    printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);

    CHIPID_Get(&gChipId);
    CHIPID_Print(&gChipId);

    return 0;
}

