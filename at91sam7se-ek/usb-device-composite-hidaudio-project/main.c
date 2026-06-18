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
/// \dir "USB COMPOSITE HID+AUDIO project"
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
///    - "hid-keyboard"
///       - "USB HID Keyboard"
///     - "audio-speaker"
///       - "USB Audio Speaker Device"
/// - projects:
///    - "usb-device-hid-keyboard-project"
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
///      refer to "USB HID Keyboard Project" & "USB audio speaker".
///
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/// \unit
///
/// !Purpose
///
/// This file contains all the specific code for the
/// usb-device-composite-hidaudio-project
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
#include <pmc/pmc.h>
#include <pit/pit.h>
#include <spi/spi.h>
#include <ssc/ssc.h>
#include <components/dac-at73c213/at73c213.h>

#include <usb/common/core/USBConfigurationDescriptor.h>
#include <usb/common/hid/HIDKeypad.h>

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

/// Number of keys used in the example.
#define NUM_KEYS                    4

/// Number of non-modifiers keys.
#define NUM_NORMAL_KEYS             3

/// Number of modifier keys.
#define NUM_MODIFIER_KEYS           (NUM_KEYS - NUM_NORMAL_KEYS)

/// Num lock LED index.
#define LED_NUMLOCK                 USBD_LEDOTHER

/// Delay for pushbutton debouncing (ms)
#define DEBOUNCE_TIME      10

/// PIT period value (useconds)
#define PIT_PERIOD        1000

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

//- HID
/// List of pinsPushButtons to configure for the applicatino.
static Pin pinsPushButtons[] = {PINS_PUSHBUTTONS};

/// Array of key codes produced by each button.
static unsigned char keyCodes[NUM_KEYS] = {
    HIDKeypad_A,
    HIDKeypad_NUMLOCK,
    HIDKeypad_9,
    HIDKeypad_RIGHTSHIFT
};

/// Current status (pressed or not) for each key.
static unsigned char keyStatus[NUM_KEYS];

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
/// Invoked when the status of the keyboard LEDs changes. Turns the num. lock
/// LED on or off.
/// \param numLockStatus Indicates the current status of the num. lock key.
/// \param capsLockStatus Indicates the current status of the caps lock key.
/// \param scrollLockStatus Indicates the current status of the scroll lock key
//-----------------------------------------------------------------------------
void HIDDKeyboardCallbacks_LedsChanged(
    unsigned char numLockStatus,
    unsigned char capsLockStatus,
    unsigned char scrollLockStatus)
{
    // Num. lock
    if (numLockStatus) {

        LED_Set(LED_NUMLOCK);
    }
    else {

        LED_Clear(LED_NUMLOCK);
    }
}


//-----------------------------------------------------------------------------
//         Internal functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/// Monitor keyboard buttons & Update key status in HID driver
//-----------------------------------------------------------------------------
static void HIDDKeyboardProcessKeys(void)
{
    unsigned int i;
    unsigned char pressedKeys[NUM_KEYS];
    unsigned char pressedKeysSize = 0;
    unsigned char releasedKeys[NUM_KEYS];
    unsigned char releasedKeysSize = 0;
    
    // Monitor buttons
    for (i=0; i < PIO_LISTSIZE(pinsPushButtons); i++) {
    
        // Check if button state has changed
        unsigned char isButtonPressed = PIO_Get(&(pinsPushButtons[i]));
        if (isButtonPressed != keyStatus[i]) {
    
            // Update button state
            if (!isButtonPressed) {
    
                // Key has been pressed
                TRACE_INFO("-I- Key %u has been pressed\n\r", i);
                keyStatus[i] = 0;
                pressedKeys[pressedKeysSize] = keyCodes[i];
                pressedKeysSize++;
                HIDDKeyboardDriver_RemoteWakeUp();
            }
            else {
    
                // Key has been released
                TRACE_INFO("-I- Key %u has been released\n\r", i);
                keyStatus[i] = 1;
                releasedKeys[releasedKeysSize] = keyCodes[i];
                releasedKeysSize++;
            }
        }
    }
    
    // Update key status in the HID driver if necessary
    if ((pressedKeysSize != 0) || (releasedKeysSize != 0)) {
    
        unsigned char status;

        do {
        
            status = HIDDKeyboardDriver_ChangeKeys(pressedKeys,
                                                   pressedKeysSize,
                                                   releasedKeys,
                                                   releasedKeysSize);
        }
        while (status != USBD_STATUS_SUCCESS);
    }
}

//-----------------------------------------------------------------------------
//         Exported function
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//          Main
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/// Initializes drivers and start the USB composite device.
//-----------------------------------------------------------------------------
int main()
{
    TRACE_CONFIGURE(DBGU_STANDARD, 115200, BOARD_MCK);
    printf("-- USB HID+AUDIO Device Project %s --\n\r", SOFTPACK_VERSION);
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

    // ----- HID Function Initialize
    // Initialize key statuses and configure push buttons
    PIO_Configure(pinsPushButtons, PIO_LISTSIZE(pinsPushButtons));
    memset(keyStatus, 1, NUM_KEYS);

    // Configure LEDs
    LED_Configure(LED_NUMLOCK);

    // ----- AUDIO Function Initialize
    // Initialize all PIOs
    PIO_Configure(pinsAud, PIO_LISTSIZE(pinsAud));

    // Configure and enable the SPI (required for accessing the DAC)
    SPI_Configure(BOARD_AT73C213_SPI,
                  BOARD_AT73C213_SPI_ID,
                  SPI_CONFIGURATION);
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

        }
        else {

            // Read the incoming audio stream
            AUDDSpeakerDriver_Read(usbAudBuffers[inBufferIndex],
                                   AUDD_BYTESPERFRAME,
                                   (TransferCallback) FrameReceived,
                                   0); // No optional argument

            HIDDKeyboardProcessKeys();
        }
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
    }
}

