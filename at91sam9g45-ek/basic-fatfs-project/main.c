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
/// \dir "Basic-fatfs-project"
///
/// !!!Purpose
///
/// The Basic-fatfs-project will help you to get familiar with filesystem on AT91SAM microcontrollers. 
/// It supplies sample code of common operations of a filesystem through a External RAM based filesystem.
/// It can also help you to configure the filesystem according to your own needs, which can be used
/// for fast implementation of your own filesystem and other applications related.
/// FatFs is a generic file system module to implement the FAT file system to small embedded systems.
///
/// !See
/// <a href="http://elm-chan.org/fsw/ff/00index_e.html">FATFs Website</a>
/// <a href="../../../external_libs/fat/fatfs/doc/00index_e.html"> FAT File System Module </a>
///
/// You can find following information depends on your needs:
/// - External RAM initialize sample code
/// - Supply a set of file system interface for user to do file related work, e.g. mount a disk, initialize 
///    a disk, create/open a file, write/read a file e.t.c, this can be used on different physical medias, 
///    such as External RAM, SD card. 
/// - Supply a set of disk level interface for user's reference, which is called by filesystem level
/// - Teaches user how to use these interfaces through sample code
/// - Configuration of a filesystem, e.g. user can build up a tiny filesystem which consume low memory,
///    or a filesystem with read-only functions
///
/// !!!Requirements
///
/// This package can be used with all Atmel evaluation kits that have EBI interface and External RAM device
///
/// !!!Description
///
/// Open HyperTerminal before running this program, use SAM-BA to download this program to 
/// SRAM or External RAM, make the program run, the HyperTerminal will give out the test results.
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
///    \code
///    -- Basic FatFS Full Version with SDRAM Project xxx --
///    -- AT91SAMxxx
///    -- Compiled: xxx --
///    -I- xxx init (xxx is based on physical medias)
///    -I- Mount disk 0
///    -I- Format disk 0
///    -I- Please wait a moment during formating...
///    -I- Format disk finished !
///    -I- Create a file : "0:Basic.bin"
///    -I- Write file
///    -I- ByteWritten=512
///    -I- f_write ok: ByteWritten=512
///    -I- Close file
///    -I- Open file : 0:Basic.bin
///    -I- Read file
///    -I- Close file
///    -I- File data Ok !
///    -I- Test passed !
///    \endcode
///
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// \unit
///
/// !Purpose
///
/// This file contains all the specific code for the basic-fatfs-project
///
/// !Contents
/// The basic-fatfs-project application can be roughly broken down as follows:
///    - The main function, which implements the program behavior
///       - External RAM initialize
///       - Mount a disk
///       - Initialize a disk in External RAM
///       - Create a new file 
///       - Write something to a opened file
///       - Close a file
///       - Open an existing file
///       - Read a file
///       - Close a file
///       - Compare the reading result, print the compare result to DBGU
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//         Headers
//------------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>
#include <board.h>
#include <board_memories.h>
#include <utility/trace.h>
#include <utility/assert.h>
#if defined(BOARD_SDRAM_SIZE) || defined (BOARD_DDRAM_SIZE)
#include <memories/MEDSdram.h>
#include <memories/MEDDdram.h>
#elif defined(BOARD_PSRAM_SIZE)
#include <memories/MEDRamDisk.h>
#endif

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
#define MAX_LUNS        1

/// Size of bytes in a block
#define BLOCK_SIZE      512

/// Size of the RAM Disk in bytes.
#define RAMDISK_SIZE    (1024*1024)

/// Size of buffer used for create and test a file
#define DATA_SIZE       512

#if defined(sdram) || defined(ddram) || defined(psram)
// The code is launch in sdram
#define CODE_SIZE           50*1024
#else
// Entire sdram is reserved for the FAT
#define CODE_SIZE           0
#endif

//------------------------------------------------------------------------------
//         Local variables
//------------------------------------------------------------------------------

#if _FATFS_TINY == 0
#define STR_ROOT_DIRECTORY "0:"
#else
#define STR_ROOT_DIRECTORY ""
#endif

/// Available medias.
Media medias[MAX_LUNS];

/// File name
const char* FileName = STR_ROOT_DIRECTORY "Basic.bin";

/// Buffer for create and use a file
unsigned char data[DATA_SIZE];

//------------------------------------------------------------------------------
/// main
//------------------------------------------------------------------------------
int main( void )
{
    unsigned int i;
    unsigned int ByteToRead;
    unsigned int ByteRead;
    unsigned int ByteWritten;
    FATFS fs;             // File system object
    FIL FileObject;
    FRESULT res;

    TRACE_CONFIGURE(DBGU_STANDARD, 115200, BOARD_MCK);
#if _FATFS_TINY != 1
    printf("-- Basic FatFS Full Version with External RAM Project %s --\n\r", SOFTPACK_VERSION);
#else
    printf("-- Basic FatFS Tiny Version with External RAM Project %s --\n\r", SOFTPACK_VERSION);
    printf("(Full Version has to be launched before to pass test)\n\r");
#endif
    printf("-- %s\n\r", BOARD_NAME);
    printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);

#if !defined(ddram) && defined(PINS_DDRAM)
    //configure DDRAM for use
    BOARD_ConfigureDdram(0, BOARD_DDRAM_BUSWIDTH);
#endif

    // Init Disk
#if defined(BOARD_SDRAM_SIZE)
    MEDSdram_Initialize(&(medias[0]),
                        BLOCK_SIZE,
                        (unsigned int)(AT91C_EBI_SDRAM + CODE_SIZE)/BLOCK_SIZE,
                        RAMDISK_SIZE / BLOCK_SIZE);
#elif defined (BOARD_DDRAM_SIZE)
    MEDDdram_Initialize(&(medias[0]),
                        BLOCK_SIZE,
                        (unsigned int)(AT91C_DDR2 + CODE_SIZE) / BLOCK_SIZE,
                        RAMDISK_SIZE / BLOCK_SIZE);
#elif defined (BOARD_PSRAM_SIZE)
   BOARD_ConfigurePsram();
   MEDRamDisk_Initialize(&(medias[0]),
                         BLOCK_SIZE,
                         (unsigned int)(BOARD_EBI_PSRAM + CODE_SIZE) / BLOCK_SIZE,
                         RAMDISK_SIZE / BLOCK_SIZE);

#else
    #error Pb no sdram
#endif

    numMedias = 1;

    // Mount Disk
    printf("-I- Mount disk 0\n\r");
    memset(&fs, 0, sizeof(FATFS));  // Clear file system object
    res = f_mount(0, &fs);
    if( res != FR_OK ) {
        printf("-E- f_mount pb: 0x%X (%s)\n\r", res, FF_GetStrResult(res));
        return 0;
    }

#if _FATFS_TINY != 1
    // Format disk
    printf("-I- Format disk 0\n\r");
    printf("-I- Please wait a moment during formating...\n\r");
    res = f_mkfs(0,    // Drv
                 0,    // FDISK partition
                 512); // AllocSize
    printf("-I- Format disk finished !\n\r");
    if( res != FR_OK ) {
        printf("-E- f_mkfs pb: 0x%X (%s)\n\r", res, FF_GetStrResult(res));
        return 0;
    }

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
    printf("-I- Open file : %s\n\r", FileName);
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
    ASSERT( FileObject.fsize == DATA_SIZE,  "File size value not expected!\n\r");

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
               "\n\r-F- Invalid data at data[%u] (expected 0x%02X, read 0x%02X)\n\r",
               i, ((i & 1) == 0) ? (i & 0x55) : (i & 0xAA), data[i]);
    }
    printf("-I- File data Ok !\n\r");

    printf("-I- Test passed !\n\r");

    return 0;
}

