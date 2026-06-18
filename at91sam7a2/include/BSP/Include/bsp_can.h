/****************************************************************************
FILE        BSP_CAN.H - (hardware dependent)

VERSION     1.00

AUTHORS     Patrice Vilchez (Europe Technologies)
            Frederic SAMSON (Europe Technologies)

COPYRIGHT   (c) Europe Technologies
            COMMERCIAL IN CONFIDENCE

CONTAINS    Declaration for CAN Package of
            BSP Layer

MODIFICATION HISTORY

* 1  29/06/2000  Patrice Vilchez
- Initial File
* 2  16/07/2001  Frederic SAMSON
- Update : - Move Mailbox enumeration, Power State enumeration
           Mailbox Configuration structure,
           Mailbox Status enumeration, Receive and Transmit Queue structure
           Can Status structure from BIOS layer to BSP layer.
           - Add BSP_CANConfigMailbox
****************************************************************************/

#include "bsp_can_conf.h"
//#include "bios.h"

#ifndef BSP_CAN_H
#define BSP_CAN_H

/****************************************************************************
* BSP CAN Definition
****************************************************************************/
#define BSP_CAN_RX_QUEUE_SIZE 32
#define BSP_CAN_TX_QUEUE_SIZE 32


/***************************************************************************/
/* Bsp CAN Mailbox Enum Definition                                          */
/***************************************************************************/
typedef enum { GENERAL_RECEIVE = -1, GENERAL_TRANSMIT, MBX1, MBX2, MBX3,
               MBX4, MBX5, MBX6, MBX7, MBX8, MBX9, MBX10, MBX11, MBX12,
	       MBX13, MBX14, MBX15, MBX16, MBX17, MBX18, MBX19, MBX20, MBX21,
	       MBX22, MBX23, MBX24, MBX25, MBX26, MBX27, MBX28, MBX29, MBX30,
	       MBX31 } BSP_CAN_MAILBOX_ID_E;

/****************************************/
/* BSP CAN packet Structure Definition  */
/****************************************/
typedef struct
{
   U32_T msecs;
   U32_T identifier;
   U8_T  data[8];
   U8_T  length ;
} BSP_CAN_PKT_T, *BSP_CAN_PKT_PTR;


/****************************************/
/* BSP CAN Receive Callback Definition  */
/****************************************/
typedef void *(BSP_CAN_CALLBACK_T)(BSP_CAN_PKT_PTR);

/*******************************************************/
/* Bios CAN Mailbox Configuration Structure Definiton  */
/*******************************************************/
typedef struct
{
   U8_T                 length;
   U32_T                identifier_mask;
   U32_T                identifier;
   BSP_CAN_CALLBACK_T   *mailbox_function;
} BSP_CAN_MAILBOX_CONFIGURE_T, *BSP_CAN_MAILBOX_CONFIGURE_PTR;

/***********************************************/
/* BSP CAN Mailbox State Structure Definition  */
/***********************************************/
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
} BSP_CAN_MAILBOX_STATUS_T, *BSP_CAN_MAILBOX_STATUS_PTR;


/****************************************************************************
* BSP CAN Structure of Transmit & Receive Queue
****************************************************************************/
typedef struct
{
   BSP_CAN_PKT_T	tx_queue[BSP_CAN_RX_QUEUE_SIZE];
   BSP_CAN_PKT_T	rx_queue[BSP_CAN_RX_QUEUE_SIZE];
   U8_T				tx_headptr;
   U8_T				tx_tailptr;
   U8_T				rx_headptr;
   U8_T				rx_tailptr;
} BSP_CAN_QUEUE_T, *BSP_CAN_QUEUE_PTR;


/****************************************************************************
* BSP CAN Status Structure
****************************************************************************/
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
   BSP_CAN_MAILBOX_STATUS_T   	mailbox_status[32];
} BSP_CAN_STATUS_T, *BSP_CAN_STATUS_PTR;

/****************************************************************************
* BSP CAN External Objects Declatation
****************************************************************************/
extern BSP_CAN_STATUS_T BSP_CanStatus_a_s[(U8_T) BSP_NB_CAN_IO];

/****************************************************************************
* BSP CAN Power Management Function Declaration
****************************************************************************/
extern void BSP_CANInit(void);
extern void BSP_CANConfigActiveMode(BSP_CAN_IO_E Can_Number);

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
extern void BSP_CANSendPacket(BSP_CAN_IO_E can_io_e);
extern void BSP_CANConfigMailbox(BSP_CAN_IO_E can_io, BSP_CAN_MAILBOX_ID_E mailbox_id, BSP_CAN_MAILBOX_CONFIGURE_PTR mailbox_config);


#endif  /* BSP_CAN_H */
