/****************************************************************************
FILE        BSP_TIME.H - (hardware dependent)

VERSION     1.00

AUTHORS     Patrice Vilchez (Europe Technologies) 
            Frederic SAMSON (Europe Technologies)

COPYRIGHT   (c) Europe Technologies
            COMMERCIAL IN CONFIDENCE

CONTAINS    Structure and Enum Definitions and Standard Typedef 
            for Time Package of EasyCan3 

 MODIFICATION HISTORY

 * 1  29/06/2000  Patrice Vilchez
        - Initial File 
 * 2  19/07/2001  Frederic SAMSON
        - Bios_time.h of VECU2002 modified for  EasyCan3 bsp layer
****************************************************************************/

#ifndef BSP_TIME_H
#define BSP_TIME_H

/************************************/
/* BSP Time Structure Definiton    */
/************************************/
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
