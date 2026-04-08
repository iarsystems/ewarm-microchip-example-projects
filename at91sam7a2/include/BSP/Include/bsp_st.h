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
* Processor   : easyCAN3
* File Name   : bsp_st.h
* Description : Definitions and Standard Typedef for Simple Timer for the BSP
* Version     : 1.01
*
*       +----- (NEW | MODify | ADD | DELete)
*       |
*  No   |   when       who                what
*-----+---+----------+------------------+--------------------------------------
* 001  MOD  01/04/01   Olivier MAZUYER    Creation
* 002  ADD  08/08/01   Frederic SAMSON    -  Add structures, variables, functions
*                                            definitions
*----------------------------------------------------------------------------*/


#ifndef BSP_ST_H
#define BSP_ST_H

/*****************************************/
/* BSP ST Callback Definition            */
/*****************************************/
typedef void *(BSP_ST_CALLBACK_T)(CSP_ST_T *st, U8_T channel_u8);

/*****************************************/
/* BSP ST Structure Definition           */
/*****************************************/
typedef struct
{
   BSP_ST_CALLBACK_T   *callback_function;
} BSP_ST_CONFIGURE_T, *BSP_ST_CONFIGURE_PTR;

/*****************************************/
/* BSP ST External variables Definitions */
/*****************************************/
extern BSP_ST_CONFIGURE_T BSP_ST_Config_s ;

/******************************************************************************
*  External Function Declaration
******************************************************************************/
extern void BSP_STWaitDelay(CSP_ST_T *const st, U16_T channel_u8, U32_T delay);
extern void BSP_STStartDelay(CSP_ST_T *const st, U16_T channel_u8, U32_T delay_u32);
extern void ST0CH0_InterruptHandler(void);
extern void ST0CH1_InterruptHandler(void);
extern void ST1CH0_InterruptHandler(void);
extern void ST1CH1_InterruptHandler(void);
extern void ST0CH0_AsmInterruptHandler(void);
extern void ST0CH1_AsmInterruptHandler(void);
extern void ST1CH0_AsmInterruptHandler(void);
extern void ST1CH1_AsmInterruptHandler(void);

#endif  /* BSP_ST_H */
