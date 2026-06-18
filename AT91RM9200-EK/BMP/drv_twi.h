/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2005
 *
 *    File name   : drv_twi.h
 *    Description : Driver of TWI Definitions
 *
 *    History :
 *    1. Data        : October, 11 2005
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 10 $
**************************************************************************/
#ifndef __DRV_TWI_H
#define __DRV_TWI_H

#include "AT91RM9200.h"
#include "lib_AT91RM9200.h"
#include "arm_comm.h"
#include "init.h"

typedef enum
{
  TwiOk = 0, TwiParametersError, TwiNak, TwiError, TwiBusy,
} Twi_Response_t;

typedef struct
{
  Int8U * pMsg;
  Int32S  DataSize;
  volatile Twi_Response_t TwiStatus;
} Twi_Ctrl_t;

/*************************************************************************
 * Function Name: Twi_Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: TWI Interrupt handler
 *
 *************************************************************************/
void Twi_Handler (void);

/*************************************************************************
 * Function Name: Twi_Init
 * Parameters: Int32U Freq, Int8U AicPriority
 *
 * Return: Twi_Response_t
 *
 * Description: Init TWI controller
 *
 *************************************************************************/
Twi_Response_t Twi_Init (Int32U Freq, Int8U AicPriority);

/*************************************************************************
 * Function Name: Twi_Write
 * Parameters: Int8U SlaveAdd, Int8U Add,
 *             const Int8U * pMsg, Int32U Size
 *
 * Return: Twi_Response_t
 *
 * Description: Write massive to address - Add
 *
 *************************************************************************/
Twi_Response_t Twi_Write (Int8U SlaveAdd, Int8U Add,
                          const Int8U * pMsg, Int32U Size);

/*************************************************************************
 * Function Name: Twi_Read
 * Parameters: Int8U SlaveAdd, Int8U Add, Int8U * pMsg, Int32U Size
 *
 * Return: Twi_Response_t
 *
 * Description: Read sequential from address Add
 *
 *************************************************************************/
Twi_Response_t Twi_Read (Int8U SlaveAdd, Int8U Add,
                         Int8U * pMsg, Int32U Size);

#endif //__DRV_1523_H
