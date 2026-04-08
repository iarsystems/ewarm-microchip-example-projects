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
* File Name       : csp_spi.h
* Description     : Definitions, Macros and function declarations for
*                   Serial Peripheral Interface module
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
* 003  MOD  19/10/01   Christophe GARDIN  Clean Up
* 004  MOD  15/03/02   Christophe GARDIN  Add Macros
*----------------------------------------------------------------------------*/

#ifndef CSP_SPI_H
#define CSP_SPI_H


/******************************************************************************
************************** SPI Structure Definition ***************************
******************************************************************************/

/* Number of SPI Chips Select                                                */
#define NB_SPI_CS 4u

/* Chip Select                                                               */
#define CS0    0u
#define CS1    1u
#define CS2    2u
#define CS3    3u

/******************************************************************************
* SPI Chip Select Structure
******************************************************************************/
typedef struct
{ 
   CSP_REGISTER_T  CSR;                /* Chips Select Registers             */
} CSP_SPI_CS_T, *CSP_SPI_CS_PTR;


/******************************************************************************
* SPI Structure
******************************************************************************/
typedef struct 
{
   CSP_REGISTER_T  PER;                /* PIO Enable Register                */
   CSP_REGISTER_T  PDR;                /* PIO Disable Register               */
   CSP_REGISTER_T  PSR;                /* PIO Status Register                */
   CSP_REGISTER_T  ReservedA;     
   CSP_REGISTER_T  OER;                /* Output Enable Register             */
   CSP_REGISTER_T  ODR;                /* Output Disable Register            */
   CSP_REGISTER_T  OSR;                /* Output Status Register             */
   CSP_REGISTER_T  ReservedB[5];     
   CSP_REGISTER_T  SODR;               /* Set Output Data Register           */
   CSP_REGISTER_T  CODR;               /* Clear Output Data Register         */
   CSP_REGISTER_T  ODSR;               /* Output Data Status Register        */
   CSP_REGISTER_T  PDSR;               /* Pin Data Status Register           */
   CSP_REGISTER_T  MDER;               /* Multi-Driver Enable Register       */
   CSP_REGISTER_T  MDDR;               /* Multi-Driver Disable Register      */
   CSP_REGISTER_T  MDSR;               /* Multi-Driver Status Register       */
   CSP_REGISTER_T  ReservedC;     
   CSP_REGISTER_T  ECR;                /* Enable Clock Register              */
   CSP_REGISTER_T  DCR;                /* Disable Clock Register             */
   CSP_REGISTER_T  PMSR;               /* Power management Status Register   */
   CSP_REGISTER_T  ReservedD;       
   CSP_REGISTER_T  CR;                 /* Control Register                   */
   CSP_REGISTER_T  MR;                 /* Mode Register                      */
   CSP_REGISTER_T  ReservedE[2];
   CSP_REGISTER_T  SR;                 /* Status Register                    */
   CSP_REGISTER_T  IER;                /* Interrupt Enable Register          */
   CSP_REGISTER_T  IDR;                /* Interrupt Disable Register         */
   CSP_REGISTER_T  IMR;                /* Interrupt Mask Register            */
   CSP_REGISTER_T  RDR;                /* Receive Data Register              */
   CSP_REGISTER_T  TDR;                /* Transmit Data Register             */
   CSP_REGISTER_T  ReservedF[2];
   CSP_SPI_CS_T    CS[NB_SPI_CS];      /* Chips Select Registers             */
} CSP_SPI_T, *CSP_SPI_PTR;


/******************************************************************************
************************** SPI Registers Definition ***************************
******************************************************************************/

/******************************************************************************
* PER, PDR, PSR, OER, ODR, OSR,      :
* SODR, CODR, ODSR, PDSR, MDER, MDDR : SPI PIO Registers, Status Register and
* MDSR, SR, IER, IDR, IMR            : Interrupt Registers
******************************************************************************/
#define SPCK         (0x01ul << 16)    /* SCK   as Open Drain                */
#define MISO         (0x01ul << 17)    /* MISO  as Open Drain                */
#define MOSI         (0x01ul << 18)    /* MOSI  as Open Drain                */
#define NPCS0        (0x01ul << 19)    /* NPCS0 as Open Drain                */
#define NPCS1        (0x01ul << 20)    /* NPCS1 as Open Drain                */
#define NPCS2        (0x01ul << 21)    /* NPCS2 as Open Drain                */
#define NPCS3        (0x01ul << 22)    /* NPCS3 as Open Drain                */

/******************************************************************************
* ECR, DCR, PMSR : SPI Power Management Registers
******************************************************************************/ 
#define PIO          (0x01ul << 0)     /* PIO Clock                          */
#define SPI          (0x01ul << 1)     /* SPI Clock                          */

/******************************************************************************
* CR : SPI Control Register
******************************************************************************/
#define SWRST        (0x01ul << 0)     /* SPI Software Reset                 */
#define SPIEN        (0x01ul << 1)     /* SPI Enable Command                 */
#define SPIDIS       (0x01ul << 2)     /* SPI Disable Command                */

/******************************************************************************
* MR : SPI Mode Register
******************************************************************************/
/* MSTR : Master / Slave Mode                                                */
#define MSTR         (0x01ul << 0)     /* MSTR Mask                          */
#define SLAVE        (0x00ul << 0)     /* Slave Mode                         */
#define MASTER       (0x01ul << 0)     /* Master Mode                        */
                           
/* PS : Peripheral Select                                                    */
#define PS           (0x01ul << 1)     /* PS Mask                            */
#define PSFIX        (0x00ul << 1)     /* Fix Peripheral Select              */
#define PSVAR        (0x01ul << 1)     /* Variable Peripheral Select         */
                                       
#define PCSDEC       (0x01ul << 2)     /* Chip Select Decode                 */
#define DIV32        (0x01ul << 3)     /* Clock Selection                    */
#define LLB          (0x01ul << 7)     /* Local Loop Back                    */

/* PCS : Peripheral Chip Select                                              */
#define PCS          (0x0Ful << 16)    /* PCS Mask                           */
/* PCSDEX = 0 : The 4 Chip select directly connected to a peripheral device  */
#define PCS0         (0x0Eul << 16)    /* Peripheral Chip Select 0           */
#define PCS1         (0x01ul << 16)    /* Peripheral Chip Select 1           */
#define PCS2         (0x03ul << 16)    /* Peripheral Chip Select 2           */
#define PCS3         (0x07ul << 16)    /* Peripheral Chip Select 3           */

/* PCSDEX = 1 : The 4 Chip select lines connected to a decoder               */
#define PCS0_OUTPUT  (0x00ul << 16)    /* Output lines = PCS 0               */
#define PCS1_OUTPUT  (0x01ul << 16)    /* Output lines = PCS 1               */
#define PCS2_OUTPUT  (0x02ul << 16)    /* Output lines = PCS 2               */
#define PCS3_OUTPUT  (0x03ul << 16)    /* Output lines = PCS 3               */
#define PCS4_OUTPUT  (0x04ul << 16)    /* Output lines = PCS 4               */
#define PCS5_OUTPUT  (0x05ul << 16)    /* Output lines = PCS 5               */
#define PCS6_OUTPUT  (0x06ul << 16)    /* Output lines = PCS 6               */
#define PCS7_OUTPUT  (0x07ul << 16)    /* Output lines = PCS 7               */
#define PCS8_OUTPUT  (0x08ul << 16)    /* Output lines = PCS 8               */
#define PCS9_OUTPUT  (0x09ul << 16)    /* Output lines = PCS 9               */
#define PCS10_OUTPUT (0x0Aul << 16)    /* Output lines = PCS 10              */
#define PCS11_OUTPUT (0x0Bul << 16)    /* Output lines = PCS 11              */
#define PCS12_OUTPUT (0x0Cul << 16)    /* Output lines = PCS 12              */
#define PCS13_OUTPUT (0x0Dul << 16)    /* Output lines = PCS 13              */
#define PCS14_OUTPUT (0x0Eul << 16)    /* Output lines = PCS 14              */
#define PCS15_OUTPUT (0x0Ful << 16)    /* Output lines = PCS 15              */

/* DLYBCS : Delay Between Chip Select                                        */
#define DLYBCS_MASK  (0xFFul << 24)             /* Mask                      */
#define DLYBCS(val)  (((val) & 0xFFul) << 24)   /* Writing Macro             */

/******************************************************************************
* SR, IER, IDR, IMR : SPI Status and Interrupt Registers
******************************************************************************/
#define RDRF         (0x01ul << 0)     /* Receive Data Register Full         */
#define TDRE         (0x01ul << 1)     /* Transmit Data Register Empty       */
#define MODF         (0x01ul << 2)     /* Mode Fault Error                   */
#define SPIOVRE      (0x01ul << 3)     /* Overrun Error                      */
#define REND         (0x01ul << 4)     /* Reception End                      */
#define SPI_TEND     (0x01ul << 5)     /* Transfer End                       */

/* SR Register Only                                                          */
#define SPIENS       (0x01ul << 8)     /* SPI Enable                         */
 
/******************************************************************************
* RDR : SPI Received Data Register 
******************************************************************************/
#define RD           (0xFFFFul << 0)   /* Receive Data                       */
/* PCS : Peripheral Chip Select - Same as MR Register                        */

/******************************************************************************
* TDR : SPI Transmit Data Register
******************************************************************************/
#define TD           (0xFFFFul << 0)   /* Transmit Data                      */
/* PCS : Peripheral Chip Select - Same as MR Register                        */

/******************************************************************************
* CSR : SPI Chip Select Register 
******************************************************************************/
#define CPOL         (0x01ul << 0)     /* Clock Polarity                     */
#define NCPHA        (0x01ul << 1)     /* Clock Phase                        */

/* BITS : Number of Bits Per Transfer                                        */
#define BITS         (0x0Ful << 4)     /* BITS Mask                          */
#define BITS_8       (0x00ul << 4)     /* 8  Bits Per Transfer               */
#define BITS_9       (0x01ul << 4)     /* 9  Bits Per Transfer               */
#define BITS_10      (0x02ul << 4)     /* 10 Bits Per Transfer               */
#define BITS_11      (0x03ul << 4)     /* 11 Bits Per Transfer               */
#define BITS_12      (0x04ul << 4)     /* 12 Bits Per Transfer               */
#define BITS_13      (0x05ul << 4)     /* 13 Bits Per Transfer               */
#define BITS_14      (0x06ul << 4)     /* 14 Bits Per Transfer               */
#define BITS_15      (0x07ul << 4)     /* 15 Bits Per Transfer               */
#define BITS_16      (0x08ul << 4)     /* 16 Bits Per Transfer               */

/* SCBR : Serial Clock Baud Rate                                             */
#define SCBR_MASK    (0xFFul << 8)              /* Mask                      */    
#define SCBR(val)    (((val) & 0xFFul) << 8)    /* Writing Macro             */    

/* DLYBS : Delay Before SCK                                                  */
#define DLYBS_MASK   (0xFFul << 16)             /* Mask                      */
#define DLYBS(val)   (((val) & 0xFFul) << 16)   /* Writing Macro             */

/* DLYBCT : Delay Between Consecutive Transfers                              */
#define DLYBCT_MASK  (0xFFul << 24)             /* Mask                      */
#define DLYBCT(val)  (((val) & 0xFFul) << 24)   /* Writing Macro             */


/******************************************************************************
*************************** SPI Macros Definition *****************************
******************************************************************************/
/* PER, PDR, PSR : SPI PIO Registers                                         */
#define CSP_SPI_SET_PER(spi, val)      ((spi)->PER = (val))    /* Enable     */
#define CSP_SPI_SET_PDR(spi, val)      ((spi)->PDR = (val))    /* Disable    */
#define CSP_SPI_GET_PSR(spi)           ((spi)->PSR)            /* Status     */

/* OER, ODR, OSR : SPI Output Registers                                      */
#define CSP_SPI_SET_OER(spi, val)      ((spi)->OER = (val))    /* Enable     */
#define CSP_SPI_SET_ODR(spi, val)      ((spi)->ODR = (val))    /* Disable    */
#define CSP_SPI_GET_OSR(spi)           ((spi)->OSR)            /* Status     */

/* SODR, CODR, ODSR, PDSR : SPI Output Data Registers                        */
#define CSP_SPI_SET_SODR(spi, val)     ((spi)->SODR = (val))   /* Set        */
#define CSP_SPI_SET_CODR(spi, val)     ((spi)->CODR = (val))   /* Clear      */
#define CSP_SPI_GET_ODSR(spi)          ((spi)->ODSR)           /* Status     */
#define CSP_SPI_GET_PDSR(spi)          ((spi)->PDSR)           /* Pin Status */

/* MDER, MDDR, MDSR : SPI Multi-Driver Registers                             */
#define CSP_SPI_SET_MDER(spi, val)     ((spi)->MDER = (val))   /* Enable     */
#define CSP_SPI_SET_MDDR(spi, val)     ((spi)->MDDR = (val))   /* Disable    */
#define CSP_SPI_GET_MDSR(spi)          ((spi)->MDSR)           /* Status     */

/* ECR, DCR, PMSR : SPI Power Management Registers                           */
#define CSP_SPI_SET_ECR(spi, val)      ((spi)->ECR = (val))    /* Enable     */
#define CSP_SPI_SET_DCR(spi, val)      ((spi)->DCR = (val))    /* Disable    */
#define CSP_SPI_GET_PMSR(spi)          ((spi)->PMSR)           /* Status     */

/* CR : SPI Control Register                                                 */
#define CSP_SPI_SET_CR(spi, val)       ((spi)->CR = (val))

/* MR : SPI Mode Register                                                    */
#define CSP_SPI_GET_MR(spi)            ((spi)->MR)
#define CSP_SPI_SET_MR(spi, mode)      ((spi)->MR = (mode))

/* SR : SPI Status Register                                                  */
#define CSP_SPI_GET_SR(spi)            ((spi)->SR)

/* IER, IDR, IMR : SPI Interrupt Registers                                   */
#define CSP_SPI_SET_IER(spi, val)      ((spi)->IER = (val))    /* Enable     */
#define CSP_SPI_SET_IDR(spi, val)      ((spi)->IDR = (val))    /* Disable    */
#define CSP_SPI_GET_IMR(spi)           ((spi)->IMR)            /* Status     */

/* RDR : SPI Receive Data Register                                           */
#define CSP_SPI_GET_RDR(spi)           ((spi)->RDR)

/* TDR : SPI Transmit Data Register                                          */
#define CSP_SPI_SET_TDR(spi, val)      ((spi)->TDR = (val))

/* CSR : SPI Chip Select Register                                            */
#define CSP_SPI_CS_GET_CSR(spi, cs)       ((spi)->CS[(cs)].CSR)
#define CSP_SPI_CS_SET_CSR(spi, cs, val)  ((spi)->CS[(cs)].CSR = (val))


/******************************************************************************
********************* SPI External Functions Declaration **********************
******************************************************************************/
extern void CSP_SPIInit(CSP_SPI_T *const spi, U32_T mode);
extern void CSP_SPIClose(CSP_SPI_T *const spi);
extern void CSP_SPIConfigureCS(CSP_SPI_T *const spi, U8_T cs, U32_T mode);
extern void CSP_SPIConfigInterrupt(CSP_SPI_T *const spi, U32_T int_mode, U32_T int_mask, U32_T callback);
extern void CSP_SPIEnable(CSP_SPI_T *const spi);
extern void CSP_SPIDisable(CSP_SPI_T *const spi);
extern void CSP_SPIReceive(CSP_SPI_T *const spi, U8_T cs, U8_T *cmd_tx, U8_T *data, U16_T length);
extern void CSP_SPITransmit(CSP_SPI_T *const spi, U8_T cs, U8_T *data, U16_T length);
extern void CSP_SPIPioInit(CSP_SPI_T *const spi, U32_T pio_mask, U32_T output_pio);
extern U32_T CSP_SPIPioGetStatus(CSP_SPI_T *const spi);
extern void CSP_SPIPioSet(CSP_SPI_T *const spi, U32_T pio_mask);
extern void CSP_SPIPioClear(CSP_SPI_T *const spi, U32_T pio_mask);


#endif   /* CSP_SPI_H */
