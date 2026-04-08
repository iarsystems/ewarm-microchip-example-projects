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
* File Name   : bsp_capt.h
* Description : Definitions and Standard Typedef for
                Capture (Board Support Package Layer)               
* Version     : 1.00
*
*       +----- (NEW | MODify | ADD | DELete)                                 
*       |                                                                    
*  No   |   when       who                what               
*-----+---+----------+------------------+--------------------------------------
* 000  NEW  31/07/01   Olivier Mazuyer    Creation
*----------------------------------------------------------------------------*/


/******************************************************************************
* Include files
******************************************************************************/


#ifndef BSP_CAPT_H
#define BSP_CAPT_H


/******************************************************************************
* BSP CAPT Function Declaration
******************************************************************************/
extern U32_T BSP_CAPT_PositiveEdgesFrequency(CSP_CAPT_T *capt, U8_T  prescalar_u8);
extern U32_T BSP_CAPT_NegativeEdgesFrequency(CSP_CAPT_T *capt, U8_T  prescalar_u8);
extern U32_T BSP_CAPT_EachEdgesFrequency(CSP_CAPT_T *capt, U8_T  prescalar_u8);

#endif  /* BSP_CAPT_H */
