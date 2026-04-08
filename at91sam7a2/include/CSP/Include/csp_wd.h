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
* File Name       : csp_wd.h
* Description     : Definitions, Macros and function declarations for
*                   WatchDog module
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
* 005  MOD  15/03/02   Christophe GARDIN  Add Macros
* 006  MOD  17/07/02   Christophe GARDIN  Modify structure and registers for
*                                         new module version
*----------------------------------------------------------------------------*/

#ifndef CSP_WD_H
#define CSP_WD_H


/******************************************************************************
************************** WD Structure Definition ****************************
******************************************************************************/
typedef struct
{ 
   CSP_REGISTER_T  ReservedA[24];
   CSP_REGISTER_T  CR;                    /* Control Register                */
   CSP_REGISTER_T  MR;                    /* Mode Register                   */
   CSP_REGISTER_T  OMR;                   /* Overflow Mode Register          */
   CSP_REGISTER_T  CSR;                   /* Clear Status Register           */
   CSP_REGISTER_T  SR;                    /* Status Register                 */
   CSP_REGISTER_T  IER;                   /* Interrupt Enable Register       */
   CSP_REGISTER_T  IDR;                   /* Interrupt Disable Register      */
   CSP_REGISTER_T  IMR;                   /* Interrupt Mask Register         */
   CSP_REGISTER_T  PWR;                   /* Pending Window Register         */
} CSP_WD_T, *CSP_WD_PTR;


/******************************************************************************
************************** WD Registers Definition ****************************
******************************************************************************/

/******************************************************************************
* CR : WD Control Register
******************************************************************************/
#define RSTKEY          (0xC071ul << 0)   /* Restart key                     */

/******************************************************************************
* MR : WD Mode Register
******************************************************************************/
/* WDPDIV : Clock Divider                                                    */
#define WDPDIV          (0x07ul << 0)     /* WDPDIV Mask                     */
#define WDPDIVCLK_2     (0x00ul << 0)     /* WDPDIVCLK / 2                   */
#define WDPDIVCLK_4     (0x01ul << 0)     /* WDPDIVCLK / 4                   */
#define WDPDIVCLK_8     (0x02ul << 0)     /* WDPDIVCLK / 8                   */
#define WDPDIVCLK_16    (0x03ul << 0)     /* WDPDIVCLK / 16                  */
#define WDPDIVCLK_32    (0x04ul << 0)     /* WDPDIVCLK / 32                  */
#define WDPDIVCLK_128   (0x05ul << 0)     /* WDPDIVCLK / 128                 */
#define WDPDIVCLK_256   (0x06ul << 0)     /* WDPDIVCLK / 256                 */
#define WDPDIVCLK_1024  (0x07ul << 0)     /* WDPDIVCLK / 1024                */

/* HPCV : High Preload Counter Value                                         */
#define HPCV_MASK       (0xFFFFul << 8)            /* Mask                   */
#define HPCV(val)       (((val) & 0xFFFFul) << 8)  /* Writing Macro          */

/* CKEY : Clock Access Key                                                   */
#define CKEY            (0xFFul << 24)    /* CKEY Mask                       */
#define CKEY_ALLOW      (0x37ul << 24)    /* Write Access in MR Allowed      */

/******************************************************************************
* OMR : WD Overflow Mode Register
******************************************************************************/
#define WDEN            (0x01ul << 0)     /* Watchdog Enable                 */
#define RSTEN           (0x01ul << 1)     /* Reset Enable                    */

/* OKEY : Overflow access key                                                */
#define OKEY            (0xFFFul << 4)    /* OKEY Mask                       */
#define OKEY_ALLOW      (0x234ul << 4)    /* Write Access in OMR Allowed     */

/******************************************************************************
* CSR, SR, IER, IDR, IMR : WD Status and Interrupt Registers
******************************************************************************/
#define WDPEND          (0x01ul << 0)     /* Watchdog Pending                */
#define WDOVF           (0x01ul << 1)     /* Watchdog Overflow               */

/* SR Register Only                                                          */
#define PENDING         (0x01ul << 8)     /* Watchdog Pending Status         */

/******************************************************************************
* PWR : WD Pending Window Register
******************************************************************************/
#define RSTALW          (0x01ul << 0)     /* Restart Allowed                 */

/* PWL : Pending Window Length                                               */
#define PWL_MASK       (0xFFFFul << 8)             /* Mask                   */
#define PWL(val)       (((val) & 0xFFFFul) << 8)   /* Writing Macro          */

/* PWKEY : Clock access key                                                  */
#define PWKEY           (0xFFul << 24)    /* PWKEY Mask                      */
#define PWKEY_ALLOW     (0x91ul << 24)    /* Write Access in PWR Allowed     */

/******************************************************************************
**************************** WD Macros Definition *****************************
******************************************************************************/
/* CR : WD Control Register                                                  */
#define CSP_WD_SET_CR(wd, val)      ((wd)->CR = (val))

/* MR : WD Mode Register                                                     */
#define CSP_WD_GET_MR(wd)           ((wd)->MR)
#define CSP_WD_SET_MR(wd, mode)     ((wd)->MR = (mode))

/* OMR : WD Overflow Mode Register                                           */
#define CSP_WD_GET_OMR(wd)          ((wd)->OMR)
#define CSP_WD_SET_OMR(wd, val)     ((wd)->OMR = (val))

/* CSR, SR : WD Status Registers                                             */
#define CSP_WD_SET_CSR(wd, val)     ((wd)->CSR = (val))        /* Clear      */
#define CSP_WD_GET_SR(wd)           ((wd)->SR)                 /* Status     */

/* IER, IDR, IMR : WD Interrupt Registers                                    */
#define CSP_WD_SET_IER(wd, val)     ((wd)->IER = (val))        /* Enable     */
#define CSP_WD_SET_IDR(wd, val)     ((wd)->IDR = (val))        /* Disable    */
#define CSP_WD_GET_IMR(wd)          ((wd)->IMR)                /* Mask       */

/* PWR : WD Pending Window Register                                          */
#define CSP_WD_GET_PWR(wd)          ((wd)->PWR)
#define CSP_WD_SET_PWR(wd, val)     ((wd)->PWR = (val))


/******************************************************************************
********************** WD External Functions Declaration **********************
******************************************************************************/
extern void CSP_WDInit(CSP_WD_T *const wd, U32_T mode, U32_T overflow_mode, U32_T pending_mode);
extern void CSP_WDConfigInterrupt(CSP_WD_T *const wd, U32_T int_mode, U32_T int_mask, U32_T callback);
extern void CSP_WDEnable(CSP_WD_T *const wd);
extern void CSP_WDDisable(CSP_WD_T *const wd);
extern void CSP_WDCounterRestart(CSP_WD_T *const wd);


#endif   /* CSP_WD_H */
