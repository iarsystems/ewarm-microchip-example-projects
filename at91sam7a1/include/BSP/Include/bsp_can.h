/*-----------------------------------------------------------------------------
*   EUROPE TECHNOLOGIES Software Support
*------------------------------------------------------------------------------
* The software is delivered "AS IS" without warranty or condition of any
* kind, either express, implied or statutory. This includes without
* limitation any warranty or condition with respect to merchantability or
* fitness for any particular purpose, or against the infringements of
* intellectual property rights of others.
*------------------------------------------------------------------------------
*
* Processor   : SmartCAN1 for EVMSmartCAN
* File Name   : bsp_can.h
* Description : Definitions and Standard Typedef for
                CAN (Board Support Package Layer)
* Version     : 1.02
*
*       +----- (NEW | MODify | ADD | DELete)
*       |
*  No   |   when       who                what
*-----+---+----------+------------------+--------------------------------------
* 000  NEW  29/06/00   Patrice Vilchez    Creation
* 001  MOD  16/07/01   Frederic SAMSON    Mailbox creation, message queue structure
* 002  MOD  21/11/02   Bruno SALLE        Update for SmartCAN1 (16 channels)
*----------------------------------------------------------------------------*/


#include "bsp_can_conf.h"
//#include "bios.h"

#ifndef BSP_CAN_H
#define BSP_CAN_H

/****************************************************************************
* BSP CAN Definition
****************************************************************************/
#define BSP_CAN_RX_QUEUE_SIZE 16
#define BSP_CAN_TX_QUEUE_SIZE 16


/***************************************************************************/
/* Bsp CAN Mailbox Enum Definition                                          */
/***************************************************************************/
typedef enum { GENERAL_RECEIVE = -1, GENERAL_TRANSMIT, MBX1, MBX2, MBX3,
               MBX4, MBX5, MBX6, MBX7, MBX8, MBX9, MBX10, MBX11, MBX12,
	       MBX13, MBX14, MBX15, MBX16} BSP_CAN_MAILBOX_ID_E;

/****************************************/
/* BSP CAN packet Structure Definition  */
/****************************************/

/*******************************************************************************
Type: BSP_CAN_PKT_T
Description: BSP CAN packet Structure Definition
Fields:
- 创U32_T创 msecs :
- 创U32_T创 identifier :
- 创U8_T创  data[8] :
- 创U8_T创  length  :
*******************************************************************************/
typedef struct
{
   U32_T msecs;
   U32_T identifier;
   U8_T  data[8];
   U8_T  length ;
} BSP_CAN_PKT_T;


/****************************************/
/* BSP CAN Receive Callback Definition  */
/****************************************/

/*******************************************************************************
Type: BSP_CAN_CALLBACK_T
Description:
Declaration: typedef *(BSP_CAN_CALLBACK_T)(BSP_CAN_PKT_PTR);
Inputs: 创BSP_CAN_PKT_T创 *
*******************************************************************************/
typedef void *(BSP_CAN_CALLBACK_T)(BSP_CAN_PKT_T *);

/*******************************************************/
/* Bios CAN Mailbox Configuration Structure Definiton  */
/*******************************************************/

/*******************************************************************************
Type: BSP_CAN_MAILBOX_CONFIGURE_T
Description: Bios CAN Mailbox Configuration Structure Definiton
Fields:
- 创U8_T创                 length :
- 创U32_T创                identifier_mask :
- 创U32_T创                identifier :
- 创BSP_CAN_CALLBACK_T创   *mailbox_function :
*******************************************************************************/
typedef struct
{
   U8_T                 length;
   U32_T                identifier_mask;
   U32_T                identifier;
   BSP_CAN_CALLBACK_T   *mailbox_function;
} BSP_CAN_MAILBOX_CONFIGURE_T;

/***********************************************/
/* BSP CAN Mailbox State Structure Definition  */
/***********************************************/

/*******************************************************************************
Type: BSP_CAN_MAILBOX_STATUS_T
Description: BSP CAN Mailbox State Structure Definition
Fields:
- 创U32_T创                  identifier :
- 创U32_T创                  identifier_mask :
- 创U8_T创                   length :
- 创U8_T创                   data[8] :
- int                    message_waiting:1 :
- int                    mailbox_valid:1 :
- int                    reserved:30 :
- 创BSP_CAN_CALLBACK_T创    *mailbox_function :
*******************************************************************************/
typedef struct
{
   U32_T                  identifier;
   U32_T                  identifier_mask;
   U8_T                   length;
   U8_T                   data[8];
   int                    message_waiting:1;
   int                    mailbox_valid:1;
   int                    reserved:30;
   BSP_CAN_CALLBACK_T    *mailbox_function;
} BSP_CAN_MAILBOX_STATUS_T;


/****************************************************************************
* BSP CAN Structure of Transmit & Receive Queue
****************************************************************************/

/*******************************************************************************
Type: BSP_CAN_QUEUE_T
Description: BSP CAN Structure of Transmit & Receive Queue
Fields:
- 创BSP_CAN_PKT_T创	tx_queue[BSP_CAN_RX_QUEUE_SIZE] :
- 创BSP_CAN_PKT_T创	rx_queue[BSP_CAN_RX_QUEUE_SIZE] :
- 创U8_T创				tx_headptr :
- 创U8_T创				tx_tailptr :
- 创U8_T创				rx_headptr :
- 创U8_T创				rx_tailptr :
*******************************************************************************/
typedef struct
{
   BSP_CAN_PKT_T	tx_queue[BSP_CAN_RX_QUEUE_SIZE];
   BSP_CAN_PKT_T	rx_queue[BSP_CAN_RX_QUEUE_SIZE];
   U8_T				tx_headptr;
   U8_T				tx_tailptr;
   U8_T				rx_headptr;
   U8_T				rx_tailptr;
} BSP_CAN_QUEUE_T;


/****************************************************************************
* BSP CAN Status Structure
****************************************************************************/

/*******************************************************************************
Type: BSP_CAN_STATUS_T
Description: BSP CAN Status Structure
Fields:
- 创U32_T创                		can_controller_u32 :
- 创U32_T创                		can_mode_u32 :
- 创U8_T创                 		number_of_mailbox_u8 :
- 创U8_T创                 		transmit_u8 :
- 创U8_T创                 		ack_limit_u8 :
- 创U8_T创                 		ack_counter_u8 :
- 创U8_T创							mailboxes_used :
- 创U8_T创							mailboxes_available :
- 创BSP_CAN_QUEUE_T创      		queue :
- 创BSP_CAN_MAILBOX_STATUS_T创   	mailbox_status[16] :
*******************************************************************************/
typedef struct
{
   U32_T                		can_controller_u32;
   U32_T                		can_mode_u32;
   U8_T                 		number_of_mailbox_u8;
   U8_T                 		transmit_u8;
   U8_T                 		ack_limit_u8;
   U8_T                 		ack_counter_u8;
   U8_T							mailboxes_used;
   U8_T							mailboxes_available;
   BSP_CAN_QUEUE_T      		queue;
   BSP_CAN_MAILBOX_STATUS_T   	mailbox_status[16];
} BSP_CAN_STATUS_T;

/****************************************************************************
* BSP CAN External Objects Declatation
****************************************************************************/
extern BSP_CAN_STATUS_T BSP_CanStatus_a_s;

/****************************************************************************
* BSP CAN Power Management Function Declaration
****************************************************************************/
extern void BSP_CANInit(void);
extern void BSP_CANConfigActiveMode(void);

/****************************************************************************
* BSP CAN Tx & Rx Queue Fucntion Declaration
****************************************************************************/
extern void BSP_CAN0MBX15InterruptHandler(void);
extern void BSP_CAN0MBX0InterruptHandler(void);

extern void BSP_CAN1MBX15InterruptHandler(void);
extern void BSP_CAN1MBX0InterruptHandler(void);

extern void BSP_CAN2MBX31InterruptHandler(void);
extern void BSP_CAN2MBX0InterruptHandler(void);

/****************************************************************************
* BSP CAN Assembler Interrupt Handler Function Declaration (Interrupt Entry)
****************************************************************************/
extern void BSP_CAN0AsmInterruptHandler(void);
extern void BSP_CAN1AsmInterruptHandler(void);
extern void BSP_CAN2AsmInterruptHandler(void);

/****************************************************************************
* BSP CAN "C" General Interrupt Handler Function Declaration
****************************************************************************/
extern void BSP_CAN0InterruptHandler(void);
extern void BSP_CAN1InterruptHandler(void);
extern void BSP_CAN2InterruptHandler(void);
extern void BSP_CANSendPacket(void);
extern void BSP_CANConfigMailbox(BSP_CAN_MAILBOX_ID_E mailbox_id, BSP_CAN_MAILBOX_CONFIGURE_T *mailbox_config);


#endif  /* BSP_CAN_H */
