/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2005
 *
 *    File name   : sys_tmr.h
 *    Description : Driver of ISC1523 Definitions
 *
 *    History :
 *    1. Data        : October, 12 2005
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 10 $
**************************************************************************/
#ifndef __SYS_TMR_H
#define __SYS_TMR_H

#include "AT91RM9200.h"
#include "lib_AT91RM9200.h"
#include "arm_comm.h"

/*************************************************************************
 * Function Name: AT91F_ST_HANDLER
 * Parameters: none
 *
 * Return: none
 *
 * Description: Get current count
 *
 *************************************************************************/
void AT91F_ST_HANDLER(void);

/*************************************************************************
 * Function Name: SysTmrInit
 * Parameters: Int32U Resolution [100us]
 *
 * Return: none
 *
 * Description: Init system timer
 *
 *************************************************************************/
void SysTmrInit (Int32U Resolution);

/*************************************************************************
 * Function Name: AT91F_GetTickCount
 * Parameters: none
 *
 * Return: Int32U
 *
 * Description: Get current count
 *
 *************************************************************************/
Int32U AT91F_GetTickCount(void);

/*************************************************************************
 * Function Name: AT91F_ST_SetPeriodIntervalTimer
 * Parameters: none
 *
 * Return: none
 *
 * Description: Get current count
 *
 *************************************************************************/
void AT91F_ST_SetPeriodIntervalTimer( AT91PS_ST pSt, unsigned int period);

#endif //__SYS_TMR_H
