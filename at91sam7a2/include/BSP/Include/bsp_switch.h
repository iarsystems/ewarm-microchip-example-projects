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
* File Name   : bsp_switch.h
* Description : Definitions and standard typedef for EsayCan3 BSP layer
* Version     : 1.01
*
*       +----- (NEW | MODify | ADD | DELete)
*       |
*  No   |   when       who                what
*-----+---+----------+------------------+--------------------------------------
* 000  NEW  05/07/01   David LEWIN        Creation
* 001  MOD  18/07/01   Frederic SAMSON    Update (see bsp_switch.c comments)
*----------------------------------------------------------------------------*/

#ifndef BSP_SWITCH_H
#define BSP_SWITCH_H

/****************************************************************************
*  BSP SWITCH Receive Callback Definition
****************************************************************************/
typedef void *(BSP_SWITCH_CALLBACK_T)(void);

/****************************************************************************
* BSP SWITCH Structure Definitions
****************************************************************************/
typedef struct
{
   BSP_SWITCH_CALLBACK_T   *TIOB1_function;
   BSP_SWITCH_CALLBACK_T   *TIOA2_function;
   BSP_SWITCH_CALLBACK_T   *IRQ0_function;
} BSP_SWITCH_T, *BSP_SWITCH_PTR;


/****************************************************************************
* BSP SWITCH External Variables Definition
****************************************************************************/
extern BSP_SWITCH_T BSP_SWITCH_a_s ;

/****************************************************************************
* BSP SWITCH "C" General Interrupt Handler Function Declaration
****************************************************************************/
extern void BSP_AsmTIOB1InterruptHandler (void);
extern void BSP_AsmTIOA2InterruptHandler (void);
extern void BSP_AsmIRQ0InterruptHandler(void);

/****************************************************************************
* BSP SWITCH Power Management Function Declaration
****************************************************************************/
extern void BSP_SWITCHInit(void);


#endif
