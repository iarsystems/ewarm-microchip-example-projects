/*----------------------------------------------------------------------------
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
/// \dir "USB Device Mass Storage Project"
///
/// !!!Purpose
///
/// The USB Massstorage Project will help you to get familiar with the
/// USB Device Port(UDP) on AT91SAM microcontrollers. Also
/// it can help you to be familiar with the USB Framework that is used for
/// rapid development of USB-compliant class drivers such as USB Mass
/// Storage class (MSD).
///
/// You can find following information depends on your needs:
/// - Sample usage of USB MSD driver.
/// - USB MSD driver development based on the AT91 USB Framework.
/// - USB enumerate sequence, the standard and class-specific descriptors and
///   requests handling.
/// - The initialize sequence and usage of UDP interface.
///
/// !See
/// - memories: Storage Media interface for MSD
/// - usb: USB Framework, USB MSD driver and UDP interface driver
///    - "AT91 USB device framework"
///       - "USBD API"
///    - "massstorage"
///       - "USB MSD Driver"
///
/// !!!Requirements
///
/// This package can be used with all Atmel evaluation kits that have USB interface
///
/// !!!Description
///
/// When an EK running this program connected to a host (PC for example), with
/// USB cable, the EK appears as a USB Disk for the host. Then the host can 
/// format/read/write on the disk.
///
/// If there is SDRAM on the EK, the disk can be up to 10M so that read/write
/// speed can be tested.
///
/// If there is no SDRAM but only internal flash, the disk is about 30K and
/// only small file can be tested.
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
///     -- USB Device Mass Storage Project xxx --
///     -- AT91xxxxxx-xx
///     -- Compiled: xxx xx xxxx xx:xx:xx --
///     \endcode
/// -# When connecting USB cable to windows, the LED blinks, and the host
///    reports a new USB %device attachment and Disk installation.
///  . Then new "USB Mass Storage Device" and
///    "ATMEL Mass Storage MSD USB Device" and "Generic volume" appear in
///    hardware %device list.
/// -# You can find the new disk on host, and to create/write file to it.
///
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/// \unit
///
/// !Purpose
///
/// This file contains all the specific code for the
/// usb-device-massstorage-project
///
/// !Contents
///
/// The code can be roughly broken down as follows:
///    - Configuration functions
///       - VBus_Configure
///       - ConfigurePit
///       - ConfigureWakeUp
///       - PIO & Timer configurations in start of main
///    - Interrupt handlers
///       - ISR_Vbus
///       - ISR_Pit
///       - WakeUpHandler
///       - ISR_Media
///    - The main function, which implements the program behavior
///
/// Please refer to the list of functions in the #Overview# tab of this unit
/// for more detailed information.
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//         Headers
//------------------------------------------------------------------------------

#include <board.h>
#include <board_lowlevel.h>
#include <board_memories.h>
#include <pio/pio.h>
#include <pio/pio_it.h>
#ifdef AT91C_BASE_PITC
#include <pit/pit.h>
#endif
#include <irq/irq.h>
#include <tc/tc.h>
#include <dbgu/dbgu.h>
#include <utility/trace.h>
#include <utility/assert.h>
#include <utility/led.h>
#include <usb/common/core/USBConfigurationDescriptor.h>
#include <usb/device/core/USBD.h>
#include <usb/device/massstorage/MSDDriver.h>
#include <usb/device/massstorage/MSDLun.h>
#include <usb/device/core/USBDCallbacks.h>
#include <memories/Media.h>
#if defined(AT91C_EBI_SDRAM)
#include <memories/MEDSdram.h>
#endif
#if defined(AT91C_BASE_DDR2C)
#include <memories/MEDDdram.h>
#endif
#if defined(BOARD_SD_MCI_BASE)
#include <memories/MEDSdcard.h>
#endif
#if defined(CHIP_FLASH_EFC) || defined(CHIP_FLASH_EEFC) && !defined(cortexm3)
#include <memories/MEDFlash.h>
#endif
#if defined(at91sam3uek)
#include <memories/MEDRamDisk.h>
#endif
#if 0
#include <memories/MEDNandFlash.h>
#include <memories/nandflash/RawNandFlash.h>
#include <memories/nandflash/TranslatedNandFlash.h>
#endif

#include <pmc/pmc.h>

#include <string.h>

//------------------------------------------------------------------------------
//         Internal definitions
//------------------------------------------------------------------------------

/// Maximum number of LUNs which can be defined.
#define MAX_LUNS            3

/// Media index for different disks
#define DRV_RAMDISK         0   /// RAM disk
#define DRV_IFLASH          0   /// Internal flash, if no RAM disk
#define DRV_SDMMC           1   /// SD card, if there is
#define DRV_NAND            2   /// Nand flash, if there is

/// Delay for pushbutton debouncing (ms)
#define DEBOUNCE_TIME       10

/// PIT period value (seconds)
#define PIT_PERIOD          1000

/// Delay for display view update (*250ms)
#define UPDATE_DELAY        4

/// Delay for waiting DBGU input (*250ms)
#define INPUT_DELAY         20

#if defined(at91sam3uek)
/// No reserved space for code in PSRAM
#define CODE_SIZE           (0)
/// Size of the RAM disk in bytes (512K).
#define RAMDISK_SIZE        (512*1024)
#else
/// Maximum code size 100K reserved for running in SDRAM and FLASH
#define CODE_SIZE           (100*1024)
/// Size of the RAM disk in bytes (10M).
#define RAMDISK_SIZE        (10*1024*1024)
#endif

/// Size of the reserved Nand Flash (4M)
#define NF_RESERVE_SIZE     (4*1024*1024)

/// Size of the managed Nand Flash (128M)
#define NF_MANAGED_SIZE     (128*1024*1024)

/// Size of one block in bytes.
#define BLOCK_SIZE          512

/// Size of the MSD IO buffer in bytes (2K, more the better).
#define MSD_BUFFER_SIZE     (12*BLOCK_SIZE)

/// Use for power management
#define STATE_IDLE    0
/// The USB device is in suspend state
#define STATE_SUSPEND 4
/// The USB device is in resume state
#define STATE_RESUME  5

/// Retry 3 times when SD is inserted
#define SD_INIT_RETRY       3

//------------------------------------------------------------------------------
//         Global variables
//------------------------------------------------------------------------------

/// Available medias.
Media medias[MAX_LUNS];

//------------------------------------------------------------------------------
//         Internal variables
//------------------------------------------------------------------------------

/// Device LUNs.
MSDLun luns[MAX_LUNS];

/// LUN read/write buffer.
unsigned char msdBuffer[MSD_BUFFER_SIZE];

/// Total data read/write by MSD
unsigned int msdReadTotal = 0;
unsigned int msdWriteTotal = 0;
unsigned short msdFullCnt = 0;
unsigned short msdNullCnt = 0;

/// Update delay counter, tick is 250ms
unsigned int updateDelay = UPDATE_DELAY;

/// Flag to update Display View
unsigned char updateView = 0;

/// State of USB, for suspend and resume
unsigned char USBState = STATE_IDLE;

#if 0//#ifdef PINS_NANDFLASH
/// Pins used to access to nandflash.
static const Pin pPinsNf[] = {PINS_NANDFLASH};
/// Nandflash device structure.
static struct TranslatedNandFlash translatedNf;
/// Address for transferring command bytes to the nandflash.
static unsigned int cmdBytesAddr = BOARD_NF_COMMAND_ADDR;
/// Address for transferring address bytes to the nandflash.
static unsigned int addrBytesAddr = BOARD_NF_ADDRESS_ADDR;
/// Address for transferring data bytes to the nandflash.
static unsigned int dataBytesAddr = BOARD_NF_DATA_ADDR;
/// Nandflash chip enable pin.
static const Pin nfCePin = BOARD_NF_CE_PIN;
/// Nandflash ready/busy pin.
static const Pin nfRbPin = BOARD_NF_RB_PIN;
#endif

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
#if defined (AT91C_BASE_PITC)
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
        MSDDriver_RemoteWakeUp();
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
#endif // AT91C_BASE_PITC

//------------------------------------------------------------------------------
/// Interrupt service routine for the remote wake-up pin. Starts the debouncing
/// sequence.
//------------------------------------------------------------------------------
static void WakeUpHandler(const Pin *pin)
{
    TRACE_DEBUG("Wake-up handler\n\r");

    // Check current level on the remote wake-up pin
    if (!PIO_Get(&pinWakeUp)) {
#ifdef AT91C_BASE_PITC
        ConfigurePit();
#endif // AT91C_BASE_PITC
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
void ISR_Vbus(const Pin *pPin)
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


//------------------------------------------------------------------------------
//         Callbacks (re)-implementation
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

//------------------------------------------------------------------------------
/// Invoked when the MSD finish a READ/WRITE.
/// \param flowDirection 1 - device to host (READ10)
///                      0 - host to device (WRITE10)
/// \param dataLength Length of data transferred in bytes.
/// \param fifoNullCount Times that FIFO is NULL to wait
/// \param fifoFullCount Times that FIFO is filled to wait
//------------------------------------------------------------------------------
void MSDCallbacks_Data(unsigned char flowDirection,
                       unsigned int  dataLength,
                       unsigned int  fifoNullCount,
                       unsigned int  fifoFullCount)
{
    if (flowDirection) {

        msdReadTotal += dataLength;
    }
    else {

        msdWriteTotal += dataLength;
    }

    msdFullCnt += fifoFullCount;
    msdNullCnt += fifoNullCount;
}

//------------------------------------------------------------------------------
//         Internal functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Interrupt handler for timer.
//------------------------------------------------------------------------------
void TC0_IrqHandler(void)
{
    volatile unsigned int dummy;
    // Clear status bit to acknowledge interrupt
    dummy = AT91C_BASE_TC0->TC_SR;

    if (-- updateDelay == 0) {

        updateDelay = UPDATE_DELAY;
        updateView = 1;
    }
}

//------------------------------------------------------------------------------
/// Configure Timer Counter 0 to generate an interrupt every 250ms.
//------------------------------------------------------------------------------
void ConfigureTc0(void)
{
    unsigned int div;
    unsigned int tcclks;

    // Enable peripheral clock
    AT91C_BASE_PMC->PMC_PCER = 1 << AT91C_ID_TC0;

    // Configure TC for a 4Hz frequency and trigger on RC compare
    TC_FindMckDivisor(4, BOARD_MCK, &div, &tcclks);
    TC_Configure(AT91C_BASE_TC0, tcclks | AT91C_TC_CPCTRG);
    AT91C_BASE_TC0->TC_RC = (BOARD_MCK / div) / 4; // timerFreq / desiredFreq

    // Configure and enable interrupt on RC compare
    IRQ_ConfigureIT(AT91C_ID_TC0, 3, TC0_IrqHandler);
    AT91C_BASE_TC0->TC_IER = AT91C_TC_CPCS;
    IRQ_EnableIT(AT91C_ID_TC0);

    TC_Start(AT91C_BASE_TC0);
}

//------------------------------------------------------------------------------
/// Interrupt handler for all media types.
//------------------------------------------------------------------------------
void ISR_Media(void)
{
    MED_HandleAll(medias, numMedias);
}

#if defined(BOARD_SD_MCI_BASE)
//------------------------------------------------------------------------------
/// SD card connection/disconnection handler, to initialize and link SD Media
/// to corresponding LUN or unlink it.
/// \param inserted SD card is inserted/ejected.
//------------------------------------------------------------------------------
static unsigned char SDConnectionUpdate(unsigned char inserted)
{
    unsigned char rc;
    if (inserted) {
      #if 1
        // Faster, non-blocked SD access function
        rc = MEDSdusb_Initialize(&(medias[DRV_SDMMC]), 0);
      #else
        // Blocked SD access function
        rc = MEDSdcard_Initialize(&(medias[DRV_SDMMC]), 0);
      #endif
        if(rc) {
            LUN_Init(&(luns[DRV_SDMMC]), &(medias[DRV_SDMMC]),
                     msdBuffer, MSD_BUFFER_SIZE,
                     0, 0, 0, 0,
                     MSDCallbacks_Data);
            return 1;
        }
    }
    else {
        LUN_Eject(&luns[DRV_SDMMC]);
    }
    return 0;
}
#endif

#if 0//defined(PINS_NANDFLASH)
#define NandFlash_Configure(nfBusWidth)    BOARD_ConfigureNandFlash(nfBusWidth)

//------------------------------------------------------------------------------
/// Initialize Nand Flash for LUN
//------------------------------------------------------------------------------
static void NandFlashInitialize(void)
{
    unsigned char nfBusWidth = 16, nfRc;
    unsigned short nfBaseBlock = 0;
    struct RawNandFlash *pRaw = (struct RawNandFlash*)&translatedNf;
    struct NandFlashModel *pModel = (struct NandFlashModel*)&translatedNf;
    unsigned int nfMamagedSize;

    // Configure SMC for NandFlash
    NandFlash_Configure(nfBusWidth);
    // Configure PIO for Nand Flash
    PIO_Configure(pPinsNf, PIO_LISTSIZE(pPinsNf));

    // Nand Flash Initialize (ALL flash mapped)
    nfRc = RawNandFlash_Initialize(pRaw,
                                   0,
                                   cmdBytesAddr,
                                   addrBytesAddr,
                                   dataBytesAddr,
                                   nfCePin,
                                   nfRbPin);
    if (nfRc) {
        printf("Nand not found\n\r");
        return;
    }
    else {
        printf("NF\tNb Blocks %d\n\r",
               NandFlashModel_GetDeviceSizeInBlocks(pModel));
        printf("\tBlock Size %dK\n\r",
               NandFlashModel_GetBlockSizeInBytes(pModel)/1024);
        printf("\tPage Size %d\n\r",
               NandFlashModel_GetPageDataSize(pModel));
        nfBaseBlock =
            NF_RESERVE_SIZE / NandFlashModel_GetBlockSizeInBytes(pModel);
    }
    printf("NF disk will use area from %dM(B%d)\n\r",
           NF_RESERVE_SIZE/1024/1024, nfBaseBlock);
    printf("!! Erase the NF Disk? (y/n):");
    updateDelay = INPUT_DELAY;
    updateView = 0;
    while(1) {
        if(DBGU_IsRxReady()) {
            char key = DBGU_GetChar();
            DBGU_PutChar(key);
            if (key == 'y') {
                if (nfRc == 0) {
                    unsigned int block;
                    printf(" Erase from %d ... ", nfBaseBlock);
                    for (block = nfBaseBlock;
                     block < NandFlashModel_GetDeviceSizeInBlocks(pModel);
                     block ++) {
                        RawNandFlash_EraseBlock(pRaw, block);
                    }
                    printf("OK");
                }
            }
            printf("\n\r");
            break;
        }
        if (updateView) {
            printf("No\n\r");
            break;
        }
    }
    nfMamagedSize = ((NandFlashModel_GetDeviceSizeInMBytes(pModel) - NF_RESERVE_SIZE/1024/1024) > NF_MANAGED_SIZE/1024/1024) ? \
                        NF_MANAGED_SIZE/1024/1024 : (NandFlashModel_GetDeviceSizeInMBytes(pModel) - NF_RESERVE_SIZE/1024/1024);
    if (TranslatedNandFlash_Initialize(&translatedNf,
                                       0,
                                       cmdBytesAddr,
                                       addrBytesAddr,
                                       dataBytesAddr,
                                       nfCePin,
                                       nfRbPin,
                                       nfBaseBlock, nfMamagedSize * 1024 * 1024/NandFlashModel_GetBlockSizeInBytes(pModel))) {
        printf("Nand init error\n\r");
        return;
    }
    // Check the data bus width of the NandFlash
    nfBusWidth =
        NandFlashModel_GetDataBusWidth(pModel);
    NandFlash_Configure(nfBusWidth);
    
    // Media initialize
    MEDNandFlash_Initialize(&medias[DRV_NAND], &translatedNf);

    // Initialize LUN
    LUN_Init(&(luns[DRV_NAND]), &(medias[DRV_NAND]),
             msdBuffer, MSD_BUFFER_SIZE,
             0, 0, 0, 0,
             MSDCallbacks_Data);
    
    numMedias ++;
}
#endif

#if !defined(cortexm3)

//------------------------------------------------------------------------------
/// Initialize memory for LUN
//------------------------------------------------------------------------------
static void MemoryInitialization(void)
{
    unsigned int i;
    for (i = 0; i < MAX_LUNS; i ++) 
        LUN_Init(&luns[i], 0, 0, 0, 0, 0, 0, 0, 0);

    // Memory initialization
#if defined(AT91C_BASE_DDR2C)
    TRACE_INFO("MEM: DDR2\n\r");
    BOARD_ConfigureDdram(0, BOARD_DDRAM_BUSWIDTH); // Micron, 16 bit data bus size

    MEDDdram_Initialize(&(medias[DRV_RAMDISK]),
                        BLOCK_SIZE,
                        (unsigned int)(AT91C_DDR2 + CODE_SIZE) / BLOCK_SIZE,
                        RAMDISK_SIZE / BLOCK_SIZE);
    LUN_Init(&(luns[DRV_RAMDISK]), &(medias[DRV_RAMDISK]),
             msdBuffer, MSD_BUFFER_SIZE,
             0, 0, 0, 0,
             MSDCallbacks_Data);
    numMedias = 1;

#elif defined(AT91C_EBI_SDRAM)

    TRACE_INFO("MEM: SDRAM\n\r");
#if !defined(sdram)
    BOARD_ConfigureSdram(BOARD_SDRAM_BUSWIDTH);
#endif
    
    MEDSdram_Initialize(&(medias[DRV_RAMDISK]),
                        BLOCK_SIZE,
                        (unsigned int)(AT91C_EBI_SDRAM + CODE_SIZE)/BLOCK_SIZE,
                        RAMDISK_SIZE / BLOCK_SIZE);
    LUN_Init(&(luns[DRV_RAMDISK]),
             &(medias[DRV_RAMDISK]),
             msdBuffer, MSD_BUFFER_SIZE,
             0, 0, 0, 0,
             MSDCallbacks_Data);
    numMedias = 1;

#endif // AT91C_EBI_SDRAM

    // SD Card
#if defined(BOARD_SD_MCI_BASE)
    TRACE_DEBUG("MEM: SD Card\n\r");
    SDConnectionUpdate(1);
    numMedias = 2;
#endif

    // Flash 
#if defined(CHIP_FLASH_EFC) || defined(CHIP_FLASH_EEFC)
    TRACE_INFO("MEM: Flash\n\r");
    if (numMedias == 0) {
 
        FLA_Initialize(&(medias[DRV_IFLASH]), AT91C_BASE_EFC);
        LUN_Init(&(luns[DRV_IFLASH]),
                 &(medias[DRV_IFLASH]),
                 msdBuffer, MSD_BUFFER_SIZE,
                 CODE_SIZE,
                 AT91C_IFLASH_SIZE - CODE_SIZE,
                 BLOCK_SIZE,
                 0,
                 MSDCallbacks_Data);
        numMedias = 1;
    }
#endif // #if defined(CHIP_FLASH_EFC) || defined(CHIP_FLASH_EEFC)
}
#else
static void MemoryInitialization(void)
{
    unsigned int i;
    for (i = 0; i < MAX_LUNS; i ++) 
        LUN_Init(&luns[i], 0, 0, 0, 0, 0, 0, 0, 0);

    #if defined(BOARD_EBI_PSRAM)
    // Currently working on Sam3u-PSRAM
    BOARD_ConfigurePsram();

    // Initialize 10M for ram disk & code.
    if (1 != MEDRamDisk_Initialize(&(medias[DRV_RAMDISK]),
                                   BLOCK_SIZE,
                                   (BOARD_EBI_PSRAM + CODE_SIZE) / BLOCK_SIZE,
                                   RAMDISK_SIZE / BLOCK_SIZE)) {
        TRACE_ERROR("FAIL!\n\r");
        return;
    }
    // 512k Disk
    LUN_Init(&(luns[DRV_RAMDISK]), &(medias[DRV_RAMDISK]), msdBuffer,
        MSD_BUFFER_SIZE, 0, 0, 0, 0 , MSDCallbacks_Data);
    #endif

    // SD Disk
    // SDConnectionUpdate(1);
    numMedias = 2;

     // Nand flash
     //NandFlashInitialize();
}
#endif

#if defined (CP15_PRESENT)
//------------------------------------------------------------------------------
/// Put the CPU in 32kHz, disable PLL, main oscillator
/// Put voltage regulator in standby mode
//------------------------------------------------------------------------------
static void LowPowerMode(void)
{
    PMC_CPUInIdleMode();
}
//------------------------------------------------------------------------------
/// Put voltage regulator in normal mode
/// Return the CPU to normal speed 48MHz, enable PLL, main oscillator
//------------------------------------------------------------------------------
static void NormalPowerMode(void)
{
}

#elif defined(at91sam7a3)
//------------------------------------------------------------------------------
/// Put the CPU in 32kHz, disable PLL, main oscillator
//------------------------------------------------------------------------------
static void LowPowerMode(void)
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
static void NormalPowerMode(void)
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
static void LowPowerMode(void)
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
static void NormalPowerMode(void)
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
static void LowPowerMode(void)
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
static void NormalPowerMode(void)
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
static void LowPowerMode(void)
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
static void NormalPowerMode(void)
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
/// Put the CPU in low power mode (for customer)
//------------------------------------------------------------------------------
static void LowPowerMode(void)
{
}

//------------------------------------------------------------------------------
/// Return the CPU to normal speed (for customer)
//------------------------------------------------------------------------------
static void NormalPowerMode(void)
{
}
#endif

//------------------------------------------------------------------------------
/// Initializes the Mass Storage driver and runs it.
//------------------------------------------------------------------------------
int main(void)
{
    unsigned char sdConnected = 0;
    unsigned char sdInitErrorCnt = 0, sdInitExecDelay = 0;

    TRACE_CONFIGURE(DBGU_STANDARD, 115200, BOARD_MCK);
    printf("-- USB Device Mass Storage Project %s --\n\r", SOFTPACK_VERSION);
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

    WAKEUP_CONFIGURE();

    // Start TC for timing & status update
    ConfigureTc0();

    MemoryInitialization();

    ASSERT(numMedias > 0, "Error: No media defined.\n\r");
    TRACE_INFO("%u medias defined\n\r", numMedias);

    // BOT driver initialization
    MSDDriver_Initialize(luns, numMedias);

    // connect if needed
    VBUS_CONFIGURE();

    while (USBD_GetState() < USBD_STATE_CONFIGURED);


    // Infinite loop
    updateDelay = UPDATE_DELAY;
    updateView = 0;
    while (1) {

      #if defined(BOARD_SD_MCI_BASE)
        // SD Card disconnection
        if (MEDSdcard_Detect(&medias[DRV_SDMMC], 0)) {
            if (sdConnected == 0) {
                // Try several times
                if (sdInitExecDelay == 0) {
                    sdInitExecDelay = sdInitErrorCnt + 1;
                }
            }
        }
        else if (sdConnected) {
            sdConnected = 0;
            sdInitErrorCnt = 0;
            SDConnectionUpdate(0);
            printf("\n\r** SD removed!\n\r");
        }
      #endif

        // Mass storage state machine
        if (USBD_GetState() < USBD_STATE_CONFIGURED){}
        else MSDDriver_StateMachine();

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

        // Update status view
        if (updateView) {

            updateView = 0;
            #if 0
            if (msdWriteTotal < 50 * 1000)
                MED_Flush(&medias[DRV_NAND]);
            #endif

            printf("Read %5dK, Write %5dK, IO %5dK; Null %4d, Full %4d\r",
                msdReadTotal/(UPDATE_DELAY*250),
                msdWriteTotal/(UPDATE_DELAY*250),
                (msdReadTotal+msdWriteTotal)/(UPDATE_DELAY*250),
                msdNullCnt, msdFullCnt);

            msdReadTotal = 0;
            msdWriteTotal = 0;
            msdNullCnt = 0;
            msdFullCnt = 0;
 
            #if defined(BOARD_SD_MCI_BASE) 
            if (sdInitExecDelay) {
                if (0 == --sdInitExecDelay) {
                    sdConnected = SDConnectionUpdate(1);
                    if (!sdConnected) {
                        if (SD_INIT_RETRY <= sdInitErrorCnt++) {
                            printf("\n\r** SD inserted but init fail!\n\r");
                            sdConnected = 1;
                        }
                    }
                }
            }
            #endif
        }
    }
}

