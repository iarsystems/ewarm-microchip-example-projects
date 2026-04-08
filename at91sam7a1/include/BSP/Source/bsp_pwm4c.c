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
* Processor   : easyCAN
* File Name   : bsp_pwm4c.c
* Description : Function declarations for PWM for the BSP
* Version     : 1.01
*
*       +----- (NEW | MODify | ADD | DELete)                                 
*       |                                                                    
*  No   |   when       who                what               
*-----+---+----------+------------------+--------------------------------------
* 000  NEW  01/05/99   Patrice VILCHEZ    Creation
* 001  MOD  01/04/01   Olivier MAZUYER    Clean up
* 002  MOD  31/01/02   Mahmoud Mesgarzadeh   Clean Up
*----------------------------------------------------------------------------*/


/******************************************************************************
* Includes 
******************************************************************************/
#include "csp.h"
#include "bsp.h"


/******************************************************************************
Function          : BSP_PWM4CSetOuput
Description       : This function switches on the PWM module, then it computes 
                    the prescalar value used to generated the counter clock 
                    (counter clock frequency = main clock frequency / 2^PRESCALAR+1) 
                    and computes the duration of the pulse (output at high level) 
                    and the duration of the delay (output at low level). 
                    Finally, it starts the generation of the signal.
                    If input parameter frequency is equal to 0, then depending of 
                    the dutycycle value, the output is set to high level (dutycycle = 100) 
                    or low level (dutycycle < 100).						    
Input             : 
- *pwm4c : Pointer to 创CSP_PWM4C_T创 structure
- channel_u8 : PWM channel
- frequency_u32 : PWM frequency (expressed in 1/256 Hz)
- dutycycle_u16 : Duty cycle (expressed in 1/256 %).
                  Ex :for a 50% dutycycle, users should set dutycycle_u16 to 128 
                  (128 x 100 / 256 = 50 %)
Functions called  : 创CSP_PWM4C_SET_ECR创, 创CSP_PWM4C_SET_MR创, 创CSP_PWM4CInit创,
                    创CSP_PWM4CConfigureChannel创, 创CSP_PWM4CEnable创, 创CSP_PWM4CSetOffstate创
Returns           : None
******************************************************************************/
void BSP_PWM4CSetOuput(CSP_PWM4C_T *pwm4c, U8_T channel_u8, U32_T frequency_u32, U16_T dutycycle_u16)
{
  /* local variables */
  U8_T  i_u8 = 0;
  U32_T delay_u32;
  U32_T pulse_u32;
  U32_T mode_u32;
  U32_T clk_u32;

  /* Switch On PWM Controller (To be sure) */
  CSP_PWM4C_SET_ECR(pwm4c, PWM); 

  /* Clear Current Clock Divider */
  CSP_PWM4C_SET_MR(pwm4c, CSP_PWM4C_GET_MR(pwm4c) & ~(0x1F << (channel_u8 * 8)));
    
  if( frequency_u32 != 0 )
  {
     /* Define delay & pulse function(frequency/dutycycle/prescalar) */
     clk_u32     = (BSP_SYSTEM_CLOCK / 2);
     delay_u32   = ((clk_u32 * dutycycle_u16) / frequency_u32);
     pulse_u32   = (clk_u32 / (frequency_u32/256)) - delay_u32;
  
     /* Condition about cycle of clock */
     while((delay_u32 > 0xFFFF) | (pulse_u32 > 0xFFFF))
     {
        /*  Compute delay_u32 & pulse_u32, increment i_u8 and divide clk_u32 */
        i_u8++;
        clk_u32 /= 2;
        /* Define delay & pulse function(frequency/dutycycle/prescalar)*/
        delay_u32   = ((clk_u32 * dutycycle_u16) / frequency_u32);
        pulse_u32   = (clk_u32 / (frequency_u32/256)) - delay_u32;
     }
   /* Define Mode */
     mode_u32 = i_u8;

     /* Init PWM Module */
     CSP_PWM4CInit(pwm4c);
     CSP_PWM4CConfigureChannel(pwm4c, channel_u8, mode_u32, (U16_T) delay_u32, (U16_T) pulse_u32);

     /* start PWM Module */
     CSP_PWM4CEnable(pwm4c , channel_u8);
   }
   else
   {
     /* Stop PWM Module */
     if( dutycycle_u16 == 100 )
     {
         CSP_PWM4CSetOffstate(pwm4c, 1, channel_u8);
     }
     else
     {
         CSP_PWM4CSetOffstate(pwm4c, 0, channel_u8);
     }
   } 
}
