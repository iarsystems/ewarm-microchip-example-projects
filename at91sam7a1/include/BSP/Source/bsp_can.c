/****************************************************************************
FILE        BSP_CAN.C - (hardware dependent)

VERSION     1.00

AUTHORS     Patrice Vilchez (Europe Technologies)
            Frederic SAMSON (Europe Technologies)

COPYRIGHT   (c) Europe Technologies
            COMMERCIAL IN CONFIDENCE

CONTAINS    Structure and Enum Definitions and Standard Typedef
            for CAN Package 

MODIFICATION HISTORY

* 1  29/06/2000  Patrice Vilchez
- Initial File
* 1  16/07/2001  Frederic SAMSON
- Clean Up : Add BSP_CANConfigMailbox, 
             BIOS_Can_Status_a_s modified to BSP_Can_Status_a_s
             Add initialisations in BSP_CANInit
             Remove configuration of STANBY mode.
             Update BSP_CANConfigActiveMode
             
* 002  MOD  31/01/02   Mahmoud Mesgarzadeh   Clean Up
****************************************************************************/

#include <string.h>
#include "csp.h"
#include "bsp.h"


/****************************************************************************
* BSP CAN Object Declatation
****************************************************************************/
BSP_CAN_STATUS_T BSP_CanStatus_a_s;

/******************************************************************************
Function          : BSP_CANInit
Description       : CAN initialization : configure the the interrupts and the 
                    general receive and transmit mailboxes.
Inputs            : None
Functions called  : 创CSP_CANInit创,创CSP_CANChannelConfigInterrupt创, 创CSP_CANReceive创 
Returns           : None
******************************************************************************/
void BSP_CANInit(void)
{
   /* Local Variable */
   U8_T   j_u8;

       /* CAN Configuration Data Initialisation */
       BSP_CanStatus_a_s.can_mode_u32           = BSP_CANMode_a_u32;
       BSP_CanStatus_a_s.can_controller_u32     = BSP_CANController_a_u32;
       BSP_CanStatus_a_s.ack_limit_u8           = BSP_CANAckLimit_a_u8;
       BSP_CanStatus_a_s.number_of_mailbox_u8   = BSP_CANNumberOfMailbox;
       BSP_CanStatus_a_s.ack_counter_u8         = 0;
       BSP_CanStatus_a_s.transmit_u8            = 0;
       BSP_CanStatus_a_s.queue.tx_tailptr       = 0;
       BSP_CanStatus_a_s.queue.tx_headptr       = 0;
       BSP_CanStatus_a_s.queue.rx_tailptr       = 0;
       BSP_CanStatus_a_s.queue.rx_headptr       = 0;

       /* Init CAN Controller */
       CSP_CANInit((CSP_CAN_PTR)(BSP_CanStatus_a_s.can_controller_u32), BSP_CanStatus_a_s.can_mode_u32);
       
       /* Configure General Transmit Mailbox */
       CSP_CANChannelConfigInterrupt((CSP_CAN_PTR)BSP_CanStatus_a_s.can_controller_u32, 0, (TXOK|ACK));
       
       /* Configure General Receive Mailbox */
       CSP_CANReceive((CSP_CAN_PTR)BSP_CanStatus_a_s.can_controller_u32, (BSP_CanStatus_a_s.number_of_mailbox_u8 - 1), 0, 0x1FFFFFFF, (8|OVERWRITE));
       CSP_CANChannelConfigInterrupt((CSP_CAN_PTR)BSP_CanStatus_a_s.can_controller_u32, (BSP_CanStatus_a_s.number_of_mailbox_u8 - 1), RXOK);
       BSP_CanStatus_a_s.mailbox_status[BSP_CanStatus_a_s.number_of_mailbox_u8 - 1].identifier_mask = 0x1FFFFFFF ;

       /* General Receive & General Transmit Used */
       BSP_CanStatus_a_s.mailboxes_available =   ( (1 << BSP_CanStatus_a_s.number_of_mailbox_u8)  -  1);
       BSP_CanStatus_a_s.mailboxes_used      =   (1 | (1 << (BSP_CanStatus_a_s.number_of_mailbox_u8 - 1)));
       BSP_CanStatus_a_s.mailboxes_available &= ~(1 | (1 << (BSP_CanStatus_a_s.number_of_mailbox_u8 - 1)));

       /* Initialize Mailbox Status */
       for(j_u8=1; j_u8<(BSP_CanStatus_a_s.number_of_mailbox_u8 - 1); j_u8++)
       {
          BSP_CanStatus_a_s.mailbox_status[j_u8].mailbox_valid = 1;
       } 
   
}

/******************************************************************************
Function          : BSP_CANConfigActiveMode
Description       : Configure CAN0 interrupt and enable CAN0
Inputs            : None
Functions called  : 创CSP_CANConfigInterrupt创,创CSP_CANEnable创
Returns           : None
******************************************************************************/
void BSP_CANConfigActiveMode(void)
{
   /* Local Variable */       

   CSP_CAN_PTR can_controller_ptr;
   U32_T       can_general_callback;
   
   /* Get Current CAN Controller */
   can_controller_ptr = (CSP_CAN_PTR) BSP_CanStatus_a_s.can_controller_u32;
   
   /* Get Appropriate Callback Function */
   can_general_callback = (U32_T) BSP_CAN0AsmInterruptHandler;
   
   /* Configure CAN Controller General Interrupt */
   CSP_CANConfigInterrupt(can_controller_ptr, (HIGH_LEVEL_SENSITIVE | PRIOR_2), 0, can_general_callback);
      
   /* Start CAN */
   CSP_CANEnable(can_controller_ptr);
}


/******************************************************************************
Function          : BSP_CANSendPacket
Description       : Transmit a packet
Inputs            : None
Functions called  : 创CSP_CAN_CHANNEL_SET_DRA创,创CSP_CAN_CHANNEL_SET_DRB创,
                    创CSP_CAN_CHANNEL_SET_IR创, 创CSP_CAN_CHANNEL_SET_CR创
Returns           : None
******************************************************************************/
void BSP_CANSendPacket(void)
{
   /* Local Variables */
   BSP_CAN_PKT_T      *can_pkt;
   U8_T               *tx_tailptr;
   U8_T               *tx_headptr;
   /* Get Current head & tail pointers in Tx Queue */
   tx_tailptr = &(BSP_CanStatus_a_s.queue.tx_tailptr);
   tx_headptr = &(BSP_CanStatus_a_s.queue.tx_headptr);
   
   /* Get Current Tx Packet in Tx Queue */
   can_pkt  = &(BSP_CanStatus_a_s.queue.tx_queue[*tx_tailptr]);
       
   /* Check if the Trasmit Queue is Empty (no more frame to send) */
   if( *tx_headptr != *tx_tailptr )
   {
     /* Copy Data to Send In CAN Mailbox*/
     CSP_CAN_CHANNEL_SET_DRA((CSP_CAN_PTR)(BSP_CanStatus_a_s.can_controller_u32), GENERAL_TRANSMIT, *(U32_T *)(&(can_pkt->data[0])));
     CSP_CAN_CHANNEL_SET_DRB((CSP_CAN_PTR)(BSP_CanStatus_a_s.can_controller_u32), GENERAL_TRANSMIT, *(U32_T *)(&(can_pkt->data[4])));

      /* Set Identifier Field */
      CSP_CAN_CHANNEL_SET_IR(((CSP_CAN_PTR)(BSP_CanStatus_a_s.can_controller_u32)), GENERAL_TRANSMIT, ((can_pkt->identifier & 0x1FFC0000 ) >> 18) | ((can_pkt->identifier & 0x3FFFF) << 11));
      /* Send Data (Extended Format) */
      CSP_CAN_CHANNEL_SET_CR(((CSP_CAN_PTR)(BSP_CanStatus_a_s.can_controller_u32)), GENERAL_TRANSMIT, ( CHANEN | IDE | PCB | ( can_pkt->length & DLC_MASK))); 

      /* Increment Tail Pointer to the next packet to send*/
      (*tx_tailptr)++;
      (*tx_tailptr) %= BSP_CAN_TX_QUEUE_SIZE;
   }
}


/******************************************************************************
Function          : BSP_CAN0InterruptHandler
Description       : CAN0 interrupt handler
Inputs            : None
Functions called  : 创CSP_CAN_GET_SR创,创CSP_CAN_GET_IMR创,
                    创CSP_CAN_SET_CSR创, 创CSP_CAN_GET_ISSR创,
					创CSP_CAN_GET_SIMR创, 创CSP_CAN_SET_CISR创, 创BSP_CAN0MBX15InterruptHandler创,  
					创BSP_CAN0MBX0InterruptHandler创, 创CSP_CAN_CHANNEL_GET_CR创, 
					创memcpy创, 创CSP_CAN_CHANNEL_GET_DRA创, 
                    创CSP_CAN_CHANNEL_GET_IR创, 创CSP_CAN_CHANNEL_GET_IR创, 创CSP_CAN_CHANNEL_SET_CR创
Returns           : None
******************************************************************************/
void BSP_CAN0InterruptHandler(void)
{
   /* Local Variables */
   BSP_CAN_PKT_T  can_pkt;
   U8_T            i;
   
   /* Check Bus Off Interrupt */
   if( (CSP_CAN_GET_SR(CAN0) & BUSOFF & CSP_CAN_GET_IMR(CAN0)) != 0 )
   {
     CSP_CAN_SET_CSR(CAN0, BUSOFF); 
     //BSP_CanStatus_a_s[0].bus_off = 1;
   }
   else
   {
      /* Check General Receive Interrupt */
      if( (CSP_CAN_GET_ISSR(CAN0) & (1 << 15) & CSP_CAN_GET_SIMR(CAN0)) != 0 )
      {
         CSP_CAN_SET_CISR(CAN0, (1 << 15));
         BSP_CAN0MBX15InterruptHandler();
      }
      
      /* Check General Transmit Interrupt */
      if( (CSP_CAN_GET_ISSR(CAN0) & (1 << 0) & CSP_CAN_GET_SIMR(CAN0)) != 0 )
      {
         CSP_CAN_SET_CISR(CAN0,(1 << 0));
         BSP_CAN0MBX0InterruptHandler();
      }
   
      /* Check Which Channel Produce an Interrupt */
      for(i=1; i<15; i++)
      {
         if( (CSP_CAN_GET_ISSR(CAN0) & (1 << i) & CSP_CAN_GET_SIMR(CAN0)) != 0 )
         {
            /* Clear Interrupt Flag */
            CSP_CAN_SET_CISR(CAN0,(1 << i));

            /* Get packet length */
            can_pkt.length = CSP_CAN_CHANNEL_GET_CR(CAN0, i) & 0x0F ;

            /* Copy Data  */
            memcpy((U8_T*)(&(can_pkt.data[0])), (U8_T *)(&(CSP_CAN_CHANNEL_GET_DRA(CAN0, i))), (8 * sizeof(U8_T)) );
             
            /* Get Idetifier Field */
            can_pkt.identifier = (( CSP_CAN_CHANNEL_GET_IR(CAN0, i) & 0x7FF ) << 18) | ((CSP_CAN_CHANNEL_GET_IR(CAN0, i) & 0x1FFFF800)  >> 11);
         
            /* Set Message Waiting Flag */
            BSP_CanStatus_a_s.mailbox_status[i].message_waiting = 1;
            memcpy((U8_T*)(&(BSP_CanStatus_a_s.mailbox_status[i].data[0])), (U8_T *)(&(CSP_CAN_CHANNEL_GET_DRA(CAN0, i))), (8 * sizeof(U8_T)));
         
            /* Call BIOS CAN Mailbox Callback */
            (BSP_CanStatus_a_s.mailbox_status[i].mailbox_function)(&can_pkt);
            
            /* Restart Reception */
            CSP_CAN_CHANNEL_SET_CR(CAN0, i, 0x98);

         }
      }
   }
}


/******************************************************************************
Function          : BSP_CAN0InterruptHandler
Description       : CAN0 interrupt handler
Inputs            : None
Functions called  : 
Returns           : None
******************************************************************************/
void BSP_CAN0MBX0InterruptHandler(void)
{
   /* Local Variables */
   BSP_CAN_PKT_T   *can_pkt;
   U8_T            *tx_tailptr;
   U8_T            *tx_headptr;
   
   /* Get Current head & tail pointers in Tx Queue */
   tx_tailptr = &(BSP_CanStatus_a_s.queue.tx_tailptr);
   tx_headptr = &(BSP_CanStatus_a_s.queue.tx_headptr);
   
   /* Get Current Tx Packet in Tx Queue */
   can_pkt  = &(BSP_CanStatus_a_s.queue.tx_queue[*tx_tailptr]);
   
   /* Check if ACK is an Interrupt Source */
   if( (CSP_CAN_CHANNEL_GET_SR(CAN0, GENERAL_TRANSMIT) & ACK & CSP_CAN_CHANNEL_GET_IMR(CAN0, GENERAL_TRANSMIT) ) != 0 )
   {
      while(CSP_CAN_CHANNEL_GET_SR(CAN0, GENERAL_TRANSMIT) & ACK)
             CSP_CAN_CHANNEL_SET_CSR(CAN0, GENERAL_TRANSMIT, ACK);

      /* Increment Ack Counter */
      BSP_CanStatus_a_s.ack_counter_u8++;
      
      /* If Ack Limit */
      if( BSP_CanStatus_a_s.ack_counter_u8 == BSP_CanStatus_a_s.ack_limit_u8 )     
      {
         /* Disable Transission */
         CSP_CAN_CHANNEL_SET_CR(CAN0, GENERAL_TRANSMIT, 0);
         
         /* Clear Ack Counter */
         BSP_CanStatus_a_s.ack_counter_u8 = 0;
         
         /* Check if the Trasmit Queue is Empty (no more frame to send) */
         if( *tx_headptr != *tx_tailptr )
         {
            /* Copy Data to Send In CAN Mailbox*/
            CSP_CAN_CHANNEL_SET_DRA(CAN0, GENERAL_TRANSMIT, *(U32_T *)(&(can_pkt->data[0])));
            CSP_CAN_CHANNEL_SET_DRB(CAN0, GENERAL_TRANSMIT, *(U32_T *)(&(can_pkt->data[4])));

            /* Set Idetifier Field */
            CSP_CAN_CHANNEL_SET_IR(CAN0, GENERAL_TRANSMIT, (( can_pkt->identifier & 0x1FFC0000 ) >> 18) | ((can_pkt->identifier & 0x3FFFF) << 11));        
         
            /* Send Data (Extended Format) */
            CSP_CAN_CHANNEL_SET_CR(CAN0, GENERAL_TRANSMIT, ( CHANEN | IDE | PCB | ( can_pkt->length & DLC_MASK)));

            /* Increment Tail Pointer to the next packet to send*/
            (*tx_tailptr)++;
            (*tx_tailptr) %= BSP_CAN_TX_QUEUE_SIZE;
         }
         /* Tx Queue is Empty */     
         else
         {
            BSP_CanStatus_a_s.transmit_u8  = 0;
         }
      }   
   }
   /* Check if TXOK is an Interrupt Source */
   else if( (CSP_CAN_CHANNEL_GET_SR(CAN0, GENERAL_TRANSMIT) & TXOK & CSP_CAN_CHANNEL_GET_IMR(CAN0, GENERAL_TRANSMIT)) != 0 )
   {
      /* Clear TXOK Error Interrupt Flag*/
      while(CSP_CAN_CHANNEL_GET_SR(CAN0, GENERAL_TRANSMIT) & TXOK )
             CSP_CAN_CHANNEL_SET_CSR(CAN0, GENERAL_TRANSMIT, TXOK);
      
      /* Check if the Trasmit Queue is Empty (no more frame to send) */
      if( *tx_headptr != *tx_tailptr )
      {
         /* Copy Data to Send In CAN Mailbox*/
            CSP_CAN_CHANNEL_SET_DRA(CAN0, GENERAL_TRANSMIT, *(U32_T *)(&(can_pkt->data[0])));
            CSP_CAN_CHANNEL_SET_DRB(CAN0, GENERAL_TRANSMIT, *(U32_T *)(&(can_pkt->data[4])));                
         
         /* Set Idetifier Field */
          CSP_CAN_CHANNEL_SET_IR(CAN0, GENERAL_TRANSMIT, (( can_pkt->identifier & 0x1FFC0000 ) >> 18) | ((can_pkt->identifier & 0x3FFFF) << 11));
         
         /* Send Data (Extended Format) */
         CSP_CAN_CHANNEL_SET_CR(CAN0, GENERAL_TRANSMIT, ( CHANEN | IDE | PCB | ( can_pkt->length & DLC_MASK))); 

         /* Increment Tail Pointer to the next packet to send*/
         (*tx_tailptr)++;
         (*tx_tailptr) %= BSP_CAN_TX_QUEUE_SIZE;
      }
      /* Tx Queue is Empty */     
      else
      {
         BSP_CanStatus_a_s.transmit_u8  = 0;
      }
   }
}

/******************************************************************************
Function          : BSP_CAN0MBX15InterruptHandler
Description       : CAN0 interrupt handler
Inputs            : None
Functions called  : 
Returns           : None
******************************************************************************/
void BSP_CAN0MBX15InterruptHandler(void)
{
   /* Local Variables */
   BSP_CAN_PKT_T  *can_pkt;
   U8_T           *rx_headptr;
   
   /* Get Current head pointers in Rx Queue */
   rx_headptr = &(BSP_CanStatus_a_s.queue.rx_headptr);
   
   /* Get Current CAN Packet in Rx Queue */
   can_pkt = &(BSP_CanStatus_a_s.queue.rx_queue[*rx_headptr]);
   
   /* Check if RXOK is an Interrupt Source */
   if( (CSP_CAN_CHANNEL_GET_SR(CAN0, MBX15) & RXOK & CSP_CAN_CHANNEL_GET_IMR(CAN0, MBX15)) != 0 )
   {
      /* Clear ACK Error Interrupt Flag*/
      CSP_CAN_CHANNEL_SET_CSR(CAN0, MBX15, RXOK);

      /* Get packet length */
      can_pkt->length = CSP_CAN_CHANNEL_GET_CR(CAN0, MBX15) & 0x0F ;

      /* Copy Data In Rx Queue */
      memcpy((U8_T*)(&(can_pkt->data[0])), (U8_T*)(&(CSP_CAN_CHANNEL_GET_DRA(CAN0, MBX15))), (8 * sizeof(U8_T)) );
             
      /* Get Idetifier Field */
      can_pkt->identifier = (( CSP_CAN_CHANNEL_GET_IR(CAN0, MBX15) & 0x7FF ) << 18) | ((CSP_CAN_CHANNEL_GET_IR(CAN0, MBX15) & 0x1FFFF800)  >> 11);
      can_pkt->msecs      = CSP_CAN_CHANNEL_GET_STP(CAN0, MBX15);
         
      /* Increment head Pointer to the next packet to receive */
      (*rx_headptr)++;
      (*rx_headptr) %= BSP_CAN_RX_QUEUE_SIZE;
   }
}

/******************************************************************************
Function          : BSP_CANConfigMailbox
Description       : CAN0 interrupt handler
Inputs            : None
Functions called  : 
Returns           : None
******************************************************************************/
void BSP_CANConfigMailbox(BSP_CAN_MAILBOX_ID_E mailbox_id, BSP_CAN_MAILBOX_CONFIGURE_T *mailbox_config)
{
   /* Local Variables */
   CSP_CAN_PTR can_controller_ptr;

      if( (mailbox_id != GENERAL_RECEIVE) &&  (mailbox_id != GENERAL_TRANSMIT) && 
          ((U8_T) mailbox_id > 0)         &&  ((U8_T) mailbox_id < (BSP_CanStatus_a_s.number_of_mailbox_u8 - 1)) )
      {
         /* Get Current CAN Controller */
         can_controller_ptr = (CSP_CAN_PTR) BSP_CanStatus_a_s.can_controller_u32;

         /* Set CAN Mailbox Status Callback Function */
         BSP_CanStatus_a_s.mailbox_status[mailbox_id].identifier       = mailbox_config->identifier;
         BSP_CanStatus_a_s.mailbox_status[mailbox_id].identifier_mask  = mailbox_config->identifier_mask;
         BSP_CanStatus_a_s.mailbox_status[mailbox_id].length           = mailbox_config->length;
         BSP_CanStatus_a_s.mailbox_status[mailbox_id].mailbox_function = mailbox_config->mailbox_function;
         BSP_CanStatus_a_s.mailboxes_used |= (1 << mailbox_id);
         BSP_CanStatus_a_s.mailboxes_available &= ~(1 << mailbox_id);

         /* Configure CAN Channel In Reception with Id and Mask */
         CSP_CANReceive(can_controller_ptr, mailbox_id,  mailbox_config->identifier,  mailbox_config->identifier_mask, ((mailbox_config->length)|OVERWRITE));

         /* Configure CAN Channel Interrupt */
         CSP_CANChannelConfigInterrupt(can_controller_ptr, mailbox_id, RXOK);
      }
      /* BIOS Fault */
      else
      {
         //BIOS_Error_e = BIOS_INVALID_PARAMETER; 
      }

}
