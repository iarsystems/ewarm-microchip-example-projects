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
 *  \page usart_synchronous  USART Synchronous Mode Example
 *
 *  \section Purpose
 *
 *  This example demonstrates the Synchronous  mode provided by the USART
 *  peripherals on SAM3X.
 *
 *  \section Requirements
 *
 *  This example can be used on sam3x evaluation kit and requires 3 fly wires 
 *  connected the following pins.
 *  - <b> Usart1   --   Usart3</b>
 *   - TXD(PA13)   --  RXD(PD5)
 *   -  RXD(PA12)  --  TXD(PD4)
 *   -  SCK(PA16)  --  SCK(PE16)
 *
 *  \section Description
 *
 *  This application gives an example of how to use USART in synchronous mode.
 *  Synchronous operations provide a high speed transfer capability. The
 *  transfer under this mode needs a pair of master and slave, which is
 *  determined by which one offers the clock source.
 *
 *  The example initialized USART1 as master by default. To enable the
 *  communication between each side of the connection,the user should change
 *  the mode of another side to slave through user interface. If well configured,
 *  transfer could be started by typing 'r' and  'w' from terminal application.
 *  This example also leaves the interface to select the clock frequency.
 *
 *  The meaning of each input character could be found in items of the main menu.
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
 *     -- USART Synchronous Mode Example  xxx --
 *     -- xxxxxx-xx
 *     -- Compiled: xxx xx xxxx xx:xx:xx --
 *     -- Menu Choices for this example --
 *     -- [0-3]:Select clock frequency of master --
 *     -- i: Display configuration info
 *     -- w: Write data block .--
 *     -- r: Read data block.--
 *     -- s: Switch between master and slave mode.--
 *     -- m: Display this menu again.--
 *     --USART1 in MASTER mode--
 *
 *     \endcode
 *  -# The main menu will guide the user to configure the device and conduct
 *     operations.
 *
 *  \section References
 *  - usart_synchronous/main.c
 *  - pio.h
 *  - usart.h
 */

/** \file
 *
 *  This file contains all the specific code for the usart_synchronous example.
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


/** size of the receive buffer used by the PDC, in bytes.*/
#define BUFFER_SIZE         2000


/** USART1 synchronous master*/
#define USART1_SYNC_MASTER    1
/** USART1 synchronous slave*/
#define USART1_SYNC_SLAVE    0

/** USART1 is reading*/
#define USART1_STATE_READ    0

/** USART1 is writing*/
#define USART1_STATE_WRITE   1

/** USART1 frequency options number */
#define FREQ_OPTIONS_NUM 4

/*----------------------------------------------------------------------------
 *         Local variables
 *----------------------------------------------------------------------------*/

/** Pins to configure for the application.*/
static const Pin pins[] = { PIN_USART1_RXD,
                     PIN_USART1_TXD,
                     PIN_USART1_SCK,
                     PIN_USART3_RXD,
                     PIN_USART3_TXD,
                     PIN_USART3_SCK
                     };

/** Transmit buffer. */
static char Buffer[]="DESCRIPTION of this example: \n\r \
**************************************************************************\n\r \
*  This application gives an example of how to use USART in synchronous mode.\n\r \
*  Synchronous operations provide a high speed transfer capability. The\n\r \
*  transfer under this mode needs a pair of master and slave, which is\n\r \
*  determined by which one offers the clock source.\n\r \
*  \n\r \
*  The example initialized USART1 as master and USART3 as slave by default. \n\r \
*  To enable the communication between each side of the connection. The user \n\r \
*  should change the mode of another side to slave through user interface. \n\r \
*  If well configured, transfer could be started by typing 't' from terminal.\n\r \
*  This example also leaves the interface to select the clock frequency.\n\r \
*  \n\r \
*  The meaning of each input character could be found in items of the main menu\n\r \
*  \n\r \
**************************************************************************\n\r \
END of DESCRIPTION \n\r \
";


/** buffer for receiving */
static char pRecvBufferUSART1[BUFFER_SIZE]= { 0 };

/** reception done*/
static volatile bool recvDone = false;
/** sending done*/
static volatile bool sentDone = false;

/** mode for usart1 and spi ,0 means usart1 as master 1 for
 another state*/
static uint8_t transfer_mode = USART1_SYNC_MASTER;

/** state of reading or writing*/
static uint8_t ucState = USART1_STATE_WRITE;

/** clock frequency*/
static uint32_t frequency[FREQ_OPTIONS_NUM]={100000UL,400000UL,1000000UL,1600000UL};

/** present freqency index in list frequency[]*/
static uint8_t freq_index = 0;
/*----------------------------------------------------------------------------
 *         Local functions
 *----------------------------------------------------------------------------*/

/* \brief USART1 IRQ handler
 *
 * Interrupt handler for USART. After reception is done,set recvDone to true,
 * and if transmission done, set sentDone to true.
 *
 *----------------------------------------------------------------------------*/
void USART1_IrqHandler(void)
{
    uint32_t dwStatus;

    /*  Read USART status */
    dwStatus = BOARD_USART1_BASE->US_CSR;
    /* Receive buffer is full */
    if ((dwStatus & US_CSR_ENDRX) == US_CSR_ENDRX && ucState == USART1_STATE_READ) {
        recvDone = true;
        USART_DisableIt(USART1, US_CSR_ENDRX);
    }
    if ((dwStatus & US_CSR_TXBUFE) == US_CSR_TXBUFE && ucState == USART1_STATE_WRITE){
        sentDone = true;
        USART_DisableIt(USART1, US_IDR_TXBUFE);
    }

}

/* \brief USART3 IRQ handler
 *
 * Interrupt handler for USART. After reception is done,set recvDone to true,
 * and if transmission done, set sentDone to true.
 *
 *----------------------------------------------------------------------------*/
void USART3_IrqHandler(void) 
{
    uint32_t dwStatus;

    /*  Read USART status */
    dwStatus = BOARD_USART3_BASE->US_CSR;
    /* Receive buffer is full */
    if ((dwStatus & US_CSR_ENDRX) == US_CSR_ENDRX && ucState == USART1_STATE_WRITE) {
        recvDone = true;
        USART_DisableIt(USART3, US_CSR_ENDRX);
    }
    if ((dwStatus & US_IER_ENDTX) == US_IER_ENDTX && ucState == USART1_STATE_READ){
        sentDone = true;
        USART_DisableIt(USART3, US_IER_ENDTX);
    }

}

/**
 * \brief Configures USART in synchronous mode,8N1
 * \param mode 1 for  master, 0 for slave
 */
static void _ConfigureUsart( uint8_t isUsart1Master, uint32_t freq )
{
    uint32_t dwModeSyncMaster = US_MR_USART_MODE_NORMAL | US_MR_USCLKS_MCK
                    | US_MR_CHMODE_NORMAL | US_MR_CLKO
                    | US_MR_SYNC | US_MR_MSBF
                    | US_MR_CHRL_8_BIT | US_MR_NBSTOP_1_BIT| US_MR_PAR_NO;

    uint32_t dwModeSyncSlave = US_MR_USART_MODE_NORMAL | US_MR_USCLKS_SCK
        | US_MR_CHMODE_NORMAL
        | US_MR_SYNC | US_MR_MSBF
        | US_MR_CHRL_8_BIT | US_MR_NBSTOP_1_BIT| US_MR_PAR_NO;

    /* Enable the peripheral clock in the PMC */
    PMC_EnablePeripheral( BOARD_ID_USART1 ) ;
    PMC_EnablePeripheral( BOARD_ID_USART3 ) ;

    /* Configure the USART in the desired mode @USART_SPI_CLK bauds*/
    if (isUsart1Master)
    {
        USART_Configure( BOARD_USART1_BASE, dwModeSyncMaster, freq, BOARD_MCK ) ;
        USART_Configure( BOARD_USART3_BASE, dwModeSyncSlave, freq, BOARD_MCK ) ;
    }
    else
    {
        USART_Configure( BOARD_USART1_BASE, dwModeSyncSlave, freq, BOARD_MCK ) ;
        USART_Configure( BOARD_USART3_BASE, dwModeSyncMaster, freq, BOARD_MCK ) ;
    }
    /* enable USART1 interrupt */
    NVIC_EnableIRQ( USART1_IRQn ) ;
    NVIC_EnableIRQ( USART3_IRQn ) ;

    /* Enable receiver & transmitter */
    USART_SetTransmitterEnabled( BOARD_USART1_BASE, 1 ) ;
    USART_SetReceiverEnabled( BOARD_USART1_BASE, 1 ) ;
    USART_SetTransmitterEnabled( BOARD_USART3_BASE, 1 ) ;
    USART_SetReceiverEnabled( BOARD_USART3_BASE, 1 ) ;
}
/**
 * \brief Display main menu.
 */
static void _DisplayMainmenu( void )
{
    printf("-- Menu Choices for this example --\n\r");
    printf("-- [0-3]:Select clock frequency of master --\n\r");
    printf("-- i: Display configuration info\n\r");
    printf("-- t: Start data transfer .--\n\r");
    printf("-- s: Switch between master and slave mode.--\n\r");
    printf("-- m: Display this menu again.--\n\r");
}

/**
 * \brief Dump buffer to uart
 *
 */
static void _DumpInfo( char *buf, uint32_t size )
{
    uint32_t i = 0 ;

    while ( (i < size) && (buf[i] != 0) )
    {
        printf( "%c", buf[i++] ) ;
    }
}

/*------------------------------------------------------------------------------
 *         Global functions
 *------------------------------------------------------------------------------*/


/**
 * \brief Application entry point.
 *
 * Configures USART1 in synchronous master/slave mode start a transmission
 * between two boards.
 * \return Unused.
 */
extern int main( void )
{
    char c;

    /* Disable watchdog */
    WDT_Disable( WDT ) ;

    /*  Configure pins */
    PIO_PinConfigure( pins, PIO_LISTSIZE( pins ) ) ;

    /* Example information log */
    printf( "-- USART Synchronous Mode Example %s --\n\r", SOFTPACK_VERSION ) ;
    printf( "-- %s\n\r", BOARD_NAME ) ;
    printf( "-- Compiled: %s %s --\n\r", __DATE__, __TIME__ ) ;

    /* display main menu*/
    _DisplayMainmenu() ;

    /* configure USART1 in  Master and SPI in slave mode*/
    _ConfigureUsart( USART1_SYNC_MASTER, frequency[freq_index] ) ;

    memset(pRecvBufferUSART1,0,BUFFER_SIZE);

    transfer_mode = USART1_SYNC_MASTER ;

    ucState = USART1_STATE_WRITE ;

    printf( "--USART1 in MASTER mode--\n\r" ) ;

    while ( 1 )
    {
        c = UART_GetChar() ;

        switch ( c )
        {
            case '0':
            case '1':
            case '2':
            case '3':
                freq_index = c - '0';
                printf("-- The clock frequency is: %u\n\r", (uint32_t)frequency[freq_index] ) ;
                _ConfigureUsart( USART1_SYNC_MASTER, frequency[freq_index] ) ;
            break ;

            case 'i':
            case 'I':
            if ( transfer_mode == USART1_SYNC_MASTER )
            {
                printf( "-- USART1 is MASTER at %u Hz.\n\r", (uint32_t)frequency[freq_index] ) ;
            }
            else
            {
                printf( "-- USART1 is SLAVE \n\r" ) ;
            }
            break;

            case 's':
            case 'S':
            if ( transfer_mode == USART1_SYNC_MASTER )
            {
                transfer_mode = USART1_SYNC_SLAVE ;
                _ConfigureUsart( USART1_SYNC_SLAVE, frequency[freq_index] ) ;
                printf( "--USART1 in SLAVE mode--\n\r" ) ;
            }
            else
            {
                if ( transfer_mode == USART1_SYNC_SLAVE )
                {
                    transfer_mode = USART1_SYNC_MASTER;
                    _ConfigureUsart(USART1_SYNC_MASTER,frequency[freq_index]);
                    printf("--USART1 in MASTER mode--\n\r");
                }
            }
            break;

            case 't':
            case 'T':
                if ( transfer_mode == USART1_SYNC_MASTER )
                {
                    ucState = USART1_STATE_WRITE;

                    USART_ReadBuffer(BOARD_USART3_BASE, pRecvBufferUSART1, BUFFER_SIZE);
                    
                    USART_WriteBuffer(BOARD_USART1_BASE, Buffer, BUFFER_SIZE);
                    USART_EnableIt(BOARD_USART1_BASE,US_IER_TXBUFE);
                    while(!sentDone);
                    if(sentDone)
                    {
                        printf(" --USART1 %s sent done\n\r", ucState? "MASTER":"SLAVE" );
                    }

                    printf("--USART3 Read--\n\r");

                    USART_EnableIt(BOARD_USART3_BASE,US_CSR_ENDRX);

                }
                else
                {
                    ucState = USART1_STATE_READ;

                    /* Setup read buffer before send */
                    USART_ReadBuffer(BOARD_USART1_BASE, pRecvBufferUSART1, BUFFER_SIZE);
                    
                    USART_WriteBuffer(BOARD_USART3_BASE, Buffer, BUFFER_SIZE);
                    USART_EnableIt(BOARD_USART3_BASE,US_IER_ENDTX);
                    
                    while(!sentDone);
                    
                    if(sentDone)
                    {
                        printf("--USART3 %s sent done\n\r", ucState? "MASTER":"SLAVE" );
                    }

                    printf("--USART1 Read--\n\r");

                    /* Enable usart1 receiver end interrupt */
                    USART_EnableIt(BOARD_USART1_BASE,US_CSR_ENDRX);
                }

                while(!recvDone);
                if(recvDone) 
                {
                  if(strncmp(pRecvBufferUSART1,Buffer,BUFFER_SIZE))
                  {
                      printf(" -F-: Failed!\n\r");
                  }else
                  {
                    /* successfully received*/
                    _DumpInfo(pRecvBufferUSART1,BUFFER_SIZE);
                  }
                  printf("--END of read--\n\r");
                  memset(pRecvBufferUSART1,0,sizeof(pRecvBufferUSART1));
                  recvDone = false;
                  sentDone = false;
                }
            break;


            case 'm':
            case 'M':
                _DisplayMainmenu() ;
            break;

        }

    }
}

