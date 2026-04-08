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
* File Name   : bsp_pwm4c.h
* Description : Definitions and Standard Typedef for
                PWM (Board Support Package Layer)               
* Version     : 1.00
*
*       +----- (NEW | MODify | ADD | DELete)                                 
*       |                                                                    
*  No   |   when       who                what               
*-----+---+----------+------------------+--------------------------------------
* 000  NEW  29/06/00   Patrice VILCHEZ      Creation
*----------------------------------------------------------------------------*/


#ifndef BSP_PWM4C_H
#define BSP_PWM4C_H

/****************************************************************************
*  External Function Declaration
****************************************************************************/
extern void BSP_PWM4CSetOuput(CSP_PWM4C_T *pwm4c, U8_T channel_u8, U32_T, U16_T);

#endif  /* BSP_PWM4C_H */
