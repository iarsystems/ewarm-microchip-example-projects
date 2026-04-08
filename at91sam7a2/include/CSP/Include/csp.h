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
* File Name       : csp.h
* Description     : Definitions and Structures for easyCan4
* Library Version : 1.00
*
*       +----- (NEW | MODify | ADD | DELete)                                 
*       |                                                                    
*  No   |   When       Who                What               
*-----+---+----------+------------------+--------------------------------------
* 000  NEW  17/07/02   Christophe GARDIN  Creation
*----------------------------------------------------------------------------*/

#ifndef CSP_H
#define CSP_H


/******************************************************************************
* Include Files
******************************************************************************/
#include "csp_types.h"
#include "csp_pwm4c.h"
#include "csp_pmc.h"
#include "csp_cm.h"
#include "csp_gpt.h"
#include "csp_can.h"
#include "csp_st.h"
#include "csp_gic.h"
#include "csp_wt.h"
#include "csp_wd.h"
#include "csp_usart.h"
#include "csp_pdc.h"
#include "csp_amc.h"
#include "csp_adc8c.h"
#include "csp_sfm.h"
#include "csp_pio.h"
#include "csp_spi.h"
#include "csp_capt.h"


/******************************************************************************
****************************** Chip Declaration *******************************
******************************************************************************/
/* Number of Peripherals                                                     */
#define PERIPH_NUMBER   19u

/******************************************************************************
* Peripheral Base Addresses
******************************************************************************/
#define AMC_BASE_ADDRESS      0xFFE00000ul   /* Advanced Memory Controller   */
#define SFM_BASE_ADDRESS      0xFFF00000ul   /* Special Function Module      */
#define WD_BASE_ADDRESS       0xFFFA0000ul   /* WATCHDOG                     */
#define WT_BASE_ADDRESS       0xFFFA4000ul   /* WATCHTIMER                   */
#define USART0_BASE_ADDRESS   0xFFFA8000ul   /* USART0                       */
#define USART1_BASE_ADDRESS   0xFFFAC000ul   /* USART1                       */
#define CAN3_BASE_ADDRESS     0xFFFB0000ul   /* CAN3 (16 channels)           */
#define SPI0_BASE_ADDRESS     0xFFFB4000ul   /* SPI 0                        */
#define CAN1_BASE_ADDRESS     0xFFFB8000ul   /* CAN1 (16 channels)           */
#define CAN2_BASE_ADDRESS     0xFFFBC000ul   /* CAN2 (32 channels)           */
#define ADC0_BASE_ADDRESS     0xFFFC0000ul   /* ADC0 (8 channels)            */
#define ADC1_BASE_ADDRESS     0xFFFC4000ul   /* ADC1 (8 channels)            */
#define GPT0_BASE_ADDRESS     0xFFFC8000ul   /* 16-bit Timer T0 (3 channels) */
#define GPT0CH0_BASE_ADDRESS  0xFFFC8000ul   /* 16-bit Timer T0 channel 0    */
#define GPT0CH1_BASE_ADDRESS  0xFFFC8100ul   /* 16-bit Timer T0 channel 1    */
#define GPT0CH2_BASE_ADDRESS  0xFFFC8200ul   /* 16-bit Timer T0 channel 2    */
#define GPT1_BASE_ADDRESS     0xFFFCC000ul   /* 16-bit Timer T1 (1 channel)  */
#define GPT1CH0_BASE_ADDRESS  0xFFFCC000ul   /* 16-bit Timer T1 channel 0    */
#define PWM_BASE_ADDRESS      0xFFFD0000ul   /* PWM (4 channels)             */
#define CAN0_BASE_ADDRESS     0xFFFD4000ul   /* CAN0 (16 channels)           */
#define UPIO_BASE_ADDRESS     0xFFFD8000ul   /* United PIO Controller        */
#define CAPT0_BASE_ADDRESS    0xFFFDC000ul   /* Capture 0                    */
#define CAPT1_BASE_ADDRESS    0xFFFE0000ul   /* Capture 1                    */
#define ST0_BASE_ADDRESS      0xFFFE4000ul   /* Simple Timer 0               */
#define ST1_BASE_ADDRESS      0xFFFE8000ul   /* Simple Timer 1               */
#define CM_BASE_ADDRESS       0xFFFEC000ul   /* Clock Manager                */
#define PMC_BASE_ADDRESS      0xFFFF4000ul   /* Power Management Controller  */
#define PDC_BASE_ADDRESS      0xFFFF8000ul   /* Peripheral Data Controller   */
#define GIC_BASE_ADDRESS      0xFFFFF000ul   /* Generic Interrupt Controller */


/******************************************************************************
**************************** Interrupt Declaration ****************************
******************************************************************************/

/******************************************************************************
* Interrupt Source
******************************************************************************/
#define FIQ             0ul            /* Fast Interrupt                     */
#define SWIRQ0          1ul            /* Software Interrupt 0               */
#define INT_0           2ul            /* Watchdog Interrupt                 */
#define INT_1           3ul            /* Watch Timer Interrupt              */
#define INT_2           4ul            /* USART0 Interrupt                   */
#define INT_3           5ul            /* USART1 Interrupt                   */
#define INT_4           6ul            /* CAN3 Interrupt                     */
#define INT_5           7ul            /* SPI Interrupt                      */
#define INT_6           8ul            /* CAN1 Interrupt                     */
#define INT_7           9ul            /* CAN2 Interrupt                     */
#define INT_8           10ul           /* ADC0 Interrupt                     */
#define INT_9           11ul           /* ADC1 Interrupt                     */
#define INT_10          12ul           /* GPT0CH0 Interrupt                  */
#define INT_11          13ul           /* GPT0CH1 Interrupt                  */
#define INT_12          14ul           /* GPT0CH2 Interrupt                  */
#define INT_13          15ul           /* GPT1CH0 Interrupt                  */
#define INT_14          16ul           /* PWM Interrupt                      */
#define SWIRQ1          17ul           /* Software Interrupt 1               */
#define SWIRQ2          18ul           /* Software Interrupt 2               */
#define SWIRQ3          19ul           /* Software Interrupt 3               */
#define INT_18          20ul           /* CAN0 Interrupt                     */
#define INT_19          21ul           /* UPIO Interrupt                     */
#define INT_20          22ul           /* CAPT0 Interrupt                    */
#define INT_21          23ul           /* CAPT1 Interrupt                    */
#define INT_22          24ul           /* ST0 Interrupt                      */
#define INT_23          25ul           /* ST1 Interrupt                      */
#define SWIRQ4          26ul           /* Software Interrupt 4               */
#define SWIRQ5          27ul           /* Software Interrupt 5               */
#define EXTIRQ0         28ul           /* External Interrupt 0               */
#define EXTIRQ1         29ul           /* External Interrupt 1               */
#define SWIRQ6          30ul           /* Software Interrupt 6               */
#define SWIRQ7          31ul           /* Software Interrupt 7               */
                        
/******************************************************************************
* GIC Bit 
******************************************************************************/
#define IBIT_FIQ        (0x01ul << 0)  /* Fast Interrupt                     */
#define IBIT_SWIRQ0     (0x01ul << 1)  /* Software Interrupt 0               */
#define IBIT_WD         (0x01ul << 2)  /* Watchdog Interrupt                 */
#define IBIT_WT         (0x01ul << 3)  /* Watch Timer Interrupt              */
#define IBIT_USART0     (0x01ul << 4)  /* USART0 Interrupt                   */
#define IBIT_USART1     (0x01ul << 5)  /* USART1 Interrupt                   */
#define IBIT_CAN3       (0x01ul << 6)  /* CAN3 Interrupt                     */
#define IBIT_SPI        (0x01ul << 7)  /* SPI Interrupt                      */
#define IBIT_CAN1       (0x01ul << 8)  /* CAN1 Interrupt                     */
#define IBIT_CAN2       (0x01ul << 9)  /* CAN2 Interrupt                     */
#define IBIT_ADC0       (0x01ul << 10) /* ADC0 Interrupt                     */
#define IBIT_ADC1       (0x01ul << 11) /* ADC1 Interrupt                     */
#define IBIT_GPT0CH0    (0x01ul << 12) /* GPT0CH0 Interrupt                  */
#define IBIT_GPT0CH1    (0x01ul << 13) /* GPT0CH1 Interrupt                  */
#define IBIT_GPT0CH2    (0x01ul << 14) /* GPT0CH2 Interrupt                  */
#define IBIT_GPT1CH0    (0x01ul << 15) /* GPT1CH0 Interrupt                  */
#define IBIT_PWM        (0x01ul << 16) /* PWM Interrupt                      */
#define IBIT_SWIRQ1     (0x01ul << 17) /* Software Interrupt 1               */
#define IBIT_SWIRQ2     (0x01ul << 18) /* Software Interrupt 2               */
#define IBIT_SWIRQ3     (0x01ul << 19) /* Software Interrupt 3               */
#define IBIT_CAN0       (0x01ul << 20) /* CAN0 Interrupt                     */
#define IBIT_UPIO       (0x01ul << 21) /* UPIO Interrupt                     */
#define IBIT_CAPT0      (0x01ul << 22) /* CAPT0 Interrupt                    */
#define IBIT_CAPT1      (0x01ul << 23) /* CAPT1 Interrupt                    */
#define IBIT_ST0        (0x01ul << 24) /* ST0 Interrupt                      */
#define IBIT_ST1        (0x01ul << 25) /* ST1 Interrupt                      */
#define IBIT_SWIRQ4     (0x01ul << 26) /* Software Interrupt 4               */
#define IBIT_SWIRQ5     (0x01ul << 27) /* Software Interrupt 5               */
#define IBIT_EXTIRQ0    (0x01ul << 28) /* External Interrupt 0               */
#define IBIT_EXTIRQ1    (0x01ul << 29) /* External Interrupt 1               */
#define IBIT_SWIRQ6     (0x01ul << 30) /* Software Interrupt 6               */
#define IBIT_SWIRQ7     (0x01ul << 31) /* Software Interrupt 7               */
#define ALL_IRQ         (0xFFFFFFFFul) /* All Interrupts                     */


/******************************************************************************
*********************** Peripherals Types Definition **************************
******************************************************************************/

/******************************************************************************
* Peripherals Type Enum
******************************************************************************/
typedef enum 
{
   UNUSED = -1, WATCHDOG_E, WATCHTIMER_E, USART_E, SPI_E, CAN16C_E, CAN32C_E, 
   ADC8C_10_E, GPT3C_E, GPT1C_E, PWM4C_E, CAPTURE_E, SIMPLETIMER_E, PIO_E
} CSP_PERIPHERAL_TYPE_E;

/******************************************************************************
* Peripherals Type Structure
******************************************************************************/
typedef struct
{ 
   CSP_PERIPHERAL_TYPE_E  Type;                 /* Type Of Peripheral        */
   U8_T                   InterruptNumber;      /* Interrupt Number          */
   U8_T                   PdcRx;                /* PDC Channel Rx            */
   U8_T                   PdcTx;                /* PDC Channel Tx            */
} CSP_PERIPHERAL_TYPE_T;


/******************************************************************************
*********************** Peripherals Macros Definition *************************
******************************************************************************/
/* Get Peripheral Psel Number (See CHIP Tab in CSP.C file)                   */
#define CSP_GET_PSEL_NUMBER(periph)       (((U32_T)(periph) - 0xFFFA0000ul) / 0x4000ul)

/* Get Peripheral Interrupt Number (See CHIP Tab in CSP.C file)              */ 
#define CSP_GET_INTERRUPT_NUMBER(periph)  (CHIP[CSP_GET_PSEL_NUMBER(periph)].InterruptNumber)

/* Get Peripheral PDC Number (See CHIP Tab in CSP.C file)                    */
#define CSP_GET_PDC_RX_NUMBER(periph)     (CHIP[CSP_GET_PSEL_NUMBER(periph)].PdcRx)
#define CSP_GET_PDC_TX_NUMBER(periph)     (CHIP[CSP_GET_PSEL_NUMBER(periph)].PdcTx)

/* Get Peripheral Type (See CHIP Tab in CSP.C file)                          */
#define CSP_GET_PERIPHERAL_TYPE(periph)   (CHIP[CSP_GET_PSEL_NUMBER(periph)].Type)


/******************************************************************************
********************* External Peripheral Declaration *************************
******************************************************************************/
/* System Peripherals                                                        */
extern CSP_AMC_T     *const AMC;
extern CSP_SFM_T     *const SFM;
extern CSP_WD_T      *const WD;
extern CSP_WT_T      *const WT;
extern CSP_USART_T   *const USART0;
extern CSP_USART_T   *const USART1;
extern CSP_CAN_T     *const CAN3;
extern CSP_SPI_T     *const SPI0;
extern CSP_CAN_T     *const CAN1;
extern CSP_CAN_T     *const CAN2;
extern CSP_ADC8C_T   *const ADC0;
extern CSP_ADC8C_T   *const ADC1;
extern CSP_GPT3C_T   *const GPT0;
extern CSP_GPT_T     *const GPT0CH0;
extern CSP_GPT_T     *const GPT0CH1;
extern CSP_GPT_T     *const GPT0CH2;
extern CSP_GPT1C_T   *const GPT1;
extern CSP_GPT_T     *const GPT1CH0;
extern CSP_PWM4C_T   *const PWM0;
extern CSP_CAN_T     *const CAN0;
extern CSP_PIO_T     *const UPIO;
extern CSP_CAPT_T    *const CAPT0;
extern CSP_CAPT_T    *const CAPT1;
extern CSP_ST_T      *const ST0;
extern CSP_ST_T      *const ST1;
extern CSP_CM_T      *const CM;
extern CSP_PMC_T     *const PMC;
extern CSP_PDC_T     *const PDC;
extern CSP_GIC_T     *const GIC;

/* Ghost System Peripherals                                                        */
extern CSP_AMC_T     *const AMC_GHOST;
extern CSP_SFM_T     *const SFM_GHOST;
extern CSP_WD_T      *const WD_GHOST;
extern CSP_WT_T      *const WT_GHOST;
extern CSP_USART_T   *const USART0_GHOST;
extern CSP_USART_T   *const USART1_GHOST;
extern CSP_CAN_T     *const CAN3_GHOST;
extern CSP_SPI_T     *const SPI0_GHOST;
extern CSP_CAN_T     *const CAN1_GHOST;
extern CSP_CAN_T     *const CAN2_GHOST;
extern CSP_ADC8C_T   *const ADC0_GHOST;
extern CSP_ADC8C_T   *const ADC1_GHOST;
extern CSP_GPT3C_T   *const GPT0_GHOST;
extern CSP_GPT_T     *const GPT0CH0_GHOST;
extern CSP_GPT_T     *const GPT0CH1_GHOST;
extern CSP_GPT_T     *const GPT0CH2_GHOST;
extern CSP_GPT1C_T   *const GPT1_GHOST;
extern CSP_GPT_T     *const GPT1CH0_GHOST;
extern CSP_PWM4C_T   *const PWM_GHOST;
extern CSP_CAN_T     *const CAN0_GHOST;
extern CSP_PIO_T     *const UPIO_GHOST;
extern CSP_CAPT_T    *const CAPT0_GHOST;
extern CSP_CAPT_T    *const CAPT1_GHOST;
extern CSP_ST_T      *const ST0_GHOST;
extern CSP_ST_T      *const ST1_GHOST;
extern CSP_CM_T      *const CM_GHOST;
extern CSP_PMC_T     *const PMC_GHOST;
extern CSP_PDC_T     *const PDC_GHOST;
extern CSP_GIC_T     *const GIC_GHOST;

/* Chip                                                                      */
extern const CSP_PERIPHERAL_TYPE_T CHIP[PERIPH_NUMBER];

#endif   /* CSP_H */
