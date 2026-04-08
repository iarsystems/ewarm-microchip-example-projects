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


//-----------------------------------------------------------------------------
/// \dir "Basic SSC-I2S WM8731  project"
///
/// !!!Purpose
///
///  This example uses the Synchronous Serial Controller (SSC) of an AT91 microcontroller 
///  to output an audio steam through the on-board WM8731 CODEC. 
/// 
///
/// !!!See
/// - ssc: SSC driver interface
///
/// !!!Description
/// 
/// This program plays a WAV file pre-loaded into the SDcard. The audio stream is sent through 
/// the SSC interface connected to the on-board WM8731, enabling the sound to be audible using a pair of headphones. 
///
/// Since the WM8731 DAC requires that it be feeded a master clock multiple of the sample rate, 
/// it is difficult to handle any WAV file. As such, this example application is limited to playing files with the following format: 
///  - Format: WAV 
///  - Sample rate: 48 kHz 
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
/// -# Start the application.
/// -# In the terminal window, the following text should appear:
///    \code
///     -- Basic SSC I2S WM8731 Project xxx --
///     -- AT91xxxxxx-xx
///     -- Compiled: xxx xx xxxx xx:xx:xx --
///     Menu :
///     ------
///     W: Play the WAV file pre-loaded in SD Card
///     I: Display the information of the WAV file
///    \endcode
/// The user can then choose any of the available options to perform the described action. 
///
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/// \unit
///
/// !Purpose
///
/// This file contains all the specific code for the
/// basic-ssc-i2s-wm8731-project
///
/// !Contents
///
/// The code can be roughly broken down as follows:
///    - Enable the clock 
///    - Load WAV file information
///    - Configure and enable the Codec
///    - Configure and enable the SSC interrupt
///    - Play WAV file
///
/// Please refer to the list of functions in the #Overview# tab of this unit
/// for more detailed information.
//-----------------------------------------------------------------------------


//------------------------------------------------------------------------------
//         Headers
//------------------------------------------------------------------------------

#include <board.h>
#include <board_memories.h>
#include <pio/pio.h>
#include <irq/irq.h>
#include <twi/twid.h>
#include <twi/twi.h>
#include <dbgu/dbgu.h>
#include <ssc/ssc.h>
#include <codec-wm8731/wm8731.h>
#include <utility/wav.h>
#include <utility/assert.h>
#include <utility/math.h>
#include <utility/trace.h>
#include <memories/MEDSdcard.h>

#include "fatfs_config.h"
#if _FATFS_TINY != 1
#include <fatfs/src/ff.h>
#else
#include <fatfs/src/tff.h>
#endif
#include <fatfs/src/ff_util.h>

#include <string.h>

#define AUDIO_USING_DMA
#if defined (AUDIO_USING_DMA)
#include <dmad/dmad.h>
#include <dma/dma.h>
#endif

//------------------------------------------------------------------------------
//         Local constants
//------------------------------------------------------------------------------

/// Master clock frequency in Hz
#define SSC_MCK    49152000

/// Address at which the WAV file is located
#define WAV_FILE_ADDRESS        (0x60000000 + 0x100)//0x8000)

/// Maximum size in bytes of the WAV file.
#define MAX_WAV_SIZE            0x100000

// TWI clock
#define TWI_CLOCK       100000

// PMC define
#define AT91C_CKGR_PLLR     AT91C_CKGR_PLLAR
#define AT91C_PMC_LOCK      AT91C_PMC_LOCKA

#define AT91C_CKGR_MUL_SHIFT         16
#define AT91C_CKGR_OUT_SHIFT         14
#define AT91C_CKGR_PLLCOUNT_SHIFT     8
#define AT91C_CKGR_DIV_SHIFT          0

/// Maximum number of LUNs which can be defined.
/// (Logical drive = physical drive = medium number)
#define MAX_LUNS        1

/// Available medias.
Media medias[MAX_LUNS];

#define ID_DRV DRV_MMC

#define     SAMPLE_RATE          (48000)
#define     SLOT_BY_FRAME        (2)
#define     BITS_BY_SLOT         (16)

#define AT91C_I2S_MASTER_TX_SETTING(nb_bit_by_slot, nb_slot_by_frame)( +\
                       AT91C_SSC_CKS_DIV   +\
                       AT91C_SSC_CKO_CONTINOUS      +\
                        AT91C_SSC_START_FALL_RF +\
                       ((1<<16) & AT91C_SSC_STTDLY) +\
                       ((((nb_bit_by_slot*nb_slot_by_frame)/2)-1) <<24))

#define AT91C_I2S_TX_FRAME_SETTING(nb_bit_by_slot, nb_slot_by_frame)( +\
                        (nb_bit_by_slot-1)  +\
                        AT91C_SSC_MSBF   +\
                        (((nb_slot_by_frame-1)<<8) & AT91C_SSC_DATNB)  +\
                        (((nb_bit_by_slot-1)<<16) & AT91C_SSC_FSLEN) +\
                        AT91C_SSC_FSOS_NEGATIVE)

//------------------------------------------------------------------------------
//         Local variables
//------------------------------------------------------------------------------

/// List of pins to configure.
static const Pin pins[] = {PINS_TWI0, PINS_SSC_CODEC, PIN_PCK0};

/// Pointer to the playback WAV file header.
static const WavHeader *userWav = (WavHeader *) (0x60000000);

/// Indicates if the WAV file is currently being played.
static unsigned char isWavPlaying;

#if defined (AUDIO_USING_DMA)
/// Number of samples which have already been transmitted.
static unsigned int transmittedSamples;
/// Number of samples that have not yet been transmitted.
static unsigned int remainingSamples;
#endif

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

const char* FileName = STR_ROOT_DIRECTORY "sample.wav";

static Twid twid;

//------------------------------------------------------------------------------
//         Local functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Display the information of the WAV file (sample rate, stereo/mono and frame
/// size) on the DBGU.
//------------------------------------------------------------------------------
static void DisplayWavInfo(void)
{   
    printf("%c[2J", 27);
    printf( "  Wave file header information\n\r");
    printf( "--------------------------------\n\r");
    printf( "  - Chunk ID        = 0x%08X\n\r", userWav->chunkID);
    printf( "  - Chunk Size      = %d\n\r", userWav->chunkSize);
    printf( "  - Format          = 0x%08X\n\r", userWav->format);
    printf( "  - SubChunk ID     = 0x%08X\n\r", userWav->subchunk1ID);
    printf( "  - Subchunk1 Size  = %d\n\r", userWav->subchunk1Size);
    printf( "  - Audio Format    = 0x%04X\n\r", userWav->audioFormat);
    printf( "  - Num. Channels   = %d\n\r", userWav->numChannels);
    printf( "  - Sample Rate     = %d\n\r", userWav->sampleRate);
    printf( "  - Byte Rate       = %d\n\r", userWav->byteRate);
    printf( "  - Block Align     = %d\n\r", userWav->blockAlign);
    printf( "  - Bits Per Sample = %d\n\r", userWav->bitsPerSample);
    printf( "  - Subchunk2 ID    = 0x%08X\n\r", userWav->subchunk2ID);
    printf( "  - Subchunk2 Size  = %d\n\r", userWav->subchunk2Size);
    printf("Press a key to return to the menu ...\n\r");
    DBGU_GetChar();
}

//------------------------------------------------------------------------------
/// Displays the user menu on the DBGU.
//------------------------------------------------------------------------------
static void DisplayMenu(void)
{
    printf("%c[2J-- Basic SSC I2S WM8731 Project xxx --\n\r", 27);
    printf("Menu :\n\r");
    printf("------\n\r");

    // Play a WAV file pre-loaded in SDCARD using SAM-BA
    if (!isWavPlaying) {

        printf("  W: Play the WAV file pre-loaded in SDCARD\n\r");
    }

    // Display the information of the WAV file (sample rate, stereo/mono and frame size)
    printf("  I: Display the information of the WAV file\n\r");

    // Stop the current playback (if any)
    if (isWavPlaying) {

        printf("  S: Stop playback\n\r");
    }
}

#if defined(AUDIO_USING_DMA)
void HDMA_IrqHandler(void)
{
    unsigned int size;
    unsigned intFlag;
   
    // One buffer sent & more buffers to send
    if (remainingSamples > 0 ) {

        size = min(remainingSamples / (userWav->bitsPerSample / 8), BOARD_SSC_DMA_FIFO_SIZE * MAX_SSC_LLI_SIZE/2);
        SSC_WriteBuffer(AT91C_BASE_SSC0, (void *) (WAV_FILE_ADDRESS + transmittedSamples), size);
        remainingSamples -= size * (userWav->bitsPerSample / 8);
        transmittedSamples += size * (userWav->bitsPerSample / 8);
        intFlag = 1 << (BOARD_SSC_DMA_CHANNEL + 8) ;
        DMA_EnableIt(intFlag);
        DMA_EnableChannel(BOARD_SSC_DMA_CHANNEL);
    }
    else if (remainingSamples == 0){
        DMA_DisableChannel(BOARD_SSC_DMA_CHANNEL);
        intFlag = 1 << (BOARD_SSC_DMA_CHANNEL + 8) ;
        DMA_DisableIt(intFlag);
        isWavPlaying = 0;
        DisplayMenu();
    }
    
}
#endif

//------------------------------------------------------------------------------
/// Play a WAV file pre-loaded in SDCARD.
//------------------------------------------------------------------------------
void PlayLoop(unsigned short *pExtMem, unsigned int numSamples)
{
    unsigned int i;
    for (i = 0; i < numSamples; i++) {

        SSC_Write(AT91C_BASE_SSC0, pExtMem[i]);
    }    
}

//------------------------------------------------------------------------------
/// Play a WAV file pre-loaded in SDCARD
//------------------------------------------------------------------------------
static void PlayWavFile(void)
{
#if !defined(AUDIO_USING_DMA)
    unsigned int size;

    size = userWav->subchunk2Size > MAX_WAV_SIZE ? MAX_WAV_SIZE : userWav->subchunk2Size;
    SSC_EnableTransmitter(AT91C_BASE_SSC0);
    PlayLoop((unsigned short *)WAV_FILE_ADDRESS, size >> 1);
#else
    unsigned int size;
    unsigned int intFlag = 0;

    size = userWav->subchunk2Size > MAX_WAV_SIZE ? MAX_WAV_SIZE : userWav->subchunk2Size;
    SSC_EnableTransmitter(AT91C_BASE_SSC0);

    // Start transmitting WAV file to SSC
    remainingSamples = userWav->subchunk2Size;
    transmittedSamples = 0;
    
    intFlag = 1 << (BOARD_SSC_DMA_CHANNEL + 8) ;
    DMA_DisableIt(intFlag);
    DMA_DisableChannel(BOARD_SSC_DMA_CHANNEL);
    
    // Fill DMA buffer
    size = min(remainingSamples / (userWav->bitsPerSample / 8), BOARD_SSC_DMA_FIFO_SIZE * MAX_SSC_LLI_SIZE/2);
    SSC_WriteBuffer(AT91C_BASE_SSC0, (void *) (WAV_FILE_ADDRESS + transmittedSamples), size);
    remainingSamples -= size * (userWav->bitsPerSample / 8);
    transmittedSamples += size * (userWav->bitsPerSample / 8);

    intFlag = 1 << (BOARD_SSC_DMA_CHANNEL + 8) ;
    DMA_EnableIt(intFlag);
    DMA_EnableChannel(BOARD_SSC_DMA_CHANNEL);

#endif
}

//------------------------------------------------------------------------------
/// Stop the current playback (if any).
//------------------------------------------------------------------------------
static void StopPlayback(void)
{
    SSC_DisableTransmitter(AT91C_BASE_SSC0);
}

//------------------------------------------------------------------------------
/// Check wav file from sdcard
//------------------------------------------------------------------------------
unsigned char CheckWavFile()
{
    FRESULT res;
    FATFS fs;             // File system object
    FIL FileObject;

    unsigned int numRead, pcmSize;

    // Init Disk
    printf("-I- Init media Sdcard\n\r");
    MEDSdcard_Initialize(&medias[ID_DRV], MCI_ID);

    // Mount disk
    printf("-I- Mount disk %d\n\r", ID_DRV);
    memset(&fs, 0, sizeof(FATFS));		// Clear file system object
    res = f_mount(ID_DRV, &fs);
    if( res != FR_OK ) {
        printf("-E- f_mount pb: 0x%X (%s)\n\r", res, FF_GetStrResult(res));
        return 0;
    }

    res = f_open(&FileObject, FileName, FA_OPEN_EXISTING|FA_READ);
    if (res == FR_OK) {
        printf("-I- File Found!\n\r");
        //f_close(&FileObject);
        //    FilePlay();
    }
    else {
        printf("-E- File Not Found!\n\r");
        return 1;
    }

    // Read header
    f_read(&FileObject, (void*)userWav, sizeof(WavHeader), &numRead);
    DisplayWavInfo();

    // Load PCM
    pcmSize = userWav->subchunk2Size;
    if (pcmSize > MAX_WAV_SIZE) {
        pcmSize = MAX_WAV_SIZE;
    }
    f_read(&FileObject, (void*)WAV_FILE_ADDRESS, pcmSize, &numRead);
    printf("-I- PCM Load to %x, size %d\n\r", WAV_FILE_ADDRESS, numRead);
    f_close(&FileObject);

    return 0;
}
        
//------------------------------------------------------------------------------
/// Main function
//------------------------------------------------------------------------------
int main(void)
{
    unsigned char key;
    unsigned char isValid;

    // Configure all pins
    PIO_Configure(pins, PIO_LISTSIZE(pins));

    // Initialize the DBGU
    TRACE_CONFIGURE(DBGU_STANDARD, 115200, BOARD_MCK);

    // Initialize PSRAM
    BOARD_ConfigurePsram();

    // Switch to Main clock
    AT91C_BASE_PMC->PMC_MCKR = (AT91C_BASE_PMC->PMC_MCKR & ~AT91C_PMC_CSS) | AT91C_PMC_CSS_MAIN_CLK;
    while ((AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY) == 0);

    // Configure PLL to 98.285MHz
    *AT91C_CKGR_PLLR = ((1 << 29) | (171 << AT91C_CKGR_MUL_SHIFT) \
        | (0x0 << AT91C_CKGR_OUT_SHIFT) |(0x3f << AT91C_CKGR_PLLCOUNT_SHIFT) \
        | (21 << AT91C_CKGR_DIV_SHIFT));
    while ((AT91C_BASE_PMC->PMC_SR & AT91C_PMC_LOCK) == 0);

    // Configure master clock in two operations
    AT91C_BASE_PMC->PMC_MCKR = (( AT91C_PMC_PRES_CLK_2 | AT91C_PMC_CSS_PLLA_CLK) & ~AT91C_PMC_CSS) | AT91C_PMC_CSS_MAIN_CLK;
    while ((AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY) == 0);
    AT91C_BASE_PMC->PMC_MCKR = ( AT91C_PMC_PRES_CLK_2 | AT91C_PMC_CSS_PLLA_CLK);
    while ((AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY) == 0);

    // DBGU reconfiguration
    DBGU_Configure(DBGU_STANDARD, 115200, SSC_MCK);

    // Configure and enable the TWI (required for accessing the DAC)
    *AT91C_PMC_PCER = (1<< AT91C_ID_TWI0); 
    TWI_ConfigureMaster(AT91C_BASE_TWI0, TWI_CLOCK, SSC_MCK);
    TWID_Initialize(&twid, AT91C_BASE_TWI0);

    // Enable the DAC master clock 
    AT91C_BASE_PMC->PMC_PCKR[0] = AT91C_PMC_CSS_PLLA_CLK | AT91C_PMC_PRES_CLK_8;
    AT91C_BASE_PMC->PMC_SCER = AT91C_PMC_PCK0;
    while ((AT91C_BASE_PMC->PMC_SR & AT91C_PMC_PCKRDY0) == 0);

    printf("-- Basic SSC I2S WM8731 Project %s --\n\r", SOFTPACK_VERSION);
    printf("-- %s\n\r", BOARD_NAME);
    printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);

    // Check and load wav file from sdcard
    isValid = CheckWavFile();
    if(isValid) {
        printf("-E- Open wav file fail!\r\n");
        return 1;
    }
    // Load WAV file information
    isValid = WAV_IsValid(userWav);
    ASSERT(isValid, "-F- Invalid WAV file provided\n\r");
    isWavPlaying = 0;

    // Sample rate must be 48kHz
    printf("-I- Sample rate = %d Hz\n\r", userWav->sampleRate);
    ASSERT(userWav->sampleRate == 48000, "-F- The WAV file must have a sample rate of 48kHz\n\r");
    
    // Initialize the audio DAC    
    WM8731_DAC_Init(&twid, WM8731_SLAVE_ADDRESS);

    // Configure SSC
    SSC_Configure(AT91C_BASE_SSC0,
                  AT91C_ID_SSC0,
                  SAMPLE_RATE * BITS_BY_SLOT * 2,
                  SSC_MCK);
    SSC_ConfigureReceiver(AT91C_BASE_SSC0, 0, 0);
    SSC_ConfigureTransmitter(AT91C_BASE_SSC0,
        AT91C_I2S_MASTER_TX_SETTING(BITS_BY_SLOT, SLOT_BY_FRAME),
        AT91C_I2S_TX_FRAME_SETTING( BITS_BY_SLOT, SLOT_BY_FRAME));
    SSC_DisableTransmitter(AT91C_BASE_SSC0);

#if defined(AUDIO_USING_DMA)
    // Initialize DMA controller.
    DMAD_Initialize(BOARD_SSC_DMA_CHANNEL);
    // Configure and enable the SSC interrupt
    IRQ_ConfigureIT(AT91C_ID_HDMA, 0, HDMA_IrqHandler);
    IRQ_EnableIT(AT91C_ID_HDMA);
#endif

    // Enter menu loop
    while (1) {

        // Display menu
        DisplayMenu();

        // Process user input
        key = DBGU_GetChar();

        // Play WAV file
        if ((key == 'W') && !isWavPlaying) {

            PlayWavFile();
            isWavPlaying = 1;
        }
        // Display WAV information
        else if (key == 'I') {

            DisplayWavInfo();
        }
        // Stop playback
        else if ((key == 'S') && isWavPlaying) {

            StopPlayback();
            isWavPlaying = 0;
        }
    }
}

