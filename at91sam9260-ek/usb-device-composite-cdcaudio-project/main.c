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
/// \dir "USB COMPOSITE CDC+AUDIO project"
///
/// !!!Purpose
///
/// The USB COMPOSITE Project will help you to get familiar with the
/// USB Device Port(UDP)interface and also some of the other interfaces in
/// AT91SAM microcontrollers. Also it can help you to be familiar with the USB
/// Framework that is used for rapid development of USB-compliant class
/// drivers such as USB Communication Device class (CDC), and how to combine
/// two USB functions to a single composite device (such as CDC + MSD).
///
/// You can find following information depends on your needs:
/// - Sample usage of USB Device Framework.
/// - USB COMPOSITE device and functions driver development based on the AT91
///   USB Device Framework and other re-usable class driver code.
/// - USB enumerate sequence, the standard and class-specific descriptors and
///   requests handling.
/// - The initialize sequence and usage of UDP interface.
///
/// !See
/// - pio: Pin configurations and peripheral configure.
/// - memories: Storage Media interface for MSD
/// - ssc: SSC interface driver
/// - "dac-at73c213": I2S codec at73c213 driver
/// - usb: USB Device Framework, USB CDC driver and UDP interface driver
///    - "AT91 USB device framework"
///        - "USBD API"
///    - "composite"
///       - "USB COMPOSITE Device"
///     - "audio-speaker"
///       - "USB Audio Speaker Device"
///    - "cdc-serial"
///       - "USB CDC Serial Device"
/// - projects:
///    - "usb-device-cdc-serial-project"
///    - "usb-device-audio-speaker-project"
///
/// !!!Requirements
///
/// This package can be used with some of Atmel evaluation kits that have UDP
/// interface, depending on the functions included.
///
/// The current supported board list:
///    - at91sam7se-ek
///    - at91sam9260-ek
///
///  !!!Windows Driver Update
///
/// The composite device is generally supported by Microsoft windows, but some
/// patches are needed for muti-interface functions such as CDC & Audio. The
/// example composite devices are tested under windows XP (SP3). For CDC
/// serial port, additional windows driver file (CompositeCDCSerial.inf) can
/// be found at at91lib\usb\device\composite\drv.
///
/// The following is alternate update to fix the composite device support
/// on windows XP:
///
/// !!Install Windows Service Pack 3 (SP3)
///
/// All the fixes for USB generic driver are included in window XP service pack
/// 3. It can be found at
/// http://technet.microsoft.com/zh-cn/windows/bb794714(en-us).aspx .
///
/// !!Install Windows Hot Fixes
///
/// Two hot fixes are necessary for window to recognize the composite device
/// correctly:
///
/// -# http://support.microsoft.com/kb/814560
/// -# http://support.microsoft.com/kb/918365
/// 
/// !!!Description
///
/// When an EK running this program connected to a host (PC for example), with
/// USB cable, host will notice the attachment of a USB %device. No %device
/// driver offered for the %device now.
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
///     \code
///     -- USB Composite Device Project xxx --
///     -- AT91xxxxxx-xx
///     -- Compiled: xxx xx xxxx xx:xx:xx --
///     \endcode
/// -# When connecting USB cable to windows, the LED blinks, and the host
///    reports a new USB %device attachment.
/// -# For the windows driver installation and the test functions, please
///      refer to "USB CDC serial converter" & "USB audio speaker".
/// -# You can use the inf file
///    at91lib\\usb\\device\\composite\\drv\\CompositeCDCSerial.inf
///    to install the CDC serial  port.
///
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/// \unit
///
/// !Purpose
///
/// This file contains all the specific code for the
/// usb-device-composite-cdcaudio-project
///
/// !Contents
///
/// The code can be roughly broken down as follows:
///    - Configuration functions
///       - VBus_Configure
///       - PIO configurations in start of main
///    - Interrupt handlers
///       - ISR_Vbus
///    - Callback functions
///       - USBDCallbacks_RequestReceived
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
#include <irq/irq.h>
#include <usart/usart.h>
#include <utility/trace.h>
#include <utility/led.h>

#include <spi/spi.h>
#include <ssc/ssc.h>
#include <components/dac-at73c213/at73c213.h>
#include <pmc/pmc.h>

#include <usb/device/composite/COMPOSITEDDriver.h>

#include <string.h>

//-----------------------------------------------------------------------------
//      Definitions
//-----------------------------------------------------------------------------

#if defined(at91sam7seek)
 /// Master clock frequency in Hz
 #define MCK             BOARD_MAINOSC
#elif defined(at91sam9260)
 /// Master clock frequency in Hz
 #define MCK             (BOARD_MAINOSC * 8 / 2)
#endif

#if defined(at91sam7seek)
 /// Priority of the SSC interrupt
 #define SSC_PRIORITY        0
#elif defined(at91sam9260)
 /// Priority of the SSC interrupt
 #define SSC_PRIORITY        7
#endif

/// Number of available audio buffers.
#define BUFFER_NUMBER   5
/// Size of one buffer in bytes.
#define BUFFER_SIZE     (AUDD_BYTESPERFRAME + \
                         AUDD_BYTESPERSUBFRAME)

/// SPI configuration value.
#define SPI_CONFIGURATION   \
    AT91C_SPI_MSTR | AT91C_SPI_PS_FIXED | SPI_PCS(BOARD_AT73C213_SPI_NPCS)

/// Delay in ms for starting the DAC transmission after a frame received.
#define DAC_DELAY           2

/// Size in bytes of the buffer used for reading data from the USB & USART
#define DATABUFFERSIZE     BOARD_USB_ENDPOINTS_MAXPACKETSIZE(2)

/// Use for power management
#define STATE_IDLE    0
/// The USB device is in suspend state
#define STATE_SUSPEND 4
/// The USB device is in resume state
#define STATE_RESUME  5

//-----------------------------------------------------------------------------
//      Internal variables
//-----------------------------------------------------------------------------
/// State of USB, for suspend and resume
unsigned char USBState = STATE_IDLE;

//- CDC
/// List of pins that must be configured for use by the application.
static const Pin pinsUsart[] = {PIN_USART0_TXD, PIN_USART0_RXD};

/// Double-buffer for storing incoming USART data.
static unsigned char usartBuffers[2][DATABUFFERSIZE];

/// Current USART buffer index.
static unsigned char usartCurrentBuffer = 0;

/// Buffer for storing incoming USB data.
static unsigned char usbSerialBuffer0[DATABUFFERSIZE];

//- AUDIO
/// Data buffers for receiving audio frames from the USB host.
static unsigned char usbAudBuffers[BUFFER_NUMBER][BUFFER_SIZE];
/// Number of bytes stored in each data buffer.
static volatile unsigned int usbAudBufferSizes[BUFFER_NUMBER];
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
static const Pin pinsAud[] = {
    PINS_DBGU,
#if defined(at91sam7seek)
    PIN_PCK2,
#elif defined(at91sam9260)
    PIN_PCK0,
#endif
    BOARD_AT73C213_SPI_PINS,
    BOARD_AT73C213_SSC_PINS
};

#if defined(usb_CDCHID) || defined(usb_HIDAUDIO) || defined(usb_HIDMSD)
//------------------------------------------------------------------------------
//         Remote wake-up support (optional)
//------------------------------------------------------------------------------ 
#if (BOARD_USB_BMATTRIBUTES == USBConfigurationDescriptor_BUSPOWERED_RWAKEUP) \
    || (BOARD_USB_BMATTRIBUTES == USBConfigurationDescriptor_SELFPOWERED_RWAKEUP)

#define WAKEUP_CONFIGURE()  ConfigureWakeUp()

/// Button for Wake-UP the USB device.
static const Pin pinWakeUp = PIN_PUSHBUTTON_1;

//------------------------------------------------------------------------------
/// Interrupt service routine for the PIT. Debounces the wake-up pin input.
//------------------------------------------------------------------------------
static void ISR_Pit(void)
{
    static unsigned long debounceCounter = DEBOUNCE_TIME;
    unsigned long pisr = 0;

    // Read the PISR
    pisr = PIT_GetStatus() & AT91C_PITC_PITS;

    if (pisr != 0) {

        // Read the PIVR. It acknowledges the IT
        PIT_GetPIVR();
    }

    // Button released
    if (PIO_Get(&pinWakeUp)) {

        debounceCounter = DEBOUNCE_TIME;
    }
    // Button still pressed
    else {

        debounceCounter--;
    }

    // End of debounce time
    if (debounceCounter == 0) {

        debounceCounter = DEBOUNCE_TIME;
        PIT_DisableIT();
        AT91C_BASE_PITC->PITC_PIMR &= ~AT91C_PITC_PITEN;
        COMPOSITEDDriver_RemoteWakeUp();
    }
}

//------------------------------------------------------------------------------
/// Configures the PIT to generate 1ms ticks.
//------------------------------------------------------------------------------
static void ConfigurePit(void)
{
    // Initialize and enable the PIT
    PIT_Init(PIT_PERIOD, BOARD_MCK / 1000000);

    // Disable the interrupt on the interrupt controller
    IRQ_DisableIT(AT91C_ID_SYS);

    // Configure the AIC for PIT interrupts
    IRQ_ConfigureIT(AT91C_ID_SYS, 0, ISR_Pit);

    // Enable the interrupt on the interrupt controller
    IRQ_EnableIT(AT91C_ID_SYS);

    // Enable the interrupt on the pit
    PIT_EnableIT();

    // Enable the pit
    PIT_Enable();
}

//------------------------------------------------------------------------------
/// Interrupt service routine for the remote wake-up pin. Starts the debouncing
/// sequence.
//------------------------------------------------------------------------------
static void WakeUpHandler(const Pin *pin)
{
    TRACE_DEBUG("Wake-up handler\n\r");

    // Check current level on the remote wake-up pin
    if (!PIO_Get(&pinWakeUp)) {

        ConfigurePit();
    }
}

//------------------------------------------------------------------------------
/// Configures the wake-up pin to generate interrupts.
//------------------------------------------------------------------------------
static void ConfigureWakeUp(void)
{
    TRACE_INFO("Wake-up configuration\n\r");

    // Configure PIO
    PIO_Configure(&pinWakeUp, 1);
    PIO_ConfigureIt(&pinWakeUp, WakeUpHandler);
    PIO_EnableIt(&pinWakeUp);
}

#else
    #define WAKEUP_CONFIGURE()
#endif
#endif // (HID defined)

//-----------------------------------------------------------------------------
//         VBus monitoring (optional)
//-----------------------------------------------------------------------------
#if defined(PIN_USB_VBUS)

#define VBUS_CONFIGURE()  VBus_Configure()

/// VBus pin instance.
static const Pin pinVbus = PIN_USB_VBUS;

//-----------------------------------------------------------------------------
/// Handles interrupts coming from PIO controllers.
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
/// Configures the VBus pin to trigger an interrupt when the level on that pin
/// changes.
//-----------------------------------------------------------------------------
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
        TRACE_INFO("conn\n\r");
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
    
    if (USBD_GetState() >= USBD_STATE_CONFIGURED)
        USBState = STATE_SUSPEND;
}

//-----------------------------------------------------------------------------
/// Invoked when an audio channel get muted or unmuted. Mutes/unmutes the
/// channel at the DAC level.
/// \param channel  Pointer to an AUDDSpeakerChannel instance.
/// \param muted  Indicates the new mute status of the channel.
//-----------------------------------------------------------------------------
void AUDDSpeakerChannel_MuteChanged(
    AUDDSpeakerChannel *channel,
    unsigned char muted)
{
    // Master channel
    if (channel->number == AUDD_MASTERCHANNEL) {

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

//-----------------------------------------------------------------------------
//         Internal functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/// Handles interrupts coming from Timer #0.
//-----------------------------------------------------------------------------
static void ISR_Timer0()
{
    unsigned char size;
    unsigned int status = AT91C_BASE_TC0->TC_SR;

    if ((status & AT91C_TC_CPCS) != 0) {
    
        // Flush PDC buffer
        size = DATABUFFERSIZE - AT91C_BASE_US0->US_RCR;
        if (size == 0) {

            AT91C_BASE_TC0->TC_CCR = AT91C_TC_CLKEN | AT91C_TC_SWTRG;
            return;
        }
        AT91C_BASE_US0->US_RCR = 0;
    
        // Send current buffer through the USB
        while (CDCDSerialDriver_Write(0, usartBuffers[usartCurrentBuffer],
                                      size, 0, 0) != USBD_STATUS_SUCCESS);
      #if defined(usb_CDCCDC)
        while (CDCDSerialDriver_Write(1, usartBuffers[usartCurrentBuffer],
                                      size, 0, 0) != USBD_STATUS_SUCCESS);
      #endif
    
        // Restart read on buffer
        USART_ReadBuffer(AT91C_BASE_US0,
                         usartBuffers[usartCurrentBuffer],
                         DATABUFFERSIZE);
        usartCurrentBuffer = 1 - usartCurrentBuffer;
        AT91C_BASE_TC0->TC_CCR = AT91C_TC_CLKEN | AT91C_TC_SWTRG;
    }
}

//-----------------------------------------------------------------------------
/// Callback invoked when data has been received on the USB.
//-----------------------------------------------------------------------------
static void UsbDataReceived0(unsigned int unused,
                             unsigned char status,
                             unsigned int received,
                             unsigned int remaining)
{
    // Check that data has been received successfully
    if (status == USBD_STATUS_SUCCESS) {

      #if defined(usb_CDCCDC)
        // Send data through USBSerial
        while (CDCDSerialDriver_Write(1, usbSerialBuffer0,
                                 received, 0, 0) != USBD_STATUS_SUCCESS);
      #endif

        // Send data through USART
        while (!USART_WriteBuffer(AT91C_BASE_US0, usbSerialBuffer0, received));
        AT91C_BASE_US0->US_IER = AT91C_US_TXBUFE;

        // Check if bytes have been discarded
        if ((received == DATABUFFERSIZE) && (remaining > 0)) {

            TRACE_WARNING(
                      "UsbDataReceived: %u bytes discarded\n\r",
                      remaining);
        }
    }
    else {

        TRACE_WARNING("UsbDataReceived: Transfer error\n\r");
    }
}

//-----------------------------------------------------------------------------
/// Handles interrupts coming from USART #0.
//-----------------------------------------------------------------------------
static void ISR_Usart0()
{
    unsigned int status = AT91C_BASE_US0->US_CSR;
    unsigned short serialState;

    // If USB device is not configured, do nothing
    if (USBD_GetState() != USBD_STATE_CONFIGURED) {

        AT91C_BASE_US0->US_IDR = 0xFFFFFFFF;
        return;
    }

    // Buffer has been read successfully
    if ((status & AT91C_US_ENDRX) != 0) {

        // Disable timer
        AT91C_BASE_TC0->TC_CCR = AT91C_TC_CLKDIS;

        // Send buffer through the USBSerial0
        while (CDCDSerialDriver_Write(0, usartBuffers[usartCurrentBuffer],
                                 DATABUFFERSIZE, 0, 0) != USBD_STATUS_SUCCESS);

        // Restart read on buffer
        USART_ReadBuffer(AT91C_BASE_US0,
                         usartBuffers[usartCurrentBuffer],
                         DATABUFFERSIZE);
        usartCurrentBuffer = 1 - usartCurrentBuffer;

        // Restart timer
        AT91C_BASE_TC0->TC_CCR = AT91C_TC_CLKEN | AT91C_TC_SWTRG;
    }

    // Buffer has been sent
    if ((status & AT91C_US_TXBUFE) != 0) {

        // Restart USB read
        CDCDSerialDriver_Read(0, usbSerialBuffer0,
                              DATABUFFERSIZE,
                              (TransferCallback) UsbDataReceived0,
                              0);
        AT91C_BASE_US0->US_IDR = AT91C_US_TXBUFE;
    }

    // Errors
    serialState = CDCDSerialDriver_GetSerialState(0);

    // Overrun
    if ((status & AT91C_US_OVER) != 0) {

        TRACE_WARNING("ISR_Usart0: Overrun\n\r");
        serialState |= CDCD_STATE_OVERRUN;
    }

    // Framing error
    if ((status & AT91C_US_FRAME) != 0) {

        TRACE_WARNING("ISR_Usart0: Framing error\n\r");
        serialState |= CDCD_STATE_FRAMING;
    }

    CDCDSerialDriver_SetSerialState(0, serialState);
}

//-----------------------------------------------------------------------------
/// Handles interrupts coming from the SSC. Sends remaining audio buffers
/// or stops the DAC transmission.
//-----------------------------------------------------------------------------
static void ISR_SSC(void)
{
    if ((BOARD_AT73C213_SSC->SSC_SR & AT91C_SSC_TXBUFE) != 0) {

        // End of transmission
        TRACE_WARNING("End ");
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
            usbAudBufferSizes[outBufferIndex] -= AUDD_NUMCHANNELS;
        }
        else if (numBuffersToSend < DAC_DELAY) {

            // Copy the last sample on all channels
            memcpy(&usbAudBuffers[outBufferIndex]
                                 [usbAudBufferSizes[outBufferIndex]],
                   &usbAudBuffers[outBufferIndex]
                                 [usbAudBufferSizes[outBufferIndex]
                                            - AUDD_BYTESPERSUBFRAME],
                   AUDD_BYTESPERSUBFRAME);
            usbAudBufferSizes[outBufferIndex] += AUDD_NUMCHANNELS;
        }

        // Load next buffer
        SSC_WriteBuffer(BOARD_AT73C213_SSC,
                        usbAudBuffers[outBufferIndex],
                        usbAudBufferSizes[outBufferIndex]);
        outBufferIndex = (outBufferIndex + 1) % BUFFER_NUMBER;
        numBuffersToSend--;
    }
    else {

        SSC_DisableInterrupts(BOARD_AT73C213_SSC,
                              AT91C_SSC_ENDTX);
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
    usbAudBufferSizes[inBufferIndex] = transferred / AUDD_BYTESPERSAMPLE;
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
    else if ((BOARD_AT73C213_SSC->SSC_PTSR & AT91C_PDC_TXTEN) == 0) {

        SSC_WriteBuffer(BOARD_AT73C213_SSC,
                        usbAudBuffers[outBufferIndex],
                        usbAudBufferSizes[outBufferIndex]);
        outBufferIndex = (outBufferIndex + 1) % BUFFER_NUMBER;
        numBuffersToSend--;
        SSC_WriteBuffer(BOARD_AT73C213_SSC,
                        usbAudBuffers[outBufferIndex],
                        usbAudBufferSizes[outBufferIndex]);
        outBufferIndex = (outBufferIndex + 1) % BUFFER_NUMBER;
        numBuffersToSend--;

        SSC_EnableInterrupts(BOARD_AT73C213_SSC,
                             AT91C_SSC_TXBUFE | AT91C_SSC_ENDTX);
    } 

    // Receive next packet
    AUDDSpeakerDriver_Read(usbAudBuffers[inBufferIndex],
                           AUDD_BYTESPERFRAME,
                           (TransferCallback) FrameReceived,
                           0); // No optional argument
}

//-----------------------------------------------------------------------------
//          Main
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/// Initializes drivers and start the USB composite device.
//-----------------------------------------------------------------------------
int main()
{
    unsigned char usbConnected = 0;

    TRACE_CONFIGURE(DBGU_STANDARD, 115200, BOARD_MCK);
    printf("-- USB CDCAUDIO Device Project %s --\n\r", SOFTPACK_VERSION);
    printf("-- %s\n\r", BOARD_NAME);
    printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);

#if defined(at91sam9260)
    // Switch to slow clock oscillator
    AT91C_BASE_PMC->PMC_MCKR = (AT91C_BASE_PMC->PMC_MCKR & ~AT91C_PMC_CSS)
                              | AT91C_PMC_CSS_SLOW_CLK;
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
    AT91C_BASE_PMC->PMC_PCKR[0] = AT91C_PMC_CSS_PLLA_CLK
                                | AT91C_PMC_PRES_CLK_8;
    AT91C_BASE_PMC->PMC_SCER = AT91C_PMC_PCK0;
    while ((AT91C_BASE_PMC->PMC_SR & AT91C_PMC_PCK0RDY) == 0);
#else
    NormalPowerMode();
#endif

    TRACE_CONFIGURE(DBGU_STANDARD, 115200, MCK);

    // If they are present, configure Vbus & Wake-up pins
    PIO_InitializeInterrupts(0);

    // If there is on board power, switch it off
  #ifdef PIN_USB_POWER_ENB
  { const Pin pinUsbPwr = PIN_USB_POWER_ENB;
    PIO_Configure(&pinUsbPwr, 1);
  }
  #endif

    // ----- CDC Function Initialize
    // Configure USART
    PIO_Configure(pinsUsart, PIO_LISTSIZE(pinsUsart));
    AT91C_BASE_PMC->PMC_PCER = 1 << AT91C_ID_US0;
    AT91C_BASE_US0->US_IDR = 0xFFFFFFFF;
    USART_Configure(AT91C_BASE_US0,
                    USART_MODE_ASYNCHRONOUS,
                    115200,
                    MCK);
    USART_SetTransmitterEnabled(AT91C_BASE_US0, 1);
    USART_SetReceiverEnabled(AT91C_BASE_US0, 1);
    IRQ_ConfigureIT(AT91C_ID_US0, 0, ISR_Usart0);
    IRQ_EnableIT(AT91C_ID_US0);

    // Configure timer 0
    AT91C_BASE_PMC->PMC_PCER = (1 << AT91C_ID_TC0);
    AT91C_BASE_TC0->TC_CCR = AT91C_TC_CLKDIS;
    AT91C_BASE_TC0->TC_IDR = 0xFFFFFFFF;
    AT91C_BASE_TC0->TC_CMR = AT91C_TC_CLKS_TIMER_DIV5_CLOCK
                             | AT91C_TC_CPCSTOP
                             | AT91C_TC_CPCDIS
                             | AT91C_TC_WAVESEL_UP_AUTO
                             | AT91C_TC_WAVE;
    AT91C_BASE_TC0->TC_RC = 0x00FF;
    AT91C_BASE_TC0->TC_IER = AT91C_TC_CPCS;
    IRQ_ConfigureIT(AT91C_ID_TC0, 0, ISR_Timer0);
    IRQ_EnableIT(AT91C_ID_TC0);

    // ----- AUDIO Function Initialize
    // Initialize all PIOs
    PIO_Configure(pinsAud, PIO_LISTSIZE(pinsAud));

    // Configure and enable the SPI (required for accessing the DAC)
    SPI_Configure(BOARD_AT73C213_SPI,
                  BOARD_AT73C213_SPI_ID,
                  SPI_CONFIGURATION);
    SPI_Enable(BOARD_AT73C213_SPI);

    // Initialize the audio DAC
    // Configure and enable the DAC
    AT73C213_Enable(AUDD_SAMPLERATE,
                    AUDD_BYTESPERSAMPLE,
                    AUDD_NUMCHANNELS,
                    1, // MCK = 384 * Fs
                    MCK);

    // Configure and enable the SSC interrupt
    IRQ_ConfigureIT(BOARD_AT73C213_SSC_ID,
                    SSC_PRIORITY,
                    ISR_SSC);
    IRQ_EnableIT(BOARD_AT73C213_SSC_ID);

    // USB COMPOSITE driver initialization
    COMPOSITEDDriver_Initialize();

    // connect if needed
    VBUS_CONFIGURE();

    // Driver loop
    while (1) {

        // Device is not configured
        if (USBD_GetState() < USBD_STATE_CONFIGURED) {
          
            if (usbConnected) {
                printf("-I- USB Disconnected\n\r");
                usbConnected = 0;
            }
        }
        else {
            
            if (usbConnected == 0) {
                
                usbConnected = 1;

                // Start receiving data on the USART
                usartCurrentBuffer = 0;
                USART_ReadBuffer(AT91C_BASE_US0, usartBuffers[0], DATABUFFERSIZE);
                USART_ReadBuffer(AT91C_BASE_US0, usartBuffers[1], DATABUFFERSIZE);
                AT91C_BASE_US0->US_IER = AT91C_US_ENDRX
                                         | AT91C_US_FRAME
                                         | AT91C_US_OVER;
                AT91C_BASE_TC0->TC_CCR = AT91C_TC_CLKEN | AT91C_TC_SWTRG;
    
                // Start receiving data on the USB
                CDCDSerialDriver_Read(0, usbSerialBuffer0,
                                      DATABUFFERSIZE,
                                      (TransferCallback) UsbDataReceived0,
                                      0);
            }

            // Start Read the incoming audio stream
            AUDDSpeakerDriver_Read(usbAudBuffers[inBufferIndex],
                                   AUDD_BYTESPERFRAME,
                                   (TransferCallback) FrameReceived,
                                   0); // No optional argument
        }
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
    }
}

