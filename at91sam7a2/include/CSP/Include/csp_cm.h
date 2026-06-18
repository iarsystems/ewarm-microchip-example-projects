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
* Library Version : 1.00
* Module Version  : 1.XX
*
*       +----- (NEW | MODify | ADD | DELete)
*       |
*  No   |   When       Who                What
*-----+---+----------+------------------+--------------------------------------
* 000  NEW  19/07/02   Christophe GARDIN  Creation
*----------------------------------------------------------------------------*/

#ifndef CSP_CM_H
#define CSP_CM_H


/******************************************************************************
************************** CM Structure Definition ****************************
******************************************************************************/
typedef struct
{
   CSP_REGISTER_T  CM_CE;              /* Clock Enable Register                 */
   CSP_REGISTER_T  CM_CD;              /* Clock Disable Register                */
   CSP_REGISTER_T  CM_CS;              /* Clock Status Register                 */
   CSP_REGISTER_T  PST;             /* PLL Stabilization Time Register       */
   CSP_REGISTER_T  PDIV;            /* PLL Divider Register                  */
   CSP_REGISTER_T  OST;             /* Oscillator Stabilization Time Register*/
   CSP_REGISTER_T  MDIV;            /* Master Clock Divider Register         */
} CSP_CM_T;


/******************************************************************************
************************** CM Registers Definition ****************************
******************************************************************************/

/******************************************************************************
* CE, CD, CS : CM Clock Registers
******************************************************************************/
#define PLLSLCT         (0x01ul << 2)     /* PLL/Master Clock Selection      */
#define LFSLCT          (0x01ul << 3)     /* Low Frequency Clock Selection   */
#define DIVSLCT         (0x01ul << 5)     /* Programmable Clock Selection    */
#define RTCSEL          (0x01ul << 6)     /* RTC Frequency Clock Selection   */
#define RTCKEN          (0x01ul << 7)     /* Low Frequency Clock Oscillator  */

/* CE Register Only                                                          */
#define CLKEKEY         (0x2305ul << 16)  /* Write Access in CE Allowed      */

/* CD Register Only                                                          */
#define CLKDKEY         (0x1807ul << 16)  /* Write Access in CD Allowed      */

/* CS Register Only                                                          */
#define MCKEN           (0x01ul << 8)     /* Master Clock Oscillator Enable status */
#define PLLEN           (0x01ul << 9)     /* PLL Enable status                     */
#define DIVEN           (0x01ul << 13)    /* Programmable Divider Enable status    */
#define RTCSLCT         (0x01ul << 14)    /* Low Frequency Clock Selection status  */

/******************************************************************************
* PST : CM PLL Stabilization Time Register
******************************************************************************/
/* PSTB : PLL Stabilization Time                                             */
#define PSTB_MASK       (0x3FFul << 0)             /* PSTB Mask              */
#define PSTB(val)       (((val) & 0x3FFul) << 0)   /* Writing Macro          */

/* PSTKEY : PST access key                                                   */
#define PSTKEY    (0x59C1ul << 16)  /* Write Access in PST Allowed     */

/******************************************************************************
* PDIV : CM PLL Divider Register
******************************************************************************/
/* PMUL : PLL Divider                                                        */
#define PMUL_MASK       (0x1Ful << 0)              /* PMUL Mask              */
#define PMUL(val)       (((val) & 0x1Ful) << 0)    /* Writing Macro          */

#define PLLDIV2         (0x01ul << 15)    /* PLL Divider                     */

/* PDIVKEY : PDIV access key                                                 */
#define PDIVKEY   (0x762Dul << 16)  /* Write Access in PDIV Allowed    */

/******************************************************************************
* OST : CM Oscillator Stabilization Time Register
******************************************************************************/
/* OSTB : Oscillator Stabilization Time                                      */
#define OSTB_MASK       (0x3FFul << 0)              /* OSTB Mask             */
#define OSTB(val)       (((val) & 0x3FFul) << 0)    /* Writing Macro         */

/* OSTKEY : OST access key                                                   */
#define OSTKEY    (0xDB5Aul << 16)  /* Write Access in OST Allowed    */

/******************************************************************************
* MDIV : CM Master Clock Divider Register
******************************************************************************/
/* MDIV : Master Clock Divider                                               */
#define MDIV_MASK       (0x7Ful << 0)               /* MDIV Mask             */
#define MDIV(val)       (((val) & 0x7Ful) << 0)     /* Writing Macro         */

/* MDIVKEY : POST access key                                                 */
#define MDIVKEY   (0xACDCul << 16)  /* Write Access in MDIV Allowed    */


/******************************************************************************
**************************** CM Macros Definition *****************************
******************************************************************************/
/* CE, CD, CS : CM Clock Registers                                           */
#define CSP_CM_SET_CE(cm, val)       ((cm)->CM_CE = (val & 0xffff) | CLKEKEY)
#define CSP_CM_SET_CD(cm, val)       ((cm)->CM_CD = (val & 0xffff) | CLKDKEY)
#define CSP_CM_GET_CS(cm)           ((cm)->CM_CS)

/* PST : CM PLL Stabilization Time Register                                  */
#define CSP_CM_SET_PST(cm, val)     ((cm)->PST = (val & 0xffff) | PSTKEY)
#define CSP_CM_GET_PST(cm)          ((cm)->PST)

/* PDIV : CM PLL Divider Register                                            */
#define CSP_CM_SET_PDIV(cm, val)    ((cm)->PDIV = (val & 0xffff) | PDIVKEY)
#define CSP_CM_GET_PDIV(cm)         ((cm)->PDIV)

/* OST : CM Oscillator Stabilization Time Register                           */
#define CSP_CM_SET_OST(cm, val)     ((cm)->OST = (val & 0xffff) | OSTKEY)
#define CSP_CM_GET_OST(cm)          ((cm)->OST)

/* MDIV : CM Master Clock Divider Register                                   */
#define CSP_CM_SET_MDIV(cm, val)    ((cm)->MDIV = (val & 0xffff) | MDIVKEY)
#define CSP_CM_GET_MDIV(cm)         ((cm)->MDIV)


/******************************************************************************
********************** CM External Functions Declaration **********************
******************************************************************************/


#endif   /* CSP_CM_H */
