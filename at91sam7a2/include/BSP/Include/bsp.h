/****************************************************************************
FILE        BSP.H - (hardware dependent)

VERSION     1.00

AUTHORS     Frederic SAMSON (Europe Technologies) 

COPYRIGHT   (c) Europe Technologies
            COMMERCIAL IN CONFIDENCE

CONTAINS    Definitions and Standard Typedef for EasyCan3
            Board Support Package Layer 

MODIFICATION HISTORY

 * 1  03/07/2001  Frederic SAMSON
        - Initial File 
 * 2  18/07/2001  Frederic SAMSON
        - Add bsp_switch.h 
****************************************************************************/

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
