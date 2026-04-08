/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : adc.h
 *    Description : Analog to Digital Converter definitions
 *
 *    History :
 *    1. Date        : Aug 11, 2006
 *       Author      : Todor Atanasov
 *       Description : Created
 *
 *    $Revision: 10 $
**************************************************************************/
#ifndef  __ADC_H
#define  __ADC_H

#define ADC_CHN_1   0x01
#define ADC_CHN_2   0x02
#define ADC_CHN_3   0x04
#define ADC_CHN_4   0x08
#define ADC_CHN_5   0x10
#define ADC_CHN_6   0x20
#define ADC_CHN_7   0x40
#define ADC_CHN_8   0x80

void InitADC(void);
unsigned int GetAdcChannel(unsigned char channel);

#endif  /* __ADC_H */
