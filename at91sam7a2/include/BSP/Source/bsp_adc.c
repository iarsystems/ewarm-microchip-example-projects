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
BSP_ADC_INPUT_T BSP_adc8_a_s[2];


/***************************************************************************
Function    : BSP_ADCInit
Description :
Inputs      : None
Outputs     : None
***************************************************************************/
void BSP_ADCInit(void)
{
   /* Init ADC0 and ADC0 - 30Mhz Master Clock - All inputs selected and single conversion mode */
   CSP_ADC8CInit(ADC0, (BSP_ADC8_STARTUPIME_30MHZ) | (BSP_ADC8_PRLVAL_30MHZ) | (NBRCH8));   
   CSP_ADC8CInit(ADC1, (BSP_ADC8_STARTUPIME_30MHZ) | (BSP_ADC8_PRLVAL_30MHZ) | (NBRCH8));

   /* Enable ADC0 and ADC1*/
   CSP_ADC8CEnable(ADC0);   
   CSP_ADC8CEnable(ADC1);   

   /* Wait for ADC0 and ADC1 are ready to convert*/
   while(((CSP_ADC8C_GET_SR(ADC0) & READY) == 0) && (((CSP_ADC8C_GET_SR(ADC1) & READY) == 0)));

   /* Configure ADC0 and ADC1 Interruption on ADC_TEND */
   CSP_ADC8CConfigInterrupt(ADC0, (POSITIVE_EDGE_TRIGGERED|PRIOR_1), ADC_TEND, (U32_T) ADC0_AsmHandler);
   CSP_ADC8CConfigInterrupt(ADC1, (POSITIVE_EDGE_TRIGGERED|PRIOR_1), ADC_TEND, (U32_T) ADC1_AsmHandler);
}


/***************************************************************************
Function    : ADC0_Handler
Description :
Inputs      : None
Outputs     : None
***************************************************************************/
void ADC0_Handler(void)
{
   /* Local variable */
   U8_T i_u8;
   
   /*Digital to Analog calculation - Read the Data Register */			     
   for (i_u8=0; i_u8<8; i_u8++)
   {
		BSP_adc8_a_s[0].analog_input[i_u8] = BSP_ADC8_VREF * BSP_adc8_a_s[0].digital_input[i_u8] / BSP_ADC8_DIV + BSP_ADC8_OFFSET;
   }
   /* Clear the TEND in CSR register */
   CSP_ADC8C_SET_CSR(ADC0, ADC_TEND);

   (BSP_adc8_a_s[0].mailbox_function) () ;

}

/***************************************************************************
Function    : ADC1_Handler
Description :
Inputs      : None
Outputs     : None
***************************************************************************/
void ADC1_Handler(void)
{
   /* Local variable */
	U8_T i_u8;
	
   /* Digital to Analog conversion - Read the Data Register */
   for (i_u8=0; i_u8<8; i_u8++)
   {
   	BSP_adc8_a_s[1].analog_input[i_u8] = BSP_ADC8_VREF * BSP_adc8_a_s[1].digital_input[i_u8] / BSP_ADC8_DIV + BSP_ADC8_OFFSET;
   }
   /* Clear the TEND in CSR register */
   CSP_ADC8C_SET_CSR(ADC1, ADC_TEND);

   (BSP_adc8_a_s[1].mailbox_function) () ;
}

