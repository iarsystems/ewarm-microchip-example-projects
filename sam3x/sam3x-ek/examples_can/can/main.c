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
 *  \page can "CAN example"
 *
 *  \section Purpose
 *
 *  This example demostrate basic functions of CAN controller.
 *
 *  \section Requirements
 *
 *  This package can be used with SAM3X evaluation kits. CAN0 and CAN1
 *  should be hooked-up externally before running the example.
 *
 *  \section Description
 *
 *  In this example, it sends and receives messages over CAN bus to manage
 *  the LED on the board and display CAN message on the terminal window.
 *  There are four basic tests:
 *  - test1: Test CAN0 Mailbox 0 transmitting to CAN1 Mailbox 0.
 *  - test2: Test CAN0 Mailboxes 1 & 2 transmitting to CAN1 Mailbox 7 without overwrite.
 *  - test3: Test CAN0 Mailboxes 1 & 2 transmitting to CAN1 Mailbox 7 with overwrite.
 *  - test4: Test CAN0 Mailbox 3 asking for CAN1 Mailbox 3 transmission.
 *
 *  \section Usage
 *
 *  -# Build the program and download it inside the evaluation board. Please
 *     refer to the
 *     <a href="http://www.atmel.com/dyn/resources/prod_documents/doc6224.pdf">
 *     SAM-BA User Guide</a>, the
 *     <a href="http://www.atmel.com/dyn/resources/prod_documents/doc6310.pdf">
 *     GNU-Based Software Development</a>
 *     application note or to the
 *     <a href="ftp://ftp.iar.se/WWWfiles/arm/Guides/EWARM_UserGuide.ENU.pdf">
 *     IAR EWARM User Guide</a>,
 *     depending on your chosen solution.
 *  -# On the computer, open and configure a terminal application
 *     (e.g. HyperTerminal on Microsoft Windows) with these settings:
 *    - 115200 bauds
 *    - 8 bits of data
 *    - No parity
 *    - 1 stop bit
 *    - No flow control
 *  -# Connect CAN0 (J17) and CAN1 (J20) CAN in loop.
 *  -# Start the application.
 *  -# Upon startup, the application will output the following lines on the terminal window.
 *      \code
 *      -- CAN Example 1.0 --
 *      -- Compiled: xxx xx xxxx xx:xx:xx --
 *      Configure LED PIOs.
 *      Configure buttons with debouncing.
 *      Press LEFT CLICK to increase button counter.
 *      CAN initialization complete.
 *      Press any key to start test
 *      \endcode
 *  -# (optional) Press LEFT CLICK several times to increase a push botton counter.
 *  -# Press a key in the terminal window to run the tests. CAN messages will
 *     displayed on the terminal window and Green LED or Blue LED will toggle
 *     according to the messages.
 */
/**
 * \file
 *
 * This file contains all the specific code for the can example.
 */

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

#include "board.h"

/*----------------------------------------------------------------------------
 *        Definitions
 *----------------------------------------------------------------------------*/

#define STRING_EOL    "\r"

#define STRING_HEADER "-- CAN Example "SOFTPACK_VERSION" --\r\n" \
                      "-- "BOARD_NAME" --\r\n" \
                      "-- Compiled: "__DATE__" "__TIME__" --"STRING_EOL

#define CAN_MSG_TOGGLE_LED_GREEN    0x11223344
#define CAN_MSG_TOGGLE_LED_BLUE     0xAABBCCDD


/*----------------------------------------------------------------------------
 *        Variables
 *----------------------------------------------------------------------------*/

/* CAN0 Transceiver */
SSN65HVD234_Data gCanTransceiver0 ;
/* CAN1 Transceiver */
SSN65HVD234_Data gCanTransceiver1 ;

/* CAN0 Transfer */
SCanTransfer gCanTransfer0 ;
/* CAN1 Transfer */
SCanTransfer gCanTransfer1 ;

/* Definition for CAN Pins */
const Pin gPinsCan[] = { PINS_CAN0, PINS_CAN1 } ;

/* Counter for push button press */
uint32_t gButtonPressCnt = 0;

/* Push button #1 pin instance. */
const Pin pinPB1 = PIN_PB_LEFT_CLICK ;

/*----------------------------------------------------------------------------
 *        Local functions
 *----------------------------------------------------------------------------*/

/**
 *  \brief Handler for Button 1 interrupt.
 *
 *  To increase button press times.
 */
static void _Button1_Handler( const Pin* pin )
{
    pin = pin; /* stop warning */
    gButtonPressCnt++ ;
}

/**
 *  \brief Configure the Pushbuttons
 *
 *  Configure the PIO as inputs and generate corresponding interrupt when pressed.
 */
static void _ConfigureButtons( void )
{
    /* Configure pios as inputs. */
    PIO_PinConfigure( &pinPB1, 1 ) ;

    /* Adjust pio debounce filter patameters, uses 10 Hz filter. */
    PIO_PinSetDebounceFilter( &pinPB1, 10 ) ;

    /* Initialize pios interrupt handlers, see PIO definition in board.h. */
    PIO_PinConfigureIt( &pinPB1, _Button1_Handler ) ;

    /* Enable PIO controller IRQs. */
    NVIC_EnableIRQ( (IRQn_Type)pinPB1.id ) ;

    /* Enable PIO line interrupts. */
    PIO_PinEnableIt( &pinPB1 ) ;
}

/**
 *  \brief Configure LEDs
 *
 *  Configures LEDs \#1 and \#2 (cleared by default).
 */
static void _ConfigureLeds( void )
{
    LED_Configure( LED_GREEN ) ;
    LED_Configure( LED_BLUE ) ;
}

/**
 * \brief Decode CAN messages.
 *
 *  \param pTransfer can transfer structure
 */
static void _DecodeCanMsg( SCanTransfer* pTransfer )
{
    uint32_t ledCtrl = pTransfer->data_low_reg;
    uint32_t cntButtonPress = pTransfer->data_high_reg;;

    puts( "CAN message:"STRING_EOL ) ;
    if (ledCtrl  == CAN_MSG_TOGGLE_LED_GREEN)
    {
        puts( "  Toggle Green LED"STRING_EOL ) ;
        LED_Toggle( LED_GREEN ) ;
    }
    else if (ledCtrl  == CAN_MSG_TOGGLE_LED_BLUE)
    {
        puts( "  Toggle Blue LED"STRING_EOL ) ;
        LED_Toggle( LED_BLUE ) ;
    }
    printf( "  Button press count: %d\n\r", (int)cntButtonPress);
}

/**
 *  \brief Test CAN0 Mailbox 0 transmitting to CAN1 Mailbox 0.
 */
static void _Test1( void )
{
    CAN_ResetAllMailbox( CAN0, &gCanTransfer0 ) ;
    CAN_ResetAllMailbox( CAN1, &gCanTransfer1 ) ;

    puts( "\n\rTest1: CAN0 Mailbox 0 transmitting to CAN1 Mailbox 0"STRING_EOL ) ;

    /* Init CAN1 Mailbox 0, receive */
    CAN_TransferReset( CAN1, &gCanTransfer1 ) ;
    gCanTransfer1.mailbox_number = 0 ;
    gCanTransfer1.mode_reg = CAN_MMR_MOT_MB_RX ;
    gCanTransfer1.acceptance_mask_reg = CAN_MAM_MIDvA_Msk | CAN_MAM_MIDvB_Msk ;
    gCanTransfer1.identifier = CAN_MAM_MIDvA( 0x07 ) ;  /* ID 7 */
    gCanTransfer1.data_low_reg =  0x00000000 ;
    gCanTransfer1.data_high_reg =  0x00000000 ;
    gCanTransfer1.control_reg = 0x00000000 ;
    CAN_InitMailboxRegisters( CAN1, &gCanTransfer1 ) ;

    /* Init CAN0 Mailbox 0, transmit */
    CAN_TransferReset( CAN0, &gCanTransfer0 ) ;
    gCanTransfer0.mailbox_number = 0 ;
    gCanTransfer0.mode_reg = CAN_MMR_MOT_MB_TX | CAN_MMR_PRIOR(15) ;
    gCanTransfer0.acceptance_mask_reg = 0x00000000;
    gCanTransfer0.identifier = CAN_MAM_MIDvA( 0x07 ) ;  /* ID 7 */
    gCanTransfer0.data_low_reg = CAN_MSG_TOGGLE_LED_GREEN ;
    gCanTransfer0.data_high_reg = gButtonPressCnt ;
    gCanTransfer0.control_reg = CAN_MCR_MDLC( 0x8 ) ;
    CAN_InitMailboxRegisters( CAN0, &gCanTransfer0 ) ;

    while ( CAN_Read( CAN1, &gCanTransfer1 ) != CAN_STATUS_SUCCESS ) ;
    while ( CAN_Write( CAN0, &gCanTransfer0 ) != CAN_STATUS_SUCCESS ) ;

    /* Wait answer */
    while ( CAN_IsInIdle( CAN1, &gCanTransfer1 ) ) ;
    while ( CAN_IsInIdle( CAN0, &gCanTransfer0 ) ) ;

    if ( (gCanTransfer1.data_low_reg == CAN_MSG_TOGGLE_LED_GREEN) && (gCanTransfer1.size == 8) )
    {
        puts( "Test1 passed"STRING_EOL ) ;
        _DecodeCanMsg( &gCanTransfer1 ) ;
    }
    else
    {
        puts( "Test1 ERROR"STRING_EOL ) ;
    }
}

/**
 *  \brief Test CAN0 Mailboxes 1 & 2 transmitting to CAN1 Mailbox 7 without overwrite.
 */
static void _Test2( void )
{
    CAN_ResetAllMailbox( CAN0, &gCanTransfer0 ) ;
    CAN_ResetAllMailbox( CAN1, &gCanTransfer1 ) ;

    puts( "\n\rTest2: CAN0 Mailboxes 1 & 2 transmitting to CAN1 Mailbox 7 without overwrite"STRING_EOL ) ;

    /* Init CAN1 Mailbox 7, reception */
    CAN_TransferReset( CAN1, &gCanTransfer1 ) ;
    gCanTransfer1.mailbox_number = 7 ;
    gCanTransfer1.mode_reg = CAN_MMR_MOT_MB_RX ;
    gCanTransfer1.acceptance_mask_reg = 0 ;
    gCanTransfer1.identifier = 0x0 ;
    gCanTransfer1.data_low_reg = 0x00000000 ;
    gCanTransfer1.data_high_reg = 0x00000000 ;
    gCanTransfer1.control_reg = 0x00000000 ;
    CAN_InitMailboxRegisters( CAN1, &gCanTransfer1 ) ;

    /* Init CAN0 Mailbox 1, transmit. This message will be discarded. */
    CAN_TransferReset( CAN0, &gCanTransfer0 ) ;
    gCanTransfer0.mailbox_number = 1;
    gCanTransfer0.mode_reg = CAN_MMR_MOT_MB_TX | CAN_MMR_PRIOR(15);
    gCanTransfer0.acceptance_mask_reg = 0x00000000;
    gCanTransfer0.identifier = CAN_MID_MIDvA(0x9);  /* ID 9 */
    gCanTransfer0.data_low_reg = CAN_MSG_TOGGLE_LED_GREEN ;
    gCanTransfer0.data_high_reg = gButtonPressCnt;
    gCanTransfer0.control_reg = CAN_MCR_MDLC( 8 ) ; /* Mailbox Data Length Code */
    CAN_InitMailboxRegisters( CAN0, &gCanTransfer0 ) ;

    /* Init CAN0 Mailbox 2, transmit. This message will be received first. */
    gCanTransfer0.mailbox_number = 2;
    gCanTransfer0.mode_reg = CAN_MMR_MOT_MB_TX | CAN_MMR_PRIOR(14);
    gCanTransfer0.acceptance_mask_reg = 0x00000000;
    gCanTransfer0.identifier = CAN_MID_MIDvA(0xA);  /* ID 10 */
    gCanTransfer0.data_low_reg = CAN_MSG_TOGGLE_LED_BLUE ;
    gCanTransfer0.data_high_reg = gButtonPressCnt;
    gCanTransfer0.control_reg = CAN_MCR_MDLC( 8 ) ; /* Mailbox Data Length Code */
    CAN_InitMailboxRegisters( CAN0, &gCanTransfer0 ) ;

    while ( CAN_STATUS_SUCCESS !=  CAN_Read( CAN1, &gCanTransfer1 ) ) ;
    while ( CAN_STATUS_SUCCESS !=  CAN_Write( CAN0, &gCanTransfer0 ) ) ;

    /* Wait answer */
    while ( CAN_IsInIdle( CAN0, &gCanTransfer0 ) ) ;
    while ( CAN_IsInIdle( CAN1, &gCanTransfer1 ) ) ;

    if ( (gCanTransfer1.data_low_reg == CAN_MSG_TOGGLE_LED_BLUE) && (gCanTransfer1.size == 8) )
    {
        puts( "Test2 passed"STRING_EOL ) ;
        _DecodeCanMsg( &gCanTransfer1 ) ;
    }
    else
    {
        puts( "Test2 ERROR"STRING_EOL ) ;
    }
}

/**
 *  \brief Test CAN0 Mailboxes 1 & 2 transmitting to CAN1 Mailbox 7 with overwrite.
 */
static void _Test3( void )
{
    CAN_ResetAllMailbox( CAN0, &gCanTransfer0 ) ;
    CAN_ResetAllMailbox( CAN1, &gCanTransfer1 ) ;

    puts( "\n\rTest3: CAN0 Mailboxes 1 & 2 transmitting to CAN1 Mailbox 7 with overwrite."STRING_EOL ) ;

    /* Init CAN1 Mailbox 7, reception with overwrite */
    CAN_TransferReset( CAN1, &gCanTransfer1 ) ;
    gCanTransfer1.mailbox_number = 7;
    gCanTransfer1.mode_reg = CAN_MMR_MOT_MB_RX_OVERWRITE;
    gCanTransfer1.acceptance_mask_reg = 0;
    gCanTransfer1.identifier = 0x0;
    gCanTransfer1.data_low_reg = 0x00000000;
    gCanTransfer1.data_high_reg = 0x00000000;
    gCanTransfer1.control_reg = 0x00000000;
    CAN_InitMailboxRegisters( CAN1, &gCanTransfer1 ) ;

    /* Init CAN0 Mailbox 1, transmit. This message will overwrite old one. */
    CAN_TransferReset( CAN0, &gCanTransfer0 ) ;
    gCanTransfer0.mailbox_number = 1;
    gCanTransfer0.mode_reg = CAN_MMR_MOT_MB_TX | CAN_MMR_PRIOR(15);
    gCanTransfer0.acceptance_mask_reg = 0x00000000;
    gCanTransfer0.identifier = CAN_MID_MIDvA(0x9);  /* ID 9 */
    gCanTransfer0.data_low_reg = CAN_MSG_TOGGLE_LED_GREEN;
    gCanTransfer0.data_high_reg = gButtonPressCnt;
    gCanTransfer0.control_reg = CAN_MCR_MDLC(8);    /* Mailbox Data Length Code */
    CAN_InitMailboxRegisters( CAN0, &gCanTransfer0 ) ;

    /* Init CAN0 Mailbox 2, transmit. This message will be received first. */
    gCanTransfer0.mailbox_number = 2;
    gCanTransfer0.mode_reg = CAN_MMR_MOT_MB_TX | CAN_MMR_PRIOR(14);
    gCanTransfer0.acceptance_mask_reg = 0x00000000;
    gCanTransfer0.identifier = CAN_MID_MIDvA(0xA);  /* ID 10 */
    gCanTransfer0.data_low_reg = CAN_MSG_TOGGLE_LED_BLUE ;
    gCanTransfer0.data_high_reg = gButtonPressCnt;
    gCanTransfer0.control_reg = CAN_MCR_MDLC(8);    /* Mailbox Data Length Code */
    CAN_InitMailboxRegisters( CAN0, &gCanTransfer0 ) ;

    while( CAN_STATUS_SUCCESS !=  CAN_Write( CAN0, &gCanTransfer0 ) ) ;
    puts( "Wait, wait, wait, wait"STRING_EOL);
    while( CAN_STATUS_SUCCESS !=  CAN_Read( CAN1, &gCanTransfer1 ) ) ;

    /* Wait answer */
    while( CAN_IsInIdle( CAN0, &gCanTransfer0 ) ) ;
    while( CAN_IsInIdle( CAN1, &gCanTransfer1 ) ) ;

    if ( (gCanTransfer1.data_low_reg == CAN_MSG_TOGGLE_LED_GREEN) && (gCanTransfer1.size == 8) )
    {
        puts( "Test3 passed"STRING_EOL ) ;
        _DecodeCanMsg( &gCanTransfer1 ) ;
    }
    else
    {
        puts( "Test3 ERROR"STRING_EOL ) ;
    }
}

/**
 *  \brief Test CAN0 Mailbox 3 asking for CAN1 Mailbox 3 transmission.
 */
static void _Test4( void )
{
    CAN_ResetAllMailbox( CAN0, &gCanTransfer0 ) ;
    CAN_ResetAllMailbox( CAN1, &gCanTransfer1 ) ;

    puts( "\n\rTest4: CAN0 Mailbox 3 asking for CAN1 Mailbox 3 transmission"STRING_EOL ) ;

    /* Init CAN1 Mailbox 7, reception with overwrite */
    CAN_TransferReset( CAN1, &gCanTransfer1 ) ;
    gCanTransfer1.mailbox_number = 3;
    gCanTransfer1.mode_reg = CAN_MMR_MOT_MB_PRODUCER | CAN_MMR_PRIOR(15);
    gCanTransfer1.acceptance_mask_reg = 0;
    gCanTransfer1.identifier = CAN_MID_MIDvA(0x0B);  /* ID 11 */
    gCanTransfer1.data_low_reg = CAN_MSG_TOGGLE_LED_GREEN;
    gCanTransfer1.data_high_reg = gButtonPressCnt;
    gCanTransfer1.control_reg = CAN_MCR_MDLC(8);
    CAN_InitMailboxRegisters( CAN1, &gCanTransfer1 ) ;
    /* Clear data for check (the data is already written to CAN_MDL and CAN_MDH) */
    gCanTransfer1.data_low_reg = 0x00000000;
    gCanTransfer1.data_high_reg = 0x00000000;

    /* Init CAN0 Mailbox 3, consumer mailbox. It sends a remote frame and waits for an answer. */
    /* Init CAN0 Mailbox 1, transmit */
    CAN_TransferReset( CAN0, &gCanTransfer0 ) ;
    gCanTransfer0.mailbox_number = 3;
    gCanTransfer0.mode_reg = CAN_MMR_MOT_MB_CONSUMER | CAN_MMR_PRIOR(15);
    gCanTransfer0.acceptance_mask_reg = CAN_MID_MIDvA_Msk | CAN_MID_MIDvB_Msk;
    gCanTransfer0.identifier = CAN_MID_MIDvA(0x0B);   /* ID 11 */
    gCanTransfer0.data_low_reg = 0x00000000;
    gCanTransfer0.data_high_reg = 0x00000000;
    gCanTransfer0.control_reg = 0x00000000;
    CAN_InitMailboxRegisters( CAN0, &gCanTransfer0 ) ;

    while( CAN_STATUS_SUCCESS !=  CAN_Write( CAN1, &gCanTransfer1 ) ) ;
    while( CAN_STATUS_SUCCESS !=  CAN_Write( CAN0, &gCanTransfer0 ) ) ;

    /* Wait answer */
    while( CAN_IsInIdle( CAN0, &gCanTransfer0) ) ;
    while( CAN_IsInIdle( CAN1, &gCanTransfer1) ) ;

    if ( (gCanTransfer1.data_low_reg == CAN_MSG_TOGGLE_LED_GREEN) && (gCanTransfer1.size == 8) )
    {
        puts( "Test4 passed"STRING_EOL ) ;
        _DecodeCanMsg( &gCanTransfer1 ) ;
    }
    else
    {
        puts( "Test4 ERROR"STRING_EOL ) ;
    }
}

/*----------------------------------------------------------------------------
 *        Exported functions
 *----------------------------------------------------------------------------*/

/**
 * \brief Default interrupt handler for CAN 0.
 */
void CAN0_IrqHandler( void )
{
    CAN_Handler( CAN0, &gCanTransfer0 ) ;
}

/**
 * \brief Default interrupt handler for CAN 1.
 */
void CAN1_IrqHandler( void )
{
    CAN_Handler( CAN1, &gCanTransfer1 ) ;
}

/**
 *  main function
 */
extern int main( void )
{
    /* Disable watchdog */
    WDT_Disable( WDT ) ;

    /* Output example information */
    puts( STRING_HEADER ) ;

    /* PIO configuration for LEDs and Buttons. */
    PIO_InitializeInterrupts( 0 ) ;

    puts( "Configure LED PIOs."STRING_EOL ) ;
    _ConfigureLeds() ;

    puts( "Configure buttons with debouncing."STRING_EOL ) ;
    _ConfigureButtons() ;

    puts( "Press LEFT CLICK to increase button counter."STRING_EOL ) ;

    /* Initialize PIOs for CAN */
    PIO_PinConfigure( gPinsCan, PIO_LISTSIZE(gPinsCan) ) ;

    /* Initialize CAN0 Transceiver */
    SN65HVD234_Init( &gCanTransceiver0 ) ;
    SN65HVD234_SetRs( &gCanTransceiver0, PIOB, PIO_PB20 ) ;
    SN65HVD234_SetEN( &gCanTransceiver0, PIOB, PIO_PB21 ) ;
    /* Enable CAN0 Transceiver */
    SN65HVD234_DisableLowPower( &gCanTransceiver0 ) ;
    SN65HVD234_Enable( &gCanTransceiver0 ) ;

    /* Initialize CAN1 Transceiver */
    SN65HVD234_Init( &gCanTransceiver1 ) ;
    SN65HVD234_SetRs( &gCanTransceiver1, PIOE, PIO_PB15 ) ;
    SN65HVD234_SetEN( &gCanTransceiver1, PIOE, PIO_PB16 ) ;
    /* Enable CAN1 Transceiver */
    SN65HVD234_DisableLowPower( &gCanTransceiver1 ) ;
    SN65HVD234_Enable( &gCanTransceiver1 ) ;

    if ( ( CAN_Init( CAN0, BOARD_MCK, 1000, &gCanTransfer0 ) == 1 ) &&
         ( CAN_Init( CAN1, BOARD_MCK, 1000, &gCanTransfer1 ) == 1 ) )
    {
        puts( "CAN initialization complete."STRING_EOL ) ;

        /* Run tests */
        puts( "Press any key to start test"STRING_EOL ) ;
        UART_GetChar() ;
        _Test1() ;

        puts( "Press any key to continue..."STRING_EOL ) ;
        UART_GetChar() ;
        _Test2() ;

        puts( "Press any key to continue..."STRING_EOL ) ;
        UART_GetChar() ;
        _Test3() ;

        puts( "Press any key to continue..."STRING_EOL ) ;
        UART_GetChar() ;
        _Test4() ;

        /* Disable CAN0 Controller */
        CAN_Disable(CAN0) ;
        /* Disable CAN0 Transceiver */
        SN65HVD234_EnableLowPower( &gCanTransceiver0 ) ;
        SN65HVD234_Disable( &gCanTransceiver0 ) ;

        /* Disable CAN1 Controller */
        CAN_Disable(CAN1) ;
        /* Disable CAN1 Transceiver */
        SN65HVD234_EnableLowPower( &gCanTransceiver1 ) ;
        SN65HVD234_Disable( &gCanTransceiver1 ) ;

        puts( "End of all test"STRING_EOL ) ;
    }
    else
    {
        puts( "ERROR CAN initialisation (synchro)"STRING_EOL ) ;
    }

    return 0 ;
}

