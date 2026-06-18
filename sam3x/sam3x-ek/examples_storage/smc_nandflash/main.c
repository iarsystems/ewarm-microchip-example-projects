/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support
 * ----------------------------------------------------------------------------
 * Copyright (c) 2010, Atmel Corporation
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

/**
 * \page smc_nandflash SMC NAND Flash Example
 *
 * \section Purpose
 *
 * This basic nandflash example shall show how to read and write data from/to
 * a nandflash connected to the SMC, taking ECC and Bad Block marking into account.
 *
 * \section Requirements
 *
 * This package can be used with SAM3X evaluation kits.
 *
 * \section Description
 *
 * The required steps are:
 * <ul>
 * <li> Configure the SMC to interface with the NAND Flash. </li>
 * <li> Read a page.</li>
 * <li> Calculate the ECC by software and check it is correct.</li>
 * <li> Prepare a buffer and calculate the ECC by software.</li>
 * <li> Write the buffer into a NAND flash page and store the ECC.</li>
 * <li> Read the page and check that ECC is correct.</li>
 * </ul>
 * \section Usage
 *
 * -# Build the program and download it inside the evaluation board. Please
 *    refer to the
 *    <a href="http://www.atmel.com/dyn/resources/prod_documents/doc6224.pdf">
 *    SAM-BA User Guide</a>, the
 *    <a href="http://www.atmel.com/dyn/resources/prod_documents/doc6310.pdf">
 *    GNU-Based Software Development</a> application note or to the
 *    <a href="ftp://ftp.iar.se/WWWfiles/arm/Guides/EWARM_UserGuide.ENU.pdf">
 *    IAR EWARM User Guide</a>, depending on your chosen solution.
 * -# On the computer, open and configure a terminal application
 *    (e.g. HyperTerminal on Microsoft Windows) with these settings:
 *   - 115200 bauds
 *   - 8 bits of data
 *   - No parity
 *   - 1 stop bit
 *   - No flow control
 * -# Start the application.
 * -# In the terminal window, the following text should appear:
 *     \code
 *     -- SMC NandFlash Example xxx --
 *     -- xxxxxx-xx
 *     -- Compiled: xxx xx xxxx xx:xx:xx --
 *     -I- Nandflash driver initialized
 *     -I- Size of the whole device in bytes : 0x10000000
 *     -I- Size in bytes of one single block of a device : 0x20000
 *     -I- Number of blocks in the entire device : 0x800
 *     -I- Size of the data area of a page in bytes : 0x800
 *     -I- Number of pages in the entire device : 0x40
 *    \endcode
 *
 * \section References
 * - smc_nandflash/main.c
 */

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

#include "board.h"
#include <memories.h>

#include <string.h>

/*----------------------------------------------------------------------------
 *         Local variables
 *----------------------------------------------------------------------------*/
/** Nandflash memory size. */
static uint32_t dwMemSize;
/** Number of blocks in nandflash.*/
static uint32_t dwNumBlocks;
/** Size of one block in the nandflash, in bytes.*/
static uint32_t dwBlockSize;
/** Size of one page in the nandflash, in bytes.*/
static uint32_t dwPageSize;
/** Number of page per block*/
static uint32_t dwNumPagesPerBlock;


/** Pins used to access to nandflash.*/
static const Pin pPinsNf[] = {PINS_NANDFLASH};
/** Nandflash device structure.*/
static struct SkipBlockNandFlash skipBlockNf;
/** Global DMA driver instance for all DMA transfers in application. */
static sDmad dmad;
/** Address for transferring command bytes to the nandflash.*/
static uint32_t cmdBytesAddr = BOARD_NF_COMMAND_ADDR;
/** Address for transferring address bytes to the nandflash.*/
static uint32_t addrBytesAddr = BOARD_NF_ADDRESS_ADDR;
/** Address for transferring data bytes to the nandflash.*/
static uint32_t dataBytesAddr = BOARD_NF_DATA_ADDR;
/** Nandflash chip enable pin.*/
static const Pin nfCePin = BOARD_NF_CE_PIN;
/** Nandflash ready/busy pin.*/
static const Pin nfRbPin = BOARD_NF_RB_PIN;

/** Nandflash page buffer. */
uint8_t pageBuffer[2048];
/** Nandflash read buffer.*/
uint8_t readBuffer[2048];

/**
 * \brief Systick handler to manage timestamping and delay
 */
void SysTick_Handler( void )
{
    TimeTick_Increment() ;
}

/**
 * Get Delayed number of tick
 * \param startTick Start tick point.
 * \param endTick   End tick point.
 */
static uint32_t _GetDelayInTicks(uint32_t dwStartTick, uint32_t dwEndTick)
{
    if (dwEndTick > dwStartTick) return (dwEndTick - dwStartTick);
    return (dwEndTick + (0xFFFFFFFF - dwStartTick));
}


/**
 * \brief  Measure throughtput for RAW data write/read with DMA enabled.
  */
static void _NandPerformanceTest( uint16_t wBlockNum )
{
    uint16_t wPage, dwNumPagesPerBlock;
    uint32_t dwTickStart, dwTickEnd, dwTicks, dwRWSpeed;
    uint32_t dw;

    dwNumPagesPerBlock = NandFlashModel_GetBlockSizeInPages(&skipBlockNf.ecc.raw.model);

    printf("-I- Erase block %d \n\r", wBlockNum);
    SkipBlockNandFlash_EraseBlock(&skipBlockNf, wBlockNum, SCRUB_ERASE);
    printf("-I- Write block %d \n\r", wBlockNum);
    for (dw = 0; dw < dwPageSize; dw++) 
    {
        pageBuffer[dw] = dw & 0xFF;
    }
    dwTickStart = GetTickCount();
    for ( wPage = 0; wPage < dwNumPagesPerBlock; wPage++)
    {
        RawNandFlash_WritePage(((struct RawNandFlash *) &skipBlockNf) ,wBlockNum, wPage, pageBuffer, 0);
    }
    dwTickEnd = GetTickCount();
    dwTicks = _GetDelayInTicks(dwTickStart, dwTickEnd);
    dwRWSpeed = ( NandFlashModel_GetBlockSizeInBytes(&skipBlockNf.ecc.raw.model)) / dwTicks;
    printf("-I- Raw block write speed %uK/s\n\r", (uint32_t)dwRWSpeed);

    printf("-I- Read block %d \n\r", wBlockNum);
    memset(pageBuffer, 0, dwPageSize);
    dwTickStart = GetTickCount();
    for ( wPage = 0; wPage < dwNumPagesPerBlock; wPage++)
    {
        RawNandFlash_ReadPage(((struct RawNandFlash *) &skipBlockNf) ,wBlockNum, wPage, pageBuffer, 0);
    }
    dwTickEnd = GetTickCount();
    dwTicks = _GetDelayInTicks(dwTickStart, dwTickEnd);
    dwRWSpeed = ( NandFlashModel_GetBlockSizeInBytes(&skipBlockNf.ecc.raw.model)) / dwTicks;
    printf("-I- Raw block Read speed %uK/s\n\r", (uint32_t)dwRWSpeed);
}


/*----------------------------------------------------------------------------
 *         Global functions
 *----------------------------------------------------------------------------*/
extern int main( void )
{
    /* Temporary buffer */
    uint16_t wBlockNum, wPage;
    uint32_t dw;
    /* Errors returned by SkipNandFlash functions */
    uint8_t ucError = 0;

    /* Disable watchdog */
    WDT_Disable( WDT ) ;
    TimeTick_Configure( BOARD_MCK ) ;

    /* Output example information */
    printf("-- SMC NandFlash Example %s --\n\r", SOFTPACK_VERSION);
    printf("-- %s\n\r", BOARD_NAME);
    printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);

    /* Configure SMC for Nandflash accesses */
    BOARD_ConfigureNandFlash(SMC);
    PIO_PinConfigure(pPinsNf, PIO_LISTSIZE(pPinsNf));
    
    /* Initialize DMA driver instance with polling mode */
    DMAD_Initialize( &dmad, 1 );
    if ( NandFlashConfigureDmaChannels( &dmad ))
    {
        printf ("-E- Initialize DMA failed !");
    }
    printf ("-I- Initialize DMA done.\n\r");

    memset(&skipBlockNf, 0, sizeof(skipBlockNf));

    if (SkipBlockNandFlash_Initialize(&skipBlockNf,
                                0,
                                cmdBytesAddr,
                                addrBytesAddr,
                                dataBytesAddr,
                                nfCePin,
                                nfRbPin)) {

        printf("-E- Device Unknown\n\r");
        return 0;
    }

    printf("-I- Nandflash driver initialized\n\r");

    /* Get device parameters */
    dwMemSize = NandFlashModel_GetDeviceSizeInBytes(&skipBlockNf.ecc.raw.model);
    dwBlockSize = NandFlashModel_GetBlockSizeInBytes(&skipBlockNf.ecc.raw.model);
    dwNumBlocks = NandFlashModel_GetDeviceSizeInBlocks(&skipBlockNf.ecc.raw.model);
    dwPageSize = NandFlashModel_GetPageDataSize(&skipBlockNf.ecc.raw.model);
    dwNumPagesPerBlock = NandFlashModel_GetBlockSizeInPages(&skipBlockNf.ecc.raw.model);

    printf("-I- Size of the whole device in bytes : 0x%x \n\r",dwMemSize);
    printf("-I- Size in bytes of one single block of a device : 0x%x \n\r",dwBlockSize);
    printf("-I- Number of blocks in the entire device : 0x%x \n\r",dwNumBlocks);
    printf("-I- Size of the data area of a page in bytes : 0x%x \n\r",dwPageSize);
    printf("-I- Number of pages in the entire device : 0x%x \n\r",dwNumPagesPerBlock);

    wBlockNum = 12;
    wPage = 6;

    /* Erase block */
    ucError = SkipBlockNandFlash_EraseBlock(&skipBlockNf, wBlockNum, NORMAL_ERASE);

    if ( ucError == NandCommon_ERROR_BADBLOCK )
    {
        printf("-E- Block %d is BAD block. \n\r",  wBlockNum);
        printf("-E- Test terminate, try another block. \n\r");
        return ucError;
    }

    /* Prepare a page size buffer in SRAM.*/
    printf("-I- Preparing a buffer in SRAM ...\n\r");
    for ( dw = 0; dw < dwPageSize; dw++ )
    {
        pageBuffer[dw] = dw & 0xFF;
    }

    /* Reset read buffer.*/
    memset( readBuffer, 0, sizeof( readBuffer ) ) ;

    /* ECC is calculated by software, it writes the buffer into a NAND flash page and stores the ECC in spare area. */
    printf("-I- Write the buffer in page %d of block %d with ECC stored in page spare area.\n\r", wPage, wBlockNum);
    ucError = SkipBlockNandFlash_WritePage(&skipBlockNf, wBlockNum, wPage, pageBuffer, 0);
    if ( ucError )
    {
        printf("-E- Cannot write page %d of block %d.\n\r", wPage, wBlockNum);
        return ucError;
    }

    /* Read the page and check that ECC is correct. */
    printf("-I- Read page %d of block %d with ECC check.\n\r", wPage, wBlockNum);
    ucError = SkipBlockNandFlash_ReadPage(&skipBlockNf, wBlockNum, wPage, readBuffer, 0);
    if ( ucError )
    {
        printf("-E- Read page %d of block %d failed.\n\r", wPage, wBlockNum);
        if ( ucError == NandCommon_ERROR_CORRUPTEDDATA )
        {
            printf("-E- Block have %d. page %d unrecoverable data\n\r", wBlockNum, wPage);
        }
        return ucError;
    }
    /* Test if the read buffer is the same as SRAM buffer */
    ucError = memcmp(readBuffer, pageBuffer, sizeof(pageBuffer));
    if ( ucError )
    {
        printf("-I- Read data is different from SRAM buffer.\n\r");
        printf("-I- Test KO.\n\r");

        return 0;
    }
    printf("-I- Read data matches SRAM buffer.\n\r");

    /* Test nand flash read/write performance */
    _NandPerformanceTest(wBlockNum);

    printf("-I- Test passed.\n\r");    
    
    return 0;
}

