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
* File Name       : csp_wt.h
* Description     : Definitions, Macros and function declarations for
*                   Watch Timer module
* Library Version : 2.00
* Module Version  : 1.XX
*
*       +----- (NEW | MODify | ADD | DELete)                                 
*       |                                                                    
*  No   |   When       Who                What               
*-----+---+----------+------------------+--------------------------------------
* 000  NEW  01/05/99   Patrice VILCHEZ    Creation
* 001  ADD  28/07/00   Patrice VILCHEZ    Add peripheral & bits declaration
* 002  MOD  01/04/01   Olivier MAZUYER    Clean up
* 003  MOD  08/06/01   Frederic SAMSON    Clean Up
* 004  MOD  19/10/01   Christophe GARDIN  Clean Up
*----------------------------------------------------------------------------*/

#ifndef CSP_WT_H
#define CSP_WT_H


/******************************************************************************
************************* WT Structure Definition *****************************
******************************************************************************/
typedef struct
{ 
   CSP_REGISTER_T  ReservedA[24];
   CSP_REGISTER_T  CR;              /* Control Register                      */
   CSP_REGISTER_T  MR;              /* Mode Register                         */
   CSP_REGISTER_T  ReservedB;
   CSP_REGISTER_T  CSR;             /* Clear Status Register                 */
   CSP_REGISTER_T  SR;              /* Status Register                       */
   CSP_REGISTER_T  IER;             /* Interrupt Enable Register             */
   CSP_REGISTER_T  IDR;             /* Interrupt Disable Register            */
   CSP_REGISTER_T  IMR;             /* Interrupt Mask Register               */
   CSP_REGISTER_T  SECS;            /* Seconds Register                      */
   CSP_REGISTER_T  ALARM;           /* Alarm Register                        */
} CSP_WT_T, *CSP_WT_PTR;


/******************************************************************************
************************** WT Registers Definition ****************************
******************************************************************************/

/******************************************************************************
* CR : WT Control Register
******************************************************************************/
#define SWRST        (0x01ul << 0)  /* Software Reset                        */
#define SECSEN       (0x01ul << 1)  /* Seconds Counter Enable                */
#define SECSDIS      (0x01ul << 2)  /* Seconds Counter Disable               */
#define ALARMEN      (0x01ul << 3)  /* Alarm Enable                          */
#define ALARMDIS     (0x01ul << 4)  /* Alarm Disable                         */

/******************************************************************************
* MR : WT Mode Register
******************************************************************************/
#define SECRST       (0x01ul << 0)  /* Second Counter Reset Mask             */
#define SECRST_A8B   (0x00ul << 0)  /* Second Counter Reset after 0xA8BFFFFF */ 
#define SECRST_FFF   (0x01ul << 0)  /* Second Counter Reset after 0xFFFFFFFF */

/******************************************************************************
* CSR, SR, IER, IDE, IMR : WT Status and Interrupt Registers
******************************************************************************/
#define ALARMINT     (0x01ul << 0)  /* Alarm Interrupt                       */

/* SECSEN   : Seconds Counter Enable Interrupt  - Same as CR Register        */
/* SECSDIS  : Seconds Counter Disable Interrupt - Same as CR Register        */
/* ALARMEN  : Alarm Enable Interrupt            - Same as CR Register        */
/* ALARMDIS : Alarm Disable Interrupt           - Same as CR Register        */

/* SR Register Only                                                          */
#define WSEC         (0x01ul << 5)  /* Write Second                          */
#define SECSENS      (0x01ul << 8)  /* Seconds Counter Enable Status         */
#define ALARMENS     (0x01ul << 9)  /* Alarm Enable Status                   */


/******************************************************************************
**************************** WT Macros Definition *****************************
******************************************************************************/
/* CR : WT Control Register                                                  */
#define CSP_WT_SET_CR(wt, val)         ((wt)->CR = (val))

/* MR : WT Mode Register                                                     */
#define CSP_WT_GET_MR(wt)              ((wt)->MR)          
#define CSP_WT_SET_MR(wt, mode)        ((wt)->MR = (mode))

/* CSR, SR : WT Status Registers                                             */
#define CSP_WT_SET_CSR(wt, val)        ((wt)->CSR = (val))        /* Clear   */
#define CSP_WT_GET_SR(wt)              ((wt)->SR)                 /* Status  */
                                       
/* IER, IDR, IMR : WT Interrupt Registers                                    */
#define CSP_WT_SET_IER(wt, val)        ((wt)->IER = (val))        /* Enable  */
#define CSP_WT_SET_IDR(wt, val)        ((wt)->IDR = (val))        /* Disable */
#define CSP_WT_GET_IMR(wt)             ((wt)->IMR)                /* Mask    */

/* SECS : WT Seconds Register                                                */
#define CSP_WT_GET_SECS(wt)            ((wt)->SECS)
#define CSP_WT_SET_SECS(wt, seconds)   ((wt)->SECS = (seconds))

/* ALARM : WT Alarm Register                                                 */
#define CSP_WT_GET_ALARM(wt)           ((wt)->ALARM)
#define CSP_WT_SET_ALARM(wt, val)      ((wt)->ALARM = (val))


/******************************************************************************
********************** WT External Functions Declaration **********************
******************************************************************************/
extern void  CSP_WTInit(CSP_WT_T *const wt, U32_T mode);
extern void CSP_WTConfigInterrupt(CSP_WT_T *const wt, U32_T int_mode, U32_T int_mask, U32_T callback);
extern U32_T CSP_WTGetTime (CSP_WT_T *const wt);
extern void  CSP_WTSetTime (CSP_WT_T *const wt, U32_T time);
extern U32_T CSP_WTGetAlarm(CSP_WT_T *const wt);
extern void  CSP_WTSetAlarm (CSP_WT_T *const wt, U32_T alarm);
extern void  CSP_WTEnable(CSP_WT_T *const wt, U8_T enable_mask);
extern void  CSP_WTDisable(CSP_WT_T *const wt, U8_T disable_mask);


#endif   /* CSP_WT_H */
