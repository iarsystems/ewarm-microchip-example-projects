/****************************************************************************
FILE        BSP_PWM.H - (hardware dependent)

VERSION     1.00

AUTHORS     Patrice Vilchez (Europe Technologies) 

COPYRIGHT   (c) Europe Technologies
            COMMERCIAL IN CONFIDENCE

CONTAINS    Definitions and Standard Typedef for VECU2 BIOS
            PWM Pin. Board Support Package Layer 

MODIFICATION HISTORY

 * 1  29/06/2000  Patrice Vilchez
        - Initial File 
****************************************************************************/

#ifndef BSP_PWM4C_H
#define BSP_PWM4C_H

/****************************************************************************
*  External Function Declaration
****************************************************************************/
extern void BSP_PWM4CSetOuput(CSP_PWM4C_T *pwm4c, U8_T channel_u8, U32_T, U16_T);

#endif  /* BSP_PWM4C_H */
