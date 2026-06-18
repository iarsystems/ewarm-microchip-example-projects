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
* File Name       : csp_pdc.h
* Description     : Definitions, Macros and function declarations for
*                   Peripheral Data Controller module
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
* 004  MOD  21/11/02   Bruno SALLE		  Modification for SmartCAN1 : NB_PDC_CHANNELS
*----------------------------------------------------------------------------*/

#ifndef CSP_PDC_H
#define CSP_PDC_H


/******************************************************************************
************************* PDC Structure Definition ****************************
******************************************************************************/

/* Number of PDC Channels                                                    */
#define NB_PDC_CHANNEL  11u

/* PDC Channel Definition                                                    */
#define PDC_CHANNEL0    0u
#define PDC_CHANNEL1    1u
#define PDC_CHANNEL2    2u
#define PDC_CHANNEL3    3u
#define PDC_CHANNEL4    4u
#define PDC_CHANNEL5    5u
#define PDC_CHANNEL6    6u
#define PDC_CHANNEL7    7u
#define PDC_CHANNEL8    8u
#define PDC_CHANNEL9    9u
#define PDC_CHANNEL10   10u

/******************************************************************************
* PDC Channel Structure
******************************************************************************/

/*******************************************************************************
Type: PDC_CHANNEL_T
Description: PDC Channel Structure
Fields:
- 创CSP_REGISTER_T创  PRA :            Channel Peripheral Register Address      
- 创CSP_REGISTER_T创  CR :             Channel Control Register                 
- 创CSP_REGISTER_T创  MPR :            Channel Memory Pointer                   
- 创CSP_REGISTER_T创  TCR :            Channel Transfer Counter                 
*******************************************************************************/
typedef struct
{
   CSP_REGISTER_T  PRA;          /* Channel Peripheral Register Address      */
   CSP_REGISTER_T  CR;           /* Channel Control Register                 */
   CSP_REGISTER_T  MPR;          /* Channel Memory Pointer                   */
   CSP_REGISTER_T  TCR;          /* Channel Transfer Counter                 */
} PDC_CHANNEL_T;


/******************************************************************************
* PDC Structure
******************************************************************************/

/*******************************************************************************
Type: CSP_PDC_T
Description: PDC Structure
Fields:
- 创CSP_REGISTER_T创  ReservedA[32] :
- 创PDC_CHANNEL_T创   CHANNEL[NB_PDC_CHANNEL] :   PDC Channels                 
- 创CSP_REGISTER_T创  ReservedB[788] :
- 创CSP_REGISTER_T创  TR :                        PDC Test Register            
*******************************************************************************/
typedef struct
{
   CSP_REGISTER_T  ReservedA[32];
   PDC_CHANNEL_T   CHANNEL[NB_PDC_CHANNEL];  /* PDC Channels                 */
   CSP_REGISTER_T  ReservedB[884];
   CSP_REGISTER_T  TR;                       /* PDC Test Register            */
} CSP_PDC_T;


/******************************************************************************
************************** PDC Registers Definition ***************************
******************************************************************************/

/******************************************************************************
* CR : PDC Control Register
******************************************************************************/
/* DIR : PDC Transfer Direction                                              */
#define DIR                (0x01ul << 0)     /* DIR Mask                     */
#define DIR_PERIPH_TO_MEM  (0x00ul << 0)     /* Peripheral to Memory         */
#define DIR_MEM_TO_PERIPH  (0x01ul << 0)     /* Memory to Peripheral         */

/* SIZE : PDC Transfer Size                                                  */
#define SIZE               (0x03ul << 1)     /* SIZE Mask                    */
#define SIZE_BYTE          (0x00ul << 1)     /* Byte Size (8 bits)           */
#define SIZE_HALFWORD      (0x01ul << 1)     /* Half Word Size (16 bits)     */
#define SIZE_WORD          (0x02ul << 1)     /* Word Size (32 bits)          */

/******************************************************************************
* TR : PDC Test Register
******************************************************************************/
#define TCH0               (0x01ul << 0)     /* PDC Trigger Channel 0        */
#define TCH1               (0x01ul << 1)     /* PDC Trigger Channel 1        */
#define TCH2               (0x01ul << 2)     /* PDC Trigger Channel 2        */
#define TCH3               (0x01ul << 3)     /* PDC Trigger Channel 3        */
#define TCH4               (0x01ul << 4)     /* PDC Trigger Channel 4        */
#define TCH5               (0x01ul << 5)     /* PDC Trigger Channel 5        */
#define TCH6               (0x01ul << 6)     /* PDC Trigger Channel 6        */
#define TCH7               (0x01ul << 7)     /* PDC Trigger Channel 7        */
#define TCH8               (0x01ul << 8)     /* PDC Trigger Channel 8        */
#define TCH9               (0x01ul << 9)     /* PDC Trigger Channel 9        */
#define TCH10              (0x01ul << 10)    /* PDC Trigger Channel 10       */

/******************************************************************************
*************************** PDC Macros Definition *****************************
******************************************************************************/
/* PRA : PDC Channel Peripheral Register Address                             */
#define CSP_PDC_CHANNEL_GET_PRA(pdc, channel)            ((pdc)->CHANNEL[(channel)].PRA)
#define CSP_PDC_CHANNEL_SET_PRA(pdc, channel, address)   ((pdc)->CHANNEL[(channel)].PRA = (address))

/* CR : PDC Control Register                                                 */
#define CSP_PDC_CHANNEL_GET_CR(pdc, channel)             ((pdc)->CHANNEL[(channel)].CR)
#define CSP_PDC_CHANNEL_SET_CR(pdc, channel, val)        ((pdc)->CHANNEL[(channel)].CR = (val))

/* MPR : PDC Channel Memory Pointer                                          */
#define CSP_PDC_CHANNEL_GET_MPR(pdc, channel)            ((pdc)->CHANNEL[(channel)].MPR)
#define CSP_PDC_CHANNEL_SET_MPR(pdc, channel, address)   ((pdc)->CHANNEL[(channel)].MPR = (address))

/* TCR : PDC Channel Transfer Counter                                        */
#define CSP_PDC_CHANNEL_GET_TCR(pdc, channel)            ((pdc)->CHANNEL[(channel)].TCR)
#define CSP_PDC_CHANNEL_SET_TCR(pdc, channel, counter)   ((pdc)->CHANNEL[(channel)].TCR = (counter))

/* TR : PDC Test Register                                                    */
#define CSP_PDC_SET_TR(pdc, trigger)                     ((pdc)->TR = (trigger))


/******************************************************************************
********************* PDC External Functions Declaration **********************
******************************************************************************/
extern void CSP_PDCInitRx(U32_T periph_address, U32_T register_address, U8_T size);
extern void CSP_PDCInitTx(U32_T periph_address, U32_T register_address, U8_T size);
extern void CSP_PDCStartRx(U32_T periph_address, U32_T mem_address, U16_T length);
extern void CSP_PDCStartTx(U32_T periph_address, U32_T mem_address, U16_T length);


#endif   /* CSP_PDC_H */
