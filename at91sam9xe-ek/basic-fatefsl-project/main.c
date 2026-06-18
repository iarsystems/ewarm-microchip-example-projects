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
/// \dir "Basic fat EFSL project"
///
/// !!!Purpose
///
/// The Basic-fatefsl-project will help you to get familiar with filesystem 
/// on AT91SAM microcontrollers.
/// This demo is based on Open Source FAT filesystem EFSL (Embedded Filesystem Library).
/// It supplies sample code of common operations of a filesystem through a External RAM
/// based filesystem.
/// It can also help you to configure the filesystem according to your own 
/// needs, which can be used for fast implementation of your own filesystem and
/// other applications related.
///
/// !See
/// <a href="http://sourceforge.net/projects/efsl/">SourceForge project of EFSL FAT</a>
/// <a href="../../../external_libs/fat/efsl/docs/manual.pdf"> EFSL Embedded Filesystem Library </a>
///
/// You can find following information depends on your needs:
/// - External RAM initialize sample code
/// - Supply a set of file system interface for user to do file related work, 
///   e.g:
///    - mount a disk
///    - initialize a disk
///    - create/open a file
///    - write/read a file
///    - etc
///   This can be used on different physical medias, such as External RAM, SD card. 
/// - Supply a set of disk level interface for user's reference, which is called
///   by filesystem level
/// - Teaches user how to use these interfaces through sample code
/// - Configuration of a filesystem
///
/// !!!Requirements
///
/// This package can be used with all Atmel evaluation kits that have EBI 
/// interface and External RAM device
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
/// -# Upon startup, the application will output the following lines on the DBGU.
///    \code
///     -- Basic Fat EFSL Project xxx --
///     -- AT91xxxxxx-xx
///     -- Compiled: xxx xx xxxx xx:xx:xx --
///     -- AT91SAMxxxx-xx
///     -- Compiled: xxxxxx --
///     -I- FAT init
///     -I- xxx init (xxx depends on different physical medias)
///     -I- External RAM already initialized
///     -I- Could not init filesystem.
///     -I- creating an empty filesystem
///     -I- Filesystem is created
///    \endcode
///
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//         Headers
//------------------------------------------------------------------------------

#include <board.h>
#include <board_memories.h>
#include <pio/pio.h>
#include <pio/pio_it.h>
#include <irq/irq.h>
#include <dbgu/dbgu.h>
#include <utility/trace.h>
#include <utility/assert.h>
#include <efsl/inc/efs.h>
#include <memories/Media.h>
#if defined(BOARD_SDRAM_SIZE)
#include <memories/MEDFlash.h>
#include <memories/MEDSdram.h>
#elif defined(BOARD_PSRAM_SIZE)
#include <memories/MEDRamDisk.h>
#endif
#include <stdio.h>
#include <string.h>
#include "mkfs.h"
#include "ls.h"

//------------------------------------------------------------------------------
//         Local constants
//------------------------------------------------------------------------------
#define MAXDIRENTRIES 1

/// Maximum number of LUNs which can be defined.
#define MAX_LUNS        1

//------------------------------------------------------------------------------
//         Local variables
//------------------------------------------------------------------------------
/// Available medias.
Media medias[MAX_LUNS];

//------------------------------------------------------------------------------
/// main
/// Fat EFSL
//------------------------------------------------------------------------------
int main(void)
{
    EmbeddedFileSystem efs;
    EmbeddedFile file_r;
    EmbeddedFile file_w;
    unsigned short e;
    char buf[512];
    unsigned int i;
    unsigned int status;
    DirList list;

    TRACE_CONFIGURE(DBGU_STANDARD, 115200, BOARD_MCK);
    printf("-- Basic Fat EFSL Project %s --\n\r", SOFTPACK_VERSION);
    printf("-- %s\n\r", BOARD_NAME);
    printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);

    printf("-I- FAT init\n\r");
    if( efs_init(&efs, 0) != 0 ){
        printf("-I- Could not init filesystem.\n\r");
        fs_initCurrentDir(&efs.myFs); 
        printf("-I- creating an empty filesystem\n\r");
        if( mkfs_makevfat(&efs.myPart) == 0 ) {
            printf("-I- Filesystem is created\n\r");
        }
        else {
            printf("-E- Problem for creating filesystem\n\r");
        }
    }

    printf("-I- creating a file\n\r");
    for( i=0; i<512; i++) {
        buf[i]=i;
    }
    if( file_fopen( &file_w, &efs.myFs, "orig.txt", 'a' ) != 0 ) {
        if (file_fopen( &file_w, &efs.myFs, "orig.txt", 'w' ) != 0) {
            printf("-E- Could not open file orig.txt\n\r");
            while(1);
        }
    }
    status = file_fwrite(&file_w, 0, 512, buf);
    printf("-I- Bytes actually written = %d\n\r", status);

    printf("-I- Copy the file\n\r");
    if( file_fopen( &file_r, &efs.myFs, "orig.txt", 'r' ) != 0 ) {
        printf("-E- Could not open file orig.txt\n\r");
        while(1);
    }

    if( file_fopen( &file_w, &efs.myFs, "copy.txt", 'a' ) != 0 ) {
        if (file_fopen( &file_w, &efs.myFs, "copy.txt", 'w' ) != 0) {
            printf("-E- Could not open file copy.txt\n\r");
            while(1);
        }
    }

    e = 0x1;
    while( e != 0 ) {
        e = file_read( &file_r, 512, (euint8*)buf );
        file_write( &file_w, e, (euint8*)buf );
    }

    file_fclose(&file_r);
    file_fclose(&file_w);

    if ( file_fopen( &file_r, &efs.myFs, "copy.txt", 'r' ) != 0) {
        printf("-E- Could not open file copy.txt for verify\n\r");
        while(1);
    }

    printf("-I- Veriry the file\n\r");
    e = 0x01;
    while( e != 0) {
        e = file_read( &file_r, 512, (euint8*)buf );
    }
    for(i = 0; i < 512; i++) {
        if (buf[i] != (unsigned char)i) {
            printf("-E- Verify fail at buffer[%d]", i);
            while(1);
        }
    }
    file_fclose(&file_r);

    fs_umount(&efs.myFs);

    printf("End test\n\r");

    return 0;
}


