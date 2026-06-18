/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : RTT.c
 *    Description : Real Time Timer handler
 *
 *    History :
 *    1. Date        : Aug 19, 2006
 *       Author      : Todor Atanasov
 *       Description : Created
 *
 *    $Revision: 10 $
 **************************************************************************/
#include "RTT.h"

Int32U Tick, RealTimeClock, TimeLoaded;

unsigned short Hour, Minute, Second;
unsigned short Set_Hour, Set_Minute, Set_Second;

/*************************************************************************
 * Function Name: Get_Time
 * Parameters:    None
 *
 * Return:        The current time in BCD format
 * Description:   Converts the RealTimeClock 32bit value into the current time
 *
 *************************************************************************/

 void Get_Time(void)
  {
    unsigned char Time_Result;
    unsigned int Time;

    Time = RealTimeClock;

    Time %= TICKS_PER_DAY;

    Time_Result = Time / TICKS_PER_HOUR;
    Hour = ((Time_Result / 10)<< 4) | (Time_Result % 10);     // Convert to BCD
    Time_Result = (Time % TICKS_PER_HOUR) / TICKS_PER_MIN;
    Minute = ((Time_Result / 10) << 4 ) | (Time_Result % 10); // Convert to BCD
    Time_Result = (Time % TICKS_PER_HOUR) % TICKS_PER_MIN;
    Second = ((Time_Result / 10) << 4) | (Time_Result % 10);  // Convert to BCD
  }

