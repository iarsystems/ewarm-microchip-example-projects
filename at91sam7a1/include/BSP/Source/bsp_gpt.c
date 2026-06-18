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
* File Name   : bsp_gpt.c
* Description : Function declarations for Timer Channel for the BSP
* Version     : 1.01
*
*       +----- (NEW | MODify | ADD | DELete)                                 
*       |                                                                    
*  No   |   when       who                what               
*-----+---+----------+------------------+--------------------------------------
* 000  NEW  15/06/00   Patrice VILCHEZ    Creation
* 001  MOD  01/04/01   Olivier MAZUYER    Clean up
* 002  MOD  13/07/01   David   LEWIN      Add functions : BSP_GPTLightOnLED 
*                                                         BSP_GPTLightOffLED 
* 003  MOD  31/01/02   Mahmoud Mesgarzadeh   Clean Up
*----------------------------------------------------------------------------*/


/******************************************************************************
Include files
******************************************************************************/
#include "csp.h"
#include "bsp.h"

/******************************************************************************
Function          : BSP_GPTSetOutput
Description       : This function produces on the pins specified by the input 
                    parameters a signal with the frequency and duty cycle is 
                    specified by the input parameters.
Input             : 
- *gpt : Pointer to 创CSP_GPT_T创 structure
- tio : GPT output used (TIOA, TIOB, or TCLK)
- frequency_u32	: Configure the frequency (expressed in 1/256 Hz)
- dutycycle_u16	: Configure the duty cycle (expressed in 100/256 %)
Functions called  : 创CSP_GPTDisable创, 创CSP_GPTInit创,创CSP_GPT_SET_CR创,
                    创CSP_GPTPioSet创, 创CSP_GPTPioClear创
Returns           : None
******************************************************************************/
void BSP_GPTSetOutput(CSP_GPT_T *const gpt, U32_T tio, U32_T frequency_u32, U16_T dutycycle_u16)
{
  /* local variables */
  U8_T i_u8 = 0;
  U32_T delay_u32;
  U32_T pulse_u32;
  U32_T clk_u32;
  U32_T mode_u32;
  U32_T pin_mask_u32;
  U16_T reg_value_a_u16[3];
     
  clk_u32  = (BSP_SYSTEM_CLOCK / 2);

  if( frequency_u32 != 0)
  {
     /* Define delay & pulse function(frequency/dutycycle/prescalar) */
     delay_u32 = ((clk_u32 * dutycycle_u16) / frequency_u32);
     pulse_u32 = (clk_u32 / (frequency_u32/256)) - delay_u32;

     /* Condition about cycle of clock */
     while( (delay_u32 + pulse_u32) > 0xFFFF )
     {
        //  a modifier fonction prescalar_0 dans MR setMode 
        i_u8++;

        /* Define delay & pulse function(frequency/dutycycle/prescalar) */
        clk_u32   /= 2;
        delay_u32  = ((clk_u32 * dutycycle_u16) / frequency_u32);
        pulse_u32  = ((clk_u32 / (frequency_u32/256)) - delay_u32);
     }
  
     /* Choice of the better Clock */
     switch (i_u8)
     {
        case 0:  
          mode_u32 = CLKS_MCK2;
          break;
        case 1:
          mode_u32 = CLKS_MCK8;
          delay_u32 /= 2;
          pulse_u32 /= 2;
          break;
       case 2:
         mode_u32 = CLKS_MCK8;
         break;
       case 3:
         mode_u32 = CLKS_MCK32;
         delay_u32 /= 2;
         pulse_u32 /= 2 ;
         break;
       case 4:
         mode_u32 = CLKS_MCK32;
         break; 
       case 5:
         mode_u32 = CLKS_MCK128;
         delay_u32 /= 2;
         pulse_u32 /= 2;
         break;
       case 6:
         mode_u32 = CLKS_MCK128; 
         break;
       case 7:
         mode_u32 = CLKS_MCK1024;
         delay_u32 /= 4;
         pulse_u32 /= 4;
         break;
       case 8:
         mode_u32 = CLKS_MCK1024;
         delay_u32 /= 2;
         pulse_u32 /= 2;
         break;
       case 9:
         mode_u32 = CLKS_MCK1024;
         break;
       default :
         mode_u32 = CLKS_MCK1024;
         delay_u32 = (frequency_u32 * dutycycle_u16 / 100) / 2^9;
         pulse_u32 = (frequency_u32 * (1-dutycycle_u16 / 100)) / 2^9;
     }

      // TIOA Or TIOB Output selected
      if (tio == TIOA)
      {  
        /* configure GPT0 with TIOA */
        mode_u32  |= ( WAVE | ACPA_SET | CPCTRG | ACPC_CLEAR);
        reg_value_a_u16[0] = pulse_u32 - 1 ;
        reg_value_a_u16[2] = delay_u32 + pulse_u32 - 1;
      }
      else if (tio == TIOB)
      {
        /* configure GPT0 with TIOB */
        mode_u32 |= ( WAVE | BCPB_SET | CPCTRG | BCPC_CLEAR);
        reg_value_a_u16[1]  = pulse_u32 - 1 ;
        reg_value_a_u16[2]  = delay_u32 + pulse_u32 - 1;
      }
  
      /* Configure GPT mode */
      CSP_GPTInit(gpt, mode_u32, reg_value_a_u16);
      
      /* GPT_Start(GPT0CH0) */
      CSP_GPT_SET_CR(gpt, (SWTRG | CLKEN));          
   }
   
   /* Frequency Egual 0 */ 
   else
   {
      /* Stop Timer Clock */
      CSP_GPTDisable(gpt);
      
      /* Which Timer Channel Line s*/
      if (tio == TIOA)
      {  
        pin_mask_u32 = TIOA;
      }
      else if (tio == TIOB)
      {
        pin_mask_u32 = TIOB;
      }

      /* Set Status Line to Low or High Level */
      if( dutycycle_u16 == 256 )
         CSP_GPTPioSet(gpt, pin_mask_u32);
      else 
         CSP_GPTPioClear(gpt, pin_mask_u32);
            
      /* Configure Line as Pio & Output */
      CSP_GPTPioInit(gpt, pin_mask_u32, pin_mask_u32);
   }
}

/******************************************************************************
Function          : BSP_GPTLightOnLED
Description       : This function lights on red, orange and/or green LED.
Input             : led = combination of RED, ORANGE, GREEN
Functions called  : 创CSP_GPT_SET_ECR创,创CSP_GPT_SET_PER创,创CSP_GPT_SET_OER创
                    创CSP_GPT_SET_SODR创 
Returns           : None
******************************************************************************/
void BSP_GPTLightOnLED(U32_T led)
{
   /* If red LED is selected, do the following to light on red LED */
   if ((led & RED) == RED)
   {
      /* Enable channel 0 PIO clock */
      CSP_GPT_SET_ECR(GPT0CH0, PIO);
      
      /* Enable the PIO to control TIOA1 pin */
      CSP_GPT_SET_PER(GPT0CH0, TIOA);

      /* Enable the PIO output on TIOA1 pin */
      CSP_GPT_SET_OER(GPT0CH0, TIOA);

      /* Set output on TIOA1 pin */
      CSP_GPT_SET_SODR(GPT0CH0, TIOA);
   }

   /* If orange LED is selected, do the following to light on orange LED */
   if ((led & ORANGE) == ORANGE)
   {
      /* Enable channel 0 PIO clock */
      CSP_GPT_SET_ECR(GPT0CH0, PIO);

      /* Enable the PIO to control TIOB0 pin */
      CSP_GPT_SET_PER(GPT0CH0, TIOB);

      /* Enable the PIO output on TIOB0 pin */
      CSP_GPT_SET_OER(GPT0CH0, TIOB);

      /* Set output on TIOB0 pin */
      CSP_GPT_SET_SODR(GPT0CH0, TIOB);
   }

   /* If green LED is selected, do the following to light on green LED */
   if ((led & GREEN) == GREEN)
   {
      /* Enable channel 1 PIO clock */
      CSP_GPT_SET_ECR(GPT0CH1, PIO);
						  
      /* Enable the PIO to control TIOA0 pin */
      CSP_GPT_SET_PER(GPT0CH1, TIOA);

      /* Enable the PIO output on TIOA0 pin */
      CSP_GPT_SET_OER(GPT0CH1, TIOA);

      /* Set output on TIOA0 pin */
      CSP_GPT_SET_SODR(GPT0CH1, TIOA);
   }
} 

/******************************************************************************
Function          : BSP_GPTLightOffLED
Description       : This function lights off red, orange and/or green LED.
Input             : led = combination of RED, ORANGE, GREEN
Functions called  : 创CSP_GPT_SET_ECR创, 创CSP_GPT_SET_PER创,创CSP_GPT_SET_OER创
                    创CSP_GPT_SET_CODR创 
Returns           : None
******************************************************************************/
void BSP_GPTLightOffLED(U32_T led)
{
   /* If red LED is selected, do the following to light off red LED */
   if ((led & RED) == RED)
   {
      /* Enable channel 0 PIO clock */
      CSP_GPT_SET_ECR(GPT0CH0, PIO);

      /* Enable the PIO to control TIOA1 pin */
      CSP_GPT_SET_PER(GPT0CH0, TIOA);

      /* Enable the PIO output on TIOA1 pin */
      CSP_GPT_SET_OER(GPT0CH0, TIOA);

      /* Clear output on TIOA1 pin */
      CSP_GPT_SET_CODR(GPT0CH0, TIOA);
   }

   /* If orange LED is selected, do the following to light on orange LED */
   if ((led & ORANGE) == ORANGE)
   {
      /* Enable channel 0 PIO clock */
      CSP_GPT_SET_ECR(GPT0CH0, PIO);

      /* Enable the PIO to control TIOB0 pin */
      CSP_GPT_SET_PER(GPT0CH0, TIOB);

      /* Enable the PIO output on TIOB0 pin */
      CSP_GPT_SET_OER(GPT0CH0, TIOB);

      /* Clear output on TIOB0 pin */
      CSP_GPT_SET_CODR(GPT0CH0, TIOB);
   }

   /* If green LED is selected, do the following to light on green LED */
   if ((led & GREEN) == GREEN)
   {
      /* Enable channel 1 PIO clock */
      CSP_GPT_SET_ECR(GPT0CH1, PIO);

      /* Enable the PIO to control TIOA0 pin */
      CSP_GPT_SET_PER(GPT0CH1, TIOA);

      /* Enable the PIO output on TIOA0 pin */
      CSP_GPT_SET_OER(GPT0CH1, TIOA);

      /* Clear output on TIOA0 pin */
      CSP_GPT_SET_CODR(GPT0CH1, TIOA);
   }
} 
