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
* File Name       : csp_gic.h
* Description     : Definitions, Macros and function declarations for
*                   Generic Interrupt Controller module
* Library Version : 2.00
* Module Version  : 1.XX
*
*       +----- (NEW | MODify | ADD | DELete)                                 
*       |                                                                    
*  No   |   When       Who                What               
*-----+---+----------+------------------+--------------------------------------
* 000  NEW  01/05/99   Patrice VILCHEZ    Creation
* 001  ADD  06/12/99   Patrice VILCHEZ    Add peripheral & bits declaration
* 002  MOD  01/04/01   Olivier MAZUYER    Clean up
* 003  MOD  08/06/01   Frederic SAMSON    Clean Up 
* 004  MOD  22/10/01   Christophe GARDIN  Clean Up
*----------------------------------------------------------------------------*/

#ifndef CSP_GIC_H
#define CSP_GIC_H


/******************************************************************************
************************* GIC Structure Definition ****************************
******************************************************************************/

/* Number of Interrupt Vectors                                               */
#define NB_INTERRUPT       32u
/* Number of Priority Level                                                  */
#define NB_INTERRUPT_LEVEL  8u

/* GIC Structure Definition                                                  */
typedef struct
{ 
   CSP_REGISTER_T  SMR[NB_INTERRUPT];  /* Source Mode Registers              */
   CSP_REGISTER_T  SVR[NB_INTERRUPT];  /* Source Vector Registers            */
   CSP_REGISTER_T  IVR;                /* IRQ Vector Register                */ 
   CSP_REGISTER_T  FVR;                /* FIQ Vector Register                */ 
   CSP_REGISTER_T  ISR;                /* Interrupt Status Register          */
   CSP_REGISTER_T  IPR;                /* Interrupt Pending Register         */  
   CSP_REGISTER_T  IMR;                /* Interrupt Mask Register            */
   CSP_REGISTER_T  CISR;               /* Core Interrupt Status Register     */
   CSP_REGISTER_T  ReservedA;          
   CSP_REGISTER_T  ReservedB;          
   CSP_REGISTER_T  IECR;               /* Interrupt Enable Command Register  */ 
   CSP_REGISTER_T  IDCR;               /* Interrupt Disable Command Register */ 
   CSP_REGISTER_T  ICCR;               /* Interrupt Clear Command Register   */
   CSP_REGISTER_T  ISCR;               /* Interrupt Set Command Register     */
   CSP_REGISTER_T  EOICR;              /* End of Interrupt Command Register  */
   CSP_REGISTER_T  SPU;                /* Spurious Vector Register           */
} CSP_GIC_T, *CSP_GIC_PTR;


/******************************************************************************
************************** GIC Registers Definition ***************************
******************************************************************************/

/******************************************************************************
* SMR : GIC Source Mode Register
******************************************************************************/
/* PRIOR : Priority Level                                                    */
#define PRIOR     (0x07ul << 0)        /* Priority Mask                      */ 
#define PRIOR_0   (0x00ul << 0)        /* Priority Level 0                   */
#define PRIOR_1   (0x01ul << 0)        /* Priority Level 1                   */
#define PRIOR_2   (0x02ul << 0)        /* Priority Level 2                   */
#define PRIOR_3   (0x03ul << 0)        /* Priority Level 3                   */
#define PRIOR_4   (0x04ul << 0)        /* Priority Level 4                   */
#define PRIOR_5   (0x05ul << 0)        /* Priority Level 5                   */
#define PRIOR_6   (0x06ul << 0)        /* Priority Level 6                   */
#define PRIOR_7   (0x07ul << 0)        /* Priority Level 7                   */

/* SRCTYP : Interrupt Source Type                                            */
#define SRCTYP                         (0x03ul << 5)  /* SRCTYP Mask         */
#define LOW_LEVEL_SENSITIVE            (0x00ul << 5)  /* Low Level Sensitive Type     */
#define NEGATIVE_EDGE_TRIGGERED        (0x01ul << 5)  /* Negative Edge Triggered Type */
#define HIGH_LEVEL_SENSITIVE           (0x02ul << 5)  /* High Level Sensitive Type    */
#define POSITIVE_EDGE_TRIGGERED        (0x03ul << 5)  /* Positive Edge Triggered Type */

/******************************************************************************
* ISR : GIC Interrupt Status Register
******************************************************************************/
#define IRQID     (0x1Ful << 0)        /* Current IRQ Identifier Mask        */

/******************************************************************************
* IPR, IMR, IECR, IDCR, ICCR, ISCR : GIC Interrupt Registers
******************************************************************************/
/* See csp.h file                                                            */
 
/******************************************************************************
* CISR : GIC Core Interrupt Status Register
******************************************************************************/
#define NFIQ      (0x01ul << 0)        /* NFIQ Status                        */
#define NIRQ      (0x01ul << 1)        /* NIRQ Status                        */


/******************************************************************************
*************************** GIC Macros Definition *****************************
******************************************************************************/
/* SMR : GIC Source Mode Register                                            */
#define CSP_GIC_GET_SMR(gic, interrupt)         ((gic)->SMR[(interrupt)])
#define CSP_GIC_SET_SMR(gic, interrupt, val)    ((gic)->SMR[(interrupt)] = (val))

/* SVR : GIC Source Vertor Register                                          */
#define CSP_GIC_GET_SVR(gic, interrupt)         ((gic)->SVR[(interrupt)])
#define CSP_GIC_SET_SVR(gic, interrupt, vector) ((gic)->SVR[(interrupt)] = (vector))

/* IVR : GIC IRQ Vector Register                                             */
#define CSP_GIC_GET_IVR(gic)              ((gic)->IVR)

/* FVR : GIC FIQ Vector Register                                             */
#define CSP_GIC_GET_FVR(gic)              ((gic)->FVR)

/* ISR, IPR, IMR : GIC Interrupt Registers                                   */
#define CSP_GIC_GET_ISR(gic)              ((gic)->ISR)            /* Status  */
#define CSP_GIC_GET_IPR(gic)              ((gic)->IPR)            /* Pending */
#define CSP_GIC_GET_IMR(gic)              ((gic)->IMR)            /* Mask    */

/* CISR : GIC Core Interrupt Status Register                                 */
#define CSP_GIC_GET_CISR(gic)             ((gic)->CISR)

/* IECR, IDCR, ICCR, ISCR : GIC Interrupt Command Registers                  */
#define CSP_GIC_SET_IECR(gic, val)        ((gic)->IECR = (val))   /* Enable  */
#define CSP_GIC_SET_IDCR(gic, val)        ((gic)->IDCR = (val))   /* Disable */
#define CSP_GIC_SET_ICCR(gic, val)        ((gic)->ICCR = (val))   /* Clear   */
#define CSP_GIC_SET_ISCR(gic, val)        ((gic)->ISCR = (val))   /* Set     */

/* EOICR : GIC End of Interrupt Command Register                             */
#define CSP_GIC_SET_EOICR(gic, val)       ((gic)->EOICR = (val))

/* SPU : GIC Spurious Vector Register                                        */
#define CSP_GIC_GET_SPU(gic)              ((gic)->SPU)
#define CSP_GIC_SET_SPU(gic, vector)      ((gic)->SPU = (vector))

         
/******************************************************************************
********************* GIC External Functions Declaration **********************
******************************************************************************/
extern void CSP_GICInit(void);
extern void CSP_GICConfigInterrupt(U8_T int_num, U32_T int_mode, U32_T callback);
extern void CSP_GICUndefinedIntHandler(void);
extern void CSP_ASM_GICSpuriousIntHandler(void);

#endif   /* CSP_GIC_H */
