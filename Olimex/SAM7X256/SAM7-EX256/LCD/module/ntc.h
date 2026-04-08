/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2005
 *
 *    File name   : ntc.h
 *    Description : NTC module definitions
 *
 *    History :
 *    1. Date        : July 28, 2005
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 10 $
 **************************************************************************/
#ifndef  __NTC_H
#define  __NTC_H

//#include "includes.h"
#include "arm_comm.h"

// NTC device depends parameters
#define A   (Flo64)-14.6337
#define B   (Flo64) 4791.842
#define C   (Flo64)-115334
#define D   (Flo64)-3730535

#define A1  (Flo64)3.354016E-3
#define B1  (Flo64)2.569355E-4
#define C1  (Flo64)2.626311E-6
#define D1  (Flo64)0.675278E-7

Flo32 NtcRatioToTemperature (Flo32 Ratio);
Flo32 NtcTemperatureToRatio (Flo32 Ratio);

#endif  /* __NTC_H */
