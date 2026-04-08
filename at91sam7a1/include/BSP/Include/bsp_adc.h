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
* Processor   : SmartCAN1 for EVMSmartCAN
* File Name   : BSP_ADC.h
* Description : Definitions and Standard Typedef for
                Analog Input. (Board Support Package Layer)
* Version     : 1.01
*
*       +----- (NEW | MODify | ADD | DELete)
*       |
*  No   |   when       who                what
*-----+---+----------+------------------+--------------------------------------
* 000  NEW  07/08/01   Frederic SAMSON    Creation
* 001  MOD  21/11/01   Bruno SALLE        ADC1 suppression
*----------------------------------------------------------------------------*/

#ifndef BSP_ADC_H
#define BSP_ADC_H


/* Start up Time - Number of Master Clock period to make at least 4 microseconds */
#define BSP_ADC4_STARTUPIME_20MHZ  (0x80000000)
#define BSP_ADC4_PRLVAL_20MHZ      (8 << 16)
#define BSP_ADC8_STARTUPIME_20MHZ  (0x8000)
#define BSP_ADC8_PRLVAL_20MHZ      (8)
/* Number of Master Clock to make 6 microseconds */
#define BSP_ADC8_STARTUPIME_30MHZ  (0xB4 << 8)
#define BSP_ADC8_PRLVAL_30MHZ      (0xC)

#define BSP_ADC8_CV1               (0x07 << 0)     /* CV1 mask                      */
#define BSP_ADC8_CV2               (0x07 << 4)     /* CV2 mask                      */
#define BSP_ADC8_CV3               (0x07 << 8)     /* CV3 mask                      */
#define BSP_ADC8_CV4               (0x07 << 12)    /* CV4 mask                      */
#define BSP_ADC8_CV5               (0x07 << 16)    /* CV5 mask                      */
#define BSP_ADC8_CV6               (0x07 << 20)    /* CV6 mask                      */
#define BSP_ADC8_CV7               (0x07 << 24)    /* CV7 mask                      */
#define BSP_ADC8_CV8               (0x07 << 28)    /* CV8 mask                      */

#define BSP_ADC8_CV1INA0           (0x00 << 0)     /* CV1 Input 0                   */
#define BSP_ADC8_CV2INA0           (0x00 << 4)     /* CV2 Input 0                   */
#define BSP_ADC8_CV3INA0           (0x00 << 8)     /* CV3 Input 0                   */
#define BSP_ADC8_CV4INA0           (0x00 << 12)    /* CV4 Input 0                   */
#define BSP_ADC8_CV5INA0           (0x00 << 16)    /* CV5 Input 0                   */
#define BSP_ADC8_CV6INA0           (0x00 << 20)    /* CV6 Input 0                   */
#define BSP_ADC8_CV7INA0           (0x00 << 24)    /* CV7 Input 0                   */
#define BSP_ADC8_CV8INA0           (0x00 << 28)    /* CV8 Input 0                   */


#define BSP_ADC8_CV1INA1           (0x01 << 0)     /* CV1 Input 1                   */
#define BSP_ADC8_CV2INA1           (0x01 << 4)     /* CV2 Input 1                   */
#define BSP_ADC8_CV3INA1           (0x01 << 8)     /* CV3 Input 1                   */
#define BSP_ADC8_CV4INA1           (0x01 << 12)    /* CV4 Input 1                   */
#define BSP_ADC8_CV5INA1           (0x01 << 16)    /* CV5 Input 1                   */
#define BSP_ADC8_CV6INA1           (0x01 << 20)    /* CV6 Input 1                   */
#define BSP_ADC8_CV7INA1           (0x01 << 24)    /* CV7 Input 1                   */
#define BSP_ADC8_CV8INA1           (0x01 << 28)    /* CV8 Input 1                   */


#define BSP_ADC8_VREF              3489            /* Vref = 3300 mV                */
#define BSP_ADC8_OFFSET            0               /* Offset needed to convert      */
                                                   /* Digital to Analog (= 0 V)     */
#define BSP_ADC8_DIV               1024            /* Diviser needed to convert     */
                                                   /* Digital to Analog             */


/****************************************/
/* BSP ADC Callback Definition          */
/****************************************/

/*******************************************************************************
Type: BSP_ADC_CALLBACK_T
Description: BSP ADC Callback Definition
Declaration: typedef *(BSP_ADC_CALLBACK_T) (void);
Inputs: None
********************************************************************************/
typedef void *(BSP_ADC_CALLBACK_T) (void);

/********************************************************************************
* BSP ADC Inputs Structure Definition
********************************************************************************/

/*******************************************************************************
Type: BSP_ADC_INPUT_T
Description: BSP ADC Inputs Structure Definition
Fields:
- 创U16_T创                analog_input[8] :       ADC Input in mV (0->3300 mV)
- 创U16_T创                digital_input[8] :      Digital Input (0->1023)
- 创BSP_ADC_CALLBACK_T创   *mailbox_function :     callback function
*******************************************************************************/
typedef struct
{
  U16_T                analog_input[8];       /* ADC Input in mV (0->3300 mV)  */
  U16_T                digital_input[8];      /* Digital Input (0->1024)       */
  BSP_ADC_CALLBACK_T   *mailbox_function;     /* callback function             */
} BSP_ADC_INPUT_T, *BSP_ADC_INPUT_PTR;


extern BSP_ADC_INPUT_T BSP_adc8_a_s;

/********************************************************************************
* BSP ADC External Asm Functions Declaration
********************************************************************************/
extern void ADC0_AsmHandler(void);
extern void ADC0_Handler(void);



/********************************************************************************
* BSP ADC External Functions Declaration
********************************************************************************/
extern void BSP_ADCInit(void);

#endif  /* BSP_ADC_H */
