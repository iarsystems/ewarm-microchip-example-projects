/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2005
 *
 *    File name   : ntc.c
 *    Description : NTC termistor module
 *
 *    History :
 *    1. Date        : July 28, 2005
 *       Author      : Stanimir Bonev
 *       Description : Created
 *
 *    $Revision: 10 $
**************************************************************************/
#include "ntc.h"
#include <math.h>
/*************************************************************************
 * Function Name: NtcRatioToTemperature
 * Parameters: Flo32 Ratio
 *
 * Return: Flo32
 *
 * Description: Calculate temperature by Rt to Rref ratio
 *
 *************************************************************************/
Flo32 NtcRatioToTemperature (Flo32 Ratio)
{
Flo64 Temp,TempLn;
  if(Ratio == 0)
  {
    return(0);
  }
  TempLn = logl(((Flo64)1 - Ratio)/Ratio);
  Temp = A1 + TempLn*B1 + TempLn*TempLn*C1+TempLn*TempLn*TempLn*D1;
  return((Flo32)((Flo64)1/Temp));
}
/*************************************************************************
 * Function Name: NtcTemperatureToRatio
 * Parameters: Flo32 Ratio
 *
 * Return: Flo32
 *
 * Description: Calculate the ratio from temperature
 *
 *************************************************************************/
Flo32 NtcTemperatureToRatio (Flo32 T)
{
Flo64 Temp;

Flo32 Temp10;
  Temp = exp(A + B/T + C/(T*T) + D/(T*T*T));

  Temp10 = (Flo32)((Flo64)1/(Temp+1));

 // return((Flo32)((Flo64)1/(Temp+1)));
  return ((Flo32)Temp10);
}
