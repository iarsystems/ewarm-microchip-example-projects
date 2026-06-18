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
* File Name       : csp_can.c
* Description     : Function declarations for Controller Area Network management
* Library Version : 2.00
* Module Version  : 1.XX
*
*       +----- (NEW | MODify | ADD | DELete)                                 
*       |                                                                    
*  No   |   When       Who                What               
*-----+---+----------+------------------+--------------------------------------
* 000  NEW  01/05/99   Patrice VILCHEZ    Creation
* 001  MOD  01/04/01   Olivier MAZUYER    Clean up
* 002  MOD  23/05/01   Tristan BONHOMME   Clean up
* 003  MOD  26/05/01   Patrice VILCHEZ    Set correct ID in SendData function
*                                         Correct SPY mode in ReceiveData
*                                         function
*                                         Use memcpy function in ReceiveData
*                                         function
* 004  MOD  19/11/01   Christophe GARDIN  Clean Up
* 005  MOD  11/04/02   Christophe GARDIN  Add CSP_CANClose function
*----------------------------------------------------------------------------*/

/******************************************************************************
* Include Files
******************************************************************************/
#include "csp.h"


/******************************************************************************
****************************  F U N C T I O N S  ******************************
******************************************************************************/

/******************************************************************************
Function          : CSP_CANInit
Description       : Switch on the clock, reset the registers and wait for channels
                    registers initialization (8 * nb channels * core clock period)
                    and configure the CAN module mode
Inputs            :
- <*can> = Pointer to CAN structure
- <mode> = Configure the CAN mode
Functions called  : None
Returns           : None
******************************************************************************/
void CSP_CANInit(CSP_CAN_T *const can, U32_T mode)
{
   /* Local Variables */
   U16_T i = 0;

   /* Enable CAN Clock */  
   CSP_CAN_SET_ECR(can, CAN);
  
   /* CAN Software Reset */  
   CSP_CAN_SET_CR(can, SWRST);
  
   /* Wait for channels registers initialization */
   /* This avoid using the interrupt that should take more time to process */
   for(i = 0; i < (8u * 32u); i++)
   {
   }

   /* Configure CAN Mode */
   CSP_CAN_SET_MR(can, mode);
}


/******************************************************************************
Function          : CSP_CANClose
Description       : Reset and switch off the clock
Inputs            : <*can> = Pointer to CAN structure
Functions called  : None
Returns           : None
******************************************************************************/
void CSP_CANClose(CSP_CAN_T *const can)
{
   /* CAN Software Reset */  
   CSP_CAN_SET_CR(can, SWRST);

   /* Disable CAN Clock */  
   CSP_CAN_SET_DCR(can, CAN);
}


/******************************************************************************
Function          : CSP_CANConfigInterrupt
Description       : Configure CAN Interrupts
Inputs            :
- <*can>     = Pointer to CAN structure
- <int_mode> = Configure the priority level and source type
- <int_mask> = Configure which interrupt bits are activated
- <callback> = Function called through the assembler interrupt handler
Functions called  : 创CSP_GET_INTERRUPT_NUMBER创, 创CSP_GICConfigInterrupt创
Returns           : None
******************************************************************************/
void CSP_CANConfigInterrupt(CSP_CAN_T *const can, 
                            U32_T int_mode, 
                            U32_T int_mask, 
                            U32_T callback)
{
   /* Local Variables */
   U8_T int_num = 0;

   /* Get Peripheral Interrupt Number */
   int_num = CSP_GET_INTERRUPT_NUMBER(can);

   /* Disable and clear all interrupt */
   CSP_CAN_SET_IDR(can, 0xFFFFFFFF);
   CSP_CAN_SET_SIDR(can, 0xFFFFFFFF);
   CSP_CAN_SET_CISR(can, 0xFFFFFFFF);

   /* Interrupt Enable in Peripheral */
   CSP_CAN_SET_IER(can, int_mask);

   /* Configure CAN controller interrupt mode in GIC module */
   CSP_GICConfigInterrupt(int_num, int_mode, callback);
}
                     

/******************************************************************************
Function          : CSP_CANChannelConfigInterrupt
Description       : Configure/Enable CAN Channel Interrupt
Inputs            :
- <*can>     = Pointer to CAN structure
- <channel>  = Channel [0-31] to be configured
- <int_mask> = Configure which channel interrupt bits are activated
Functions called  : None
Returns           : None
******************************************************************************/
void CSP_CANChannelConfigInterrupt(CSP_CAN_T *const can, 
                                   U8_T channel, 
                                   U32_T int_mask)
{
   /* Set Channel Interrupt Enable */ 
   CSP_CAN_CHANNEL_SET_IER(can, channel, int_mask);

   /* Set Channel Interrupt Source in CAN Controller */
   CSP_CAN_SET_SIER(can, (CSP_CAN_GET_SIMR(can) | (0x1ul << channel)));
}


/******************************************************************************
Function          : CSP_CANEnable
Description       : Enable CAN (12 祍 is necessary at 30MHz after an enable
                    command to transfer and receive data)
Inputs            : <*can> = Pointer to CAN structure
Functions called  : None
Returns           : None
******************************************************************************/
void CSP_CANEnable(CSP_CAN_T *const can)
{
   /* Enable CAN */
   CSP_CAN_SET_CR(can, CANEN);
}  


/******************************************************************************
Function          : CSP_CANDisable
Description       : Disable CAN
Inputs            : <*can> = Pointer to CAN structure
Functions called  : None
Returns           : None
******************************************************************************/
void CSP_CANDisable(CSP_CAN_T *const can)
{
   /* Disable CAN */
   CSP_CAN_SET_CR(can, CANDIS);
}  


/******************************************************************************
Function          : CSP_CANReceive
Description       : Configure a CAN channel to pick up message from the network
                    User defines if the message waited is a remote frame or a
                    data frame.
Inputs            :
- <*can>    = Pointer to CAN structure
- <channel> = Receiver channel [0-31]
- <id>      = Configure the identifier and the remote request
- <mask>    = Configure the mask and the remote request
- <control> = Configure the number of byte to receive and the control bits
Functions called  : None
Returns           : None
******************************************************************************/
void CSP_CANReceive(CSP_CAN_T *const can, 
                    U8_T channel, 
                    U32_T id,
                    U32_T mask, 
                    U8_T control)
{
   if((control & IDE) == IDE)
   {
      /* Set Id Register (29 bits length) */
      CSP_CAN_CHANNEL_SET_IR(can, channel, (((id & 0x1FFC0000ul) >> 18) | 
                                            ((id & 0x3FFFFul) << 11)) | 
                                             (id & 0xE0000000ul));
      /* Set Mask Register (29 bits length) */
      CSP_CAN_CHANNEL_SET_MSK(can, channel, (((mask & 0x1FFC0000ul) >> 18) | 
                                             ((mask & 0x3FFFFul) << 11)) |
                                              (mask & 0xE0000000ul));
   }                                          
   else
   {
      /* Set Id Register (11 bits length) */
      CSP_CAN_CHANNEL_SET_IR(can, channel, (id & 0xE00007FFul));
      /* Set Mask Register (11 bits length) */
      CSP_CAN_CHANNEL_SET_MSK(can, channel, (mask & 0xE00007FFul));
   }

   /* Set the length and the receive command */
   CSP_CAN_CHANNEL_SET_CR(can, channel, (CHANEN | control));
}


/******************************************************************************
Function          : CSP_CANTransmit
Description       : Configure a CAN channel to transmit message on the network.
                    The message can be a remote frame or a data frame.
Inputs            :
- <*can>    = Pointer to CAN structure
- <channel> = Transmitter channel [0-31]
- <id>      = Configure the identifier and the remote request
- <*data>   = Pointer to an array where data packet sent is stored
- <control> = Configure the number of byte to transmit and the control bits
Functions called  : None
Returns           : None
******************************************************************************/
void CSP_CANTransmit(CSP_CAN_T *const can, 
                     U8_T channel, 
                     U32_T id, 
                     U8_T *data, 
                     U16_T control)
{
   /* Local Variables */
   U8_T i = 0; 
   U8_T length = (control & 0xF);
   U8_T dra_drb[8] = {0, 0, 0, 0, 0, 0, 0, 0};     /* Clear DRA and DRB */

   /* Set Id Register */
   if((control & IDE) == IDE)
   {
      /* Set Id Register (29 bits length) */
      CSP_CAN_CHANNEL_SET_IR(can, channel, (((id & 0x1FFC0000ul) >> 18) | 
                                            ((id & 0x3FFFFul) << 11)) |
                                             (id & 0xE0000000ul));
   }
   else
   {
      /* Set Id Register (11 bits length) */
      CSP_CAN_CHANNEL_SET_IR(can, channel, (id & 0xE00007FFul));
   }

   /* No copy if remote transmission request */
   if((CSP_CAN_CHANNEL_GET_IR(can, channel) & RTR) == 0)
   {
      /* Copy N bytes in DRA and DRB to Send */
      for(i = 0; i < length; i++)
      {
         dra_drb[i] = data[i];
      }
 
      /* The writing in DRA and DRB must be in 32 bits */
      CSP_CAN_CHANNEL_SET_DRA(can, channel, (*(U32_T*)&dra_drb[0]));
      CSP_CAN_CHANNEL_SET_DRB(can, channel, (*(U32_T*)&dra_drb[4]));
   }
                                       
   /* Set the length and the transmit command */
   CSP_CAN_CHANNEL_SET_CR(can, channel, (CHANEN | PCB | control));
}

