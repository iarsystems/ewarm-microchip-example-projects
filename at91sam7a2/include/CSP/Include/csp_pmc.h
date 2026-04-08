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
* File Name       : csp_pmc.h
* Description     : Definitions, Macros and function declarations for
*                   Power Management Controller module
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
* 003  MOD  19/10/01   Christophe GARDIN  Clean Up
*----------------------------------------------------------------------------*/

#ifndef CSP_PMC_H
#define CSP_PMC_H


/******************************************************************************
************************* PMC Structure Definition ****************************
******************************************************************************/
typedef struct 
{
   CSP_REGISTER_T  Reserved[20];
   CSP_REGISTER_T  ECR;              /* Enable Clock Register                */
   CSP_REGISTER_T  DCR;              /* Disable Clock Register               */
   CSP_REGISTER_T  PMSR;             /* Power Management Status Register     */
} CSP_PMC_T, *CSP_PMC_PTR;


/******************************************************************************
************************** PMC Registers Definition ***************************
******************************************************************************/

/******************************************************************************
* ECR, DCR, PMSR : PMC Power Management Registers 
******************************************************************************/
#define PMC_ARM      (0x01ul << 0)      /* Core Clock                        */
#define PMC_PDC      (0x01ul << 1)      /* PDC Clock                         */


/******************************************************************************
*************************** PMC Macros Definition *****************************
******************************************************************************/
/* ECR, DCR, PMSR : PMC Power Management Registers                           */
#define CSP_PMC_SET_ECR(pmc, val)   ((pmc)->ECR = (val))    /* Enable        */
#define CSP_PMC_SET_DCR(pmc, val)   ((pmc)->DCR = (val))    /* Disable       */
#define CSP_PMC_GET_PMSR(pmc)       ((pmc)->PMSR)           /* Status        */


/******************************************************************************
********************* PMC External Functions Declaration **********************
******************************************************************************/
extern void CSP_PMCEnableClock(U32_T enable_mask);
extern void CSP_PMCDisableClock(U32_T disable_mask);
extern U32_T CSP_PMCGetClockStatus(void);


#endif   /* CSP_PMC_H */

