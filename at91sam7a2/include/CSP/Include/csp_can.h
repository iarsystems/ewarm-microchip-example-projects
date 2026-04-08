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
* File Name       : csp_can.h
* Description     : Definitions, Macros and function declarations for
*                   Controller Area Network module
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
* 003  ADD  24/05/01   Tristan BONHOMME   Add Spy bit
* 004  ADD  26/05/01   Patrice VILCHEZ    Add CSP_CAN_GET_ID(ID) Macro
                                          Add CSP_CAN_SET_ID(ID) Macro
* 005  MOD  08/06/01   Frederic SAMSON    Clean Up
* 006  MOD  26/10/01   Christophe GARDIN  Clean Up
* 007  MOD  11/03/02   Christophe GARDIN  Add macros
* 008  MOD  24/06/02   Christophe GARDIN  Modify BD and DATA bits
*----------------------------------------------------------------------------*/

#ifndef CSP_CAN_H
#define CSP_CAN_H


/******************************************************************************
************************** CAN Structure Definition ***************************
******************************************************************************/

/* Number of CAN Channels                                                    */
#define NB_CAN_CHANNEL     32u

/******************************************************************************
* CAN Channel Structure
******************************************************************************/
typedef struct
{ 
   CSP_REGISTER_T  ReservedA[5];   
   CSP_REGISTER_T  DRA;             /* Data Register A Channel X             */
   CSP_REGISTER_T  DRB;             /* Data Register B Channel X             */
   CSP_REGISTER_T  MSK;             /* Mask Register Channel X               */
   CSP_REGISTER_T  IR;              /* Identifier Register Channel X         */
   CSP_REGISTER_T  CR;              /* Control Register Channel X            */
   CSP_REGISTER_T  STP;             /* Stamp Register Channel X              */
   CSP_REGISTER_T  CSR;             /* Clear Status Register Channel X       */ 
   CSP_REGISTER_T  SR;              /* Status Register Channel X             */
   CSP_REGISTER_T  IER;             /* Interrupt Enable Register Channel X   */
   CSP_REGISTER_T  IDR;             /* Interrupt Disable Register Channel X  */
   CSP_REGISTER_T  IMR;             /* Interrupt Mask Register Channel X     */
} CSP_CAN_CHANNEL_T;

/******************************************************************************
* CAN Structure 32 Channels
******************************************************************************/
typedef struct
{ 
   CSP_REGISTER_T     ReservedA[20]; 
   CSP_REGISTER_T     ECR;          /* Enable Clock Register                 */
   CSP_REGISTER_T     DCR;          /* Disable Clock Register                */
   CSP_REGISTER_T     PMSR;         /* Power Management Status Register      */
   CSP_REGISTER_T     ReservedB;      
   CSP_REGISTER_T     CR;           /* Control Register                      */
   CSP_REGISTER_T     MR;           /* Mode Register                         */
   CSP_REGISTER_T     ReservedC;      
   CSP_REGISTER_T     CSR;          /* Clear Status Register                 */
   CSP_REGISTER_T     SR;           /* Status Register                       */
   CSP_REGISTER_T     IER;          /* Interrupt Enable Register             */
   CSP_REGISTER_T     IDR;          /* Interrupt Disable Register            */
   CSP_REGISTER_T     IMR;          /* Interrupt Mask Register               */
   CSP_REGISTER_T     CISR;         /* Clear Interrupt Source Register       */
   CSP_REGISTER_T     ISSR;         /* Interrupt Source Status Register      */
   CSP_REGISTER_T     SIER;         /* Source Interrupt Enable Register      */
   CSP_REGISTER_T     SIDR;         /* Source Interrupt Disable Register     */
   CSP_REGISTER_T     SIMR;         /* Source Interrupt Mask Register        */
   CSP_REGISTER_T     ReservedD[22];  
   CSP_CAN_CHANNEL_T  CHANNEL[NB_CAN_CHANNEL];  /* CAN Channels              */
} CSP_CAN_T, *CSP_CAN_PTR;


/******************************************************************************
************************** CAN Registers Definition ***************************
******************************************************************************/

/******************************************************************************
* ECR, DCR, PMSR : CAN Power Management Registers
******************************************************************************/
#define CAN          (0x01ul << 1)              /* CAN Clock                 */

/******************************************************************************
* CR : CAN Control Register
******************************************************************************/
#define SWRST        (0x01ul << 0)              /* CAN Software Reset        */  
#define CANEN        (0x01ul << 1)              /* CAN Enable                */ 
#define CANDIS       (0x01ul << 2)              /* CAN Disable               */ 
#define ABEN         (0x01ul << 3)              /* Abort Request Enable      */ 
#define ABDIS        (0x01ul << 4)              /* Abort Request Disable     */ 
#define OVEN         (0x01ul << 5)              /* Overload Request Enable   */ 
#define OVDIS        (0x01ul << 6)              /* Overload Request Disable  */ 

/******************************************************************************
* MR : CAN Mode Register
******************************************************************************/
/* BD : Baud rate Prescalar                                                  */
#define BD_MASK      (0x3Ful << 0)              /* Mask                      */
#define BD(val)      (((val) & 0x3Ful) << 0)    /* Writing Macro             */

/* PROP : Propagation Segment Value                                          */
#define PROP_MASK    (0x07ul << 8)              /* Mask                      */
#define PROP(val)    (((val) & 0x07ul) << 8)    /* Writing Macro             */

/* SJW : Synchronization Jump width                                          */
#define SJW_MASK     (0x03ul << 12)             /* Mask                      */
#define SJW(val)     (((val) & 0x03ul) << 12)   /* Writing Macro             */

#define SMP          (0x01ul << 14)             /* Sampling Mode             */

/* PHSEG1 : Phase Segment 1 Value                                            */
#define PHSEG1_MASK  (0x07ul << 16)             /* Mask                      */    
#define PHSEG1(val)  (((val) & 0x07ul) << 16)   /* Writing Macro             */    
                                                
/* PHSEG2 : Phase Segment 2 Value                                            */
#define PHSEG2_MASK  (0x07ul << 20)             /* Mask                      */
#define PHSEG2(val)  (((val) & 0x07ul) << 20)   /* Writing Macro             */    

/******************************************************************************
* CSR, SR, IER, IDR, IMR : CAN Status and Interrupt Registers
******************************************************************************/
#define ENDINIT      (0x01ul << 2)              /* End of CAN Initialization */

/* SR, IER, IDR, IMR Registers Only                                          */
#define ERPAS        (0x01ul << 3)              /* Error Passive             */
#define BUSOFF       (0x01ul << 4)              /* Bus Off                   */

/* SR Register Only                                                          */
#define CANENA       (0x01ul << 0)              /* CAN Enable                */
#define CANINIT      (0x01ul << 1)              /* CAN Initialized           */
#define ABRQ         (0x01ul << 5)              /* CAN Abort Request         */
#define OVRQ         (0x01ul << 6)              /* CAN Overload Request      */
#define ISS          (0x01ul << 7)              /* Interrupt Source Status   */
#define REC          (0xFFul << 16)             /* Reception Error Counter   */
#define TEC          (0xFFul << 24)             /* Transmit Error Counter    */

/******************************************************************************
* CISR, ISSR, SIER, SIDR, SIMR : CAN Interrupt Source Registers
******************************************************************************/
#define CH0          (0x01ul << 0)              /* Interrupt channel 0       */
#define CH1          (0x01ul << 1)              /* Interrupt channel 1       */
#define CH2          (0x01ul << 2)              /* Interrupt channel 2       */
#define CH3          (0x01ul << 3)              /* Interrupt channel 3       */
#define CH4          (0x01ul << 4)              /* Interrupt channel 4       */
#define CH5          (0x01ul << 5)              /* Interrupt channel 5       */
#define CH6          (0x01ul << 6)              /* Interrupt channel 6       */
#define CH7          (0x01ul << 7)              /* Interrupt channel 7       */
#define CH8          (0x01ul << 8)              /* Interrupt channel 8       */
#define CH9          (0x01ul << 9)              /* Interrupt channel 9       */
#define CH10         (0x01ul << 10)             /* Interrupt channel 10      */
#define CH11         (0x01ul << 11)             /* Interrupt channel 11      */
#define CH12         (0x01ul << 12)             /* Interrupt channel 12      */
#define CH13         (0x01ul << 13)             /* Interrupt channel 13      */
#define CH14         (0x01ul << 14)             /* Interrupt channel 14      */
#define CH15         (0x01ul << 15)             /* Interrupt channel 15      */
#define CH16         (0x01ul << 16)             /* Interrupt channel 16      */
#define CH17         (0x01ul << 17)             /* Interrupt channel 17      */
#define CH18         (0x01ul << 18)             /* Interrupt channel 18      */
#define CH19         (0x01ul << 19)             /* Interrupt channel 19      */
#define CH20         (0x01ul << 20)             /* Interrupt channel 20      */
#define CH21         (0x01ul << 21)             /* Interrupt channel 21      */
#define CH22         (0x01ul << 22)             /* Interrupt channel 22      */
#define CH23         (0x01ul << 23)             /* Interrupt channel 23      */
#define CH24         (0x01ul << 24)             /* Interrupt channel 24      */
#define CH25         (0x01ul << 25)             /* Interrupt channel 25      */
#define CH26         (0x01ul << 26)             /* Interrupt channel 26      */
#define CH27         (0x01ul << 27)             /* Interrupt channel 27      */
#define CH28         (0x01ul << 28)             /* Interrupt channel 28      */
#define CH29         (0x01ul << 29)             /* Interrupt channel 29      */
#define CH30         (0x01ul << 30)             /* Interrupt channel 30      */
#define CH31         (0x01ul << 31)             /* Interrupt channel 31      */

/******************************************************************************
* MSK : CAN Mask Register Channel X
******************************************************************************/
/* MASK : Identifier Mask                                                    */
#define MASK_MASK    (0x1FFFFFFFul << 0)           /* Mask                   */
#define MASK(val)    (((val) & 0x1FFFFFFFul) << 0) /* Writing Macro          */  

/* MRB : Reserved Mask Bits                                                  */
#define MRB_MASK     (0x03ul << 29)                /* Mask                   */                   
#define MRB(val)     (((val) & 0x03ul) << 29)      /* Writing Macro          */  

#define MRTR         (0x01ul << 31)          /* Remote Transmission Resquest */ 

/******************************************************************************
* IR : CAN Identifier Register Channel X
******************************************************************************/
/* ID : Identifier                                                           */
#define ID_MASK      (0x1FFFFFFFul << 0)           /* Mask                   */
#define ID(val)      (((val) & 0x1FFFFFFFul) << 0) /* Writing Macro          */

/* IRB : Reserved Bits                                                       */
#define IRB_MASK     (0x03ul << 29)                /* Mask                   */
#define IRB(val)     (((val) & 0x03ul) << 29)      /* Writing Macro          */

#define RTR          (0x01ul << 31)          /* Remote Transmission Resquest */

/******************************************************************************
* CR : CAN Control Register Channel X
******************************************************************************/
/* DLC : Data Length Code                                                    */
#define DLC_MASK     (0x0Ful << 0)              /* Mask                      */
#define DLC(val)     (((val) & 0x0Ful) << 0)    /* Writing Macro             */       

#define IDE          (0x01ul << 4)              /* Extended Identifier Flag  */
#define RPLYV        (0x01ul << 5)              /* Automatic Reply           */ 
#define PCB          (0x01ul << 6)              /* Channel Producer          */
#define CHANEN       (0x01ul << 7)              /* Channel Enable            */
#define OVERWRITE    (0x01ul << 8)              /* Channel Overwrite Mode    */

/******************************************************************************
* CSR, SR, IER, IDR, IMR : CAN Interrupt Registers Channel X
******************************************************************************/
#define ACK          (0x01ul << 0)              /* Acknowledge Error         */
#define CAN_FRAME    (0x01ul << 1)              /* Frame Error               */
#define CRC          (0x01ul << 2)              /* CRC Error                 */
#define STUFF        (0x01ul << 3)              /* Stuffing Error            */
#define BUS          (0x01ul << 4)              /* Bus Error                 */
#define RXOK         (0x01ul << 5)              /* Reception Completed       */
#define TXOK         (0x01ul << 6)              /* Transmission Completed    */
#define RFRAME       (0x01ul << 7)              /* Remote Frame              */

/* CSR and SR Registers Only                                                 */
#define DLCW         (0x01ul << 9)              /* DLC Warning               */
#define FILLED       (0x01ul << 10)             /* Filled Flag               */
#define OVRUN        (0x01ul << 11)             /* Overrun Flag              */


/******************************************************************************
*************************** CAN Macros Definition *****************************
******************************************************************************/
/* ECR, DCR, PMSR : CAN Power Management Registers                           */
#define CSP_CAN_SET_ECR(can, val)         ((can)->ECR = (val))    /* Enable  */
#define CSP_CAN_SET_DCR(can, val)         ((can)->DCR = (val))    /* Disable */
#define CSP_CAN_GET_PMSR(can)             ((can)->PMSR)           /* Status  */

/* CR : CAN Control Register                                                 */
#define CSP_CAN_SET_CR(can, val)          ((can)->CR = (val))

/* MR : CAN Mode Register                                                    */
#define CSP_CAN_GET_MR(can)               ((can)->MR)
#define CSP_CAN_SET_MR(can, mode)         ((can)->MR = (mode))

/* CSR, SR : CAN Status Register                                             */
#define CSP_CAN_SET_CSR(can, val)         ((can)->CSR = (val))    /* Clear   */
#define CSP_CAN_GET_SR(can)               ((can)->SR)             /* Status  */

/* IER, IDR, IMR : CAN Interrupt Registers                                   */
#define CSP_CAN_SET_IER(can, val)         ((can)->IER = (val))    /* Enable  */
#define CSP_CAN_SET_IDR(can, val)         ((can)->IDR = (val))    /* Disable */
#define CSP_CAN_GET_IMR(can)              ((can)->IMR)            /* Mask    */

/* CISR, ISSR, SIER, SIDR, SIMR : CAN Interrupt Source Registers             */
#define CSP_CAN_SET_CISR(can, val)        ((can)->CISR = (val))   /* Clear   */
#define CSP_CAN_GET_ISSR(can)             ((can)->ISSR)           /* Status  */
#define CSP_CAN_SET_SIER(can, val)        ((can)->SIER = (val))   /* Enable  */
#define CSP_CAN_SET_SIDR(can, val)        ((can)->SIDR = (val))   /* Disable */
#define CSP_CAN_GET_SIMR(can)             ((can)->SIMR)           /* Mask    */

/* DRA : CAN Data A Register Channel X                                       */
#define CSP_CAN_CHANNEL_GET_DRA(can, channel)       ((can)->CHANNEL[(channel)].DRA)
#define CSP_CAN_CHANNEL_SET_DRA(can, channel, val)  ((can)->CHANNEL[(channel)].DRA = (val))

/* DRB : CAN Data B Register Channel X                                       */
#define CSP_CAN_CHANNEL_GET_DRB(can, channel)       ((can)->CHANNEL[(channel)].DRB)
#define CSP_CAN_CHANNEL_SET_DRB(can, channel, val)  ((can)->CHANNEL[(channel)].DRB = (val))

/* MSK : CAN Mask Register Channel X                                         */
#define CSP_CAN_CHANNEL_GET_MSK(can, channel)       ((can)->CHANNEL[(channel)].MSK)
#define CSP_CAN_CHANNEL_SET_MSK(can, channel, val)  ((can)->CHANNEL[(channel)].MSK = (val))

/* IR : CAN Identifier Register Channel X                                    */
#define CSP_CAN_CHANNEL_GET_IR(can, channel)        ((can)->CHANNEL[(channel)].IR)
#define CSP_CAN_CHANNEL_SET_IR(can, channel, val)   ((can)->CHANNEL[(channel)].IR = (val))

/* CR : CAN Control Register Channel X                                       */
#define CSP_CAN_CHANNEL_GET_CR(can, channel)        ((can)->CHANNEL[(channel)].CR)
#define CSP_CAN_CHANNEL_SET_CR(can, channel, val)   ((can)->CHANNEL[(channel)].CR = (val))

/* STP : CAN Stamp Register Channel X                                        */ 
#define CSP_CAN_CHANNEL_GET_STP(can, channel)       ((can)->CHANNEL[(channel)].STP)

/* CSR : CAN Clear Status Register Channel X                                 */
#define CSP_CAN_CHANNEL_SET_CSR(can, channel, val)  ((can)->CHANNEL[(channel)].CSR = (val))

/* SR : CAN Status Register Channel X                                        */
#define CSP_CAN_CHANNEL_GET_SR(can, channel)        ((can)->CHANNEL[(channel)].SR)

/* IER, IDR, IMR : CAN Interrupt Registers Channel X                         */
#define CSP_CAN_CHANNEL_SET_IER(can, channel, val)  ((can)->CHANNEL[(channel)].IER = (val))
#define CSP_CAN_CHANNEL_SET_IDR(can, channel, val)  ((can)->CHANNEL[(channel)].IDR = (val))
#define CSP_CAN_CHANNEL_GET_IMR(can, channel)       ((can)->CHANNEL[(channel)].IMR)


/******************************************************************************
********************* CAN External Functions Declaration **********************
******************************************************************************/
extern void CSP_CANInit(CSP_CAN_T *const can, U32_T mode);
extern void CSP_CANClose(CSP_CAN_T *const can);
extern void CSP_CANConfigInterrupt(CSP_CAN_T *const can, U32_T int_mode, U32_T int_mask, U32_T callback);
extern void CSP_CANChannelConfigInterrupt(CSP_CAN_T *const can, U8_T channel, U32_T int_mask);
extern void CSP_CANEnable(CSP_CAN_T *const can);
extern void CSP_CANDisable(CSP_CAN_T *const can);
extern void CSP_CANReceive(CSP_CAN_T *const can, U8_T channel, U32_T id, U32_T mask, U16_T control);
extern void CSP_CANTransmit(CSP_CAN_T *const can, U8_T channel, U32_T id, U8_T *data, U16_T control);


#endif   /* CSP_CAN_H */
