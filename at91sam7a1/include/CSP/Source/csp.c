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
* Processor       : smartCAN1                          
* File Name       : csp.c
* Description     : SmartCAN1 peripherals mapping (CSP layer)
* Library Version : 2.00
*
*       +----- (NEW | MODify | ADD | DELete)                                 
*       |                                                                    
*  No   |   When       Who                What               
*-----+---+----------+------------------+--------------------------------------
* 000  NEW  01/05/99   Patrice VILCHEZ    Creation
* 001  MOD  01/04/01   Olivier MAZUYER    Clean up
* 002  MOD  08/06/01   Frederic SAMSON    Clean Up 
* 003  MOD  29/10/01   Christophe GARDIN  Clean Up
* 004  MOD  14/10/02   Bruno SALLE        Modification CHIP[PERIPH_NUMBER]
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
CSP_USART_T *const USART2  = ((CSP_USART_T*) USART2_BASE_ADDRESS); /* USART2 (J1708)              */ 
CSP_SPI_T   *const SPI0    = ((CSP_SPI_T*)   SPI0_BASE_ADDRESS);   /* SPI0                        */ 
CSP_ADC8C_T *const ADC0    = ((CSP_ADC8C_T*) ADC0_BASE_ADDRESS);   /* ADC0 (8 channels)           */ 
CSP_GPT3C_T *const GPT0    = ((CSP_GPT3C_T*) GPT0_BASE_ADDRESS);   /* 16-bit Timer T0 (3 channels)*/ 
CSP_GPT_T   *const GPT0CH0 = ((CSP_GPT_T*)   GPT0CH0_BASE_ADDRESS);/* 16-bit Timer T0 channel 0   */ 
CSP_GPT_T   *const GPT0CH1 = ((CSP_GPT_T*)   GPT0CH1_BASE_ADDRESS);/* 16-bit Timer T0 channel 1   */ 
CSP_GPT_T   *const GPT0CH2 = ((CSP_GPT_T*)   GPT0CH2_BASE_ADDRESS);/* 16-bit Timer T0 channel 2   */ 
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
* Chip Description
******************************************************************************/
const CSP_PERIPHERAL_TYPE_T CHIP[PERIPH_NUMBER] = 
{ 
/* {Peripheral Type, Interrupt Number, PDC Channel Rx, PDC Channel Tx}       */   
   {WATCHDOG_E,     2,  0,  0},  /*  0 */
   {WATCHTIMER_E,   3,  0,  0},  /*  1 */
   {USART_E,        4,  0,  1},  /*  2 */
   {USART_E,        5,  2,  3},  /*  3 */
   {USART_E,        6,  4,  5},  /*  4 */
   {SPI_E,          7,  6,  7},  /*  5 */
   {UNUSED,         0,  0,  0},  /*  6 */
   {UNUSED,         0,  0,  0},  /*  7 */
   {ADC8C_10_E,    10, 10,  0},  /*  8 */
   {UNUSED,         0,  0,  0},  /*  9 */
   {GPT3C_E,       12,  0,  0},  /*  10 */
   {UNUSED,         0,  0,  0},  /*  11 */
   {PWM4C_E,       16,  0,  0},  /*  12 */
   {CAN16C_E,      20,  0,  0},  /* 13 */
   {PIO_E,         21,  0,  0},  /* 14 */
   {CAPTURE_E,     22,  8,  0},  /* 15 */
   {CAPTURE_E,     23,  9,  0},  /* 16 */
   {SIMPLETIMER_E, 24,  0,  0},  /* 17 */
   {SIMPLETIMER_E, 25,  0,  0}   /* 18 */
};

