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
/// \dir "Basic SSC-I2S AT73C213  project"
///
/// !!!Purpose
///
///  This example uses the Synchronous Serial Controller (SSC) of an AT91 microcontroller 
///  to output an audio steam through the on-board AT73C213 audio DAC. 
/// 
///
/// !!!See
/// - ssc: SSC driver interface
///
/// !!!Description
/// 
/// This program plays a WAV file pre-loaded into the external SDRAM. The audio stream is sent through 
/// the SSC interface connected to the on-board AT73C213, enabling the sound to be audible using a pair of headphones. 
///
/// Since the AT73C213 DAC requires that it be feeded a master clock multiple of the sample rate, 
/// it must be downloaded in external SDRAM, at address 0x20005000 
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
///     -- Basic SSC I2S AT73C213 Project xxx --
///     -- AT91xxxxxx-xx
///     -- Compiled: xxx xx xxxx xx:xx:xx --
///     Menu :
///     ------
///     W: Play the WAV file pre-loaded in SDRAM
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
/// basic-ssc-i2s-at73c213-project
///
/// !Contents
///
/// The code can be roughly broken down as follows:
///    - Enable the clock 
///    - Load WAV file information
///    - Configure and enable the DAC
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
#include <spi/spi.h>
#include <dbgu/dbgu.h>
#include <ssc/ssc.h>
#include <dac-at73c213/at73c213.h>
#include <utility/wav.h>
#include <utility/assert.h>
#include <utility/math.h>
#include <utility/trace.h>

//------------------------------------------------------------------------------
//         Local constants
//------------------------------------------------------------------------------

/// Master clock frequency in Hz
#if defined(at91sam7seek)
    #define MCK             BOARD_MAINOSC
#elif defined(at91sam9260)
    #define MCK             (BOARD_MAINOSC * 8 / 2)
#elif defined(at91cap9stk)
    #define MCK             ((BOARD_MAINOSC / 250)*  1024)
#endif

/// AT73C213 Master Clock selector
#if defined(at91sam7seek)
    #define MCK_SEL  AT73C213_MCK_SEL_384_FS
#elif defined(at91sam9260)
    #define MCK_SEL  AT73C213_MCK_SEL_384_FS
#elif defined(at91cap9stk)
    #define MCK_SEL  AT73C213_MCK_SEL_256_FS
#endif

/// Address at which the WAV file is located
#define WAV_FILE_ADDRESS        (AT91C_EBI_SDRAM + 0x5000)

/// Maximum size in bytes of the WAV file.
#define MAX_WAV_SIZE            0x2000000

/// SPI configuration value.
#define SPI_CONFIGURATION   (AT91C_SPI_MSTR | AT91C_SPI_PS_FIXED | SPI_PCS(BOARD_AT73C213_SPI_NPCS))

//------------------------------------------------------------------------------
//         Local variables
//------------------------------------------------------------------------------

/// List of pins to configure.
static const Pin pins[] = {
    BOARD_AT73C213_MCK,
    BOARD_AT73C213_SPI_PINS,
    BOARD_AT73C213_SSC_PINS};

/// Pointer to the playback WAV file header.
static const WavHeader *userWav = (WavHeader *) WAV_FILE_ADDRESS;

/// Indicates if the WAV file is currently being played.
static unsigned char isWavPlaying;

/// Number of samples which have already been transmitted.
static unsigned int transmittedSamples;
/// Number of samples that have not yet been transmitted.
static unsigned int remainingSamples;

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
    printf("%c[2J-- Basic SSC I2S AT73C213 Project xxx --\n\r", 27);
    printf("Menu :\n\r");
    printf("------\n\r");

    // Play a WAV file pre-loaded in SDRAM using SAM-BA
    if (!isWavPlaying) {

        printf("  W: Play the WAV file pre-loaded in SDRAM\n\r");
    }

    // Display the information of the WAV file (sample rate, stereo/mono and frame size)
    printf("  I: Display the information of the WAV file\n\r");

    // Stop the current playback (if any)
    if (isWavPlaying) {

        printf("  S: Stop playback\n\r");
    }
}

//------------------------------------------------------------------------------
/// Interrupt handler for the SSC. Loads the PDC with the audio data to stream.
//------------------------------------------------------------------------------
static void ISR_Ssc(void)
{
    unsigned int status = BOARD_AT73C213_SSC->SSC_SR;
    unsigned int size;

    // Last buffer sent
    if ((status & AT91C_SSC_TXBUFE) != 0) {

        isWavPlaying = 0;
        SSC_DisableInterrupts(BOARD_AT73C213_SSC, AT91C_SSC_ENDTX | AT91C_SSC_TXBUFE);
        BOARD_AT73C213_SSC->SSC_PTCR = AT91C_PDC_TXTDIS;
        DisplayMenu();
    }
    // One buffer sent & more buffers to send
    else if (remainingSamples > 0) {

        size = min(remainingSamples / (userWav->bitsPerSample / 8), 65535);
        SSC_WriteBuffer(BOARD_AT73C213_SSC, (void *) (WAV_FILE_ADDRESS + sizeof(WavHeader) + transmittedSamples), size);
        remainingSamples -= size * (userWav->bitsPerSample / 8);
        transmittedSamples += size * (userWav->bitsPerSample / 8);
    }
    // One buffer sent, no more buffers
    else {

        SSC_DisableInterrupts(BOARD_AT73C213_SSC, AT91C_SSC_ENDTX);
    }
}

//------------------------------------------------------------------------------
/// Play a WAV file pre-loaded in SDRAM using SAM-BA.
//------------------------------------------------------------------------------
static void PlayWavFile(void)
{
    unsigned int size;

    ASSERT(userWav->chunkSize < MAX_WAV_SIZE,
           "-F- WAV file too big (increase MAX_WAV_SIZE)\n\r");

    // Start transmitting WAV file to SSC
    remainingSamples = userWav->subchunk2Size;
    transmittedSamples = 0;
    
    // Fill first PDC buffer
    size = min(remainingSamples / (userWav->bitsPerSample / 8), 65535);
    SSC_WriteBuffer(BOARD_AT73C213_SSC, (void *) (WAV_FILE_ADDRESS + sizeof(WavHeader) + transmittedSamples), size);
    remainingSamples -= size * (userWav->bitsPerSample / 8);
    transmittedSamples += size * (userWav->bitsPerSample / 8);

    // Fill second buffer if necessary
    if (remainingSamples > 0) {

        size = min(remainingSamples / (userWav->bitsPerSample / 8), 65535);
        SSC_WriteBuffer(BOARD_AT73C213_SSC, (void *) (WAV_FILE_ADDRESS + sizeof(WavHeader) + transmittedSamples), size);
        remainingSamples -= size * (userWav->bitsPerSample / 8);
        transmittedSamples += size * (userWav->bitsPerSample / 8);
    }

    SSC_EnableInterrupts(BOARD_AT73C213_SSC, AT91C_SSC_TXBUFE | AT91C_SSC_ENDTX);
}

//------------------------------------------------------------------------------
/// Stop the current playback (if any).
//------------------------------------------------------------------------------
static void StopPlayback(void)
{
    SSC_DisableInterrupts(BOARD_AT73C213_SSC, AT91C_SSC_TXBUFE | AT91C_SSC_ENDTX);
    BOARD_AT73C213_SSC->SSC_PTCR = AT91C_PDC_TXTDIS;
    BOARD_AT73C213_SSC->SSC_TNCR = 0;
    BOARD_AT73C213_SSC->SSC_TCR = 0;
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
    TRACE_CONFIGURE(DBGU_STANDARD, 115200, MCK);

    // Configure and enable the SPI (required for accessing the DAC)
    SPI_Configure(BOARD_AT73C213_SPI, BOARD_AT73C213_SPI_ID, SPI_CONFIGURATION);
    SPI_Enable(BOARD_AT73C213_SPI);

    // Clock configuration
#if defined(at91sam7seek)

    // Switch to main oscillator
    AT91C_BASE_PMC->PMC_MCKR = (AT91C_BASE_PMC->PMC_MCKR & ~AT91C_PMC_CSS) | AT91C_PMC_CSS_MAIN_CLK;
    while ((AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY) != AT91C_PMC_MCKRDY);
    AT91C_BASE_PMC->PMC_MCKR = AT91C_PMC_CSS_MAIN_CLK;
    while ((AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY) != AT91C_PMC_MCKRDY);

    // Enable its master clock (settings for a 48kHz sample rate)
    AT91C_BASE_PMC->PMC_PCKR[2] = AT91C_PMC_CSS_MAIN_CLK;
    AT91C_BASE_PMC->PMC_SCER = AT91C_PMC_PCK2;
    while ((AT91C_BASE_PMC->PMC_SR & AT91C_PMC_PCK2RDY) == 0);

#elif defined(at91sam9260)

    // Switch to slow clock oscillator
    AT91C_BASE_PMC->PMC_MCKR = (AT91C_BASE_PMC->PMC_MCKR & ~AT91C_PMC_CSS) | AT91C_PMC_CSS_SLOW_CLK;
    while ((AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY) != AT91C_PMC_MCKRDY);

    // Configure PLLA at 147.456MHz
    AT91C_BASE_PMC->PMC_PLLAR = 0x20073F01;
    while (!(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_LOCKA));

    // Switch to slow clock + prescaler
    AT91C_BASE_PMC->PMC_MCKR = AT91C_PMC_MDIV_2;
    while (!(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY));

    // Switch to PLL + prescaler
    AT91C_BASE_PMC->PMC_MCKR |= AT91C_PMC_CSS_PLLA_CLK;
    while (!(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY));

    // Enable the DAC master clock
    AT91C_BASE_PMC->PMC_PCKR[0] = AT91C_PMC_CSS_PLLA_CLK | AT91C_PMC_PRES_CLK_8;
    AT91C_BASE_PMC->PMC_SCER = AT91C_PMC_PCK0;
    while ((AT91C_BASE_PMC->PMC_SR & AT91C_PMC_PCK0RDY) == 0);
    
#elif defined(at91cap9stk)

    // Switch to slow clock oscillator
    AT91C_BASE_PMC->PMC_MCKR = (AT91C_BASE_PMC->PMC_MCKR & ~AT91C_PMC_CSS) | AT91C_PMC_CSS_SLOW_CLK;
    while ((AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY) != AT91C_PMC_MCKRDY);

    // Configure PLLA at 98.304MHz
    AT91C_BASE_PMC->PMC_PLLAR = 0x23FF3F7D;
    while (!(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_LOCKA));

    // Switch to slow clock + prescaler
    AT91C_BASE_PMC->PMC_MCKR = AT91C_PMC_MDIV_2;
    while (!(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY));

    // Switch to PLL + prescaler
    AT91C_BASE_PMC->PMC_MCKR |= AT91C_PMC_CSS_PLLA_CLK;
    while (!(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY));

    // Enable the DAC master clock : 12 288 000 Hz (= (98.304MHz / 8) = (48kHz * 256) )
    AT91C_BASE_PMC->PMC_PCKR[2] = AT91C_PMC_CSS_PLLA_CLK | AT91C_PMC_PRES_CLK_8;
    AT91C_BASE_PMC->PMC_SCER = AT91C_PMC_PCK2;
    while ((AT91C_BASE_PMC->PMC_SR & AT91C_PMC_PCK2RDY) == 0);
    
#endif    

    printf("-- Basic SSC I2S AT73C213 Project %s --\n\r", SOFTPACK_VERSION);
    printf("-- %s\n\r", BOARD_NAME);
    printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);
    
#if !defined(sdram) && defined(PINS_SDRAM)
    //configure SDRAM for use
    BOARD_ConfigureSdram(BOARD_SDRAM_BUSWIDTH);
#endif
    
    // Load WAV file information
    isValid = WAV_IsValid(userWav);
    ASSERT(isValid, "-F- Invalid WAV file provided\n\r");
    isWavPlaying = 0;

    // Sample rate must be 48kHz
    printf("-I- Sample rate = %d Hz\n\r", userWav->sampleRate);
    ASSERT(userWav->sampleRate == 48000, "-F- The WAV file must have a sample rate of 48kHz\n\r");
    
    // Initialize the audio DAC
    // Configure and enable the DAC
    AT73C213_Enable(userWav->sampleRate,
                    userWav->bitsPerSample / 8,
                    userWav->numChannels,
                    MCK_SEL,
                    MCK);

    // Configure and enable the SSC interrupt
    IRQ_ConfigureIT(BOARD_AT73C213_SSC_ID, 0, ISR_Ssc);
    IRQ_EnableIT(BOARD_AT73C213_SSC_ID);
    
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

