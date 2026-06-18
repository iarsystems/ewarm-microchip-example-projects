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
#include <utility/trace.h>
#include <utility/math.h>
#include <memories/nandflash/SkipBlockNandFlash.h>

#include <string.h>

//------------------------------------------------------------------------------
/// \dir "Basic-nandflash-project"
///
/// !!!Purpose
///
/// The Basic-nandflash-project will help you to get familiar with nandflash interface on AT91SAM 
/// microcontrollers. It can also help you to get familiar with the nandflash operation flow
/// which can be used for fast implementation of your own nandflash drivers and other 
/// applications related.
///
/// You can find following information depends on your needs:
/// - Nandflash Initialize, this include a find model procedure, the program will try to find 
///    the device model with the readout Nandflash id.
/// - Basic operations of Nandflash, such as erase blocks, write blocks, read blocks.
/// - Get Nandflash parameters after find the Nandflash model. User can refer to NandflashModel
///    for those parameters.
/// 
/// !See
/// - nandflash : nandflash drivers
///
/// !!!Requirements
///
/// This package can be used with all Atmel evaluation kits that have EBI interface, the 
/// package runs at SRAM or SDRAM, so EBI interface and SDRAM device is needed if you
/// want to run this package in SDRAM
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
/// -# In HyperTerminal, it will show something like
///     \code
///     -- Basic NandFlash Project xxx --
///     -- AT91xxxxxx-xx
///     -- Compiled: xxx xx xxxx xx:xx:xx --
///     Nandflash ID is 0x1580DA2C
///     -I-     Nandflash driver initialized
///     -I- Size of the whole device in bytes : 0x10000000
///     -I- Size in bytes of one single block of a device : 0x20000
///     -I- Number of blocks in the entire device : 0x800
///     -I- Size of the data area of a page in bytes : 0x800
///     -I- Number of pages in the entire device : 0x40
///     -I- Bus width : 0x8
///     -I- Test in progress on block:     22
///     \endcode
///
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// \unit
///
/// !Purpose
///
/// This file contains all the specific code for the basic-nandflash-project
///
/// !Contents
/// The basic-nandflash-project application can be roughly broken down as follows:
///    - Configure Nandflash working frequency.
///    - I/O configuration.
///    - Nandflash initialize.
///    - Get Nandflash parameters.
///    - Erase Nandflash blocks.
///    - Write Nandflash blocks.
///    - Read Nandflash blocks.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//         Local definitions
//------------------------------------------------------------------------------

/// Effective master clock for applets.
#define MCK     48000000
#if defined(at91sam9g45ek) || defined(at91sam9m10ek)
#define BUFFER_ADDRESS (AT91C_DDR2 + 0x00100000)
#elif defined(at91sam3uek)
#define BUFFER_ADDRESS (BOARD_EBI_PSRAM)
#else
#define BUFFER_ADDRESS (AT91C_EBI_SDRAM + 0x00100000)
#endif

//------------------------------------------------------------------------------
//         Local variables
//------------------------------------------------------------------------------

/// Nandflash memory size.
static unsigned int memSize;
/// Number of blocks in nandflash.
static unsigned int numBlocks;
/// Size of one block in the nandflash, in bytes.
static unsigned int blockSize;
/// Size of one page in the nandflash, in bytes.
static unsigned int pageSize;
/// Number of page per block
static unsigned int numPagesPerBlock;
// Nandflash bus width
static unsigned char nfBusWidth = 16;


#ifdef PINS_NANDFLASH

/// Pins used to access to nandflash.
static const Pin pPinsNf[] = {PINS_NANDFLASH};
/// Nandflash device structure.
static struct SkipBlockNandFlash skipBlockNf;
/// Address for transferring command bytes to the nandflash.
static unsigned int cmdBytesAddr = BOARD_NF_COMMAND_ADDR;
/// Address for transferring address bytes to the nandflash.
static unsigned int addrBytesAddr = BOARD_NF_ADDRESS_ADDR;
/// Address for transferring data bytes to the nandflash.
static unsigned int dataBytesAddr = BOARD_NF_DATA_ADDR;
/// Nandflash chip enable pin.
static const Pin nfCePin = BOARD_NF_CE_PIN;
/// Nandflash ready/busy pin.
static const Pin nfRbPin = BOARD_NF_RB_PIN;


#else

/// Pins used to access to nandflash.
static const Pin pPinsNf[] = {{0, 0, 0, 0, 0}};
/// Nandflash device structure.
static struct SkipBlockNandFlash skipBlockNf;
/// Address for transferring command bytes to the nandflash.
static unsigned int cmdBytesAddr = 0;
/// Address for transferring address bytes to the nandflash.
static unsigned int addrBytesAddr = 0;
/// Address for transferring data bytes to the nandflash.
static unsigned int dataBytesAddr = 0;
/// Nandflash chip enable pin.
static const Pin nfCePin = {0, 0, 0, 0, 0};
/// Nandflash ready/busy pin.
static const Pin nfRbPin = {0, 0, 0, 0, 0};

#endif


//------------------------------------------------------------------------------
//         Global functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Applet main entry. This function decodes received command and executes it.
/// \param argc  always 1
/// \param argv  Address of the argument area.
//------------------------------------------------------------------------------
int main()
{
    unsigned char testFailed;
    // Temporary buffer used for non block aligned read / write
    unsigned char * pBuffer;
    unsigned short block;
    unsigned int i;
    // Errors returned by SkipNandFlash functions
    unsigned char error = 0;

    // Configure the DBGU
    TRACE_CONFIGURE(DBGU_STANDARD, 115200, BOARD_MCK);
    printf("-- Basic NandFlash Project %s --\n\r", SOFTPACK_VERSION);
    printf("-- %s\n\r", BOARD_NAME);
    printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);

#if !defined(ddram) && defined(BOARD_DDRAM_SIZE)
    BOARD_ConfigureDdram(0, BOARD_DDRAM_BUSWIDTH);
#endif

#if !defined(sdram) && defined(BOARD_SDRAM_SIZE)
    BOARD_ConfigureSdram(BOARD_SDRAM_BUSWIDTH);
#endif

#if !defined(psram) && defined(BOARD_PSRAM_SIZE)
    BOARD_ConfigurePsram();
#endif

    // Configure SMC for Nandflash accesses (done each time applet is launched because of old ROM codes)
    BOARD_ConfigureNandFlash(nfBusWidth);
    PIO_Configure(pPinsNf, PIO_LISTSIZE(pPinsNf));

    memset(&skipBlockNf, 0, sizeof(skipBlockNf));

    if (SkipBlockNandFlash_Initialize(&skipBlockNf,
                                              0,
                                              cmdBytesAddr,
                                              addrBytesAddr,
                                              dataBytesAddr,
                                              nfCePin,
                                              nfRbPin)) {

        TRACE_ERROR("\tDevice Unknown\n\r");
        return 0;
    }

    // Check the data bus width of the NandFlash
    nfBusWidth = NandFlashModel_GetDataBusWidth((struct NandFlashModel *)&skipBlockNf);
    // Reconfigure bus width
    BOARD_ConfigureNandFlash(nfBusWidth);

    TRACE_INFO("\tNandflash driver initialized\n\r");

    // Get device parameters
    memSize = NandFlashModel_GetDeviceSizeInBytes(&skipBlockNf.ecc.raw.model);
    blockSize = NandFlashModel_GetBlockSizeInBytes(&skipBlockNf.ecc.raw.model);
    numBlocks = NandFlashModel_GetDeviceSizeInBlocks(&skipBlockNf.ecc.raw.model);
    pageSize = NandFlashModel_GetPageDataSize(&skipBlockNf.ecc.raw.model);
    numPagesPerBlock = NandFlashModel_GetBlockSizeInPages(&skipBlockNf.ecc.raw.model);

    TRACE_INFO("Size of the whole device in bytes : 0x%x \n\r",memSize);
    TRACE_INFO("Size in bytes of one single block of a device : 0x%x \n\r",blockSize);
    TRACE_INFO("Number of blocks in the entire device : 0x%x \n\r",numBlocks);
    TRACE_INFO("Size of the data area of a page in bytes : 0x%x \n\r",pageSize);
    TRACE_INFO("Number of pages in the entire device : 0x%x \n\r",numPagesPerBlock);
    TRACE_INFO("Bus width : 0x%x \n\r",nfBusWidth);

    // Test all blocks
    testFailed = 0;
    block = 0;
    pBuffer = (unsigned char *) BUFFER_ADDRESS;

    while (!testFailed && (block < numBlocks)) {
        TRACE_INFO("Test in progress on block: %6d\r", block);

        // Erase block
        error = SkipBlockNandFlash_EraseBlock(&skipBlockNf, block, NORMAL_ERASE);
        if (error == NandCommon_ERROR_BADBLOCK) {
            TRACE_INFO("Skip bad block %6d: \n\r", block);
            block++;
            continue;
        }

        // Verify that block has been erased correctly
        memset(pBuffer, 0, blockSize);
        SkipBlockNandFlash_ReadBlock(&skipBlockNf, block, pBuffer);
        for (i=0; i < blockSize; i++) {
            if (pBuffer[i] != 0xff) {
                TRACE_ERROR("Could not erase block %d\n\r", block);
                testFailed = 1;
                break;
            }
        }

        // Write block
        for (i=0; i < blockSize; i++) {

            pBuffer[i] = i & 0xFF;
        }
        // Write target block
        SkipBlockNandFlash_WriteBlock(&skipBlockNf, block, pBuffer);

        // Check that data has been written correctly
        memset(pBuffer, 0, blockSize);
        SkipBlockNandFlash_ReadBlock(&skipBlockNf, block, pBuffer);

        for (i=0; i < blockSize; i++) {
            if (pBuffer[i] != (i & 0xFF)) {
                TRACE_ERROR("Could not write block %d\n\r", block);
                testFailed = 1;
                break;
            }
        }
        block++;
    }
    // Display test result
    if (testFailed) {

        TRACE_ERROR("Test failed.\n\r");
    }
    else {

         TRACE_INFO("Test passed.\n\r");
    }

    return 0;
}

