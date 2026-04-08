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
* File Name       : csp_adc8c.h
* Description     : Definitions, Macros and function declarations for
*                   Analog to Digital Converter 8 channels module
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
* 004  MOD  23/10/01   Christophe GARDIN  Clean Up
* 005  MOD  15/03/02   Christophe GARDIN  Add macros and bits
*----------------------------------------------------------------------------*/

#ifndef CSP_ADC8C_H
#define CSP_ADC8C_H


/******************************************************************************
************************* ADC8C Structure Definition **************************
******************************************************************************/
typedef struct
{
   CSP_REGISTER_T  ReservedA[20];
   CSP_REGISTER_T  ECR;                /* Clock Enable Register              */
   CSP_REGISTER_T  DCR;                /* Clock Disable Register             */
   CSP_REGISTER_T  PMSR;               /* Power Management Status Register   */
   CSP_REGISTER_T  ReservedB;
   CSP_REGISTER_T  CR;                 /* Control Register                   */
   CSP_REGISTER_T  MR;                 /* Mode Register                      */ 
   CSP_REGISTER_T  CMR;                /* Conversion Mode Register           */ 
   CSP_REGISTER_T  CSR;                /* Clear Status Register              */ 
   CSP_REGISTER_T  SR;                 /* Status Register                    */
   CSP_REGISTER_T  IER;                /* Interrupt Enable Register          */
   CSP_REGISTER_T  IDR;                /* Interrupt Disable Register         */ 
   CSP_REGISTER_T  IMR;                /* Interrupt Mask Register            */
   CSP_REGISTER_T  DR;                 /* Convert Data Register              */ 
   CSP_REGISTER_T  ReservedC[2]; 
   CSP_REGISTER_T  TSTR;               /* Test Mode Register                 */
} CSP_ADC8C_T, *CSP_ADC8C_PTR;


/******************************************************************************
************************* ADC8C Registers Definition **************************
******************************************************************************/

/******************************************************************************
* ECR, DCR, PMSR : ADC8C Power Management Registers
******************************************************************************/
#define ADC             (0x01ul << 1)           /* ADC Clock                 */

/******************************************************************************
* CR : ADC8C Control Register
******************************************************************************/
#define SWRST           (0x01ul << 0)           /* Software Reset            */
#define ADCEN           (0x01ul << 1)           /* ADC Enable                */
#define ADCDIS          (0x01ul << 2)           /* ADC Disable               */
#define START           (0x01ul << 3)           /* Start Conversion          */
#define STOP            (0x01ul << 4)           /* Stop Conversion           */

/******************************************************************************
* MR : ADC8C Mode Register
******************************************************************************/
/* PRVAL : Preload Value                                                     */
#define PRLVAL_MASK     (0x1Ful << 0)           /* Mask                      */
#define PRLVAL(val)     (((val) & 0x1Ful) << 0) /* Writing Macro             */

#define STOPEN          (0x01ul << 6)           /* Stop Enable               */

/* STARTTIME : Start-up Time value                                           */
#define STARTUPTIME_MASK   (0xFFul << 8)           /* Mask                   */
#define STARTUPTIME(val)   (((val) & 0xFFul) << 8) /* Writing Macro          */

/* NBRCH : Number of conversions                                             */
#define NBRCH           (0x07ul << 16)          /* NBRCH Mask                */
#define NBRCH1          (0x00ul << 16)          /* 1 conversion              */
#define NBRCH2          (0x01ul << 16)          /* 2 conversions             */
#define NBRCH3          (0x02ul << 16)          /* 3 conversions             */
#define NBRCH4          (0x03ul << 16)          /* 4 conversions             */
#define NBRCH5          (0x04ul << 16)          /* 5 conversions             */
#define NBRCH6          (0x05ul << 16)          /* 6 conversions             */
#define NBRCH7          (0x06ul << 16)          /* 7 conversions             */
#define NBRCH8          (0x07ul << 16)          /* 8 conversions             */

#define CONTCV          (0x01ul << 19)          /* Continuous Conversion     */

/******************************************************************************
* CMR : ADC8C Conversion Mode Register
******************************************************************************/
/* CV : Input Selection                                                      */
#define CV1             (0x03ul << 0)           /* CV1 Mask                  */
#define CV2             (0x03ul << 4)           /* CV2 Mask                  */
#define CV3             (0x03ul << 8)           /* CV3 Mask                  */
#define CV4             (0x03ul << 12)          /* CV4 Mask                  */
#define CV5             (0x03ul << 16)          /* CV5 Mask                  */
#define CV6             (0x03ul << 20)          /* CV6 Mask                  */
#define CV7             (0x03ul << 24)          /* CV7 Mask                  */
#define CV8             (0x03ul << 28)          /* CV8 Mask                  */

/* CV1 Input Selection                                                       */
#define CV1IN0          (0x00ul << 0)           /* Input 0                   */
#define CV1IN1          (0x01ul << 0)           /* Input 1                   */
#define CV1IN2          (0x02ul << 0)           /* Input 2                   */
#define CV1IN3          (0x03ul << 0)           /* Input 3                   */
#define CV1IN4          (0x04ul << 0)           /* Input 4                   */
#define CV1IN5          (0x05ul << 0)           /* Input 5                   */
#define CV1IN6          (0x06ul << 0)           /* Input 6                   */
#define CV1IN7          (0x07ul << 0)           /* Input 7                   */

/* CV2 Input Selection                                                       */
#define CV2IN0          (0x00ul << 4)           /* Input 0                   */
#define CV2IN1          (0x01ul << 4)           /* Input 1                   */
#define CV2IN2          (0x02ul << 4)           /* Input 2                   */
#define CV2IN3          (0x03ul << 4)           /* Input 3                   */
#define CV2IN4          (0x04ul << 4)           /* Input 4                   */
#define CV2IN5          (0x05ul << 4)           /* Input 5                   */
#define CV2IN6          (0x06ul << 4)           /* Input 6                   */
#define CV2IN7          (0x07ul << 4)           /* Input 7                   */

/* CV3 Input Selection                                                       */
#define CV3IN0          (0x00ul << 8)           /* Input 0                   */
#define CV3IN1          (0x01ul << 8)           /* Input 1                   */
#define CV3IN2          (0x02ul << 8)           /* Input 2                   */
#define CV3IN3          (0x03ul << 8)           /* Input 3                   */
#define CV3IN4          (0x04ul << 8)           /* Input 4                   */
#define CV3IN5          (0x05ul << 8)           /* Input 5                   */
#define CV3IN6          (0x06ul << 8)           /* Input 6                   */
#define CV3IN7          (0x07ul << 8)           /* Input 7                   */

/* CV4 Input Selection                                                       */
#define CV4IN0          (0x00ul << 12)          /* Input 0                   */
#define CV4IN1          (0x01ul << 12)          /* Input 1                   */
#define CV4IN2          (0x02ul << 12)          /* Input 2                   */
#define CV4IN3          (0x03ul << 12)          /* Input 3                   */
#define CV4IN4          (0x04ul << 12)          /* Input 4                   */
#define CV4IN5          (0x05ul << 12)          /* Input 5                   */
#define CV4IN6          (0x06ul << 12)          /* Input 6                   */
#define CV4IN7          (0x07ul << 12)          /* Input 7                   */

/* CV5 Input Selection                                                       */
#define CV5IN0          (0x00ul << 16)          /* Input 0                   */
#define CV5IN1          (0x01ul << 16)          /* Input 1                   */
#define CV5IN2          (0x02ul << 16)          /* Input 2                   */
#define CV5IN3          (0x03ul << 16)          /* Input 3                   */
#define CV5IN4          (0x04ul << 16)          /* Input 4                   */
#define CV5IN5          (0x05ul << 16)          /* Input 5                   */
#define CV5IN6          (0x06ul << 16)          /* Input 6                   */
#define CV5IN7          (0x07ul << 16)          /* Input 7                   */

/* CV6 Input Selection                                                       */
#define CV6IN0          (0x00ul << 20)          /* Input 0                   */
#define CV6IN1          (0x01ul << 20)          /* Input 1                   */
#define CV6IN2          (0x02ul << 20)          /* Input 2                   */
#define CV6IN3          (0x03ul << 20)          /* Input 3                   */
#define CV6IN4          (0x04ul << 20)          /* Input 4                   */
#define CV6IN5          (0x05ul << 20)          /* Input 5                   */
#define CV6IN6          (0x06ul << 20)          /* Input 6                   */
#define CV6IN7          (0x07ul << 20)          /* Input 7                   */

/* CV7 Input Selection                                                       */
#define CV7IN0          (0x00ul << 24)          /* Input 0                   */
#define CV7IN1          (0x01ul << 24)          /* Input 1                   */
#define CV7IN2          (0x02ul << 24)          /* Input 2                   */
#define CV7IN3          (0x03ul << 24)          /* Input 3                   */
#define CV7IN4          (0x04ul << 24)          /* Input 4                   */
#define CV7IN5          (0x05ul << 24)          /* Input 5                   */
#define CV7IN6          (0x06ul << 24)          /* Input 6                   */
#define CV7IN7          (0x07ul << 24)          /* Input 7                   */

/* CV8 Input Selection                                                       */
#define CV8IN0          (0x00ul << 28)          /* Input 0                   */
#define CV8IN1          (0x01ul << 28)          /* Input 1                   */
#define CV8IN2          (0x02ul << 28)          /* Input 2                   */
#define CV8IN3          (0x03ul << 28)          /* Input 3                   */
#define CV8IN4          (0x04ul << 28)          /* Input 4                   */
#define CV8IN5          (0x05ul << 28)          /* Input 5                   */
#define CV8IN6          (0x06ul << 28)          /* Input 6                   */
#define CV8IN7          (0x07ul << 28)          /* Input 7                   */

#define ADCIN0          0u                      /* ADC Analog Input 0        */
#define ADCIN1          1u                      /* ADC Analog Input 1        */
#define ADCIN2          2u                      /* ADC Analog Input 2        */
#define ADCIN3          3u                      /* ADC Analog Input 3        */
#define ADCIN4          4u                      /* ADC Analog Input 4        */
#define ADCIN5          5u                      /* ADC Analog Input 5        */
#define ADCIN6          6u                      /* ADC Analog Input 6        */
#define ADCIN7          7u                      /* ADC Analog Input 7        */

/******************************************************************************
* CSR, SR, IER, IDR, IMR : ADC8C Status Registers and Interrupt Registers
******************************************************************************/
#define OVR             (0x01ul << 2)           /* Over Run                  */
#define ADC_TEND        (0x01ul << 3)           /* PDC Transfer End          */

/* SR, IER, IDR, IMR Registers only                                          */
#define EOC             (0x01ul << 0)           /* End Of Conversion         */
#define READY           (0x01ul << 1)           /* Ready to Start            */

/* SR Register Only                                                          */
#define ACDEN           (0x01ul << 8)           /* ADC Enable Status         */
#define CTCVS           (0x01ul << 9)           /* Continuous Conversion Status */

/******************************************************************************
* DR : ADC8C Convert Data Register  
******************************************************************************/
#define DATA            (0x3FFul << 0)          /* ADC10 Converted Data Mask */

/******************************************************************************
* TSTR : ADC8C Test Mode Register
******************************************************************************/
#define TEST            (0x01ul << 0)           /* Test Mode                 */


/******************************************************************************
************************** ADC8C Macros Definition ****************************
******************************************************************************/
/* ECR, DCR, PMSR : ADC8C Power Management Registers                         */
#define CSP_ADC8C_SET_ECR(adc, val)    ((adc)->ECR = (val))    /* Enable     */
#define CSP_ADC8C_SET_DCR(adc, val)    ((adc)->DCR = (val))    /* Disable    */
#define CSP_ADC8C_GET_PMSR(adc)        ((adc)->PMSR)           /* Status     */

/* CR : ADC8C Control Register                                               */
#define CSP_ADC8C_SET_CR(adc, val)     ((adc)->CR = (val))

/* MR : ADC8C Mode Register                                                  */
#define CSP_ADC8C_GET_MR(adc)          ((adc)->MR)
#define CSP_ADC8C_SET_MR(adc, mode)    ((adc)->MR = (mode))

/* CMR : ADC8C Conversion Mode Register                                      */
#define CSP_ADC8C_GET_CMR(adc)         ((adc)->CMR)
#define CSP_ADC8C_SET_CMR(adc, val)    ((adc)->CMR = (val))

/* CSR, SR : ADC8C Status Registers                                          */
#define CSP_ADC8C_SET_CSR(adc, val)    ((adc)->CSR = (val))    /* Clear      */
#define CSP_ADC8C_GET_SR(adc)          ((adc)->SR)             /* Status     */

/* IER, IDR, IMR : ADC8C Interrupt Registers                                 */
#define CSP_ADC8C_SET_IER(adc, val)    ((adc)->IER = (val))    /* Enable     */
#define CSP_ADC8C_SET_IDR(adc, val)    ((adc)->IDR = (val))    /* Disable    */
#define CSP_ADC8C_GET_IMR(adc)         ((adc)->IMR)            /* Mask       */

/* DR : ADC8C Convert Data Register                                          */
#define CSP_ADC8C_GET_DR(adc)          ((adc)->DR)

/* TSTR : ADC8C Test Mode Register                                           */
#define CSP_ADC8C_GET_TSTR(adc)        ((adc)->TSTR)
#define CSP_ADC8C_SET_TSTR(adc, val)   ((adc)->TSTR = (val))


/******************************************************************************
******************** ADC8C External Functions Declaration *********************
******************************************************************************/
extern void CSP_ADC8CInit(CSP_ADC8C_T *const adc, U32_T mode);
extern void CSP_ADC8CClose(CSP_ADC8C_T *const adc);
extern void CSP_ADC8CConfigInterrupt(CSP_ADC8C_T *const adc, U32_T int_mode, U32_T int_mask, U32_T callback);
extern void CSP_ADC8CStartConversion(CSP_ADC8C_T *const adc, U16_T *data, U32_T order_conversion, U8_T nb_conversion);
extern void CSP_ADC8CStopConversion(CSP_ADC8C_T *const adc);
extern void CSP_ADC8CEnable(CSP_ADC8C_T *const adc);
extern void CSP_ADC8CDisable(CSP_ADC8C_T *const adc);


#endif   /* CSP_ADC8C_H */
