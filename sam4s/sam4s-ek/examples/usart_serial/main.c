/* ----------------------------------------------------------------------------
 *         SAM Software Package License
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
 * \page usart_serial USART Serial Example
 *
 * \section Purpose
 * This example demonstrates the normal (serial) mode provided by the USART
 * peripherals.
 *
 * \section Requirements
 *  This package can be used with SAM4S-EK.
 *
 * \section Description
 *
 * On start up, the debug information is dumpped to on-board UART port.
 * A terminal application, such as hyperterminal, is used to monitor these
 * debug information. Open another hyperterminal to connect with
 * on-board USART port. Then the program works in ECHO mode, so USART will
 * sends back anything it receives from the hyperterminal.  You can send a text
 * file from the hyperterminal connected with USART port to the device (without
 * any protocol such as X-modem).
 *
 * \section Usage
 *
 * -# Build the program and download it inside the evaluation board. Please
 *    refer to the
 *    <a href="http://www.atmel.com/dyn/resources/prod_documents/6421B.pdf">
 *    SAM-BA User Guide</a>, the
 *    <a href="http://www.atmel.com/dyn/resources/prod_documents/doc6310.pdf">
 *    GNU-Based Software Development</a>
 *    application note or to the
 *    <a href="http://www.iar.com/website1/1.0.1.0/78/1/">
 *    IAR EWARM User and reference guides</a>,
 *    depending on your chosen solution.
 * -# On the computer, open and configure a terminal application
 *    (e.g. HyperTerminal on Microsoft Windows) with these settings:
 *   - 115200 bauds
 *   - 8 bits of data
 *   - No parity
 *   - 1 stop bit
 *   - No flow control
 * -# In the terminal window, the
 *    following text should appear (values depend on the board and chip used):
 *    \code
 *     -- Basic USART Serial Project xxx --
 *     -- xxxxxx-xx
 *     -- Compiled: xxx xx xxxx xx:xx:xx --
 *     -- Start to echo serial inputs --
 *    \endcode
 * -# Send a file in text format from the hyperterminal connected with USART port to
 *    the device. On HyperTerminal, this is done by selecting "Transfer -> Send Text File"
 *    (this does not prevent you from sending binary files). The transfer will start and the
 *    you could read the file in the hyperterminal.
 *
 * \section References
 * - usart_serial/main.c
 * - usart.h
 */

/**
 * \file
 * This file contains all the specific code for the usart_serial
 */

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

#include "board.h"

#include <stdint.h>
#include <stdio.h>

/*----------------------------------------------------------------------------
 *        Local definitions
 *----------------------------------------------------------------------------*/

/** Size of the receive buffer used by the PDC, in bytes. */
#define BUFFER_SIZE         150

/** USART PDC transfer type definiton */
#define PDC_TRANSFER        1

/** USART FIFO transfer type definiton */
#define BYTE_TRANSFER       0

/*----------------------------------------------------------------------------
 *        Local variables
 *----------------------------------------------------------------------------*/

/** Pins to configure for the application. */
static const Pin pins[] = {
    PIN_USART1_RXD,
    PIN_USART1_TXD,
    PIN_USART1_EN
};

/** Receive buffer. */
static uint8_t pBuffer[2][BUFFER_SIZE];

/** Current bytes in buffer */
static uint32_t dwSizeBuffer = BUFFER_SIZE;

/** Current read buffer. */
static uint8_t rBuffer = 0;

/** Current write buffer. */
static uint8_t wBuffer = 0;

/** Byte read buffer */
static uint16_t uwReadBuffer = 0;

/** Current transfer mode */
static uint8_t ucTransMode = PDC_TRANSFER;

/*----------------------------------------------------------------------------
 *        Local functions
 *----------------------------------------------------------------------------*/

/**
 * \brief Interrupt handler for USART. Echo the bytes received and starts next
 * receive.
 */
void USART1_IrqHandler(void)
{
    uint32_t dwStatus;

    /* Read USART Status */
    dwStatus = USART1->US_CSR;

    /* Receive buffer is full */
    if ((dwStatus & US_CSR_ENDRX) == US_CSR_ENDRX
        && USART1->US_RNCR == 0 && ( ucTransMode == PDC_TRANSFER ))
    {
        /* Disable timer */
        TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKDIS;

        /* Clear ENDRX interrupt */
        USART1->US_RNCR = 0;

        /* Echo back buffer */
        USART_WriteBuffer(USART1,
                          pBuffer[wBuffer],
                          dwSizeBuffer);
        wBuffer = 1 - wBuffer;
        dwSizeBuffer = BUFFER_SIZE;

        /* Restart timer */
        TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
    }

    /* Buffer has been sent */
    if ((dwStatus & US_CSR_ENDTX) == US_CSR_ENDTX && USART1->US_TNCR == 0)
    {
        if (ucTransMode == BYTE_TRANSFER)
        {
            /* Clear ENDTX */
            USART1->US_TNCR = 0;
            USART1->US_IDR = US_IDR_ENDTX;
        }
        else
        {
            /* Clear ENDTX */
            USART1->US_TNCR = 0;

            /* Restart read on buffer */
            USART_ReadBuffer(USART1,
                            pBuffer[rBuffer],
                            BUFFER_SIZE);
            rBuffer = 1 - rBuffer;

            if (rBuffer == wBuffer)
            {
                USART1->US_IDR = US_IDR_ENDTX;
            }
        }
    }

    /* Transfer without PDC */
    if ((dwStatus & US_CSR_RXRDY) == US_CSR_RXRDY && ( ucTransMode == BYTE_TRANSFER ))
    {
        uwReadBuffer = USART1->US_RHR;

        /* Wait untill byte write ready */
        while ((USART1->US_CSR & US_CSR_TXEMPTY) == 0);

        USART1->US_THR = uwReadBuffer;
    }
}

/**
 * \brief Interrupt handler for TC0. Record the number of bytes received,
 * then restarts a read transfer on the USART if the transfer was stopped.
 */
void TC0_IrqHandler(void)
{
    uint32_t dwStatus;
    uint32_t dwBytesTotal = 0;

    /* Read TC0 Status */
    dwStatus = TC0->TC_CHANNEL[0].TC_SR;

    /* RC compare */
    if (((dwStatus & TC_SR_CPCS) == TC_SR_CPCS) && ( ucTransMode == PDC_TRANSFER ))
    {
        /* Flush PDC buffer */
        dwBytesTotal = BUFFER_SIZE - USART1->US_RCR;
        if (dwBytesTotal == 0) {

            /* Return when no bytes received */
            return;
        }

        /* Log current size */
        dwSizeBuffer = dwBytesTotal;

        /* Trigger USART ENDRX */
        USART1->US_RCR = 0;
    }
}

/**
 * \brief Configures USART in normal (serial rs232) mode, asynchronous, 8 bits, 1 stop
 * bit, no parity, 115200 bauds and enables its transmitter and receiver.
 */
static void _ConfigureUsart(void)
{
    uint32_t dwMode = US_MR_USART_MODE_NORMAL
                        | US_MR_USCLKS_MCK
                        | US_MR_CHRL_8_BIT
                        | US_MR_PAR_NO
                        | US_MR_NBSTOP_1_BIT
                        | US_MR_CHMODE_NORMAL;

    /* Enable the peripheral clock in the PMC */
    PMC_EnablePeripheral(ID_USART1);

    /* Configure the USART in the desired mode @115200 bauds */
    USART_Configure(USART1, dwMode, 115200, BOARD_MCK);

    /* Enable receiver & transmitter */
    USART_SetTransmitterEnabled(USART1, 1);
    USART_SetReceiverEnabled(USART1, 1);

    /* Configure the RXBUFF interrupt */
    NVIC_EnableIRQ( USART1_IRQn );
}

/**
 * \brief Configures Timer Counter 0 (TC0) to generate an interrupt every 20ms.
 * This interrupt will be used to flush USART input and echo back.
 */
static void _ConfigureTc(void)
{
    uint32_t div;
    uint32_t tcclks;

    /* Enable peripheral clock. */
    PMC_EnablePeripheral(ID_TC0);

    /* Configure TC for a 50Hz frequency and trigger on RC compare. */
    TC_FindMckDivisor( 50, BOARD_MCK, &div, &tcclks, BOARD_MCK );
    TC_Configure( TC0, 0, tcclks | TC_CMR_CPCTRG );
    TC0->TC_CHANNEL[0].TC_RC = ( BOARD_MCK / div ) / 50;

    /* Configure and enable interrupt on RC compare */
    NVIC_EnableIRQ( (IRQn_Type)ID_TC0 );
    TC0->TC_CHANNEL[0].TC_IER = TC_IER_CPCS;
}

/**
 * \brief Display main menu.
 */
static void _DisplayMainmenu( void )
{
    printf("-- Menu Choices for this example --\n\r");
    printf("-- s: Switch mode for USART1 between PDC and without PDC.--\n\r");
    printf("-- m: Display this menu again.--\n\r");
}

/**
 * \brief Reset the usart to default status
 */
static void _UsartReset( void )
{
    /* Reset and disable receiver & transmitter*/
    USART1->US_CR = US_CR_RSTRX | US_CR_RSTTX
                   | US_CR_RXDIS | US_CR_TXDIS;

    /* Clear PDC counter */
    USART1->US_RCR = 0;
    USART1->US_RNCR = 0;
    USART1->US_TCR = 0;
    USART1->US_TNCR = 0;

    /* Enable receiver & transmitter */
    USART_SetTransmitterEnabled(USART1, 1);
    USART_SetReceiverEnabled(USART1, 1);
}

/*----------------------------------------------------------------------------
 *        Exported functions
 *----------------------------------------------------------------------------*/

/**
 * \brief Application entry point for usart_serial example.
 *
 * \return Unused (ANSI-C compatibility).
 * \callgraph
 */
int main( void )
{
    uint8_t ucChar;
    /* Disable watchdog */
    WDT_Disable( WDT );

    /* Output example information */
    printf("-- USART Serial Example %s --\n\r", SOFTPACK_VERSION );
    printf("-- %s\n\r", BOARD_NAME);
    printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);
    printf("-- Start to echo serial inputs --\n\r");

    /* Configure pins */
    PIO_Configure(pins, PIO_LISTSIZE(pins));

    /* Configure USART */
    _ConfigureUsart();

    /* Configure TC */
    _ConfigureTc();

    /* Start receiving data and start timer */
    rBuffer = wBuffer = 0;
    USART_ReadBuffer(USART1, pBuffer[0], BUFFER_SIZE);
    USART_ReadBuffer(USART1, pBuffer[1], BUFFER_SIZE);

    printf("-I- Default Transfer with PDC \n\r");
    printf("-I- Press 's' to switch transfer mode \n\r");
    ucTransMode = PDC_TRANSFER;
    USART_DisableIt( USART1, US_IER_RXRDY );
    USART_EnableIt( USART1, US_IER_ENDTX | US_IER_ENDRX);

    TC_Start(TC0, 0);

    /* Infinite loop */
    while (1)
    {
        ucChar = UART_GetChar() ;

        switch ( ucChar )
        {
            case 's':
            case 'S':
            if ( ucTransMode == PDC_TRANSFER )
            {
                /* Disable PDC controller */
                USART1->US_PTCR = US_PTCR_RXTDIS | US_PTCR_TXTDIS;
                USART_DisableIt( USART1, US_IDR_ENDTX | US_IDR_ENDRX);

                /* Clean Usart controller */
                _UsartReset();

                USART_EnableIt(USART1, US_IER_RXRDY);
                ucTransMode = BYTE_TRANSFER ;

                printf("-I- Transfer without PDC \n\r");

            }
            else if ( ucTransMode == BYTE_TRANSFER )
            {
                /* Clean Usart controller */
                _UsartReset();

                /* reset pdc current buffer size */
                dwSizeBuffer = 0;

                /* Start receiving data and start timer */
                rBuffer = wBuffer = 0;
                USART_ReadBuffer(USART1, pBuffer[0], BUFFER_SIZE);
                USART_ReadBuffer(USART1, pBuffer[1], BUFFER_SIZE);

                USART_DisableIt( USART1, US_IER_RXRDY );
                USART_EnableIt( USART1, US_IER_ENDTX | US_IER_ENDRX);

                ucTransMode = PDC_TRANSFER ;
                printf("-I- Transfer with PDC \n\r");
            }
            break;

            case 'm':
            case 'M':
                _DisplayMainmenu() ;
            break;

            default :
               break;
        }

    }
}

