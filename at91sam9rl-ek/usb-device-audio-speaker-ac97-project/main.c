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
/// \dir "USB audio speaker with AC97"
///
/// !!!Purpose
///
/// The USB Audio Speaker AC97 Project will help you to get familiar with the
/// USB Device Port(UDP) and AC97 interface on AT91SAM microcontrollers. Also
/// it can help you to be familiar with the USB Framework that is used for
/// rapid development of USB-compliant class drivers such as USB Audio Device
/// class.
///
/// You can find following information depends on your needs:
/// - Sample usage of USB Audio Device Class driver and AC97 driver.
/// - USB Audio Class driver development based on the AT91 USB Framework.
/// - USB enumerate sequence, the standard and class-specific descriptors and
///   requests handling.
/// - The initialize sequence and usage of UDP interface.
/// - The initialize sequence and usage of AC97 interface with PDC.
///
/// !See
/// - ac97c: AC97 interface driver
/// - usb: USB Framework, Audio Device Class driver and UDP interface driver
///     - "AT91 USB device framework"
///        - "USBD API"
///     - "audio-speaker"
///
/// !!!Requirements
///
/// This package can be used with all Atmel evaluation kits that have both
/// UDP and AC97 interface.
///
/// The current supported board list:
/// - at91sam9263-ek
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
///    \code
///     -- USB Desktop speaker (AC97) driver xxx --
///     -- AT91xxxxxx-xx
///     -- Compiled: xxx xx xxxx xx:xx:xx --
///    \endcode
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
/// usb-device-audio-speaker-ac97-project
///
/// !Contents
///
/// The code can be roughly broken down as follows:
///    - Configuration functions
///    - Interrupt handlers
///    - Callback functions
///    - The main function, which implements the program behavior
///
/// Please refer to the list of functions in the #Overview# tab of this unit
/// for more detailed information.
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//         Headers
//-----------------------------------------------------------------------------

#include <board.h>
#include <pio/pio.h>
#include <pio/pio_it.h>
#include <irq/irq.h>
#include <ac97c/ac97c.h>
#include <utility/trace.h>
#include <utility/assert.h>
#include <utility/led.h>
#include <components/codec-ad1981b/ad1981b.h>
#include <usb/device/core/USBD.h>
#include <usb/device/audio-speaker/AUDDSpeakerDriver.h>
#include <usb/device/audio-speaker/AUDDSpeakerChannel.h>
#include <pmc/pmc.h>

#include <string.h>

//-----------------------------------------------------------------------------
//         Definitions
//-----------------------------------------------------------------------------

/// USB audio data buffers
/// Number of available buffers.
#define BUFFER_NUMBER           5
/// Size of one buffer in bytes.
#define BUFFER_SIZE             (AUDDSpeakerDriver_BYTESPERFRAME + \
                                 AUDDSpeakerDriver_BYTESPERSUBFRAME)

/// Delay in ms for starting the DAC transmission after a frame received
#define DAC_DELAY               2

/// Use for power management
#define STATE_IDLE    0
/// The USB device is in suspend state
#define STATE_SUSPEND 4
/// The USB device is in resume state
#define STATE_RESUME  5

//-----------------------------------------------------------------------------
//         Internal variables
//-----------------------------------------------------------------------------
/// State of USB, for suspend and resume
unsigned char USBState = STATE_IDLE;

/// Pins for AC97 component
static const Pin pinsAc97[] = {PINS_AC97};

/// Command buffer used during codec initialization.
static const unsigned int pInitBuffer[] = {

    // 0.  Reset
    AD1981B_CMD(AD1981B_RESET, 0x0090),
    // 1.  Master Volume 
    AD1981B_CMD(AD1981B_MASTER, 0),
    // 2.  Headphone Volume
    AD1981B_CMD(AD1981B_HEADPHONE, 0x0E0E),
    // 3.  PCM Volume
    AD1981B_CMD(AD1981B_PCM, 0),
    // 4.  Extern Audio set as variable rate audio mode
    AD1981B_CMD(AD1981B_EXTAUDIO, AD1981B_EXTAUDIO_VRA),
    // 5.  General Purpose
    AD1981B_CMD(AD1981B_GENERAL_PURPOSE, 0x00),
    // 6. Serial configuration
    AD1981B_CMD(AD1981B_SERIAL_CONFIG, 0x7000)
};

/// Buffers for USB audio
/// Data buffers for receiving audio frames from the USB host
static unsigned char buffers[BUFFER_NUMBER][BUFFER_SIZE];
/// Number of samples stored in each data buffer.
static volatile unsigned short bufferSizes[BUFFER_NUMBER];
/// Next buffer in which USB data can be stored.
static volatile unsigned int inBufferIndex = 0;
/// Next buffer which should be sent to the DAC.
static volatile unsigned int outBufferIndex = 0;
/// Number of buffers that can be sent to the DAC.
static volatile unsigned int numBuffersToSend = 0;

/// DAC
/// Current state of the DAC transmission.
static volatile unsigned int isDacActive = 0;
/// Number of buffers to wait for before the DAC starts to transmit data.
static volatile unsigned int dacDelay;

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
    USBState = STATE_SUSPEND;
}


//-----------------------------------------------------------------------------
/// Invoked when an audio channel get muted or unmuted. Mutes/unmutes the DAC
/// level
/// \param channel Pointer to an AUDDSpeakerChannel instance.
/// \param muted   Indicates the new mute status of the channel.
//-----------------------------------------------------------------------------
void AUDDSpeakerChannel_MuteChanged(AUDDSpeakerChannel *channel,
                                    unsigned char muted)
{
    unsigned int sample;

    // Master channel
    if (AUDDSpeakerChannel_GetNumber(channel) ==
        AUDDSpeakerDriver_MASTERCHANNEL) {

        if (muted) {

            TRACE_WARNING( "MuteMaster ");
            sample = AD1981B_CMD(AD1981B_HEADPHONE,
                                 AD1981B_HEADPHONE_HPM | AD1981B_HEADPHONE_RM);
        }
        else {

            TRACE_WARNING( "UnmuteMaster ");
            sample = AD1981B_CMD(AD1981B_HEADPHONE, 0x0E0E);
        }

        AC97C_Transfer(AC97C_CODEC_TRANSFER,
                       (unsigned char *) &sample, 1, 0, 0);
    }
}

//-----------------------------------------------------------------------------
//         Internal functions
//-----------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Playback invoked whenever the AC97 has finished playing a sound.
/// \param unused  Unused parameter.
/// \param status  Transfer result.
//------------------------------------------------------------------------------
static void PlaybackFinished(unsigned int unused, unsigned char status)
{
    if (numBuffersToSend > 0) {

        // Check the number of available buffers
        if (numBuffersToSend > DAC_DELAY) {

            // Strip one sample on all channels
            bufferSizes[outBufferIndex] -= AUDDSpeakerDriver_NUMCHANNELS;
        }
        else if (numBuffersToSend < DAC_DELAY) {

            // Copy the last sample on all channels
            memcpy(&buffers[outBufferIndex][bufferSizes[outBufferIndex]
                                          - AUDDSpeakerDriver_BYTESPERSUBFRAME],
                 &buffers[outBufferIndex][bufferSizes[outBufferIndex]],
                 AUDDSpeakerDriver_BYTESPERSUBFRAME);
            bufferSizes[outBufferIndex] += AUDDSpeakerDriver_BYTESPERSUBFRAME;
        }

        // Load next buffer
        AC97C_Transfer(AC97C_CHANNEL_A_TRANSMIT, 
                       (unsigned char *) buffers[outBufferIndex],
                       bufferSizes[outBufferIndex],
                       (Ac97Callback) PlaybackFinished,
                       0);

        outBufferIndex = (outBufferIndex + 1) % BUFFER_NUMBER;
        numBuffersToSend--;
    }
    else {

        // End of transmission
        TRACE_INFO("End ");
        isDacActive = 0;
    }
}


//-----------------------------------------------------------------------------
/// Invoked when a frame has been received.
//-----------------------------------------------------------------------------
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

        TRACE_INFO("Start ");
        dacDelay = DAC_DELAY;
        isDacActive = 1;
    }
    // Wait until a few buffers have been received
    else if (dacDelay > 0) {

        dacDelay--;
    }
    // Start sending buffers
    else if (AC97C_IsFinished(AC97C_CHANNEL_A_TRANSMIT)) {

        AC97C_Transfer(AC97C_CHANNEL_A_TRANSMIT, 
                       (unsigned char *) buffers[outBufferIndex],
                       bufferSizes[outBufferIndex],
                       (Ac97Callback) PlaybackFinished,
                       0);

        outBufferIndex = (outBufferIndex + 1) % BUFFER_NUMBER;
        numBuffersToSend--;
    }

    // Receive next packet
    AUDDSpeakerDriver_Read(buffers[inBufferIndex],
                           AUDDSpeakerDriver_BYTESPERFRAME,
                           (TransferCallback) FrameReceived,
                           0); // No optional argument

}

//-----------------------------------------------------------------------------
//         Exported functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/// Starts the driver and waits for an audio input stream to forward to AC97
//-----------------------------------------------------------------------------
int main()
{
    unsigned int vendorId;

    // Initialize traces
    TRACE_CONFIGURE(DBGU_STANDARD, 115200, BOARD_MCK);
    printf("-- USB Device Audio Speaker AC97 Project %s --\n\r", SOFTPACK_VERSION);
    printf("-- %s\n\r", BOARD_NAME);
    printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);

    // If they are present, configure Vbus & Wake-up pins
    PIO_InitializeInterrupts(0);

    // If there is on board power, switch it off
  #ifdef PIN_USB_POWER_ENB
  { const Pin pinUsbPwr = PIN_USB_POWER_ENB;
    PIO_Configure(&pinUsbPwr, 1);
  }
  #endif

    // AC97 Audio driver & pins initialize
    PIO_Configure(pinsAc97, PIO_LISTSIZE(pinsAc97));

    // Initialize AC97 driver
    AC97C_Configure();
    AC97C_AssignOutputSlots(AC97C_CHANNEL_A, (1 << 3) | (1 << 4));
    AC97C_ConfigureChannel(AC97C_CHANNEL_A, AT91C_AC97C_CEN);

    // Check that the codec is present by reading the vendor ID
    vendorId = AC97C_ReadCodec(AD1981B_VENDOR_ID1);
    ASSERT(vendorId == AD1981B_ID_1,
        "-F- Codec probe: Wrong AD1981B_ID1 value.\n\r");
    
    vendorId = AC97C_ReadCodec(AD1981B_VENDOR_ID2);
    ASSERT(vendorId == AD1981B_ID_2,
        "-F- Codec probe: Wrong AD1981B_ID2 value.\n\r");
    
    TRACE_INFO("Codec probed correctly\r\n");

    // Initialize the codec
    AC97C_Transfer(AC97C_CODEC_TRANSFER,
                   (unsigned char *) pInitBuffer,
                   sizeof(pInitBuffer)/sizeof(int),
                   0,
                   0);
    while (!AC97C_IsFinished(AC97C_CODEC_TRANSFER));

    // Change channel size to 16 bits
    AC97C_SetChannelSize(AC97C_CHANNEL_A, AUDDSpeakerDriver_BYTESPERSAMPLE<<3);

    // Set sample rate
    AC97C_WriteCodec(AD1981B_PMC_DAC, AUDDSpeakerDriver_SAMPLERATE);

    // USB audio driver initialization
    AUDDSpeakerDriver_Initialize();

    // connect if needed
    VBUS_CONFIGURE();
    while (USBD_GetState() < USBD_STATE_CONFIGURED);

    // Infinite loop
    while (1) {

        if( USBState == STATE_SUSPEND ) {
            TRACE_DEBUG("suspend  !\n\r");
            LowPowerMode();
            USBState = STATE_IDLE;
        }
        if( USBState == STATE_RESUME ) {
            // Return in normal MODE
            TRACE_DEBUG("resume !\n\r");
            NormalPowerMode();
            USBState = STATE_IDLE;
        }
    
        // Read the incoming audio stream
        AUDDSpeakerDriver_Read(buffers[inBufferIndex],
                       AUDDSpeakerDriver_BYTESPERFRAME,
                       (TransferCallback) FrameReceived,
                       0); // No optional argument
    }
}

