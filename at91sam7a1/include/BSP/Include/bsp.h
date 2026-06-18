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
* File Name   : bsp.h
* Description : Definitions and Standard Typedef for Board Support Package Layer
*               
* Version     : 1.00
*
*       +----- (NEW | MODify | ADD | DELete)                                 
*       |                                                                    
*  No   |   when       who                what               
*-----+---+----------+------------------+--------------------------------------
* 000  NEW  01/04/01   Olivier MAZUYER    Creation
*----------------------------------------------------------------------------*/


#ifndef BSP_H
#define BSP_H

#include "csp.h"
#include "bsp_adc.h"
#include "bsp_buzzer.h"
#include "bsp_can.h"
#include "bsp_capt.h"
#include "bsp_eeprom.h"
#include "bsp_gpt.h"
#include "bsp_lcd.h"
#include "bsp_pwm4c.h"
#include "bsp_st.h"
#include "bsp_switch.h"


/******************************************************************************
* Clock Value 
******************************************************************************/
#define BSP_SYSTEM_CLOCK   30000000 /* 30 MHz */
#define BSP_EXT_CLOCK      32768    /* 32KHZ */

#endif
