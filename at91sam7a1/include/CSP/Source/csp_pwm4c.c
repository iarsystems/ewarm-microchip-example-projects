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
* 007  MOD  21/11/02   Bruno SALLE        Disable PIO block
*----------------------------------------------------------------------------*/

/******************************************************************************
* Include Files
******************************************************************************/
#include "csp.h"


/******************************************************************************
****************************  F U N C T I O N S  ******************************
******************************************************************************/

/******************************************************************************
Function          : CSP_PWM4CInit
Description       : Switch on the clock, reset the registers
                    A PWM software reset is made at the fisrt CSP_PWM4CInit call
Input             : <*pwm> = Pointer to PWM structure
Functions called  : None
Returns           : None
******************************************************************************/
void CSP_PWM4CInit(CSP_PWM4C_T *const pwm)
{
static U8_T Nb_Call = 0;

   /* Enable PWM and PIO Clock */
   CSP_PWM4C_SET_ECR(pwm, PWM | PIO);

   /* PWM Software Reset if first call */
   if (Nb_Call == 0)
      CSP_PWM4C_SET_CR(pwm, SWRST);
 
   /* Disable PIO */
   CSP_PWM4C_SET_PDR(pwm, PIOPWM0 | PIOPWM1 | PIOPWM2 | PIOPWM3);

   /* Disable PIO Clock */
   CSP_PWM4C_SET_DCR(pwm, PIO);

   /* For the next call */
   Nb_Call = 1;

}


/******************************************************************************
Function          : CSP_PWM4CClose
Description       : Reset and switch off the clock
Input             : <*pwm> = Pointer to PWM structure
Functions called  : None
Returns           : None
******************************************************************************/
void CSP_PWM4CClose(CSP_PWM4C_T *const pwm)
{
   /* PWM Software Reset */
   CSP_PWM4C_SET_CR(pwm, SWRST);

   /* Disable PWM Clock */
   CSP_PWM4C_SET_DCR(pwm, PWM);
}


/******************************************************************************
Function          : CSP_PWM4CConfigureChannel
Description       : Configure the PWM channel
Inputs            :
- <*pwm>    = Pointer to PWM structure
- <channel> = Channel [0-3] to be configured
- <mode>    = Configure the PWM mode (Prescalar + PL)
- <delay>   = Configure the PWM delay. Number of cycles
  during which the output is inactive
- <pulse>   = Configure the PWM pulse. Number of cycles
  during which the output is active
Functions called  : None
Returns           : None
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
Function          : CSP_PWM4CConfigInterrupt
Description       : Configure PWM Interrupts
Inputs            :
- <*pwm>     = Pointer to PWM structure
- <int_mode> = Configure the priority level and source type
- <int_mask> = Configure which interrupt bits are activated
- <callback> = Function called through the assembler interrupt handler
Functions called  : 创CSP_GET_INTERRUPT_NUMBER创, 创CSP_GICConfigInterrupt创
Returns           : None
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
Function          : CSP_PWM4CEnable
Description       : Enable PWM Channel
Input             :
- <*pwm>    = Pointer to PWM structure
- <channel> = Channel [0-3] to be enabled
Functions called  : None
Returns           : None
******************************************************************************/
void CSP_PWM4CEnable(CSP_PWM4C_T *const pwm , U8_T channel)
{
   /* Enable PWM Channel */
   CSP_PWM4C_SET_CR(pwm, (PWMEN << (2u * channel)));
}


/******************************************************************************
Function          : CSP_PWM4CDisable
Description       : Disable PWM Channel
Input             :
- <*pwm>    = Pointer to PWM structure
- <channel> = Channel [0-3] to be disabled
Functions called  : None
Returns           : None
******************************************************************************/
void CSP_PWM4CDisable(CSP_PWM4C_T *const pwm, U8_T channel)
{
   /* Disable PWM Channel */
   CSP_PWM4C_SET_CR(pwm, (PWMDIS << (2u * channel)));
}


/******************************************************************************
Function          : CSP_PWM4CSetOffstate
Description       : Stop and reset PWM channel specified, leaving output pin in
                    the offstate state
Input             :
- <*pwm>     = Pointer to PWM structure
- <channel>  = Channel [0-3] to be stopped
- <offstate> = Set LOW (0) or HIGH (1) level on output pin
  when the PWM channel is stopped
Functions called  : 创CSP_PWM4CConfigureChannel创
Returns           : None
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



/******************************************************************************
Function          : CSP_PWM4CPioInit
Description       : Configure PWM4C PIO
Inputs            :
- <*pwm>     = Pointer to PWM4C structure
- <pio_mask>   = Configure which pins are activated
- <output_pio> = Configure which pins are configured as output
Functions called  : None
Returns           : None
******************************************************************************/
void CSP_PWM4CPioInit(CSP_PWM4C_T *const pwm, U32_T pio_mask, U32_T output_pio)
{
   /* Enable PIO block */
   CSP_PWM4C_SET_ECR(pwm, PIO);

   /* Disable all PIO */
   CSP_PWM4C_SET_PDR(pwm, 0xFFFFFFFF);
   CSP_PWM4C_SET_ODR(pwm, 0xFFFFFFFF);

   /* Enable PIO */
   CSP_PWM4C_SET_PER(pwm, pio_mask);
  
   /* Set Output PIO */
   CSP_PWM4C_SET_OER(pwm, output_pio);
}


/******************************************************************************
Function          : CSP_PWM4CPioGetStatus
Description       : Read the pin data status
Inputs            : <*pwm> = Pointer to PWM4C structure
Functions called  : None
Returns           : 32-bit value of pin data status
******************************************************************************/
U32_T CSP_PWM4CPioGetStatus(CSP_PWM4C_T *const pwm)
{
   /* Return PIO State */
   return (CSP_PWM4C_GET_PDSR(pwm));
}


/******************************************************************************
Function          : CSP_PWM4CPioClear
Description       : Set the PIO to low level
Inputs            :
- <*pwm>   = Pointer to PWM4C structure
- <pio_mask> = Configure which pins are set to low level
Functions called  : None
Returns           : None
******************************************************************************/
void CSP_PWM4CPioClear(CSP_PWM4C_T *const pwm, U32_T pio_mask)
{
   /* Set PIO State */
   CSP_PWM4C_SET_CODR(pwm, pio_mask);
}


/******************************************************************************
Function          : CSP_PWM4CPioSet
Description       : Set the PIO to high level
Inputs            :
- <*pwm>   = Pointer to PWM4C structure
- <pio_mask> = Configure which pins are set to high level
Functions called  : None
Returns           : None
******************************************************************************/
void CSP_PWM4CPioSet(CSP_PWM4C_T *const pwm, U32_T pio_mask)
{
   /* Set PIO State */
   CSP_PWM4C_SET_SODR(pwm, pio_mask);
}
