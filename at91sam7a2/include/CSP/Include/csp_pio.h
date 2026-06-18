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
* File Name       : csp_pio.h
* Description     : Definitions, Macros and function declarations for
*                   Parallel Input/Ouput module
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
*----------------------------------------------------------------------------*/

#ifndef CSP_PIO_H
#define CSP_PIO_H


/******************************************************************************
************************** PIO Structure Definition ***************************
******************************************************************************/
typedef struct
{
   CSP_REGISTER_T  ReservedA[4];
   CSP_REGISTER_T  OER;          /* Output Enable Register                   */
   CSP_REGISTER_T  ODR;          /* Output Disable Register                  */
   CSP_REGISTER_T  OSR;          /* Output Status Register                   */
   CSP_REGISTER_T  ReservedB[5];     
   CSP_REGISTER_T  SODR;         /* Set Output Data Register                 */
   CSP_REGISTER_T  CODR;         /* Clear Output Data Register               */
   CSP_REGISTER_T  ODSR;         /* Output Data Status Register              */
   CSP_REGISTER_T  PDSR;         /* Pin Data Status Register                 */
   CSP_REGISTER_T  MDER;         /* Multi-Driver Enable Register             */
   CSP_REGISTER_T  MDDR;         /* Multi-Driver Disable Register            */
   CSP_REGISTER_T  MDSR;         /* Multi-Driver Status Register             */
   CSP_REGISTER_T  ReservedC;     
   CSP_REGISTER_T  ECR;          /* Enable Clock Register                    */
   CSP_REGISTER_T  DCR;          /* Disable Clock Register                   */
   CSP_REGISTER_T  PMSR;         /* Power Management Status Register         */
   CSP_REGISTER_T  ReservedD;     
   CSP_REGISTER_T  CR;           /* Control Register                         */
   CSP_REGISTER_T  ReservedE[3];    
   CSP_REGISTER_T  SR;           /* Status Register                          */
   CSP_REGISTER_T  IER;          /* Interrupt Enable Register                */
   CSP_REGISTER_T  IDR;          /* Interrupt Disable Register               */
   CSP_REGISTER_T  IMR;          /* Interrupt Mask Register                  */
} CSP_PIO_T, *CSP_PIO_PTR;


/******************************************************************************
************************** PIO Registers Definition ***************************
******************************************************************************/

/******************************************************************************
* OER, ODR, OSR,                     :
* SODR, CODR, ODSR, PDSR, MDER, MDDR : PIO Registers, Status Register and 
* MDSR, SR, IER, IDR, IMR            : Interrupt Registers
******************************************************************************/
#define PIO0   (0x01ul << 0)        /* Pin 0                                 */
#define PIO1   (0x01ul << 1)        /* Pin 1                                 */
#define PIO2   (0x01ul << 2)        /* Pin 2                                 */
#define PIO3   (0x01ul << 3)        /* Pin 3                                 */
#define PIO4   (0x01ul << 4)        /* Pin 4                                 */
#define PIO5   (0x01ul << 5)        /* Pin 5                                 */
#define PIO6   (0x01ul << 6)        /* Pin 6                                 */
#define PIO7   (0x01ul << 7)        /* Pin 7                                 */
#define PIO8   (0x01ul << 8)        /* Pin 8                                 */
#define PIO9   (0x01ul << 9)        /* Pin 9                                 */
#define PIO10  (0x01ul << 10)       /* Pin 10                                */
#define PIO11  (0x01ul << 11)       /* Pin 11                                */
#define PIO12  (0x01ul << 12)       /* Pin 12                                */
#define PIO13  (0x01ul << 13)       /* Pin 13                                */
#define PIO14  (0x01ul << 14)       /* Pin 14                                */
#define PIO15  (0x01ul << 15)       /* Pin 15                                */
#define PIO16  (0x01ul << 16)       /* Pin 16                                */
#define PIO17  (0x01ul << 17)       /* Pin 17                                */
#define PIO18  (0x01ul << 18)       /* Pin 18                                */
#define PIO19  (0x01ul << 19)       /* Pin 19                                */
#define PIO20  (0x01ul << 20)       /* Pin 20                                */
#define PIO21  (0x01ul << 21)       /* Pin 21                                */
#define PIO22  (0x01ul << 22)       /* Pin 22                                */
#define PIO23  (0x01ul << 23)       /* Pin 23                                */
#define PIO24  (0x01ul << 24)       /* Pin 24                                */
#define PIO25  (0x01ul << 25)       /* Pin 25                                */
#define PIO26  (0x01ul << 26)       /* Pin 26                                */
#define PIO27  (0x01ul << 27)       /* Pin 27                                */
#define PIO28  (0x01ul << 28)       /* Pin 28                                */
#define PIO29  (0x01ul << 29)       /* Pin 29                                */
#define PIO30  (0x01ul << 30)       /* Pin 30                                */
#define PIO31  (0x01ul << 31)       /* Pin 31                                */

/******************************************************************************
* ECR, DCR, PMSR : PIO Power Management Registers
******************************************************************************/
#define PIO    (0x01ul << 0)        /* PIO Clock                             */

/******************************************************************************
* CR : PIO Control Register
******************************************************************************/
#define SWRST  (0x01ul << 0)        /* PIO Software Reset                    */


/******************************************************************************
*************************** PIO Macros Definition *****************************
******************************************************************************/
/* OER, ODR, OSR : PIO Output Registers                                      */
#define CSP_PIO_SET_OER(pio, val)      ((pio)->OER = (val))    /* Enable     */
#define CSP_PIO_SET_ODR(pio, val)      ((pio)->ODR = (val))    /* Disable    */
#define CSP_PIO_GET_OSR(pio)           ((pio)->OSR)            /* Status     */

/* SODR, CODR, ODSR, PDSR : PIO Output Data Registers                        */
#define CSP_PIO_SET_SODR(pio, val)     ((pio)->SODR = (val))   /* Set        */
#define CSP_PIO_SET_CODR(pio, val)     ((pio)->CODR = (val))   /* Clear      */
#define CSP_PIO_GET_ODSR(pio)          ((pio)->ODSR)           /* Status     */
#define CSP_PIO_GET_PDSR(pio)          ((pio)->PDSR)           /* Pin Status */

/* MDER, MDDR, MDSR : PIO Multi-Driver Registers                             */
#define CSP_PIO_SET_MDER(pio, val)     ((pio)->MDER = (val))   /* Enable     */
#define CSP_PIO_SET_MDDR(pio, val)     ((pio)->MDDR = (val))   /* Disable    */
#define CSP_PIO_GET_MDSR(pio)          ((pio)->MDSR)           /* Status     */

/* ECR, DCR, PMSR : PIO Power Management Registers                           */
#define CSP_PIO_SET_ECR(pio, val)      ((pio)->ECR = (val))    /* Enable     */
#define CSP_PIO_SET_DCR(pio, val)      ((pio)->DCR = (val))    /* Disable    */
#define CSP_PIO_GET_PMSR(pio)          ((pio)->PMSR)           /* Status     */

/* CR : PIO Control Register                                                 */
#define CSP_PIO_SET_CR(pio, val)       ((pio)->CR = (val))

/* SR : PIO Status Register                                                  */
#define CSP_PIO_GET_SR(pio)            ((pio)->SR)             /* Status     */

/* IER, IDR, IMR : PIO Interrupt Registers                                   */
#define CSP_PIO_SET_IER(pio, val)      ((pio)->IER = (val))    /* Enable     */
#define CSP_PIO_SET_IDR(pio, val)      ((pio)->IDR = (val))    /* Disable    */
#define CSP_PIO_GET_IMR(pio)           ((pio)->IMR)            /* Mask       */


/******************************************************************************
********************* PIO External Functions Declaration **********************
******************************************************************************/
extern void CSP_PIOInit(CSP_PIO_T *const pio, U32_T output_pio, U32_T multidriver_pio);
extern void CSP_PIOClose(CSP_PIO_T *const pio);
extern void CSP_PIOConfigInterrupt(CSP_PIO_T *const pio,U32_T int_mode, U32_T int_mask, U32_T callback);
extern U32_T CSP_PIOGetStatus(CSP_PIO_T *const pio);
extern void CSP_PIOSet(CSP_PIO_T *const pio, U32_T pio_mask);
extern void CSP_PIOClear(CSP_PIO_T *const pio, U32_T pio_mask);


#endif   /* CSP_PIO_H */

