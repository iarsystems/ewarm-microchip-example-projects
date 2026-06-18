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


/**
* \page hsmci_sdcard_fatfs  Basic SDCARD fatfs project
*
* \section purpose
*
* The Basic-fatfs-multimedia-card-project will help you to get familiar with filesystem and
* sdmmc_mci interface on AT91 microcontrollers.
* It provides sample code of common operations of a filesystem through a SD
* Card based filesystem.
* It can also help you to configure the filesystem according to your own
* needs, which can be used for fast implementation of your own filesystem and
* other applications related.
* FatFs is a generic file system module to implement the FAT file system to
* small embedded systems.
*
* \section See
* - <a href="http://elm-chan.org/fsw/ff/00index_e.html">FATFs Website</a>
*
* You can find following information depends on your needs:
* - Supply a set of file system interface for user to do file related work,
*   e.g. mount a disk, initialize a disk, create/open a file, write/read a
*   file e.t.c.
* - Supply a set of disk level interface for user's reference, which is called
*   by filesystem level.
* - Demonstrate user how to use these interfaces through sample code.
* - Configuration of a filesystem, e.g. user can build up a tiny filesystem
*   which consume low memory, or a filesystem with read-only functions.
*   Configuration is set in the %fatfs_config.h file in the project folder.
*
* \section Requirements
*
* - This package can be used with SAM3X evaluation kit.
*
* \section Description
*
* When launched, this program asks user to plug a MMC card in the MCI
* connector.
* Once the SD card is plugged, there are 2 cases:
*   - The SD card is not yet formated, the program formats it with FAT file
*     system.
*   - The SD card is already formated, depending on the user answer,
*     the SD card is erased and re-formated or nothing is done.
*
* Then a Basic.bin file is created, written with a special pattern and closed.
* Finally the file is reopened and its %data are verified.
*
* For the FATFS TINY project version, copy previously the Basic.bin file on
* the SD Card in the root directory.
* This file can be found in the project folder.
* The FAT system is compiled with only the reading functions (see _FS_READONLY
* option in %fatfs_config.h).
* This option can be modified.
*
* \section Note
*
* The basic-multimedia-card-project can be used to "unformat" a SD card. By
* writing dummy values in the first blocks, the FAT file system %data are
* erased.
*
* \section Usage
*
* -# Build the program and download it on the evaluation board. Please
*    refer to one of the documents below according to your development tool:
*   - <a href="http://www.atmel.com/dyn/resources/prod_documents/doc6421.pdf"> SAM-BA User Guide</a>
*   - <a href="http://www.atmel.com/dyn/resources/prod_documents/doc6310.pdf">
*       GNU-Based Software Development application note</a>
*   - <a href="http://www.iar.se/website1/1.0.1.0/78/1/"> IAR EWARM User Guide</a>
*   - <a href="http://www.keil.com/support/man/docs/gsac/"> MDK-ARM Primer</a>
* -# On the computer, open and configure a terminal application
*    (e.g. HyperTerminal on Microsoft Windows) with these settings:
*   - 115200 bauds
*   - 8 bits of data
*   - No parity
*   - 1 stop bit
*   - No flow control
* -# Start the application
* -# In HyperTerminal, it will show something like
*     \code
*     -- Basic FatFS Full Version with SDCard Project xxx --
*     -- AT91xxxxxx-xx
*     -- Compiled: xxx xx xxxx xx:xx:xx --
*     -I- Please connect a SD card ...
*     -I- SD card connection detected        (After a SD card is connected correctly)
*     -I- Init media Sdcard
*     -I- MEDSdcard init
*     -I- DMAD_Initialize channel 0
*     -I- Card Type 1, CSD_STRUCTURE 0
*     -I- SD/MMC TRANS SPEED 25000 KBit/s
*     -I- SD 4-BITS BUS
*     -I- CMD6(1) arg 0x80FFFF01
*     -I- SD HS Enable
*     -I- SD/MMC TRANS SPEED 50000 KBit/s
*     -I- SD/MMC card initialization successful
*     -I- Card size: 1914 MB
*     -I- Mount disk 0
*     -I- The disk is already formated.
*     -I- Display files contained on the SDcard :
*     0:/BASIC.bin
*     -I- Do you want to erase the sdcard to re-format disk ? (y/n)!  (suppose you choose 'y' here)
*     -I- Erase the first 100 blocks complete !
*     -I- Format disk 0
*     -I- Please wait a moment during formating...
*     -I- Format disk finished !
*     -I- Create a file : "0:Basic.bin"
*     -I- Write file
*     -I- ByteWritten=2064
*     -I- f_write ok: ByteWritten=2064
*     -I- Close file
*     -I- Open the same file : "0:Basic.bin"
*     -I- Read file
*     -I- Close file
*     -I- File data Ok !
*     -I- Test passed !
*
*     \endcode
*
*  \section References
*  - hsmci_sdcard_fatfs/main.c
*  - hsmci.h
*  - pio.h
*
*/

/**
*  \file
*
* \section Purpose
*
* This file contains all the specific code for the basic-fatfs-sdcard-project
*
* \Contents
* The basic-fatfs-sdcard-project application can be roughly broken down as follows:
*    - The main function, which implements the program behavior
*       - SD Card interface initialize
*       - Mount a disk on SD card
*       - Test if the SD Card is already formated
*       - Display all the files contained on the SD card if already formated
*       - Format SD Card if not yet formated
*       - Create a new file
*       - Write something to a opened file
*       - Close a file
*       - Open an existing file
*       - Read a file
*       - Close a file
*       - Compare the reading result, print the compare result to DBGU
*/

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include "memories.h"
#include "fatfs_config.h"

/*----------------------------------------------------------------------------
 *        Local constants
 *----------------------------------------------------------------------------*/

/** Maximum number of LUNs which can be defined.*/
/** (Logical drive = physical drive = medium number)*/
#define MAX_LUNS        2

/** Available medias.*/
Media medias[MAX_LUNS];

/*----------------------------------------------------------------------------
 *        Local variables
 *----------------------------------------------------------------------------*/

/** DMA driver instance */
static sDmad dmaDrv;

/** MCI driver instance. */
static sMcid mciDrv[BOARD_NUM_MCI];

/** SDCard driver instance. */
sSdCard sdDrv[BOARD_NUM_MCI];

/** SD card pins instance. */
static const Pin pinsSd[] = {BOARD_SD_PINS};

/** SD card detection pin instance. */
static const Pin pinsCd[] = {BOARD_SD_PIN_CD};

#define ID_DRV DRV_MMC

#if _FS_TINY == 0
#define STR_ROOT_DIRECTORY "1:"
#else
#define STR_ROOT_DIRECTORY ""
#endif

#if defined(at91cap9stk)
#define MCI_ID 1 
#else
#define MCI_ID 0
#endif

const char* FileName = STR_ROOT_DIRECTORY "Basic.bin";

/** size of the file to write/read.minimum size 512 for erase operation*/
#define DATA_SIZE 2048

uint8_t data[DATA_SIZE];

/** Test settings: Number of bytes to test performance */
#define TEST_PERFORMENCT_SIZE   (4*1024*1024)


#define ASSERT(condition, ...)  { \
            if (!(condition)) { \
                printf("-F- ASSERT: "); \
                printf(__VA_ARGS__); \
                while (1); \
            } \
        }
/*----------------------------------------------------------------------------
 *        Local Functions
 *----------------------------------------------------------------------------*/
/**
 * DMA interrupt handler.
 */
void DMAC_IrqHandler(void)
{
    DMAD_Handler(&dmaDrv);
}

/**
 * MCI interrupt handler. Forwards the event to the MCI driver handlers.
 */
void HSMCI_IrqHandler(void)
{
    uint32_t i;
    for (i = 0; i < BOARD_NUM_MCI; i ++)
    {
        MCID_Handler(&mciDrv[i]);
    }
}
/**
 *  \brief Handler for System Tick interrupt.
 *
 *  Accumulate time_stamp.
 */
void SysTick_Handler( void )
{
    TimeTick_Increment() ;
}
/*----------------------------------------------------------------------------
 *         Optional: SD card detection (connection, protection)
 *----------------------------------------------------------------------------*/

/**
 * Configure for SD detect pin
 */
static void CardDetectConfigure(void)
{
    PIO_PinConfigure(pinsCd, PIO_LISTSIZE(pinsCd));
    /* No protection detect pin */
}

/**
 * Return 1 if card is insertted.
 */
static uint8_t CardIsConnected(uint8_t iMci)
{
    return PIO_PinGet(&pinsCd[iMci]) ? 0 : 1;
}

/**
 * Return 1 if any card is inserted.
 */
static uint8_t AnyCardIsConnected(void)
{
    uint32_t i;
    for (i = 0; i < BOARD_NUM_MCI; i ++)
    {
        if ( CardIsConnected(i) )
        {
            return 1;
        }
    }
    return 0;
}

/**
 * Return 1 if card is protected.
 */
static uint8_t CardIsProtected(uint8_t iMci)
{
    printf("-I- Cannot check if SD card %d is write-protected\n\r", iMci);
    return 0;
}

/**
 * Delay some loop
 */
static void LoopDelay(volatile uint32_t loop)
{
    for(;loop > 0; loop --);
}
/**
 * Get Delayed number of tick
 * \param startTick Start tick point.
 * \param endTick   End tick point.
 */
static uint32_t GetDelayInTicks(uint32_t startTick, uint32_t endTick)
{
    if (endTick > startTick) return (endTick - startTick);
    return (endTick + (0xFFFFFFFF - startTick));
}
/**
 * Display: Dump Splitting row
 */
static void DumpSeperator(void)
{
    printf("\n\r==========================================\n\r");
}


/**
 * Dump card registers
 * \param slot Card slot (not used now).
 */
static void DumpCardInfo(uint8_t iMci)
{
    sSdCard *pSd = &sdDrv[iMci];

    if (SD_GetCardType(pSd) & CARD_TYPE_bmSDIO)
    {
        SDIO_DumpCardInformation(pSd);
    }

    if (SD_GetCardType(pSd) & CARD_TYPE_bmSDMMC)
    {
        SD_DumpCID(pSd->CID);
        SD_DumpCSD(pSd->CSD);
    }
}

/**
 * Run tests on the inserted card
 * \param slot Card slot (not used now).
 */
static void CardInit(uint8_t iMci)
{
    sSdCard *pSd = &sdDrv[iMci];
    uint8_t error;

    DumpSeperator();

    error = SD_Init(pSd);
    if (error)
    {
        printf("-E- SD/MMC card initialization failed: %d\n\r", error);
        return;
    }
    else
    {
        printf("-I- SD/MMC card initialization successful\n\r");
        printf("-I- Card size: %d MB", (int32_t)SD_GetTotalSizeKB(pSd)/1000);
        printf(", %d * %dB\n\r", (int32_t)SD_GetNumberBlocks(pSd), (int32_t)SD_GetBlockSize(pSd));
    }

    DumpCardInfo(iMci);
}

/**
* Scan files under a certain path
* \param path    folder path
* return scan result, 1: success.
*/
static FRESULT scan_files (char* path)
{
    FRESULT res;
    FILINFO fno;
    DIR dir;
    int32_t i;
    char *fn;
#if _USE_LFN
    static char lfn[_MAX_LFN * (_DF1S ? 2 : 1) + 1];
    fno.lfname = lfn;
    fno.lfsize = sizeof(lfn);
#endif


    res = f_opendir(&dir, path);
    if (res == FR_OK) {
        i = strlen(path);
        for (;;) {
            res = f_readdir(&dir, &fno);
            if (res != FR_OK || fno.fname[0] == 0) break;
#if _USE_LFN
            fn = *fno.lfname ? fno.lfname : fno.fname;
#else
            fn = fno.fname;
#endif
            if (*fn == '.') continue;
            if (fno.fattrib & AM_DIR) {
                sprintf(&path[i], "/%s", fn);
                res = scan_files(path);
                if (res != FR_OK) break;
                path[i] = 0;
            } else {
                printf("%s/%s\n\r", path, fn);
            }
        }
    }

    return res;
}

/**
* Do file system tests
* \return test result, 1: success.
*/
static uint8_t RunFsTest(void)
{
    uint32_t i;
    uint32_t ByteToRead;
    uint32_t ByteRead;
#if _FS_TINY == 0
    uint32_t ByteWritten;
    char key;
#endif

    FRESULT res;
    DIR dirs;
    FATFS fs;
    FIL FileObject;
    uint32_t  tickStart, tickEnd, ticks, rwSpeed;

    printf("-I- Init media Sdcard\n\r");
    if (!MEDSdcard_Initialize(&medias[ID_DRV], &sdDrv[0])) {
        printf("-E- SD Init fail\n\r");
        return 0;
    }
    numMedias = 1;

    /** Mount disk*/
    printf("-I- Mount disk %d\n\r", ID_DRV);
    /** Clear file system object*/
    memset(&fs, 0, sizeof(FATFS));
    res = f_mount(ID_DRV, &fs);
    if( res != FR_OK ) {
        printf("-E- f_mount pb: 0x%X\n\r", res);
        return 0;
    }

    /** Test if the disk is formated*/
    res = f_opendir (&dirs,STR_ROOT_DIRECTORY);
    if(res == FR_OK ){

        /** erase sdcard to re-format it ?*/
        printf("-I- The disk is already formated.\n\r");

        /** Display the file tree*/
        printf("-I- Display files contained on the SDcard :\n\r");
        scan_files(STR_ROOT_DIRECTORY);

#if _FS_TINY == 0
        printf("-I- Do you want to erase the sdcard to re-format disk ? (y/n)!\n\r");

        key = UART_GetChar();
        if( (key == 'y') ||  (key == 'Y'))
        {
          for(i=0;i<100;i++) {
              MEDSdcard_EraseBlock(&medias[ID_DRV], i);
          }
          printf("-I- Erase the first 100 blocks complete !\n\r");
          res = FR_NO_FILESYSTEM;
        }
#endif
    }

    if( res == FR_NO_FILESYSTEM ) {

#if _FS_TINY == 0
        /** Format disk*/
        printf("-I- Format disk %d\n\r", ID_DRV);
        printf("-I- Please wait a moment during formating...\n\r");
        res = f_mkfs(ID_DRV,    // Drv
                        0,    // FDISK partition
                        512); // AllocSize
        printf("-I- Format disk finished !\n\r");
        if( res != FR_OK ) {
            printf("-E- f_mkfs pb: 0x%X\n\r", res);
            return 0;
        }
#else
        printf("-I- Please run Full version FAT FS test first\n\r");
        return 0;
      #endif
    }

#if _FS_TINY == 0
    /** Create a new file*/
    printf("-I- Create a file : \"%s\"\n\r", FileName);
    res = f_open(&FileObject, FileName, FA_CREATE_ALWAYS|FA_WRITE);
    if( res != FR_OK ) {
        printf("-E- f_open create pb: 0x%X\n\r", res);
        return 0;
    }

    /** Write a checkerboard pattern in the buffer*/
    for (i=0; i < sizeof(data); i++) {
        if ((i & 1) == 0) {
            data[i] = (i & 0x55);
        }
        else {
            data[i] = (i & 0xAA);
        }
    }
    printf("-I- Write file\n\r");
    tickStart = GetTickCount();
    for(i = 0; i< TEST_PERFORMENCT_SIZE;i+=DATA_SIZE)
   {
        res = f_write(&FileObject, data, DATA_SIZE, &ByteWritten);

           if( res != FR_OK ) {
            printf("-E- f_write pb: 0x%X\n\r", res);
            return 0;
            }
    }
    tickEnd = GetTickCount();
    ticks = GetDelayInTicks(tickStart, tickEnd);
    rwSpeed = TEST_PERFORMENCT_SIZE / ticks;
    printf("Done, Bad %u, Speed %uK\n\r", (uint32_t)0, (uint32_t)rwSpeed);
    /** Close the file*/
    printf("-I- Close file\n\r");
    res = f_close(&FileObject);
    if( res != FR_OK ) {
        printf("-E- f_close pb: 0x%X\n\r", res);
        return 0;
    }
#endif

    /** Open the file*/
    printf("-I- Open the same file : \"%s\"\n\r", FileName);
    res = f_open(&FileObject, FileName, FA_OPEN_EXISTING|FA_READ);
    if( res != FR_OK ) {
        printf("-E- f_open read pb: 0x%X\n\r", res);
        return 0;
    }
    /** Read file*/
    printf("-I- Read file\n\r");
    memset(data, 0, DATA_SIZE);
    ByteToRead = FileObject.fsize;
    tickStart = GetTickCount();
    for(i = 0; i< ByteToRead;i+=DATA_SIZE) 
   {
        res = f_read(&FileObject, data, DATA_SIZE, &ByteRead);
        if(res != FR_OK) {
            printf("-E- f_read pb: 0x%X\n\r", res);
            return 0;
        }
    }
    tickEnd = GetTickCount();
    ticks = GetDelayInTicks(tickStart, tickEnd);
    rwSpeed = ByteToRead / ticks;
    printf("Done, Bad %u, Speed %uK\n\r", (uint32_t)0, (uint32_t)rwSpeed);

    /** Close the file*/
    printf("-I- Close file\n\r");
    res = f_close(&FileObject);
    if( res != FR_OK ) {
        printf("-E- f_close pb: 0x%X\n\r", res);
        return 0;
    }

    /** compare read data with the expected data*/
    for (i=0; i < sizeof(data); i++) {
        ASSERT((((i & 1) == 0) && (data[i] == (i & 0x55)))
               || (data[i] == (i & 0xAA)),
               "Invalid data at data[%u] (expected 0x%02X, read 0x%02X)\n\r",
               i, ((i & 1) == 0) ? (i & 0x55) : (i & 0xAA), data[i]);
    }
    printf("-I- File data Ok !\n\r");

    return 1;
}
/**
 * Initialize PIOs
 */
static void _ConfigurePIOs(void)
{
    /* Configure SDcard pins */
    PIO_PinConfigure(pinsSd, PIO_LISTSIZE(pinsSd));
    /* Configure SD card detection */
    CardDetectConfigure();
    /* Check if card is write-protected (if supported) */
    CardIsProtected(0);
}

/**
 * Initialize driver instances.
 */
static void _ConfigureDrivers(void)
{
    uint32_t i;
    /* Initialize the DMA driver */
    DMAD_Initialize(&dmaDrv, 0);
    NVIC_EnableIRQ( DMAC_IRQn ) ;
    /* Initialize the HSMCI driver */
    MCID_Init(&mciDrv[0], HSMCI, ID_HSMCI, BOARD_MCK, &dmaDrv, 0 ) ;
    NVIC_EnableIRQ( HSMCI_IRQn );
    /* Initialize SD driver */
    for (i = 0; i < BOARD_NUM_MCI; i++)
    {
        SDD_InitializeSdmmcMode(&sdDrv[i], &mciDrv[i], 0);
    }
}
/**
 *  \brief Application entry point for FATFS mmc card Example
 */
int main( void )
{
     uint32_t i;
     uint8_t connected[BOARD_NUM_MCI];
    /* Disable watchdog */
    WDT_Disable( WDT ) ;

    printf("-- Basic FatFS Full Version with SDCard Project %s --\n\r",
           SOFTPACK_VERSION);
    printf("-- %s\n\r", BOARD_NAME);
    printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);

    /* Initialize system tick */
    TimeTick_Configure(BOARD_MCK);

    /* Initialize PIO pins */
    _ConfigurePIOs();
    
    /* Initialize drivers */
    _ConfigureDrivers();

    while(1) {

         /* Initialize connections */
        for (i = 0; i < BOARD_NUM_MCI; i ++)
        {
            connected[i] = 0;
        }

        /* Check if any card is inserted */
        if (!AnyCardIsConnected())
        {
            printf("-- Please insert a card\n\r");
            while(!AnyCardIsConnected());
        }

        /* Test all cards */
        for (i = 0; i < BOARD_NUM_MCI; i ++)
        {
            if (connected[i] == 0 && CardIsConnected(i))
            {
                connected[i] = 1;

                LoopDelay(BOARD_MCK/1000/200);
                CardInit(i);
            }

            if (connected[i])
            {
                 if (RunFsTest()) {
                printf("-I- Test passed !\n\r");
            }
            else {
                printf("-F- Test Failed !\n\r");
            }
          }
        }

        break;
    }

    return 0;
}
