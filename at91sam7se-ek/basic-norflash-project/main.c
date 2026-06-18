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
/// \dir "Basic-norflash-project"
///
/// !!!Purpose
///
/// The Basic %Norflash project gives you CFI supported %Norflash programming 
/// through the External Bus Interfac (EBI), so that can help develop your own 
/// %norflash devices applications with maximum efficiency.
///
/// You can find following information depends on your needs:
/// - Configures the EBI for %NorFlash access.
/// - Usage of auto detection of CFI supported  %Norflash device.
/// - API layer consists of several functions that allow user to 
///   do operations with %norflash in a unified way.
/// - Low-level driver implement procedures to program basic operations
///   described in the datasheets for %norflash devices.
/// - Sample code for accessing %norflash device.
///
/// !!!See
///
///    - norflash : %Norflash interface driver.
///
/// !!!Requirements
///
/// This package can be used with all Atmel evaluation kits that have the 
/// External Bus Interfac (EBI) and an external %NorFlash chip connecteda.
/// the package runs at SRAM or SDRAM, so SDRAM device is needed if you want 
/// to run this package in SDRAM.
///
/// !!!Description
///
/// At startup, the program configures the SMC to access the %NorFlash and tries 
/// to identify it by CFI detectiion. If it succeed, it retrieves its parameter 
/// and starts testing its blocks. Each block is first erased, then all its pages 
/// are written and verified. 
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
/// -# Upon startup, the application will output the following lines on the DBGU.
///    \code
///     -- Basic NorFlash Project xxx --
///     -- AT91xxxxxx-xx
///     -- Compiled: xxx xx xxxx xx:xx:xx --
///     -I- Common Flash Interface detecting...
///     -I- Try bus width 8 bits
///     -I- Try bus width 16 bits
///     -I- CFI detected and driver initialized
///     -I- manufactureID : 0xxxxx, deviceID : 0xxxxx
///     -I- Test in progress on block: xxxx
///    \endcode
/// -# Eventually, the test result (pass or fail) will be output on the DBGU.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// \unit
///
/// !Purpose
///
/// This file contains all the specific code for the basic-norflash-project.
/// It tests the external %NorFlash chip connected through the External Bus 
/// Interfac (EBI) by erasing and writing each one of its blocks.
/// 
/// !!!Usage
///   -# Configures the EBI for %NorFlash access at 48MHz with 
///      BOARD_ConfigureNorFlash48MHz().
///   -# Identify %norflash chip by detect CFI interface using 
///      NorFlash_CFI_Detect().
///   -# Get %Norflash device description such as memory size, byte and word 
///      configuration, block configurations using various CFI releated 
///      functions.
///   -# Erases the specified block of the device using NORFLASH_EraseSector().
///   -# Write data to specified address of the device using 
///      NORFLASH_WriteData().
///   -# Read data to specified address of the device using 
///      NORFLASH_ReadData().
///
/// !See also
///    - norflash : %Norflash interface driver.
/// Please refer to the list of functions in the #Overview# tab of this unit
/// for more detailed information.
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//         Headers
//------------------------------------------------------------------------------

#include <board.h>
#include <board_memories.h>
#include <utility/trace.h>
#include <utility/math.h>
#include <memories/norflash/NorFlashCFI.h>
#include <memories/norflash/NorFlashApi.h>

#include <string.h>

//------------------------------------------------------------------------------
//         Local variables
//------------------------------------------------------------------------------

/// Norflash device structure.
static struct NorFlash norFlash;

/// Pins to configure for the application
#ifdef PINS_NORFLASH
static const Pin pPins[] = {
   PINS_NORFLASH
};
#endif

/// Temporary buffer for unaligned read/write operations.
static unsigned char pBuffer[1024];

//------------------------------------------------------------------------------
//         Global functions
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/// Tests the norflash connected to the board by performing several command
/// on each of its pages.
//------------------------------------------------------------------------------
int main(void)
{
    unsigned int blockNumber, block, blockSize, blockAddress, pageSize, packetSize, i;
    unsigned char testFailed;
    const unsigned char busWidth[3] = {FLASH_CHIP_WIDTH_8BITS, FLASH_CHIP_WIDTH_16BITS, FLASH_CHIP_WIDTH_32BITS};

    // Configure pins and DBGU
#ifdef PINS_NORFLASH
    PIO_Configure(pPins, PIO_LISTSIZE(pPins));
#endif
    TRACE_CONFIGURE(DBGU_STANDARD, 115200, BOARD_MCK);
    printf("-- Basic NorFlash Project %s --\n\r", SOFTPACK_VERSION);
    printf("-- %s\n\r", BOARD_NAME);
    printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);

    norFlash.norFlashInfo.baseAddress = BOARD_NORFLASH_ADDR;
    // Check device CFI and get Vendor setting from it.
    printf("-I- \t Common Flash Interface detecting...\n\r");
    for (i = 0; i < 3; i++) {
        // Configure SMC for Norflash accesses 
        TRACE_INFO("\t Try bus width %d bits\n\r", busWidth[i] * 8);
        #if !defined(at91sam9m10ek) && !defined(at91sam9g45ek)
        BOARD_ConfigureNorFlash48MHz(busWidth[i] * 8);
        #else
        BOARD_ConfigureNorFlash(busWidth[i] * 8);
        #endif
        if(! NorFlash_CFI_Detect(&norFlash, busWidth[i])) break;
    }

    if (norFlash.norFlashInfo.cfiCompatible == 0) {
        TRACE_ERROR("Device Unknown\n\r");
        testFailed = 1;
        goto exit;
    }
       
    printf("-I- \t CFI detected and driver initialized\n\r");
    printf("-I- \t manufactureID : 0x%08x, deviceID : 0x%08x\n\r",
            NORFLASH_ReadManufactoryID(&norFlash),
            NORFLASH_ReadDeviceID(&norFlash));

    // Test all pages
      testFailed = 0;
    block = 0;
    blockNumber = NorFlash_GetDeviceNumOfBlocks(&(norFlash.norFlashInfo));
    pageSize = min(NorFlash_GetDeviceMinBlockSize(&(norFlash.norFlashInfo)), 1024);

    while (!testFailed && (block < blockNumber)) {

        TRACE_INFO("Test in progress on block: %6d\r", block);
        // Erase block
        NORFLASH_EraseSector(&norFlash, NorFlash_GetDeviceSectorAddress(&(norFlash.norFlashInfo), block));
        
        blockSize =  NorFlash_GetDeviceBlockSize(&(norFlash.norFlashInfo), block);
        blockAddress = NorFlash_GetDeviceSectorAddress(&(norFlash.norFlashInfo), block);
        packetSize = pageSize;
        
        while (blockSize) {
            // Verify that page has been erased correctly
            memset(pBuffer, 0, packetSize);
            NORFLASH_ReadData(&norFlash, blockAddress, pBuffer, packetSize);
            for (i=0; i < packetSize; i++) {
            
                if (pBuffer[i] != 0xFF) {
                    TRACE_ERROR("Could not erase block %d\n\r", block);
                    testFailed = 1;
                    goto exit;
                }
            }
            
            blockAddress += packetSize;
            blockSize -= packetSize;
            if (blockSize < pageSize) {
                 packetSize = blockSize;
            }
        }
        
        blockSize =  NorFlash_GetDeviceBlockSize(&(norFlash.norFlashInfo), block);
        blockAddress = NorFlash_GetDeviceSectorAddress(&(norFlash.norFlashInfo), block);
        packetSize = pageSize;
        
        while (blockSize) {
            // Write page
            for (i = 0; i < packetSize; i++) {
                pBuffer[i] = i & 0xFF;
            }
            NORFLASH_WriteData(&norFlash, blockAddress, pBuffer, packetSize);
            // Check that data has been written correctly                    
            memset(pBuffer, 0, packetSize);
            NORFLASH_ReadData(&norFlash, blockAddress, pBuffer, packetSize);
        
            for (i = 0; i < packetSize; i++) {
                if (pBuffer[i] != (i & 0xFF)) {
                    TRACE_ERROR("Could not write block %d\n\r", block);
                    testFailed = 1;
                    goto exit;
                }
            }
            blockAddress += packetSize;
            blockSize -= packetSize;
            if (blockSize < pageSize) {
                 packetSize = blockSize;
            }
        }
        block++;    
    }
    exit:
    // Display test result
    if (testFailed) {
    
        printf("-E- Test failed.\n\r");
    }
    else {
    
         printf("\n\r-I- Test passed.\n\r");
    }

    return 0;
}

