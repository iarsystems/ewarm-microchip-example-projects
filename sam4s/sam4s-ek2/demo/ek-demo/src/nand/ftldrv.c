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

#include <string.h>
#include <stdio.h>
#include <assert.h>

/* Include FreeRTOS API */
#include "libsam_gui.h"

/* Include AT91SAM3S-EK definitions and initialization API */
#include "board.h"

/* Atmel NAND FTL library API. */
#include "memories.h"

/* Atmel Freetos wrapper for FTL layers */
#include "rtmedia.h"
#include "ftldrv.h"

#include "fatfs_config.h"

/*-----------------------------------------------------------*/
/*             Application specific                          */
/** Size of the Nand Logical unit */
#define NF_LUN_SIZE 128 * 1024 * 1024

/** Physical offset of the Nand Logical Unit */
#define NF_LUN_BASE  0

/** FTL_FLUSH_RATE defines the period after which all temporary structures
  * shall be stored in the nand */
#define FTL_FLUSH_RATE 1000
/*-----------------------------------------------------------*/


/** RtMedia object for Nand */
RtMedia gRtNandMedias;

/* File system objects */
static FATFS fs;
static DIR dirs;

/// Pins used to access to nandflash.
static const Pin pPinsNf[] = {PINS_NANDFLASH};
/// Nandflash device structure.
static struct TranslatedNandFlash translatedNf;
/// Nandflash chip enable pin.
static const Pin nfCePin = BOARD_NF_CE_PIN;
/// Nandflash ready/busy pin.
static const Pin nfRbPin = BOARD_NF_RB_PIN;

#if _FS_TINY == 0
#define STR_ROOT_DIRECTORY "0:"
#else
#define STR_ROOT_DIRECTORY ""
#endif

/** NandFlashInitialize initialise the controller HW and the FTL objects
  * required before using the RtMedia object interface */
int NandFlashInitialize( void )
{
    uint8_t nfRc ;
    struct RawNandFlash *pRaw = (struct RawNandFlash*)&translatedNf;
    struct NandFlashModel *pModel = (struct NandFlashModel*)&translatedNf;
    unsigned int block;

    // Configure SMC for NandFlash
    BOARD_ConfigureNandFlash(SMC) ;

    // Configure PIO for Nand Flash
    PIO_Configure(pPinsNf, PIO_LISTSIZE(pPinsNf));

    /* Retrieve nandflash parameters */
    nfRc = RawNandFlash_Initialize(pRaw,
                                   0,
                                   BOARD_NF_COMMAND_ADDR,
                                   BOARD_NF_ADDRESS_ADDR,
                                   BOARD_NF_DATA_ADDR,
                                   nfCePin,
                                   nfRbPin);
    if ( nfRc )
    {
        printf("Nand not found\n\r");
        return 1;
    }
    else
    {
        printf("NF\tNb Blocks %d\n\r", NandFlashModel_GetDeviceSizeInBlocks(pModel));
        printf("\tBlock Size %dK\n\r", NandFlashModel_GetBlockSizeInBytes(pModel)/1024);
        printf("\tPage Size %d\n\r", NandFlashModel_GetPageDataSize(pModel));
    }

    /* FTL stack initialization */
    if (TranslatedNandFlash_Initialize(&translatedNf,
                                       0,
                                   BOARD_NF_COMMAND_ADDR,
                                   BOARD_NF_ADDRESS_ADDR,
                                   BOARD_NF_DATA_ADDR,
                                   nfCePin,
                                   nfRbPin,
                                   NF_LUN_BASE, NF_LUN_SIZE/NandFlashModel_GetBlockSizeInBytes(pModel)/*NandFlashModel_GetDeviceSizeInBlocks(pModel)*/))
    {
        printf("Nand init error\n\r");
        printf("Erase All to force format\n\r");
        for ( block = 0 ; block < NandFlashModel_GetDeviceSizeInBlocks(pModel) ; block++ )
        {
            RawNandFlash_EraseBlock(pRaw, block);
        }

        if (TranslatedNandFlash_Initialize(&translatedNf,
                                           0,
                                       BOARD_NF_COMMAND_ADDR,
                                       BOARD_NF_ADDRESS_ADDR,
                                       BOARD_NF_DATA_ADDR,
                                       nfCePin,
                                       nfRbPin,
                                       NF_LUN_BASE, NF_LUN_SIZE/NandFlashModel_GetBlockSizeInBytes(pModel)/*NandFlashModel_GetDeviceSizeInBlocks(pModel)*/))
        {
            printf("Nand init fatal error\n\r");
            return 1;
        }
    }

    /* Media object initialization with Translated NandFlash api */
    MEDNandFlash_Initialize(&(gRtNandMedias.media), &translatedNf);

    /* RtMedia object initialization */
    RTMEDIA_Init(&gRtNandMedias);

   // Mount Disk

    FRESULT res;

    printf("-I- Mount disk 0\n\r");
    memset(&fs, 0, sizeof(FATFS));  // Clear file system object
    res = f_mount(0, &fs);

    if( res != FR_OK )
    {
        printf("-E- f_mount pb\n\r");
        return 1;
    }
    // Test if the disk is formated
    res = f_opendir (&dirs,STR_ROOT_DIRECTORY);
    if(res == FR_OK )
    {
        printf("-I- The disk is already formated.\n\r");
    }

    // Format disk
    if (res == FR_NO_FILESYSTEM)
    {
        printf("-I- Format disk 0\n\r");
        printf("-I- Please wait a moment during formating...\n\r");
        res = f_mkfs(0,    // Drv
                     0,    // FDISK partition
                     512); // AllocSize
        printf("-I- Format disk finished !\n\r");
        RTMEDIA_Flush(&(gRtNandMedias));
        if( res != FR_OK )
        {
            printf("-E- f_mkfs 0x%Xpb\n\r", res);
            return 1; //0;
        }
    }

    return 0;
}

/**
 * \brief FTL task.
 * This task periodically calls MEDNandFlash_Flush() to store temporary
 * objects in the Nandflash each FTL_FLUSH_RATE ms.
 */
void TaskFtl( void* pParameter )
{
    for ( ; ; )
    {
//        printf("-I- TaskFTL flush\n\r");
        RTMEDIA_Flush(&(gRtNandMedias));
        SAMGUI_TaskDelay( FTL_FLUSH_RATE ) ;
    }
}

static const char* gTestFileName = STR_ROOT_DIRECTORY "Basic.bin";
#define MAX_FILE_SIZE 500 * 1024

void TaskFtlTestWrite ( void* pParameter )
{
    char data[100];
    FIL FileObject;
    unsigned int ByteWritten;
    FRESULT res;

    int fileLength = 10, i=0;

    for ( ; ; ) {
       // Create a new file
        printf("-I- Create a file : \"%s\" of %ld bytes\n\r", gTestFileName, fileLength);
        res = f_open(&FileObject, gTestFileName, FA_CREATE_ALWAYS|FA_WRITE);
        if( res != FR_OK ) {
            printf("-E- f_open create pb: 0x%X \n\r", res);
            return ; //0;
        }
        // Write a  pattern in the buffer
        i = 0;
        while (i < fileLength) {
            data[i%sizeof(data)] = i;
            if ((++i % sizeof(data)) == 0) {
                res = f_write(&FileObject, data, sizeof(data), &ByteWritten);
                if( res != FR_OK ) {
                    printf("-E- f_write pb: 0x%X\n\r", res);
                    return ; //0;
                }
            }
        }
        if ((i % sizeof(data)) > 0) {
                res = f_write(&FileObject, data, i % sizeof(data), &ByteWritten);
                if( res != FR_OK ) {
                    printf("-E- f_write pb: 0x%X\n\r", res);
                    return ; //0;
                }
        }
        res = f_close(&FileObject);
        if( res != FR_OK ) {
            printf("-E- f_close pb: 0x%X\n\r", res);
            return ; //0;
        }

        SAMGUI_TaskDelay( 100 ) ;

        fileLength += 100;
        if (fileLength > MAX_FILE_SIZE)
          fileLength = 100;
     }
}

void TaskFtlTestRead ( void* pParameter )
{
    char data[100];
    FIL FileObject;
    unsigned int byteRead, i, j;
    FRESULT res;

    for ( ; ; ) {
       // Create a new file
        res = f_open(&FileObject, gTestFileName, FA_OPEN_EXISTING|FA_READ);
        if( res != FR_OK ) {
            printf("-E- f_open create pb: 0x%X \n\r", res);
        }
        else {
            i = 0;
            do {
                res = f_read(&FileObject, data, sizeof(data), &byteRead);
                if( res != FR_OK ) {
                    printf("-E- f_write pb: 0x%X\n\r", res);
                    return ; //0;
                }
                for (j = 0; j < byteRead; ++j) {
                    if (data[j] != (unsigned char)(i++)) {
                        printf("-E- Wrong value\n\r");
                    }
                }
            } while (byteRead);
            res = f_close(&FileObject);
            if( res != FR_OK ) {
                printf("-E- f_close pb: 0x%X\n\r", res);
                return ; //0;
            }
            printf("-I- Read a file : \"%s\" of %ld bytes\n\r", gTestFileName, i);
        }
        SAMGUI_TaskDelay( 100 ) ;
     }
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
