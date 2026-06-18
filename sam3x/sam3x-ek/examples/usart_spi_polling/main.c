/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support
 * ----------------------------------------------------------------------------
 * Copyright (c) 2011, Atmel Corporation
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

/**
 *  \page usart_spi_polling USART SPI Polling Example
 *
 *  \section Purpose
 *
 *  This example demonstrates how to use the SPI mode without IRQ provided by the USART peripherals on
 *  SAM3X.
 *
 *  \section Requirements
 *
 *  This example can be used on sam3x evaluation kit, the usart0 and an AT45 board
 *  should be connected as following matching table:
 *  - <b> USART0  -- AT45 </b>
 *   - SCK0(PA17) -- SCK
 *   - TXD0(PA11) -- MOSI
 *   - RXD0(PA10) -- MISO
 *   - RTS0(PB25) -- NPCS
 *   - +3.3V      -- Vcc
 *   - GND        -- GND
 *
 *  \section Description
 *
 * This example demonstrates how to use USART in SPI mode without IRQ. The USART is
 * configured as SPI master. This example gives you an AT45 Dataflash 
 * programming code so that can help develop your own SPI devices 
 * applications with maximum efficiency.
 *
 *
 * \section Usage
 *
 *  -# Build the program and download it inside the evaluation board. Please
 *     refer to the <a href="http://www.atmel.com/dyn/resources/prod_documents/doc6224.pdf">SAM-BA User Guide</a>,
 *     the <a href="http://www.atmel.com/dyn/resources/prod_documents/doc6310.pdf">GNU-Based Software Development</a>
 *     application note or to the <a href="ftp://ftp.iar.se/WWWfiles/arm/Guides/EWARM_UserGuide.ENU.pdf">IAR EWARM User Guide</a>,
 *     depending on your chosen solution.
 *  -# Connect a serial cable to the UART port on the evaluation kit.
 *  -# On the computer, open and configure a terminal application (e.g.
 *     HyperTerminal on Microsoft Windows) with these settings:
 *        - 115200 bauds
 *        - 8 data bits
 *        - No parity
 *        - 1 stop bit
 *        - No flow control
 *  -# Start the application. The following traces shall appear on the terminal:
 *     \code
 *     -- USART SPI Polling Example xxx --
 *     -- xxxxxx-xx
 *     -- Compiled: xxx xx xxxx xx:xx:xx --
 *    -I- Initializing the SPI and AT45 drivers
 *    -I- At45 enabled
 *    -I- Waiting for a dataflash to be connected ... 
 *     \endcode
 *
 *  \section References
 *  - usart_spi_polling/main.c
 *  - usart_spi.c
 *  - pio.h
 *  - usart.h
 */

/** \file
 *
 *  This file contains all the specific code for the usart_spi_polling example.
 *
 */
/*----------------------------------------------------------------------------
 *         Headers
 *----------------------------------------------------------------------------*/

#include "board.h"

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

/*----------------------------------------------------------------------------
 *         Local definition
 *----------------------------------------------------------------------------*/
/** clock of USART0 in spi mode*/
#define USART_SPI_CLK       8000000UL

/*----------------------------------------------------------------------------
 *         Local variables
 *----------------------------------------------------------------------------*/

/** Pins to configure for the application.*/
static const Pin pins[] = { PIN_USART0_RXD,  //MISO
                     PIN_USART0_TXD,  //MOSI
                     PIN_USART0_SCK,  //SCK
                     PIN_USART0_RTS,  //CS
                     PIN_USART0_EN
                     };

/** Page buffer.*/
static uint8_t pBuffer[2112];

/** Usart device */
static Usartd UsartDev;

/** AT45 driver instance.*/
static At45 at45;

/*----------------------------------------------------------------------------
 *         Local functions
 *----------------------------------------------------------------------------*/
/**
 * \brief Configure usart device parameters 
 */
static void _InitUsart( Usartd* usartd)
{
    /* Configure usart device logical parameters */
    usartd->pUsartHw = USART0;
    usartd->dwUsartID = ID_USART0; 
    usartd->eUsartIRQn = USART0_IRQn;

    usartd->ucSemaphore = 1;
    usartd->eTransferType = USART_FIFO_TRANS_TYPE;
    usartd->pCurrentCommand = 0;
}

/**
 * \brief Configures USART in spi mode  with respect to state
 * \param state the master or slave of USART
 */
static void _ConfigureUsart( Usartd* usartd)
{
    uint32_t mode = US_MR_USART_MODE_SPI_MASTER | US_MR_USCLKS_MCK | US_MR_CHRL_8_BIT
                    | US_MR_PAR_NO | US_MR_CHMODE_NORMAL | US_MR_CLKO 
                    | US_SPI_BPMODE_0 ;

    /* Enable the peripheral clock in the PMC */
    PMC_EnablePeripheral( usartd->dwUsartID ) ;

    /* Configure the USART in the desired mode @USART_SPI_CLK bauds*/
    USART_Configure( usartd->pUsartHw, mode, USART_SPI_CLK, BOARD_MCK ) ;
    
    /* Enable receiver & transmitter */
    USART_SetTransmitterEnabled( usartd->pUsartHw, 1 ) ;
    USART_SetReceiverEnabled( usartd->pUsartHw, 1 ) ;
}

/**
 *  \brief Handler for USART0 interrupt.
 *
 */
void USART0_IrqHandler( void )
{
    uint32_t status;
    status = USART_GetStatus( USART0 ) ;

    if ((status & US_CSR_RXBUFF) == US_CSR_RXBUFF)
    {
       UsartDev.ucUsartTransEndSig = 1;
       USART_DisableIt(USART0, US_CSR_RXBUFF);
    }

}

/*------------------------------------------------------------------------------
 *         Global functions
 *------------------------------------------------------------------------------*/
/**
 * \brief Application entry point.
 *
 * Configures USART0 in spi mastermode and start a transmission to the dataflash.
 * \return Unused.
 */
extern int main( void )
{
    uint32_t dw;
    uint32_t dwPage ;
    uint8_t ucTestFailed ;
    const At45Desc *pDesc ;

    /* Disable watchdog */
    WDT_Disable( WDT ) ;

    /*  Configure pins */
    PIO_PinConfigure( pins, PIO_LISTSIZE( pins ) ) ;

    /* Example information log */
    printf( "-- USART SPI Polling Example %s --\n\r", SOFTPACK_VERSION ) ;
    printf( "-- %s\n\r", BOARD_NAME ) ;
    printf( "-- Compiled: %s %s --\n\r", __DATE__, __TIME__ ) ;

    _InitUsart(&UsartDev);

     /* configure USART0 in  Master mode*/
    _ConfigureUsart( &UsartDev ) ;

    memset(&at45, 0xff, sizeof(At45));

    at45.dwInterfaceType = AT45_INTERFACE_TYPE_USART_SPI;

    AT45_Configure( &at45, &UsartDev,0,0) ;

    /* Identify the At45 device*/
    printf( "-I- Waiting for a dataflash to be connected ...\n\r" ) ;
    pDesc = 0 ;
    while ( !pDesc )
    {
        pDesc = AT45_FindDevice( &at45, AT45D_GetStatus( &at45 ) ) ;
    }
    printf( "-I- %s detected\n\r", at45.pDesc->pName ) ;

    /* Test all pages. */
    ucTestFailed = 0 ;
    dwPage = 0 ;

    while ( !ucTestFailed && (dwPage < AT45_PageNumber( &at45 )) )
    {
        printf( "-I- Test in progress on page: %6u\r", (uint32_t)dwPage ) ;

        /* Erase dwPage */
        AT45D_Erase( &at45, dwPage * AT45_PageSize( &at45 ) ) ;

        /* Verify that dwPage has been erased correctly */
        memset( pBuffer, 0, AT45_PageSize( &at45 ) ) ;
        AT45D_Read( &at45, pBuffer, AT45_PageSize( &at45 ), dwPage * AT45_PageSize( &at45 ) ) ;

        for ( dw=0 ; dw < AT45_PageSize( &at45 ) ; dw++ )
        {
            if ( pBuffer[dw] != 0xff )
            {
                printf( "-E- Could not erase page %u\n\r", (uint32_t)dwPage ) ;
                ucTestFailed = 1 ;
                break;
            }
        }

        /* Write dwPage */
        for ( dw=0; dw < AT45_PageSize( &at45 ) ; dw++ )
        {
            pBuffer[dw] = dw & 0xFF ;
        }
        AT45D_Write( &at45, pBuffer, AT45_PageSize( &at45 ), dwPage * AT45_PageSize( &at45 ) ) ;

        /* Check that data has been written correctly.*/
        memset( pBuffer, 0, AT45_PageSize( &at45 ) ) ;
        AT45D_Read( &at45, pBuffer, AT45_PageSize( &at45 ), dwPage * AT45_PageSize( &at45 ) ) ;

        for ( dw=0; dw < AT45_PageSize( &at45 ) ; dw++ )
        {
            if ( pBuffer[dw] != (dw & 0xFF) )
            {
                printf( "-E- Could not write dwPage %u\n\r", (uint32_t)dwPage ) ;
                ucTestFailed = 1 ;
                break ;
            }
        }

        dwPage++ ;
    }

    /* Display test result */
    if ( ucTestFailed )
    {
        printf( "\n\r-E- Test failed.\n\r" ) ;
    }
    else
    {
         printf( "\n\r-I- Test passed.\n\r" ) ;
    }

    return 0 ;

    
}

