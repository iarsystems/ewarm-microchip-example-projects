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
* File Name       : csp_pio.c
* Description     : Function declarations for Parallel Input/Ouput management
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
* 003  MOD  25/10/01   Christophe GARDIN  Clean Up
* 004  MOD  11/04/02   Christophe GARDIN  Add CSP_PIOClose function
*----------------------------------------------------------------------------*/

/******************************************************************************
* Include Files
******************************************************************************/
#include "csp.h"


/******************************************************************************
****************************  F U N C T I O N S  ******************************
******************************************************************************/

/******************************************************************************
* Function          : CSP_PIOInit
* Description       : Switch on the clock, reset the registers and configure 
*                     the PIO module mode 
* Inputs            : <*pio>            = Pointer to PIO structure
*                     <output_pio>      = Configure which pins are configured as output
*                     <multidriver_pio> = Configure which pin are configured 
*                                         as Multi-Driver (open drain)
* Functions called  : None
* Returns           : None
******************************************************************************/
void CSP_PIOInit(CSP_PIO_T *const pio, U32_T output_pio, U32_T multidriver_pio)
{
   /* Enable PIO block */
   CSP_PIO_SET_ECR(pio, PIO);

   /* PIO Software Reset */
   CSP_PIO_SET_CR(pio, SWRST);

   /* Set Multi-Driver PIO */
   CSP_PIO_SET_MDER(pio, multidriver_pio);

   /* Set Output PIO */
   CSP_PIO_SET_OER(pio, output_pio);
}


/******************************************************************************
* Function          : CSP_PIOClose
* Description       : Reset and switch off the clock
* Inputs            : <*pio> = Pointer to PIO structure
* Functions called  : None
* Returns           : None
******************************************************************************/
void CSP_PIOClose(CSP_PIO_T *const pio)
{
   /* PIO Software Reset */
   CSP_PIO_SET_CR(pio, SWRST);

   /* Disable PIO Clock */  
   CSP_PIO_SET_DCR(pio, PIO);
}


/******************************************************************************
* Function          : CSP_PIOConfigInterrupt
* Description       : Configure PIO Interrupts 
* Inputs            : <*pio>     = Pointer to PIO structure
*                     <int_mode> = Configure the priority level and source type
*                     <int_mask> = Configure which interrupt bits are activated
*                     <callback> = Function called through the assembler interrupt handler
* Functions called  : CSP_GET_INTERRUPT_NUMBER, CSP_GICConfigInterrupt
* Returns           : None
******************************************************************************/
void CSP_PIOConfigInterrupt(CSP_PIO_T *const pio, 
                            U32_T int_mode, 
                            U32_T int_mask, 
                            U32_T callback)
{
   /* Local Variables */
   U8_T int_num = 0;

   /* Get Peripheral Interrupt Number */
   int_num = CSP_GET_INTERRUPT_NUMBER(pio);

   /* Disable all interrupt */
   CSP_PIO_SET_IDR(pio, 0xFFFFFFFF);
   
   /* Interrupt Enable */
   CSP_PIO_SET_IER(pio, int_mask);

   /* Configure PIO controller interrupt mode in GIC module */
   CSP_GICConfigInterrupt(int_num, int_mode, callback);
}


/******************************************************************************
* Function          : CSP_PIOGetStatus
* Description       : Get PIO Pin Data Status 
* Inputs            : <*pio> = Pointer to PIO structure 
* Functions called  : None
* Returns           : 32-bit value of pin data status
******************************************************************************/
U32_T CSP_PIOGetStatus(CSP_PIO_T *const pio)
{
   /* Return PIO State */
   return (CSP_PIO_GET_PDSR(pio));
}


/******************************************************************************
* Function          : CSP_PIOClear
* Description       : Set the PIO to low level 
* Inputs            : <*pio>     = Pointer to PIO structure
*                     <pio_mask> = Configure which pins are set to low level
* Functions called  : None
* Returns           : None
******************************************************************************/
void CSP_PIOClear(CSP_PIO_T *const pio, U32_T pio_mask)
{
   /* Set PIO State */
   CSP_PIO_SET_CODR(pio, pio_mask);
}


/******************************************************************************
* Function          : CSP_PIOSet
* Description       : Set the PIO to high level 
* Inputs            : <*pio>     = Pointer to PIO structure
*                     <pio_mask> = Configure which pins are set to high level
* Functions called  : None
* Returns           : None
******************************************************************************/
void CSP_PIOSet(CSP_PIO_T *const pio, U32_T pio_mask)
{  
   /* Set PIO State */
   CSP_PIO_SET_SODR(pio, pio_mask);
}





