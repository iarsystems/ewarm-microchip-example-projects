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
* File Name       : csp_amc.h
* Description     : Definitions, Macros and function declarations for
*                   Advanced Memory Controller module
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
*----------------------------------------------------------------------------*/

#ifndef CSP_AMC_H
#define CSP_AMC_H


/******************************************************************************
************************** AMC Structure Definition ***************************
******************************************************************************/
typedef struct
{ 
   CSP_REGISTER_T CSR[8];              /* AMC Chip Select Register           */
   CSP_REGISTER_T RCR;                 /* AMC Remap Control Register         */
   CSP_REGISTER_T MCR;                 /* AMC Memory Control Register        */
} CSP_AMC_T, *CSP_AMC_PTR;


/******************************************************************************
************************** AMC Registers Definition ***************************
******************************************************************************/

/******************************************************************************
* CSR : AMC Chip Select Register
******************************************************************************/
/* DBW : Data Bus Width                                                      */
#define DBW            (0x03ul << 0)   /* DBW Mask                           */
#define DBW_16         (0x01ul << 0)   /* 16-bit Data Bus                    */
#define DBW_8          (0x01ul << 1)   /* 8-bit Data Bus                     */

/* NWS : Number of Wait States                                               */
#define NWS            (0x07ul << 2)   /* NWS Mask                           */
#define NWS_1          (0x00ul << 2)   /* 1 Wait State added                 */
#define NWS_2          (0x01ul << 2)   /* 2 Wait States added                */
#define NWS_3          (0x02ul << 2)   /* 3 Wait States added                */
#define NWS_4          (0x03ul << 2)   /* 4 Wait States added                */
#define NWS_5          (0x04ul << 2)   /* 5 Wait States added                */
#define NWS_6          (0x05ul << 2)   /* 6 Wait States added                */
#define NWS_7          (0x06ul << 2)   /* 7 Wait States added                */
#define NWS_8          (0x07ul << 2)   /* 8 Wait States added                */

/* WSE : Wait State Enable                                                   */
#define WSE            (0x01ul << 5)   /* WSE Mask                           */
#define WSE_ENABLE     (0x01ul << 5)   /* Wait State Enable                  */
#define WSE_DISABLE    (0x00ul << 5)   /* Wait State Disable                 */

/* PAGES : PAGE Size (MBytes)                                                */
#define PAGES          (0x03ul << 7)   /* PAGES Mask                         */
#define PAGES_1        (0x00ul << 7)   /* 1  MBytes Page Size                */
#define PAGES_4        (0x01ul << 7)   /* 4  MBytes Page Size                */
#define PAGES_16       (0x02ul << 7)   /* 16 MBytes Page Size                */
#define PAGES_64       (0x03ul << 7)   /* 64 MBytes Page Size                */

/* TDF : Data Float Output Time                                              */
#define TDF            (0x07ul << 9)   /* TDF Mask                           */
#define TDF_0          (0x00ul << 9)   /* 0 cycle added                      */
#define TDF_1          (0x01ul << 9)   /* 1 cycle added                      */
#define TDF_2          (0x02ul << 9)   /* 2 cycle added                      */
#define TDF_3          (0x03ul << 9)   /* 3 cycle added                      */
#define TDF_4          (0x04ul << 9)   /* 4 cycle added                      */
#define TDF_5          (0x05ul << 9)   /* 5 cycle added                      */
#define TDF_6          (0x06ul << 9)   /* 6 cycle added                      */
#define TDF_7          (0x07ul << 9)   /* 7 cycle added                      */

/* BAT : Byte Access Type                                                    */
#define BAT            (0x01ul << 12)  /* BAT Mask                           */
#define BAT_WRITE      (0x00ul << 12)  /* Byte Write Access Type             */
#define BAT_SELECT     (0x01ul << 12)  /* Byte Select Access Type            */
                             
/* CSEN : Chip Select Enable                                                 */
#define CSEN           (0x01ul << 13)  /* CSEN Mask                          */
#define CSEN_ENABLE    (0x01ul << 13)  /* Chip Select Enable                 */
#define CSEN_DISABLE   (0x00ul << 13)  /* Chip Select Disable                */

/* BA : Base Address                                                         */
#define BA             (0x3FF00000ul)  /* BA Mask                            */

/******************************************************************************
* RCR : AMC Remap Control Register
******************************************************************************/
/* RCB : Remap Command Bit                                                   */
#define RCB            (0x01ul << 0)

/******************************************************************************
* MCR : AMC Memory Control Register
******************************************************************************/
/* ALE : Address Line Enable                                                 */
#define ALE            (0x07ul << 0)   /* ALE Mask                           */
#define ALE_2          (0x06ul << 0)   /* 2 Mbytes Chip Select Access        */
#define ALE_1          (0x07ul << 0)   /* 1 Mbytes Chip Select Access        */

/* DRP : Data Read Protocol                                                  */
#define DRP            (0x01ul << 4)


/******************************************************************************
**************************** AMC Macros Definition ****************************
******************************************************************************/
/* CSR : AMC Chip Select Registers                                           */
#define CSP_AMC_GET_CSR(amc, cs)       ((amc)->CSR[(cs)])
#define CSP_AMC_SET_CSR(amc, cs, val)  ((amc)->CSR[(cs)] = (val))

/* RCR : AMC Remap Control Register                                          */
#define CSP_AMC_GET_RCR(amc)           ((amc)->RCR)
#define CSP_AMC_SET_RCR(amc, val)      ((amc)->RCR = (val))

/* MCR : AMC Memory Control Register                                         */
#define CSP_AMC_GET_MCR(amc)           ((amc)->MCR)
#define CSP_AMC_SET_MCR(amc, val)      ((amc)->MCR = (val))


/******************************************************************************
********************** AMC External Functions Declaration *********************
******************************************************************************/
extern U32_T CSP_AMCGetCSRegister(U16_T cs);
extern void CSP_AMCSetCSRegister(U16_T cs, U32_T mode);
extern void CSP_AMCRemapControl(void);
extern void CSP_AMCSetMemoryControl(U32_T val);


#endif   /* CSP_AMC_H */

