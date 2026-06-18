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
/// \dir "USB audio speaker"
///
/// !!!Purpose
///
/// The USB Audio Speaker Project will help you to get familiar with the
/// USB Device Port(UDP) and SSC interface on AT91SAM microcontrollers. Also
/// it can help you to be familiar with the USB Framework that is used for
/// rapid development of USB-compliant class drivers such as USB Audio Device
/// class.
///
/// You can find following information depends on your needs:
/// - Sample usage of USB Audio Device Class driver and SSC-I2S driver.
/// - USB Audio Class driver development based on the AT91 USB Framework.
/// - USB enumerate sequence, the standard and class-specific descriptors and
///   requests handling.
/// - The initialize sequence and usage of UDP interface.
/// - The initialize sequence and usage of SSC interface with PDC.
///
/// !See
/// - ssc: SSC interface driver
/// - "dac-at73c213": I2S codec at73c213 driver
/// - usb: USB Framework, Audio Device Class driver and UDP interface driver
///     - "AT91 USB device framework"
///        - "USBD API"
///     - "audio-speaker"
///
/// !!!Requirements
///
/// This package can be used with all Atmel evaluation kits that have both
/// UDP and SSC interface.
///
/// The current supported board list:
/// - at91sam7se-ek
/// - at91sam9260-ek
///
/// !!!Description
///
/// When an EK running this program connected to a host (PC for example), with
/// USB cable, the EK appears as a desktop speaker for the host. Then the host
/// can play sound through host software. The %audio stream from the host is
/// then sent to the EK, and eventually sent to %audio DAC connected to AC97
/// of AT91SAM chips. At the same time, the %audio stream received is also sent
/// back to host from EK for recording.
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
/// \code
/// -- USB Device Audio Speaker Project xxx --
/// -- AT91xxxxxx-xx
/// -- Compiled: xxx xx xxxx xx:xx:xx --
/// \endcode
/// -# When connecting USB cable to windows, the LED blinks, and the host
///    reports a new USB %device attachment (if it's the first time you connect
///    an %audio speaker demo board to your host). You can find new
///    "USB Composite Device" and "USB Audio Device" appear in the hardware
///    %device list.
/// -# You can play sound in host side through the USB Audio Device, and it
///    can be heard from the earphone connected to the EK.
/// -# When playing sound, you can also record through the USB Audio Device on
///    the host.
///
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/// \unit
///
/// !Purpose
///
/// This file contains all the specific code for the
/// usb-device-audio-speaker-project
///
/// !Contents
///
/// The code can be roughly broken down as follows:
///    - Configuration functions
///       - VBus_Configure
///       - PIO & Clock configurations in start of main
///    - Interrupt handlers
///       - ISR_SSC
///    - Callback functions
///       - AUDDSpeakerChannel_MuteChanged
///       - FrameReceived
///    - The main function, which implements the program behavior
///
/// Please refer to the list of functions in the #Overview# tab of this unit
/// for more detailed information.
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//         Headers
//------------------------------------------------------------------------------

#include <board.h>
#include <pio/pio.h>
#include <pio/pio_it.h>
#include <spi/spi.h>
#include <ssc/ssc.h>
#include <irq/irq.h>
#include <dbgu/dbgu.h>
#include <utility/trace.h>
#include <utility/led.h>
#include <components/dac-at73c213/at73c213.h>
#include <usb/device/core/USBD.h>
#include <usb/device/audio-speaker/AUDDSpeakerDriver.h>
#include <usb/device/audio-speaker/AUDDSpeakerChannel.h>
#include <pmc/pmc.h>

#include <string.h>
#include <stdio.h>

//------------------------------------------------------------------------------
//         Definitions
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

#if defined(at91sam7seek)
    /// Priority of the SSC interrupt
    #define SSC_PRIORITY        0
#elif defined(at91sam9260)
    /// Priority of the SSC interrupt
    #define SSC_PRIORITY        7
#elif defined(at91cap9)
    /// Priority of the SSC interrupt
    #define SSC_PRIORITY        7
#endif

/// Number of available audio buffers.
#define BUFFER_NUMBER   5
/// Size of one buffer in bytes.
#define BUFFER_SIZE     (AUDDSpeakerDriver_BYTESPERFRAME + \
                         AUDDSpeakerDriver_BYTESPERSUBFRAME)

/// SPI configuration value.
#define SPI_CONFIGURATION   AT91C_SPI_MSTR | AT91C_SPI_PS_FIXED | SPI_PCS(BOARD_AT73C213_SPI_NPCS)

/// Delay in ms for starting the DAC transmission after a frame has been received.
#define DAC_DELAY           2

/// Use for power management
#define STATE_IDLE    0
/// The USB device is in suspend state
#define STATE_SUSPEND 4
/// The USB device is in resume state
#define STATE_RESUME  5

//------------------------------------------------------------------------------
//         Internal variables
//------------------------------------------------------------------------------
/// State of USB, for suspend and resume
unsigned char USBState = STATE_IDLE;

/// Data buffers for receiving audio frames from the USB host.
static unsigned char buffers[BUFFER_NUMBER][BUFFER_SIZE];
/// Number of bytes stored in each data buffer.
static volatile unsigned int bufferSizes[BUFFER_NUMBER];
/// Next buffer in which USB data can be stored.
static volatile unsigned int inBufferIndex = 0;
/// Next buffer which should be sent to the DAC.
static volatile unsigned int outBufferIndex = 0;
/// Number of buffers that can be sent to the DAC.
static volatile unsigned int numBuffersToSend = 0;

/// Current state of the DAC transmission.
static volatile unsigned int isDacActive = 0;
/// Number of buffers to wait for before the DAC starts to transmit data.
static volatile unsigned int dacDelay;

/// List of pins to configure for the application.
static const Pin pins[] = {
#if defined(at91sam7seek)
    PIN_PCK2,
#elif defined(at91sam9260)
    PIN_PCK0,
#endif
    BOARD_AT73C213_SPI_PINS,
    BOARD_AT73C213_SSC_PINS};

//------------------------------------------------------------------------------
//         VBus monitoring (optional)
//------------------------------------------------------------------------------
#if defined(PIN_USB_VBUS)

#define VBUS_CONFIGURE()  VBus_Configure()

/// VBus pin instance.
static const Pin pinVbus = PIN_USB_VBUS;

//------------------------------------------------------------------------------
/// Handles interrupts coming from PIO controllers.
//------------------------------------------------------------------------------
static void ISR_Vbus(const Pin *pPin)
{
    // Check current level on VBus
    if (PIO_Get(&pinVbus)) {

        TRACE_INFO("VBUS conn\n\r");
        USBD_Connect();
    }
    else {

        TRACE_INFO("VBUS discon\n\r");
        USBD_Disconnect();
    }
}

//------------------------------------------------------------------------------
/// Configures the VBus pin to trigger an interrupt when the level on that pin
/// changes.
//------------------------------------------------------------------------------
static void VBus_Configure( void )
{
    TRACE_INFO("VBus configuration\n\r");

    // Configure PIO
    PIO_Configure(&pinVbus, 1);
    PIO_ConfigureIt(&pinVbus, ISR_Vbus);
    PIO_EnableIt(&pinVbus);

    // Check current level on VBus
    if (PIO_Get(&pinVbus)) {

        // if VBUS present, force the connect
        TRACE_INFO("VBUS conn\n\r");
        USBD_Connect();
    }
    else {
        TRACE_INFO("VBUS discon\n\r");        
        USBD_Disconnect();
    }           
}

#else
    #define VBUS_CONFIGURE()    USBD_Connect()
#endif //#if defined(PIN_USB_VBUS)

#if defined (CP15_PRESENT)
//------------------------------------------------------------------------------
/// Put the CPU in 32kHz, disable PLL, main oscillator
/// Put voltage regulator in standby mode
//------------------------------------------------------------------------------
void LowPowerMode(void)
{
    PMC_CPUInIdleMode();
}
//------------------------------------------------------------------------------
/// Put voltage regulator in normal mode
/// Return the CPU to normal speed 48MHz, enable PLL, main oscillator
//------------------------------------------------------------------------------
void NormalPowerMode(void)
{
}

#elif defined(at91sam7a3)
//------------------------------------------------------------------------------
/// Put the CPU in 32kHz, disable PLL, main oscillator
//------------------------------------------------------------------------------
void LowPowerMode(void)
{
    // MCK=48MHz to MCK=32kHz
    // MCK = SLCK/2 : change source first from 48 000 000 to 18. / 2 = 9M
    AT91C_BASE_PMC->PMC_MCKR = AT91C_PMC_PRES_CLK_2;
    while( !( AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY ) );
    // MCK=SLCK : then change prescaler
    AT91C_BASE_PMC->PMC_MCKR = AT91C_PMC_CSS_SLOW_CLK;
    while( !( AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY ) );
    // disable PLL
    AT91C_BASE_PMC->PMC_PLLR = 0;
    // Disable Main Oscillator
    AT91C_BASE_PMC->PMC_MOR = 0;

    PMC_DisableProcessorClock();
}
//------------------------------------------------------------------------------
/// Return the CPU to normal speed 48MHz, enable PLL, main oscillator
//------------------------------------------------------------------------------
void NormalPowerMode(void)
{
    // MCK=32kHz to MCK=48MHz
    // enable Main Oscillator
    AT91C_BASE_PMC->PMC_MOR = (( (AT91C_CKGR_OSCOUNT & (0x06 <<8)) | AT91C_CKGR_MOSCEN ));
    while( !( AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MOSCS ) );

    // enable PLL@96MHz
    AT91C_BASE_PMC->PMC_PLLR = ((AT91C_CKGR_DIV & 0x0E) |
         (AT91C_CKGR_PLLCOUNT & (28<<8)) |
         (AT91C_CKGR_MUL & (0x48<<16)));
    while( !( AT91C_BASE_PMC->PMC_SR & AT91C_PMC_LOCK ) );
    while( !( AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY ) );
    AT91C_BASE_CKGR->CKGR_PLLR |= AT91C_CKGR_USBDIV_1 ;
    // MCK=SLCK/2 : change prescaler first
    AT91C_BASE_PMC->PMC_MCKR = AT91C_PMC_PRES_CLK_2;
    while( !( AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY ) );
    // MCK=PLLCK/2 : then change source
    AT91C_BASE_PMC->PMC_MCKR |= AT91C_PMC_CSS_PLL_CLK  ;
    while( !( AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY ) );
}

#elif defined (at91sam7se)
//------------------------------------------------------------------------------
/// Put the CPU in 32kHz, disable PLL, main oscillator
/// Put voltage regulator in standby mode
//------------------------------------------------------------------------------
void LowPowerMode(void)
{
    // MCK=48MHz to MCK=32kHz
    // MCK = SLCK/2 : change source first from 48 000 000 to 18. / 2 = 9M
    AT91C_BASE_PMC->PMC_MCKR = AT91C_PMC_PRES_CLK_2;
    while( !( AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY ) );

    // MCK=SLCK : then change prescaler
    AT91C_BASE_PMC->PMC_MCKR = AT91C_PMC_CSS_SLOW_CLK;
    while( !( AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY ) );

    // disable PCK
    AT91C_BASE_PMC->PMC_PCKR[2] = 0;

    // disable PLL
    AT91C_BASE_PMC->PMC_PLLR = 0;

    // Disable Main Oscillator
    AT91C_BASE_PMC->PMC_MOR = 0;

    // Voltage regulator in standby mode : Enable VREG Low Power Mode
    AT91C_BASE_VREG->VREG_MR |= AT91C_VREG_PSTDBY;

    PMC_DisableProcessorClock();
}
//------------------------------------------------------------------------------
/// Put voltage regulator in normal mode
/// Return the CPU to normal speed 48MHz, enable PLL, main oscillator
//------------------------------------------------------------------------------
void NormalPowerMode(void)
{
    // Voltage regulator in normal mode : Disable VREG Low Power Mode
    AT91C_BASE_VREG->VREG_MR &= ~AT91C_VREG_PSTDBY;

    // Keep MCK=32kHz
    // enable Main Oscillator
    AT91C_BASE_PMC->PMC_MOR = (( (AT91C_CKGR_OSCOUNT & (0x06 <<8)) | AT91C_CKGR_MOSCEN ));
    while( !( AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MOSCS ) );

    // Switch to main oscillator
    AT91C_BASE_PMC->PMC_MCKR = (AT91C_BASE_PMC->PMC_MCKR & ~AT91C_PMC_CSS) | AT91C_PMC_CSS_MAIN_CLK;
    while ((AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY) != AT91C_PMC_MCKRDY);
    AT91C_BASE_PMC->PMC_MCKR = AT91C_PMC_CSS_MAIN_CLK;
    while ((AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY) != AT91C_PMC_MCKRDY);

    // enable PLL@96MHz
    AT91C_BASE_PMC->PMC_PLLR = ((AT91C_CKGR_DIV & 0x0E) |
         (AT91C_CKGR_PLLCOUNT & (28<<8)) |
         (AT91C_CKGR_MUL & (0x48<<16)));
    while( !( AT91C_BASE_PMC->PMC_SR & AT91C_PMC_LOCK ) );
    while( !( AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY ) );
    AT91C_BASE_CKGR->CKGR_PLLR |= AT91C_CKGR_USBDIV_1 ;

    // enable PCK for Codec
    AT91C_BASE_PMC->PMC_PCKR[2] = AT91C_PMC_CSS_MAIN_CLK;
    AT91C_BASE_PMC->PMC_SCER = AT91C_PMC_PCK2;
    while ((AT91C_BASE_PMC->PMC_SR & AT91C_PMC_PCK2RDY) == 0);
}

#elif defined (at91sam7s)
//------------------------------------------------------------------------------
/// Put the CPU in 32kHz, disable PLL, main oscillator
/// Put voltage regulator in standby mode
//------------------------------------------------------------------------------
void LowPowerMode(void)
{
    // MCK=48MHz to MCK=32kHz
    // MCK = SLCK/2 : change source first from 48 000 000 to 18. / 2 = 9M
    AT91C_BASE_PMC->PMC_MCKR = AT91C_PMC_PRES_CLK_2;
    while( !( AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY ) );
    // MCK=SLCK : then change prescaler
    AT91C_BASE_PMC->PMC_MCKR = AT91C_PMC_CSS_SLOW_CLK;
    while( !( AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY ) );
    // disable PLL
    AT91C_BASE_PMC->PMC_PLLR = 0;
    // Disable Main Oscillator
    AT91C_BASE_PMC->PMC_MOR = 0;

    // Voltage regulator in standby mode : Enable VREG Low Power Mode
    AT91C_BASE_VREG->VREG_MR |= AT91C_VREG_PSTDBY;

    PMC_DisableProcessorClock();
}

//------------------------------------------------------------------------------
/// Put voltage regulator in normal mode
/// Return the CPU to normal speed 48MHz, enable PLL, main oscillator
//------------------------------------------------------------------------------
void NormalPowerMode(void)
{
    // Voltage regulator in normal mode : Disable VREG Low Power Mode
    AT91C_BASE_VREG->VREG_MR &= ~AT91C_VREG_PSTDBY;

    // MCK=32kHz to MCK=48MHz
    // enable Main Oscillator
    AT91C_BASE_PMC->PMC_MOR = (( (AT91C_CKGR_OSCOUNT & (0x06 <<8)) | AT91C_CKGR_MOSCEN ));
    while( !( AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MOSCS ) );

    // enable PLL@96MHz
    AT91C_BASE_PMC->PMC_PLLR = ((AT91C_CKGR_DIV & 0x0E) |
         (AT91C_CKGR_PLLCOUNT & (28<<8)) |
         (AT91C_CKGR_MUL & (0x48<<16)));
    while( !( AT91C_BASE_PMC->PMC_SR & AT91C_PMC_LOCK ) );
    while( !( AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY ) );
    AT91C_BASE_CKGR->CKGR_PLLR |= AT91C_CKGR_USBDIV_1 ;
    // MCK=SLCK/2 : change prescaler first
    AT91C_BASE_PMC->PMC_MCKR = AT91C_PMC_PRES_CLK_2;
    while( !( AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY ) );
    // MCK=PLLCK/2 : then change source
    AT91C_BASE_PMC->PMC_MCKR |= AT91C_PMC_CSS_PLL_CLK  ;
    while( !( AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY ) );

}

#elif defined (at91sam7x) || defined (at91sam7xc)
//------------------------------------------------------------------------------
/// Put the CPU in 32kHz, disable PLL, main oscillator
/// Put voltage regulator in standby mode
//------------------------------------------------------------------------------
void LowPowerMode(void)
{
    // MCK=48MHz to MCK=32kHz
    // MCK = SLCK/2 : change source first from 48 000 000 to 18. / 2 = 9M
    AT91C_BASE_PMC->PMC_MCKR = AT91C_PMC_PRES_CLK_2;
    while( !( AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY ) );
    // MCK=SLCK : then change prescaler
    AT91C_BASE_PMC->PMC_MCKR = AT91C_PMC_CSS_SLOW_CLK;
    while( !( AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY ) );
    // disable PLL
    AT91C_BASE_PMC->PMC_PLLR = 0;
    // Disable Main Oscillator
    AT91C_BASE_PMC->PMC_MOR = 0;

    // Voltage regulator in standby mode : Enable VREG Low Power Mode
    AT91C_BASE_VREG->VREG_MR |= AT91C_VREG_PSTDBY;

    PMC_DisableProcessorClock();
}

//------------------------------------------------------------------------------
/// Put voltage regulator in normal mode
/// Return the CPU to normal speed 48MHz, enable PLL, main oscillator
//------------------------------------------------------------------------------
void NormalPowerMode(void)
{
    // Voltage regulator in normal mode : Disable VREG Low Power Mode
    AT91C_BASE_VREG->VREG_MR &= ~AT91C_VREG_PSTDBY;

    // MCK=32kHz to MCK=48MHz
    // enable Main Oscillator
    AT91C_BASE_PMC->PMC_MOR = (( (AT91C_CKGR_OSCOUNT & (0x06 <<8)) | AT91C_CKGR_MOSCEN ));
    while( !( AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MOSCS ) );

    // enable PLL@96MHz
    AT91C_BASE_PMC->PMC_PLLR = ((AT91C_CKGR_DIV & 0x0E) |
         (AT91C_CKGR_PLLCOUNT & (28<<8)) |
         (AT91C_CKGR_MUL & (0x48<<16)));
    while( !( AT91C_BASE_PMC->PMC_SR & AT91C_PMC_LOCK ) );
    while( !( AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY ) );
    AT91C_BASE_CKGR->CKGR_PLLR |= AT91C_CKGR_USBDIV_1 ;
    // MCK=SLCK/2 : change prescaler first
    AT91C_BASE_PMC->PMC_MCKR = AT91C_PMC_PRES_CLK_2;
    while( !( AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY ) );
    // MCK=PLLCK/2 : then change source
    AT91C_BASE_PMC->PMC_MCKR |= AT91C_PMC_CSS_PLL_CLK  ;
    while( !( AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY ) );
}

#endif

//------------------------------------------------------------------------------
//         Callbacks re-implementation
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/// Invoked when the USB device leaves the Suspended state. By default,
/// configures the LEDs.
//------------------------------------------------------------------------------
void USBDCallbacks_Resumed(void)
{
    // Initialize LEDs
    LED_Configure(USBD_LEDPOWER);
    LED_Set(USBD_LEDPOWER);
    LED_Configure(USBD_LEDUSB);
    LED_Clear(USBD_LEDUSB);
    USBState = STATE_RESUME;
}

//------------------------------------------------------------------------------
/// Invoked when the USB device gets suspended. By default, turns off all LEDs.
//------------------------------------------------------------------------------
void USBDCallbacks_Suspended(void)
{
    // Turn off LEDs
    LED_Clear(USBD_LEDPOWER);
    LED_Clear(USBD_LEDUSB);
    if (USBD_GetState() < USBD_STATE_CONFIGURED)
        USBState = STATE_SUSPEND;
}


//------------------------------------------------------------------------------
/// Invoked when an audio channel get muted or unmuted. Mutes/unmutes the
/// channel at the DAC level.
/// \param channel  Pointer to an AUDDSpeakerChannel instance.
/// \param muted  Indicates the new mute status of the channel.
//------------------------------------------------------------------------------
void AUDDSpeakerChannel_MuteChanged(
    AUDDSpeakerChannel *channel,
    unsigned char muted)
{
    // Master channel
    if (AUDDSpeakerChannel_GetNumber(channel) == AUDDSpeakerDriver_MASTERCHANNEL) {

        if (muted) {

            TRACE_INFO("MuteMaster ");
            AT73C213_SetMuteStatus(1, 1);
        }
        else {

            TRACE_INFO("UnmuteMaster ");
            AT73C213_SetMuteStatus(0, 0);
        }
    }
}

//------------------------------------------------------------------------------
//         Internal functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Handles interrupts coming from the SSC. Sends remaining audio buffers
/// or stops the DAC transmission.
//------------------------------------------------------------------------------
static void ISR_SSC(void)
{
    if ((BOARD_AT73C213_SSC->SSC_SR & AT91C_SSC_TXBUFE) != 0) {

        // End of transmission
        TRACE_DEBUG("End ");
        SSC_DisableInterrupts(BOARD_AT73C213_SSC,
                              AT91C_SSC_TXBUFE
                              | AT91C_SSC_ENDTX);
        BOARD_AT73C213_SSC->SSC_PTCR = AT91C_PDC_TXTDIS;
        isDacActive = 0;
    }
    else if (numBuffersToSend > 0) {

        // Check the number of available buffers
        if (numBuffersToSend > DAC_DELAY) {

            // Strip one sample on all channels
            bufferSizes[outBufferIndex] -= AUDDSpeakerDriver_NUMCHANNELS;
        }
        else if (numBuffersToSend < DAC_DELAY) {

            // Copy the last sample on all channels
            memcpy(&buffers[outBufferIndex][bufferSizes[outBufferIndex]],
                   &buffers[outBufferIndex][bufferSizes[outBufferIndex] - AUDDSpeakerDriver_BYTESPERSUBFRAME],
                   AUDDSpeakerDriver_BYTESPERSUBFRAME);
            bufferSizes[outBufferIndex] += AUDDSpeakerDriver_NUMCHANNELS;
        }

        // Load next buffer
        SSC_WriteBuffer(BOARD_AT73C213_SSC,
                        buffers[outBufferIndex],
                        bufferSizes[outBufferIndex]);
        outBufferIndex = (outBufferIndex + 1) % BUFFER_NUMBER;
        numBuffersToSend--;
    }
    else {

        SSC_DisableInterrupts(BOARD_AT73C213_SSC,
                              AT91C_SSC_ENDTX);
    }
}

//------------------------------------------------------------------------------
/// Invoked when a frame has been received.
//------------------------------------------------------------------------------
static void FrameReceived(unsigned int unused,
                          char status,
                          unsigned int transferred,
                          unsigned int remaining)
{
    bufferSizes[inBufferIndex] = transferred / AUDDSpeakerDriver_BYTESPERSAMPLE;
    inBufferIndex = (inBufferIndex + 1) % BUFFER_NUMBER;
    numBuffersToSend++;

    // Start DAc transmission if necessary
    if (!isDacActive) {

        TRACE_DEBUG("Start ");
        dacDelay = DAC_DELAY;
        isDacActive = 1;
    }
    // Wait until a few buffers have been received
    else if (dacDelay > 0) {

        dacDelay--;
    }
    // Start sending buffers
    else if ((BOARD_AT73C213_SSC->SSC_PTSR & AT91C_PDC_TXTEN) == 0) {

        SSC_WriteBuffer(BOARD_AT73C213_SSC,
                        buffers[outBufferIndex],
                        bufferSizes[outBufferIndex]);
        outBufferIndex = (outBufferIndex + 1) % BUFFER_NUMBER;
        numBuffersToSend--;
        SSC_WriteBuffer(BOARD_AT73C213_SSC,
                        buffers[outBufferIndex],
                        bufferSizes[outBufferIndex]);
        outBufferIndex = (outBufferIndex + 1) % BUFFER_NUMBER;
        numBuffersToSend--;

        SSC_EnableInterrupts(BOARD_AT73C213_SSC,
                             AT91C_SSC_TXBUFE | AT91C_SSC_ENDTX);
    } 

    // Receive next packet
    AUDDSpeakerDriver_Read(buffers[inBufferIndex],
                           AUDDSpeakerDriver_BYTESPERFRAME,
                           (TransferCallback) FrameReceived,
                           0); // No optional argument
}

//------------------------------------------------------------------------------
//         Exported functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Starts the driver and waits for an audio input stream to forward to the SSC.
//------------------------------------------------------------------------------
int main(void)
{
    TRACE_CONFIGURE(DBGU_STANDARD, 115200, BOARD_MCK);
    printf("-- USB Device Audio Speaker Project %s --\n\r", SOFTPACK_VERSION);
    printf("-- %s\n\r", BOARD_NAME);
    printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);

    // Initialize all PIOs
    PIO_Configure(pins, PIO_LISTSIZE(pins));

    // If there is on board power, switch it off
  #ifdef PIN_USB_POWER_ENB
  { const Pin pinUsbPwr = PIN_USB_POWER_ENB;
    PIO_Configure(&pinUsbPwr, 1);
  }
  #endif

    // Configure and enable the SPI (required for accessing the DAC)
    SPI_Configure(BOARD_AT73C213_SPI, BOARD_AT73C213_SPI_ID, SPI_CONFIGURATION);
    SPI_Enable(BOARD_AT73C213_SPI);

#if defined(at91sam9260)
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
#else
    NormalPowerMode();
#endif

    TRACE_CONFIGURE(DBGU_STANDARD, 115200, MCK);

    // Initialize the audio DAC
    // Configure and enable the DAC
    AT73C213_Enable(AUDDSpeakerDriver_SAMPLERATE,
                    AUDDSpeakerDriver_BYTESPERSAMPLE,
                    AUDDSpeakerDriver_NUMCHANNELS,
                    MCK_SEL,
                    MCK);

    // Configure and enable the SSC interrupt
    IRQ_ConfigureIT(BOARD_AT73C213_SSC_ID,
                    SSC_PRIORITY,
                    ISR_SSC);
    IRQ_EnableIT(BOARD_AT73C213_SSC_ID);
    
    // USB audio driver initialization
    AUDDSpeakerDriver_Initialize();

    // connect if needed
    VBUS_CONFIGURE();
    
    // Infinite loop
    while (1) {
    
        if( USBState == STATE_SUSPEND ) {
            TRACE_DEBUG("suspend  !\n\r");
            USBState = STATE_IDLE;
            LowPowerMode();
        }
        if( USBState == STATE_RESUME ) {
            // Return in normal MODE
            NormalPowerMode();
            USBState = STATE_IDLE;
            TRACE_DEBUG("resume !\n\r");
        }
        
        if (USBD_GetState() < USBD_STATE_CONFIGURED)
            continue;
        // Read the incoming audio stream
        AUDDSpeakerDriver_Read(buffers[inBufferIndex],
                               AUDDSpeakerDriver_BYTESPERFRAME,
                               (TransferCallback) FrameReceived,
                               0); // No optional argument
    }

    return 0;
}

