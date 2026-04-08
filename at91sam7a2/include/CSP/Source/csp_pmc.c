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
* File Name       : csp_pmc.c
* Description     : Function declarations for Power Management Controller 
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
* 003  MOD  12/11/01   Christophe GARDIN  Clean Up
*----------------------------------------------------------------------------*/

/******************************************************************************
* Include Files
******************************************************************************/
#include "csp.h"


/******************************************************************************
****************************  F U N C T I O N S  ******************************
******************************************************************************/

/******************************************************************************
* Function          : CSP_PMCEnableClock
* Description       : Enable ARM and/or PDC clock 
* Inputs            : <enable_mask> = Configure which modules (ARM and/or PDC) 
*                     are enabled
* Functions called  : None
* Returns           : None
******************************************************************************/
void CSP_PMCEnableClock(U32_T enable_mask)
{
   /* Enable Clock */
   CSP_PMC_SET_ECR(PMC, enable_mask);
}
 

/******************************************************************************
* Function          : CSP_PMCDisableClock
* Description       : Disable ARM and/or PDC clock 
* Inputs            : <disable_mask> = Configure which modules (ARM and/or PDC) 
*                     are disabled
* Functions called  : None
* Returns           : None
******************************************************************************/
void CSP_PMCDisableClock(U32_T disable_mask)
{
   /* Disable Clock */
   CSP_PMC_SET_DCR(PMC, disable_mask);
}


/******************************************************************************
* Function          : CSP_PMCGetClockStatus
* Description       : Read ARM and PDC clock status
* Inputs            : None 
* Functions called  : None
* Returns           : 32-bit value of ARM and PDC clock status
******************************************************************************/
U32_T CSP_PMCGetClockStatus(void)
{
   /* Read Power Management Status */
   return (CSP_PMC_GET_PMSR(PMC));
}
