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
* File Name       : csp_capt.c
* Description     : Function declarations for Capture management
* Library Version : 2.00
* Module Version  : 1.XX
*
*       +----- (NEW | MODify | ADD | DELete)                                 
*       |                                                                    
*  No   |   When       Who                What               
*-----+---+----------+------------------+--------------------------------------
* 000  NEW  01/05/99   Patrice VILCHEZ    Creation
* 001  MOD  01/04/01   David COMBE        Introduce CSP_CAPTConfigInterrupt 
                                          function
                                          Define Global CSP_CAPTCounter Variable
* 002  MOD  01/04/01   Olivier MAZUYER    Clean up
* 003  MOD  08/06/01   Frederic SAMSON    Clean Up
* 004  MOD  12/11/01   Christophe GARDIN  Clean Up
* 005  MOD  11/04/02   Christophe GARDIN  Add CSP_CAPTClose function
* 006  MOD  21/11/02   Bruno SALLE        Disable PIO block
*----------------------------------------------------------------------------*/

/******************************************************************************
* Include Files
******************************************************************************/
#include "csp.h"


/******************************************************************************
****************************  F U N C T I O N S  ******************************
******************************************************************************/

/******************************************************************************
Function          : CSP_CAPTInit
Description       : Switch on the clock, reset the registers and configure the
                    CAPTURE module mode and the PDC RX line
Inputs            :
- <*capt> = Pointer to CAPTURE structure
- <mode>  = Configure the CAPTURE mode
Functions called  : 创CSP_PDCInitRx创
Returns           : None
******************************************************************************/
void CSP_CAPTInit(CSP_CAPT_T *const capt, U32_T mode)
{

  /* Enable CAP and PIO Clock */
   CSP_CAPT_SET_ECR(capt, CAP | PIO);

   /* CAPTURE Software Reset */
   CSP_CAPT_SET_CR(capt, SWRST);
 
   /* Disable PIO */
   CSP_CAPT_SET_PDR(capt, CAPTPIN);

   /* Disable PIO Clock */
   CSP_CAPT_SET_DCR(capt, PIO);

   /* Configure CAPTURE Mode */
   CSP_CAPT_SET_MR(capt, mode);
     
   /* Configure CAPTURE PDC Rx Line */
   CSP_PDCInitRx((U32_T) capt, (U32_T) &(capt->DR), SIZE_HALFWORD);
}


/******************************************************************************
Function          : CSP_CAPTClose
Description       : Reset and switch off the clock
Inputs            : <*capt> = Pointer to CAPTURE structure
Functions called  : None
Returns           : None
******************************************************************************/
void CSP_CAPTClose(CSP_CAPT_T *const capt)
{
   /* CAPTURE Software Reset */
   CSP_CAPT_SET_CR(capt, SWRST);

   /* Disable CAPTURE Clock */  
   CSP_CAPT_SET_DCR(capt, CAP);
}


/******************************************************************************
Function          : CSP_CAPTConfigInterrupt
Description       : Configure CAPTURE Interrupts
Inputs            :
- <*capt>    = Pointer to CAPTURE structure
- <int_mode> = Configure the priority level and source type
- <int_mask> = Configure which interrupt bits are activated
- <callback> = Function called through the assembler interrupt handler
Functions called  : 创CSP_GET_INTERRUPT_NUMBER创, 创CSP_GICConfigInterrupt创
Returns           : None
******************************************************************************/
void CSP_CAPTConfigInterrupt(CSP_CAPT_T *const capt, 
                             U32_T int_mode, 
                             U32_T int_mask, 
                             U32_T callback)
{
   /* Local Variables */
   U8_T int_num = 0;

   /* Get Peripheral Interrupt Number */
   int_num = CSP_GET_INTERRUPT_NUMBER(capt);

   /* Disable all interrupt */
   CSP_CAPT_SET_IDR(capt, 0xFFFFFFFF);
      
   /* Interrupt Enable */
   CSP_CAPT_SET_IER(capt, int_mask);

   /* Configure CAPTURE controller interrupt mode in GIC module */
   CSP_GICConfigInterrupt(int_num, int_mode, callback);
}


/******************************************************************************
Function          : CSP_CAPTEnable
Description       : Enable CAPTURE
Inputs            : <*capt> = Pointer to CAPTURE structure
Functions called  : None
Returns           : None
******************************************************************************/
void CSP_CAPTEnable(CSP_CAPT_T *const capt)
{
   /* Enable CAPTURE */
   CSP_CAPT_SET_CR(capt, CAPEN);
}


/******************************************************************************
Function          : CSP_CAPTDisable
Description       : Disable CAPTURE
Inputs            : <*capt> = Pointer to CAPTURE structure
Functions called  : None
Returns           : None
******************************************************************************/
void CSP_CAPTDisable(CSP_CAPT_T *const capt)
{
   /* Disable CAPTURE */
   CSP_CAPT_SET_CR(capt, CAPDIS);
}


/******************************************************************************
Function          : CSP_CAPTStart
Description       : Start CAPTURE
Inputs            : <*capt> = Pointer to CAPTURE structure
Functions called  : None
Returns           : None
******************************************************************************/
void CSP_CAPTStart(CSP_CAPT_T *const capt)
{
   /* Start CAPTURE */
   CSP_CAPT_SET_CR(capt, STARTCAPT);
} 



/******************************************************************************
Function          : CSP_CAPTPioInit
Description       : Configure CAPT PIO
Inputs            :
- <*capt>     = Pointer to CAPT structure
- <pio_mask>   = Configure which pins are activated
- <output_pio> = Configure which pins are configured as output
Functions called  : None
Returns           : None
******************************************************************************/
void CSP_CAPTPioInit(CSP_CAPT_T *const capt, U32_T pio_mask, U32_T output_pio)
{
   /* Enable PIO block */
   CSP_CAPT_SET_ECR(capt, PIO);

   /* Disable all PIO */
   CSP_CAPT_SET_PDR(capt, 0xFFFFFFFF);
   CSP_CAPT_SET_ODR(capt, 0xFFFFFFFF);

   /* Enable PIO */
   CSP_CAPT_SET_PER(capt, pio_mask);
  
   /* Set Output PIO */
   CSP_CAPT_SET_OER(capt, output_pio);
}


/******************************************************************************
Function          : CSP_CAPTPioGetStatus
Description       : Read the pin data status
Inputs            : <*capt> = Pointer to CAPT structure
Functions called  : None
Returns           : 32-bit value of pin data status
******************************************************************************/
U32_T CSP_CAPTPioGetStatus(CSP_CAPT_T *const capt)
{
   /* Return PIO State */
   return (CSP_CAPT_GET_PDSR(capt));
}


/******************************************************************************
Function          : CSP_CAPTPioClear
Description       : Set the PIO to low level
Inputs            :
- <*capt>   = Pointer to CAPT structure
- <pio_mask> = Configure which pins are set to low level
Functions called  : None
Returns           : None
******************************************************************************/
void CSP_CAPTPioClear(CSP_CAPT_T *const capt, U32_T pio_mask)
{
   /* Set PIO State */
   CSP_CAPT_SET_CODR(capt, pio_mask);
}


/******************************************************************************
Function          : CSP_CAPTPioSet
Description       : Set the PIO to high level
Inputs            :
- <*capt>   = Pointer to CAPT structure
- <pio_mask> = Configure which pins are set to high level
Functions called  : None
Returns           : None
******************************************************************************/
void CSP_CAPTPioSet(CSP_CAPT_T *const capt, U32_T pio_mask)
{
   /* Set PIO State */
   CSP_CAPT_SET_SODR(capt, pio_mask);
}
