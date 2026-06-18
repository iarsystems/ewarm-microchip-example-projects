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

/* Include AT91SAM3S-EK definitions and initialization API */
#include "board.h"
#include "gui/screens.h"
#include "demo_parameters.h"

#if defined _DEMO_USE_USB_
/* Atmel USB MSD include */
#include "usb/usbmsd.h"
#include "usb/MSDDriverDescriptors.h"
#endif // _DEMO_USE_USB_

/* Include USB definitions */
#include "MSDDriver.h"
#include "MSDLun.h"

/* Include Media definitions */
#include "nand/rtmedia.h"
#include "nand/ftldrv.h"

#include "board_dependencies.h"

#include <string.h>
#include <stdio.h>
#include <assert.h>


/** Size of one block in bytes. */
#define BLOCK_SIZE          512

/** Size of the MSD IO buffer in bytes (6K, more the better). */
#define MSD_BUFFER_SIZE     (8*BLOCK_SIZE)

/** Maximum number of LUNs which can be defined. */
#define MAX_LUNS            1
//#define MAX_LUNS            2

/** Available medias. */
Media medias[MAX_LUNS];

/** Device LUNs. NAND & µSD */
static MSDLun msdLUN[MAX_LUNS] ;

/** Mass storage device driver instance. */
static MSDDriver msdDriver ;

/** LUN read/write buffer. */
static uint8_t msdBuffer[MSD_BUFFER_SIZE] ;

/** Total data read/write by MSD */
static uint32_t msdReadTotal = 0 ;
static uint32_t msdWriteTotal = 0 ;
static uint16_t msdFullCnt = 0 ;
static uint16_t msdNullCnt = 0 ;

SAMGUI_TaskHandle hTskUSB ;

/**
 * Invoked after the USB driver has been initialized. By default, configures
 * the UDP/UDPHS interrupt.
 */
void USBDCallbacks_Initialized(void)
{
    NVIC_EnableIRQ(UDP_IRQn);
}

/**
 * Invoked when a new SETUP request is received from the host. Forwards the
 * request to the Mass Storage device driver handler function.
 * \param request  Pointer to a USBGenericRequest instance.
 */
//void USBDCallbacks_RequestReceived(const USBGenericRequest *request)
//{
//    MSDDriver_RequestHandler(request);

//    USBReq = *request;
//    dwCBReady = 1;
//}

/**
 * Invoked when the configuration of the device changes. Resets the mass
 * storage driver.
 * \param cfgnum New configuration number.
 */
void USBDDriverCallbacks_ConfigurationChanged(uint8_t cfgnum)
{
   MSDDriver_ConfigurationChangeHandler(cfgnum);
}
/*----------------------------------------------------------------------------
 *        VBus monitoring (optional)
 *----------------------------------------------------------------------------*/

/** VBus pin instance. */
static const Pin _gPinVbus = PIN_USB_VBUS ;

#if defined _DEMO_USE_USB_
/**
 * \brief VBUS detect IRQ Handler.
 */
static void _VBus_Handler( const Pin* pPin )
{
    /* Check current level on VBus */
    if ( PIO_Get( &_gPinVbus ) )
    {
        USBD_Connect() ;
    }
    else
    {
        USBD_Disconnect() ;
    }
}

/**
 * \brief Configures the VBus Pin
 *
 * To trigger an interrupt when the level on that pin changes.
 */
static void _VBus_Configure( void )
{
    /* Configure PIO */
    PIO_Configure( &_gPinVbus, 1 ) ;

    /* Configure PIO It */
    PIO_ConfigureIt( &_gPinVbus, _VBus_Handler ) ;

    /* Enable PIO interrupt */
    PIO_EnableIt( &_gPinVbus ) ;

    /* Check current level on VBus */
    if ( PIO_Get( &_gPinVbus ) )
    {
        /* if VBUS present, force the connect */
        USBD_Connect() ;
    }
    else
    {
        USBD_Disconnect() ;
    }
}

/**
 * Invoked when the MSD finish a READ/WRITE.
 * \param ucFlowDirection 1 - device to host (READ10)
 *                        0 - host to device (WRITE10)
 * \param dwDataLength    Length of data transferred in bytes.
 * \param dwFifoNullCount Times that FIFO is NULL to wait
 * \param dwFifoFullCount Times that FIFO is filled to wait
 */
void MSDCallbacks_Data( uint8_t ucFlowDirection, uint32_t dwDataLength,
                        uint32_t dwFifoNullCount, uint32_t dwFifoFullCount )
{
    if ( ucFlowDirection )
    {
        msdReadTotal += dwDataLength ;
    }
    else
    {
        msdWriteTotal += dwDataLength ;
    }

    msdFullCnt += dwFifoFullCount ;
    msdNullCnt += dwFifoNullCount ;
}

int UsbMsdInitialize( void )
{
    return 0 ;
}

/**
 * \brief USB Mass storage task.
 * This task periodically calls MEDNandFlash_Flush() to store temporary
 * objects in the Nandflash each FTL_FLUSH_RATE ms.
 */
extern void TaskUsbMsd( void* pParameter )
{
    /* Initialize LUN */
    RTLUN_Init( msdLUN, &(gRtNandMedias), msdBuffer, MSD_BUFFER_SIZE, 0, 0, 0, 0, MSDCallbacks_Data ) ;

    /* USB clock source has been initialized in Lowlevel Init */

    /* BOT driver initialization */
    MSDDriver_Initialize( &msdDriverDescriptors, msdLUN, 1 ) ;

    _VBus_Configure() ;

    for ( ; ; )
    {
        /* Invoke Mass storage state machine */
        MSDDriver_StateMachine( ) ;
        SAMGUI_TaskDelay( 1 ) ;
    }
}

extern void USB_Start( void )
{
    if ( SAMGUI_TaskCreate( TaskUsbMsd, "USB_MSD", NULL, TASK_USB_STACK_SIZE,
                            TASK_USB_STACK_PRIORITY, NULL, &hTskUSB ) != SAMGUI_E_OK )
    {
        printf( "Failed to create USB_MSD task\r\n" ) ;
    }
}
#endif // _DEMO_USE_USB_

