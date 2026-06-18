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
* File Name   : csp_switch.c
* Description : Function declarations for GPT Management with the switches.
*		There are 3 goals 
* Version     : 1.01
*
*       +----- (NEW | MODify | ADD | DELete)                                 
*       |                                                                    
*  No   |   when       who                what               
*-----+---+----------+------------------+--------------------------------------
* 000  NEW  09/07/01   David LEWIN        Creation
* 001  MOD  18/07/01   Frederic SAMSON    - Remove all menu configurations to
*                                           the application layer
*                                         - Update Interrupt handlers
*                                         - Add structure declaration : BSP_SWITCH_a_s
*                                         - Update included files : call to csp.h and bsp.h
*                                         - Add comments 
* 002  MOD  31/01/02   Mahmoud Mesgarzadeh   Clean Up

*----------------------------------------------------------------------------*/

/******************************************************************************
Include files
******************************************************************************/
#include "csp.h"
#include "bsp.h"

BSP_SWITCH_T BSP_SWITCH_a_s ;

/******************************************************************************
Function          : BSP_SWITCHTIOB1InterruptHandler
Description       : SWITCH (GPT0CH1-TIOB) interrupt handler.
                    The interrupt is cleared and the function TIOB1_function()
					of the 创BSP_SWITCH_T创 BSP_SWITCH_a_s structure is called
Inputs            : None
Functions called  : (BSP_SWITCH_a_s.TIOB1_function)(), 创CSP_GPT_GET_SR创, 
                    创CSP_GPT_GET_IMR创, 创CSP_GPT_GET_PDSR创  
Returns           : None
******************************************************************************/
void BSP_SWITCHTIOB1InterruptHandler (void)
{
  
  U32_T status_u32; /* Status Register */
 
 
  /* To clear the INT : read the SR */
  status_u32 = CSP_GPT_GET_SR(GPT0CH1);		

  if( (status_u32 & TIOB & CSP_GPT_GET_IMR(GPT0CH1)) != 0 )
  {
     /* INT raised and PIO pin high */
     if( (CSP_GPT_GET_PDSR(GPT0CH1) & TIOB) != 0)
     {
        /* Call BSP Switch TIOB1 Callback */
        (BSP_SWITCH_a_s.TIOB1_function)();

		/* Wait until switch is released */
		while((CSP_GPT_GET_PDSR(GPT0CH1) & TIOB) != 0);
     }
  }

}

/******************************************************************************
Function          : BSP_SWITCHTIOA2InterruptHandler
Description       : SWITCH (GPT0CH2-TIOA) interrupt handler.
                    The interrupt is cleared and the function TIOA2_function()
					of the 创BSP_SWITCH_T创 BSP_SWITCH_a_s structure is called
Inputs            : None
Functions called  : (BSP_SWITCH_a_s.TIOA2_function)(), 创CSP_GPT_GET_SR创, 
                    创CSP_GPT_GET_IMR创, 创CSP_GPT_GET_PDSR创  
Returns           : None
******************************************************************************/
void BSP_SWITCHTIOA2InterruptHandler (void)
{
  U32_T status_u32; /* Status Register */


  /* To clear the INT : read the SR */
  status_u32 = CSP_GPT_GET_SR(GPT0CH2);		

  if( (status_u32 & TIOA & CSP_GPT_GET_IMR(GPT0CH2)) != 0 )
  {
     /* INT raised and PIO pin high */
     if( (CSP_GPT_GET_PDSR(GPT0CH2) & TIOA) != 0)
     {
        /* Call BSP Switch TIOA2 Callback */
        (BSP_SWITCH_a_s.TIOA2_function)();

	/* Wait until switch is released */
        while(((CSP_GPT_GET_PDSR(GPT0CH2) & TIOA) != 0));
       
     }
  }
  
}

/******************************************************************************
Function          : BSP_SWITCHIRQ0InterruptHandler
Description       : SWITCH (IRQ0) interrupt handler.
                    The function IRQ0_function()
					of the 创BSP_SWITCH_T创 BSP_SWITCH_a_s structure is called
Inputs            : None
Functions called  : (BSP_SWITCH_a_s.IRQ0_function)(), 创CSP_GIC_GET_SMR创, 
Returns           : None
******************************************************************************/
void BSP_SWITCHIRQ0InterruptHandler(void)
{

   /* Is the IRQ switch pressed ? */
   if ((CSP_GIC_GET_SMR(GIC, EXTIRQ0) & SRCTYP) == 0x20)
   {
      /* Yes, call the correponding callback function */
      (BSP_SWITCH_a_s.IRQ0_function)();
   }

}

/******************************************************************************
Function          : BSP_SWITCHInit
Description       : This function initializes the 3 pins connected to switches 
                    to be in input and their interruptions. The interruptions 
                    handlers for each pins are situated in BSP_SWITCH_INT.s. 
                    The assembler handler calls its C handler defined in the 
                    BSP SWITCH library (创BSP_SWITCHIRQ0InterruptHandler创 , 
                    创BSP_SWITCHTIOB1InterruptHandler创, 
                    创BSP_SWITCHTIOA2InterruptHandler创). 
                    Then the C handler calls the function that should be defined 
                    in the structure (BSP_SWITCH_a_s.TIOB1_function for TIOB1 switch).
                    Note : Prior to call BSP_SWITCHInit(), the functions from the 
                    Object BSP_SWITCH_a_s should be user defined.
Inputs            : None
Functions called  : 创CSP_GPTPioInit创, 创CSP_GPTConfigInterrupt创, 创CSP_GICConfigInterrupt创
Returns           : None
******************************************************************************/
void BSP_SWITCHInit(void)
{

   /* 1st Configure the GPT for PIO.The output mask is forced to 0 */
   CSP_GPTPioInit(GPT0CH1, TIOB, 0);
   CSP_GPTPioInit(GPT0CH2, TIOA, 0);
 
   /* Then Enable the INT */
   CSP_GPTConfigInterrupt(GPT0CH1, (HIGH_LEVEL_SENSITIVE | PRIOR_1)  ,TIOB, (U32_T) BSP_AsmTIOB1InterruptHandler);
   CSP_GPTConfigInterrupt(GPT0CH2, (HIGH_LEVEL_SENSITIVE | PRIOR_0)  ,TIOA, (U32_T) BSP_AsmTIOA2InterruptHandler);

   /* IRQ0 */
   CSP_GICConfigInterrupt(EXTIRQ0, (NEGATIVE_EDGE_TRIGGERED | PRIOR_0), (U32_T) BSP_AsmIRQ0InterruptHandler);
}


