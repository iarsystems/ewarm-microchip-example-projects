/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : temperature.c
 *    Description : Temperature measurement functions
 *
 *    History :
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
#include "Temperature.h"
#include "arm_comm.h"
#include "adc.h"

Int32U DataBuf[16];
Int32U Result;
Int8U  DataInd;

Flo32 K = 1.0;
Flo32 Temperature;

/*************************************************************************
 * Function Name: TempConverter
 * Parameters:    Flo32 Temperature,
 *                tTemperatureUnits CurrUnit ,
 *                tTemperatureUnits Unit
 * Return:        Float
 * Description: Convert the temperature from "Current Units" to " Desired Units"
 *
 *************************************************************************/
Flo32 TempConverter(Flo32 Temperature,tTemperatureUnits CurrUnit , tTemperatureUnits Unit)
{
  if(CurrUnit != Unit)
  {
    switch(CurrUnit)
    {
    case KELVIN:
      if(Unit == CELSIUS)
      {
        Temperature += KELVIN_TO_CELSIUS_OFFSET;
      }
      else
      {
        Temperature *= GRADUS_TO_FAHRENHEIT_GRADUS;
        Temperature += KELVIN_TO_FAHRENHEIT_OFFSET;
      }
      break;
    case CELSIUS:
      if(Unit == KELVIN)
      {
        Temperature -= KELVIN_TO_CELSIUS_OFFSET;
      }
      else
      {
        Temperature *= GRADUS_TO_FAHRENHEIT_GRADUS;
        Temperature += CELSIUS_TO_FAHRENHEIT_OFFSET;
      }
      break;
    case FAHRENHEIT:
      if(Unit == KELVIN)
      {
        Temperature -= KELVIN_TO_FAHRENHEIT_OFFSET;
        Temperature /= GRADUS_TO_FAHRENHEIT_GRADUS;
      }
      else
      {
        Temperature -= CELSIUS_TO_FAHRENHEIT_OFFSET;
        Temperature /= GRADUS_TO_FAHRENHEIT_GRADUS;
      }
      break;
    }
  }
  return(Temperature);
}
/*************************************************************************
 * Function Name: MeasureTerm
 * Parameters: none
 *
 * Return: Int32U
 *
 * Description: Measures the NTC channel and filtering the result
 *
 *************************************************************************/
Int32U MeasureTerm (void)
{
Int32U * pData = &DataBuf[DataInd++&0xF];

  Result -= *pData;
  *pData = GetAdcChannel(ADC_CHN_6);
  Result += *pData;
  return(Result >> 4);
}
