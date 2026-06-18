/**************************************************************************
MODULE:    MCOHWATMEL
CONTAINS:  Preliminary, limited hardware driver implementation for
           Atmel - tested using the Atmel demo board and CANopen demo board.
           This version re-uses functions provided by
           www.esacademy.com/faq/progs
COPYRIGHT: Embedded Systems Academy, Inc. 2002-2007
           All rights reserved. www.microcanopen.com
           This software was written in accordance to the guidelines at
           www.esacademy.com/software/softwarestyleguide.pdf
DISCLAIM:  Read and understand our disclaimer before using this code!
           www.esacademy.com/disclaim.htm
LICENSE:   THIS IS THE EDUCATIONAL VERSION OF MICROCANOPEN
           See file license_educational.txt or
           www.microcanopen.com/license_educational.txt
VERSION:   3.30, ESA 30-JAN-07
           $LastChangedDate: 2014-01-27 15:12:42 +0100 (mån, 27 jan 2014) $
           $LastChangedRevision: 10 $
---------------------------------------------------------------------------
Known shortcoming:
Only supports a transmit queue of length "1"
If queue occupied, waits until it is clear
***************************************************************************/ 

#include "mcohw.h"
#include "board.h"
#include <can/can.h>
#include <pio/pio.h>
#include <pio/pio_it.h>
#include <irq/irq.h>
#include <pit/pit.h>
#include <utility/trace.h>

// Global timer/conter variable, incremented every millisecond
UNSIGNED16 MEM_NEAR gTimCnt = 0;

// Counts number of filters (CAN message objects) used
UNSIGNED8 MEM_FAR gCANFilter = 0;

CanTransfer can0Transfer;
CanTransfer can1Transfer;

/**************************************************************************
DOES:    Sets one of the four screeners (acceptance filters) of
         the CAN controller.
CAUTION: For the AT89C51CC01 from Atmel the screeners translate
         to individual message buffers 1-14. The parameters
         x_Mask and Bx_Match are ignored.
RETURNS: nothing
**************************************************************************/
void set_screener_std
  (
  UNSIGNED8 Screener,  // 1 - 4, one of the four screeners
  UNSIGNED16 ID_Match   // match/code value for id
  )
{
    AT91PS_CAN_MB CAN_mailbox;

    // select message object
    CAN_mailbox = AT91C_BASE_CAN0_MB0;
    CAN_mailbox = (AT91PS_CAN_MB)((unsigned int)CAN_mailbox+(unsigned int)(0x20*Screener));

  
//    if(STANDARD_FORMAT==0) {    // IDvA Standard Format
        CAN_mailbox->CAN_MB_MID = ID_Match<<18;
//    }
//    else { // IDvB Extended Format
//        CAN_mailbox->CAN_MB_MID = ID_Match | (1<<29); // set MIDE bit
//    }

    // CfgMessageAcceptanceMaskReg
    CAN_mailbox->CAN_MB_MAM = 0x3FFFFFFF;

    // CfgMessageModeReg
    CAN_mailbox->CAN_MB_MMR = AT91C_CAN_MOT_RX | AT91C_CAN_PRIOR;

    // InitTransferRequest
    AT91C_BASE_CAN0->CAN_TCR = 1<<Screener;
}


/************************************************************************
DOES: This function implements a CAN receive queue. With each
function call a message is pulled from the queue.
RETURNS: 1 Message was pulled from receive queue
0 Queue empty, no message received
NOTES: Implementation of this function greatly varies with CAN
controller used. In an SJA1000 style controller, the hardware
queue inside the controller can be used as the queue.
Controllers with just one receive buffer need a bigger software
queue. "Full CAN" style controllers might just implement
multiple message objects, one each for each ID received (using
function MCOHW_SetCANFilter).
************************************************************************/
UNSIGNED8 MCOHW_PullMessage (
    CAN_MSG MEM_FAR *pReceiveBuf // Data structure with message received
)
{
    UNSIGNED8  Length;
    UNSIGNED8  i,j;
    unsigned int  BufferLo;
    unsigned int  BufferHi;

    TRACE_INFO("PullMessage READ\n\r");

    // loop through all the receive message objects
    for (j=1; j<=gCANFilter; j++)
    {
        // Init CAN0 Mailbox 1, receive, 
        CAN_ResetTransfer( &can0Transfer );
        can0Transfer.can_number = 0;
        can0Transfer.mailbox_number = 1;
        can0Transfer.mode_reg = AT91C_CAN_MOT_RX;
        can0Transfer.acceptance_mask_reg = 0;
        can0Transfer.identifier = 0x0;
        can0Transfer.data_low_reg = 0x00000000;
        can0Transfer.data_high_reg = 0x00000000;
        can0Transfer.control_reg = 0x00000000;
        CAN_InitMailboxRegisters( &can0Transfer );

        while( CAN_STATUS_SUCCESS != CAN_Read( &can0Transfer) ) {}
        while( CAN_IsInIdle(&can0Transfer) ) {}

        // copy message identifer and length to application message buffer
        pReceiveBuf->ID  = can0Transfer.identifier;;
        pReceiveBuf->LEN = can0Transfer.size;

        BufferLo = can0Transfer.data_low_reg;
        BufferHi = can0Transfer.data_high_reg;

        // copy message data to application message buffer     
        for (i=0; i < Length; i++)
        {
            if(i<4) {
                // copy bytes
                *(UNSIGNED8 *)(pReceiveBuf->BUF+i) = (UNSIGNED8) (BufferLo >> (8*i));
            }
            else { 
                // 4<i<8
                // copy bytes
                *(UNSIGNED8 *)(pReceiveBuf->BUF+i) = (UNSIGNED8) (BufferHi >> (8*i));
            }
        }
      
        // clear receive ok flag so we can receive another message
        // re-enable message object - message object receives and is enabled
        // 8 bytes expected
        //AT91C_BASE_CAN0->CAN_TCR = 1<<j;
        // Message was pulled from receive queue
        return 1;
    }

    // Queue empty, no message received
    return 0;
}


/************************************************************************
DOES: This function implements a CAN transmit queue. With each
function call a message is added to the queue.
RETURNS: 1 Message was added to the transmit queue
0 If queue is full, message was not added,
NOTES: The MicroCANopen stack will not try to add messages to the queue
"back-to-back". With each call to MCO_ProcessStack, a maximum
of one message is added to the queue. For many applications
a queue with length "1" will be sufficient. Only applications
with a high busload or very slow bus speed might need a queue
of length "3" or more.
************************************************************************/
UNSIGNED8 MCOHW_PushMessage (
    CAN_MSG MEM_FAR *pTransmitBuf // Data structure with message to be send
)
{
    UNSIGNED8  BufferHi=0;
    UNSIGNED8  BufferLo=0;
    UNSIGNED16 timeout;
    // local loop counter
    UNSIGNED8  i;

    timeout = AT91C_CAN_TIMEOUT;
    // Wait for Last Transmit Mailbox
    while( ((((&AT91C_BASE_CAN0->CAN_MB0)->CAN_MB_MSR) & AT91C_CAN_MRDY) != AT91C_CAN_MRDY)
            && timeout )
    {
        timeout--;
    }

    if(timeout == 0) {
        // queue is full
        return 0;
    }

    // write data to transmit buffer
    for( i=0; i<4; i++ )
    {
        BufferLo |= (pTransmitBuf->BUF[i]) << (8*i);
        BufferHi |= (pTransmitBuf->BUF[i+4]) << (8*i);
    }

    // Init Mailbox 0
    CAN_ResetTransfer( &can0Transfer );
    can0Transfer.can_number = 0;
    can0Transfer.mailbox_number = 0;
    can0Transfer.mode_reg = AT91C_CAN_MOT_TX | ((0xF<<16) & AT91C_CAN_PRIOR);
    can0Transfer.acceptance_mask_reg = 0x00000000;
    can0Transfer.identifier = AT91C_CAN_MIDvA & (pTransmitBuf->ID<<18);
    can0Transfer.data_low_reg = BufferLo;
    can0Transfer.data_high_reg = BufferHi;
    can0Transfer.control_reg = (AT91C_CAN_MDLC & (pTransmitBuf->LEN<<16));
    CAN_InitMailboxRegisters( &can0Transfer );

    // message sent
    while( CAN_STATUS_SUCCESS !=  CAN_Write( &can0Transfer ) ){}
    while( CAN_IsInIdle(&can0Transfer) ) {}

    return 1;
}


/**************************************************************************
DOES:    Gets the value of the current 1 millisecond system timer 
RETURNS: The current timer tick                                   
**************************************************************************/
UNSIGNED16 MCOHW_GetTime
  (
  void
  )
{
  return gTimCnt;
}

/**************************************************************************
DOES:    Checks if a moment in time has passed (a timestamp has expired)
RETURNS: 0 if timestamp has not yet expired, 1 if the             
         timestamp has expired                                    
**************************************************************************/
UNSIGNED8 MCOHW_IsTimeExpired
  (
  UNSIGNED16 timestamp  // timestamp to check for expiration
  )
{
  UNSIGNED16 time_now;

  // get a copy of the current time
  time_now = gTimCnt;
  // to ensure the minimum runtime
  timestamp++;
  // if timestamp is less than the current time...
  if (time_now > timestamp)
  {
    // check if the timestamp has expired
    if ((time_now - timestamp) < 0x8000)
    {
      return 1;
    }
    // timestamp has not expired
    else
    {
      return 0;
    }
  }
  // if timestamp is greater than the current time...
  else
  {
    // check if the timestamp has expired
    if ((timestamp - time_now) > 0x8000)
    {
      return 1;
    }
    // timestamp has not expired
    else
    {
      return 0;
    }
  }
}

/**************************************************************************
DOES:    Timer interrupt service routine                          
         Increments the global millisecond counter tick           
         This function needs to be called once every millisecond  
RETURNS: nothing                                                     
**************************************************************************/
void MCOHW_TimerISR
  (
  void
  ) //interrupt 1
{
  // increment global counter
  gTimCnt++;
}

//*----------------------------------------------------------------------------
//* \fn    PITC_Handler
//* \brief
//*----------------------------------------------------------------------------
void PITC_Handler(void)
{
  volatile unsigned int status=AT91C_BASE_PITC->PITC_PISR;

  // Periodic Interval Timer Status
  if (status & AT91C_PITC_PITS)
  {
    // Interrupt Acknowledge
    status = AT91C_BASE_PITC->PITC_PIVR;
    MCOHW_TimerISR();
    return;
  }
}


/************************************************************************
DOES: This function implements the initialization of the CAN
interface.
RETURNS: 1 if init is completed
0 if init failed
************************************************************************/
UNSIGNED8 MCOHW_Init (
    UNSIGNED16 BaudRate // Allowed values: 1000, 800, 500, 250, 125, 50, 25, 10
)
{
    //  Init CAN0
#if defined(at91sam7a3ek)
    if( CAN_Init( BaudRate, &can0Transfer, &can1Transfer ) == 0 ) {
        return 0;
    }
#else
    if( CAN_Init( BaudRate, &can0Transfer, NULL ) == 0 ) {
        return 0;
    }
#endif
    CAN_ResetAllMailbox();
    // Init All The Mailbox Registers
    // Init Mailbox 0
    CAN_ResetTransfer( &can0Transfer );
    can0Transfer.can_number = 0;
    can0Transfer.mailbox_number = 0;
    can0Transfer.mode_reg = AT91C_CAN_MOT_TX | ((0xF<<16) & AT91C_CAN_PRIOR);
    can0Transfer.acceptance_mask_reg = 0;
    can0Transfer.identifier = AT91C_CAN_MIDvA & 0x00000000;
    can0Transfer.data_low_reg = 0x00000000;
    can0Transfer.data_high_reg = 0x00000000;
    can0Transfer.control_reg = (AT91C_CAN_MDLC & (0x0<<16));
    CAN_InitMailboxRegisters( &can0Transfer );

    // no filters configured
    gCANFilter = 0;

    // Initialize Timer interrupt here.
    // MCOHW_TimerISR must be executed once every millisecond.
    IRQ_ConfigureIT( AT91C_ID_SYS, 0x5, PITC_Handler );

    // Enable the interrupt on the interrupt controller
    IRQ_EnableIT( AT91C_ID_SYS );

    PIT_Init( 1000, (BOARD_MCK/1000000) );
    PIT_EnableIT();

    // return result of initialization
    return 1;
}


/************************************************************************
DOES: This function implements the initialization of a CAN ID hardware
filter as supported by many CAN controllers.
RETURNS: 1 if filter was set
2 if this HW does not support filters
(in this case HW will receive EVERY CAN message)
0 if no more filter is available
************************************************************************/
UNSIGNED8 MCOHW_SetCANFilter (
    UNSIGNED16 CANID // CAN-ID to be received by filter
)
{
  gCANFilter++;
  // if all filters used then fail
  if (gCANFilter > 14)
  {
    // No more filter available
    return 0;
  }
  // filter available
  else
  {
    // configure the filter
    set_screener_std(gCANFilter, CANID);
    // Filter was set
    return 1;
  }
}

