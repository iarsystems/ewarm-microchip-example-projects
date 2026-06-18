/****************************************************************************
FILE        BSP_CAN_CONF.C - (hardware dependent)

VERSION     1.00

AUTHORS     Patrice Vilchez (Europe Technologies)
            David Combe     (Europe Technologies)

COPYRIGHT   (c) Europe Technologies
            COMMERCIAL IN CONFIDENCE

CONTAINS    Constant declarations for Specific Bsp (CAN) 
            Configuration Data For Mack Application

 MODIFICATION HISTORY

 * 1  02/10/00  Patrice Vilchez / David Combe
                - Initial Document
****************************************************************************/

#include "csp.h"
#include "bsp.h"

/***************************************************************************
* BSP CAN Controller Configuration Data
****************************************************************************/
const U32_T  BSP_CANController_a_u32   = CAN0_BASE_ADDRESS;

const U32_T  BSP_CANMode_a_u32  = 0x0012110E;

const U8_T  BSP_CANNumberOfMailbox = 16;

const U8_T  BSP_CANAckLimit_a_u8 = 3;
