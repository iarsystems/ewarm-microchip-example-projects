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
/// \dir "basic-sdmmc-test-project"
///
/// !!!Purpose
///
/// The basic-sdmmc-test-project is a collection of methods to test SD related
/// performances. You can also find SD & USB MSD related informations here.
///
/// You can find following information depends on your needs:
/// - Sample usage of SD/MMC card initialize, write and read
///    - Usage of auto detection of sdcard insert and sdcard write-protection
///    - MCI interface initialize sequence and interrupt installation
///    - SD/MMC card driver implementation based on MCI interface 
///    - SD card physical layer initialize sequence implementation
///    - MMC card physical layer initialize sequence implementation
/// - Sample usage of USB MSD driver.
///    - USB MSD driver development based on the AT91 USB Framework.
///    - USB enumerate sequence, the standard and class-specific descriptors and
///      requests handling.
///    - The initialize sequence and usage of UDP interface.
/// 
/// !See
/// - sdmmc_mci : SD/MMC card physical layer driver with mci-interface
/// - mci       : driver for mci-interface
/// - mci_hs    : driver for mci2-interface
/// - memories: Storage Media interface for MSD
/// - usb: USB Framework, USB MSD driver and UDP interface driver
///    - "AT91 USB device framework"
///       - "USBD API"
///    - "massstorage"
///       - "USB MSD Driver"
/// - "Basic-sdcard-project"
/// - "USB Device Mass Storage Project"
///
/// !!!Requirements
///
/// This package can be used with all Atmel evaluation kits that have MCI/MCI2
/// interface.
///
/// !!!Description
///
/// Open HyperTerminal before running this program, use SAM-BA to download this
/// program to SDRAM or DDRAM, make the program run, the HyperTerminal will
/// give out the test hints, you can run different tests on a inserted card.
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
/// -# In HyperTerminal, it will show something like on start up
///     \code
///     -- Basic SD/MMC Test Project xxx --
///     -- AT91xxxxxx-xx
///     -- Compiled: xxx xx xxxx xx:xx:xx --
///     -I- Cannot check if SD card is write-protected
///     -I- DMAD_Initialize channel 0  
///     TC Start ... OK
///
///     ==========================================
///     -I- Card Type 1, CSD_STRUCTURE 0
///     -I- SD 4-BITS BUS
///     -I- CMD6(1) arg 0x80FFFF01
///     -I- SD HS Not Supported
///     -I- SD/MMC TRANS SPEED 25000 KBit/s
///     -I- SD/MMC card initialization successful
///     -I- Card size: 483 MB, 990976 * 512B
///     ...
///     \endcode
/// -# Test function menu is like this
///     \code
///     # 0,1,2 : Block read test
///     # w,W   : Write block test(With data or 0)
///     # b,B   : eMMC boot mode or access boot partition change
///     # i,I   : Re-initialize card
///     # t     : Disk R/W/Verify test
///     # T     : Disk performance test
///     # p     : Change number of blocks in one access for test
///     # s     : Change MCI Clock for general test
///     # h     : Auto Switch to HS mode on init On/Off
///     \endcode
/// -# If connect USB to PC the current MSD R/W speed will be displayed
///     \code
///     Read     0K, Max 14876K; Write     0K, Max 14094K; IO     0K
///     \endcode
///
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// \unit
///
/// !Purpose
///
/// This file contains all the specific code for the basic-sdmmc-test-project
///
/// !Contents
/// The basic-sdmmc-project application can be roughly broken down as follows:
///    - Optional functions
///       - CheckProtection
///       - CardDetectConfigure, CardIsConnected
///    - Interrupt handlers
///       - ISR_Mci0
///       - ISR_Tc0
///    - The main function, which implements the program behavior
///       - I/O configuration
///       - TC0 configuration
///       - SD/MMC card auto-detect write-protected-check (if supported)
///       - Initialize MCI interface and installing an isr relating to MCI
///       - Initialize sdcard, get necessary sdcard's parameters
///       - write/read sdcard at 10MHz SD/MMC clock
///       - write/read sdcard at Maxmum SD/MMC clock 
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//         Header
//------------------------------------------------------------------------------

#include <board.h>
#include <irq/irq.h>

#include <pio/pio.h>
#include <pio/pio_it.h>

#include <dbgu/dbgu.h>
#include <utility/assert.h>
#include <utility/trace.h>
#include <sdmmc/sdmmc_mci.h>

#include <usb/device/massstorage/MSDDriver.h>
#include <usb/device/massstorage/MSDLun.h>

#include <memories/MEDSdmmc.h>

#include "tc/tc.h"
#if defined(MCI2_INTERFACE)
#include "dmad/dmad.h"
#endif

#include <string.h>

//------------------------------------------------------------------------------
//         Macros
//------------------------------------------------------------------------------
#define MAX(a,b)    (((a)>(b))?(a):(b))

//------------------------------------------------------------------------------
//         Local consts
//------------------------------------------------------------------------------
/// Maximum number of blocks read once (for performance test)
#define NB_MULTI_BLOCKS     16
#define NB_MULTI_STEP       3
#define NB_MULTI_MAX        16

/// Split R/W to 2, first R/W 4 blocks then remaining
#define NB_SPLIT_MULTI      4

/// Test settings
#define TEST_BLOCK_START    (128 * 2)   // From 128K
//#define TEST_BLOCK_END      15100
#define TEST_BLOCK_END      SD_TOTAL_BLOCK(&sdDrv) // Whole SD Card

#define TEST_BLOCK_SKIP     (100 * 1024 * 2)    // 100M

#define TEST_PERFORMENCT_SIZE   (4*1024*1024)

#define TEST_FILL_VALUE_U32     (0x5A6C1439)
//#define TEST_FILL_VALUE_U32     (0x5AF01439)
//#define TEST_FILL_VALUE_U32     (0x50F01439)

//#define TEST_FILL_VALUE_U32     (0x5A601439)
//#define TEST_FILL_VALUE_U32     (0x5A001439)
//#define TEST_FILL_VALUE_U32     (0x5b401439)

/// Number of errors displayed
#define NB_ERRORS       15
#define NB_BAD_BLOCK    200

/// Maximum number of LUNs
#define MAX_LUNS        1

/// Media index for disks
#define DRV_SDMMC       0

/// Retry 3 times when SD is inserted
#define SD_INIT_RETRY       3

/// Size of the MSD IO buffer in bytes
/// Should be at least 3 * MSDIO_READ10/WRITE10_CHUNK_SIZE(MSDIOFifo.h)
#define MSD_BUFFER_SIZE     (16*SD_BLOCK_SIZE)

/// Size of buffer for SD.MMC test
#define TEST_BUFFER_SIZE    (MAX(NB_MULTI_MAX*SD_BLOCK_SIZE, MSD_BUFFER_SIZE))

/// Delay for display view update (*0.1ms, 1s)
#define UPDATE_DELAY        10000

/// Delay for USB connection (*0.1ms)
#define USB_CONN_DELAY      5000

//------------------------------------------------------------------------------
//         Global variables
//------------------------------------------------------------------------------

/// SD Auto HS mode control
extern unsigned char gSdmmcAutoHsEnable;

/// Available medias.
Media medias[MAX_LUNS];

//------------------------------------------------------------------------------
//         Local variables
//------------------------------------------------------------------------------

/// MCI driver instance list.
static Mci mciDrv;

/// SDCard driver instance list.
static SdCard sdDrv;

/// Current SD speed
unsigned int sdSpeed = MCI_INITIAL_SPEED;

/// SD card pins.
static const Pin pinsSd[] = {BOARD_SD_PINS};

#if MCI_BUSY_CHECK_FIX && defined(BOARD_SD_DAT0)
/// SD DAT0 pin
static const Pin pinSdDAT0 = BOARD_SD_DAT0;
#endif

//- Global Date buffer(s)
/// Large buffer block for MSD && SD Performance test
static unsigned char pBuffer[TEST_BUFFER_SIZE];
/// Single block buffer for data dump
static unsigned char pBlockBuffer[SD_BLOCK_SIZE];

static unsigned int testFillValue = TEST_FILL_VALUE_U32;

static unsigned int nbErrors;

static unsigned int performanceMultiBlock = NB_MULTI_BLOCKS;

static volatile unsigned int tick100u = 0;

/// Test MCI clock speed (in MHz)
static const unsigned char speedList[] = {5, 10, 15, 20, 25, 30, 50, 60};

/// Device LUNs.
static MSDLun luns[MAX_LUNS];

/// LUN read/write buffer.
static unsigned char * msdBuffer = pBuffer;

/// Total data read/write by MSD
static unsigned int msdReadTotal = 0;
static unsigned int msdWriteTotal = 0;
static unsigned int msdReadPeak = 0;
static unsigned int msdWritePeak = 0;

/// Update delay counter, tick is 0.1ms
static unsigned int updateDelay = 10000;

/// Flag to update Display View
static unsigned char updateView = 0;

//------------------------------------------------------------------------------
//         Local macros
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//         Local functions
//------------------------------------------------------------------------------

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
}

//------------------------------------------------------------------------------
/// Interrupt handler for timer.
//------------------------------------------------------------------------------
void TC0_IrqHandler(void)
{
    volatile unsigned int dummy;
    // Clear status bit to acknowledge interrupt
    dummy = AT91C_BASE_TC0->TC_SR;

    tick100u ++;
    if (-- updateDelay == 0) {
        updateDelay = UPDATE_DELAY;
        updateView = 1;
    }
}

//------------------------------------------------------------------------------
/// Configure Timer Counter 0 to generate an interrupt every 0.1ms.
//------------------------------------------------------------------------------
void ConfigureTc0(void)
{
    unsigned int div;
    unsigned int tcclks;

    // Enable peripheral clock
    AT91C_BASE_PMC->PMC_PCER = 1 << AT91C_ID_TC0;

    // Configure TC for a 10000Hz frequency and trigger on RC compare
    TC_FindMckDivisor(10000, BOARD_MCK, &div, &tcclks);
    TC_Configure(AT91C_BASE_TC0, tcclks | AT91C_TC_CPCTRG);
    AT91C_BASE_TC0->TC_RC = (BOARD_MCK / div) / 10000; // timerFreq/desiredFreq

    // Configure and enable interrupt on RC compare
    IRQ_ConfigureIT(AT91C_ID_TC0, 1, TC0_IrqHandler);
    AT91C_BASE_TC0->TC_IER = AT91C_TC_CPCS;
    IRQ_EnableIT(AT91C_ID_TC0);

    tick100u = 0;
    TC_Start(AT91C_BASE_TC0);
}

//------------------------------------------------------------------------------
/// MCI0 interrupt handler. Forwards the event to the MCI driver handler.
//------------------------------------------------------------------------------
// Defined in MEDSD
void MCI0_IrqHandler(void)
{
    MCI_Handler(&mciDrv);
}

//------------------------------------------------------------------------------
//         Optional: SD card detection
//------------------------------------------------------------------------------

#ifdef BOARD_SD_PIN_CD

/// SD card detection pin instance.
static const Pin pinCardDetect = BOARD_SD_PIN_CD;

//------------------------------------------------------------------------------
/// Configure for SD detect pin
//------------------------------------------------------------------------------
static void CardDetectConfigure(void)
{
    PIO_Configure(&pinCardDetect, 1);
}

static unsigned char CardIsConnected(unsigned char slot)
{
    return PIO_Get(&pinCardDetect) ? 0 : 1;
}
#else

#define CardDetectConfigure()   \
    printf("-I- No Card Detect Pin, default connected\n\r")

#define CardIsConnected(slot)       1
#endif

//------------------------------------------------------------------------------
//         Optional: Write protection status
//------------------------------------------------------------------------------

#ifdef BOARD_SD_PIN_WP

/// Write protection status pin instance.
static const Pin pinMciWriteProtect = BOARD_SD_PIN_WP;

//------------------------------------------------------------------------------
/// Checks if the device is write protected.
//------------------------------------------------------------------------------
void CheckProtection(unsigned char slot)
{
    PIO_Configure(&pinMciWriteProtect, 1);
    if (PIO_Get(&pinMciWriteProtect) != 0) {

        printf("-I- SD card is write-protected\n\r");
    }
    else {

        printf("-I- SD card is NOT write-protected.\n\r");
    }
}

#else

//------------------------------------------------------------------------------
/// Dummy implementation.
//------------------------------------------------------------------------------
void CheckProtection(unsigned char slot)
{
    printf("-I- Cannot check if SD card is write-protected\n\r");
}

#endif

//------------------------------------------------------------------------------
/// Execute SD initialize procedure.
/// \return 1 if success
//------------------------------------------------------------------------------
static unsigned char SDInitExec(void)
{
    unsigned char error;
    volatile unsigned int sizeInMB, sizeOfBlock;
    unsigned int actSpeed;

     // Init MCI interface driver
    MCI_Init(&mciDrv, BOARD_SD_MCI_BASE, BOARD_SD_MCI_ID, BOARD_SD_SLOT);

  #if MCI_BUSY_CHECK_FIX && defined(BOARD_SD_DAT0)
    MCI_SetBusyFix(&mciDrv, &pinSdDAT0);
  #endif

  #if defined(MCI2_INTERFACE)
    // Enable DMA so SD data can be transferred
    DMAD_Initialize(BOARD_MCI_DMA_CHANNEL);
  #endif
    error = SD_Init(&sdDrv, (SdDriver *)&mciDrv);
    if (error) {

      #if defined(MCI2_INTERFACE)
        DMA_DisableChannel(BOARD_MCI_DMA_CHANNEL);
      #endif
        printf("-E- SD/MMC card initialization failed: %d\n\r", error);
        return 0;
    }
    else {

        if (SD_TOTAL_SIZE(&sdDrv) == 0xFFFFFFFF) {
            sizeInMB = SD_TOTAL_BLOCK(&sdDrv) / (1024 * 2);
            sizeOfBlock = 512;
        }
        else {
            sizeInMB = SD_TOTAL_SIZE(&sdDrv) / (1024 * 1024);
            sizeOfBlock = SD_TOTAL_SIZE(&sdDrv) / SD_TOTAL_BLOCK(&sdDrv);
        }

        printf("-I- SD/MMC card initialization successful\n\r");
        printf("-I- Card size: %u MB, %u * %dB\n\r",
               sizeInMB, SD_TOTAL_BLOCK(&sdDrv), sizeOfBlock);
    }
    actSpeed = MCI_SetSpeed(&mciDrv,
                            sdDrv.transSpeed,
                            sdDrv.transSpeed,
                            BOARD_MCK);
    printf("-I- MCI Clock set to %d\n\r", actSpeed);
    return 1;
}

//------------------------------------------------------------------------------
/// Release SD resources
//------------------------------------------------------------------------------
static void SDDeInitExec(void)
{
    SD_Stop(&sdDrv, (SdDriver *)&mciDrv);
  #if defined(MCI2_INTERFACE)
    DMA_DisableChannel(BOARD_MCI_DMA_CHANNEL);
  #endif
}

//------------------------------------------------------------------------------
/// SD card connection/disconnection handler, to initialize and link SD Media
/// to corresponding LUN or unlink it.
/// \param inserted SD card is inserted/ejected.
//------------------------------------------------------------------------------
static unsigned char SDConnectionUpdate(unsigned char inserted)
{
    if (inserted) {
        // Initialize SD card
        if (!SDInitExec())
            return 0;
        // Faster, non-blocked SD access function
        MEDSdusb_Initialize(&(medias[DRV_SDMMC]), &sdDrv, 0);
        LUN_Init(&(luns[DRV_SDMMC]), &(medias[DRV_SDMMC]),
                 msdBuffer, MSD_BUFFER_SIZE,
                 0, 0, 0, 0,
                 MSDCallbacks_Data);
        return 1;
    }
    else {
        LUN_Eject(&luns[DRV_SDMMC]);
    }
    return 0;
}

//------------------------------------------------------------------------------
/// Get Input
//------------------------------------------------------------------------------
static char GetDecInput(unsigned char numChar, unsigned int *pInt)
{
    unsigned char key;
    unsigned int  i;
    unsigned int  result = 0;
    for (i = 0; i < numChar;) {
        key = DBGU_GetChar();
        if (key == 27) {
            printf(" Canceled\n\r");
            return key;
        }
        if (key > '9' || key < '0') continue;
        DBGU_PutChar(key);
        result = result * 10 + (key - '0');
        i ++;
    }
    if (pInt) *pInt = result;
    return 0;
}
static char GetHexInput(unsigned char numChar, unsigned int * pInt)
{
    unsigned char key, num;
    unsigned int  i;
    unsigned int  result = 0;
    for (i = 0; i < numChar;) {
        key = DBGU_GetChar();
        if (key == 27) {
            printf(" Canceled\n\r");
            return key;
        }
        if (key >= '0' && key <= '9') num = key - '0';
        else if (key >= 'a' && key <= 'f') num = key - 'a' + 0xA;
        else if (key >= 'A' && key <= 'F') num = key - 'A' + 0xA;
        else continue;
        DBGU_PutChar(key);
        result = (result << 4) + num;
        i ++;
    }
    if (pInt) *pInt = result;
    return 0;
}

//------------------------------------------------------------------------------
/// Get Delayed number of tick
//------------------------------------------------------------------------------
unsigned int GetDelayInTicks(unsigned int startTick, unsigned int endTick)
{
    if (endTick > startTick) return (endTick - startTick);
    return (endTick + (0xFFFFFFFF - startTick));
}


//------------------------------------------------------------------------------
/// Delay ms
//------------------------------------------------------------------------------
void DelayMs(unsigned char ms)
{
    unsigned int oldTick;
    unsigned int cnt;

    while(ms --) {
        for(cnt = 0; cnt < 10; cnt ++) {
            oldTick = tick100u;
            while(oldTick == tick100u);
        }
    }
}

//------------------------------------------------------------------------------
/// Max Error Break
//------------------------------------------------------------------------------
unsigned char MaxErrorBreak(unsigned char halt)
{
    if (NB_ERRORS) {
        if (nbErrors ++ > NB_ERRORS) {

            while(halt);

            nbErrors = 0;
            return 1;
        }
    }
    return 0;
}

//------------------------------------------------------------------------------
/// Dump Splitting row
//------------------------------------------------------------------------------
static void DumpSeperator(void)
{
    printf("\n\r==========================================\n\r");
}

//------------------------------------------------------------------------------
/// Dump main menu
//------------------------------------------------------------------------------
void DumpMenu(void)
{
    DumpSeperator();
    printf("# 0,1,2 : Block read test\n\r");
    printf("# w,W   : Write block test(With data or 0)\n\r");
    printf("# i,I   : Re-initialize card\n\r");
    printf("# t     : Disk R/W/Verify test\n\r");
    printf("# T     : Disk performance test\n\r");
    printf("# p     : Change number of blocks in one access for test\n\r");
    printf("# s     : Change MCI Clock for general test\n\r");
  #if defined(MCI2_INTERFACE)
    printf("# h     : Auto Switch to HS mode on init On/Off\n\r");
  #endif
}

//------------------------------------------------------------------------------
/// Dump block & information
//------------------------------------------------------------------------------
void DumpBlock(unsigned char * pData, unsigned int block)
{
    unsigned int i;
    //printf("-I- Block %d: %s .. %s..", block, pData, &pData[8]);
    printf("-I- Block %d: %c .. %c .. %c .. %c..",
            block, pData[0], pData[3], pData[8], pData[8+5]);
    for (i = 0; i < 512; i ++) {
        if((i % 16) == 0) printf("\n\r%3x:", i);
        printf(" %02X", pData[i]);
    }
    printf("\n\r");
}

//------------------------------------------------------------------------------
/// Dump card registers
//------------------------------------------------------------------------------
static void DumpCardInfo(unsigned char slot)
{
    SD_DisplayRegisterCID(&sdDrv);
    SD_DisplayRegisterCSD(&sdDrv);
    SD_DisplayRegisterECSD(&sdDrv);
    SD_DisplayRegisterSCR(&sdDrv);
    SD_DisplaySdStatus(&sdDrv);
}

//------------------------------------------------------------------------------
/// Run boot access test on the inserted card
/// \param slot     The slot used for card accessing.
/// \param newSpeed Target speed.
//------------------------------------------------------------------------------
unsigned char ChangeMciSpeed(unsigned char slot, unsigned int newSpeed)
{
    unsigned char overrun = 0;
    unsigned int  mciDiv;

    MCI_SetSpeed(&mciDrv, newSpeed, 0, BOARD_MCK);
    sdSpeed = MCI_GetSpeed(&mciDrv, &mciDiv);
    printf("-I- MCI DIV -> %d, Speed %d\n\r", mciDiv, sdSpeed);
    if (sdSpeed > sdDrv.transSpeed) {
        overrun = 1;
        printf("-W- Overrun speed!!!!\n\r");
    }

    return overrun;
}

//------------------------------------------------------------------------------
/// Run tests on the inserted card
//------------------------------------------------------------------------------
void CardInit(unsigned char slot)
{
    unsigned int mciSpeed;
    unsigned int mciDiv;

    DumpSeperator();

    if (!SDInitExec())
        return;

    DumpCardInfo(slot);

    mciSpeed = 10000000;
    ChangeMciSpeed(slot, mciSpeed);

    MCI_GetSpeed(&mciDrv, &mciDiv);
    printf("-I- MCK %dK Hz, MCI Speed %dK, divisor %d. \r\n",
           BOARD_MCK/1000, mciSpeed/1000, mciDiv);

}

//------------------------------------------------------------------------------
/// Block test (read)
//------------------------------------------------------------------------------
void BlockTest(unsigned char slot, unsigned int block)
{
    DumpSeperator();

    block *= 256;
    printf("-I- Read Block %d: %d\n\r",
            block, SD_ReadBlock(&sdDrv, block, 1, pBlockBuffer));
    DumpBlock(pBlockBuffer, block);

    block ++;
    printf("-I- Read Block %d: %d\n\r",
            block, SD_ReadBlock(&sdDrv, block, 1, pBlockBuffer));
    DumpBlock(pBlockBuffer, block);
}

//------------------------------------------------------------------------------
/// Block Dump (read)
//------------------------------------------------------------------------------
void BlockDump(unsigned char slot)
{
    unsigned int block;
    DumpSeperator();
    printf("-!- Input block:");
    if (GetDecInput(5, &block))
        return;
    printf("-I- Dump Block %d: %d\n\r",
            block, SD_ReadBlock(&sdDrv, block, 1, pBlockBuffer));
    DumpBlock(pBlockBuffer, block);
}

//------------------------------------------------------------------------------
/// Write test
//------------------------------------------------------------------------------
void WriteTest(unsigned char slot, unsigned char part, unsigned char opt)
{
    unsigned int i, block;
    const char* str[] = {"USER 0",
                         "BOOT 1",
                         "BOOT 2"};
    unsigned char rBuffer[512];

    DumpSeperator();

    printf("-I- Write first 128K with %s\n\r", opt ? "0x00" : "data");


    for (block = 0; block < 256; block ++) {

        // Prepare data
        memset(pBuffer, 0, 512);
        if (opt == 0) {
            sprintf((char*)pBuffer, "%04u", block);
            memcpy(&pBuffer[8], str[part], 6);

            for (i = 16; i < 512; i ++) {
                pBuffer[i] = (unsigned char)i;
            }
        }

        printf("-I- Write Block(%d) %s .. %x ..: %d\n\r",
                block, pBuffer, pBuffer[8],
                SD_WriteBlock(&sdDrv, block, 1, pBuffer));

        memset(rBuffer, 0xFF, 512);
        printf("-I- Read Block(%d):%d, ",
                block,
                SD_ReadBlock(&sdDrv, block, 1, rBuffer));

        printf("Verify ..");
        for (i = 0; i < 512; i ++) {
            if (rBuffer[i] != pBuffer[i]) {
                printf("Fail@ %d ..", i);
                break;
            }
        }
        printf("Done\n\r");
    }
    
}

//------------------------------------------------------------------------------
/// Disk test
//------------------------------------------------------------------------------
void DiskTest(unsigned char slot,
              unsigned char clr,
              unsigned char wr,
              unsigned char rd)
{
    unsigned char error = 0;
    unsigned int i, errcnt = 0;
    unsigned int multiBlock, block, splitMulti;

    DumpSeperator();

    // Perform tests on each block
    multiBlock = 0;
    for (block = TEST_BLOCK_START;
         block < TEST_BLOCK_END;
         block += multiBlock) {

        // Perform different single or multiple bloc operations
        if (++multiBlock > NB_MULTI_BLOCKS)
            multiBlock = 1;

        // Multi-block adjustment
        if (block + multiBlock > TEST_BLOCK_END) {
            multiBlock = TEST_BLOCK_END - block;
        }

        // ** Perform single block or multi block transfer
        printf("\r-I- Testing block [%6u - %6u] ...",
               block, (block + multiBlock -1));

        if (clr) {
            // - Clear the block
            memset(pBuffer, 0, SD_BLOCK_SIZE * multiBlock);
            for (i=0; i < SD_BLOCK_SIZE * multiBlock; i++) {
                if (pBuffer[i] != 0) {
                    printf("\n\r-E- Data @ %u for write : 0x00 <> 0x%02x\n\r",
                           i, pBuffer[i]);
                    if(MaxErrorBreak(0)) return;
                    // Only find first verify error.
                    continue;
                }
            }
            error = SD_WriteBlock(&sdDrv, block, multiBlock, pBuffer);
            if (error) {
                printf("\n\r-E- 1. Write block (%d) #%u\n\r", error, block);
                if(MaxErrorBreak(0)) return;
                // Skip following test
                continue;
            }
            // - Read back the data to check the write operation
            memset(pBuffer, 0xFF, SD_BLOCK_SIZE * multiBlock);
            error = SD_ReadBlock(&sdDrv, block, multiBlock, pBuffer);
            if (error) {
                printf("\n\r-E- 1. Read block (%d) #%u\n\r", error, block);
                if(MaxErrorBreak(0)) return;
                // Skip following test
                continue;
            }
            for (i=0; i < SD_BLOCK_SIZE * multiBlock; i++) {
                if (pBuffer[i] != 0) {
                    printf("\n\r-E- 1. B%u.D[%u] : 0 <> 0x%02X\n\r",
                           block,
                           i, pBuffer[i]);
                    if(MaxErrorBreak(0)) return;
                    // Only find first verify error.
                    break;
                }
            }
        }

        if (wr) {
            // - Write a checkerboard pattern on the block
            for (i=0; i < SD_BLOCK_SIZE * multiBlock; i++) {
                if ((i & 1) == 0)  pBuffer[i] = (i & 0x55);
                else               pBuffer[i] = (i & 0xAA);
            }
            for (i = 0; i < multiBlock; ) {
                splitMulti = ((multiBlock - i) > NB_SPLIT_MULTI) ?
                                        NB_SPLIT_MULTI : (multiBlock - i);
                error = SD_WriteBlock(&sdDrv,
                                      block + i,
                                      splitMulti,
                                      &pBuffer[i * SD_BLOCK_SIZE]);
                if (error) break;
                i += splitMulti;
            }
            ASSERT(i == multiBlock, "Unexpected W, %u!", i);
            if (error) {
                printf("\n\r-E- 2. Write block #%u(%u+%u)\n\r",
                       block+i, block, i);
                if(MaxErrorBreak(0)) return;
                // Skip Following Test
                continue;
            }
        }

        if (rd) {
            // - Read back the data to check the write operation
            memset(pBuffer, 0, SD_BLOCK_SIZE * multiBlock);
            for (i = 0; i < multiBlock; ) {
                splitMulti = ((multiBlock - i) > NB_SPLIT_MULTI) ?
                                        NB_SPLIT_MULTI : (multiBlock - i);
                error = SD_ReadBlock(&sdDrv,
                                     block + i,
                                     splitMulti,
                                     &pBuffer[i * SD_BLOCK_SIZE]);
                if (error) break;
                i += splitMulti;
            }
            ASSERT(i == multiBlock, "Unexpected R, %u!", i);
            if (error) {
                printf("\n\r-E- 2. Read block #%u(%u+%u)\n\r",
                       block + i, block, i);
                if(MaxErrorBreak(0)) return;
                // Skip Following Test
                continue;
            }
            errcnt = 0;
            for (i=0; i < SD_BLOCK_SIZE * multiBlock; i++) {

                if (!(((i & 1) == 0) && (pBuffer[i] == (i & 0x55))) &&
                    !(((i & 1) != 0) && (pBuffer[i] == (i & 0xAA))) ) {
                    unsigned int j, js;
                    printf("\n\r-E- 2.%d. Data @ %u (0x%x)\n\r", errcnt, i, i);
                    printf("  -Src:");
                    js = (i > 8) ? (i - 8) : 0;
                    for (j = js; j < i + 8; j ++)
                        printf(" %02x", ((j & 1)!= 0) ? (j & 0xAA):(j & 0x55));
                    printf("\n\r  -Dat:");
                    for (j = js; j < i + 8; j ++)
                        printf("%c%02x", (i == j) ? '!' : ' ', pBuffer[j]);
                    printf("\n\r");
                    if(MaxErrorBreak(0)) return;
                    // Only find first 3 verify error.
                    if (errcnt ++ >= 3)
                        break;
                }
            }
        }

        if (DBGU_IsRxReady()) {
            switch(DBGU_GetChar()) {
                // Skip 100M
                case 'k':
                    block += TEST_BLOCK_SKIP;
                    if (block > TEST_BLOCK_END) {
                        block -= 5 + multiBlock;
                    }
                    printf("\n\r");
                    break;
                // Cancel
                case 'c':
                    return;
            }
        }
    }

    printf("All block tested!\n\r");
}

//------------------------------------------------------------------------------
/// Run performence test
/// R/W test can be masked to verify previous written data only
//------------------------------------------------------------------------------
void PerformanceTest(unsigned char slot,
                     unsigned char wr,
                     unsigned char rd,
                     unsigned char errDetail)
{
    unsigned char error = 0;
    unsigned char round, pause = 0;
    unsigned int  block, i, nBadBlock = 0, nErrors;
    unsigned int  speed = sdSpeed;
    unsigned int  tickStart, tickEnd, ticks, rwSpeed;

    DumpSeperator();

    printf("-I- Performence test, size %dK, Multi %d, MCK %dMHz\n\r",
                                    TEST_PERFORMENCT_SIZE/1024,
                                    performanceMultiBlock,
                                    BOARD_MCK/1000000);
    printf("-I- RW by blocks, block size %d, fill value 0x%x\n\r",
                                    SD_BLOCK_SIZE,
                                    testFillValue);

    for(round = 0;; round ++) {

        printf("-I- Round %d:\n\r", round);

        if (round > 0) {
            pause = 1;
            printf("-I- (c)ancel? any key to continue:");
        }

        // Selection, to start another loop
        while(pause) {
            // UI, Cancel accepted
            if (DBGU_IsRxReady()) {
                switch(DBGU_GetChar()) {
                    // Cancel
                    case 'c':
                        printf("c\n\r");
                        return;
                }
                printf("\n\r");
                break;
            }
        }

        // Decrease speed
        if (error) {
            while(1) {
                if      (speed >= 50000000) speed = 30000000;
                else if (speed >= 30000000) speed = 25000000;
                else if (speed >= 25000000) speed = 20000000;
                else if (speed >= 20000000) speed = 15000000;
                else if (speed >= 15000000) speed = 10000000;
                else if (speed >= 10000000) speed = 5000000;
                else if (speed >= 5000000)  speed = 2000000;
                else {
                    printf("-!- Fail\n\r");
                    break;
                }
                ChangeMciSpeed(slot, speed);
                speed = sdSpeed;
            }
        }
        printf("-I- MCI Clock %d\n\r", speed);

        if (wr) {
            printf("--- Write test .. ");
            for (i = 0; i < SD_BLOCK_SIZE * performanceMultiBlock; i += 4) {
                *(unsigned int*)&pBuffer[i] = testFillValue;
            }
            nBadBlock = 0;
            tickStart = tick100u;
            for (block = TEST_BLOCK_START;
                 block < (TEST_PERFORMENCT_SIZE/SD_BLOCK_SIZE)
                            + TEST_BLOCK_START;
                 block += performanceMultiBlock) {

                *(unsigned int*)pBuffer = block;
                error = SD_WriteBlock(&sdDrv,
                                      block, performanceMultiBlock,
                                      pBuffer);
                if (error) {
                    if (nBadBlock ++ >= NB_BAD_BLOCK) {
                        printf("-E- WR_B(%u)\n\r", block);
                        break;
                    }
                    else error = 0;
                }
            }
            if (error) continue;
            tickEnd = tick100u;
            ticks = GetDelayInTicks(tickStart, tickEnd);
            rwSpeed = (TEST_PERFORMENCT_SIZE
                        - nBadBlock * performanceMultiBlock * SD_BLOCK_SIZE)
                            * 10 / ticks;
            printf("Done, Bad %u, Speed %uK\n\r", nBadBlock, rwSpeed);
        }

        if (rd) {
            printf("--- Read test .. ");
            nBadBlock = 0;
            tickStart = tick100u;
            for (block = TEST_BLOCK_START;
                 block < (TEST_PERFORMENCT_SIZE/SD_BLOCK_SIZE)
                            + TEST_BLOCK_START;
                 block += performanceMultiBlock) {

                error = SD_ReadBlock(&sdDrv,
                                     block, performanceMultiBlock,
                                     pBuffer);
                if (error) {
                    if (nBadBlock ++ >= NB_BAD_BLOCK) {
                        printf("-E- RD_B(%u)\n\r", block);
                        break;
                    }
                    else error = 0;
                }
                if (error) break;
            }
            if (error) continue;
            tickEnd = tick100u;
            ticks = GetDelayInTicks(tickStart, tickEnd);
            rwSpeed = (TEST_PERFORMENCT_SIZE
                        - nBadBlock * performanceMultiBlock * SD_BLOCK_SIZE)
                            * 10 / ticks;
            printf("Done, Bad %u, Speed %uK\n\r", nBadBlock, rwSpeed);
        }

        printf("--- Data verify .. ");
        nErrors = 0;
        for (block = TEST_BLOCK_START;
             block < (TEST_PERFORMENCT_SIZE/SD_BLOCK_SIZE) + TEST_BLOCK_START;
             block += performanceMultiBlock) {

            memset(pBuffer, 0x00, SD_BLOCK_SIZE * performanceMultiBlock);
            error = SD_ReadBlock(&sdDrv,
                                 block, performanceMultiBlock,
                                 pBuffer);
            if (error) {
                printf("-E- RD_B(%u)\n\r", block);
                break;
            }
            if (*(unsigned int*)pBuffer != block) {
                if (errDetail) {
                    if (nErrors ++ < NB_ERRORS) {
                        printf("-E- Blk(%u)[0](%08x<>%08x)\n\r",
                               block, block, *(unsigned int*)pBuffer);
                    }
                }
                else {
                    printf("-E- BlkN(%x<>%x)\n\r",
                           block, *(unsigned int*)pBuffer);
                    error = 1;
                    break;
                }
            }
            for (i = 4; i < SD_BLOCK_SIZE * performanceMultiBlock; i += 4) {
                if ( (*(unsigned int*)&pBuffer[i]) != testFillValue) {
                    if (errDetail) {
                        // Dump 10 errors only
                        if (nErrors ++ < NB_ERRORS) {
                            unsigned int j;
                            printf("-E- Blk(%u)[%u](%08x.. <>",
                                    block, i, testFillValue);
                            for (j = (i > 4) ? (i - 4) : i;
                                 j <= i + 4;
                                 j += 4) {
                                printf("%c%08X",
                                        (i == j) ? '!' : ' ',
                                        *(unsigned int*)&pBuffer[j]);
                            }
                            printf(")\n\r");
                        }
                    }
                    else {
                    printf("-E- Blk(%u)[%u](%x<>%x)\n\r", block, i,
                            testFillValue,
                            *(unsigned int*)&pBuffer[i]);
                    error = 1;
                    break;
                    }
                }
            }
            if (error) break;
        }
        if (errDetail && nErrors) {
            printf("-I- %u u32 ERRORS found!\n\r", nErrors);
        }
        // Continue to next round if any error happened
        if (error) continue;
        printf("OK\n\r");

        break;
    }
}

static void MemoryInitialization(void)
{
    unsigned int i;
    for (i = 0; i < MAX_LUNS; i ++) 
        LUN_Init(&luns[i], 0, 0, 0, 0, 0, 0, 0, 0);
    numMedias = MAX_LUNS;
}

//------------------------------------------------------------------------------
//         Global functions
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/// Main function
//------------------------------------------------------------------------------
int main(void)
{
    unsigned int i;
    unsigned char sdConnected = 0, usbConnected = 0;
    unsigned char speedNdx = 0;
    unsigned char sdInitErrorCnt = 0, sdInitExecDelay = 0;
    unsigned int usbConnTick = 0;
    
    // Enable auto HS mode default
  #if defined(MCI2_INTERFACE)
    gSdmmcAutoHsEnable = 1;
  #endif

    TRACE_CONFIGURE(DBGU_STANDARD, 115200, BOARD_MCK);
    printf("-- Basic SD/MMC Test Project %s --\n\r", SOFTPACK_VERSION);
    printf("-- %s @ %uHz\n\r", BOARD_NAME, BOARD_MCK);
    printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);
    printf("-- Buffer@%x,size 0x%x\n\r",
           (unsigned int)pBuffer, sizeof(pBuffer));
    
    // If there is on board power, switch it off
  #ifdef PIN_USB_POWER_ENB
  { const Pin pinUsbPwr = PIN_USB_POWER_ENB;
    PIO_Configure(&pinUsbPwr, 1);
  }
  #endif

    // Configure PIO interrupts
    PIO_InitializeInterrupts(0);

    // Configure SDcard pins
    PIO_Configure(pinsSd, PIO_LISTSIZE(pinsSd));

    // Wait for SD card connection (if supported)
    CardDetectConfigure();

    // Check if card is write-protected (if supported)
    CheckProtection(0);

    // Initialize the SD card driver
    // Initialize the MCI driver
    IRQ_ConfigureIT(BOARD_SD_MCI_ID,  1, MCI0_IrqHandler);
    IRQ_EnableIT(BOARD_SD_MCI_ID);

    printf("-I- TC Start ... ");
    ConfigureTc0();
    i = tick100u;
    while(i == tick100u);
    printf("OK\n\r");

    // MSD LUNs initialization
    MemoryInitialization();
    // BOT driver initialization
    MSDDriver_Initialize(luns, MAX_LUNS);
    // connect if needed
    VBUS_CONFIGURE();

    // Card insert detection loop
    for(;;) {

        if (usbConnected) {
            // SD Card disconnection in MSD mode
            if (CardIsConnected(0)) {
                if (sdConnected == 0) {
                    // Try several times
                    if (sdInitExecDelay == 0) {
                        sdInitExecDelay = sdInitErrorCnt + 1;
                    }
                }
            }
            else if (sdConnected) {
                sdConnected = 0;
                msdReadPeak = 0;
                msdWritePeak = 0;
                sdInitErrorCnt = 0;
                SDConnectionUpdate(0);
                printf("\n\r-MSD- SD removed!\n\r");
            }
            // MSD general task
            if (USBD_GetState() < USBD_STATE_CONFIGURED) {
                printf("\n\rQuit MSD mode\n\r");
                msdReadPeak = 0;
                msdWritePeak = 0;
                usbConnected = 0;
                SDDeInitExec();
                sdConnected = 0;
            }
            else MSDDriver_StateMachine();
            // Update MSD status view
            if (updateView) {

                updateView = 0;
                if (msdReadTotal > msdReadPeak)
                    msdReadPeak = msdReadTotal;
                if (msdWriteTotal > msdWritePeak)
                    msdWritePeak = msdWriteTotal;

                printf("Read %5dK, Max %5dK; Write %5dK, Max %5dK; IO %5dK\r",
                       msdReadTotal / (UPDATE_DELAY/10),
                       msdReadPeak / (UPDATE_DELAY/10),
                       msdWriteTotal / (UPDATE_DELAY/10),
                       msdWritePeak / (UPDATE_DELAY/10),
                       (msdReadTotal+msdWriteTotal) / (UPDATE_DELAY/10));

                msdReadTotal = 0;
                msdWriteTotal = 0;
     
                if (sdInitExecDelay) {
                    if (0 == --sdInitExecDelay) {
                        sdConnected = SDConnectionUpdate(1);
                        if (!sdConnected) {
                            if (SD_INIT_RETRY <= sdInitErrorCnt++) {
                                printf("\n\r-MSD- SD inserted but init fail!\n\r");
                                sdConnected = 1;
                            }
                        }
                    }
                }
            }
        }
        else {
            // Check USB connection
            if (!usbConnected) {
                if (USBD_GetState() >= USBD_STATE_CONFIGURED ||
                    usbConnTick) {
                    if (usbConnTick == 0) {
                        usbConnTick = tick100u;
                    }
                    else if (GetDelayInTicks(usbConnTick,
                                             tick100u) >= USB_CONN_DELAY) {
                        SDDeInitExec();
                        usbConnected = 1; sdConnected = 0;
                        usbConnTick = 0;
                        printf("\n\r");
                    }
                    continue;
                }
            }

            if (CardIsConnected(0)) {
                if (sdConnected == 0) {
                    sdConnected = 1;

                    // Delay before card initialize
                    DelayMs(200);

                    // Do card test
                    CardInit(0);
                }
            }
            else if (sdConnected) {
                sdConnected = 0;
                printf("** Card Disconnected\n\r");
                //SD_Stop(&sdDrv, (SdDriver *)&mciDrv);
                SDDeInitExec();
            }

            if (sdConnected) {

                if (DBGU_IsRxReady()) {
                    switch(DBGU_GetChar()) {
                        case '0':  BlockTest(0, 0);             break;
                        case '1':  BlockTest(0, 1);             break;
                        case '2':  BlockTest(0, 2);             break;
                        case 'd':  BlockDump(0);                break;
                        case 'w':  WriteTest(0, 0, 0);          break;
                        case 'W':  WriteTest(0, 0, 1);          break;
                        // Initialize the card again
                        case 'I':  case 'i':
                            //SD_Stop(&sdDrv, (SdDriver *)&mciDrv);
                            SDDeInitExec();
                            CardInit(0);
                            break;

                        // Run test on whole disk
                        case 't':
                            DiskTest(0, 1, 1, 1);
                            printf("\n\r");
                            break;

                        // Run performence test
                        case 'T':
                            PerformanceTest(0, 1, 1, 0);
                            printf("\n\r");
                            break;

                        // Read/Verify ONLY test
                        case 'v':
                            DiskTest(0, 0, 0, 1);
                            printf("\n\r");
                            break;
                        case 'V':
                            PerformanceTest(0, 0, 1, 1);
                            printf("\n\r");
                            break;

                        // Change card speed
                        case 's':
                        {   unsigned int newSpeed =
                                speedList[speedNdx] * 1000000;
                            if (ChangeMciSpeed(0, newSpeed)) {
                                speedNdx = 0;
                            }
                            else {
                                speedNdx ++;
                                if (speedNdx > sizeof(speedList)) speedNdx = 0;
                            }
                            printf("-!- Change MCI CLOCK -> %d\n\r", sdSpeed);
                        }
                            break;

                        // Change performance test block size
                        case 'p':
                        {   if (performanceMultiBlock >= NB_MULTI_BLOCKS) {
                                performanceMultiBlock += NB_MULTI_STEP;
                                if (performanceMultiBlock >= NB_MULTI_MAX) {
                                    performanceMultiBlock = 1;
                                }
                            }
                            else
                                performanceMultiBlock <<= 1;
                            printf("-!- Performance Multi set to %d\n\r",
                                performanceMultiBlock);
                        }
                            break;
                        
                        // Auto HS mode enable/disable
                        case 'h':
                            if (gSdmmcAutoHsEnable) {
                                gSdmmcAutoHsEnable = 0;
                                printf("-!- Auto HS Disabled");
                            }
                            else {
                                gSdmmcAutoHsEnable = 1;
                                printf("-!- Auto HS Enabled");
                            }
                            printf(", Use 'i' to re-init card\n\r");
                            break;
                        // Test Value Setup
                        case 'f':
                        {   unsigned int input;
                            printf("-!- Input new test value: 0x");
                            if (GetHexInput(8, &input) == 0) {
                                printf("\n\r-!- Test Value 0x%x -> 0x%x\n\r",
                                    testFillValue, input);
                                testFillValue = input;
                            }
                        }
                            break;
                        // Show help information
                        default:    DumpMenu();
                    }
                }
            }
        }
    }

    return 0;
}

