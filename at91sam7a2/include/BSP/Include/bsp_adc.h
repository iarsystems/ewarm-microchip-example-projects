/***************************************************************************************************
FILE        BSP_ADC.H

VERSION     1.00

AUTHORS     Frederic SAMSON (Europe Technologies)

COPYRIGHT   (c) Europe Technologies
            COMMERCIAL IN CONFIDENCE

CONTAINS    Definitions and Standard Typedef for EVMCAN
            Analog Input. Board Support Package Layer

MODIFICATION HISTORY

 * 1  07/08/2001  Frederic SAMSON
        - Initial File
****************************************************************************/

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

#define BSP_ADC8_VREF              3300            /* Vref = 3300 mV                */
#define BSP_ADC8_OFFSET            0               /* Offset needed to convert      */
                                                   /* Digital to Analog (= 0 V)     */
#define BSP_ADC8_DIV               1024            /* Diviser needed to convert     */
                                                   /* Digital to Analog             */


/****************************************/
/* BSP ADC Callback Definition          */
/****************************************/
typedef void *(BSP_ADC_CALLBACK_T)(void);


/********************************************************************************
* BSP ADC Inputs Structure Definition
********************************************************************************/
typedef struct
{
  U16_T                analog_input[8];       /* ADC Input in mV (0->3300 mV)  */
  U16_T                digital_input[8];      /* Digital Input (0->1024)       */
  BSP_ADC_CALLBACK_T   *mailbox_function;     /* callback function             */
} BSP_ADC_INPUT_T, *BSP_ADC_INPUT_PTR;


extern BSP_ADC_INPUT_T BSP_adc8_a_s[2];

/********************************************************************************
* BSP ADC External Asm Functions Declaration
********************************************************************************/
extern void ADC0_AsmHandler(void);
extern void ADC1_AsmHandler(void);
extern void ADC0_Handler(void);
extern void ADC1_Handler(void);


/********************************************************************************
* BSP ADC External Functions Declaration
********************************************************************************/
extern void BSP_ADCInit(void);

#endif  /* BSP_ADC_H */
