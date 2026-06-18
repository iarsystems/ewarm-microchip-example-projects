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
/// \dir "Basic-fatfs-SDCARD-project"
///
/// !!!Purpose
///
/// The Basic-fatfs-sdcard-project is the concatenation of 2 basic projects, 
/// basic-fatfs-project and basic-sdcard-project.
/// It will help you to get familiar with filesystem and sdmmc_mci interface on
/// AT91 microcontrollers. 
/// It supplies sample code of common operations of a filesystem through a SD
/// Card based filesystem.
/// It can also help you to configure the filesystem according to your own
/// needs, which can be used for fast implementation of your own filesystem and
/// other applications related.
/// FatFs is a generic file system module to implement the FAT file system to
/// small embedded systems.
///
/// !See
/// - <a href="http://elm-chan.org/fsw/ff/00index_e.html">FATFs Website</a>
/// - <a href="../../../external_libs/fat/fatfs/doc/00index_e.html">
///   FAT File System Module </a>
///
/// You can find following information depends on your needs:
/// - Supply a set of file system interface for user to do file related work,
///   e.g. mount a disk, initialize a disk, create/open a file, write/read a
///   file e.t.c.
/// - Supply a set of disk level interface for user's reference, which is called
///   by filesystem level
/// - Teaches user how to use these interfaces through sample code
/// - Configuration of a filesystem, e.g. user can build up a tiny filesystem
///   which consume low memory, or a filesystem with read-only functions.
///   Configuration is set in the fatfs_config.h file.
///
/// !!!Requirements
///
/// This package can be used with all Atmel evaluation kits that have MCI
/// interface, the package runs at SRAM or external RAM, so EBI interface and
/// external RAM device is needed if you want to run this package in external
/// RAM
///
/// !!!Description
///
/// When launched, this program asks user to plug a SD card in the MCI
/// connector.
/// Once the SD card is plugged, there are 2 cases. 
/// - The SD card is not yet formated, the program formats it with FAT file
///   system. 
/// - The SD card is already formated. All the files contained on the SD Card
///   are displayed. Then, depending on the user answer, the SD card is erased
///   and re-formated or nothing is done.
///
/// Finally a Basic.bin file is created and filled with a special pattern. Then
/// the file is closed, reopened and its data are verified.
///
/// For the FATFS TINY project version, copy previously the Basic.bin file on
/// the SD Card in the root directory.
/// This file can be found in the project.
/// The FAT system is compiled with only the reading functions (see _FS_READONLY
/// option in fatfs_config.h). 
/// This option can be modified.
/// 
/// !!!Note
///
/// The project basic-sdcard-project can be used to "unformat" the SD card. By
/// writing dummy values in the first blocks, the FAT file system data are
/// erased.
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
///     -- Basic FatFS Full Version with SDCard Project xxx --
///     -- AT91xxxxxx-xx
///     -- Compiled: xxx xx xxxx xx:xx:xx --
///     -I- Init media Sdcard
///     -I- MEDSdcard init
///     -I- Please connect a SD card ...
///     \endcode
///
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// \unit
///
/// !Purpose
///
/// This file contains all the specific code for the basic-fatfs-sdcard-project
///
/// !Contents
/// The basic-fatfs-sdcard-project application can be roughly broken down as follows:
///    - The main function, which implements the program behavior
///       - SD Card interface initialize
///       - Mount a disk on SD card
///       - Test if the SD Card is already formated
///       - Display all the files contained on the SD card if already formated
///       - Format SD Card if not yet formated
///       - Create a new file
///       - Write something to a opened file
///       - Close a file
///       - Open an existing file
///       - Read a file
///       - Close a file
///       - Compare the reading result, print the compare result to DBGU
//-----------------------------------------------------------------------------

//------------------------------------------------------------------------------
//         Headers
//------------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>
#include <board.h>
#include <utility/trace.h>
#include <utility/assert.h>
#include <utility/math.h>
#include <memories/MEDSdcard.h>

#include "fatfs_config.h"
#if _FATFS_TINY != 1
#include <fatfs/src/ff.h>
#else
#include <fatfs/src/tff.h>
#endif
#include <fatfs/src/ff_util.h>

//------------------------------------------------------------------------------
//         Local constants
//------------------------------------------------------------------------------

/// Maximum number of LUNs which can be defined.
/// (Logical drive = physical drive = medium number)
#define MAX_LUNS        1

/// Available medias.
Media medias[MAX_LUNS];

//------------------------------------------------------------------------------
//         Local variables
//------------------------------------------------------------------------------

#define ID_DRV DRV_MMC

#if _FATFS_TINY == 0
#define STR_ROOT_DIRECTORY "0:"
#else
#define STR_ROOT_DIRECTORY ""
#endif

#if defined(at91cap9stk)
#define MCI_ID 1 //no connector for MCIO/SPI0
#else
#define MCI_ID 0
#endif

const char* FileName = STR_ROOT_DIRECTORY "Basic.bin";

#if 0
//Bigger than this value will cause SD HC error now.
//#define DATA_SIZE (7*1024 + 512)
#define DATA_SIZE (9*1024)
//unsigned char data[DATA_SIZE];
unsigned char * data = (unsigned char*)0x60000400;
#else
#define DATA_SIZE 2064 // size of the file to write/read
                       // !!!minimum size 512 for erase operation !!!

unsigned char data[DATA_SIZE];
#endif

//------------------------------------------------------------------------------
//         Local functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Do file system tests
/// \return test result, 1: success.
//------------------------------------------------------------------------------
static unsigned char RunFsTest(void)
{
    unsigned int i;
    unsigned int ByteToRead;
    unsigned int ByteRead;
  #if _FATFS_TINY == 0
    unsigned int ByteWritten;
    char key;
  #endif

    FRESULT res;
    DIR dirs;
    FATFS fs;             // File system object
    FIL FileObject;

    // Init Disk
    printf("-I- Please connect a SD card ...\n\r");
    while(!MEDSdcard_Detect(&medias[ID_DRV], MCI_ID));
    printf("-I- SD card connection detected\n\r");

    printf("-I- Init media Sdcard\n\r");
    if (!MEDSdcard_Initialize(&medias[ID_DRV], MCI_ID)) {
        printf("-E- SD Init fail\n\r");
        return 0;
    }
    numMedias = 1;

    // Mount disk
    printf("-I- Mount disk %d\n\r", ID_DRV);
    memset(&fs, 0, sizeof(FATFS));      // Clear file system object
    res = f_mount(ID_DRV, &fs);
    if( res != FR_OK ) {
        printf("-E- f_mount pb: 0x%X (%s)\n\r", res, FF_GetStrResult(res));
        return 0;
    }

    // Test if the disk is formated
    res = f_opendir (&dirs,STR_ROOT_DIRECTORY);
    if(res == FR_OK ){

        // erase sdcard to re-format it ?
        printf("-I- The disk is already formated.\n\r");

        // Display the file tree
        printf("-I- Display files contained on the SDcard :\n\r");
        FF_ScanDir(STR_ROOT_DIRECTORY);

      #if _FATFS_TINY == 0
        printf("-I- Do you want to erase the sdcard to re-format disk ? (y/n)!\n\r");

        key = DBGU_GetChar();
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

      #if _FATFS_TINY == 0
        //printf("-I- Press 'y' to start format:\n\r");
        //if (DBGU_GetChar() != 'y') return 0;
        // Format disk
        printf("-I- Format disk %d\n\r", ID_DRV);
        printf("-I- Please wait a moment during formating...\n\r");
        res = f_mkfs(ID_DRV,    // Drv
                        0,    // FDISK partition
                        512); // AllocSize
        printf("-I- Format disk finished !\n\r");
        if( res != FR_OK ) {
            printf("-E- f_mkfs pb: 0x%X (%s)\n\r", res, FF_GetStrResult(res));
            return 0;
        }
      #else
        printf("-I- Please run Full version FAT FS test first\n\r");
        return 0;
      #endif
    }

  #if _FATFS_TINY == 0
    // Create a new file
    printf("-I- Create a file : \"%s\"\n\r", FileName);
    res = f_open(&FileObject, FileName, FA_CREATE_ALWAYS|FA_WRITE);
    if( res != FR_OK ) {
        printf("-E- f_open create pb: 0x%X (%s)\n\r", res, FF_GetStrResult(res));
        return 0;
    }

    // Write a checkerboard pattern in the buffer
    for (i=0; i < sizeof(data); i++) {
        if ((i & 1) == 0) {
            data[i] = (i & 0x55);
        }
        else {
            data[i] = (i & 0xAA);
        }
    }
    printf("-I- Write file\n\r");
    res = f_write(&FileObject, data, DATA_SIZE, &ByteWritten);
    printf("-I- ByteWritten=%d\n\r", (int)ByteWritten);
    if( res != FR_OK ) {
        printf("-E- f_write pb: 0x%X (%s)\n\r", res, FF_GetStrResult(res));
        return 0;
    }
    else {
        printf("-I- f_write ok: ByteWritten=%d\n\r", (int)ByteWritten);
    }

    // Close the file
    printf("-I- Close file\n\r");
    res = f_close(&FileObject);
    if( res != FR_OK ) {
        printf("-E- f_close pb: 0x%X (%s)\n\r", res, FF_GetStrResult(res));
        return 0;
    }
  #endif

    // Open the file
    printf("-I- Open the same file : \"%s\"\n\r", FileName);
    res = f_open(&FileObject, FileName, FA_OPEN_EXISTING|FA_READ);
    if( res != FR_OK ) {
        printf("-E- f_open read pb: 0x%X (%s)\n\r", res, FF_GetStrResult(res));
        return 0;
    }
    ASSERT( FileObject.fsize == DATA_SIZE,  "File size value not expected!\n\r");

    // Read file
    printf("-I- Read file\n\r");
    memset(data, 0, DATA_SIZE);
    ByteToRead = FileObject.fsize;
    res = f_read(&FileObject, data, ByteToRead, &ByteRead);
    if(res != FR_OK) {
        printf("-E- f_read pb: 0x%X (%s)\n\r", res, FF_GetStrResult(res));
        return 0;
    }

    // Close the file
    printf("-I- Close file\n\r");
    res = f_close(&FileObject);
    if( res != FR_OK ) {
        printf("-E- f_close pb: 0x%X (%s)\n\r", res, FF_GetStrResult(res));
        return 0;
    }

    // compare read data with the expected data
    for (i=0; i < sizeof(data); i++) {
        ASSERT((((i & 1) == 0) && (data[i] == (i & 0x55)))
               || (data[i] == (i & 0xAA)),
               "Invalid data at data[%u] (expected 0x%02X, read 0x%02X)\n\r",
               i, ((i & 1) == 0) ? (i & 0x55) : (i & 0xAA), data[i]);
    }
    printf("-I- File data Ok !\n\r");

    return 1;
}

//------------------------------------------------------------------------------
//         Global functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// main
/// FatFs Full version
//------------------------------------------------------------------------------
int main( void )
{
    TRACE_CONFIGURE(DBGU_STANDARD, 115200, BOARD_MCK);
    printf("-- Basic FatFS Full Version with SDCard Project %s --\n\r",
           SOFTPACK_VERSION);
    printf("-- %s\n\r", BOARD_NAME);
    printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);

    while(1) {
        if (RunFsTest()) {
            printf("-I- Test passed !\n\r");
        }
        else {
            printf("-F- Test Failed !\n\r");
        }
        SD_Stop(MEDSdcard_GetDriver(MCI_ID),
                MEDSdcard_GetDriver(MCI_ID)->pSdDriver);

        printf("\n\r** any key to start test again **\n\r");
        DBGU_GetChar();
    }

    return 0;
}
