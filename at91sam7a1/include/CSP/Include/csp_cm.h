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
* File Name       : csp_cm.h
* Description     : Definitions, Macros and function declarations for
*                   Clock Manager module
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
* 005  MOD  21/11/02   Bruno SALLE		  Re-definitions for SmartCAN1
*----------------------------------------------------------------------------*/

#ifndef CSP_CM_H
#define CSP_CM_H


/******************************************************************************
************************** CM Structure Definition ****************************
******************************************************************************/

/*******************************************************************************
Type: CSP_CM_T
Description: CM Structure Definition
Fields:
- 创CSP_REGISTER_T创  CM_CE :            CM Clock Enable
- 创CSP_REGISTER_T创  CM_CD :            CM Clock Disable
- 创CSP_REGISTER_T创  CM_CS :            CM Clock Status
- 创CSP_REGISTER_T创  PST :              CM PLL Stabilisation Time
- 创CSP_REGISTER_T创  PDIV :             CM PLL Divider
- 创CSP_REGISTER_T创  OST :              CM Oscillator Stabilization Time
- 创CSP_REGISTER_T创  MDIV :             CM Master Clock Divider
*******************************************************************************/
typedef struct
{
  CSP_REGISTER_T  CM_CE;               /* CM Clock Enable                        */
  CSP_REGISTER_T  CM_CD;               /* CM Clock Disable                       */
  CSP_REGISTER_T  CM_CS;               /* CM Clock Status                        */
  CSP_REGISTER_T  PST;              /* CM PLL Stabilisation Time              */
  CSP_REGISTER_T  PDIV;             /* CM PLL Divider                         */
  CSP_REGISTER_T  OST;              /* CM Oscillator Stabilization Time       */
  CSP_REGISTER_T  MDIV;             /* CM Master Clock Divider                */
} CSP_CM_T;


/******************************************************************************
*  Clock Manager Clock Register: CE, CD, CS
******************************************************************************/

#define  PLLSLCT     (0x01 << 2)   /* PLL/Master clock selection             */
#define  LFSLCT      (0x01 << 3)   /* Low Frequency clock selection          */
#define  DIVSLCT     (0x01 << 5)   /* Programmable clock selection           */
#define  RTCSEL      (0x01 << 6)   /* RTC Frequency clock selection          */
#define  RTCKEN      (0x01 << 7)   /* Low Frequency clock oscillator         */

#define  CLKEKEY     (0x2305 << 16)   /*Key for write access into the CE register */
#define  CLKDKEY     (0x1807 << 16)   /*Key for write access into the CD register */

#define  MCKEN       (0x01 << 8)   /* Master clock Oscillator Enable          */
#define  PLLEN       (0x01 << 9)   /* PLL Enable                              */
#define  DIVEN       (0x01 << 13)  /* Programmable divider Enable             */
#define  RTCSLCT     (0x01 << 14)  /* Low Frequency clock selection           */

/******************************************************************************
*  Clock Manager PLL Stabilization Timer : PST
******************************************************************************/

#define  PSTB       (0x3FF  <<  0)  /* Mask PLL Stabilization Time            */
#define  PSTKEY     (0x59C1 << 16)  /* Key for write access into the PST register */


/******************************************************************************
*  Clock Manager PLL Divider : PDIV
******************************************************************************/

#define  PMUL       (0x1F << 0)     /* Mask PLL Multiplier                         */
#define  PLLDIV2    (0x01 << 15)    /* PLL Divider                                 */
#define  PDIVKEY    (0x762D << 16)  /* Key for write access into the PDIV register */

/******************************************************************************
*  Clock Manager Oscillator Stabilization Timer : OST
******************************************************************************/

#define  OSTB       (0x3FF  <<  0)  /* Mask Oscillator Stabilization Time         */
#define  OSTKEY     (0xDB5A << 16)  /* Key for write access into the OST register */

/******************************************************************************
*  Clock Manager Clock Divider : MDIV
******************************************************************************/

#define  MDIV       (0x7F   <<  0)     /* Mask Master clock divider                   */
#define  MDIVKEY    (0xACDC << 16)     /* Key for write access into the MDIV register */

/******************************************************************************
*************************** CM Macros Definition *****************************
******************************************************************************/
/* CE, CD, CS : CM Clock Registers*/
#define CSP_CM_SET_CE(cm, val)      (cm->CM_CE =  (val & 0xffff) | CLKEKEY )   /* Enable  */
#define CSP_CM_SET_CD(cm, val)      (cm->CM_CD =  (val & 0xffff) | CLKDKEY )   /* Disable */
#define CSP_CM_GET_CS(cm)           (cm->CM_CS)                                /* Status  */

/* PST : CM PLL Stabilisation Timer */
#define CSP_CM_SET_PST(cm, val)     (cm->PST = (val & 0xffff) | PSTKEY )    /* Write   */
#define CSP_CM_GET_PST(cm)          (cm->PST)                               /* Read    */

/* PDIV : CM PLL Divider */
#define CSP_CM_SET_PDIV(cm, val)    (cm->PDIV = (val & 0xffff) | PDIVKEY )  /* Write   */
#define CSP_CM_GET_PDIV(cm)         (cm->PDIV)                              /* Read    */

/* OST :  CM Oscillator Stabilization Timer */
#define CSP_CM_SET_OST(cm, val)     (cm->OST = (val & 0xffff) | OSTKEY )    /* Write   */
#define CSP_CM_GET_OST(cm)          (cm->OST)                               /* Read    */

/* MDIV : CM Master Clock Divider */
#define CSP_CM_SET_MDIV(cm, val)    (cm->MDIV = (val & 0xffff) | MDIVKEY )  /* Write   */
#define CSP_CM_GET_MDIV(cm)         (cm->MDIV)                              /* Read    */


/******************************************************************************
********************** CM External Functions Declaration **********************
******************************************************************************/
extern void CSP_CMInit(void);
extern void CSP_CMHighSpeed(void);
extern void CSP_CMLowSpeed(void);


#endif   /* CSP_CM_H */
