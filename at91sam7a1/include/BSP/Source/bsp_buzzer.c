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
* File Name   : bsp_lcd.c
* Description : Function declarations for Buzzer for the BSP.
* Version     : 1.01
*
*       +----- (NEW | MODify | ADD | DELete)                                 
*       |                                                                    
*  No   |   when       who                what               
*-----+---+----------+------------------+--------------------------------------
* 000  MOD  01/04/01   Olivier MAZUYER    Creation
* 001  MOD  31/01/02   Mahmoud Mesgarzadeh   Clean Up
*----------------------------------------------------------------------------*/


/******************************************************************************
* Includes 
******************************************************************************/
#include "csp.h"
#include "bsp.h"

/****************************************************************************
* BSP Buzzer 'Au Clair de la Lune' Definition                             
****************************************************************************/
BSP_NOTE_T AuClairDeLaLune[] = 
{
   DO,  500,
   DO,  500,
   DO,  500,
   RE,  500,
   MI, 1000,
   RE, 1000,
   DO,  500,
   MI,  500,
   RE,  500,
   RE,  500,
   DO, 1500
};


/******************************************************************************
Function          : BSP_BuzzerSoundsFreq
Description       : This function configures and starts PWM and ST modules to 
                    make the buzzer sounds a note with a frequency and duration 
                    specified by the input parameters.
Input             : 
- freq_u16 : sound frequency 
- delay_u32 : sound delay 
Functions called  : 创BSP_PWM4CSetOuput创, 创BSP_STWaitDelay创, 创CSP_PWM4CDisable创
Returns           : None
******************************************************************************/
void BSP_BuzzerSoundsFreq(U32_T freq_u32, U32_T delay_u32)
{
   /* Set the PWM4C system to freq frequency and 50 % dutycycle */
   /* Note: for a frequency of 0 Hz, the PWM module is stopped */
      BSP_PWM4CSetOuput(PWM0, CH_BUZZER, freq_u32 * 256, HALF_DUTY);      
      
   /* Wait for delay ms. The function uses delay in ns */
      BSP_STWaitDelay(ST0, CH_BUZZER, delay_u32);

   /* Stop PWM module */
      CSP_PWM4CDisable(PWM0, CH_BUZZER);
}

