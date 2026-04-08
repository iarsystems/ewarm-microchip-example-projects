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
* File Name       : csp_amc.c
* Description     : Function declarations for Advanced Memory Controller 
                    management
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
* 003  MOD  09/10/01   Christophe GARDIN  Clean Up
*----------------------------------------------------------------------------*/

/******************************************************************************
* Include Files
******************************************************************************/
#include "csp.h"


/******************************************************************************
****************************  F U N C T I O N S  ******************************
******************************************************************************/

/******************************************************************************
* Function          : CSP_MCGetCSRegister
* Description       : Read the specified Chip Select configuration
* Inputs            : <cs> = Chip Select [0-2] to be read
* Functions called  : None
* Returns           : 32-bit value of the Chip Select configuration  
******************************************************************************/
U32_T CSP_AMCGetCSRegister(U16_T cs)
{
   /* Get CS mode */
   return (CSP_AMC_GET_CSR(AMC, cs));
} 


/******************************************************************************
* Function          : CSP_AMCSetCSRegister
* Description       : Configure the Chip Select 
* Inputs            : <cs>   = Chip Select [0-2] to be configured
*                     <mode> = Configure the Chip Select
* Functions called  : None
* Returns           : None
******************************************************************************/
void CSP_AMCSetCSRegister(U16_T cs, U32_T mode)
{
   /* Set CS mode */
   CSP_AMC_SET_CSR(AMC, cs, mode);
}


/******************************************************************************
* Function          : CSP_AMCRemapControl
* Description       : Cancel the remapping (performed at reset) of the two 
*                     memory devices (Internal RAM and external memory on NCS0)
* Inputs            : None    
* Functions called  : None
* Returns           : None
******************************************************************************/
void CSP_AMCRemapControl(void)
{
   /* Defines the cancel remapping */
   CSP_AMC_SET_RCR(AMC, RCB);
}


/******************************************************************************
* Function          : CSP_AMCSetMemoryControl
* Description       : Configure the Memory Control
* Inputs            : <val> = Configure the memory control options
* Functions called  : None
* Returns           : None
******************************************************************************/
void CSP_AMCSetMemoryControl(U32_T val)
{
   /* Set Memory Control options */
   CSP_AMC_SET_MCR(AMC, val);
}
