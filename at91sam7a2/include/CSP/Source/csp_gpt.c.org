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
* File Name       : csp_gpt.c
* Description     : Function declarations for General Purpose Timer management
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
* 004  MOD  11/04/02   Christophe GARDIN  Add CSP_GPTClose function
*----------------------------------------------------------------------------*/

/******************************************************************************
* Include Files
******************************************************************************/
#include "csp.h"


/******************************************************************************
****************************  F U N C T I O N S  ******************************
******************************************************************************/

/******************************************************************************
* Function          : CSP_GPTInit
* Description       : Switch on the clock, reset the registers and configure 
*                     the GPT module mode
* Inputs            : <*gpt>       = Pointer to GPT structure
*                     <mode>       = Configure the GPT Mode
*                     <*reg_value> = Pointer on array where counter values of 
*                                    RA, RB and RC will be defined
* Functions called  : None
* Returns           : None
******************************************************************************/
void CSP_GPTInit(CSP_GPT_T *const gpt, U32_T mode, U16_T *reg_value)
{
   /* Enable GPT Clock */
   CSP_GPT_SET_ECR(gpt, (TC | PIO));

   /* GPT Software Reset */
   CSP_GPT_SET_CR(gpt, SWRST);

   /* Disable GPT PIO */
   CSP_GPT_SET_PDR(gpt, (TCLK | TIOA | TIOB));
   
   /* Configure GPT Mode */
   CSP_GPT_SET_MR(gpt, mode);

   /* Set Registers Value */
   CSP_GPT_SET_RA(gpt, reg_value[0]);
   CSP_GPT_SET_RB(gpt, reg_value[1]);
   CSP_GPT_SET_RC(gpt, reg_value[2]);
}


/******************************************************************************
* Function          : CSP_GPTClose
* Description       : Reset and switch off the clock
* Inputs            : <*gpt> = Pointer to GPT structure
* Functions called  : None
* Returns           : None
******************************************************************************/
void CSP_GPTClose(CSP_GPT_T *const gpt)
{
   /* GPT Software Reset */
   CSP_GPT_SET_CR(gpt, SWRST);

   /* Disable GPT Clock */  
   CSP_GPT_SET_DCR(gpt, (TC | PIO));
}


/******************************************************************************
* Function          : CSP_GPTConfigInterrupt
* Description       : Configure GPT Interrupts
* Inputs            : <*gpt>     = Pointer to GPT structure
*                     <int_mode> = Configure the priority level and source type
*                     <int_mask> = Configure which interrupt bits are activated
*                     <callback> = Function called through the assembler interrupt handler
* Functions called  : CSP_GET_INTERRUPT_NUMBER, CSP_GICConfigInterrupt
* Returns           : None
******************************************************************************/
void CSP_GPTConfigInterrupt(CSP_GPT_T *const gpt, 
                            U32_T int_mode, 
                            U32_T int_mask, 
                            U32_T callback)
{
   /* Local Variables */
   U8_T int_num = 0;

   /* Get Peripheral Interrupt Number */
   int_num = CSP_GET_INTERRUPT_NUMBER(gpt);
   int_num += (U8_T)(((U32_T)gpt & 0xFFFul) >> 8u);

   /* Disable all interrupt */
   CSP_GPT_SET_IDR(gpt, 0xFFFFFFFF);
   
   /* Interrupt Enable */
   CSP_GPT_SET_IER(gpt, int_mask);

   /* Configure GPT controller interrupt mode in GIC module */
   CSP_GICConfigInterrupt(int_num, int_mode, callback);
}


/******************************************************************************
* Function          : CSP_GPTEnable
* Description       : Enable GPT
* Inputs            : <*gpt> = Pointer to GPT structure
* Functions called  : None
* Returns           : None
******************************************************************************/
void CSP_GPTEnable(CSP_GPT_T *const gpt)
{
   /* Enable GPT */ 
   CSP_GPT_SET_CR(gpt, CLKEN | SWTRG);
}


/******************************************************************************
* Function          : CSP_GPTDisable
* Description       : Disable GPT
* Inputs            : <*gpt> = Pointer to GPT structure
* Functions called  : None
* Returns           : None
******************************************************************************/
void CSP_GPTDisable(CSP_GPT_T *const gpt)
{
   /* Disable GPT */
   CSP_GPT_SET_CR(gpt, CLKDIS);
}


/******************************************************************************
* Function          : CSP_GPTPioInit
* Description       : Configure GPT PIO
* Inputs            : <*gpt>       = Pointer to GPT structure
*                     <pio_mask>   = Configure which pins are actived
*                     <output_pio> = Configure which pins are configured as output
* Functions called  : None
* Returns           : None
******************************************************************************/
void CSP_GPTPioInit(CSP_GPT_T *const gpt, U32_T pio_mask, U32_T output_pio)
{
   /* Enable PIO block */
   CSP_GPT_SET_ECR(gpt, PIO);
   
   /* Disable all PIO */
   CSP_GPT_SET_PDR(gpt, 0xFFFFFFFF);
   CSP_GPT_SET_ODR(gpt, 0xFFFFFFFF);

   /* Enable PIO */
   CSP_GPT_SET_PER(gpt, pio_mask);

   /* Set Output PIO */
   CSP_GPT_SET_OER(gpt, output_pio);
}


/******************************************************************************
* Function          : CSP_GPTPioGetStatus
* Description       : Read the pin data status 
* Inputs            : <*gpt> = Pointer to GPT structure
* Functions called  : None
* Returns           : 32-bit value of pin data status
******************************************************************************/
U32_T CSP_GPTPioGetStatus(CSP_GPT_T *const gpt)
{
   /* Return PIO State */
   return (CSP_GPT_GET_PDSR(gpt));
}


/******************************************************************************
* Function          : CSP_GPTPioClear
* Description       : Set the PIO to low level 
* Inputs            : <*gpt>     = Pointer to GPT structure
*                     <pio_mask> = Configure which pins are set to low level
* Functions called  : None
* Returns           : None
******************************************************************************/
void CSP_GPTPioClear(CSP_GPT_T *const gpt, U32_T pio_mask)
{
   /* Set PIO State */
   CSP_GPT_SET_CODR(gpt, pio_mask);
}


/******************************************************************************
* Function          : CSP_GPTPioSet
* Description       : Set the PIO to high level 
* Inputs            : <*gpt>     = Pointer to GPT structure
*                     <pio_mask> = Configure which pins are set to high level
* Functions called  : None
* Returns           : None
******************************************************************************/
void CSP_GPTPioSet(CSP_GPT_T *const gpt, U32_T pio_mask)
{  
   /* Set PIO State */
   CSP_GPT_SET_SODR(gpt, pio_mask);
}


