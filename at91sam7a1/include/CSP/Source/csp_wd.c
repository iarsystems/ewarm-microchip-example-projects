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
* File Name       : csp_wd.c
* Description     : Function declarations for WatchDog management
* Library Version : 2.01
* Module Version  : 1.XX
*
*       +----- (NEW | MODify | ADD | DELete)                                 
*       |                                                                    
*  No   |   When       Who                What               
*-----+---+----------+------------------+--------------------------------------
* 000  NEW  01/05/99   Patrice VILCHEZ    Creation
* 001  MOD  01/04/01   Olivier MAZUYER    Clean up
* 002  MOD  08/06/01   Frederic SAMSON    Clean Up 
* 003  MOD  30/10/01   Christophe GARDIN  Clean Up
* 004  MOD  23/09/02   Christophe GARDIN  Fix bug on WDDisable function
*----------------------------------------------------------------------------*/

/******************************************************************************
* Include Files
******************************************************************************/
#include "csp.h"


/******************************************************************************
****************************  F U N C T I O N S  ******************************
******************************************************************************/

/******************************************************************************
Function          : CSP_WDInit
Description       : Configure the WD module mode
Inputs            :
- <*wd>  = Pointer to WD structure
- <mode> = Configure the mode of running and especially
  the clock selection
- <overflow_mode> = Configure the WatchDog Overflow Mode
Functions called  : None
Returns           : None
******************************************************************************/
void CSP_WDInit(CSP_WD_T *const wd, U32_T mode, U32_T overflow_mode)
{
   /* Write Access Allowed in MR + Define Clock */
   CSP_WD_SET_MR(wd, (CKEY_ALLOW | mode));

   /* Set Overflow Mode */
   CSP_WD_SET_OMR(wd, (OKEY_ALLOW | overflow_mode));
}


/******************************************************************************
Function          : CSP_WDConfigInterrupt
Description       : Configure WD Interrupts
Inputs            :
- <*wd>      = Pointer to WD structure
- <int_mode> = Configure the priority level and source type
- <int_mask> = Configure which interrupt bits are activated
- <callback> = Function called through the assembler interrupt handler
Functions called  : 创CSP_GET_INTERRUPT_NUMBER创, 创CSP_GICConfigInterrupt创
Returns           : None
******************************************************************************/
void CSP_WDConfigInterrupt(CSP_WD_T *const wd, 
                           U32_T int_mode, 
                           U32_T int_mask, 
                           U32_T callback)
{
   /* Local Variables */
   U8_T int_num = 0;

   /* Get Peripheral Interrupt Number */
   int_num = CSP_GET_INTERRUPT_NUMBER(wd);

   /* Disable all interrupt */
   CSP_WD_SET_IDR(wd, 0xFFFFFFFF);
   
   /* Interrupt always Enable */
   CSP_WD_SET_IER(wd, int_mask);

   /* Configure WD controller interrupt mode in GIC module */
   CSP_GICConfigInterrupt(int_num, int_mode, callback);
}


/******************************************************************************
Function          : CSP_WDEnable
Description       : Enable WatchDog
Inputs            : <*wd> = Pointer to WD structure
Functions called  : None
Returns           : None
******************************************************************************/
void CSP_WDEnable(CSP_WD_T *const wd)
{
   /* Write Access Allowed in MR + Enable WatchDog */
   CSP_WD_SET_OMR(wd, (CSP_WD_GET_OMR(wd) | (OKEY_ALLOW | WDEN)));
}


/******************************************************************************
Function          : CSP_WDDisable
Description       : Disable WatchDog
Inputs            : <*wd> = Pointer to WD structure
Functions called  : None
Returns           : None
******************************************************************************/
void CSP_WDDisable(CSP_WD_T *const wd)
{
   /* Write Access Allowed in MR + Disable WatchDog */
   CSP_WD_SET_OMR(wd, ((CSP_WD_GET_OMR(wd) & (~WDEN)) | OKEY_ALLOW));
}


/******************************************************************************
Function          : CSP_WDCounterRestart
Description       : Restart Watchdog Counter
Inputs            : <*wd> = Pointer to WD structure
Functions called  : None
Returns           : None
******************************************************************************/
void CSP_WDCounterRestart(CSP_WD_T *const wd)
{
   /* Restart Key */
   CSP_WD_SET_CR(wd, RSTKEY);
}
