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
* File Name       : csp_pwm4c.c
* Description     : Function declarations for Pulse Width Modulator 4 channels 
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
* 003  MOD  03/08/01   Olivier MAZUYER    Rename BSP_PWMStop in BSP_PWMSetOffstate
*                                         Create function BSP_PWMStop
* 004  MOD  12/11/01   Christophe GARDIN  Clean Up
* 005  MOD  28/03/02   Christophe GARDIN  Modify CSP_PWM4CInit function and add
*                                         CSP_PWM4CConfigureChannel function 
* 006  MOD  11/04/02   Christophe GARDIN  Add CSP_PWM4CClose function
*----------------------------------------------------------------------------*/

/******************************************************************************
* Include Files
******************************************************************************/
#include "csp.h"


/******************************************************************************
****************************  F U N C T I O N S  ******************************
******************************************************************************/

/******************************************************************************
* Function          : CSP_PWM4CInit
* Description       : Switch on the clock, reset the registers 
* Input             : <*pwm> = Pointer to PWM structure
* Functions called  : None
* Returns           : None
******************************************************************************/
void CSP_PWM4CInit(CSP_PWM4C_T *const pwm)
{
   /* Enable PWM Clock */
   CSP_PWM4C_SET_ECR(pwm, PWM);
 
   /* PWM Software Reset */
   CSP_PWM4C_SET_CR(pwm, SWRST);
}


/******************************************************************************
* Function          : CSP_PWM4CClose
* Description       : Reset and switch off the clock
* Input             : <*pwm> = Pointer to PWM structure
* Functions called  : None
* Returns           : None
******************************************************************************/
void CSP_PWM4CClose(CSP_PWM4C_T *const pwm)
{
   /* PWM Software Reset */
   CSP_PWM4C_SET_CR(pwm, SWRST);

   /* Disable PWM Clock */
   CSP_PWM4C_SET_DCR(pwm, PWM);
}


/******************************************************************************
* Function          : CSP_PWM4CConfigureChannel
* Description       : Configure the PWM channel
* Inputs            : <*pwm>    = Pointer to PWM structure
*                     <channel> = Channel [0-3] to be configured
*                     <mode>    = Configure the PWM mode (Prescalar + PL)
*                     <delay>   = Configure the PWM delay. Number of cycles 
*                                 during which the output is inactive
*                     <pulse>   = Configure the PWM pulse. Number of cycles 
*                                 during which the output is active
* Functions called  : None
* Returns           : None
******************************************************************************/
void CSP_PWM4CConfigureChannel(CSP_PWM4C_T *const pwm, 
                               U8_T channel, 
                               U32_T mode, 
                               U16_T delay,
                               U16_T pulse)
{
   /* Configure channel mode */
   CSP_PWM4C_SET_MR(pwm, (CSP_PWM4C_GET_MR(pwm) & ~(0xFFul << (8u * channel))));
   CSP_PWM4C_SET_MR(pwm, (CSP_PWM4C_GET_MR(pwm) | (mode << (8u * channel))));
      
   /* Configure PWM Signal */
   CSP_PWM4C_CHANNEL_SET_DLY(pwm, channel, delay);
   CSP_PWM4C_CHANNEL_SET_PUL(pwm, channel, pulse);
}


/******************************************************************************
* Function          : CSP_PWM4CConfigInterrupt
* Description       : Configure PWM Interrupts
* Inputs            : <*pwm>     = Pointer to PWM structure
*                     <int_mode> = Configure the priority level and source type
*                     <int_mask> = Configure which interrupt bits are activated
*                     <callback> = Function called through the assembler interrupt handler
* Functions called  : CSP_GET_INTERRUPT_NUMBER, CSP_GICConfigInterrupt
* Returns           : None
******************************************************************************/
void CSP_PWM4CConfigInterrupt(CSP_PWM4C_T *const pwm, 
                              U32_T int_mode, 
                              U32_T int_mask, 
                              U32_T callback)
{
   /* Local Variables */
   U8_T int_num = 0;

   /* Get Peripheral Interrupt Number */
   int_num = CSP_GET_INTERRUPT_NUMBER(pwm);

   /* Disable all interrupt */
   CSP_PWM4C_SET_IDR(pwm, 0xFFFFFFFF);
      
   /* Interrupt Enable */
   CSP_PWM4C_SET_IER(pwm, int_mask);

   /* Configure PWM4C controller interrupt mode in GIC module */
   CSP_GICConfigInterrupt(int_num, int_mode, callback);
}


/******************************************************************************
* Function          : CSP_PWM4CEnable
* Description       : Enable PWM Channel
* Input             : <*pwm>    = Pointer to PWM structure
*                     <channel> = Channel [0-3] to be enabled
* Functions called  : None
* Returns           : None
******************************************************************************/
void CSP_PWM4CEnable(CSP_PWM4C_T *const pwm , U8_T channel)
{
   /* Enable PWM Channel */
   CSP_PWM4C_SET_CR(pwm, (PWMEN << (2u * channel)));
}


/******************************************************************************
* Function          : CSP_PWM4CDisable
* Description       : Disable PWM Channel
* Input             : <*pwm>    = Pointer to PWM structure
*                     <channel> = Channel [0-3] to be disabled
* Functions called  : None
* Returns           : None
******************************************************************************/
void CSP_PWM4CDisable(CSP_PWM4C_T *const pwm, U8_T channel)
{
   /* Disable PWM Channel */
   CSP_PWM4C_SET_CR(pwm, (PWMDIS << (2u * channel)));
}


/******************************************************************************
* Function          : CSP_PWM4CSetOffstate
* Description       : Stop and reset PWM channel specified, leaving output pin in 
*                     the offstate state 
* Input             : <*pwm>     = Pointer to PWM structure
*                     <channel>  = Channel [0-3] to be stopped
*                     <offstate> = Set LOW (0) or HIGH (1) level on output pin 
*                                  when the PWM channel is stopped
* Functions called  : CSP_PWM4CConfigureChannel
* Returns           : None
******************************************************************************/
void CSP_PWM4CSetOffstate(CSP_PWM4C_T *const pwm, U8_T channel, U8_T offstate)
{
   /* Init PWM Module */
   if(offstate == 0)
   {
      /* Offstate Level = 0 */
      CSP_PWM4CConfigureChannel(pwm, channel, 0x10, 0x1, 0x0);
   }
   else
   {
      /* Offstate Level = 1 */
      CSP_PWM4CConfigureChannel(pwm, channel, 0x10, 0x0, 0x1);
   }   

   /* Start PWM Module */
   CSP_PWM4C_SET_CR(pwm, (PWMEN << (2u * channel)));
}

