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
* Processor   : easyCAN3
* File Name   : bsp_capt.c
* Description : It computes signal frequency from CAPT0 or CAPT1 input.
* Version     : 1.01
*
*       +----- (NEW | MODify | ADD | DELete)                                 
*       |                                                                    
*  No   |   when       who                what               
*-----+---+----------+------------------+--------------------------------------
* 000  NEW  30/07/01   Olivier MAZUYER    Creation
* 001  MOD  31/01/02   Mahmoud Mesgarzadeh   Clean Up
*----------------------------------------------------------------------------*/


/******************************************************************************
* Include files
******************************************************************************/
#include "csp.h"
#include "bsp.h"

/******************************************************************************
* Function          : BSP_CAPT_PositiveEdgesFrequency
* Description       : It establishes duration between two consecutive positive
*                     edges, and finally, it computes and returns the frequency
* Input             : *capt, prescalar_u8
* Functions called  : CSP_CAPTInit
* Returns           : frequency of the input signal
******************************************************************************/
U32_T BSP_CAPT_PositiveEdgesFrequency(CSP_CAPT_T *capt, U8_T  prescalar_u8)
{
   /* Local Variable */
   U32_T status_u32 = 0x100;

   /* Initialize Capture module */
   CSP_CAPTInit(capt, (ONESHOT | MEASMODE_POS | prescalar_u8));
   
   /* Enable the capture */
   CSP_CAPT_SET_CR(capt, CAPEN);
   
   /* Start the capture */
   CSP_CAPT_SET_CR(capt, STARTCAPT);

   /* Wait until data is captured */
   while( (status_u32 & 0x108) != 0x108 )
   {
      /* Read Status Register */ 
      status_u32 = CSP_CAPT_GET_SR(capt);
   }

   /* Read Data Register and Compute the frequency of the sigal captured */
   return ((BSP_SYSTEM_CLOCK >> (CSP_CAPT_GET_MR(capt) & PRESCALAR_MASK)) / (2 * CSP_CAPT_GET_DR(capt)));
}

/******************************************************************************
* Function          : BSP_CAPT_NegativeEdgesFrequency
* Description       : It establishes duration between two consecutive negative
*                     edges, and finally, it computes and returns the frequency
* Input             : *capt, prescalar_u8
* Functions called  : CSP_CAPTInit
* Returns           : frequency of the input signal
******************************************************************************/
U32_T BSP_CAPT_NegativeEdgesFrequency(CSP_CAPT_T *capt, U8_T  prescalar_u8)
{
   /* Local Variable */
   U32_T status_u32 = 0x100;

   /* Initialize Capture module */
   CSP_CAPTInit(capt, (ONESHOT | MEASMODE_NEG | prescalar_u8));

   /* Enable the capture */
   CSP_CAPT_SET_CR(capt, CAPEN);

   /* Start the capture */
   CSP_CAPT_SET_CR(capt, STARTCAPT);

   /* Wait until data is captured */
   while( (status_u32 & 0x108) != 0x108 )
   {
      /* Read Status Register */ 
      status_u32 = CSP_CAPT_GET_SR(capt);
   }

   /* Read Data Register and Compute the frequency of the sigal captured */
   return ((BSP_SYSTEM_CLOCK >> (CSP_CAPT_GET_MR(capt) & PRESCALAR_MASK)) / (2 * CSP_CAPT_GET_DR(capt)));
}

/******************************************************************************
* Function          : BSP_CAPT_EachEdgesFrequency
* Description       : It establishes duration between a positive and a negative
*                     edge, and finally, it computes and returns the frequency
* Input             : *capt, prescalar_u8
* Functions called  : CSP_CAPTInit
* Returns           : frequency of the input signal
******************************************************************************/
U32_T BSP_CAPT_EachEdgesFrequency(CSP_CAPT_T *capt, U8_T  prescalar_u8)
{
   /* Local Variable */
   U32_T status_u32 = 0x100;

   /* Initialize Capture module */
   CSP_CAPTInit(capt, (ONESHOT | MEASMODE_NEG | prescalar_u8));

   /* Enable the capture */
   CSP_CAPT_SET_CR(capt, CAPEN);

   /* Start the capture */
   CSP_CAPT_SET_CR(capt, STARTCAPT);

   /* Wait until data is captured */
   while( (status_u32 & 0x108) != 0x108 )
   {
      /* Read Status Register */ 
      status_u32 = CSP_CAPT_GET_SR(capt);
   }

   /* Read Data Register and Compute the frequency of the sigal captured */
   return ((BSP_SYSTEM_CLOCK >> (CSP_CAPT_GET_MR(capt) & PRESCALAR_MASK)) / (2 * CSP_CAPT_GET_DR(capt)));
}