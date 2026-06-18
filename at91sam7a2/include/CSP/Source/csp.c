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
* Processor       : easyCAN4                          
* File Name       : csp.c
* Description     : Structure for FCM Chip easyCAN4
* Library Version : 1.00
*
*       +----- (NEW | MODify | ADD | DELete)                                 
*       |                                                                    
*  No   |   When       Who                What               
*-----+---+----------+------------------+--------------------------------------
* 000  NEW  17/07/02   Christophe GARDIN  Creation
*----------------------------------------------------------------------------*/

/******************************************************************************
* Include Files
******************************************************************************/
#include "csp.h"


/******************************************************************************
* Peripheral Declaration 
******************************************************************************/
CSP_AMC_T   *const AMC     = ((CSP_AMC_T*)   AMC_BASE_ADDRESS);    /* Advanced Memory Controller  */     
CSP_SFM_T   *const SFM     = ((CSP_SFM_T*)   SFM_BASE_ADDRESS);    /* Special Function Module     */    
CSP_WD_T    *const WD      = ((CSP_WD_T*)    WD_BASE_ADDRESS);     /* WATCHDOG                    */    
CSP_WT_T    *const WT      = ((CSP_WT_T*)    WT_BASE_ADDRESS);     /* WATCH TIMER                 */    
CSP_USART_T *const USART0  = ((CSP_USART_T*) USART0_BASE_ADDRESS); /* USART0                      */ 
CSP_USART_T *const USART1  = ((CSP_USART_T*) USART1_BASE_ADDRESS); /* USART1                      */ 
CSP_CAN_T   *const CAN3    = ((CSP_CAN_T*)   CAN3_BASE_ADDRESS);   /* CAN3 (16 channels)          */ 
CSP_SPI_T   *const SPI0    = ((CSP_SPI_T*)   SPI0_BASE_ADDRESS);   /* SPI0                        */ 
CSP_CAN_T   *const CAN1    = ((CSP_CAN_T*)   CAN1_BASE_ADDRESS);   /* CAN1 (16 channels)          */ 
CSP_CAN_T   *const CAN2    = ((CSP_CAN_T*)   CAN2_BASE_ADDRESS);   /* CAN2 (32 channels)          */ 
CSP_ADC8C_T *const ADC0    = ((CSP_ADC8C_T*) ADC0_BASE_ADDRESS);   /* ADC0 (8 channels)           */ 
CSP_ADC8C_T *const ADC1    = ((CSP_ADC8C_T*) ADC1_BASE_ADDRESS);   /* ADC1 (8 channels)           */ 
CSP_GPT3C_T *const GPT0    = ((CSP_GPT3C_T*) GPT0_BASE_ADDRESS);   /* 16-bit Timer T0 (3 channels)*/ 
CSP_GPT_T   *const GPT0CH0 = ((CSP_GPT_T*)   GPT0CH0_BASE_ADDRESS);/* 16-bit Timer T0 channel 0   */ 
CSP_GPT_T   *const GPT0CH1 = ((CSP_GPT_T*)   GPT0CH1_BASE_ADDRESS);/* 16-bit Timer T0 channel 1   */ 
CSP_GPT_T   *const GPT0CH2 = ((CSP_GPT_T*)   GPT0CH2_BASE_ADDRESS);/* 16-bit Timer T0 channel 2   */ 
CSP_GPT1C_T *const GPT1    = ((CSP_GPT1C_T*) GPT1_BASE_ADDRESS);   /* 16-bit Timer T1 (1 channel) */ 
CSP_GPT_T   *const GPT1CH0 = ((CSP_GPT_T*)   GPT1CH0_BASE_ADDRESS);/* 16-bit Timer T1 channel 0   */ 
CSP_PWM4C_T *const PWM0    = ((CSP_PWM4C_T*) PWM_BASE_ADDRESS);    /* PWM (4 channels)            */ 
CSP_CAN_T   *const CAN0    = ((CSP_CAN_T*)   CAN0_BASE_ADDRESS);   /* CAN0 (16 channels)          */ 
CSP_PIO_T   *const UPIO    = ((CSP_PIO_T*)   UPIO_BASE_ADDRESS);   /* United PIO Controller       */ 
CSP_CAPT_T  *const CAPT0   = ((CSP_CAPT_T*)  CAPT0_BASE_ADDRESS);  /* Capture 0                   */ 
CSP_CAPT_T  *const CAPT1   = ((CSP_CAPT_T*)  CAPT1_BASE_ADDRESS);  /* Capture 1                   */ 
CSP_ST_T    *const ST0     = ((CSP_ST_T*)    ST0_BASE_ADDRESS);    /* Simple Timer 0              */ 
CSP_ST_T    *const ST1     = ((CSP_ST_T*)    ST1_BASE_ADDRESS);    /* Simple Timer 1              */ 
CSP_CM_T    *const CM      = ((CSP_CM_T*)    CM_BASE_ADDRESS);     /* Clock Manager               */ 
CSP_PMC_T   *const PMC     = ((CSP_PMC_T*)   PMC_BASE_ADDRESS);    /* Power Management Controller */ 
CSP_PDC_T   *const PDC     = ((CSP_PDC_T*)   PDC_BASE_ADDRESS);    /* Peripheral Data Controller  */ 
CSP_GIC_T   *const GIC     = ((CSP_GIC_T*)   GIC_BASE_ADDRESS);    /* Generic Interrupt Controller*/

/******************************************************************************
* Ghost Peripheral Declaration : Ghost field do not disturb the behaviour of module
******************************************************************************/
#define  BIT_13   (0x01ul << 13)

CSP_AMC_T   *const AMC_GHOST     = ((CSP_AMC_T*)   (AMC_BASE_ADDRESS     | BIT_13));
CSP_SFM_T   *const SFM_GHOST     = ((CSP_SFM_T*)   (SFM_BASE_ADDRESS     | BIT_13));
CSP_WD_T    *const WD_GHOST      = ((CSP_WD_T*)    (WD_BASE_ADDRESS      | BIT_13));   
CSP_WT_T    *const WT_GHOST      = ((CSP_WT_T*)    (WT_BASE_ADDRESS      | BIT_13));    
CSP_USART_T *const USART0_GHOST  = ((CSP_USART_T*) (USART0_BASE_ADDRESS  | BIT_13));
CSP_USART_T *const USART1_GHOST  = ((CSP_USART_T*) (USART1_BASE_ADDRESS  | BIT_13));
CSP_CAN_T   *const CAN3_GHOST    = ((CSP_CAN_T*)   (CAN3_BASE_ADDRESS    | BIT_13));
CSP_SPI_T   *const SPI0_GHOST    = ((CSP_SPI_T*)   (SPI0_BASE_ADDRESS    | BIT_13));
CSP_CAN_T   *const CAN1_GHOST    = ((CSP_CAN_T*)   (CAN1_BASE_ADDRESS    | BIT_13));
CSP_CAN_T   *const CAN2_GHOST    = ((CSP_CAN_T*)   (CAN2_BASE_ADDRESS    | BIT_13));
CSP_ADC8C_T *const ADC0_GHOST    = ((CSP_ADC8C_T*) (ADC0_BASE_ADDRESS    | BIT_13));
CSP_ADC8C_T *const ADC1_GHOST    = ((CSP_ADC8C_T*) (ADC1_BASE_ADDRESS    | BIT_13));
CSP_GPT3C_T *const GPT0_GHOST    = ((CSP_GPT3C_T*) (GPT0_BASE_ADDRESS    | BIT_13)); 
CSP_GPT_T   *const GPT0CH0_GHOST = ((CSP_GPT_T*)   (GPT0CH0_BASE_ADDRESS | BIT_13)); 
CSP_GPT_T   *const GPT0CH1_GHOST = ((CSP_GPT_T*)   (GPT0CH1_BASE_ADDRESS | BIT_13)); 
CSP_GPT_T   *const GPT0CH2_GHOST = ((CSP_GPT_T*)   (GPT0CH2_BASE_ADDRESS | BIT_13)); 
CSP_GPT1C_T *const GPT1_GHOST    = ((CSP_GPT1C_T*) (GPT1_BASE_ADDRESS    | BIT_13)); 
CSP_GPT_T   *const GPT1CH0_GHOST = ((CSP_GPT_T*)   (GPT1CH0_BASE_ADDRESS | BIT_13)); 
CSP_PWM4C_T *const PWM0_GHOST    = ((CSP_PWM4C_T*) (PWM_BASE_ADDRESS     | BIT_13)); 
CSP_CAN_T   *const CAN0_GHOST    = ((CSP_CAN_T*)   (CAN0_BASE_ADDRESS    | BIT_13)); 
CSP_PIO_T   *const UPIO_GHOST    = ((CSP_PIO_T*)   (UPIO_BASE_ADDRESS    | BIT_13)); 
CSP_CAPT_T  *const CAPT0_GHOST   = ((CSP_CAPT_T*)  (CAPT0_BASE_ADDRESS   | BIT_13)); 
CSP_CAPT_T  *const CAPT1_GHOST   = ((CSP_CAPT_T*)  (CAPT1_BASE_ADDRESS   | BIT_13)); 
CSP_ST_T    *const ST0_GHOST     = ((CSP_ST_T*)    (ST0_BASE_ADDRESS     | BIT_13)); 
CSP_ST_T    *const ST1_GHOST     = ((CSP_ST_T*)    (ST1_BASE_ADDRESS     | BIT_13)); 
CSP_CM_T    *const CM_GHOST      = ((CSP_CM_T*)    (CM_BASE_ADDRESS      | BIT_13));
CSP_PMC_T   *const PMC_GHOST     = ((CSP_PMC_T*)   (PMC_BASE_ADDRESS     | BIT_13)); 
CSP_PDC_T   *const PDC_GHOST     = ((CSP_PDC_T*)   (PDC_BASE_ADDRESS     | BIT_13));
CSP_GIC_T   *const GIC_GHOST     = ((CSP_GIC_T*)   (GIC_BASE_ADDRESS    & ~BIT_13));


/******************************************************************************
* Chip Description
******************************************************************************/
const CSP_PERIPHERAL_TYPE_T CHIP[PERIPH_NUMBER] = 
{ 
/* {Peripheral Type, Interrupt Number, PDC Channel Rx, PDC Channel Tx}       */   
   {WATCHDOG_E,     2,  0,  0},  /*  1 */
   {WATCHTIMER_E,   3,  0,  0},  /*  2 */
   {USART_E,        4,  0,  1},  /*  3 */
   {USART_E,        5,  2,  3},  /*  4 */
   {CAN16C_E,       6,  0,  0},  /*  5 */
   {SPI_E,          7,  6,  7},  /*  6 */
   {CAN16C_E,       8,  0,  0},  /*  7 */
   {CAN32C_E,       9,  0,  0},  /*  8 */
   {ADC8C_10_E,    10,  4,  0},  /*  9 */
   {ADC8C_10_E,    11,  5,  0},  /* 10 */
   {GPT3C_E,       12,  0,  0},  /* 11 */
   {GPT1C_E,       18,  0,  0},  /* 12 */
   {PWM4C_E,       19,  0,  0},  /* 13 */
   {CAN16C_E,      20,  0,  0},  /* 14 */
   {PIO_E,         21,  0,  0},  /* 15 */
   {CAPTURE_E,     22,  8,  0},  /* 16 */
   {CAPTURE_E,     23,  9,  0},  /* 17 */
   {SIMPLETIMER_E, 24,  0,  0},  /* 18 */
   {SIMPLETIMER_E, 25,  0,  0}   /* 19 */
};

