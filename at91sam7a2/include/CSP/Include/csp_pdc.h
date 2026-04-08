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
*----------------------------------------------------------------------------*/

#ifndef CSP_PDC_H
#define CSP_PDC_H


/******************************************************************************
************************* PDC Structure Definition ****************************
******************************************************************************/

/* Number of PDC Channels                                                    */
#define NB_PDC_CHANNEL  32u

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
#define PDC_CHANNEL11   11u
#define PDC_CHANNEL12   12u
#define PDC_CHANNEL13   13u
#define PDC_CHANNEL14   14u
#define PDC_CHANNEL15   15u
#define PDC_CHANNEL16   16u
#define PDC_CHANNEL17   17u
#define PDC_CHANNEL18   18u
#define PDC_CHANNEL19   19u
#define PDC_CHANNEL20   20u
#define PDC_CHANNEL21   21u
#define PDC_CHANNEL22   22u
#define PDC_CHANNEL23   23u
#define PDC_CHANNEL24   24u
#define PDC_CHANNEL25   25u
#define PDC_CHANNEL26   26u
#define PDC_CHANNEL27   27u
#define PDC_CHANNEL28   28u
#define PDC_CHANNEL29   29u
#define PDC_CHANNEL30   30u
#define PDC_CHANNEL31   31u


/******************************************************************************
* PDC Channel Structure
******************************************************************************/
typedef struct 
{
   CSP_REGISTER_T  PRA;          /* Channel Peripheral Register Address      */
   CSP_REGISTER_T  CR;           /* Channel Control Register                 */
   CSP_REGISTER_T  MPR;          /* Channel Memory Pointer                   */
   CSP_REGISTER_T  TCR;          /* Channel Transfer Counter                 */
} PDC_CHANNEL_T, *PDC_CHANNEL_PTR;


/******************************************************************************
* PDC Structure
******************************************************************************/
typedef struct
{
   CSP_REGISTER_T  ReservedA[32];
   PDC_CHANNEL_T   CHANNEL[NB_PDC_CHANNEL];  /* PDC Channels                 */
   CSP_REGISTER_T  ReservedB[788];
   CSP_REGISTER_T  TR;                       /* PDC Test Register            */
} CSP_PDC_T, *CSP_PDC_PTR;


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
#define TCH11              (0x01ul << 11)    /* PDC Trigger Channel 11       */
#define TCH12              (0x01ul << 12)    /* PDC Trigger Channel 12       */
#define TCH13              (0x01ul << 13)    /* PDC Trigger Channel 13       */
#define TCH14              (0x01ul << 14)    /* PDC Trigger Channel 14       */
#define TCH15              (0x01ul << 15)    /* PDC Trigger Channel 15       */
#define TCH16              (0x01ul << 16)    /* PDC Trigger Channel 16       */
#define TCH17              (0x01ul << 17)    /* PDC Trigger Channel 17       */
#define TCH18              (0x01ul << 18)    /* PDC Trigger Channel 18       */
#define TCH19              (0x01ul << 19)    /* PDC Trigger Channel 19       */
#define TCH20              (0x01ul << 20)    /* PDC Trigger Channel 20       */
#define TCH21              (0x01ul << 21)    /* PDC Trigger Channel 21       */
#define TCH22              (0x01ul << 22)    /* PDC Trigger Channel 22       */
#define TCH23              (0x01ul << 23)    /* PDC Trigger Channel 23       */
#define TCH24              (0x01ul << 24)    /* PDC Trigger Channel 24       */
#define TCH25              (0x01ul << 25)    /* PDC Trigger Channel 25       */
#define TCH26              (0x01ul << 26)    /* PDC Trigger Channel 26       */
#define TCH27              (0x01ul << 27)    /* PDC Trigger Channel 27       */
#define TCH28              (0x01ul << 28)    /* PDC Trigger Channel 28       */
#define TCH29              (0x01ul << 29)    /* PDC Trigger Channel 29       */
#define TCH30              (0x01ul << 30)    /* PDC Trigger Channel 30       */
#define TCH31              (0x01ul << 31)    /* PDC Trigger Channel 31       */


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
