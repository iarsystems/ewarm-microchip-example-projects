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
* File Name   : bsp_adc.c
* Description : Function declarations for ADC Management
* Version     : 1.01
*
*       +----- (NEW | MODify | ADD | DELete)                                 
*       |                                                                    
*  No   |   when       who                what               
*-----+---+----------+------------------+--------------------------------------
* 000  NEW  07/08/01   Frederic SAMSON    Creation
* 002  ADD  17/10/01   Olivier MAZUYER    Add the Digital to Analog calculation
*														for each channels    
* 003  MOD  31/01/02   Mahmoud Mesgarzadeh   Clean Up
*----------------------------------------------------------------------------*/


/***************************************************************************
* Include Directives
***************************************************************************/
#include "csp.h"
#include "bsp.h"

/***************************************************************************
* Global Variables
***************************************************************************/
BSP_ADC_INPUT_T BSP_adc8_a_s;

/***************************************************************************
Function    : BSP_ADCInit
Description : This function initializes and enables the ADC with their 
              interruptions. The interruption handler for each ADC are 
              situated in bsp_adc_int.s. The assembler handler calls its C 
              handler defined in the BSP ADC library 
              (ADC0_Handler) which calculates the analogue 
              value from the captured digital value in the structure. 
              Then the C handler calls the function that should be defined 
              in the structure (BSP_adc8_a_s.mailbox_function)
              Note : 
- Prior to start the ADC, the mailbox functions should be user defined.
- To start the ADC, the CSP_ADC8StartConversion function from the CSP should 
be called by the user
Functions called  : 创CSP_ADC8CInit创, 创CSP_ADC8CEnable创, 创CSP_ADC8C_GET_SR创 
                    创CSP_ADC8CConfigInterrupt创
Inputs      : None
Returns     : None
***************************************************************************/
void BSP_ADCInit(void)
{
   /* Init ADC0 - 30Mhz Master Clock - All inputs selected and single conversion mode */
   CSP_ADC8CInit(ADC0, (BSP_ADC8_STARTUPIME_30MHZ) | (BSP_ADC8_PRLVAL_30MHZ) | (NBRCH8));   

   /* Enable ADC0 */
   CSP_ADC8CEnable(ADC0);   

   /* Wait for ADC0 is ready to convert*/
   while((CSP_ADC8C_GET_SR(ADC0) & READY) == 0);

   /* Configure ADC0 Interruption on ADC_TEND */
   CSP_ADC8CConfigInterrupt(ADC0, (POSITIVE_EDGE_TRIGGERED|PRIOR_1), ADC_TEND, (U32_T) ADC0_AsmHandler);
}


/***************************************************************************
Function    : ADC0_Handler
Description : ADC interrupt handler ; for each channel, calculate the input
              voltage versus the voltage reference BSP_ADC8_VREF and the 
              offset BSP_ADC8_OFFSET.
              The result are saved in the analog_input[] table of the
              创BSP_ADC_INPUT创 BSP_adc8_a_s structure of which the 
              mailbox_function() is then called			  
Functions called  : 创CSP_ADC8C_SET_CSR创, (BSP_adc8_a_s.mailbox_function) () 
Inputs      : None
Returns     : None
***************************************************************************/
void ADC0_Handler(void)
{
   /* Local variable */
   U8_T i_u8;
   
   /*Digital to Analog calculation - Read the Data Register */			     
   for (i_u8=0; i_u8<8; i_u8++)
   {
		BSP_adc8_a_s.analog_input[i_u8] = BSP_ADC8_VREF * BSP_adc8_a_s.digital_input[i_u8] / BSP_ADC8_DIV + BSP_ADC8_OFFSET;
   }
   /* Clear the TEND in CSR register */
   CSP_ADC8C_SET_CSR(ADC0, ADC_TEND);

   (BSP_adc8_a_s.mailbox_function) () ;

}


