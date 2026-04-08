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
* File Name       : csp_st.c
* Description     : Function declarations for Simple Timer management
* Library Version : 2.00
* Module Version  : 1.XX
*
*       +----- (NEW | MODify | ADD | DELete)                                 
*       |                                                                    
*  No   |   When       Who                What               
*-----+---+----------+------------------+--------------------------------------
* 000  NEW  01/05/99   Patrice VILCHEZ    Creation
* 001  MOD  11/08/00   David COMBE        Rename ST
*                                         Update of this file with new notations
* 002  MOD  01/04/01   Olivier MAZUYER    Clean up
* 003  MOD  08/06/01   Frederic SAMSON    Clean Up 
* 004  MOD  06/08/01   Frederic SAMSON    Modified for new specifications of
*                                         CSR registers 
* 005  MOD  05/11/01   Christophe GARDIN  Clean Up
* 006  MOD  18/03/02   Christophe GARDIN  Modify CSP_STInit function and add 
*                                         CSP_STConfigureChannel function
* 007  MOD  11/04/02   Christophe GARDIN  Add CSP_STClose function
*----------------------------------------------------------------------------*/

/******************************************************************************
* Include Files
******************************************************************************/
#include "csp.h"


/******************************************************************************
****************************  F U N C T I O N S  ******************************
******************************************************************************/

/******************************************************************************
Function          : CSP_STInit
Description       : Switch on the clock and reset the registers
Inputs            : <*st> = Pointer to ST structure
Functions called  : None
Returns           : None
******************************************************************************/
void CSP_STInit(CSP_ST_T *const st)
{
   /* Enable ST Clock */
   CSP_ST_SET_ECR(st, ST);

   /* ST Software Reset */
   CSP_ST_SET_CR(st, SWRST);
}


/******************************************************************************
Function          : CSP_STClose
Description       : Reset and switch off the clock
Inputs            : <*st> = Pointer to ST structure
Functions called  : None
Returns           : None
******************************************************************************/
void CSP_STClose(CSP_ST_T *const st)
{
   /* ST Software Reset */
   CSP_ST_SET_CR(st, SWRST);

   /* Disable ST Clock */
   CSP_ST_SET_DCR(st, ST);
}


/******************************************************************************
Function          : CSP_STConfigureChannel
Description       : Configure the ST channel
Inputs            :
- <*st>          = Pointer to ST structure
- <channel>      = Channel [0-1] to be configured
- <prescal_mode> = Configure the channel prescalar mode
- <load_counter> = Configure the counter value
Functions called  : None
Returns           : None
******************************************************************************/
void CSP_STConfigureChannel(CSP_ST_T *const st, 
                            U8_T channel, 
                            U32_T prescal_mode, 
                            U32_T load_counter)
{
   /* Configure Counter Value */
   CSP_ST_CHANNEL_SET_CT(st, channel, load_counter);
        
   /* Configure Prescalar Value */
   CSP_ST_CHANNEL_SET_PR(st, channel, prescal_mode);
}


/******************************************************************************
Function          : CSP_STConfigInterrupt
Description       : Configure ST Interrupts
Inputs            :
- <*st>      = Pointer to ST structure
- <int_mode> = Configure the priority level and source type
- <int_mask> = Configure which interrupt bits are activated
- <callback> = Function called through the assembler interrupt handler
Functions called  : 创CSP_GET_INTERRUPT_NUMBER创, 创CSP_GICConfigInterrupt创
Returns           : None
******************************************************************************/
void CSP_STConfigInterrupt(CSP_ST_T *const st, 
                           U32_T int_mode, 
                           U32_T int_mask, 
                           U32_T callback)
{
   /* Local Variables */
   U8_T int_num = 0;

   /* Get Peripheral Interrupt Number */
   int_num = CSP_GET_INTERRUPT_NUMBER(st);

   /* Disable all interrupt */
   CSP_ST_SET_IDR(st, 0xFFFFFFFF);
      
   /* Interrupt Enable */
   CSP_ST_SET_IER(st, int_mask);

   /* Configure ST controller interrupt mode in GIC module */
   CSP_GICConfigInterrupt(int_num, int_mode, callback);
}


/******************************************************************************
Function          : CSP_STEnable
Description       : Enable ST Channel
Inputs            :
- <*st>     = Pointer to ST structure
- <channel> = Channel [0-1] to be enabled
Functions called  : None
Returns           : None
******************************************************************************/
void CSP_STEnable(CSP_ST_T *const st, U8_T channel)
{
   /* Enable ST Channel */
   CSP_ST_SET_CR(st, (CHEN <<  (2u * channel)));
}
 

/******************************************************************************
Function          : CSP_STDisable
Description       : Disable ST Channel
Inputs            :
- <*st>     = Pointer to ST structure
- <channel> = Channel [0-1] to be disabled
Functions called  : None
Returns           : None
******************************************************************************/
void CSP_STDisable(CSP_ST_T *const st, U8_T channel)
{
   /* Disable ST Channel */
   CSP_ST_SET_CR(st, (CHDIS << (2u * channel)));
}


/******************************************************************************
Function          : CSP_STReload
Description       : Reload ST
Inputs            :
- <*st>     = Pointer to ST structure
- <channel> = Channel [0-1] to be configured
- <counter> = Configure counter value
Functions called  : None
Returns           : None
******************************************************************************/
void CSP_STReload(CSP_ST_T *const st, U8_T channel, U16_T counter)
 {  
   /* Disable Simple Timer */
   CSP_ST_SET_CR(st, (CHDIS << (2u * channel)));
   CSP_ST_SET_CSR(st, (CHEND << (3u * channel)));

   /* Set Counter Value */
   CSP_ST_CHANNEL_SET_CT(st, channel, counter);
   CSP_ST_SET_CR(st, (CHEN << (2u * channel)));
}

