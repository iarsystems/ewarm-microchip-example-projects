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
* File Name   : bsp_st.c
* Description : Function declarations for Simple Timer for the BSP
* Version     : 1.01
*
*       +----- (NEW | MODify | ADD | DELete)                                 
*       |                                                                    
*  No   |   when       who                what               
*-----+---+----------+------------------+--------------------------------------
* 001  MOD  01/04/01   Olivier MAZUYER    Creation
* 002  MOD  06/08/01   Frederic SAMSON    - Take account of both ST0 and ST1 timers
*                                         - Add comments 
* 003  ADD  08/08/01   Frederic SAMSON    - Add BSP_STStartDelay, ST1CH1_InterruptHandler
*                                           ST0CH1_InterruptHandler, ST1CH0_InterruptHandler
*                                           ST1CH1_InterruptHandler 
* 004  MOD  31/01/02   Mahmoud Mesgarzadeh   Clean Up
*----------------------------------------------------------------------------*/


/******************************************************************************
* Includes 
******************************************************************************/
#include "csp.h"
#include "bsp.h"

BSP_ST_CONFIGURE_T BSP_ST_Config_s ;

/******************************************************************************
Function          : BSP_STWaitDelay
Description       : Set the Simple Timer to count down during a particular 
                    delay in ms. Rq : Blocking function.
Input             : *st,channel_u8, delay_u32
Functions called  : CSP_STInit, CSP_STStart, CSP_STStop
Returns           : None
******************************************************************************/
void BSP_STWaitDelay(CSP_ST_T *const st, U16_T channel_u16, U32_T delay_u32)
{
   /* local variables */
   U16_T sysscal_u16  = 0x0 ;
   U8_T  prescalar_u8 = 0x4 ;
   U32_T status_u32   = 0x0;
   unsigned long long counter_val_u64;  
   U32_T prescal_mode_u32;

   /* Compute counter value */
   counter_val_u64 = delay_u32 * (BSP_SYSTEM_CLOCK >> prescalar_u8) / (2 * (sysscal_u16 + 1)) / 1000;

   /* Do until counter value is less than a 16 bits word */
   while (counter_val_u64 > 0xFFFF)
   {
      /* Increment Sysscal : system clock prescalar value */
      sysscal_u16 += 1;

      /* Compute new counter value */
      counter_val_u64 = delay_u32 * (BSP_SYSTEM_CLOCK >> prescalar_u8) / (2 * (sysscal_u16 + 1)) / 1000;        
   }  

   /* Define prescal_mode value */
   prescal_mode_u32 = (sysscal_u16 << 8 | SELECTCLK_SYS << 4 | prescalar_u8);

   /* Simple Timer Init */
   CSP_STInit(st);
   CSP_STConfigureChannel(st, channel_u16, prescal_mode_u32, (U16_T) counter_val_u64);

   /* Simple Timer Start */
   CSP_STEnable(st, channel_u16);
   

   /* Is it Channel 1 ? */
   if(channel_u16 == 0)
   {
      /* Yes, Wait until the channel 1 has reached the end of down counting */
      while ( (status_u32 & CHEND0) != CHEND0 )
      {
         /* Read Status Register */
         status_u32 = CSP_ST_GET_SR(st);
      }
   }

   else
   {
      /* No, Wait until the channel 2 has reach the end of down counting */
      while ( (status_u32 & CHEND1) != CHEND1 )
      {
         /* Read Status Register */
         status_u32 = CSP_ST_GET_SR(st);
      }
   }

   /* Simple Timer Stop */
   CSP_STDisable(st, channel_u16);
}


/******************************************************************************
Function          : BSP_STStartDelay
Description       : Set the Simple Timer to count down during a particular 
                    delay in ms and generate an interrupt on the specified channel
Input             : *st, channel_u8, delay_u32
Functions called  : CSP_STInit, CSP_STStart, CSP_STConfigInterrupt
Returns           : None
******************************************************************************/
void BSP_STStartDelay(CSP_ST_T *const st, U16_T channel_u16, U32_T delay_u32)
{
   /* local variables */
   U16_T sysscal_u16  = 0x0 ;
   U8_T  prescalar_u8 = 0x4 ;
   unsigned long long  counter_val_u64;  
   U32_T prescal_mode_u32;
   U32_T callback_u32;

   /* Compute counter value */
   counter_val_u64 = delay_u32 * (BSP_SYSTEM_CLOCK >> prescalar_u8) / (2 * (sysscal_u16 + 1)) / 1000;

   /* Do until counter value is less than a 16 bits word */
   while (counter_val_u64 > 0xFFFF)
   {
      /* Increment Sysscal : system clock prescalar value */
      sysscal_u16 += 1;

      /* Compute new counter value */
      counter_val_u64 = delay_u32 * (BSP_SYSTEM_CLOCK >> prescalar_u8) / (2 * (sysscal_u16 + 1)) / 1000;        
   }  


   /* Define prescal_mode value */
   prescal_mode_u32 = (sysscal_u16 << 8 | SELECTCLK_SYS | prescalar_u8);

   /* Simple Timer Init */
   CSP_STInit(st);
   CSP_STConfigureChannel(st, channel_u16, prescal_mode_u32, (U16_T) counter_val_u64);

   if (st==ST0) 
   {
       if (channel_u16==0)
       {
           callback_u32= (U32_T) ST0CH0_AsmInterruptHandler;
       }
       else
       {
           callback_u32= (U32_T) ST0CH1_AsmInterruptHandler;
       }
   }
   else
   {
       if (channel_u16==0)
       {
           callback_u32= (U32_T) ST1CH0_AsmInterruptHandler;
       }
       else
       {
           callback_u32= (U32_T) ST1CH1_AsmInterruptHandler;
       }
   }
   
   /* Config interrupt */
   CSP_STConfigInterrupt(st, (HIGH_LEVEL_SENSITIVE|PRIOR_1), channel_u16 + 1, (U32_T) callback_u32);
 
   /* Simple Timer Start */
   CSP_STEnable(st, channel_u16);

}

/******************************************************************************
Function          : ST0CH0_InterruptHandler
Description       : Call to ST0 CH0 callback function 
Input             : None
Functions called  : None
Returns           : None
******************************************************************************/
void ST0CH0_InterruptHandler(void)
{
   (BSP_ST_Config_s.callback_function)(ST0,0x00);
   CSP_ST_SET_CSR(ST0, CHEND0);
}

/******************************************************************************
Function          : ST0CH1_InterruptHandler
Description       : Call to ST0 CH1 callback function 
Input             : None
Functions called  : None
Returns           : None
******************************************************************************/
void ST0CH1_InterruptHandler(void)
{
   (BSP_ST_Config_s.callback_function)(ST0,0x01);
   CSP_ST_SET_CSR(ST0, CHEND1);
}

/******************************************************************************
Function          : ST1CH0_InterruptHandler
Description       : Call to ST1 CH0 callback function 
Input             : None
Functions called  : None
Returns           : None
******************************************************************************/
void ST1CH0_InterruptHandler(void)
{
   (BSP_ST_Config_s.callback_function)(ST1,0x00);
   CSP_ST_SET_CSR(ST1, CHEND0);
}

/******************************************************************************
Function          : ST1CH1_InterruptHandler
Description       : Call to ST1 CH1 callback function 
Input             : None
Functions called  : None
Returns           : None
******************************************************************************/
void ST1CH1_InterruptHandler(void)
{
   (BSP_ST_Config_s.callback_function)(ST1,0x01);
   CSP_ST_SET_CSR(ST1, CHEND1);
}

