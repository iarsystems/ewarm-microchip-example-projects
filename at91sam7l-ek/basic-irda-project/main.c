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
/// \dir "Basic IRDA Project with AT91SAM Microcontrollers"
///
/// !!!Purpose
///
/// This example demonstrates the IrDA (Infrared Data Association) on AT91
/// microcontrollers.
///
/// !!!Requirements
///
/// This example can be used on AT91SAM7L-EK.
///
/// !!!Description
///
/// The provided program uses the USART in IrDA mode for transmit and receive
/// one octet. The use of two boards are required for this test.
/// The program receive one octet, increment it, and send it on the IrDA chanel. 
///
/// !!!Usage
///
/// -# Build the program and download it inside the evaluation board. Please
///    refer to the <a href="http://www.atmel.com/dyn/resources/prod_documents/doc6224.pdf">SAM-BA User Guide</a>,
///    the <a href="http://www.atmel.com/dyn/resources/prod_documents/doc6310.pdf">GNU-Based Software Development</a>
///    application note or to the <a href="ftp://ftp.iar.se/WWWfiles/arm/Guides/EWARM_UserGuide.ENU.pdf">IAR EWARM User Guide</a>,
///    depending on your chosen solution.
/// -# download the program inside the second evaluation board.
/// -# Connect a serial cable to the DEBUG port on the evaluation kit. It will
///    most often be labeled "SERIAL DEBUG port".
/// -# On the computer, open and configure a terminal application (e.g.
///    HyperTerminal on Microsoft Windows) with these settings:
///       - 115200 bauds
///       - 8 data bits
///       - No parity
///       - 1 stop bit
///       - Hardware flow control (RTS/CTS)
/// -# Start the application by pressing the wake-up button on the two boards.
/// -# The following traces shall appear on the terminal:
///    \code
/// -- Basic IRDA Project xxx --
/// -- AT91xxxxxx-xx
/// -- Compiled: xxx xx xxxx xx:xx:xx --
/// -I- Received 0x01
/// -I- Data sent
/// -I- Received 0x03
/// -I- Data sent
/// -I- Received 0x05
/// -I- Data sent
/// -I- Received 0x07
/// -I- Data sent
/// -I- Received 0x09
/// -I- Data sent
/// -I- Received 0x0B
/// -I- Data sent
/// -I- Received 0x0D
/// -I- Data sent
/// -I- Received 0x0F
/// -I- Data sent
///    \endcode
///
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
//         Headers
//------------------------------------------------------------------------------

#include <stdio.h>
#include <board.h>
#include <pio/pio.h>
#include <usart/usart.h>
#include <utility/trace.h>
#include <pmc/pmc.h>

//------------------------------------------------------------------------------
//         Local variables
//------------------------------------------------------------------------------

/// List of pins to configure for the application
static const Pin pins[] = {PIN_USART0_RXD, PIN_USART0_TXD, PIN_IRDA_SD};

//------------------------------------------------------------------------------
//         Global functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//         Global functions
//------------------------------------------------------------------------------
//-----------------------------------------------------------------------------
/// Application entry point. 
/// Initializes the IrDA and starts the loopback
/// \return Unused (ANSI-C compatibility)
//-----------------------------------------------------------------------------
int main(void)
{
    unsigned char data = 0;

    // Configure DBGU
    TRACE_CONFIGURE(DBGU_STANDARD, 115200, BOARD_MCK);
    printf("-- Basic IRDA Project %s --\n\r", SOFTPACK_VERSION);
    printf("-- %s\n\r", BOARD_NAME);
    printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);

    // Configure USART0 in IRDA mode
    PMC_EnablePeripheral(AT91C_ID_US0);

    PIO_Configure(pins, PIO_LISTSIZE(pins));
    PIO_Clear(&(pins[2]));
    USART_Configure(AT91C_BASE_US0, USART_MODE_IRDA, 115200, BOARD_MCK);
    USART_SetIrdaFilter(AT91C_BASE_US0, 4);

    // Loopback
    while (1) {
        // Receive one byte of data
        // Disable Transmitter
        USART_SetTransmitterEnabled(AT91C_BASE_US0, 0);
        // Enable receiver
        USART_SetReceiverEnabled(AT91C_BASE_US0, 1);
        data = USART_Read(AT91C_BASE_US0, 0xFFFF);
        printf("-I- Received 0x%02X\n\r", data);

        // Send the byte back +1
        data++;
        // Disable Receiver
        USART_SetReceiverEnabled(AT91C_BASE_US0, 0);
        // Enable transmitter
        USART_SetTransmitterEnabled(AT91C_BASE_US0, 1);
        USART_Write(AT91C_BASE_US0, data, 0xFFFF);
        printf("-I- Data sent\n\r");
    }
    return 0;
}


