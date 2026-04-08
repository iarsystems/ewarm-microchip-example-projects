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
* File Name       : csp_capt.h
* Description     : Definitions, Macros and function declarations for
*                   Capture module
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
* 003  MOD  24/10/01   Christophe GARDIN  Clean Up
*----------------------------------------------------------------------------*/

#ifndef CSP_CAPT_H
#define CSP_CAPT_H


/******************************************************************************
************************ CAPTURE Structure Definition *************************
******************************************************************************/
typedef struct
{
   CSP_REGISTER_T  ReservedA[20];     
   CSP_REGISTER_T  ECR;                   /* Enable Clock Register           */
   CSP_REGISTER_T  DCR;                   /* Disable Clock Register          */
   CSP_REGISTER_T  PMSR;                  /* Power Management Status Register*/
   CSP_REGISTER_T  ReservedB;      
   CSP_REGISTER_T  CR;                    /* Control Register                */
   CSP_REGISTER_T  MR;                    /* Mode Register                   */
   CSP_REGISTER_T  ReservedC;      
   CSP_REGISTER_T  CSR;                   /* Clear Status Register           */
   CSP_REGISTER_T  SR;                    /* Status Register                 */
   CSP_REGISTER_T  IER;                   /* Interrupt enable Register       */
   CSP_REGISTER_T  IDR;                   /* Interrupt Disable Register      */
   CSP_REGISTER_T  IMR;                   /* Interrupt Mask Register         */
   CSP_REGISTER_T  DR;                    /* Capture Data Register           */
} CSP_CAPT_T, *CSP_CAPT_PTR;


/******************************************************************************
************************ CAPTURE Registers Definition *************************
******************************************************************************/

/******************************************************************************
* ECR, DCR, PMSR : CAPTURE Power Management Registers
******************************************************************************/
#define CAP                (0x01ul << 1)  /* CAPTURE Clock                   */

/******************************************************************************
* CR : CAPTURE Control Register 
******************************************************************************/
#define SWRST              (0x01ul << 0)  /* CAPTURE Software Reset          */
#define CAPEN              (0x01ul << 1)  /* CAPTURE Enable                  */
#define CAPDIS             (0x01ul << 2)  /* CAPTURE Disable                 */
#define STARTCAPT          (0x01ul << 3)  /* Start Capture                   */

/******************************************************************************
* MR : CAPTURE Mode Register
******************************************************************************/
/* PRESCALAR : Counter Clock Prescalar                                       */
#define PRESCALAR_MASK        (0x0Ful << 0)              /* Mask             */   
#define CAPT_PRESCALAR(val)   (((val) & 0x0Ful) << 0)    /* Writing Macro    */ 

#define MEASMODE_POS_NEG   (0x00ul << 4)  /* Measure Between Positive and Negative Edge */
#define MEASMODE_POS       (0x02ul << 4)  /* Measure Between Positive Edge   */
#define MEASMODE_NEG       (0x03ul << 4)  /* Measure Between Negative Edge   */
#define OVERMODE           (0x01ul << 6)  /* Overrun Mode                    */
#define ONESHOT            (0x01ul << 7)  /* One Shot                        */

/******************************************************************************
* CSR, SR, IER, IDR, IMR : CAPTURE Status and Interrupt Registers 
******************************************************************************/
#define PDCEND             (0x01ul << 0)  /* PDC End                         */
#define OVERRUN            (0x01ul << 1)  /* Over Run                        */
#define OVERFLOW           (0x01ul << 2)  /* Over Flow                       */

/* SR, IER, IDR, IMR Registers only                                          */
#define DATACAPT           (0x01ul << 3)  /* Data Captured                   */

/* SR Register Only                                                          */
#define CAPENS             (0x01ul << 8)  /* Capture Enable Status           */

/******************************************************************************
* DR : CAPTURE Data Register
******************************************************************************/
/* DURATION : CAPTURE duration                                               */
#define DURATION_MASK      (0x7FFFul << 0)               /* Mask             */
#define DURATION(val)      (((val) & 0x7FFFul) << 0)     /* Writing Macro    */

/* LEVEL : Level Measured                                                    */
#define LEVEL              (0x01ul << 15) /* LEVEL Mask                      */
#define LEVEL_HIGHT        (0x01ul << 15) /* Duration Concerns High Level    */
#define LEVEL_LOW          (0x00ul << 15) /* Duration Concerns Low Level     */


/******************************************************************************
************************* CAPTURE Macros Definition ***************************
******************************************************************************/
/* ECR, DCR, PMSR : CAPTURE Power Management Registers                       */
#define CSP_CAPT_SET_ECR(capt, val)       ((capt)->ECR = (val))   /* Enable  */
#define CSP_CAPT_SET_DCR(capt, val)       ((capt)->DCR = (val))   /* Disable */
#define CSP_CAPT_GET_PMSR(capt)           ((capt)->PMSR)          /* Status  */

/* CR : CAPTURE Control Register                                             */
#define CSP_CAPT_SET_CR(capt, val)        ((capt)->CR = (val))

/* MR : CAPTURE Mode Register                                                */
#define CSP_CAPT_GET_MR(capt)             ((capt)->MR)
#define CSP_CAPT_SET_MR(capt, mode)       ((capt)->MR = (mode))

/* CSR, SR : CAPTURE Status Registers                                        */
#define CSP_CAPT_SET_CSR(capt, val)       ((capt)->CSR = (val))   /* Clear   */
#define CSP_CAPT_GET_SR(capt)             ((capt)->SR)            /* Status  */

/* IER, IDR, IMR : CAPTURE Interrupt Registers                               */
#define CSP_CAPT_SET_IER(capt, val)       ((capt)->IER = (val))   /* Enable  */
#define CSP_CAPT_SET_IDR(capt, val)       ((capt)->IDR = (val))   /* Disable */
#define CSP_CAPT_GET_IMR(capt)            ((capt)->IMR)           /* Mask    */

/* DR : CAPTURE Data Register                                                */
#define CSP_CAPT_GET_DR(capt)             ((capt)->DR)


/******************************************************************************
******************* CAPTURE External Functions Declaration ********************
******************************************************************************/
extern void CSP_CAPTInit(CSP_CAPT_T *const capt, U32_T mode);
extern void CSP_CAPTClose(CSP_CAPT_T *const capt);
extern void CSP_CAPTConfigInterrupt(CSP_CAPT_T *const capt, U32_T int_mode, U32_T int_mask, U32_T callback);
extern void CSP_CAPTEnable(CSP_CAPT_T *const capt);
extern void CSP_CAPTDisable(CSP_CAPT_T *const capt);
extern void CSP_CAPTStart(CSP_CAPT_T *const capt);


#endif   /* CSP_CAPT_H */
