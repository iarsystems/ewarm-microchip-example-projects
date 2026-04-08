/**
 * \file
 *
 * \brief USART functions
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
#include "compiler.h"
#include "conf_usb.h"
#include "usb_protocol_cdc.h"

#include "usartd.h"
#include "main.h"

/*_____ D E F I N I T I O N S ______________________________________________*/

#define USART           USART0
#define USART_IRQn      USART0_IRQn
#define ID_USART        ID_USART0
#define PINS_USART      PIN_USART0_RXD, PIN_USART0_TXD,\
                        PIN_USART0_EN, \
                        /*PIN_USART0_RTS, PIN_USART0_CTS*/
#define usart_interrupt USART0_IrqHandler

/*_____ V A R I A B L E S __________________________________________________*/

/** Pins to configure for the application. */
static const Pin pinsUSART[] = {PINS_USART};

/*_____ L O C A L   F U N C T I O N S ______________________________________*/

/*_____ G L O B A L   F U N C T I O N S ____________________________________*/

ISR(usart_interrupt, AVR32_USART1_IRQ_GROUP, 3)
{
    uint32_t dwCsr = USART_GetStatus(USART);
    if (dwCsr & US_CSR_RXRDY) {
        // Data received
        int value;
        bool b_error = (dwCsr & (US_CSR_FRAME | US_CSR_TIMEOUT | US_CSR_PARE));
        value = USART->US_RHR;
        if (b_error) {
            USART->US_CR = US_CR_RSTRX;
        }
        main_usart_rx_occur(b_error, value);
        return;
    }

    if (dwCsr & US_CSR_TXRDY) {
        uint8_t value_tx;
        if (main_usart_tx_free(&value_tx)) {
            // Transmit next data
            USART->US_THR = value_tx;
        } else {
            // Fifo empty then Stop UART transmission
            USART_DisableIt(USART, US_IDR_TXRDY);
        }
    }
}


void usart_config(usb_cdc_line_coding_t * cfg)
{
    uint32_t dwMode = US_MR_USART_MODE_NORMAL
                        | US_MR_USCLKS_MCK
                        | US_MR_CHRL_8_BIT
                        | 0//US_MR_PAR_NO
                        | 0//US_MR_NBSTOP_1_BIT
                        | US_MR_CHMODE_NORMAL;
    uint32_t ier;

    switch (cfg->bCharFormat) {
    case CDC_STOP_BITS_2:
        dwMode |= US_MR_NBSTOP_2_BIT;
        break;
    case CDC_STOP_BITS_1_5:
        dwMode |= US_MR_NBSTOP_1_5_BIT;
        break;
    case CDC_STOP_BITS_1:
    default:
        // Default stop bit = 1 stop bit
        dwMode |= US_MR_NBSTOP_1_BIT;
        break;
    }

    switch (cfg->bParityType) {
    case CDC_PAR_EVEN:
        dwMode |= US_MR_PAR_EVEN;
        break;
    case CDC_PAR_ODD:
        dwMode |= US_MR_PAR_ODD;
        break;
    case CDC_PAR_MARK:
        dwMode |= US_MR_PAR_MARK;
        break;
    case CDC_PAR_SPACE:
        dwMode |= US_MR_PAR_SPACE;
        break;
    default:
    case CDC_PAR_NONE:
        dwMode |= US_MR_PAR_NO;
        break;
    }

    // Options for USART.
    ier = USART->US_IMR ;
    USART_Configure(USART, dwMode, cfg->dwDTERate, BOARD_MCK);
    // Restore both RX and TX
    USART->US_CR = US_CR_RXEN | US_CR_TXEN;
    USART->US_IER = ier;
}

void usart_open(void)
{
    uint32_t dwMode = US_MR_USART_MODE_NORMAL
                        | US_MR_USCLKS_MCK
                        | US_MR_CHRL_8_BIT
                        | US_MR_PAR_NO
                        | US_MR_NBSTOP_1_BIT
                        | US_MR_CHMODE_NORMAL;

    PIO_PinConfigure(pinsUSART, PIO_LISTSIZE(pinsUSART));
    PIO_PinClear( &pinsUSART[2] );
    PMC_EnablePeripheral( ID_USART );

    // Initialize it in RS232 mode.
    USART_Configure(USART, dwMode, 115200, BOARD_MCK);

    // Enable both RX and TX
    USART_SetTransmitterEnabled( USART, 1 );
    USART_SetReceiverEnabled( USART, 1 );
    NVIC_EnableIRQ( USART_IRQn );

    USART_EnableIt( USART, US_IER_RXRDY | US_IER_TXRDY );
}

void usart_close(void)
{
    // Disable interrupts
    USART_DisableIt( USART, 0xFFFFFFFF );
    // Close RS232 communication
    PIO_PinSet( &pinsUSART[2] );
}

void usart_enable_tx(void)
{
    // Enable UART TX interrupt to send values
    USART_EnableIt( USART, US_IER_TXRDY );
    USART_SetTransmitterEnabled( USART, 1 );
}
