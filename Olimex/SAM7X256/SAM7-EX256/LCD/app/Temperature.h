/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2005
 *
 *    File name   : temperature.h
 *    Description : temperature measurement definitions
 *
 *    History :
 *
 *    1. Date        : July 25, 2005
 *       Author      : Stanimir Bonev
 *       Description : Created
 *
 *    2. Date        : Aug. 14, 2006
 *       Author      : Todor Atanasov
 *       Description : Changed
 *
 *    $Revision: 10 $
 **************************************************************************/
#ifndef __TEMPERATURE_H
#define __TEMPERATURE_H
//#include "includes.h"

#include "arm_comm.h"


extern Flo32 K;
extern Flo32 Temperature;

#define KELVIN_TO_CELSIUS_OFFSET      (Flo32)(-273.15)
#define KELVIN_TO_FAHRENHEIT_OFFSET   (Flo32)(-459.67)
#define CELSIUS_TO_FAHRENHEIT_OFFSET  (Flo32)(32)
#define GRADUS_TO_FAHRENHEIT_GRADUS   (Flo32)(1.8)

typedef enum
{
  KELVIN = 0,CELSIUS,FAHRENHEIT
} tTemperatureUnits;

Flo32 TempConverter(Flo32 Temperature,tTemperatureUnits CurrUnit , tTemperatureUnits Unit);

Int32U MeasureTerm (void);

#endif    /* __TEMPERATURE_H */
