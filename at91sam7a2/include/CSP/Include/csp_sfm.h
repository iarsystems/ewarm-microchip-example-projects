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
* File Name       : csp_sfm.h
* Description     : Definitions, Macros and function declarations for
*                   Special Function module
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

#ifndef CSP_SFM_H
#define CSP_SFM_H


/******************************************************************************
************************* SFM Structure Definition ****************************
******************************************************************************/
typedef struct
{
   CSP_REGISTER_T CIDR;                   /* Chip ID Register                */
   CSP_REGISTER_T EXID;                   /* Chip ID Extension Register      */
   CSP_REGISTER_T RSR;                    /* Reset Status Register           */
   CSP_REGISTER_T ReservedA[2];
   CSP_REGISTER_T TME;                    /* Test Mode Enable Register       */
} CSP_SFM_T, *CSP_SFM_PTR;


/******************************************************************************
************************** SFM Registers Definition ***************************
******************************************************************************/

/******************************************************************************
* CIDR : SFM Chip ID Register
******************************************************************************/
/* NVPMS : Non Volatile Program Memory Size                                  */
#define NVPMS           (0x0Ful << 0)     /* NVPMS Mask                      */
#define NVPMS_NONE      (0x00ul << 0)     /* NVPMS None                      */

/* NVDMS : Non Volatile Data Memory Size                                     */
#define NVDMS           (0x0Ful << 4)     /* NVDMS Mask                      */
#define NVDMS_NONE      (0x00ul << 4)     /* NVDMS None                      */

/* IRS : Internal RAM Size                                                   */
#define IRS             (0x0Ful << 8)     /* IRS Mask                        */
#define IRS_NONE        (0x00ul << 8)     /* IRS None                        */

/* NVPMT : Non Volatile Program Memory Type                                  */
#define NVPMT           (0x0Ful << 12)    /* NVPMT Mask                      */
#define NVPMT_NONE      (0x00ul << 12)    /* ROM less                        */
#define NVPMT_MASKROM   (0x01ul << 12     /* Mask ROM                        */

/* ARCH : Core Architecture                                                  */
#define ARCH            (0x0Ful << 16)    /* ARCH Mask                       */
#define ARCH_ARM7TDMI   (0x00ul << 16)    /* ARM7TDMI core                   */

/* EXT : Extension Flag                                                      */
#define EXT             (0x01ul << 31)    /* EXT Mask                        */

/******************************************************************************
* EXID : SFM Chip ID Extension Register
******************************************************************************/
#define REVT            (0x0Ful << 0)     /* Revision number Third digit     */
#define REVS            (0x0Ful << 4)     /* Revision number Second digit    */
#define REVF            (0x0Ful << 8)     /* Revision number First digit     */
#define PRJCT           (0x0Ful << 12)    /* ET Project code Third digit     */
#define PRJCS           (0x0Ful << 16)    /* ET Project code Second digit    */
#define PRJCF           (0x0Ful << 20)    /* ET Project code First digit     */

/* TYPE : Europe Technologies project code type                              */
#define TYPE            (0x0Ful << 24)    /* Type Mask                       */
#define TYPE_MET        (0x00ul << 24)    /* Metering                        */
#define TYPE_AUT        (0x01ul << 24)    /* Automotive                      */
#define TYPE_IND        (0x10ul << 24)    /* Industry                        */

/******************************************************************************
* RSR : SFM Reset Status Register
******************************************************************************/
/* RESET : Cause of Reset                                                    */
#define RESET_MASK      (0xFF   << 0)     /* RESET Mask                      */
#define RESET_EXTERNAL  (0x6Cul << 0)     /* External Reset                  */
#define RESET_INTERNAL  (0x53ul << 0)     /* Internal Watchdog Reset         */

/******************************************************************************
* TME : SFM Test Mode Enable Register
******************************************************************************/
#define TESTEN          (0x01ul << 1)     /* Test Enable                     */
#define TESTKEY         (0xD64Aul << 16)  /* Test Key for test mode entry    */


/******************************************************************************
*************************** SFM Macros Definition *****************************
******************************************************************************/
/* CIDR : SFM Chip ID Register                                               */
#define CSP_SFM_GET_CIDR(sfm)       ((sfm)->CIDR)

/* EXID : SFM Chip ID Extension Register                                     */
#define CSP_SFM_GET_EXID(sfm)       ((sfm)->EXID)

/* RSR : SFM Reset Status Register                                           */
#define CSP_SFM_GET_RSR(sfm)        ((sfm)->RSR)

/* TME : SFM Test Mode Enable Register                                       */
#define CSP_SFM_GET_TME(sfm)        ((sfm)->TME)
#define CSP_SFM_SET_TME(sfm, val)   ((sfm)->TME = (val))


/******************************************************************************
********************* SFM External Functions Declaration **********************
******************************************************************************/


#endif   /* CSP_SFM_H */
