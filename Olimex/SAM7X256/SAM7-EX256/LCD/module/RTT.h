/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : RTT.h
 *    Description : Real Time Timer descriptions
 *
 *    History :
 *    1. Date        : Aug 19, 2006
 *       Author      : Todor Atanasov
 *       Description : Created
 *
 *    $Revision: 10 $
 **************************************************************************/
#ifndef  __RTT_H
#define  __RTT_H

#include "arm_comm.h"

void Get_Time(void);

extern unsigned short Hour, Minute, Second;
extern unsigned short Set_Hour, Set_Minute, Set_Second;
extern Int32U Tick, RealTimeClock, TimeLoaded;


// 1 TICK = 1 second
#define TICKS_PER_MIN           60
#define MINUTES_PER_HOUR        60
#define TICKS_PER_HOUR          (Int32U)(TICKS_PER_MIN*MINUTES_PER_HOUR)  // 3600UL
#define HOURS_PER_DAY           24
#define TICKS_PER_DAY           (Int32U)(TICKS_PER_HOUR*HOURS_PER_DAY)    // 86400UL

#endif  /* __RTT_H */
