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
* File Name       : csp_st.h
* Description     : Definitions, Macros and function declarations for
*                   Simple Timer module
* Library Version : 2.00
* Module Version  : 1.XX
*
*       +----- (NEW | MODify | ADD | DELete)                                 
*       |                                                                    
*  No   |   When       Who                What               
*-----+---+----------+------------------+--------------------------------------
* 000  NEW  01/05/99   Patrice VILCHEZ    Creation
* 001  ADD  05/05/00   Patrice VILCHEZ    Add peripheral & bits declaration
* 002  MOD  01/04/01   Olivier MAZUYER    Clean up
* 003  MOD  08/06/01   Frederic SAMSON    Clean Up 
* 004  MOD  03/08/01   Frederic SAMSON    Modify Interrupt and Status Registers
* 005  MOD  22/10/01   Christophe GARDIN  Clean Up
* 006  MOD  15/03/02   Christophe GARDIN  Add Macros
* 007  MOD  19/07/02   Christophe GARDIN  Add AUTOREL bit in PR
*----------------------------------------------------------------------------*/

#ifndef CSP_ST_H
#define CSP_ST_H


/******************************************************************************
************************* ST Structure Definition *****************************
******************************************************************************/

/* Number of ST Channels                                                     */
#define NB_ST_CHANNEL   2u

/******************************************************************************
* ST Channel Structure
******************************************************************************/
typedef struct
{ 
   CSP_REGISTER_T  PR;                    /* Channel Prescalar Register      */
   CSP_REGISTER_T  CT;                    /* Channel Counter Register        */
} CSP_ST_CHANNEL_T, *CSP_ST_CHANNEL_PTR;

/******************************************************************************
* ST Structure
******************************************************************************/
typedef struct 
{ 
   CSP_REGISTER_T    ReservedA[20]; 
   CSP_REGISTER_T    ECR;                 /* Enable Clock Register           */
   CSP_REGISTER_T    DCR;                 /* Disable Clock Register          */
   CSP_REGISTER_T    PMSR;                /* Power Management Status Register*/
   CSP_REGISTER_T    ReservedB;       
   CSP_REGISTER_T    CR;                  /* Control Register                */
   CSP_REGISTER_T    ReservedC[2];  
   CSP_REGISTER_T    CSR;                 /* Clear Status Register           */
   CSP_REGISTER_T    SR;                  /* Status Register                 */
   CSP_REGISTER_T    IER;                 /* Interrupt Enable Register       */
   CSP_REGISTER_T    IDR;                 /* Interrupt Disable Register      */
   CSP_REGISTER_T    IMR;                 /* Interrupt Mask Register         */
   CSP_ST_CHANNEL_T  CHANNEL[NB_ST_CHANNEL]; /* ST Channels                  */
   CSP_REGISTER_T    ReservedD[92]; 
   CSP_REGISTER_T    CCV[NB_ST_CHANNEL];  /* Current Counter Value Register  */
} CSP_ST_T, *CSP_ST_PTR;


/******************************************************************************
************************** ST Registers Definition ****************************
******************************************************************************/

/******************************************************************************
* ECR, DCR, PMSR : ST Power Management Registers
******************************************************************************/
#define ST              (0x01ul << 1)     /* ST Clock                        */

/******************************************************************************
* CR : ST Control Register
******************************************************************************/
#define SWRST           (0x01ul << 0)     /* Software Reset                  */
#define CHEN            (0x01ul << 1)     /* Channel 0 Enable                */ 
#define CHEN0           (0x01ul << 1)     /* Channel 0 Enable                */ 
#define CHDIS           (0x01ul << 2)     /* Channel 0 Disable               */
#define CHDIS0          (0x01ul << 2)     /* Channel 0 Disable               */
#define CHEN1           (0x01ul << 3)     /* Channel 1 Enable                */ 
#define CHDIS1          (0x01ul << 4)     /* Channel 1 Disable               */

/******************************************************************************
* CSR, SR, IER, IDR, IMR : ST Status and Interrupt Registers
******************************************************************************/
#define CHEND           (0x01ul << 0)     /* Channel 0 End Status            */
#define CHEND0          (0x01ul << 0)     /* Channel 0 End Status            */
#define CHDISS0         (0x01ul << 1)     /* Channel 0 Disable Status        */
#define CHLD0           (0x01ul << 2)     /* Channel 0 Load Status           */
#define CHEND1          (0x01ul << 3)     /* Channel 1 End Status            */
#define CHDISS1         (0x01ul << 4)     /* Channel 1 Disable Status        */
#define CHLD1           (0x01ul << 5)     /* Channel 1 Load Status           */

/* SR Register Only                                                          */
#define CHENS0          (0x01ul << 24)    /* Channel 0 Enable Status         */
#define CHENS1          (0x01ul << 25)    /* Channel 1 Enable Status         */

/******************************************************************************
* PR : ST Channel Prescalar Register
******************************************************************************/
/* PRESCALAR : Channel Prescalar                                             */
#define PRESCALAR_MASK     (0x0Ful << 0)            /* Mask                  */
#define ST_PRESCALAR(val)  (((val) & 0x0Ful) << 0)  /* Writing Macro         */

/* SELECTCLK : Select Clock                                                  */
#define SELECTCLK          (0x01ul << 4)            /* SELECTCLK Mask        */
#define SELECTCLK_SYS      (0x00ul << 4)            /* Select System Clock   */
#define SELECTCLK_EXT      (0x01ul << 4)            /* Select External Clock */

#define AUTOREL            (0x01ul << 5)            /* Auto Reload           */

/* SYSCAL : System Clock Prescalar                                           */
#define ST_SYSCAL_MASK     (0x7FFul << 8)           /* Mask                  */ 
#define ST_SYSCAL(val)     (((val) & 0x7FFul) << 8) /* Writing Macro         */

/******************************************************************************
* CT : ST Channel Counter Register
******************************************************************************/
#define LOAD            (0xFFFFul << 0)   /* Counter Value Mask              */
             
/******************************************************************************
* CCV : ST Current Counter Value Registers
******************************************************************************/
#define COUNT           (0xFFFFul << 0)   /* Current Counter Value Mask      */


/******************************************************************************
**************************** ST Macros Definition *****************************
******************************************************************************/
/* ECR, DCR, PMSR : ST Power Management Registers                            */
#define CSP_ST_SET_ECR(st, val)              ((st)->ECR = (val))  /* Enable  */
#define CSP_ST_SET_DCR(st, val)              ((st)->DCR = (val))  /* Disable */
#define CSP_ST_GET_PMSR(st)                  ((st)->PMSR)         /* Status  */

/* CR : ST Control Register                                                  */
#define CSP_ST_SET_CR(st, val)               ((st)->CR = (val))

/* CSR, CR : ST Status Registers                                             */
#define CSP_ST_SET_CSR(st, val)              ((st)->CSR = (val))  /* Clear   */
#define CSP_ST_GET_SR(st)                    ((st)->SR)           /* Status  */

/* IER, IDR, IMR : ST Interrupt Registers                                    */
#define CSP_ST_SET_IER(st, val)              ((st)->IER = (val))  /* Enable  */
#define CSP_ST_SET_IDR(st, val)              ((st)->IDR = (val))  /* Disable */
#define CSP_ST_GET_IMR(st)                   ((st)->IMR)          /* Mask    */

/* PR : ST Channel Prescalar Register                                        */
#define CSP_ST_CHANNEL_GET_PR(st, channel)           ((st)->CHANNEL[(channel)].PR)
#define CSP_ST_CHANNEL_SET_PR(st, channel, prescal)  ((st)->CHANNEL[(channel)].PR = (prescal))

/* CT : ST Channel Counter Register                                          */
#define CSP_ST_CHANNEL_GET_CT(st, channel)           ((st)->CHANNEL[(channel)].CT)
#define CSP_ST_CHANNEL_SET_CT(st, channel, counter)  ((st)->CHANNEL[(channel)].CT = (counter))

/* CCV : ST Current Counter Value Register                                   */
#define CSP_ST16_CHANNEL_GET_CCV(st, channel)        ((st)->CCV[(channel)])


/******************************************************************************
********************** ST External Functions Declaration **********************
******************************************************************************/
extern void CSP_STInit(CSP_ST_T *const st);
extern void CSP_STClose(CSP_ST_T *const st);
extern void CSP_STConfigureChannel(CSP_ST_T *const st, U8_T channel, U32_T prescal_mode, U32_T load_counter);
extern void CSP_STConfigInterrupt(CSP_ST_T *const st, U32_T int_mode, U32_T int_mask, U32_T callback);
extern void CSP_STEnable(CSP_ST_T *const st, U8_T channel);
extern void CSP_STDisable(CSP_ST_T *const st, U8_T channel);


#endif   /* CSP_ST_H */
