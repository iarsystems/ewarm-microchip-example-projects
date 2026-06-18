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
* File Name       : csp_adc8c.c
* Description     : Function declarations for Analog to Digital Converter 
*                   8 channels management
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
* 003  ADD  07/08/01   Frederic SAMSON    Add CSP_ADCClearInterrupt function
* 004  MOD  30/10/01   Christophe GARDIN  Clean Up
* 005  MOD  11/04/02   Christophe GARDIN  Add CSP_ADC8CClose function
*----------------------------------------------------------------------------*/

/******************************************************************************
* Include Files
******************************************************************************/
#include "csp.h"


/******************************************************************************
****************************  F U N C T I O N S  ******************************
******************************************************************************/

/******************************************************************************
* Function          : CSP_ADC8CInit
* Description       : Switch on the clock, reset the registers and configure 
*                     the ADC module mode and the PDC RX line
* Inputs            : <*adc> = Pointer to ADC structure
*                     <mode> = Configure the ADC mode
* Functions called  : CSP_PDCInitRx
* Returns           : None
******************************************************************************/
void CSP_ADC8CInit(CSP_ADC8C_T *const adc, U32_T mode)
{
   /* Enable ADC Clock */  
   CSP_ADC8C_SET_ECR(adc, ADC);
  
   /* ADC Software Reset */  
   CSP_ADC8C_SET_CR(adc, SWRST);
  
   /* Configure ADC Mode */
   CSP_ADC8C_SET_MR(adc, mode);
                        
   /* Configure CAPTURE PDC Rx Line */
   CSP_PDCInitRx((U32_T)adc, (U32_T) &(adc->DR), SIZE_HALFWORD);
}


/******************************************************************************
* Function          : CSP_ADC8CClose
* Description       : Reset and switch off the clock
* Inputs            : <*adc> = Pointer to ADC structure
* Functions called  : None
* Returns           : None
******************************************************************************/
void CSP_ADC8CClose(CSP_ADC8C_T *const adc)
{
   /* ADC Software Reset */  
   CSP_ADC8C_SET_CR(adc, SWRST);

   /* Disable ADC Clock */  
   CSP_ADC8C_SET_DCR(adc, ADC);
}


/******************************************************************************
* Function          : CSP_ADC8CConfigInterrupt
* Description       : Configure ADC Interrupts
* Inputs            : <*adc>     = Pointer to ADC structure
*                     <int_mode> = Configure the priority level and source type
*                     <int_mask> = Configure which interrupt bits are activated
*                     <callback> = Function called through the assembler interrupt handler
* Functions called  : CSP_GET_INTERRUPT_NUMBER, CSP_GICConfigInterrupt
* Returns           : None
******************************************************************************/
void CSP_ADC8CConfigInterrupt(CSP_ADC8C_T *const adc, 
                              U32_T int_mode, 
                              U32_T int_mask, 
                              U32_T callback)
{
   /* Local Variables */
   U8_T int_num = 0;

   /* Get Peripheral Interrupt Number */
   int_num = CSP_GET_INTERRUPT_NUMBER(adc);

   /* Disable all interrupt */
   CSP_ADC8C_SET_IDR(adc, 0xFFFFFFFF);
      
   /* Interrupt Enable */
   CSP_ADC8C_SET_IER(adc, int_mask);

   /* Configure ADC controller interrupt mode in GIC module */
   CSP_GICConfigInterrupt(int_num, int_mode, callback);
}


/******************************************************************************
* Function          : CSP_ADC8CEnable
* Description       : Enable ADC
* Preconditions     : You must wait the set of the READY bit after an enable 
*                     command (4 µs) which corresponds to the initialization time 
*                     of the analog part.
* Inputs            : <*adc> = Pointer to ADC structure
* Functions called  : None
* Returns           : None
******************************************************************************/
void CSP_ADC8CEnable(CSP_ADC8C_T *const adc)
{
   /* Enable ADC */
   CSP_ADC8C_SET_CR(adc, ADCEN);
}


/******************************************************************************
* Function          : CSP_ADC8CDisable
* Description       : Disable ADC
* Inputs            : <*adc> = Pointer to ADC structure
* Functions called  : None
* Returns           : None
******************************************************************************/
void CSP_ADC8CDisable(CSP_ADC8C_T *const adc)
{
   /* Disable ADC */  
   CSP_ADC8C_SET_CR(adc, ADCDIS);
}


/******************************************************************************
* Function          : CSP_ADC8CStartConversion
* Description       : Configure ADC module, start conversions on 10 bits ADC in 
*                     the chosen order, transfer results to memory thanks to PDC
* Inputs            : <*adc>  = Pointer to ADC structure
*                     <*data> = Pointer to the address where the converted 
*                               values will be stored
*                     <order_conversion> = Configure the order of conversion 
*                                          (eg: CV1 to CV8)
*                     <nb_conversion> = Number of conversion [0-7] to be carried out
* Functions called  : CSP_PDCStartRx
* Returns           : None
******************************************************************************/
void CSP_ADC8CStartConversion(CSP_ADC8C_T *const adc, 
                              U16_T *data, 
                              U32_T order_conversion, 
                              U8_T nb_conversion)
{
   /* Clear Number of Conversion */
   CSP_ADC8C_SET_MR(adc, (CSP_ADC8C_GET_MR(adc) & ~NBRCH));
  
   /* Set New Input Configuration */
   CSP_ADC8C_SET_MR(adc, (CSP_ADC8C_GET_MR(adc) | 
                              ((((U32_T)nb_conversion - 1ul) << 16) & NBRCH)));

   /* Clear Input Configuration */
   CSP_ADC8C_SET_CMR(adc, 0);
      
   /* Set New Input Configuration */
   CSP_ADC8C_SET_CMR(adc, order_conversion);
    
   /* Configure PDC Rx Line to Receive Data */
   CSP_PDCStartRx((U32_T)adc, (U32_T)data, nb_conversion);

   /* Start Conversion */
   CSP_ADC8C_SET_CR(adc, START);
}


/******************************************************************************
* Function          : CSP_ADC8CStopConversion
* Description       : Stop Current Conversion
* Inputs            : <*adc> = Pointer to ADC structure
* Functions called  : None
* Returns           : None
******************************************************************************/
void CSP_ADC8CStopConversion(CSP_ADC8C_T *const adc)
{
   /* Stop Conversion */
   CSP_ADC8C_SET_CR(adc, STOP);
}


