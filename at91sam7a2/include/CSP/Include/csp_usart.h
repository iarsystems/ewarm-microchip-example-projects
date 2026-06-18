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
* File Name       : csp_usart.h
* Description     : Definitions, Macros and function declarations for
*                   Universal Synchronous/Asynchronous Receiver/Transmitter module
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
* 003  MOD  22/10/01   Christophe GARDIN  Clean Up
* 004  MOD  18/07/02   Christophe GARDIN  Add LIN Registers
*----------------------------------------------------------------------------*/

#ifndef CSP_USART_H
#define CSP_USART_H


/******************************************************************************
************************ USART Structure Definition ***************************
******************************************************************************/
typedef struct
{
   CSP_REGISTER_T  PER;                   /* PIO Enable Register             */
   CSP_REGISTER_T  PDR;                   /* PIO Disable Register            */
   CSP_REGISTER_T  PSR;                   /* PIO Status Register             */
   CSP_REGISTER_T  ReservedA;
   CSP_REGISTER_T  OER;                   /* Output Enable Register          */
   CSP_REGISTER_T  ODR;                   /* Output Disable Register         */
   CSP_REGISTER_T  OSR;                   /* Output Status Register          */
   CSP_REGISTER_T  ReservedB[5];
   CSP_REGISTER_T  SODR;                  /* Set Output Data Register        */
   CSP_REGISTER_T  CODR;                  /* Clear Output Data Register      */
   CSP_REGISTER_T  ODSR;                  /* Output Data Status Register     */ 
   CSP_REGISTER_T  PDSR;                  /* Pin Data Status Register        */
   CSP_REGISTER_T  MDER;                  /* Multi-Driver Enable Register    */
   CSP_REGISTER_T  MDDR;                  /* Multi-Driver Disable Register   */
   CSP_REGISTER_T  MDSR;                  /* Multi-Driver Status Register    */
   CSP_REGISTER_T  ReservedC;
   CSP_REGISTER_T  ECR;                   /* Enable Clock Register           */
   CSP_REGISTER_T  DCR;                   /* Disable Clock Register          */
   CSP_REGISTER_T  PMSR;                  /* Power Management Status Register*/
   CSP_REGISTER_T  ReservedD;    
   CSP_REGISTER_T  CR;                    /* Control Register                */
   CSP_REGISTER_T  MR;                    /* Mode Register                   */
   CSP_REGISTER_T  ReservedE;
   CSP_REGISTER_T  CSR;                   /* Clear Status Register           */
   CSP_REGISTER_T  SR;                    /* Status Register                 */
   CSP_REGISTER_T  IER;                   /* Interrupt Enable Register       */
   CSP_REGISTER_T  IDR;                   /* Interrupt Disable Register      */
   CSP_REGISTER_T  IMR;                   /* Interrupt Mask Register         */
   CSP_REGISTER_T  RHR;                   /* Receiver Holding Register       */
   CSP_REGISTER_T  THR;                   /* Transmit Holding Register       */
   CSP_REGISTER_T  BRGR;                  /* Baud Rate Generator Register    */
   CSP_REGISTER_T  RTOR;                  /* Receiver Time-out Register      */
   CSP_REGISTER_T  TTGR;                  /* Transmitter Time-guard Register */
   CSP_REGISTER_T  LIR;                   /* LIN Identifier Register         */
   CSP_REGISTER_T  DFWR0;                 /* Data Field Write 0 Register     */
   CSP_REGISTER_T  DFWR1;                 /* Data Field Write 1 Register     */
   CSP_REGISTER_T  DFRR0;                 /* Data Field Read 0 Register      */
   CSP_REGISTER_T  DFRR1;                 /* Data Field Read 1 Register      */
   CSP_REGISTER_T  SBLR;                  /* Sync Break Length Register      */
} CSP_USART_T, *CSP_USART_PTR;


/******************************************************************************
************************* USART Registers Definition **************************
******************************************************************************/

/******************************************************************************
* PER, PDR, PSR, OER, ODR, OSR,      :
* SODR, CODR, ODSR, PDSR, MDER, MDDR : USART PIO Registers, Status Registers and
* MDSR, SR, IER, IDR, IMR            : Interrupt Registers
******************************************************************************/
#define SCK             (0x01ul << 16)    /* SCK                             */
#define TXD             (0x01ul << 17)    /* TXD                             */
#define RXD             (0x01ul << 18)    /* RXD                             */

/******************************************************************************
* ECR, DCR, PMSR : USART Power Management Registers
******************************************************************************/
#define PIO             (0x01ul << 0)     /* PIO Clock                       */
#define USART           (0x01ul << 1)     /* USART Clock                     */

/******************************************************************************
* CR : USART Control Register
******************************************************************************/
#define SWRST           (0x01ul << 0)     /* Software Reset                  */
#define RSTRX           (0x01ul << 2)     /* Reset Receiver                  */
#define RSTTX           (0x01ul << 3)     /* Reset Transmitter               */
#define RXEN            (0x01ul << 4)     /* Receiver Enable                 */
#define RXDIS           (0x01ul << 5)     /* Receiver Disable                */
#define TXEN            (0x01ul << 6)     /* Transmitter Enable              */
#define TXDIS           (0x01ul << 7)     /* Transmitter Disable             */
#define RSTSTA          (0x01ul << 8)     /* Reset Status Bits               */
#define STTBRK          (0x01ul << 9)     /* Start Break                     */
#define STPBRK          (0x01ul << 10)    /* Stop Break                      */
#define STTTO           (0x01ul << 11)    /* Start Time-out                  */
#define SENDA           (0x01ul << 12)    /* Send Address                    */
#define STHEADER        (0x01ul << 16)    /* Start Header                    */
#define STRESP          (0x01ul << 17)    /* Start Response                  */

/******************************************************************************
* MR : USART Mode Register
******************************************************************************/
#define LIN             (0x01ul << 0)     /* Local Interconnect Network mode */

/* SENDTIME : Send Time                                                      */
#define SENDTIME        (0x03ul << 2)     /* SENDTIME Mask                   */
#define SENDTIME_0      (0x00ul << 2)     /* Number of Time = 0              */
#define SENDTIME_1      (0x01ul << 2)     /* Number of Time = 1              */
#define SENDTIME_2      (0x02ul << 2)     /* Number of Time = 2              */
#define SENDTIME_3      (0x03ul << 2)     /* Number of Time = 3              */

/* USCLKS : Clock Selection                                                  */
#define USCLKS          (0x03ul << 4)     /* USCLKS Mask                     */
#define USCLKS_MCKI     (0x00ul << 4)     /* Core Clock (MCKI)               */
#define USCLKS_MCKI_8   (0x01ul << 4)     /* Core Clock / 8 (MCKI/8)         */
#define USCLKS_SCK      (0x02ul << 4)     /* External Clock (SCK)            */

/* CHRL : Character Length                                                   */
#define CHRL            (0x03ul << 6)     /* CHRL Mask                       */
#define CHRL_5          (0x00ul << 6)     /* Five bits length                */
#define CHRL_6          (0x01ul << 6)     /* Six bits length                 */
#define CHRL_7          (0x02ul << 6)     /* Seven bits length               */
#define CHRL_8          (0x03ul << 6)     /* Height bits length              */

/* SYNC : Synchronous Mode Select                                            */
#define SYNC            (0x01ul << 8)     /* Synchronous mode                */
#define ASYNC           (0x00ul << 8)     /* Asynchronous mode               */

/* PAR : Parity Type                                                         */
#define PAR             (0x07ul << 9)     /* PAR Mask                        */
#define PAR_EVEN        (0x00ul << 9)     /* Even parity                     */
#define PAR_ODD         (0x01ul << 9)     /* Odd parity                      */
#define PAR_SPACE       (0x02ul << 9)     /* Space parity (forced to 0)      */
#define PAR_MARK        (0x03ul << 9)     /* Mark parity (forced to 1)       */
#define PAR_NO          (0x04ul << 9)     /* No parity                       */
#define PAR_MULTIDROP   (0x06ul << 9)     /* Multi Drop parity               */
 
/* NBSTOP : Number of Stop Bits                                              */
#define NBSTOP          (0x03ul << 12)    /* NBSTOP Mask                     */
#define NBSTOP_1        (0x00ul << 12)    /* 1 Stop bit                      */
#define NBSTOP_15       (0x01ul << 12)    /* 1.5 Stop bit                    */
#define NBSTOP_2        (0x02ul << 12)    /* 2 Stop bit                      */

/* CHMODE : Channel Mode                                                     */
#define CHMODE          (0x03ul << 14)    /* CHMODE Mask                     */
#define CHMODE_NORMAL   (0x00ul << 14)    /* Normal channel                  */
#define CHMODE_AUTO     (0x01ul << 14)    /* Automatic echo channel          */
#define CHMODE_LOCAL    (0x02ul << 14)    /* Local loop back channel         */
#define CHMODE_REMOTE   (0x03ul << 14)    /* Remote loop back channel        */

#define SMCARDPT        (0x01ul << 16)    /* Smart Card Protocol valid / not valid */

/* MODE9 : 9-Bit Character Length                                            */
#define MODE8           (0x00ul << 17)    /* 8-Bits Mode                     */
#define MODE9           (0x01ul << 17)    /* 9-Bits Mode                     */

/* CLKO : Clock Output Select                                                */
#define CLKO            (0x01ul << 18)    /* Clock Output                    */
#define CLKI            (0x00ul << 18)    /* Clock Input                     */

/******************************************************************************
* CSR, SR, IER, IDR, IMR : USART Status and Interrupt Registers
******************************************************************************/
#define ENDHEADER       (0x01ul << 24)    /* End of Header occurred on a LIN Frame        */
#define ENDMESS         (0x01ul << 25)    /* End of Message occurred on a LIN Frame       */
#define NOTRESP         (0x01ul << 26)    /* Not Responding detected on a LIN Frame       */
#define BITERROR        (0x01ul << 27)    /* Bit Error detectedon a LIN Frame             */
#define IPERROR         (0x01ul << 28)    /* Identity Parity Error detectedon a LIN Frame */
#define CHECKSUM        (0x01ul << 29)    /* Checksum error detected on a LIN Frame       */
#define WAKEUP          (0x01ul << 30)    /* Wake up detected                */

/* SR, IER, IDR, IMR Registers Only                                          */
#define RXRDY           (0x01ul << 0)     /* Receiver ready                  */
#define TXRDY           (0x01ul << 1)     /* Transmitter ready               */
#define RXBRK           (0x01ul << 2)     /* Receiver break                  */
#define ENDRX           (0x01ul << 3)     /* End of receiver PDC transfer    */
#define ENDTX           (0x01ul << 4)     /* End of transmitter PDC transfer */
#define USOVRE          (0x01ul << 5)     /* Overrun Error                   */
#define FRAME           (0x01ul << 6)     /* Framing Error                   */
#define PARE            (0x01ul << 7)     /* Parity Error                    */
#define TIMEOUT         (0x01ul << 8)     /* Receiver time out               */
#define TXEMPTY         (0x01ul << 9)     /* Transmitter empty               */
#define IDLE            (0x01ul << 10)    /* IDLE (J1708 Protocol)           */

/* SR Register Only                                                          */
#define IDLEFLAG        (0x01ul << 11)    /* IDLE Flag (J1708 Protocol)      */

/******************************************************************************
* RHR : USART Receiver Holding Register
******************************************************************************/
#define RXCHR           (0x1FFul << 0)    /* Received Character Mask         */

/******************************************************************************
* THR : USART Transmit Holding Register
******************************************************************************/
#define TXCHR           (0x1FFul << 0)    /* Character to be transmitted Mask*/

/******************************************************************************
* BRGR : USART Baud Rate Generator Register
******************************************************************************/
/* CD : Clock Divisor                                                        */
#define CD              (0xFFul << 0)     /* CD Mask                         */
#define CD_DISABLE      (0x00ul << 0)     /* Disable Clock                   */
#define CD_BYPASS       (0x01ul << 0)     /* Clock Divisor Bypass            */

/******************************************************************************
* RTOR : Receiver Time-out Register
******************************************************************************/
#define TO              (0xFFul << 0)     /* Time-out value Mask             */
#define TO_DISABLE      (0x00ul << 0)     /* Disable Receiver Time-out       */

/******************************************************************************
* TTGR : Transmit Time-guard Register
******************************************************************************/
#define TG              (0xFFul << 0)     /* Time-guard value Mask           */
#define TG_DISABLE      (0x00ul << 0)     /* Disable Transmit Time-guard     */

/******************************************************************************
* LIR : LIN Identifier Register
******************************************************************************/
#define IDENTIFIER      (0x3Ful << 0)     /* LIN's IDENTIFER Mask            */

/******************************************************************************
* SBLR : Sync Break Length Register
******************************************************************************/
#define SYNC_BRK        (0x1Ful << 0)     /* Sync Break Length Mask          */


/******************************************************************************
************************** USART Macros Definition ****************************
******************************************************************************/
/* PER, PDR, PSR : USART PIO Registers                                       */
#define CSP_USART_SET_PER(usart, val)  ((usart)->PER = (val))  /* Enable     */
#define CSP_USART_SET_PDR(usart, val)  ((usart)->PDR = (val))  /* Disable    */
#define CSP_USART_GET_PSR(usart)       ((usart)->PSR)          /* Status     */

/* OER, ODR, OSR : USART Output Registers                                    */
#define CSP_USART_SET_OER(usart, val)  ((usart)->OER = (val))  /* Enable     */
#define CSP_USART_SET_ODR(usart, val)  ((usart)->ODR = (val))  /* Disable    */
#define CSP_USART_GET_OSR(usart)       ((usart)->OSR)          /* Status     */

/* SODR, CODR, ODSR, PDSR : USART Output Data Registers                      */
#define CSP_USART_SET_SODR(usart, val) ((usart)->SODR = (val)) /* Set        */
#define CSP_USART_SET_CODR(usart, val) ((usart)->CODR = (val)) /* Clear      */
#define CSP_USART_GET_ODSR(usart)      ((usart)->ODSR)         /* Status     */
#define CSP_USART_GET_PDSR(usart)      ((usart)->PDSR)         /* Pin Status */

/* MDER, MDDR, MDSR : USART Multi-Driver Registers                           */
#define CSP_USART_SET_MDER(usart, val) ((usart)->MDER = (val)) /* Enable     */
#define CSP_USART_SET_MDDR(usart, val) ((usart)->MDDR = (val)) /* Disable    */
#define CSP_USART_GET_MDSR(usart)      ((usart)->MDSR)         /* Status     */

/* ECR, DCR, PMSR : USART Power Management Registers                         */
#define CSP_USART_SET_ECR(usart, val)  ((usart)->ECR = (val))  /* Enable     */
#define CSP_USART_SET_DCR(usart, val)  ((usart)->DCR = (val))  /* Disable    */
#define CSP_USART_GET_PMSR(usart)      ((usart)->PMSR)         /* Status     */

/* CR : USART Control Register                                               */
#define CSP_USART_SET_CR(usart, val)   ((usart)->CR = (val))

/* MR : USART Mode Register                                                  */
#define CSP_USART_SET_MR(usart, mode)  ((usart)->MR = (mode))
#define CSP_USART_GET_MR(usart)        ((usart)->MR)

/* SR : USART Status Register                                                */
#define CSP_USART_GET_SR(usart)        ((usart)->SR)

/* IER, IDR, IMR : USART Interrupt Registers                                 */
#define CSP_USART_SET_IER(usart, val)  ((usart)->IER = (val))  /* Enable     */
#define CSP_USART_SET_IDR(usart, val)  ((usart)->IDR = (val))  /* Disable    */
#define CSP_USART_GET_IMR(usart)       ((usart)->IMR)          /* Mask       */

/* RHR : USART Receive Holding Register                                      */
#define CSP_USART_GET_RHR(usart)       ((usart)->RHR)

/* THR : USART Transmit Holding Register                                     */
#define CSP_USART_SET_THR(usart, val)  ((usart)->THR = (val))

/* BRGR : USART Baud Rate Generator Register                                 */
#define CSP_USART_GET_BRGR(usart)      ((usart)->BRGR)
#define CSP_USART_SET_BRGR(usart, val) ((usart)->BRGR = (val))

/* RTOR : USART Receiver Time-out Register                                   */
#define CSP_USART_GET_RTOR(usart)      ((usart)->RTOR)
#define CSP_USART_SET_RTOR(usart, val) ((usart)->RTOR = (val))

/* TTGR : USART Transmitter Time-guard Register                              */
#define CSP_USART_GET_TTGR(usart)      ((usart)->TTGR)
#define CSP_USART_SET_TTGR(usart, val) ((usart)->TTGR = (val))

/* LIR : USART LIN Identifier Register                                       */
#define CSP_USART_GET_LIR(usart)       ((usart)->LIR)
#define CSP_USART_SET_LIR(usart, val)  ((usart)->LIR = (val))

/* DFWR0 : USART Data Field Write 0 Register                                 */
#define CSP_USART_GET_DFWR0(usart)        ((usart)->DFWR0)
#define CSP_USART_SET_DFWR0(usart, val)   ((usart)->DFWR0 = (val))

/* DFWR1 : USART Data Field Write 1 Register                                 */
#define CSP_USART_GET_DFWR1(usart)        ((usart)->DFWR1)
#define CSP_USART_SET_DFWR1(usart, val)   ((usart)->DFWR1 = (val))

/* DFRR0 : USART Data Field Read 0 Register                                  */
#define CSP_USART_GET_DFRR0(usart)        ((usart)->DFRR0)

/* DFRR1 : USART Data Field Read 1 Register                                  */
#define CSP_USART_GET_DFRR1(usart)        ((usart)->DFRR1)

/* SBLR : USART Sync Break Length Register                                   */
#define CSP_USART_GET_SBLR(usart)         ((usart)->SBLR)
#define CSP_USART_SET_SBLR(usart, val)    ((usart)->SBLR = (val))


/******************************************************************************
******************** USART External Functions Declaration *********************
******************************************************************************/
extern void CSP_USARTInit(CSP_USART_T *const usart, U32_T mode, U16_T baudrate, U8_T time_out, U8_T time_guard);
extern void CSP_USARTInitLin(CSP_USART_T *const usart, U8_T time_guard);
extern void CSP_USARTClose(CSP_USART_T *const usart);
extern void CSP_USARTConfigInterrupt(CSP_USART_T *const usart, U32_T int_mode, U32_T int_mask, U32_T callback);
extern void CSP_USARTEnable(CSP_USART_T *const usart, U32_T enable_mask);
extern void CSP_USARTDisable(CSP_USART_T *const usart, U32_T disable_mask);
extern void CSP_USARTReceive(CSP_USART_T *const usart, U8_T *data, U16_T length);
extern void CSP_USARTTransmit(CSP_USART_T *const usart, U8_T *data, U16_T length);
extern void CSP_USARTTransmitLinHeaderFrame(CSP_USART_T *const usart, U8_T sblr, U8_T identifier);
extern void CSP_USARTTransmitLinResponseFrame(CSP_USART_T *const usart, U8_T *data, U8_T length);
extern void CSP_USARTPioInit(CSP_USART_T *const usart, U32_T pio_mask, U32_T output_pio);
extern U32_T CSP_USARTPioGetStatus(CSP_USART_T *const usart);
extern void CSP_USARTPioClear(CSP_USART_T *const usart, U32_T pio_mask);
extern void CSP_USARTPioSet(CSP_USART_T *const usart, U32_T pio_mask);


#endif   /* CSP_USART_H */

