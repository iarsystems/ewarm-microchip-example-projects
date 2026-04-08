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
/// \dir "USB CDCMSD CDC+MSD project"
///
/// !!!Purpose
///
/// The USB CDCMSD Project will help you to get familiar with the
/// USB Device Port(UDP)interface and also some of the other interfaces in
/// AT91SAM microcontrollers. Also it can help you to be familiar with the USB
/// Framework that is used for rapid development of USB-compliant class
/// drivers such as USB Communication Device class (CDC), and how to combine
/// two USB functions to a single CDCMSD device (such as CDC + MSD).
///
/// You can find following information depends on your needs:
/// - Sample usage of USB Device Framework.
/// - USB CDCMSD device and functions driver development based on the AT91
///   USB Device Framework and other re-usable class driver code.
/// - USB enumerate sequence, the standard and class-specific descriptors and
///   requests handling.
/// - The initialize sequence and usage of UDP interface.
///
/// !!!See
///
/// - pio: Pin configurations and peripheral configure.
/// - memories: Storage Media interface for MSD
/// - usb: USB Device Framework, USB CDC driver and UDP interface driver
///    - "AT91 USB device framework"
///        - "USBD API"
///    - "CDCMSD"
///       - "USB CDCMSD Device"
///    - "massstorage"
///       - "USB MSD Driver"
///    - "cdc-serial"
///       - "USB CDC Serial Device"
/// - projects:
///    - "usb-device-massstorage-project"
///    - "usb-device-cdc-serial-project"
///
/// !!!Requirements
///
/// This package can be used with some of Atmel evaluation kits that have UDP
/// interface, depending on the functions included.
///
/// The current supported board list:
///    - at91sam7s-ek (exclude at91sam7s32)
///    - at91sam7x-ek
///    - at91sam7xc-ek
///    - at91sam7a3-ek
///    - at91sam7se-ek
///    - at91sam9260-ek
///    - at91sam9263-ek
///
///  !!!Windows Driver Update
///
/// The CDCMSD device is generally supported by Microsoft windows, but some
/// patches are needed for muti-interface functions such as CDC & Audio. The
/// example CDCMSD devices are tested under windows XP (SP3). For CDC
/// serial port, additional windows driver file (CDCMSDCDCSerial.inf) can
/// be found at at91lib\usb\device\CDCMSD\drv.
///
/// The following is alternate update to fix the CDCMSD device support
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
/// Two hot fixes are necessary for window to recognize the CDCMSD device
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
///     -- USB CDCMSD Device Project xxx --
///     -- AT91xxxxxx-xx
///     -- Compiled: xxx xx xxxx xx:xx:xx --
///     \endcode
/// -# When connecting USB cable to windows, the LED blinks, and the host
///    reports a new USB %device attachment.
/// -# For the windows driver installation and the test functions, please
///      refer to "USB CDC serial converter" &
///      "USB Device Mass Storage Project".
/// -# You can use the inf file
///    at91lib\\usb\\device\\CDCMSD\\drv\\CDCMSDCDCSerial.inf
///    to install the CDC serial  port.
///
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/// \unit
///
/// !Purpose
///
/// This file contains all the specific code for the
/// usb-device-CDCMSD-cdcmsd-project
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
#include <tc/tc.h>
#include <usart/usart.h>
#include <utility/assert.h>
#include <utility/trace.h>
#include <utility/led.h>

#include <board_memories.h>
#include <usb/device/massstorage/MSDDriver.h>
#include <usb/device/massstorage/MSDLun.h>
#include <memories/MEDSdram.h>
#include <memories/MEDRamDisk.h>
#include <memories/MEDDdram.h>
#include <pmc/pmc.h>

#include <usb/device/composite/CDCMSDDDriver.h>

#include <string.h>

#if defined(CHIP_FLASH_EFC) || defined(CHIP_FLASH_EEFC)
#include <memories/MEDFlash.h>
#endif

//-----------------------------------------------------------------------------
//      Definitions
//-----------------------------------------------------------------------------

/// Master clock frequency in Hz
#define MCK                         BOARD_MCK

/// Size in bytes of the buffer used for reading data from the USB & USART
#define DATABUFFERSIZE \
    BOARD_USB_ENDPOINTS_MAXPACKETSIZE(2)

/// Maximum number of LUNs which can be defined.
#define MAX_LUNS            1
#define DRV_DISK            0

/// Delay for pushbutton debouncing (ms)
#define DEBOUNCE_TIME       10

/// PIT period value (useconds)
#define PIT_PERIOD          1000

/// Maximum code size reserved for running in EBI RAM
#define CODE_SIZE           0x10000

/// Size of one block in bytes.
#define BLOCK_SIZE          512

#define RAMDISK_SIZE        (10*1024*1024)

/// Size of the MSD IO buffer in bytes.
#define MSD_BUFFER_SIZE     (12*BLOCK_SIZE)

/// Use for power management
#define STATE_IDLE    0
/// The USB device is in suspend state
#define STATE_SUSPEND 4
/// The USB device is in resume state
#define STATE_RESUME  5

#if defined(at91sam9m10ek) || defined(at91sam3uek)
#define PINS_USART      PIN_USART1_TXD, PIN_USART1_RXD
#define BASE_USART      AT91C_BASE_US1
#define ID_USART        AT91C_ID_US1
#define USART_Handler   USART1_IrqHandler
#else
#define PINS_USART      PIN_USART0_TXD, PIN_USART0_RXD
#define BASE_USART      AT91C_BASE_US0
#define ID_USART        AT91C_ID_US0
#define USART_Handler   USART0_IrqHandler
#endif

//-----------------------------------------------------------------------------
//      Internal variables
//-----------------------------------------------------------------------------
/// State of USB, for suspend and resume
unsigned char USBState = STATE_IDLE;

//- CDC
/// List of pins that must be configured for use by the application.
static const Pin pinsUsart[] = {PINS_USART};

/// Double-buffer for storing incoming USART data.
static unsigned char usartBuffers[2][DATABUFFERSIZE];

/// Current USART buffer index.
static unsigned char usartCurrentBuffer = 0;

/// Buffer for storing incoming USB data.
static unsigned char usbSerialBuffer0[DATABUFFERSIZE];

//- MSD
/// Available medias.
Media medias[MAX_LUNS];

/// Number of medias which are effectively used (Defined in Media.c).
//unsigned int numMedias = 0;

/// Device LUNs.
MSDLun luns[MAX_LUNS];

/// LUN read/write buffer.
unsigned char msdBuffer[MSD_BUFFER_SIZE];

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
    TRACE_INFO("VBUS ");

    // Check current level on VBus
    if (PIO_Get(&pinVbus)) {

        TRACE_INFO("conn\n\r");
        USBD_Connect();
    }
    else {

        TRACE_INFO("discon\n\r");
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
#else
//------------------------------------------------------------------------------
/// Put the CPU in 32kHz, disable PLL, main oscillator
/// Put voltage regulator in standby mode
//------------------------------------------------------------------------------
void LowPowerMode(void){}
//------------------------------------------------------------------------------
/// Put voltage regulator in normal mode
/// Return the CPU to normal speed 48MHz, enable PLL, main oscillator
//------------------------------------------------------------------------------
void NormalPowerMode(void){}

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
//         Internal functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/// Handles interrupts coming from Timer #0.
//-----------------------------------------------------------------------------
void TC0_IrqHandler()
{
    unsigned char size;
    unsigned int status = AT91C_BASE_TC0->TC_SR;

    if ((status & AT91C_TC_CPCS) != 0) {
    
        // Flush PDC buffer
        size = DATABUFFERSIZE - BASE_USART->US_RCR;
        if (size == 0) {

            AT91C_BASE_TC0->TC_CCR = AT91C_TC_CLKEN | AT91C_TC_SWTRG;
            return;
        }
        BASE_USART->US_RCR = 0;
    
        // Send current buffer through the USB
        while (CDCDSerialDriver_Write(0, usartBuffers[usartCurrentBuffer],
                                      size, 0, 0) != USBD_STATUS_SUCCESS);
    
        // Restart read on buffer
        USART_ReadBuffer(BASE_USART,
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

        // Send data through USART
        while (!USART_WriteBuffer(BASE_USART, usbSerialBuffer0, received));
        BASE_USART->US_IER = AT91C_US_TXBUFE;

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
/// Handles interrupts coming from USART #.
//-----------------------------------------------------------------------------
void USART_Handler()
{
    unsigned int status = BASE_USART->US_CSR;
    unsigned short serialState;

    // If USB device is not configured, do nothing
    if (USBD_GetState() != USBD_STATE_CONFIGURED) {

        BASE_USART->US_IDR = 0xFFFFFFFF;
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
        USART_ReadBuffer(BASE_USART,
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
        BASE_USART->US_IDR = AT91C_US_TXBUFE;
    }

    // Errors
    serialState = CDCDSerialDriver_GetSerialState(0);

    // Overrun
    if ((status & AT91C_US_OVER) != 0) {

        TRACE_WARNING("USART0_IrqHandler: Overrun\n\r");
        serialState |= CDCD_STATE_OVERRUN;
    }

    // Framing error
    if ((status & AT91C_US_FRAME) != 0) {

        TRACE_WARNING("USART0_IrqHandler: Framing error\n\r");
        serialState |= CDCD_STATE_FRAMING;
    }

    CDCDSerialDriver_SetSerialState(0, serialState);
}

//-----------------------------------------------------------------------------
//         Internal functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/// Interrupt handler for all media types.
//-----------------------------------------------------------------------------
void ISR_Media()
{
    MED_HandleAll(medias, numMedias);
}

//-----------------------------------------------------------------------------
/// Initialize MSD Media & LUNs
//-----------------------------------------------------------------------------
void MemoriesInitialize()
{
    // Reset all LUNs
    unsigned int i;
    for (i = 0; i < MAX_LUNS; i ++) 
        LUN_Init(&luns[i], 0, 0, 0, 0, 0, 0, 0, 0);

#if defined(AT91C_BASE_DDR2C)
    printf("DDR2 Disk init\n\r");
  #if !defined(ddram)
    BOARD_ConfigureDdram();
  #endif
    MEDDdram_Initialize(&medias[DRV_DISK],
                        BLOCK_SIZE,
                        (AT91C_DDR2 + CODE_SIZE) / BLOCK_SIZE,
                        RAMDISK_SIZE / BLOCK_SIZE);
    LUN_Init(&lun[DRV_DISK], &medias[DRV_DISK],
             msdBuffer, MSD_BUFFER_SIZE,
             0, 0, 0, 0, 0);
    numMedias ++;

#elif defined(BOARD_EBI_PSRAM)
    printf("PSRAM Disk init\n\r");
    BOARD_ConfigurePsram();
    // Initialize 512K for ram disk & code.
    if (1 != MEDRamDisk_Initialize(&(medias[DRV_DISK]),
                                   BLOCK_SIZE,
                                   (BOARD_EBI_PSRAM + CODE_SIZE) / BLOCK_SIZE,
                                   512*1024 / BLOCK_SIZE)) {
        printf("-E- FAIL!\n\r");
        return;
    }
    // 512k Disk
    LUN_Init(&(luns[DRV_DISK]), &(medias[DRV_DISK]),
             msdBuffer, MSD_BUFFER_SIZE,
             0, 0, 0, 0, 0);
    numMedias ++;

#elif defined(AT91C_EBI_SDRAM)
    printf("SDRAM Disk init\n\r");
  #if !defined(sdram)
    BOARD_ConfigureSdram(BOARD_SDRAM_BUSWIDTH);
  #endif
    MEDSdram_Initialize(&(medias[DRV_DISK]),
                        BLOCK_SIZE,
                        (unsigned int)(AT91C_EBI_SDRAM + CODE_SIZE)/BLOCK_SIZE,
                        RAMDISK_SIZE / BLOCK_SIZE);
    LUN_Init(&(luns[DRV_DISK]),
             &(medias[DRV_DISK]),
             msdBuffer, MSD_BUFFER_SIZE,
             0, 0, 0, 0,
             0);
    numMedias = 1;

#elif defined(AT91C_IFLASH)
    printf("IFLASH Disk init\n\r");
    FLA_Initialize(&(medias[DRV_DISK]), AT91C_BASE_EFC);
    LUN_Init(&(luns[DRV_DISK]),
             &(medias[DRV_DISK]),
             msdBuffer, MSD_BUFFER_SIZE,
             CODE_SIZE,
             AT91C_IFLASH_SIZE - CODE_SIZE,
             BLOCK_SIZE,
             0,
             0);
    numMedias ++;
#else
#endif

    ASSERT(numMedias > 0, "Error: No media defined.\n\r");
    TRACE_DEBUG("%u medias defined\n\r", numMedias);
}

//-----------------------------------------------------------------------------
//         Exported function
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//          Main
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/// Initializes drivers and start the USB CDCMSD device.
//-----------------------------------------------------------------------------
int main()
{
    unsigned char usbConnected = 0;

    TRACE_CONFIGURE(DBGU_STANDARD, 115200, BOARD_MCK);
    printf("-- USB CDCMSD Device Project %s --\n\r", SOFTPACK_VERSION);
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

    // ----- CDC Function Initialize
    // Configure USART
    PIO_Configure(pinsUsart, PIO_LISTSIZE(pinsUsart));
    AT91C_BASE_PMC->PMC_PCER = 1 << ID_USART;
    BASE_USART->US_IDR = 0xFFFFFFFF;
    USART_Configure(BASE_USART,
                    USART_MODE_ASYNCHRONOUS,
                    115200,
                    MCK);
    USART_SetTransmitterEnabled(BASE_USART, 1);
    USART_SetReceiverEnabled(BASE_USART, 1);
    IRQ_ConfigureIT(ID_USART, 0, USART_Handler);
    IRQ_EnableIT(ID_USART);

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
    IRQ_ConfigureIT(AT91C_ID_TC0, 0, TC0_IrqHandler);
    IRQ_EnableIT(AT91C_ID_TC0);

    MemoriesInitialize();

    // USB CDCMSD driver initialization
    CDCMSDDDriver_Initialize(luns, numMedias);

    // connect if needed
    VBUS_CONFIGURE();

    // Driver loop
    while (1) {

        // Device is not configured
        if (USBD_GetState() < USBD_STATE_CONFIGURED) {

            if (usbConnected) {
                printf("-I- USB Disconnect/Suspend\n\r");
                usbConnected = 0;
                AT91C_BASE_TC0->TC_CCR = AT91C_TC_CLKDIS;
            }
        }
        else {

            if (usbConnected == 0) {
                printf("-I- USB Connect\n\r");
                usbConnected = 1;

                // Start receiving data on the USART
                usartCurrentBuffer = 0;
                USART_ReadBuffer(BASE_USART,
                                 usartBuffers[0],
                                 DATABUFFERSIZE);
                USART_ReadBuffer(BASE_USART,
                                 usartBuffers[1],
                                 DATABUFFERSIZE);
                BASE_USART->US_IER = AT91C_US_ENDRX
                                         | AT91C_US_FRAME
                                         | AT91C_US_OVER;
                AT91C_BASE_TC0->TC_CCR = AT91C_TC_CLKEN | AT91C_TC_SWTRG;

                // Start receiving data on the USB
                CDCDSerialDriver_Read(0, usbSerialBuffer0,
                                      DATABUFFERSIZE,
                                      (TransferCallback) UsbDataReceived0,
                                      0);
            }

            MSDDriver_StateMachine();
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

