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
* File Name       : csp_gic.c
* Description     : Function declarations for Generic Interrupt Controller
*                   management
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
* 003  MOD  30/10/01   Christophe GARDIN  Clean Up
*----------------------------------------------------------------------------*/

/******************************************************************************
* Include Files
******************************************************************************/
#include "csp.h"


/******************************************************************************
****************************  F U N C T I O N S  ******************************
******************************************************************************/

/******************************************************************************
* Function          : CSP_GICInit
* Description       : Initialize each GIC Source Vector and Source Mode to 
*                     sensitive level and 0 priority
* Input             : None
* Functions called  : None
* Returns           : None
******************************************************************************/
void CSP_GICInit(void)
{
   /* Local Variables */
   U8_T i = 0;
  
   /* Disable & CLear All Interrupts */
   CSP_GIC_SET_IDCR(GIC, 0xFFFFFFFF);
   CSP_GIC_SET_ICCR(GIC, 0xFFFFFFFF);

   /* Reset source vector and source mode registers */
   for(i = 0; i < NB_INTERRUPT; i++)
   {
      CSP_GIC_SET_SVR(GIC, i, 0x0);
      CSP_GIC_SET_SMR(GIC, i, 0x0);
   }
   
   /* Initialize spurious interrupt vector handler */
   CSP_GIC_SET_SPU(GIC, (U32_T) CSP_ASM_GICSpuriousIntHandler);
       
   /* Validate all interrupts levels */
   for(i = 0; i < NB_INTERRUPT_LEVEL; i++)
   {
      CSP_GIC_SET_EOICR(GIC, 0x1);
   }
}


/******************************************************************************
* Function          : CSP_GICConfigInterrupt
* Description       : Configure an interrupt in GIC Module
* Input               <int_num>  : Interrupt number [0-31] to be configured 
*                     <int_mode> : Configure the priority level and source type 
*                     <callback> : Function called through the assembler interrupt handler
* Functions called  : None
* Returns           : None
******************************************************************************/
void CSP_GICConfigInterrupt(U8_T int_num, U32_T int_mode, U32_T callback)
{
   /* Configure interrupt mode in GIC module */
   CSP_GIC_SET_SMR(GIC, int_num, int_mode);
     
   /* Configure interrupt vector in GIC module callback = interrupt handler */
   CSP_GIC_SET_SVR(GIC, int_num, callback);

   /* Enable Interrupt */
   CSP_GIC_SET_IECR(GIC, (0x1ul << int_num));
}


/******************************************************************************
* Function          : CSP_GICUndefinedIntHandler
* Description       : Undefined interrupt (this function should never be called). 
*                     It just unstacks the interrupt
* Input             : None
* Functions called  : None
* Returns           : None
******************************************************************************/
void CSP_GICUndefinedIntHandler(void)
{
   return;
}
