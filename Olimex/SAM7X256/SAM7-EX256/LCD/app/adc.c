/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : adc.c
 *    Description : Analog to Digital Converter
 *
 *    History :
 *    1. Date        : Aug 11, 2006
 *       Author      : Todor Atanasov
 *       Description : Created
 *
 *    $Revision: 10 $
**************************************************************************/
#include "adc.h"
#include "Temperature.h"
#include "ioat91sam7x256.h"

AT91PS_ADC a_pADC = AT91C_BASE_ADC;
AT91PS_PMC a_pPMC = AT91C_BASE_PMC;

/*************************************************************************
 * Function Name: InitADC
 * Parameters:    None
 *
 * Return:        None
 *
 * Description:   ADC module initialization
 *
 *************************************************************************/
void InitADC(void) {

  // Enable the interface clock
  a_pPMC->PMC_PCER = 1 << AT91C_ID_ADC;

  // Reset
  a_pADC->ADC_CR = 0x1;
  a_pADC->ADC_CR = 0x0;

  // Set maximum startup time and hold time
  a_pADC->ADC_MR = 0x0F1F0F00;

  // Buffer initialization
  for(int i = 16; i ; --i)
  {
    MeasureTerm();
  }
}
/*************************************************************************
 * Function Name: GetAdcChannel
 * Parameters:    The desired channel
 *
 * Return:        ADC result
 *
 * Description: Starts an A/D conversion from a desired channel and returns
 *              the result.
 *
 *************************************************************************/
unsigned int GetAdcChannel(unsigned char channel)
{
  // variable
  unsigned int result;

  // Enable desired channel
  a_pADC->ADC_CHER = channel;

  // Start conversion
  a_pADC->ADC_CR = 0x2;

  // wait for convertion to complete
  while(!(a_pADC->ADC_SR&channel));

  switch (channel)
  {
    case ADC_CHN_1:
        result = a_pADC->ADC_CDR0;
    break;
    case ADC_CHN_2:
        result = a_pADC->ADC_CDR1;
    break;
    case ADC_CHN_3:
        result = a_pADC->ADC_CDR2;
    break;
    case ADC_CHN_4:
        result = a_pADC->ADC_CDR3;
    break;
    case ADC_CHN_5:
        result = a_pADC->ADC_CDR4;
    break;
    case ADC_CHN_6:
        result = a_pADC->ADC_CDR5;
    break;
    case ADC_CHN_7:
        result = a_pADC->ADC_CDR6;
    break;
    case ADC_CHN_8:
        result = a_pADC->ADC_CDR7;
    break;
  }

  return result;
}
