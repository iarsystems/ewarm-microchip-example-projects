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
/// \dir "Basic AC97 project"
///
/// !!!Purpose
///
/// This example shows how to use the AC97 controller of AT91 microcontrollers
/// to play and record sound, as well as control the on-board AC97 codec 
/// configuration. 
///
/// !!!See
/// - ac97c: AC97 interface driver
///
/// !!!Description
///
/// When launched, this program displays a menu on the DBGU,
/// enabling the user to choose between several options
/// - Play a pre-loaded WAV file using the AC97 codec output 
/// - Display the WAV file information (sample rate, etc.) 
/// - Record a sample using the AC97 codec microphone input 
/// - Play back the recorded file 
/// - Adjust the playback volume 
///
/// To be able to play a WAV file, it must first be loaded prior to launching the application.
/// It must be downloaded in external SDRAM, at address 0x20005000; or in board without sdram, 
/// but with DDRAM, it must be downloaded in external DDRAM, at address 0x70005000, this can be
/// easily done using SAM-BA, but the sound file must be with sample rate 48KHZ ,  
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
///     -- Basic AC97 Project xxx --
///     -- AT91xxxxxx-xx
///     -- Compiled: xxx xx xxxx xx:xx:xx --
///     -I- Codec probed correctly
///     Menu :
///     ------
///       W: Play the WAV file pre-loaded in SDRAM
///       I: Display the information of the WAV file
///       R: Start recording sound
///       +/-: Adjust volume (16)
///     
///       Wave file header information
///     --------------------------------
///       - Chunk ID        = 0x46464952
///       - Chunk Size      = 377896
///       - Format          = 0x45564157
///       - SubChunk ID     = 0x20746D66
///       - Subchunk1 Size  = 16
///       - Audio Format    = 0x0001
///       - Num. Channels   = 2
///       - Sample Rate     = 8000
///       - Byte Rate       = 32000
///       - Block Align     = 4
///       - Bits Per Sample = 16
///       - Subchunk2 ID    = 0x61746164
///       - Subchunk2 Size  = 377860
///     Press a key to return to the menu ...
///    \endcode
/// User can choose any of the available options(capital) to perform the described action,
/// connecting earphone to the board to hear the sound of WAV file.
/// -# for debug the application, compile and link for SDRAM.
/// -# Then type inside gdb: restore sample.wav binary 0x20005000
/// -# Then continue
///
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/// \unit
///
/// !Purpose
///
/// This file contains all the specific code for the
/// basic-ac97-project
///
/// !Contents
///
/// The code can be roughly broken down as follows:
///    -  Initialize the AC97 controller
///    -  Initialize the codec
///    -  Load WAV file information
///    -  Play a WAV file pre-loaded in SDRAM
///    -  The main function, which implements the program behavior
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
#include <dbgu/dbgu.h>
#include <ac97c/ac97c.h>
#include <codec-ad1981b/ad1981b.h>
#include <utility/wav.h>
#include <utility/assert.h>
#include <utility/trace.h>

//------------------------------------------------------------------------------
//         Local constants
//------------------------------------------------------------------------------
/// Address at which the WAV file is located
#if defined(PINS_SDRAM)
#define WAV_FILE_ADDRESS        (AT91C_EBI_SDRAM + 0x00005000)
#elif defined(PINS_DDRAM)
#define WAV_FILE_ADDRESS        (AT91C_DDR2 + 0x00005000)
#endif

/// Maximum size in bytes of the WAV file.
#define MAX_WAV_SIZE            0x2000000

/// Address at which the recorded sample is stored.
#define RECORD_ADDRESS          (WAV_FILE_ADDRESS + MAX_WAV_SIZE)

/// Maximum size of the recorded sound.
#define MAX_RECORD_SIZE         0xFFFFFFFF

//------------------------------------------------------------------------------
//         Local variables
//------------------------------------------------------------------------------

/// List of pins to configure.
static const Pin pinsAc97[] = {PINS_AC97};

/// Command buffer used during codec initialization.
static const unsigned int pInitBuffer[] = {

    // Reset
    AD1981B_CMD(AD1981B_RESET, 0x0299),
    // Master Volume 
    AD1981B_CMD(AD1981B_MASTER, 0),
    // Headphone Volume
    AD1981B_CMD(AD1981B_HEADPHONE, 0x0F0F),
    // PCM Volume
    AD1981B_CMD(AD1981B_PCM, 0),
    // Record Select 
    AD1981B_CMD(AD1981B_REC_SEL, 0x0505),
    // Record Gain
    AD1981B_CMD(AD1981B_REC_GAIN, 0x0F0F),
    // Extern Audio set as variable rate audio mode
    AD1981B_CMD(AD1981B_EXTAUDIO, 0x0001),
    // Record Gain MIC
    AD1981B_CMD(AD1981B_MIC, 0),
    // Record Gain MIC
    AD1981B_CMD(AD1981B_REC_GAIN_MIC, 0x0303),
    // General Purpose
    AD1981B_CMD(AD1981B_GENERAL_PURPOSE, 0x00),
    // Serial configuration
    AD1981B_CMD(AD1981B_SERIAL_CONFIG, 0x7000),
};

/// Pointer to the playback WAV file header.
static const WavHeader *userWav = (WavHeader *) WAV_FILE_ADDRESS;

/// Size of recorded sample in bytes.
static unsigned int recordSize;

/// Indicates if the user-provided WAV file is valid.
static unsigned char isWavValid;

/// Indicates if the WAV file is currently being played.
static unsigned char isWavPlaying;

/// Indicates if the recording is being played.
static unsigned char isRecordPlaying;

/// Indicates if a recording is being performed.
static unsigned char isRecording;

/// Indicates if a recording has been performed.
static unsigned char isRecordAvailable;

/// Master volume level.
static unsigned int volume = 0x0F;

//------------------------------------------------------------------------------
//         Local functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Display the information of the WAV file (sample rate, stereo/mono and frame
/// size) on the DBGU.
//------------------------------------------------------------------------------
static void DisplayWavInfo(void)
{   
    printf( "\n\r  Wave file header information\n\r");
    printf( "--------------------------------\n\r");
    printf( "  - Chunk ID        = 0x%08X\n\r", userWav->chunkID);
    printf( "  - Chunk Size      = %u\n\r", userWav->chunkSize);
    printf( "  - Format          = 0x%08X\n\r", userWav->format);
    printf( "  - SubChunk ID     = 0x%08X\n\r", userWav->subchunk1ID);
    printf( "  - Subchunk1 Size  = %u\n\r", userWav->subchunk1Size);
    printf( "  - Audio Format    = 0x%04X\n\r", userWav->audioFormat);
    printf( "  - Num. Channels   = %d\n\r", userWav->numChannels);
    printf( "  - Sample Rate     = %u\n\r", userWav->sampleRate);
    printf( "  - Byte Rate       = %u\n\r", userWav->byteRate);
    printf( "  - Block Align     = %d\n\r", userWav->blockAlign);
    printf( "  - Bits Per Sample = %d\n\r", userWav->bitsPerSample);
    printf( "  - Subchunk2 ID    = 0x%08X\n\r", userWav->subchunk2ID);
    printf( "  - Subchunk2 Size  = %u\n\r", userWav->subchunk2Size);
    printf("Press a key to return to the menu ...\n\r");
    DBGU_GetChar();
}

//------------------------------------------------------------------------------
/// Displays the user menu on the DBGU.
//------------------------------------------------------------------------------
static void DisplayMenu(void)
{
    printf("Menu :\n\r");
    printf("------\n\r");

    // Play a WAV file pre-loaded in SDRAM using SAM-BA
    if (isWavValid && !isWavPlaying) {

        printf("  W: Play the WAV file pre-loaded in SDRAM\n\r");
    }
    else if (!isWavValid) {

        printf("  W: Not available, invalid WAV file provided.\n\r");
    }

    // Display the information of the WAV file (sample rate, stereo/mono and frame size)
    if (isWavValid) {

        printf("  I: Display the information of the WAV file\n\r");
    }

    // Record sound (if not already recording)
    if (!isRecording) {

        printf("  R: Start recording sound\n\r");
    }

    // Stop recording sound (if recording)
    if (isRecording) {

        printf("  R: Stop recording sound\n\r");
    }

    // Play back the recorded sound (if a recording has been performed)
    if (isRecordAvailable && !isRecordPlaying) {

        printf("  P: Play back the recorded sound\n\r");        
    }

    // Stop the current playback (if any)
    if (isWavPlaying || isRecordPlaying) {

        printf("  S: Stop playback\n\r");
    }

    printf("  +/-: Adjust volume (%u)\n\r", 0x1F - volume);
}

//------------------------------------------------------------------------------
/// Callback function invoked by AC97C_Transfer when a recording has been
/// completed, returning the sample size.
/// \param unused  Unused parameter.
/// \param status  Transfer result.
/// \param remaining  Bytes not transferred.
//------------------------------------------------------------------------------
static void RecordFinished(
    unsigned int unused,
    unsigned char status,
    unsigned int remaining)
{
    ASSERT(!status || (status == AC97C_ERROR_STOPPED),
           "-F- Recording error (%d)\n\r", status);
    recordSize = MAX_RECORD_SIZE - remaining;
}

//------------------------------------------------------------------------------
/// Playback invoked whenever the AC97 has finished playing a sound.
/// \param unused  Unused parameter.
/// \param status  Transfer result.
//------------------------------------------------------------------------------
static void PlaybackFinished(unsigned int unused, unsigned char status)
{
    isWavPlaying = 0;
    isRecordPlaying = 0;
    DisplayMenu();
}

//------------------------------------------------------------------------------
/// Play a WAV file pre-loaded in SDRAM using SAM-BA.
//------------------------------------------------------------------------------
static void PlayWavFile(void)
{
    ASSERT(userWav->chunkSize < MAX_WAV_SIZE,
           "-F- WAV file too big (increase MAX_WAV_SIZE)\n\r");

    // Configure sample rate of codec
    AC97C_WriteCodec(AD1981B_PMC_DAC, userWav->sampleRate);

    // Set channel size
    AC97C_SetChannelSize(AC97C_CHANNEL_A, userWav->bitsPerSample);

    // Start channel A transfer
    AC97C_Transfer(AC97C_CHANNEL_A_TRANSMIT, 
                   (unsigned char *) (WAV_FILE_ADDRESS + sizeof(WavHeader)),
                   userWav->subchunk2Size / userWav->numChannels,
                   (Ac97Callback) PlaybackFinished, 
                   0);
}

//------------------------------------------------------------------------------
/// Record sound (if not already recording).
//------------------------------------------------------------------------------
static void StartRecording(void)
{
    // Enable recording
    AC97C_WriteCodec(AD1981B_REC_SEL, 0);

    // Set sample rate
    AC97C_WriteCodec(AD1981B_PMC_ADC, 7000);

    // Always use 16-bits recording
    AC97C_SetChannelSize(AC97C_CHANNEL_A, 16);

    // Start recording
    recordSize = 0;
    AC97C_Transfer(AC97C_CHANNEL_A_RECEIVE, 
                   (unsigned char *) RECORD_ADDRESS,
                   MAX_RECORD_SIZE,
                   (Ac97Callback) RecordFinished,
                   0);
}

//------------------------------------------------------------------------------
/// Stop recording sound (if recording).
//------------------------------------------------------------------------------
static void StopRecording(void)
{
    unsigned int command;

    // Stop transfer
    AC97C_CancelTransfer(AC97C_CHANNEL_A_RECEIVE);

    // Disable recording in codec
    command = AD1981B_CMD(AD1981B_REC_SEL, 0x0505);
    AC97C_Transfer(AC97C_CODEC_TRANSFER, (unsigned char *) &command, 1, 0, 0);
}

//------------------------------------------------------------------------------
/// Play back the recorded sound (if a recording has been performed).
//------------------------------------------------------------------------------
static void PlayRecord(void)
{   
    unsigned int size;

    // Change channel size to 16 bits
    AC97C_SetChannelSize(AC97C_CHANNEL_A, 16);

    // Set sample rate
    AC97C_WriteCodec(AD1981B_PMC_DAC, 7000);

    // Start playing
    if (recordSize > 0) {

        size = recordSize;
    }
    else {

        size = MAX_RECORD_SIZE;
    }

    AC97C_Transfer(AC97C_CHANNEL_A_TRANSMIT, 
                   (unsigned char *) RECORD_ADDRESS,
                   size,
                   (Ac97Callback) PlaybackFinished,
                   0);
}

//------------------------------------------------------------------------------
/// Stop the current playback (if any).
//------------------------------------------------------------------------------
static void StopPlayback(void)
{
    AC97C_CancelTransfer(AC97C_CHANNEL_A_TRANSMIT);
}     
        
//------------------------------------------------------------------------------
/// Main function
//------------------------------------------------------------------------------
int main(void)
{
    unsigned char key;
    unsigned int vendorId;
    unsigned int command;

    // Initialize the DBGU
    TRACE_CONFIGURE(DBGU_STANDARD, 115200, BOARD_MCK);
    printf("-- Basic AC97 Project %s --\n\r", SOFTPACK_VERSION);
    printf("-- %s\n\r", BOARD_NAME);
    printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);
    
    // Initialize the AC97 controller
    PIO_Configure(pinsAc97, PIO_LISTSIZE(pinsAc97));
    AC97C_Configure();
    AC97C_AssignInputSlots(AC97C_CHANNEL_A, (1 << 3) | (1 << 4));
    AC97C_AssignOutputSlots(AC97C_CHANNEL_A, (1 << 3) | (1 << 4));
    AC97C_ConfigureChannel(AC97C_CHANNEL_A, AT91C_AC97C_CEN);
    
    // Check that the codec is present by reading the vendor ID
    vendorId = AC97C_ReadCodec(AD1981B_VENDOR_ID1);
    ASSERT(vendorId == AD1981B_ID_1,  "-F- Codec probe: Wrong AD1981B_ID1 value.\n\r");
    
    vendorId = AC97C_ReadCodec(AD1981B_VENDOR_ID2);
    ASSERT(vendorId == AD1981B_ID_2,  "-F- Codec probe: Wrong AD1981B_ID2 value.\n\r");
    
    printf("-I- Codec probed correctly\n\r");

    // Initialize the codec
    AC97C_Transfer(AC97C_CODEC_TRANSFER, (unsigned char *) pInitBuffer, 11, 0, 0);
    
#if !defined(sdram) && defined(PINS_SDRAM)
    //configure SDRAM for use
    BOARD_ConfigureSdram(BOARD_SDRAM_BUSWIDTH);
#endif

    
#if !defined(ddram) && defined(PINS_DDRAM)
    //configure DDRAM for use
    BOARD_ConfigureDdram(0, BOARD_DDRAM_BUSWIDTH);
#endif

    // Load WAV file information
    isWavValid = WAV_IsValid(userWav);
    isWavPlaying = 0;
    isRecordPlaying = 0;
    isRecording = 0; 
    isRecordAvailable = 0;
    
    // Enter menu loop
    while (1) {

        // Display menu
        DisplayMenu();

        // Process user input
        key = DBGU_GetChar();

        // Play WAV file
        if ((key == 'W') && isWavValid && !isWavPlaying) {

            PlayWavFile();
            isWavPlaying = 1;
            isRecordPlaying = 0;
        }
        // Display WAV information
        else if ((key == 'I') && isWavValid) {

            DisplayWavInfo();
        }
        // Start recording
        else if ((key == 'R') && !isRecording) {

            StartRecording();
            isRecording = 1;
            isRecordAvailable = 1;
        }
        // Stop recording
        else if ((key == 'R') && isRecording) {

            StopRecording();
            isRecording = 0;
        }
        // Play recorded sample
        else if ((key == 'P') && isRecordAvailable && !isRecordPlaying) {

            PlayRecord();
            isRecordPlaying = 1;
            isWavPlaying = 0;
        }
        // Stop playback
        else if ((key == 'S') && (isWavPlaying || isRecordPlaying)) {

            StopPlayback();
            isRecordPlaying = 0;
            isWavPlaying = 0;
        }
        // Decrease volume
        else if (key == '-') {

            if (volume < (0x1F - 1)) {

                volume++;
                command = AD1981B_CMD(AD1981B_HEADPHONE, volume | volume << 8);
                AC97C_Transfer(AC97C_CODEC_TRANSFER, (unsigned char *) &command, 1, 0, 0);
            }
        }
        // Increase volume
        else if (key == '+') {

            if (volume > 1) {

                volume--;
                command = AD1981B_CMD(AD1981B_HEADPHONE, volume | volume<< 8);
                AC97C_Transfer(AC97C_CODEC_TRANSFER, (unsigned char *) &command, 1, 0, 0);
            }
        }
    }
}

