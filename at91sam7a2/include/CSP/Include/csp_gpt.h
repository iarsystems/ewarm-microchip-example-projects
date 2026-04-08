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
* File Name       : csp_gpt.h
* Description     : Definitions, Macros and function declarations for
*                   General Purpose Timer module
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

#ifndef CSP_GPT_H
#define CSP_GPT_H


/******************************************************************************
************************** GPT Structure Definition ***************************
******************************************************************************/

/* Physical Timer Definition                                                 */
#define GPT_ONE_CHANNEL    1u
#define GPT_THREE_CHANNEL  3u

/******************************************************************************
* GPT Channel Structure
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
   CSP_REGISTER_T  PMSR;               /* Power Management Status Register   */
   CSP_REGISTER_T  ReservedD;
   CSP_REGISTER_T  CR;                 /* Control Register                   */
   CSP_REGISTER_T  MR;                 /* Mode Register                      */
   CSP_REGISTER_T  ReservedE[2];
   CSP_REGISTER_T  SR;                 /* Status Register                    */
   CSP_REGISTER_T  IER;                /* Interrupt Enable Register          */
   CSP_REGISTER_T  IDR;                /* Interrupt Disable Register         */
   CSP_REGISTER_T  IMR;                /* Interrupt Mask Register            */
   CSP_REGISTER_T  CV;                 /* Counter value Register             */
   CSP_REGISTER_T  RA;                 /* Register A                         */
   CSP_REGISTER_T  RB;                 /* Register B                         */
   CSP_REGISTER_T  RC;                 /* Register C                         */
   CSP_REGISTER_T  ReservedF[28];
} CSP_GPT_T, *CSP_GPT_PTR;

/******************************************************************************
* GPT 1 Channel Structure
******************************************************************************/
typedef struct
{
   CSP_GPT_T       CHANNEL[GPT_ONE_CHANNEL];    /* GPT Channel               */
   CSP_REGISTER_T  ReservedA[192];
   CSP_REGISTER_T  TSTC;                        /* Test Control Register     */
   CSP_REGISTER_T  TSTM;                        /* Test Mode Register        */
} CSP_GPT1C_T, *CSP_GPT1C_PTR;


/******************************************************************************
* GPT 3 Channels Structure
******************************************************************************/
typedef struct
{
   CSP_GPT_T       CHANNEL[GPT_THREE_CHANNEL];  /* GPT Channel               */
   CSP_REGISTER_T  BCR;                         /* Block Control Register    */
   CSP_REGISTER_T  BMR;                         /* Clock Mode Register       */
   CSP_REGISTER_T  ReservedA[62];
   CSP_REGISTER_T  TSTC;                        /* Test Control Register     */
   CSP_REGISTER_T  TSTM;                        /* Test Mode Register        */
} CSP_GPT3C_T, *CSP_GPT3C_PTR;


/******************************************************************************
************************** GPT Registers Definition ***************************
******************************************************************************/

/******************************************************************************
* PER, PDR, PSR, OER, ODR, OSR,      :
* SODR, CODR, ODSR, PDSR, MDER, MDDR : GPT PIO Registers, Status Register and
* MDSR, SR, IER, IDR, IMR            : Interrupt Registers
******************************************************************************/
#define TIOB            (0x01ul << 16) /* TIOB                               */
#define TIOA            (0x01ul << 17) /* TIOA                               */
#define TCLK            (0x01ul << 18) /* TCLK                               */

/******************************************************************************
* ECR, DCR, PMSR : GPT Power Management Registers
******************************************************************************/
#define PIO             (0x01ul << 0)  /* PIO Clock                          */
#define TC              (0x01ul << 1)  /* GPT Clock                          */

/******************************************************************************
* CR : GPT Control Register
******************************************************************************/
#define SWRST           (0x01ul << 0)  /* GPT Software Reset                 */
#define CLKEN           (0x01ul << 1)  /* Counter Clock Enable               */
#define CLKDIS          (0x01ul << 2)  /* Counter Clock Disable              */
#define SWTRG           (0x01ul << 3)  /* Software Trigger                   */

/******************************************************************************
* MR : GPT Mode Register (Capture mode)
******************************************************************************/
/* CLKS : Clock Source                                                       */
#define CLKS            (0x07ul << 0)  /* CLKS Mask                          */
#define CLKS_MCK2       (0x00ul << 0)  /* MCK/2                              */
#define CLKS_MCK8       (0x01ul << 0)  /* MCK/8                              */
#define CLKS_MCK32      (0x02ul << 0)  /* MCK/32                             */
#define CLKS_MCK128     (0x03ul << 0)  /* MCK/128                            */
#define CLKS_MCK1024    (0x04ul << 0)  /* MCK/1024                           */
#define CLKS_XC0        (0x05ul << 0)  /* External XC0                       */
#define CLKS_XC1        (0x06ul << 0)  /* External XC1                       */
#define CLKS_XC2        (0x07ul << 0)  /* External XC2                       */

/* CLKI : Clock Inverter                                                     */
#define CLKINV          (0x01ul << 3)

/* BURST : Burst Signal                                                      */
#define BURST           (0x03ul << 4)  /* BURST Mask                         */
#define BURST_NONE      (0x00ul << 4)  /* No signal selected for burst       */
#define BURST_XC0       (0x01ul << 4)  /* XC0 selected for burst             */
#define BURST_XC1       (0x02ul << 4)  /* XC1 selected for burst             */
#define BURST_XC2       (0x03ul << 4)  /* XC2 selected for burst             */

/* LDBSTOP : Counter clock stopped with RB Loading                           */
#define LDBSTOP         (0x01ul << 6)

/* LDBDIS : Counter clock disable with RB Loading                            */
#define LDBDIS          (0x01ul << 7)

/* ETRGEDG : External Trigger Edge                                           */
#define ETRGEDG         (0x03ul << 8)  /* ETRGEDG Mask                       */
#define ETRGEDG_NONE    (0x00ul << 8)  /* No external trigger                */
#define ETRGEDG_RISING  (0x01ul << 8)  /* Rising edge trigger                */
#define ETRGEDG_FALLING (0x02ul << 8)  /* Falling edge trigger               */
#define ETRGEDG_EACH    (0x03ul << 8)  /* Each edge trigger                  */

/* ABETRG : TIOA or TIOB External Trigger                                    */
#define ABETRG          (0x01ul << 10) /* ABETRG Mask                        */
#define ABETRG_TIOA     (0x01ul << 10) /* TIOA selected for external trigger */
#define ABETRG_TIOB     (0x00ul << 10) /* TIOB selected for external trigger */
                               
/* CPCTRG : RC Compare Trigger Enable                                        */
#define CPCTRG          (0x01ul << 14)

/* WAVE = CAPT_ENA : Capture Mode is enabled                                 */
#define WAVE            (0x01ul << 15) /* WAVE Mask                          */
#define CAPT_ENA        (0x00ul << 15) /* Capture Mode                       */
#define WAVE_ENA        (0x01ul << 15) /* Waveform Mode                      */

/* LDRA : Load RA                                                            */
#define LDRA            (0x03ul << 16) /* LDRA Mask                          */
#define LDRA_NONE       (0x00ul << 16) /* No external trigger selected       */
#define LDRA_RISING     (0x01ul << 16) /* Rising edge on TIOA                */
#define LDRA_FALLING    (0x02ul << 16) /* Falling edge on TIOA               */
#define LDRA_EACH       (0x03ul << 16) /* Each edge on TIOA                  */

/* LDRB : Load RB                                                            */
#define LDRB            (0x03ul << 18) /* LRRB Mask                          */
#define LDRB_NONE       (0x00ul << 18) /* No external trigger selected       */
#define LDRB_RISING     (0x01ul << 18) /* Rising edge on TIOA                */
#define LDRB_FALLING    (0x02ul << 18) /* Falling edge on TIOA               */
#define LDRB_EACH       (0x03ul << 18) /* Each edge on TIOA                  */

/******************************************************************************
* MR : Mode Register (Waveform mode)
******************************************************************************/
/* CLKS  : Clock source   - Same as Capture Mode                             */
/* CLKI  : Clock inverter - Same as Capture Mode                             */
/* BURST : Burst signal   - Same as Capture Mode                             */

/* CPCSTOP : Compare RC Stops the counter                                    */
#define CPCSTOP         (0x01ul << 6)  /* CPCSTOP Mask                       */
#define CPCSTOP_DIS     (0x00ul << 6)  /* Counter is not stopped when equal condition on RC    */
#define CPCSTOP_ENA     (0x01ul << 6)  /* Counter is stopped when equal condition on RC        */

/* CPCDIS : Compare RC Disables Clock                                        */
#define CPCSDIS         (0x01ul << 7)  /* CPCSDIS Mask                       */
#define CPCSDIS_DIS     (0x00ul << 7)  /* Counter clock is disables when equal condition on RC */
#define CPCSDIS_ENA     (0x01ul << 7)  /* Counter clock disables when equal condition on RC    */

/* EEVTEDG : External Event Edge                                             */
#define EEVTEDG         (0x03ul << 8)  /* EEVTEDG Mask                       */
#define EEVTEDG_NONE    (0x00ul << 8)  /* No external event edge             */
#define EEVTEDG_RISING  (0x01ul << 8)  /* Rising event edge                  */
#define EEVTEDG_FALLING (0x02ul << 8)  /* Falling event edge                 */ 
#define EEVTEDG_EACH    (0x03ul << 8)  /* Each event edge                    */

/* EEVT : External Event                                                     */
#define EEVT            (0x03ul << 10) /* EEVT Mask                          */
#define EEVT_TIOB       (0x00ul << 10) /* TIOB External Trigger              */
#define EEVT_XC0        (0x01ul << 10) /* XC0 External Trigger               */
#define EEVT_XC1        (0x02ul << 10) /* XC1 External Trigger               */
#define EEVT_XC2        (0x03ul << 10) /* XC2 External Trigger               */

/* ENETRG : Enable External Trigger                                          */
#define ENETRG          (0x01ul << 12)

/* CPCTRG : RC compare trigger enable - Same as Capture Mode                 */
/* WAVE = WAVE_ENA : Waveform mode is enable                                 */

/* ACPA : RA Compare Effect on TIOA                                          */
#define ACPA            (0x03ul << 16) /* ACPA Mask                          */
#define ACPA_NONE       (0x00ul << 16) /* No effect on TIOA output           */
#define ACPA_SET        (0x01ul << 16) /* SET TIOA output                    */
#define ACPA_CLEAR      (0x02ul << 16) /* Clear TIOA output                  */
#define ACPA_TOGGLE     (0x03ul << 16) /* Toggle TIOA output                 */

/* ACPC : RC Compare Effect on TIOA                                          */
#define ACPC            (0x03ul << 18) /* ACPC Mask                          */
#define ACPC_NONE       (0x00ul << 18) /* No effect on TIOA output           */
#define ACPC_SET        (0x01ul << 18) /* SET TIOA output                    */
#define ACPC_CLEAR      (0x02ul << 18) /* Clear TIOA output                  */
#define ACPC_TOGGLE     (0x03ul << 18) /* Toggle TIOA output                 */

/* AEEVT : External Event Effect on TIOA                                     */
#define AEEVT           (0x03ul << 20) /* AEEVT Mask                         */
#define AEEVT_NONE      (0x00ul << 20) /* No effect on TIOA output           */
#define AEEVT_SET       (0x01ul << 20) /* SET TIOA output                    */
#define AEEVT_CLEAR     (0x02ul << 20) /* Clear TIOA output                  */
#define AEEVT_TOGGLE    (0x03ul << 20) /* Toggle TIOA output                 */

/* ASWTRG : Software Trigger Effect on TIOA                                  */
#define ASWTRG          (0x03ul << 22) /* ASWTRG Mask                        */
#define ASWTRG_NONE     (0x00ul << 22) /* No effect on TIOA output           */
#define ASWTRG_SET      (0x01ul << 22) /* SET TIOA output                    */
#define ASWTRG_CLEAR    (0x02ul << 22) /* Clear TIOA output                  */
#define ASWTRG_TOGGLE   (0x03ul << 22) /* Toggle TIOA output                 */

/* BCPB : RB Compare Effect on TIOB                                          */
#define BCPB            (0x03ul << 24) /* BCPB Mask                          */
#define BCPB_NONE       (0x00ul << 24) /* No effect on TIOB output           */
#define BCPB_SET        (0x01ul << 24) /* SET TIOB output                    */
#define BCPB_CLEAR      (0x02ul << 24) /* Clear TIOB output                  */
#define BCPB_TOGGLE     (0x03ul << 24) /* Toggle TIOB output                 */

/* BCPC : RC Compare Effect on TIOB                                          */
#define BCPC            (0x03ul << 26) /* BCPC Mask                          */
#define BCPC_NONE       (0x00ul << 26) /* No effect on TIOB output           */
#define BCPC_SET        (0x01ul << 26) /* SET TIOB output                    */
#define BCPC_CLEAR      (0x02ul << 26) /* Clear TIOB output                  */
#define BCPC_TOGGLE     (0x03ul << 26) /* Toggle TIOB output                 */

/* BEEVT : External Event Effect on TIOB                                     */
#define BEEVT           (0x03ul << 28) /* BEEVT Mask                         */
#define BEEVT_NONE      (0x00ul << 28) /* No effect on TIOB output           */
#define BEEVT_SET       (0x01ul << 28) /* SET TIOB output                    */
#define BEEVT_CLEAR     (0x02ul << 28) /* Clear TIOB output                  */
#define BEEVT_TOGGLE    (0x03ul << 28) /* Toggle TIOB output                 */

/* BSWTRG : Software Trigger Effect on TIOB                                  */
#define BSWTRG          (0x03ul << 30) /* BSWTRG Mask                        */
#define BSWTRG_NONE     (0x00ul << 30) /* No effect on TIOB output           */
#define BSWTRG_SET      (0x01ul << 30) /* SET TIOB output                    */
#define BSWTRG_CLEAR    (0x02ul << 30) /* Clear TIOB output                  */
#define BSWTRG_TOGGLE   (0x03ul << 30) /* Toggle TIOB output                 */

/******************************************************************************
* SR, IER, IDR, IMR : GPT Status and Interrupt Registers (capture mode)
******************************************************************************/
#define COVFS           (0x01ul << 0)  /* Counter overflow                   */
#define LOVRS           (0x01ul << 1)  /* Load overrun                       */
#define CPCS            (0x01ul << 4)  /* Compare Register C                 */
#define LDRAS           (0x01ul << 5)  /* Load Register A                    */
#define LDRBS           (0x01ul << 6)  /* Load Register B                    */
#define ETRGS           (0x01ul << 7)  /* External Trigger                   */
#define TIOBS           (0x01ul << 16) /* TIOB Status                        */
#define TIOAS           (0x01ul << 17) /* TIOA Status                        */
#define TCLKS           (0x01ul << 18) /* TCLK Status                        */

/* SR Register Only                                                          */
#define CLKSTA          (0x01ul << 8)  /* Clock Status                       */
#define MTIOA           (0x01ul << 9)  /* TIOA Mirror                        */
#define MTIOB           (0x01ul << 10) /* TIOB Mirror                        */

/******************************************************************************
* SR, IER, IDR, IMR : GPT Status and Interrupt Registers (Waveform mode)
******************************************************************************/
/* COVFS  : Counter Overflow   - Same as Capture Mode                        */

#define CPAS            (0x01ul << 2)  /* Compare Register A                 */
#define CPBS            (0x01ul << 3)  /* Compare Register B                 */

/* CPCS   : Compare Register C - Same as Capture Mode                        */
/* ETRGS  : External Trigger   - Same as Capture Mode                        */
/* TIOBS  : TIOB Status        - Same as Capture Mode                        */
/* TIOAS  : TIOA Status        - Same as Capture Mode                        */
/* TCLKS  : TCLK Status        - Same as Capture Mode                        */

/* SR Register Only                                                          */
/* CLKSTA : Clock Status       - Same as Capture Mode                        */
/* MTIOA  : TIOA mirror        - Same as Capture Mode                        */
/* MTIOB  : TIOB mirror        - Same as Capture Mode                        */

/******************************************************************************
* CV : GPT Counter Value Register
******************************************************************************/
#define CV_MASK         (0xFFFFul << 0)   /* Counter Value Mask              */
      
/******************************************************************************
* RA : GPT Register A Value
******************************************************************************/
#define RA_MASK         (0xFFFFul << 0)   /* RA Mask                         */

/******************************************************************************
* RB : GPT Register B Value
******************************************************************************/
#define RB_MASK         (0xFFFFul << 0)   /* RB Mask                         */

/******************************************************************************
* RC : GPT Register C Value
******************************************************************************/
#define RC_MASK         (0xFFFFul << 0)   /* RC Mask                         */

/******************************************************************************
* BCR : GPT Block Control Register - 3 Channels Only
******************************************************************************/
/* SWRST : GPT Software Block Reset - Same as CR                             */
 
#define TCSYNC          (0x01ul << 1)     /* Synchronization Bit             */

/******************************************************************************
* BMR : GPT Block Mode Register - 3 Channels Only
******************************************************************************/
/* GPT0XC0S : External Clock XC0 Selection                                   */
#define GPT0XC0S        (0x03ul << 0)  /* GPT0XC0S Mask                      */
#define TCLK0_XC0       (0x00ul << 0)  /* XC0 signal select                  */
#define NONE_XC0        (0x01ul << 0)  /* None signal select                 */
#define TIOA1_XC0       (0x02ul << 0)  /* TIOA1 signal select                */
#define TIOA2_XC0       (0x03ul << 0)  /* TIOA2 signal select                */

/* GPT1XC1S : External Clock XC1 Selection                                   */
#define GPT1XC1S        (0x03ul << 2)  /* GPT0XC1S Mask                      */
#define TCLK1_XC1       (0x00ul << 2)  /* XC1 signal select                  */
#define NONE_XC1        (0x01ul << 2)  /* None signal select                 */
#define TIOA0_XC1       (0x02ul << 2)  /* TIOA0 signal select                */
#define TIOA2_XC1       (0x03ul << 2)  /* TIOA2 signal select                */

/* GPT2XC2S : External Clock XC2 Selection                                   */
#define GPT2XC2S        (0x03ul << 4)  /* GPT0XC2S Mask                      */
#define TCLK2_XC2       (0x00ul << 4)  /* XC2 signal select                  */
#define NONE_XC2        (0x01ul << 4)  /* None signal select                 */
#define TIOA0_XC2       (0x02ul << 4)  /* TIOA0 signal select                */
#define TIOA1_XC2       (0x03ul << 4)  /* TIOA1 signal select                */

/******************************************************************************
* TSTC : GPT Test Control Register in test mode
******************************************************************************/
#define LDCT            (0x01ul << 0)  /* Load Timer Counter (GPT1C)         */
#define LDCT0           (0x01ul << 0)  /* Load Timer 0 Counter (GPT3C)       */
#define LDCT1           (0x01ul << 1)  /* Load Timer 1 Counter (GPT3C)       */
#define LDCT2           (0x01ul << 2)  /* Load Timer 2 Counter (GPT3C)       */

/******************************************************************************
* TSTM : GPT Test Mode Register in test mode
******************************************************************************/
#define OCLKEN          (0x01ul << 0)  /* Output Clock on TIOB0 (GPT1C)      */
#define OCLKEN0         (0x01ul << 0)  /* Output Clock on TIOB0 for Timer 0 (GPT3C) */
#define OCLKEN1         (0x01ul << 1)  /* Output Clock on TIOB0 for Timer 1 (GPT3C) */
#define OCLKEN2         (0x01ul << 2)  /* Output Clock on TIOB0 for Timer 2 (GPT3C) */


/******************************************************************************
*************************** GPT Macros Definition *****************************
******************************************************************************/
/* PER, PDR, PSR : GPT PIO Registers                                         */
#define CSP_GPT_SET_PER(gpt, val)      ((gpt)->PER = (val))    /* Enable     */
#define CSP_GPT_SET_PDR(gpt, val)      ((gpt)->PDR = (val))    /* Disable    */
#define CSP_GPT_GET_PSR(gpt)           ((gpt)->PSR)            /* Status     */

/* OER, ODR, OSR : GPT Output Registers                                      */
#define CSP_GPT_SET_OER(gpt, val)      ((gpt)->OER = (val))    /* Enable     */
#define CSP_GPT_SET_ODR(gpt, val)      ((gpt)->ODR = (val))    /* Disable    */
#define CSP_GPT_GET_OSR(gpt)           ((gpt)->OSR)            /* Status     */

/* SODR, CODR, ODSR, PDSR : GPT Output Data Registers                        */
#define CSP_GPT_SET_SODR(gpt, val)     ((gpt)->SODR = (val))   /* Set        */
#define CSP_GPT_SET_CODR(gpt, val)     ((gpt)->CODR = (val))   /* Clear      */
#define CSP_GPT_GET_ODSR(gpt)          ((gpt)->ODSR)           /* Status     */
#define CSP_GPT_GET_PDSR(gpt)          ((gpt)->PDSR)           /* Pin Status */

/* MDER, MDDR, MDSR : GPT Multi-Driver Registers                             */
#define CSP_GPT_SET_MDER(gpt, val)     ((gpt)->MDER = (val))   /* Enable     */
#define CSP_GPT_SET_MDDR(gpt, val)     ((gpt)->MDDR = (val))   /* Disable    */
#define CSP_GPT_GET_MDSR(gpt)          ((gpt)->MDSR)           /* Status     */

/* ECR, DCR, PMSR : GPT Power Management Registers                           */
#define CSP_GPT_SET_ECR(gpt, val)      ((gpt)->ECR = (val))    /* Enable     */
#define CSP_GPT_SET_DCR(gpt, val)      ((gpt)->DCR = (val))    /* Disable    */
#define CSP_GPT_GET_PMSR(gpt)          ((gpt)->PMSR)           /* Status     */

/* CR : GPT Control Register                                                 */
#define CSP_GPT_SET_CR(gpt, val)       ((gpt)->CR = (val))

/* MR : GPT Mode Register                                                    */
#define CSP_GPT_GET_MR(gpt)            ((gpt)->MR)
#define CSP_GPT_SET_MR(gpt, mode)      ((gpt)->MR = (mode))

/* SR : GPT Status Register                                                  */
#define CSP_GPT_GET_SR(gpt)            ((gpt)->SR)

/* IER, IDR, IMR : GPT Interrupt Registers                                   */
#define CSP_GPT_SET_IER(gpt, val)      ((gpt)->IER = (val))    /* Enable     */
#define CSP_GPT_SET_IDR(gpt, val)      ((gpt)->IDR = (val))    /* Disable    */
#define CSP_GPT_GET_IMR(gpt)           ((gpt)->IMR)            /* Mask       */

/* CV : GPT Counter Value Register                                           */
#define CSP_GPT_GET_CV(gpt)            ((gpt)->CV)

/* RA : GPT Capture - Compare Register A                                     */
#define CSP_GPT_GET_RA(gpt)            ((gpt)->RA)
#define CSP_GPT_SET_RA(gpt, val)       ((gpt)->RA = (val))

/* RB : GPT Capture - Compare Register B                                     */
#define CSP_GPT_GET_RB(gpt)            ((gpt)->RB)
#define CSP_GPT_SET_RB(gpt, val)       ((gpt)->RB = (val))

/* RC : GPT Compare Register C                                               */
#define CSP_GPT_GET_RC(gpt)            ((gpt)->RC)
#define CSP_GPT_SET_RC(gpt, val)       ((gpt)->RC = (val))

/* BCR : GPT Block Control Register - 3 Channels Only                        */
#define CSP_GPT_SET_BCR(gpt, val)      ((gpt)->BCR = (val))

/* BMR : GPT Block Mode Register - 3 Channels Only                           */
#define CSP_GPT_GET_BMR(gpt)           ((gpt)->BMR)
#define CSP_GPT_SET_BMR(gpt, val)      ((gpt)->BMR = (val))

/* TSTC : GPT Test Control Register in Test Mode                             */
#define CSP_GPT_SET_TSTC(gpt, val)     ((gpt)->TSTC = (val))

/* TSTM : GPT Test Mode Register in Test Mode                                */
#define CSP_GPT_GET_TSTM(gpt)          ((gpt)->TSTM)
#define CSP_GPT_SET_TSTM(gpt, val)     ((gpt)->TSTM = (val))


/******************************************************************************
********************* GPT External Functions Declaration **********************
******************************************************************************/
extern void CSP_GPTInit(CSP_GPT_T *const gpt, U32_T mode, U16_T *reg_value);
extern void CSP_GPTClose(CSP_GPT_T *const gpt);
extern void CSP_GPTConfigInterrupt(CSP_GPT_T *const gpt, U32_T int_mode, U32_T int_mask, U32_T callback);
extern void CSP_GPTEnable(CSP_GPT_T *const gpt);
extern void CSP_GPTDisable(CSP_GPT_T *const gpt);
extern void CSP_GPTPioInit(CSP_GPT_T *const gpt, U32_T pio_mask, U32_T output_pio);
extern U32_T CSP_GPTPioGetStatus(CSP_GPT_T *const gpt);
extern void CSP_GPTPioSet(CSP_GPT_T *const gpt, U32_T pio_mask);
extern void CSP_GPTPioClear(CSP_GPT_T *const gpt, U32_T pio_mask);


#endif   /* CSP_GPT_H */



