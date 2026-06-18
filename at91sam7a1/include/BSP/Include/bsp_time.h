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
* Processor   : SmartCAN1 for EVMSmartCAN
* File Name   : bsp_time.h
* Description : Definitions and Standard Typedef for
                Time management (Board Support Package Layer)               
* Version     : 1.01
*
*       +----- (NEW | MODify | ADD | DELete)                                 
*       |                                                                    
*  No   |   when       who                what               
*-----+---+----------+------------------+--------------------------------------
* 000  NEW  29/06/00   Patrice Vilchez        Creation
* 001  MOD  19/07/01   Frederic SAMSON        Update
*----------------------------------------------------------------------------*/


#ifndef BSP_TIME_H
#define BSP_TIME_H

/************************************/
/* BSP Time Structure Definiton    */
/************************************/

/*******************************************************************************
Type: BSP_TIME_T
Description: BSP time Structure Definition
Fields:
- 创U64_T创  counts :
- 创U32_T创  msecs :
- 创U32_T创  seconds :
- 创int创    TimeInvalid:1 : 
- 创int创    Reserved:31 : 
*******************************************************************************/
typedef struct 
{
  U64_T  counts;
  U32_T  msecs;
  U32_T  seconds;
  int    TimeInvalid:1;
  int    Reserved:31;
} BSP_TIME_T, *BSP_TIME_PTR;

/****************************************************************************
* BSP Object Declaration
****************************************************************************/
extern BSP_TIME_T  BSP_Time_s;

/****************************************************************************
* BSP TIME External Functions Declaration
****************************************************************************/
extern void BSP_TimeInit(void);
extern void BSP_TimeGet(BSP_TIME_PTR bsp_time_p_s);
extern void BSP_TimeSet(U32_T time_u32);


#endif  /* BSP_TIME_H */
