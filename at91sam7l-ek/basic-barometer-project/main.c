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
/// \dir "Basic Barometer Project"
///
/// !!!Purpose
///
/// This example shows how to use the on-board barometer sensor (MS5540B) 
/// to retrieve temperature and pressure information. 
///
///
/// !!!Description
///
/// The application connects to the external MS5540B barometer sensor through the SPI 
///    to fetch the current temperature and pressure. This information is then displayed on the DBGU. 
///
/// Since the usual point for pressure is taken at sea level, the measurement obtained from the sensor must be normalized. 
/// The algorithm to apply is described in the <a href="http://www.intersema.com/site/technical/files/an501.pdf"> following application note</a>
/// from the manufacturer (Intersema). 
///
/// The current altitude is fixed during compilation, but can be modified by changing the appropriate definition
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
/// -# The program will immediately start displaying measurements on the DBGU, as shown below: 
///    \code
///    -- Basic Barometer Project xxx --
///    -- AT91xxxxxx-xx
///    -- Compiled: xxx xx xxxx xx:xx:xx --
///    Current altitude is 205m
///    Temperature = 26.4 C | Pressure at sea level = 1026.9 mbar
///    \endcode
///
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/// \unit
///
/// !Purpose
///
/// This file contains all the specific code for the
/// basic-barometer-project
///
/// !Contents
///
/// The code can be roughly broken down as follows:
///    - Configure SPI
///    - Initialize MS5540B sensor
///    - Start a temperature and a pressure conversion
///
/// Please refer to the list of functions in the #Overview# tab of this unit
/// for more detailed information.
//-----------------------------------------------------------------------------

//------------------------------------------------------------------------------
//         Headers
//------------------------------------------------------------------------------

#include "pressure.h"
#include <board.h>
#include <pio/pio.h>
#include <dbgu/dbgu.h>
#include <spi/spi.h>
#include <sensors/ms5540b/ms5540b.h>
#include <utility/trace.h>

#include <stdio.h>

//------------------------------------------------------------------------------
//         Local definitions
//------------------------------------------------------------------------------

/// Current altitude in meters.
#define ALTITUDE        205

//------------------------------------------------------------------------------
//         Local variables
//------------------------------------------------------------------------------

/// Pio pins to configure.
static const Pin pins[] = {PINS_SPI, PIN_PCK0, PIN_SPI_NPCS3};

/// MS5540B DOUT signal pin.
static const Pin pinDout = PIN_MS5540B_DOUT;

/// SPI MISO pin.
static const Pin pinMiso = PIN_SPI_MISO;

//------------------------------------------------------------------------------
//         Local functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Enables the MS5540B master clock.
//------------------------------------------------------------------------------
static void EnableSensorClock()
{
    AT91C_BASE_PMC->PMC_SCER = AT91C_PMC_PCK0;
}

//------------------------------------------------------------------------------
/// Disables the MS5540B master clock.
//------------------------------------------------------------------------------
static void DisableSensorClock()
{
    AT91C_BASE_PMC->PMC_SCDR = AT91C_PMC_PCK0;
}

//------------------------------------------------------------------------------
/// Main function
//------------------------------------------------------------------------------
int main()
{
    unsigned short temperature, pressure;

    // Configure pins
    PIO_Configure(pins, PIO_LISTSIZE(pins));

    // Configure DBGU
    TRACE_CONFIGURE(DBGU_STANDARD, 115200, BOARD_MCK);
    printf("-- Basic Barometer Project %s --\n\r", SOFTPACK_VERSION);
    printf("-- %s\n\r", BOARD_NAME);
    printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);

    printf("Current altitude is %dm\n\r", ALTITUDE);

    // Configure SPI
    SPI_Configure(AT91C_BASE_SPI, AT91C_ID_SPI, AT91C_SPI_MSTR | SPI_PCS(3));
    SPI_Enable(AT91C_BASE_SPI);

    // Configure PCK0 signal to clock the MS5540B sensor
    AT91C_BASE_PMC->PMC_PCKR[0] = 0;

    // Initialize MS5540B sensor
    EnableSensorClock();
    MS5540B_Initialize(AT91C_BASE_SPI, 3);

    while (1) {
    
        // Start a temperature conversion
        MS5540B_AcquireTemperature();
        PIO_Configure(&pinDout, 1);
        while (PIO_Get(&pinDout));
        PIO_Configure(&pinMiso, 1);
        temperature = MS5540B_ConversionFinished();
    
        // Start a pressure conversion
        MS5540B_AcquirePressure();
        PIO_Configure(&pinDout, 1);
        while (PIO_Get(&pinDout));
        PIO_Configure(&pinMiso, 1);
        pressure = MS5540B_ConversionFinished();

        // Calculate pressure at 0 meters
        pressure = CalcPressureAt0Meters(ALTITUDE, pressure);

        printf("Temperature = %2d.%1d C | Pressure at sea level = %4d.%1d mbar\r",
               temperature / 10, temperature % 10,
               pressure / 10, pressure % 10);
    }

    return 0;
}

