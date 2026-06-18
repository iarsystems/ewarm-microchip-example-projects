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
* File Name       : csp_wt.c
* Description     : Function declarations for Watch Timer management
* Library Version : 2.00
* Module Version  : 1.XX
*
*       +----- (NEW | MODify | ADD | DELete)                                 
*       |                                                                    
*  No   |   When       Who                What               
*-----+---+----------+------------------+--------------------------------------
* 000  NEW  01/05/99   Patrice VILCHEZ    Creation
* 001  MOD  01/04/01   Olivier MAZUYER    Clean up
* 002  MOD  08/06/01   Frederic SAMSON    Clean Up 
* 003  MOD  29/10/01   Christophe GARDIN  Clean Up
*----------------------------------------------------------------------------*/

/******************************************************************************
* Include Files
******************************************************************************/
#include "csp.h"


/******************************************************************************
****************************  F U N C T I O N S  ******************************
******************************************************************************/

/******************************************************************************
Function Name     : CSP_WTInit
Description       : Switch on the clock, reset the registers and configure
                    the WT module mode
Inputs            :
- <*wt>  = Pointer to WT structure
- <mode> = Configure when the counter should be reset
Functions called  : None
Returns           : None
******************************************************************************/
void CSP_WTInit(CSP_WT_T *const wt, U32_T mode)
{
   /* WT Software Reset */
   CSP_WT_SET_CR(wt, SWRST);
  
   /* Configure the mode */
   CSP_WT_SET_MR(wt, mode);
}


/******************************************************************************
Function          : CSP_WTConfigInterrupt
Description       : Configure WT Interrupts
Inputs            :
- <*wt>      = Pointer to WT structure
- <int_mode> = Configure the priority level and source type
- <int_mask> = Configure which interrupt bits are activated
- <callback> = Function called through the assembler interrupt handler
Functions called  : 创CSP_GET_INTERRUPT_NUMBER创, 创CSP_GICConfigInterrupt创
Returns           : None
******************************************************************************/
void CSP_WTConfigInterrupt(CSP_WT_T *const wt, 
                           U32_T int_mode, 
                           U32_T int_mask, 
                           U32_T callback)
{
   /* Local Variables */
   U8_T int_num = 0;

   /* Get Peripheral Interrupt Number */
   int_num = CSP_GET_INTERRUPT_NUMBER(wt);

   /* Disable all interrupt */
   CSP_WT_SET_IDR(wt, 0xFFFFFFFF);
   
   /* Interrupt always Enable */
   CSP_WT_SET_IER(wt, int_mask);

   /* Configure WT controller interrupt mode in GIC module */
   CSP_GICConfigInterrupt(int_num, int_mode, callback);
}


/******************************************************************************
Function          : CSP_WTEnable
Description       : Enable WT Seconds Counter and/or Alarm
Inputs            :
- <*wt>         = Pointer to WT structure
- <enable_mask> = Configure which functions (Seconds Counter
  and/or Alarm) are enabled
Functions called  : None
Returns           : None
******************************************************************************/
void CSP_WTEnable(CSP_WT_T *const wt, U8_T enable_mask)
{
   /* Enable WT Seconds Counter and/or Alarm */
   CSP_WT_SET_CR(wt, enable_mask);
}


/******************************************************************************
Function          : CSP_WTDisable
Description       : Disable WT Seconds Counter and/or Alarm
Inputs            :
- <*wt>          = Pointer to WT structure
- <disable_mask> = Configure which functions (Seconds Counter
  and/or Alarm) are disabled
Functions called  : None
Returns           : None
******************************************************************************/
void CSP_WTDisable(CSP_WT_T *const wt, U8_T disable_mask)
{
   /* Disable WT Seconds Counter and/or Alarm */
   CSP_WT_SET_CR(wt, disable_mask);
}


/******************************************************************************
Function          : CSP_WTGetTime
Description       : Read the time passed since the last counter reset
Inputs            : <*wt> = Pointer to WT structure
Functions called  : None
Returns           : 32-bit value of the seconds counter
******************************************************************************/
U32_T CSP_WTGetTime(CSP_WT_T *const wt)
{
   /* Get time in seconds counter */
   return (CSP_WT_GET_SECS(wt));
}


/******************************************************************************
Function          : CSP_WTSetTime
Description       : Configure the seconds counter time
Inputs            :
- <*wt>  = Pointer to WT structure
- <time> = Configure the time value
Functions called  : None
Returns           : None
******************************************************************************/
void CSP_WTSetTime(CSP_WT_T *const wt, U32_T time)
{
   /* Set time in seconds counter */
   CSP_WT_SET_SECS(wt, time);
}


/******************************************************************************
Function          : CSP_WTGetAlarm
Description       : Read the Alarm time
Inputs            : <*wt> = Pointer to WT structure
Functions called  : None
Returns           : None
******************************************************************************/
U32_T CSP_WTGetAlarm(CSP_WT_T *const wt)
{
   /* Get Alarm time */
   return(CSP_WT_GET_ALARM(wt));
}


/******************************************************************************
Function          : CSP_WTSetAlarm
Description       : Configure the Alarm time. An interrupt can be generated
                    when the seconds register reaches this value
Inputs            :
- <*wt>   = Pointer to WT structure
- <alarm> = Configure the alarm time
Functions called  : None
Returns           : None
******************************************************************************/
void CSP_WTSetAlarm(CSP_WT_T *const wt, U32_T alarm)
{
   /* Set Alarm time */
   CSP_WT_SET_ALARM(wt, alarm);
}


