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
/// \dir "TWI Slave project"
///
/// !!!Purpose
///
/// This project demonstrates the TWI peripheral in slave mode. It mimics the
/// behavior of a serial memory, enabling the TWI master to read and write
/// data in its internal SRAM.
///
/// !!!Requirements
///
/// In addition, another device will be needed to act as the TWI master. The
/// basic-twi-eeprom-project can be used for that, in which case a second kit
/// supported by that project is needed.
/// Note: For AT91SAM9261-EK, the TWI slave is available on AT91SAM9261S chip.
///
/// !!!Description
///
/// After launching the program, the device will act as a simple TWI-enabled
/// serial memory containing 512 bytes. This enables this project to be used
/// with the basic-twi-eeprom-project as the master.
///
/// To write in the memory, the TWI master must address the device first, then
/// send two bytes containing the memory address to access. Additional bytes are
/// treated as the data to write.
///
/// Reading is done in the same fashion, except that after receiving the memory
/// address, the device will start outputting data until a STOP condition is
/// sent by the master.
///
/// !!!Usage
///
/// -# The default address for the TWI slave is fixed to Ox50.
///    If the board has a TWI component with this adress, you can change
///    the define AT24C_ADDRESS in main.c of twi-eeprom project, and the define 
///    SLAVE_ADDRESS in main.c of twi-slave project.
///
/// -# For AT91CAP9-DK board:
/// -# Connect TWD (SDA) for the 2 boards: pin 6 of connector J18
/// -# Connect TWCK(SCL) for the 2 boards: pin 5 of connector J18
/// -# Connect GND for the 2 boards: pin 34 of connector J18
/// -# Add a pull up of 2,2KOhms on TWD and TWCK (pin 36 of J18 is 3,3V)
///
/// -# For AT91SAM7L-EK board:
/// -# Connect TWD (SDA) for the 2 boards: pin 12 of connector PIOC
/// -# Connect TWCK(SCL) for the 2 boards: pin 11 of connector PIOC
/// -# Connect GND for the 2 boards: pin 34 of connector PIOC
/// -# Add a pull up of 2,2KOhms on TWD and TWCK (pin 36 of PIOC is 3,3V)
///
/// -# For AT91SAM7SE-EK board:
/// -# Connect TWD (SDA) for the 2 boards: pin 3 of connector J15
/// -# Connect TWCK(SCL) for the 2 boards: pin 6 of connector J15
/// -# Connect GND for the 2 boards: pin 38 of connector J15
/// -# Add a pull up of 2,2KOhms on TWD and TWCK (pin 36 of J15 is 3,3V)
///
/// -# For AT91SAM9260-EK board:
/// -# Connect TWD (SDA) for the 2 boards: pin 23 of connector J23
/// -# Connect TWCK(SCL) for the 2 boards: pin 26 of connector J23
/// -# Connect GND for the 2 boards: pin 36 of connector J23
/// -# Add a pull up of 2,2KOhms on TWD and TWCK (pin 34 of J23 is 3,3V)
///
/// -# For AT91SAM9G20-EK board:
/// -# Connect TWD (SDA) for the 2 boards: pin 23 of connector J23
/// -# Connect TWCK(SCL) for the 2 boards: pin 26 of connector J23
/// -# Connect GND for the 2 boards: pin 36 of connector J23
/// -# Add a pull up of 2,2KOhms on TWD and TWCK (pin 34 of J23 is 3,3V)
///
/// -# For AT91SAM9M10-EK board:
/// -# No pin accessible, user should solder wires.
///
/// -# For AT91SAM9RL-EK board:
/// -# Connect TWD (SDA) for the 2 boards: pin 23 of connector J23
/// -# Connect TWCK(SCL) for the 2 boards: pin 26 of connector J23
/// -# Connect GND for the 2 boards: pin 36 of connector J23
/// -# Add a pull up of 2,2KOhms on TWD and TWCK (pin 34 of J23 is 3,3V)
///
/// -# For AT91SAM9XE-EK board:
/// -# Connect TWD (SDA) for the 2 boards: pin 23 of connector J23
/// -# Connect TWCK(SCL) for the 2 boards: pin 26 of connector J23
/// -# Connect GND for the 2 boards: pin 36 of connector J23
/// -# Add a pull up of 2,2KOhms on TWD and TWCK (pin 34 of J23 is 3,3V)
///
/// -# For the TWI Slave board:
/// -# Compile the application. 
/// -# Connect the DBGU port of the evaluation board to the computer and open 
/// it in a terminal.
///    - Settings: 115200 bauds, 8 bits, 1 stop bit, no parity, no flow control. 
/// -# Download the program inside the evaluation board and run it. Please refer to
///    <a href="http://www.atmel.com/dyn/resources/prod_documents/doc6132.pdf">
///    the SAM-BA User Guide, 
///    <a href="http://www.atmel.com/dyn/resources/prod_documents/doc6310.pdf">
///    the GNU-Based Software Development</a> application note or to the 
///    <a href="http://www.iar.se/website1/1.0.1.0/78/1/index.php?">
///    IAR EWARM User Guide</a>, 
///    depending on your chosen solution. 
/// -# Upon startup, the application will output the following line on the DBGU: 
///    \code
///     -- Basic TWI Slave Project xxx --
///     -- AT91xxxxxx-xx
///     -- Compiled: xxx xx xxxx xx:xx:xx --
///     -I- Configuring the TWI in slave mode
///    \endcode
/// -# For the TWI Master board, see the description inside his project
/// -# and the "Master" board will output:
///    \code
///     -- Basic TWI EEPROM Project xxx --
///     -- AT91xxxxxx-xx
///     -- Compiled: xxx xx xxxx xx:xx:xx --
///     -I- Filling page #0 with zeroes ...
///     -I- Filling page #1 with zeroes ...
///     -I- Read/write on page #0 (polling mode)
///     -I- 0 comparison error(s) found
///     -I- Read/write on page #1 (IRQ mode)
///     -I- Callback fired !
///     -I- Callback fired !
///     -I- 0 comparison error(s) found
///    \endcode
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//         Headers
//------------------------------------------------------------------------------
#include <board.h>
#include <pio/pio.h>
#include <dbgu/dbgu.h>
#include <utility/trace.h>
#include <utility/assert.h>
#include <pmc/pmc.h>
#include <irq/irq.h>
#include <twi/twi.h>
#include <stdio.h>

//------------------------------------------------------------------------------
//         Local constants
//------------------------------------------------------------------------------
/// Slave address of the device on the TWI bus.
#define SLAVE_ADDRESS       0x50
/// Memory size in bytes (example AT24C1024)
#define MEMORY_SIZE         512
/// Page size in bytes (example AT24C1024)
#define PAGE_SIZE           256

/// Memory size in bytes (example AT24C512)
//#define MEMORY_SIZE         512
/// Page size in bytes (example AT24C512)
//#define PAGE_SIZE           128

#ifndef AT91C_BASE_TWI
#define AT91C_BASE_TWI AT91C_BASE_TWI0
#define PINS_TWI       PINS_TWI0
#define AT91C_ID_TWI   AT91C_ID_TWI0
#endif

//------------------------------------------------------------------------------
//         Local variables
//------------------------------------------------------------------------------
/// Pio pins to configure.
const Pin pins[] = {PINS_DBGU, PINS_TWI};

/// The slave device instance
typedef struct {

    /// PageAddress of the slave device
    unsigned short pageAddress;
    /// Offset of the memory access
    unsigned short offsetMemory;
    //unsigned int status;
    /// Read address of the request
    unsigned char acquireAddress;
    /// Memory buffer
    unsigned char pMemory[MEMORY_SIZE];

} SlaveDeviceDriver;

SlaveDeviceDriver EmulateDriver;

//------------------------------------------------------------------------------
//         Global functions
//------------------------------------------------------------------------------
void TWI0_IrqHandler(void)
{
    unsigned int status;

    status = TWI_GetStatus(AT91C_BASE_TWI);

    if( ((status & AT91C_TWI_SVACC) == AT91C_TWI_SVACC)
     && (EmulateDriver.acquireAddress == 0) ) {
        TWI_DisableIt(AT91C_BASE_TWI, AT91C_TWI_SVACC);
        TWI_EnableIt(AT91C_BASE_TWI, AT91C_TWI_RXRDY
                                   | AT91C_TWI_GACC
                                   | AT91C_TWI_NACK_SLAVE 
                                   | AT91C_TWI_EOSACC 
                                   | AT91C_TWI_SCLWS );
        EmulateDriver.acquireAddress++;
        EmulateDriver.pageAddress = 0;
        EmulateDriver.offsetMemory = 0;
    }

    if( (status & AT91C_TWI_GACC) == AT91C_TWI_GACC ) {
        printf("General Call Treatment\n\r");
        printf("not treated");
    }

    if( ((status & AT91C_TWI_SVACC) == AT91C_TWI_SVACC )
       && ((status & AT91C_TWI_GACC) == 0 )
       && ((status & AT91C_TWI_RXRDY) == AT91C_TWI_RXRDY ) ){

        if( EmulateDriver.acquireAddress == 1 ) {
            // Acquire LSB address
            EmulateDriver.pageAddress = (TWI_ReadByte(AT91C_BASE_TWI) & 0xFF);
            EmulateDriver.acquireAddress++;
        }
        else if( EmulateDriver.acquireAddress == 2 ) {
            // Acquire MSB address
            EmulateDriver.pageAddress |= (TWI_ReadByte(AT91C_BASE_TWI) & 0xFF)<<8;
            EmulateDriver.acquireAddress++;
        }
        else {
            // Read one byte of data from master to slave device
            EmulateDriver.pMemory[(PAGE_SIZE*EmulateDriver.pageAddress)+EmulateDriver.offsetMemory] = (TWI_ReadByte(AT91C_BASE_TWI) & 0xFF);
            EmulateDriver.offsetMemory++;
        }
    }
    else if( ((status & AT91C_TWI_TXRDY_SLAVE) == AT91C_TWI_TXRDY_SLAVE )
          && ((status & AT91C_TWI_TXCOMP_SLAVE) == AT91C_TWI_TXCOMP_SLAVE )
          && ((status & AT91C_TWI_EOSACC) == AT91C_TWI_EOSACC ) ) {
        // End of transfert, end of slave access
        EmulateDriver.offsetMemory = 0;
        EmulateDriver.acquireAddress = 0;
        EmulateDriver.pageAddress = 0;
        TWI_EnableIt(AT91C_BASE_TWI, AT91C_TWI_SVACC);
        TWI_DisableIt(AT91C_BASE_TWI, AT91C_TWI_RXRDY
                                    | AT91C_TWI_GACC
                                    | AT91C_TWI_NACK_SLAVE 
                                    | AT91C_TWI_EOSACC 
                                    | AT91C_TWI_SCLWS );
    }
    else if( ((status & AT91C_TWI_SVACC) == AT91C_TWI_SVACC )
       && ((status & AT91C_TWI_GACC) == 0 )
       && ( EmulateDriver.acquireAddress == 3 )
       && ((status & AT91C_TWI_SVREAD) == AT91C_TWI_SVREAD )
       && ((status & AT91C_TWI_NACK_SLAVE) == 0 ) ) {
        // Write one byte of data from slave to master device
        TWI_WriteByte(AT91C_BASE_TWI, EmulateDriver.pMemory[(PAGE_SIZE*EmulateDriver.pageAddress)+EmulateDriver.offsetMemory]);
        EmulateDriver.offsetMemory++;
    }
}

//------------------------------------------------------------------------------
/// Default main() function. Initializes the DBGU and writes a string on the
/// DBGU.
//------------------------------------------------------------------------------
int main(void)
{
    unsigned int i;

    PIO_Configure(pins, PIO_LISTSIZE(pins));
    DBGU_Configure(DBGU_STANDARD, 115200, BOARD_MCK);
    printf("-- Basic TWI Slave Project %s --\n\r", SOFTPACK_VERSION);
    printf("-- %s\n\r", BOARD_NAME);
    printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);

    PMC_EnablePeripheral(AT91C_ID_TWI);

    for (i=0; i<MEMORY_SIZE; i++) {
        EmulateDriver.pMemory[i] = 0;
    }
    EmulateDriver.offsetMemory = 0;
    EmulateDriver.acquireAddress = 0;
    EmulateDriver.pageAddress = 0;

    // Configure TWI as slave
    printf("-I- Configuring the TWI in slave mode\n\r");
    TWI_ConfigureSlave(AT91C_BASE_TWI, SLAVE_ADDRESS);

    // Clear receipt buffer
    TWI_ReadByte(AT91C_BASE_TWI);

    TRACE_DEBUG("TWI is in slave mode\n\r");

    IRQ_ConfigureIT(AT91C_ID_TWI, 0, TWI0_IrqHandler);
    IRQ_EnableIT(AT91C_ID_TWI);

    TWI_EnableIt(AT91C_BASE_TWI, AT91C_TWI_SVACC );

    while (1) {
    }
}


