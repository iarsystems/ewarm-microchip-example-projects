/**
 * \file
 *
 * \brief Main functions for USB composite example
 *
 * Copyright (C) 2009 Atmel Corporation. All rights reserved.
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 * Atmel AVR product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 */

/*_____  I N C L U D E S ___________________________________________________*/

#include "board.h"
#include "memories.h"

#include "compiler.h"
#include "conf_usb.h"
#include "udd.h"
#include "udc.h"
#include "udi_msc.h"
#include "udi_hid.h"
#include "ui.h"
#include "conf_access.h"
#include "media_mem.h"

/*_____ D E F I N I T I O N S ______________________________________________*/

/** Maximum number of Medias which can be defined. */
#define MAX_MEDS            3

#define DRV_RAMDISK         0   /**< Media ID for RAM Disk */
#define DRV_SDMMC           1   /**< Media ID for SD/MMC card */
#define DRV_NAND            2   /**< Media ID for Nand Flash */

#define DRV_REMOVED         0   /**< No media present */
#define DRV_INSERTED        1   /**< Media inserted */
#define DRV_READY           2   /**< Media ready */
#define DRV_HALT            3   /**< Halt, no action, wait removing */

/** Interrupt proiority (0 is highest, 0~15) */
#define HSMCI_PRI       3
#define DMAC_PRI        5
#define UOTGHS_PRI      5

/** RamDisk address base (in bytes) */
#define RAMDISK_BASE        (EBI_SDRAMC_ADDR)
/** RamDisk size (in bytes) */
#define RAMDISK_SIZE        (10*1024*1024)

/** MSC buffer address base (in bytes) */
#define MSC_BUFFER_BASE     (RAMDISK_BASE+RAMDISK_SIZE)

/** Size of the reserved Nand Flash (4M) */
#define NF_RESERVE_SIZE     (4*1024*1024)

/** Size of the managed Nand Flash (128M) */
#define NF_MANAGED_SIZE     (128*1024*1024)

#define STRING_EOL    "\r"
#define STRING_HEADER "-- USB Device MSC HID Example "SOFTPACK_VERSION" --\r\n" \
                      "-- "BOARD_NAME" --\r\n" \
                      "-- Compiled: "__DATE__" "__TIME__" --"STRING_EOL

/*_____ G L O B A L   V A R I A B L E S ____________________________________*/

/** Available medias. */
Media medias[MAX_MEDS];

/*_____ L O C A L   V A R I A B L E S ______________________________________*/

/** PIOs for USB OTGHS */
static const Pin pUOTGHSPins[] = {PIN_UOTGHS_VBOF, PIN_UOTGHS_ID, PIN_UOTGHS_FAULT};
static const Pin pUOTGHS_Fault = PIN_UOTGHS_FAULT;

/** Pins used to access to nandflash. */
static const Pin pPinsNf[] = {PINS_NANDFLASH};
/** Nandflash device structure. */
static struct TranslatedNandFlash translatedNf;
/** Address for transferring command bytes to the nandflash. */
static uint32_t cmdBytesAddr = BOARD_NF_COMMAND_ADDR;
/** Address for transferring address bytes to the nandflash. */
static uint32_t addrBytesAddr = BOARD_NF_ADDRESS_ADDR;
/** Address for transferring data bytes to the nandflash. */
static uint32_t dataBytesAddr = BOARD_NF_DATA_ADDR;
/** Nandflash chip enable pin. */
static const Pin nfCePin = BOARD_NF_CE_PIN;
/** Nandflash ready/busy pin. */
static const Pin nfRbPin = BOARD_NF_RB_PIN;

/** DMA driver instance */
static sDmad dmaDrv;

/** MCI driver instance. */
static sMcid mciDrv[BOARD_NUM_MCI];

/** SDCard driver instance. */
static sSdCard sdDrv[BOARD_NUM_MCI];

/** SDCard inserted status */
static U8 sdStatus[BOARD_NUM_MCI] = {DRV_REMOVED};

/** SD card pins instance. */
static const Pin pinsSd[] = {BOARD_SD_PINS};

/** SD card detection pin instance. */
static const Pin pinsSdCd[] = {BOARD_SD_PIN_CD};

/** Initialize retry */
static U8 retryInit = 0;

/** Time tick (SOF) */
static U32 sofTick = 0;

/** Nand flash ready status */
static bool main_b_nf_ready = false;

//! Mouse Enabled status
static bool main_b_mouse_enable = false;

//! Mass Storage Enabled status
static bool main_b_msc_enable = false;

/*_____ G L O B A L   H A N D O L E R S ____________________________________*/

/**
 * DMA interrupt handler.
 */
void DMAC_IrqHandler(void)
{
    DMAD_Handler(&dmaDrv);
}

/**
 * MCI interrupt handler. Forwards the event to the MCI driver handlers.
 */
void HSMCI_IrqHandler(void)
{
    uint32_t i;
    for (i = 0; i < BOARD_NUM_MCI; i ++)
    {
        MCID_Handler(&mciDrv[i]);
    }
}

/*_____ L O C A L   F U N C T I O N S _______________________________________*/

/**
 *  \brief Wait DEBUG Key Input
 *  \param nbloop   Wait time in number of loops.
 *  \param echo     Wether echo the key.
 *  \return key or 0 for nothing.
 */
static uint8_t WaitKey(volatile uint32_t nbloop, bool echo)
{
    uint8_t key;
    do {

        if (UART_IsRxReady()) {

            key = UART_GetChar();
            if (echo) UART_PutChar(key);
            return key;
        }

        if (nbloop) {

            nbloop --;
            if (0 == nbloop)
                break;
        }
    } while (1);

    return 0;
}

/**
 * Configure OTG settings for USB device
 */
static void _ConfigureUOtghs(void)
{
    /* UTMI parallel mode, High/Full/Low Speed */
    /* UOTGCK not used in this configuration (High Speed) */
    PMC->PMC_SCDR = PMC_SCDR_UOTGCK;
    /* USB clock register: USB Clock Input is UTMI PLL */
    PMC->PMC_USB = PMC_USB_USBS;
    /* Enable peripheral clock for UOTGHS */
    PMC_EnablePeripheral(ID_UOTGHS);
    UOTGHS->UOTGHS_CTRL = UOTGHS_CTRL_UIMOD_Device;
    /* Enable PLL 480 MHz */
    PMC->CKGR_UCKR = CKGR_UCKR_UPLLEN | CKGR_UCKR_UPLLCOUNT(0xF);
    /* Wait that PLL is considered locked by the PMC */
    while( !(PMC->PMC_SR & PMC_SR_LOCKU) );

    /* UOTGHS pins */
    PIO_PinConfigure( pUOTGHSPins, PIO_LISTSIZE( pUOTGHSPins ) );
    if ( PIO_PinGet(&pUOTGHS_Fault) == 0 )
    {
        puts( "UOTGHS_Fault = 0 (active low  ERROR FLAG !"STRING_EOL );
        puts( "Undervoltage, Soft Start, Overcurrent, or Overtemperature"STRING_EOL );
        while(1);
    }

    /* IRQ */
    NVIC_EnableIRQ(UOTGHS_IRQn) ;
}

/**
 *  \brief Initialize Nand Flash
 *  \return true if initialized succesfully.
 */
static uint8_t NandFlashInitialize(void)
{
    uint8_t  nfRc;
    uint16_t nfBaseBlock = 0;
    struct RawNandFlash *pRaw = (struct RawNandFlash*)&translatedNf;
    struct NandFlashModel *pModel = (struct NandFlashModel*)&translatedNf;
    uint32_t nfManagedSize;

    /* Configure for NandFlash */
    BOARD_ConfigureNandFlash(SMC);
    /* Configure PIO for Nand Flash */
    PIO_PinConfigure(pPinsNf, PIO_LISTSIZE(pPinsNf));

    /* Nand Flash Initialize (ALL flash mapped) */
    nfRc = RawNandFlash_Initialize(pRaw,
                                   0,
                                   cmdBytesAddr,
                                   addrBytesAddr,
                                   dataBytesAddr,
                                   nfCePin,
                                   nfRbPin);
    if ( nfRc )
    {
        printf("Nand not found\n\r");
        return false;
    }
    else
    {
        printf("NF\tNb Blocks %d\n\r", NandFlashModel_GetDeviceSizeInBlocks(pModel));
        printf("\tBlock Size %dK\n\r", (int)(NandFlashModel_GetBlockSizeInBytes(pModel)/1024));
        printf("\tPage Size %d\n\r", NandFlashModel_GetPageDataSize(pModel));
        nfBaseBlock = NF_RESERVE_SIZE / NandFlashModel_GetBlockSizeInBytes(pModel);
    }
    printf("NF disk will use area from %dM(B%d)\n\r", NF_RESERVE_SIZE/1024/1024, nfBaseBlock);

    /* Wait 1.2s for input */
    printf("!! Erase the NF Disk? (y/n):");

    if ( WaitKey( BOARD_MCK / 25 , 1 ) == 'y' )
    {
        if ( nfRc == 0 )
        {
            uint32_t block;
            printf(" Erase from %d ... ", nfBaseBlock ) ;
            for ( block = nfBaseBlock ; block < NandFlashModel_GetDeviceSizeInBlocks(pModel); block ++ )
            {
                RawNandFlash_EraseBlock(pRaw, block);
            }
            printf("OK");
        }
    }
    printf("\n\r");

    nfManagedSize = ((NandFlashModel_GetDeviceSizeInMBytes(pModel) - NF_RESERVE_SIZE/1024/1024) > NF_MANAGED_SIZE/1024/1024) ? \
                        NF_MANAGED_SIZE/1024/1024 : (NandFlashModel_GetDeviceSizeInMBytes(pModel) - NF_RESERVE_SIZE/1024/1024);
    if (TranslatedNandFlash_Initialize(&translatedNf,
                                       0,
                                       cmdBytesAddr,
                                       addrBytesAddr,
                                       dataBytesAddr,
                                       nfCePin,
                                       nfRbPin,
                                       nfBaseBlock, nfManagedSize * 1024 * 1024/NandFlashModel_GetBlockSizeInBytes(pModel))) {
        printf("Nand init error\n\r");
        return false;
    }
    /* Media initialize */
    MEDNandFlash_Initialize(&medias[DRV_NAND], &translatedNf);

    return true;
}

/**
 * Configure memories
 */
static void _ConfigureMemories(void)
{
    int i;

    /* Configure media access buffer */
    media_access_init_default();
    //media_access_init_customize((void*)MSC_BUFFER_BASE, 2, 16);

    /* RAM disk space */
    BOARD_ConfigureSdram();

    /* Configure SDRAM Disk */
    MEDRamDisk_Initialize(&medias[DRV_RAMDISK],
                          MEDIA_SECTOR_SIZE,
                          RAMDISK_BASE / MEDIA_SECTOR_SIZE,
                          RAMDISK_SIZE / MEDIA_SECTOR_SIZE);
    /* Configure SD/MMC */
  #if 1
    /* Initialize the HSMCI driver */
    MCID_Init(&mciDrv[0], HSMCI, ID_HSMCI, BOARD_MCK, &dmaDrv, 0 ) ;
    NVIC_EnableIRQ( HSMCI_IRQn );   
    /* Initialize SD driver */
    for (i = 0; i < BOARD_NUM_MCI; i ++)
    {
        SDD_InitializeSdmmcMode(&sdDrv[i], &mciDrv[i], 0);
    }
    /* Initialize SD bus pins */
    PIO_PinConfigure(pinsSd, PIO_LISTSIZE(pinsSd));
    /* Initialize SD card detect */
    PIO_PinConfigure(pinsSdCd, PIO_LISTSIZE(pinsSdCd));
  #else
    MEDRamDisk_Initialize(&medias[DRV_SDMMC],
                          MEDIA_SECTOR_SIZE,
                          RAMDISK_BASE / MEDIA_SECTOR_SIZE,
                          RAMDISK_SIZE / MEDIA_SECTOR_SIZE);
    medias[DRV_SDMMC].mappedRD = 0;
    medias[DRV_SDMMC].mappedWR = 0;
  #endif
    /* Configure NandFlash */
  #if 1
    if ( 0 == NandFlashConfigureDmaChannels( &dmaDrv ) )
    {
        /* Init NandFlash Disk */
        if (NandFlashInitialize()) {
            MEDNandFlash_Initialize(&medias[DRV_NAND], &translatedNf);
            main_b_nf_ready = true;
        }
        else {
            printf("-E- NF initialize error\n\r");
        }
    }
    else
    {
        printf ("-E- Initialize DMA for NF failed!\n\r");
    }
  #else
    main_b_nf_ready = true;
    MEDRamDisk_Initialize(&medias[DRV_NAND],
                          MEDIA_SECTOR_SIZE,
                          RAMDISK_BASE / MEDIA_SECTOR_SIZE,
                          RAMDISK_SIZE / MEDIA_SECTOR_SIZE);
    medias[DRV_NAND].mappedRD = 0;
    medias[DRV_NAND].mappedWR = 0;
  #endif
}

/*! \brief Main function. Execution starts here.
 */
int main(void)
{
    /* Set 4 WS for Embedded Flash Access for 84 MHz */
    EFC0->EEFC_FMR = EEFC_FMR_FWS( 4 );
    EFC1->EEFC_FMR = EEFC_FMR_FWS( 4 );

    /* Disable watchdog */
    WDT_Disable( WDT ) ;

    /* Output example information */
    puts( STRING_HEADER ) ;

    /* Interrupt priority */
    NVIC_SetPriority( DMAC_IRQn, DMAC_PRI );
    NVIC_SetPriority( HSMCI_IRQn, HSMCI_PRI );
    NVIC_SetPriority( UOTGHS_IRQn, UOTGHS_PRI );

    /* Initialize the DMA driver */
    DMAD_Initialize(&dmaDrv, 0);
    NVIC_EnableIRQ( DMAC_IRQn );

    /* Configure memories */
    _ConfigureMemories();

    /* Configure OTGHS settings */
    _ConfigureUOtghs();

    ui_init();
    ui_powerdown();

    // Start USB stack to authorize VBus monitoring
    udc_start();

    if (!udc_include_vbus_monitoring()) {
        // VBUS monitoring is not available on this product
        // thereby VBUS has to be considered as present
        main_vbus_action(true);
    }

    // The main loop manages only the power mode
    // because the USB management is done by interrupt
    while (true) {
      sSdCard *pSd = &sdDrv[0];

      if (main_b_msc_enable) {
        udi_msc_process_trans();
        /* Flush NF every 250ms when there is no data R/W */
        if ((sofTick % 250) == 0) {
            if (ms_current_io < 10) MED_Flush(&medias[DRV_NAND]);
        }
      }
      if (0 == PIO_PinGet(&pinsSdCd[0])) {
        if (DRV_REMOVED == sdStatus[0]) {
            sdStatus[0] = DRV_INSERTED;
            retryInit = 3; /* retry 3 times */
        }
        if (DRV_INSERTED == sdStatus[0]) {
            /* Wait some time */
            WaitKey(BOARD_MCK/1000, false);
            if (SDMMC_SUCCESS == SD_Init(pSd)) {
                if (SD_GetCardType(pSd) & CARD_TYPE_bmSD) {
                    printf("\n\rSD Initialized\n\r");
                    //MEDSdcard_Initialize(&medias[DRV_SDMMC], pSd);
                    MEDSdusb_Initialize(&medias[DRV_SDMMC], pSd);
                    sdStatus[0] = DRV_READY;
                }
                if (SD_GetCardType(pSd) == CARD_SDIO) {
                    printf("\n\rSD IO detected\n\r");
                    sdStatus[0] = DRV_HALT;
                }
            }
            else {
                retryInit --;
                if (retryInit == 0) {
                    printf("\n\rSD Initialize fail\n\r");
                    sdStatus[0] = DRV_HALT;
                }
            }
        }
      }
      else if ( DRV_REMOVED != sdStatus[0] ) {
        if (DRV_READY == sdStatus[0]) {
            SD_DeInit(pSd);
            printf("\n\rSD rejected\n\r");
        }
        sdStatus[0] = DRV_REMOVED;
      }
    }
}

void main_vbus_action(bool b_high)
{
    if (b_high) {
        // Attach USB Device
        udc_attach();
    } else {
        // VBUS not present
        udc_detach();
    }
}

void main_suspend_action(void)
{
    ui_powerdown();
}

void main_resume_action(void)
{
    ui_wakeup();
}

void main_sof_action(void)
{
    sofTick ++;
	if ((!main_b_mouse_enable) || (!main_b_msc_enable))
		return;
	ui_process(udd_get_frame_number());
}

void main_remotewakeup_enable(void)
{
	ui_wakeup_enable();
}

void main_remotewakeup_disable(void)
{
	ui_wakeup_disable();
}

bool main_mouse_enable(void)
{
	main_b_mouse_enable = true;
	return true;
}

void main_mouse_disable(void)
{
	main_b_mouse_enable = false;
}

bool main_media_check(U8 media)
{
    switch(media)
    {
        case DRV_NAND:      return main_b_nf_ready;
        case DRV_SDMMC:     return (sdStatus[0] == DRV_READY);
        case DRV_RAMDISK:   return true;
        default:            return false;
    }
}

bool main_is_media_protected(U8 media)
{
    switch(media)
    {
        case DRV_RAMDISK:   return false;
        case DRV_SDMMC:     return false;
        case DRV_NAND:      return false;
        default:            return true;
    }
}

bool main_msc_enable(void)
{
    main_b_msc_enable = true;
    return true;
}

void main_msc_disable(void)
{
    main_b_msc_enable = false;
}

/**
 * \page usb_msc_hid USB Device Composite MSC HID Example
 *
 * \section Purpose
 *
 * This example shows how to implement a USB Device Composite with HID mouse and 
 * Mass Storage interfaces on ARM products with USB module.
 *
 * \section Requirements
 *
 * This package can be used with SAM3X evaluation kits.
 *
 * \section Description
 *
 * The example uses all memories available on the board and connects these to
 * USB Device Mass Storage stack. 
 * Also, the example uses the buttons or sensors available on the board 
 * to simulate a standard mouse.
 * After loading firmware, connect hardware board (SAM3X-EK) to the USB Host.
 * When connected to a USB host system this application allows to display 
 * all available memories as a removable disks and provides a mouse in
 * the Unix/Mac/Windows operating systems.
 * \note
 * This example uses the native MSC and HID drivers on Unix/Mac/Windows OS, except for Win98.
 *
 * \copydoc usb_msc_hid_ui 
 *
 * \section Usage
 *
 * -# Build the program and download it inside the evaluation board. Please
 *    refer to the
 *    <a href="http://www.atmel.com/dyn/resources/prod_documents/doc6224.pdf">
 *    SAM-BA User Guide</a>, the
 *    <a href="http://www.atmel.com/dyn/resources/prod_documents/doc6310.pdf">
 *    GNU-Based Software Development</a>
 *    application note or to the
 *    <a href="ftp://ftp.iar.se/WWWfiles/arm/Guides/EWARM_UserGuide.ENU.pdf">
 *    IAR EWARM User Guide</a>,
 *    depending on your chosen solution.
 * -# On the computer, open and configure a terminal application
 *    (e.g. HyperTerminal on Microsoft Windows) with these settings:
 *   - 115200 bauds
 *   - 8 bits of data
 *   - No parity
 *   - 1 stop bit
 *   - No flow control
 * -# Start the application.
 * -# In the terminal window, the following text should appear:
 *     \code
 *     -- USB Device MSC HID Example xxx --
 *     -- xxxxxx-xx
 *     -- Compiled: xxx xx xxxx xx:xx:xx --
 *     \endcode
 * -# When connecting USB cable to windows, the host
 *    reports a new USB %device attachment, Disk installation and HID device
 *    installation.
 *    Then new "USB Composite Device", "USB Mass Storage Device",
 *    "Generic volume" and "USB Human Interface Device"
 *    appear in hardware %device list.
 * -# You can find the new disk on host, and to create/write file to it.
 * -# You can uses push buttons on EK to simulate mouse move.
 *
 * \section example About example
 *
 * The example uses the following module groups:
 * - Basic modules:
 *   Startup, board, clock, interrupt, power management
 * - USB Device stack and HID & MSC modules:
 *   <br>libraries/usb_device/
 *   <br>libraries/usb_device/udc/
 *   <br>libraries/usb_device/class/msc/
 *   <br>libraries/usb_device/class/hid/
 *   <br>libraries/usb_device/class/hid/mouse/
 * - Specific implementation:
 *    - main.c,
 *      <br>initializes clock
 *      <br>initializes interrupt
 *      <br>initializes memories modules
 *      <br>\subpage power_management
 *      <br>manages UI
 *    - udi_composite_desc.c,udi_composite_conf.h,
 *      <br>USB Device composite definition
 *    - specific implementation:
 *       - conf_foo.h   configuration of each module
 *       - ui.c        implement of user's interface (buttons, leds)
 *
 * <SUP>1</SUP> The memory data transfers are done outside USB interrupt routine. 
 * This is done in the MSC process ("udi_msc_process_trans()") called by main loop.
 *
 * \section References
 * - usb_msc_hid/main.c
 * - pio.h
 * - pio_it.h
 * - led.h
 */
