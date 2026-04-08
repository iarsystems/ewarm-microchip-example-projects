/****************************************************************************
FILE        BSP_CAN_CONF.H - (hardware dependent)

VERSION     1.00

AUTHORS     Patrice Vilchez (Europe Technologies)
            David Combe     (Europe Technologies)

COPYRIGHT   (c) Europe Technologies
            COMMERCIAL IN CONFIDENCE

CONTAINS    Declarations for Specific Bsp (CAN) 
            Configuration Data For Mack

 MODIFICATION HISTORY

 * 1  02/10/00  Patrice Vilchez / David Combe
                - Initial Document
****************************************************************************/


/***************************************************************************/
/* Bsp CAN IO Enum Definition                                               */
/***************************************************************************/
typedef enum {CAN0_IO, CAN1_IO, CAN2_IO, BSP_NB_CAN_IO} BSP_CAN_IO_E ;


/***************************************************************************
* BSP CAN Controller Configuration Data
****************************************************************************/
extern const U32_T  BSP_CANController_a_u32[BSP_NB_CAN_IO];
extern const U32_T  BSP_CANMode_a_u32[BSP_NB_CAN_IO];
extern const U8_T   BSP_CANNumberOfMailbox[BSP_NB_CAN_IO];
extern const U8_T   BSP_CANAckLimit_a_u8[BSP_NB_CAN_IO]; 

