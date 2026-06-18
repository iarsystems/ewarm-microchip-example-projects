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
/// \dir "demo-multimedia-project"
///
/// !!!Purpose
///
/// The demo-multimedia-project concatenates several basic projects to create 
/// a multimedia application. This application uses the touchscreen driver,
/// the LCD driver, the MCI/SD Card driver and a FAT file system.
/// 
/// !See
/// - tsd : touchscreen controller driver
/// - lcd: LCD controller driver
/// - sdmmc_mci : sdcard physical layer driver with mci-interface
/// - mci : sdcard physical layer driver with mci-interface
/// - <a href="http://elm-chan.org/fsw/ff/00index_e.html">FATFs Website</a>
/// - <a href="../../../external_libs/fat/fatfs/doc/00index_e.html"> FAT File System Module </a>
///
/// !!!Requirements
///
/// This package can be used with all Atmel evaluation kits that have MCI interface, the 
/// package runs in external RAM, so EBI interface and external RAM device is needed.
///
/// !!!Description
/// The application reads images from a SD Card and displays them on the LCD screen.
/// The current displayed image is changed thanks to a stylus movement on the touch screen.
/// A movement on the left displays the previous picture. A movement on the rigth
/// displays the following image. Vertical movement has no effect. Horizontal movement has
/// to be enough long to be taken into account.
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
///     -- Demo Multimedia Project xxx --
///     -- AT91xxxxxx-xx
///     -- Compiled: xxx xx xxxx xx:xx:xx --
///     -I- MEDSdcard init
///     -I- Please connect a SD card ...
///     \endcode
/// -# Plug in the rigth connector a SD Card. Pictures files contained in the 
///    project have to be previously copied in the root directory of the SD card.
/// -# Do the Touchscreen calibation
/// -# a first image is displayed. To change it, do a movement on the left or
///    on the right with a stylus on the touchscreen.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// \unit
///
/// !Purpose
///
/// This file contains all the specific code for the demo-multimedia-project
///
/// !Contents
/// The demo-multimedia-project application can be roughly broken down as follows:
///    - SD Card initialization (MCI configuration + FAT file system)
///    - LCD initialization
///    - Touchscreen initialization and calibration
///    - Read the list of the pictures contained in the root directory of the SD Card
///    - Display the first image of the list.
///    - In interrupt, the events Pen Pressed and Pen Released are managed thanks 
///      to the callbacks functions TSD_PenPressed and TSD_PenReleased 
///      (called in interrupt). These callback modify the global variable imageToDisplay.
///    - In the main infinite loop, the current image index is compared with the value 
///      of imageToDisplay. If different, the new picture is displayed on LCD screen.

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//         Headers
//------------------------------------------------------------------------------

#include <board.h>
#include <board_memories.h>
#include <pio/pio.h>
#include <dbgu/dbgu.h>
#include <utility/assert.h>
#include <utility/bmp.h>
#include <utility/math.h>
#include <utility/trace.h>
#include <drivers/lcd/lcdd.h>
#include <drivers/lcd/draw.h>
#include <drivers/lcd/color.h>
#include <drivers/tsd/tsd.h>
#include <memories/MEDSdcard.h>
#include <fatfs/src/tff.h>
#include <fatfs/src/ff_util.h>

#include <stdio.h>
#include <string.h>

//------------------------------------------------------------------------------
//         Local definitions
//------------------------------------------------------------------------------

#if defined(at91sam9rlek)
#define MCI_ID 0
#elif defined(at91cap9dk)
#define MCI_ID 1 //MCIO/SPI0 is used for SPI0 (Touch screen)
#elif defined(at91cap9stk)
#define MCI_ID 1 //no connector for MCIO/SPI0
#else
#error Not yet supported
#endif

#if _FATFS_TINY == 0
#define STR_ROOT_DIRECTORY "0:"
#else
#define STR_ROOT_DIRECTORY ""
#endif

/// Maximum number of images that can be displayed.
#define MAX_IMAGES             20

// Minimum variation on X to display an another image
#define X_DIFF_MIN  50
// Maximum variation on Y to display an another image
#define Y_DIFF_MAX  30

//------------------------------------------------------------------------------
//         Local variables
//------------------------------------------------------------------------------

/// LCD & file buffers (forced into SDRAM).
static unsigned char *pLcdBuffer = (unsigned char *) (AT91C_EBI_SDRAM + 0x00100000);
static unsigned char *pFileBuffer  = (unsigned char *) (AT91C_EBI_SDRAM + 0x00200000);

/// List of medias that can be used by the FAT filesystem.
Media medias[1];

/// Fat filesystem structure.
FATFS fs;

/// Image filenames.
static FILINFO images[MAX_IMAGES];

/// Number of images that have been found on the SDcard.
static unsigned int nbImages = 0;

/// Index of image being currently displayed.
static volatile unsigned int currentImage = 0;
/// Index of image to display
static volatile unsigned int imageToDisplay = 0;

/// Coordinates at which the pen was last pressed.
static volatile unsigned int xPressed;
static volatile unsigned int yPressed;

//------------------------------------------------------------------------------
//         Local functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Displays the given image on the LCD.
/// \param image  Image index in images[] array.
//------------------------------------------------------------------------------
static void DisplayImage(unsigned int index)
{
    FIL file;
    FILINFO fileinfo = images[index];
    unsigned int width, height;
  
    printf("-I- Displaying image %s\n\r", fileinfo.fname);
  
    // Open image file to read its contents
    ASSERT(!f_open(&file, fileinfo.fname, FA_OPEN_EXISTING | FA_READ),
           "-F- Could not open image file %s\n\r", fileinfo.fname);
    ASSERT(!f_read(&file, pFileBuffer, fileinfo.fsize, 0),
           "-F- Could not read image file %s\n\r", fileinfo.fname);
    f_close(&file);
  
    // Decode image in display buffer
    BMP_Decode(pFileBuffer, pLcdBuffer, BOARD_LCD_WIDTH, BOARD_LCD_HEIGHT, 24);
    
    // Draw image name at top of screen
    LCDD_GetStringSize(fileinfo.fname, &width, &height);
    LCDD_DrawRectangle(pLcdBuffer, (BOARD_LCD_WIDTH - (width + 2)) / 2,
                       299, width+2, height+2, 0xFFFFFF);
    LCDD_DrawString(pLcdBuffer, (BOARD_LCD_WIDTH - width) / 2, 300,
                    fileinfo.fname, 0x000000);
}

//------------------------------------------------------------------------------
/// Looks for images at the root of the SDcard and stores them in an array for
/// further use.
//------------------------------------------------------------------------------
static void GetImageList(void)
{
    struct BMPHeader header;
    FILINFO fileinfo;
    DIR dir;
    FIL file;

    // Open all files to see which ones are compatible BMP
    ASSERT(!f_opendir(&dir, STR_ROOT_DIRECTORY), "-F- Could not open SDCard root directory\n\r");
    
    while (!f_readdir(&dir, &fileinfo)) {
    
        if (!fileinfo.fname[0]) break; // End of listing
    
        // Do not open directories
        if (fileinfo.fattrib & AM_DIR) {  
            printf("-I- Skipping %s (directory)\n\r", fileinfo.fname);
            continue;
        }
    
        // Open file
        printf("-I- Opening %s\n\r", fileinfo.fname);
        ASSERT(!f_open(&file, fileinfo.fname, FA_OPEN_EXISTING | FA_READ),
               "-F- Could not open %s\n\r", fileinfo.fname);
    
        // Read start of file to see if it is a BMP header
        printf("-I- Reading %s\n\r", fileinfo.fname);
        ASSERT(!f_read(&file, &header, sizeof(header), 0),
               "-F- Could not read %s\n\r", fileinfo.fname);
    
        // Check if header is valid and the BMP is supported
        if (!BMP_Decode(&header, 0, BOARD_LCD_WIDTH, BOARD_LCD_HEIGHT, 24)) {
          
            printf("-I- %s is a supported BMP file, adding it to the list\n\r", fileinfo.fname);
            images[nbImages] = fileinfo;
            //memcpy(&images[nbImages], &fileinfo, sizeof(fileinfo));
            nbImages++;
      
            // Get out of loop if the max number of images has been reached
            if (nbImages == MAX_IMAGES) {
                printf("-I- Maximum number of images reached\n\r");
                break;
            }
        }
        else printf("-I- %s is not a BMP or is not supported\n\r", fileinfo.fname);
    
        // Close file
        printf("-I- Closing %s\n\r", fileinfo.fname);
        f_close(&file);
    }
}

//------------------------------------------------------------------------------
//         Global functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Callback invoked by the touchscreen driver whenever the pen is pressed on
/// the screen.
/// \param x  X-coordinate of point where pen press occured.
/// \param y  Y-coordinate of point where pen press occured.
//------------------------------------------------------------------------------
void TSD_PenPressed(unsigned int x, unsigned int y)
{
    printf("-I- Pen pressed at (%u, %u)\n\r", x, y);
  
    // Save the coordinates for use by TSD_PenReleased
    xPressed = x;
    yPressed = y;
}

//------------------------------------------------------------------------------
/// Callback invoked by the touchscreen driver whenever the pen is removed from
/// the screen.
/// \param x  X-coordinate of point where pen release occured.
/// \param y  Y-coordinate of point where pen release occured.
//------------------------------------------------------------------------------
void TSD_PenReleased(unsigned int x, unsigned int y)
{
    unsigned int xDiff = absv((signed int) x - (signed int) xPressed);
    unsigned int yDiff = absv((signed int) y - (signed int) yPressed);
  
    printf("-I- Pen released at (%u, %u)\n\r", x, y);
    printf("-I- xDiff %d yDiff %d\n\r", xDiff, yDiff);
    
    // Require a big enough variation in X, not much in Y (this should detect
    // horizontal lines)
    if ((xDiff >= X_DIFF_MIN) && (yDiff <= Y_DIFF_MAX)) {

        // Right (next image) or left (previous image) ?
        if (xPressed < x) {
            // Display the next image
            imageToDisplay = (currentImage + 1) % nbImages;
        } else {
            // Display the previous image          
            imageToDisplay = (currentImage + nbImages - 1) % nbImages;
        }
    }
}

// Not used, required by touchscreen driver
void TSD_PenMoved(unsigned int x, unsigned int y) {}

//------------------------------------------------------------------------------
/// Default main() function. Initializes the DBGU and writes a string on the
/// DBGU.
//------------------------------------------------------------------------------
int main(void)
{
    TRACE_CONFIGURE(DBGU_STANDARD, 115200, BOARD_MCK);
    printf("-- Demo Multimedia Project %s --\n\r", SOFTPACK_VERSION);
    printf("-- %s\n\r", BOARD_NAME);
    printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);

    // Initialize filesystem
    MEDSdcard_Initialize(&medias[DRV_MMC], MCI_ID); 
    memset(&fs, 0, sizeof(fs));
    ASSERT(!f_mount(DRV_MMC, &fs), "-F- Could not mount filesystem on SDCard\n\r");
  
    // Initialize LCD
    LCDD_Fill(pLcdBuffer, COLOR_BLACK);
    LCDD_Initialize();
    LCDD_DisplayBuffer(pLcdBuffer);
  
    // Initialize touchscreen
    TSD_Initialize(pLcdBuffer);
        
    // Populate image list, check that at least one image was found and display it   
    GetImageList();
    ASSERT(nbImages > 0, "-F- No image found\n\r");
    
    // Display the first image
    imageToDisplay = 0;
    currentImage = imageToDisplay;
    DisplayImage(currentImage);

    // Infinite loop
    while (1) {
        if(currentImage != imageToDisplay) {
            currentImage = imageToDisplay;
            DisplayImage(currentImage);      
        }
    }
}
