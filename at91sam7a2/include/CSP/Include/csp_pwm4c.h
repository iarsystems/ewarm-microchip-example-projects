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
* File Name       : csp_pwm4c.h
* Description     : Definitions, Macros and function declarations for
*                   Pulse Width Modulator 4 channels module
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
* 003  MOD  25/10/01   Christophe GARDIN  Clean Up
* 004  MOD  28/03/02   Christophe GARDIN  Add PRESCAL macros 
*----------------------------------------------------------------------------*/

#ifndef CSP_PWM4C_H
#define CSP_PWM4C_H


/******************************************************************************
************************ PWM4C Structure Definition ***************************
******************************************************************************/

/* Number of PWM Channels                                                    */
#define NB_PWM_CHANNEL  4u

/******************************************************************************
* PWM Channel Structure
******************************************************************************/
typedef struct
{
   CSP_REGISTER_T  DLY;             /* Delay Register                        */
   CSP_REGISTER_T  PUL;             /* Pulse Register                        */
} CSP_PWM_CHANNEL_T, *CSP_PWM_CHANNEL_PTR;


/******************************************************************************
* PWM 4 Channels Structure
******************************************************************************/
typedef struct 
{
   CSP_REGISTER_T     ReservedA[20];
   CSP_REGISTER_T     ECR;          /* Enable Clock Register                 */
   CSP_REGISTER_T     DCR;          /* Disable Clock Register                */
   CSP_REGISTER_T     PMSR;         /* Power Management Status Register      */
   CSP_REGISTER_T     ReservedD;
   CSP_REGISTER_T     CR;           /* Control Register                      */
   CSP_REGISTER_T     MR;           /* Mode Register                         */
   CSP_REGISTER_T     ReservedB;      
   CSP_REGISTER_T     CSR;          /* Clear Status Register                 */
   CSP_REGISTER_T     SR;           /* Status Register                       */
   CSP_REGISTER_T     IER;          /* Interrupt Enable Register             */
   CSP_REGISTER_T     IDR;          /* Interrupt Disable Register            */
   CSP_REGISTER_T     IMR;          /* Interrupt Mask Register               */
   CSP_PWM_CHANNEL_T  CHANNEL[NB_PWM_CHANNEL];  /* PWM Channels              */
} CSP_PWM4C_T, *CSP_PWM4C_PTR;


/******************************************************************************
* PWM PIO Registers PER, PDR, PSR, OER, ODR, OSR, SODR, CODR, ODSR, PDSR
*                   MDER, MDDR, MDSR, CSR, SR, IER, IDR, IMR
******************************************************************************/  
#define PIOPWM0     ( 0x01 << 16 )          /* PWM I/O 0                        */
#define PIOPWM1     ( 0x01 << 17 )          /* PWM I/O 1                        */
#define PIOPWM2     ( 0x01 << 18 )          /* PWM I/O 2                        */
#define PIOPWM3     ( 0x01 << 19 )          /* PWM I/O 3                        */

/******************************************************************************
* ECR, DCR, PMSR : PWM4C Power Management Registers
******************************************************************************/
#define PWM       (0x01ul << 1)     /* PWM Clock                             */
          
/******************************************************************************
* CR : PWM4C Control Register
******************************************************************************/
#define SWRST     (0x01ul << 0)     /* PWM Software Reset                    */
#define PWMEN     (0x01ul << 1)     /* PWM Channel Enable 0                  */
#define PWMEN0    (0x01ul << 1)     /* PWM Channel Enable 0                  */
#define PWMDIS    (0x01ul << 2)     /* PWM Channel Disable 0                 */
#define PWMDIS0   (0x01ul << 2)     /* PWM Channel Disable 0                 */
#define PWMEN1    (0x01ul << 3)     /* PWM Channel Enable 1                  */
#define PWMDIS1   (0x01ul << 4)     /* PWM Channel Disable 1                 */
#define PWMEN2    (0x01ul << 5)     /* PWM Channel Enable 2                  */
#define PWMDIS2   (0x01ul << 6)     /* PWM Channel Disable 2                 */
#define PWMEN3    (0x01ul << 7)     /* PWM Channel Enable 3                  */
#define PWMDIS3   (0x01ul << 8)     /* PWM Channel Disable 3                 */

/******************************************************************************
* MR : PWM4C Mode Register
******************************************************************************/
#define PRESCAL_MASK    (0x0Ful << 0)     /* Counter Clock Prescalar Mask    */
#define PL              (0x01ul << 4)     /* Pulse Level Mask                */

#define PRESCAL0(val)   (((val) & 0x0Ful) << 0)   /* Counter Clock Prescalar for Channel 0  */
#define PL0             (0x01ul << 4)             /* Pulse Level for PWM Channel 0          */
#define PRESCAL1(val)   (((val) & 0x0Ful) << 8)   /* Counter Clock Prescalar for Channel 1  */
#define PL1             (0x01ul << 12)            /* Pulse Level for PWM Channel 1          */
#define PRESCAL2(val)   (((val) & 0x0Ful) << 16)  /* Counter Clock  Prescalar for Channel 2 */
#define PL2             (0x01ul << 20)            /* Pulse Level for PWM Channel 2          */
#define PRESCAL3(val)   (((val) & 0x0Ful) << 24)  /* Counter Clock Prescalar for Channel 3  */
#define PL3             (0x01ul << 28)            /* Pulse Level for PWM Channel 3          */

/******************************************************************************
* CSR, SR, IER, IDR, IMR : PWM4C Status Register and Interrupt Registers
******************************************************************************/
#define PSTA0     (0x01ul << 0)     /* Pulse Start Channel 0                 */
#define PEND0     (0x01ul << 1)     /* Pulse End Channel 0                   */
#define PSTA1     (0x01ul << 2)     /* Pulse Start Channel 1                 */
#define PEND1     (0x01ul << 3)     /* Pulse End Channel 1                   */
#define PSTA2     (0x01ul << 4)     /* Pulse Start Channel 2                 */
#define PEND2     (0x01ul << 5)     /* Pulse End Channel 2                   */
#define PSTA3     (0x01ul << 6)     /* Pulse Start Channel 3                 */
#define PEND3     (0x01ul << 7)     /* Pulse End Channel 3                   */

/* SR Register Only                                                          */
#define PWMENS0   (0x01ul << 8)     /* PWM Enable Status of Channel 0        */
#define PWMENS1   (0x01ul << 9)     /* PWM Enable Status of Channel 1        */
#define PWMENS2   (0x01ul << 10)    /* PWM Enable Status of Channel 2        */
#define PWMENS3   (0x01ul << 11)    /* PWM Enable Status of Channel 3        */

/******************************************************************************
* DLY : PWM4C Delay Register
******************************************************************************/
#define DELAY     (0xFFFFul << 0)   /* Channel Delay Mask                    */

/******************************************************************************
* PUL : PWM4C Pulse Register
******************************************************************************/
#define PULSE     (0xFFFFul << 0)   /* Channel Pulse Width Mask              */


/******************************************************************************
************************** PWM4C Macros Definition ****************************
******************************************************************************/
/* ECR, DCR, PMSR : PWM4C Power Management registers                         */
#define CSP_PWM4C_SET_ECR(pwm, val)    ((pwm)->ECR = (val))       /* Enable  */
#define CSP_PWM4C_SET_DCR(pwm, val)    ((pwm)->DCR = (val))       /* Disable */
#define CSP_PWM4C_GET_PMSR(pwm)        ((pwm)->PMSR)              /* Status  */

/* CR : PWM4C Control Register                                               */
#define CSP_PWM4C_SET_CR(pwm, val)     ((pwm)->CR = (val))

/* MR : PWM4C Mode Register                                                  */
#define CSP_PWM4C_GET_MR(pwm)          ((pwm)->MR)
#define CSP_PWM4C_SET_MR(pwm, mode)    ((pwm)->MR = (mode))

/* CSR, SR : PWM4C Status Registers                                          */
#define CSP_PWM4C_SET_CSR(pwm, val)    ((pwm)->CSR = (val))       /* Clear   */
#define CSP_PWM4C_GET_SR(pwm)          ((pwm)->SR)                /* Status  */

/* IER, IDR, IMR : PWM4C Interrupt Registers                                 */
#define CSP_PWM4C_SET_IER(pwm, val)    ((pwm)->IER = (val))       /* Enable  */
#define CSP_PWM4C_SET_IDR(pwm, val)    ((pwm)->IDR = (val))       /* Disable */
#define CSP_PWM4C_GET_IMR(pwm)         ((pwm)->IMR)               /* Mask    */

/* DLY : PWM4C Delay Register                                                */
#define CSP_PWM4C_CHANNEL_GET_DLY(pwm, channel)          ((pwm)->CHANNEL[(channel)].DLY)
#define CSP_PWM4C_CHANNEL_SET_DLY(pwm, channel, delay)   ((pwm)->CHANNEL[(channel)].DLY = (delay))

/* PUL : PWM4C Pulse Register                                                */
#define CSP_PWM4C_CHANNEL_GET_PUL(pwm, channel)          ((pwm)->CHANNEL[(channel)].PUL)
#define CSP_PWM4C_CHANNEL_SET_PUL(pwm, channel, pulse)   ((pwm)->CHANNEL[(channel)].PUL = (pulse))


/******************************************************************************
******************** PWM4C External Functions Declaration *********************
******************************************************************************/
extern void CSP_PWM4CInit(CSP_PWM4C_T *const pwm);
extern void CSP_PWM4CClose(CSP_PWM4C_T *const pwm);
extern void CSP_PWM4CConfigureChannel(CSP_PWM4C_T *const pwm, U8_T channel, U32_T mode, U16_T delay, U16_T pulse);
extern void CSP_PWM4CConfigInterrupt(CSP_PWM4C_T *const pwm, U32_T int_mode, U32_T int_mask, U32_T callback);
extern void CSP_PWM4CEnable(CSP_PWM4C_T *const pwm, U8_T channel);
extern void CSP_PWM4CDisable(CSP_PWM4C_T *const pwm, U8_T channel);
extern void CSP_PWM4CSetOffstate(CSP_PWM4C_T *const pwm, U8_T channel, U8_T offstate);


#endif   /* CSP_PWM4C_H */
