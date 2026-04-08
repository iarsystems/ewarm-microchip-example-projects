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
#include <irq/irq.h>
#include <pio/pio.h>
#include <dbgu/dbgu.h>
#include <utility/assert.h>
#include <utility/trace.h>
#include <sdmmc/sdmmc_mci.h>

#include <string.h>

//------------------------------------------------------------------------------
/// \dir "Basic-sdcard-project"
///
/// !!!Purpose
///
/// The Basic-sdcard-project will help you to get familiar with sdmmc_mci interface on 
/// AT91SAM microcontrollers. It can also help you to get familiar with the SD operation flow
/// which can be used for fast implementation of your own sd drivers and other applications 
/// related.
///
/// You can find following information depends on your needs:
/// - Usage of auto detection of sdcard insert and sdcard write-protection detection
/// - MCI interface initialize sequence and interrupt installation
/// - Sdcard driver implementation based on MCI interface 
/// - Sdcard physical layer initialize sequence implementation
/// - Sample usage of sdcard write and read
/// 
/// !See
/// - sdmmc_mci : sdcard physical layer driver with mci-interface
/// - mci : sdcard physical layer driver with mci-interface
///
/// !!!Requirements
///
/// This package can be used with all Atmel evaluation kits that have MCI interface, the 
/// package runs at SRAM or SDRAM, so EBI interface and SDRAM device is needed if you
/// want to run this package in SDRAM
///
/// !!!Description
///
/// Open HyperTerminal before running this program, use SAM-BA to download this program to 
/// SRAM or external RAM, make the program run, the HyperTerminal will give out the test results.
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
///     -- Basic SD-Card MCI Mode Project xxx --
///     -- AT91xxxxxx-xx
///     -- Compiled: xxx xx xxxx xx:xx:xx --
///     -I- Please connect a SD card ...
///     -I- SD card connection detected
///     -I- Cannot check if SD card is write-protected
///     -I- SD/MMC card initialization successful
///     -I- Card size: *** MB
///     -I- Block size: *** Bytes
///     -I- Testing block [  *** -   ***] ..."
///     \endcode
///
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// \unit
///
/// !Purpose
///
/// This file contains all the specific code for the basic-sdcard-project
///
/// !Contents
/// The basic-sdcard-project application can be roughly broken down as follows:
///    - Optional functions
///       - CheckProtection
///       - WaitSdConn
///    - Interrupt handlers
///       - ISR_Mci0
///    - The main function, which implements the program behavior
///       - I/O configuration
///       - sd card auto-detect and check whether sd card is write-protected (if supported)
///       - Initialize MCI interface and installing an isr relating to MCI
///       - Initialize sdcard, get necessary sdcard's parameters
///       - write/read sdcard at 10MHz sd clock
///       - write/read sdcard at 1MHz sd clock 
//------------------------------------------------------------------------------

//         Local variables
//------------------------------------------------------------------------------

#include "rtc/rtc.h"
#include "tc/tc.h"

#if defined(MCI2_INTERFACE)
#include "dmad/dmad.h"
#endif
/// MCI driver instance.
static Mci mciDrv;

/// SDCard driver instance.
static SdCard sdDrv;

/// SD card pins.
static const Pin pinsSd[] = {BOARD_SD_PINS};

#define NB_MULTI_BLOCKS     16

#define NB_SPLIT_MULTI      2

/// Date buffer
static unsigned char pBuffer[SD_BLOCK_SIZE * NB_MULTI_BLOCKS];

/// Number of errors displayed
#define NB_ERRORS       5

static unsigned int nbErrors;

static volatile unsigned int tick100u = 0;

//------------------------------------------------------------------------------
//         Local macros
//------------------------------------------------------------------------------
//#define MULTIPLE_TEST
#if defined (at91sam9m10) || defined(at91sam9rl64)
#define PERFORMANCE_TEST
#endif

#define NB_REPEAT_TIME      128*10

#define TEST_BLOCK_START    0
//#define TEST_BLOCK_END      15100
#define TEST_BLOCK_END      SD_TOTAL_BLOCK(&sdDrv) // Whole SD Card

//------------------------------------------------------------------------------
//         Local functions
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/// MCI0 interrupt handler. Forwards the event to the MCI driver handler.
//------------------------------------------------------------------------------
void MCI0_IrqHandler(void)
{
    MCI_Handler(&mciDrv);
}

//------------------------------------------------------------------------------
//         Optional: SD card detection
//------------------------------------------------------------------------------

#ifdef BOARD_SD_PIN_CD

/// SD card detection pin instance.
static const Pin pinMciCardDetect = BOARD_SD_PIN_CD;

//------------------------------------------------------------------------------
/// Waits for a SD card to be connected.
//------------------------------------------------------------------------------
void WaitSdConn(void)
{
    PIO_Configure(&pinMciCardDetect, 1);
    printf("-I- Please connect a SD card ...\n\r");
    while (PIO_Get(&pinMciCardDetect) != 0);
    printf("-I- SD card connection detected\n\r");
}

#else

//------------------------------------------------------------------------------
/// Dummy function.
//------------------------------------------------------------------------------
void WaitSdConn(void)
{
    printf("-I- SD card detection not available, assuming card is present\n\r");
}

#endif

//------------------------------------------------------------------------------
//         Optional: Write protection status
//------------------------------------------------------------------------------

#ifdef BOARD_SD_PIN_WP

/// Write protection status pin instance.
static const Pin pinMciWriteProtect = BOARD_SD_PIN_WP;

//------------------------------------------------------------------------------
/// Checks if the device is write protected.
//------------------------------------------------------------------------------
void CheckProtection(void)
{
    PIO_Configure(&pinMciWriteProtect, 1);
    if (PIO_Get(&pinMciWriteProtect) != 0) {

        printf("-I- SD card is write-protected\n\r");
    }
    else {

        printf("-I- SD card is NOT write-protected.\n\r");
    }
}

#else

//------------------------------------------------------------------------------
/// Dummy implementation.
//------------------------------------------------------------------------------
void CheckProtection(void)
{
    printf("-I- Cannot check if SD card is write-protected\n\r");
}

#endif

//------------------------------------------------------------------------------
/// Max Error Break
//------------------------------------------------------------------------------
unsigned char MaxErrorBreak(void)
{
    if (NB_ERRORS) {
        if (nbErrors ++ > NB_ERRORS) {
            while(1);
            return 1;
        }
    }
    return 0;
}

//------------------------------------------------------------------------------
//         Global functions
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/// Main function
//------------------------------------------------------------------------------
int main(void)
{
    unsigned char rc;
    unsigned int block;
    unsigned int i;
    unsigned short multiBlock;
    unsigned char error;
    unsigned int mciSpeed;
    unsigned int mciDiv;
    #ifdef PERFORMANCE_TEST
    unsigned char wHour, wMinute, wSecond;
    unsigned char rHour, rMinute, rSecond;
    unsigned int wData, rData,wTime, rTime, wSpeed, rSpeed;
    #endif
    //unsigned int retry;

    TRACE_CONFIGURE(DBGU_STANDARD, 115200, BOARD_MCK);
    printf("-- Basic SD-Card MCI Mode Project %s --\n\r", SOFTPACK_VERSION);
    printf("-- %s\n\r", BOARD_NAME);
    printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);

    // Configure SDcard pins
    PIO_Configure(pinsSd, PIO_LISTSIZE(pinsSd));

    // Wait for SD card connection (if supported)
    WaitSdConn();

    // Check if card is write-protected (if supported)
    CheckProtection();

    // Initialize the MCI driver
    IRQ_ConfigureIT(BOARD_SD_MCI_ID,  1, MCI0_IrqHandler);
    MCI_Init(&mciDrv, BOARD_SD_MCI_BASE, BOARD_SD_MCI_ID, BOARD_SD_SLOT);
    #if defined(MCI2_INTERFACE)
    DMAD_Initialize(BOARD_MCI_DMA_CHANNEL);
    #endif
    IRQ_EnableIT(BOARD_SD_MCI_ID);

    // Initialize the SD card driver
    rc = SD_Init((SdCard*)&sdDrv, (SdDriver *)&mciDrv);
    if (rc) {

        printf("-E- SD/MMC card initialization failed\n\r");
    }
    else {

        volatile unsigned int sizeInMB, sizeOfBlock;
        if (SD_TOTAL_SIZE(&sdDrv) == 0xFFFFFFFF) {
            sizeInMB = SD_TOTAL_BLOCK(&sdDrv) / (1024 * 2);
            sizeOfBlock = 512;
        }
        else {
            sizeInMB = SD_TOTAL_SIZE(&sdDrv) / (1024 * 1024);
            sizeOfBlock = SD_TOTAL_SIZE(&sdDrv) / SD_TOTAL_BLOCK(&sdDrv);
        }

        printf("-I- SD/MMC card initialization successful\n\r");
        printf("-I- Card size: %u MB, %u * %dB\n\r",
               sizeInMB, SD_TOTAL_BLOCK(&sdDrv), sizeOfBlock);
    }

    SD_DisplayRegisterCID((SdCard*)&sdDrv);
    SD_DisplayRegisterCSD((SdCard*)&sdDrv);

#if 0
    ConfigureTc0();
    printf("Test TC\n\r");
    i = tick100u;
    while(i == tick100u);
    printf("TC works\n\r");
    //MMC_TestExtCSD(&sdDrv, &tick100u, &pinTrg);
    MMC_TestExtCSD(&sdDrv, 0, &pinTrg);
#endif

    SD_DisplayRegisterECSD((SdCard*)&sdDrv);

    if(rc) for(;;);

    mciSpeed = 10000000;
    MCI_SetSpeed(&mciDrv, mciSpeed, sdDrv.transSpeed, BOARD_MCK);
    MCI_GetSpeed(&mciDrv, &mciDiv);
    printf("-I- MCK %dK Hz, MCI Speed %dK, divisor %d. \r\n",
           BOARD_MCK/1000, mciSpeed/1000, mciDiv);

    // Perform tests on each block
    multiBlock = 0;
    for (block = TEST_BLOCK_START;
         block < TEST_BLOCK_END;
         block += multiBlock) {
		 	 
         unsigned int splitMulti;

        // Perform different single or multiple bloc operations
        if (++multiBlock > NB_MULTI_BLOCKS)
            multiBlock = 1;

        // Multi-block adjustment
        if (block + multiBlock > TEST_BLOCK_END) {
            multiBlock = TEST_BLOCK_END - block;
        }

        // Perform single block or multi block transfer
        printf("\r-I- Testing block [%6u - %6u] ...",
               block, (block + multiBlock -1));

        // Clear the block
        memset(pBuffer, 0, SD_BLOCK_SIZE * multiBlock);
        for (i=0; i < SD_BLOCK_SIZE * multiBlock; i++) {

            if (pBuffer[i] != 0) {

                printf("\n\r-E- Data @ %u before write : 0x00 <> 0x%02x\n\r",
                       i, pBuffer[i]);
                MaxErrorBreak();
                // Only find first verify error.
                continue;
            }
        }

        error = SD_WriteBlock(&sdDrv, block, multiBlock, pBuffer);
        if (error) {
            printf("\n\r-E- 1. Write block (%d) #%u\n\r", error, block);
            MaxErrorBreak();
            // Skip following test
            continue;
        }

        // Read back the data to check the write operation
        memset(pBuffer, 0xFF, SD_BLOCK_SIZE * multiBlock);
        error = SD_ReadBlock(&sdDrv, block, multiBlock, pBuffer);
        if (error) {
            printf("\n\r-E- 1. Read block (%d) #%u\n\r", error, block);
            MaxErrorBreak();
            // Skip following test
            continue;
        }

        for (i=0; i < SD_BLOCK_SIZE * multiBlock; i++) {

            if (pBuffer[i] != 0) {
                printf("\n\r-E- 1. Data @ %u : 0x00 <> 0x%02X\n\r",
                       i, pBuffer[i]);
                MaxErrorBreak();
                // Only find first verify error.
                break;
            }
        }

        // Write a checkerboard pattern on the block
        for (i=0; i < SD_BLOCK_SIZE * multiBlock; i++) {
            if ((i & 1) == 0)  pBuffer[i] = (i & 0x55);
            else               pBuffer[i] = (i & 0xAA);
        }
#if 1
        for (i = 0;i < multiBlock;) {
            splitMulti = ((multiBlock - i) > NB_SPLIT_MULTI) ?
                                    NB_SPLIT_MULTI : (multiBlock - i);
            error = SD_WriteBlock(&sdDrv,
                                  block + i,
                                  splitMulti,
                                  &pBuffer[i * SD_BLOCK_SIZE]);
            if (error)
                break;
            i += splitMulti;
        }
        ASSERT(i == multiBlock, "Unexpected W, %d!", i);
#else
        splitMulti = (multiBlock > NB_SPLIT_MULTI) ? NB_SPLIT_MULTI : 0;
        //splitMulti = 0;
        if (splitMulti) {
            error = SD_WriteBlock(&sdDrv, block, splitMulti, pBuffer);
        }
        if (!error) {
            error = SD_WriteBlock(&sdDrv,
                                  block + splitMulti,
                                  multiBlock - splitMulti,
                                  &pBuffer[splitMulti * SD_BLOCK_SIZE]);
        }
#endif
        if (error) {
            printf("\n\r-E- 2. Write block #%u\n\r", block);
            MaxErrorBreak();
            // Skip Following Test
            continue;
        }

        // Read back the data to check the write operation
        memset(pBuffer, 0, SD_BLOCK_SIZE * multiBlock);
#if 1
        for (i = 0;i < multiBlock;) {
            splitMulti = ((multiBlock - i) > NB_SPLIT_MULTI) ?
                                    NB_SPLIT_MULTI : (multiBlock - i);
            error = SD_ReadBlock(&sdDrv,
                                 block + i,
                                 splitMulti,
                                 &pBuffer[i * SD_BLOCK_SIZE]);
            if (error)
                break;
            i += splitMulti;
        }
        ASSERT(i == multiBlock, "Unexpected R %d!", i);
#else
        splitMulti = (multiBlock > NB_SPLIT_MULTI) ? NB_SPLIT_MULTI : 0;
        //splitMulti = 0;
        if (splitMulti) {
            error = SD_ReadBlock(&sdDrv, block, splitMulti, pBuffer);
        }
        if (!error) {
            error = SD_ReadBlock(&sdDrv,
                                 block + splitMulti,
                                 multiBlock - splitMulti,
                                 &pBuffer[splitMulti * SD_BLOCK_SIZE]);
        }
#endif
        if (error) {
            printf("\n\r-E- 2. Read block #%u\n\r", block);
            MaxErrorBreak();
            // Skip Following Test
            continue;
        }

        for (i=0; i < SD_BLOCK_SIZE * multiBlock; i++) {

            if (!(((i & 1) == 0) && (pBuffer[i] == (i & 0x55))) &&
                !(((i & 1) != 0) && (pBuffer[i] == (i & 0xAA))) ) {
                printf("\n\r-E- 2. Data @ %u 0x%02X <> 0x%02X\n\r",
                    i, ((i & 1) == 0) ? (i & 0x55) : (i & 0xAA), pBuffer[i]);
                MaxErrorBreak();
                // Only find first verify error.
                break;
            }
        }

        // Uses DBGU key to skip blocks
        if (DBGU_IsRxReady()) {
            if (DBGU_GetChar() == 27) {
                block += 500 * 2;  // Skip 500K data
            }
        }
    }

    #ifdef MULTIPLE_TEST
    mciSpeed = 30000000;
    MCI_SetSpeed(&mciDrv, mciSpeed, sdDrv.transSpeed, BOARD_MCK);
    MCI_GetSpeed(&mciDrv, &mciDiv);
    TRACE_DEBUG("Board MCK is %d Hz, MCI divisor is %d. \r\n", BOARD_MCK, mciDiv);
    block=0;
    for (; block < SD_TOTAL_BLOCK(&sdDrv)-NB_MULTI_BLOCKS; ) {

        for (multiBlock = 0; multiBlock < NB_MULTI_BLOCKS; multiBlock++) {
            for (i=0; i < SD_BLOCK_SIZE; i++) {
                    pBuffer[i] = ((block + multiBlock) & 0xFF);
            }
            printf("\r-I- Writing block [%6u] ...             ", (block + multiBlock));
            error = SD_WriteBlock(&sdDrv, (block + multiBlock), 1, pBuffer);
            ASSERT(!error, "\n\r-F- Failed to write block #%u\n\r", (block + multiBlock));
        }

        printf("\n\r");

        for (multiBlock = 0; multiBlock < NB_MULTI_BLOCKS; multiBlock++) {
            printf("\r-I- Reading block [%6u] ... ", (block + multiBlock));
            error = SD_ReadBlock(&sdDrv, (block + multiBlock), 1, pBuffer);
            ASSERT(!error, "\n\r-F- Failed to read block #%u\n\r", (block + multiBlock));
            for (i=0; i < SD_BLOCK_SIZE; i++) {

                ASSERT(pBuffer[i] == ((block + multiBlock) & 0xFF),
                       "\n\r-F- Invalid data at pBuffer[%u] (expected 0x%02X, read 0x%02X)\n\r",
                       i, ((block + multiBlock) & 0xFF), pBuffer[i]);
            }
        }
        printf("\n\r");

        block+= NB_MULTI_BLOCKS;
    }
    #endif

    #ifdef PERFORMANCE_TEST
    // Configure RTC
    RTC_SetHourMode(0); // 24-hour mode
    RTC_SetTimeAlarm(0, 0, 0);
    RTC_SetDateAlarm(0, 0);

    mciSpeed = sdDrv.transSpeed;
    printf("\n\rMCI performance test start.\n\r");
    wHour = wMinute = wSecond = 0;
    rHour = rMinute = rSecond = 0;
    MCI_SetSpeed(&mciDrv, mciSpeed, sdDrv.transSpeed, BOARD_MCK);
    block=0;
    for (i=0; i < SD_BLOCK_SIZE * NB_MULTI_BLOCKS; i++) {
            pBuffer[i] = (0x5a);
    }

    RTC_SetTime(0, 0, 0);
    for (; block < NB_REPEAT_TIME* NB_MULTI_BLOCKS; ) {
        error = SD_WriteBlock(&sdDrv, block, NB_MULTI_BLOCKS, pBuffer);
        block+= NB_MULTI_BLOCKS;
    }
    RTC_GetTime(&wHour, &wMinute, &wSecond);
    printf("Write finish time %d H, %d M, %d S \r\n", wHour, wMinute, wSecond);

    block=0;
    RTC_SetTime(0, 0, 0);
    for (; block <  NB_REPEAT_TIME* NB_MULTI_BLOCKS; ) {
        error = SD_ReadBlock(&sdDrv, block , NB_MULTI_BLOCKS, pBuffer);
        block+= NB_MULTI_BLOCKS;
    }
    RTC_GetTime(&rHour, &rMinute, &rSecond);
    printf("Read finish time %d H, %d M, %d S \r\n", rHour, rMinute, rSecond);

    wData = rData = NB_REPEAT_TIME* NB_MULTI_BLOCKS * SD_BLOCK_SIZE;
    wTime = wHour*3600 + wMinute*60 + wSecond;
    rTime = rHour*3600 + rMinute*60 + rSecond;
    wSpeed = wData/wTime;
    rSpeed = rData/rTime;
    MCI_GetSpeed(&mciDrv, &mciDiv);
    printf("MCI performance test: MCK = %d Hz, MCI clock = %d Hz, div %d. \r\n",
        BOARD_MCK, BOARD_MCK/(mciDiv+1)/2, mciDiv);
    printf("Write/Read multiple blocks %d blocks. \r\n", NB_MULTI_BLOCKS);
    printf("Write/Read data amount %d KB. \r\n", (NB_REPEAT_TIME* NB_MULTI_BLOCKS/2));
    printf("Write performance: %d Bytes/Sec. \r\n", wSpeed);
    printf("Read performance: %d Bytes/Sec. \r\n", rSpeed);
    #endif

    error = SD_Stop((SdCard*)&sdDrv, (SdDriver *)&mciDrv);
    ASSERT(!error, "\n\r-F- Failed to stop SD card!\n\r");

    return 0;
}

