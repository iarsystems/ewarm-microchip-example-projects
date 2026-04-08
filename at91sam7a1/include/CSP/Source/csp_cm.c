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
* File Name       : csp_cm.c
* Description     : Function declarations for Clock Manager management
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
* 004  MOD  21/11/02   Bruno SALLE        Suppression Clock High and Clock Low
                                          Re-definition CM_Init
*----------------------------------------------------------------------------*/

/******************************************************************************
* Include Files
******************************************************************************/
#include "csp.h"


/******************************************************************************
****************************  F U N C T I O N S  ******************************
******************************************************************************/

/******************************************************************************
Function          : CSP_CMInit
Description       : Initialize the clock
Inputs            : None
Functions called  : None
Returns           : None
******************************************************************************/
void CSP_CMInit(void)
{

  /* Disable PLL */
  CSP_CM_SET_CD(CM, PLLSLCT);

  /* PLL = 15 ==> Coreclock = 4Mhz*15/2 = 30 Mhz */
  CSP_CM_SET_PDIV(CM, PLLDIV2 | 13);

  /* Enable PLL */
  CSP_CM_SET_CE(CM, PLLSLCT);
  
}

