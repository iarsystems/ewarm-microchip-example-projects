/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : main.c
 *    Description :
 *
 *   This example project shows how to use the IAR Embedded Workbench for ARM to
 * develop code for the SAM7-EX256 evaluation boards.
 * The main purpose of the project is to show how the LCD works. It contains
 * functions for the LCD driver initialization and basic LCD functionality -
 * display an image, symbol, backlight control, contrast adjustment etc. After
 * running the application an image with the IAR logo will appear. Current
 * measured temperature will be displayed as well. The Real Time Timer is reset
 * and we will see a timer started from 00:00:00. We can set this clock by
 * pressing the joystick button. This way time adjust mode is entered. Then we can
 * select which values we want to change - HH:MM:SS by using LEFT and RIGHT
 * joystick positions. After we have placed the marker to the desired position we
 * can change its' value by using UP and DOWN positions. To enter the selected
 * time press Button SW2. For ESCAPE - joystick middle position.
 * In normal mode (not time adjust mode) there is a possibility to adjust the contrast.
 * Press Button SW1 and hold it. Then adjust the contrast by using the TRIM.
 * Pressing Button SW2 will TURN OFF the backlight. To TURN it ON - press button
 * SW1. In time adjust mode by pressing Button SW1 we can change the background
 * image. Currently we can choose between 5 different pictures.
 *
 *
 *    History :
 *    1. Date        : Aug 7, 2006
 *       Author      : Todor Atanasov
 *       Description : Created
 *
 *    $Revision: 10 $
**************************************************************************/
#include "ioat91sam7x256.h"
#include "system.h"
#include "lcd.h"
#include "adc.h"
#include "Temperature.h"
#include "arm_comm.h"
#include "ntc.h"
#include <stdio.h>
#include "project.h"
#include "RTT.h"

AT91PS_PIO    m_pPioA  = AT91C_BASE_PIOA;
AT91PS_PIO    m_pPioB  = AT91C_BASE_PIOB;
AT91PS_PMC    m_pPMC   = AT91C_BASE_PMC;
AT91PS_USART  m_pUSART = AT91C_BASE_US0;
AT91PS_PDC    m_pPDC   = AT91C_BASE_PDC_US0;
AT91PS_MC     m_pMC    = AT91C_BASE_MC;
AT91PS_AIC    m_pAic   = AT91C_BASE_AIC;
AT91PS_SYS    m_pSys   = AT91C_BASE_SYS;
AT91PS_ADC    m_pADC   = AT91C_BASE_ADC;
AT91PS_RSTC   m_pRSTC  = AT91C_BASE_RSTC;

unsigned char Picture;
extern const unsigned char bmp[],bmp_pic2[], bmp_pic3[], bmp_pic4[], bmp_pic7[];
char c=0;

Flo32 CurrentTemp;

// a simple delay
void Delay (unsigned long a) { while (--a!=0); }

int main()
{
  //Enable RESET
  m_pRSTC->RSTC_RCR = 0xA5000008;
  m_pRSTC->RSTC_RMR = 0xA5000001;
  Delay(1000);

  InitFrec();

  // Real Time Timer
  AT91F_RTTSetPrescaler(AT91C_BASE_RTTC,0x8000);
  AT91F_RTTRestart(AT91C_BASE_RTTC);

  InitPeriphery();
  InitLCD();
  LCDSettings();

  WriteSpiCommand(DISPLAYOFF);
  // Load bitmap
  LCDWrite130x130bmp(&bmp[0]);
  WriteSpiCommand(DISPLAYON);

  InitADC();

  AT91F_PIOA_CfgPMC();
  // AT91F_PIO_CfgOutput(AT91C_BASE_PIOA,AT91C_PIO_PA1);

  //*************** MAIN LOOP *********************
  while(1)
  {
    Tick = AT91C_BASE_RTTC->RTTC_RTSR;
    if(Tick&0x0002)
    {
      Get_Time();
      RealTimeClock = AT91C_BASE_RTTC->RTTC_RTVR + TimeLoaded;
    }
    // Calculate the current temperature in Kelvins
    CurrentTemp = NtcRatioToTemperature(((Flo32)MeasureTerm()*K)/1024.0);
    // Convert the temperature from Kelvins to Celsius
    CurrentTemp = TempConverter(CurrentTemp,KELVIN,CELSIUS);
    sprintf(Tempbuf,"%2.1f",CurrentTemp);
    ShowTemp();       // Display the temperature on the LCD
    ShowTime();       // Display the time on the LCD

    // joystick control
    if(!((m_pPioA->PIO_PDSR) & BIT7))         // LEFT
    {
      if(Position > 0)
      {
        Position--;
      }
      else
      {
        Position = 2;
      }
    }
    else if(!((m_pPioA->PIO_PDSR) & BIT8))    // DOWN
    {
      if(SetTime)
      {
        if(Position == 0)
        {
          if(Set_Hour <= 0)
              Set_Hour = 23;
          else
              Set_Hour--;
        }
        else
        {
          if(Position == 1)
          {
            if(Set_Minute <= 0)
                Set_Minute = 59;
            else
                Set_Minute--;
          }
          else
          {
            if(Set_Second <= 0)
                Set_Second = 59;
            else
                Set_Second--;
          }
        }
      }
    }
    else if(!((m_pPioA->PIO_PDSR) & BIT9))    // UP
    {
      if(SetTime)
      {
        if(Position == 0)
        {
          if(Set_Hour >= 23)
              Set_Hour = 0;
          else
              Set_Hour++;
        }
        else
        {
          if(Position == 1)
          {
            if(Set_Minute >= 59)
                Set_Minute = 0;
            else
                Set_Minute++;
          }
          else
          {
            if(Set_Second >= 59)
                Set_Second = 0;
            else
                Set_Second++;
          }
        }
      }
    }
      else if(!((m_pPioA->PIO_PDSR) & BIT14)) // RIGHT
      {
        if(Position > 1)
        {
          Position = 0;
        }
        else
        {
          Position++;
        }
      }
        else if(!((m_pPioA->PIO_PDSR) & BIT15)) // PRESS
        {
          if(SetTime)
          {
              SetTime = 0;
          }
          else
          {
              SetTime = 1;
              Set_Hour = RealTimeClock / TICKS_PER_HOUR;
              Set_Minute = (RealTimeClock % TICKS_PER_HOUR) / TICKS_PER_MIN;
              Set_Second = (RealTimeClock % TICKS_PER_HOUR) % TICKS_PER_MIN;
          }
        }

    // check button SW1
    if(!((m_pPioB->PIO_PDSR) & BIT24))
    {
      if(SetTime)
      {
        switch (Picture)    // Change the background picture
          {
            case 0 :
              LCDWrite130x130bmp(&bmp[0]);
            break;

            case 1 :
              LCDWrite130x130bmp(&bmp_pic2[0]);
            break;

            case 2 :
              LCDWrite130x130bmp(&bmp_pic3[0]);
            break;

            case 3 :
              LCDWrite130x130bmp(&bmp_pic4[0]);
            break;

            case 4 :
              LCDWrite130x130bmp(&bmp_pic7[0]);
            break;

            default:
              LCDWrite130x130bmp(&bmp[0]);
            break;
          }

          Picture++;
          if ( Picture >=5 )
          {
            Picture = 0;
          }
          if(Picture == 1)
          {
            Background_T = 0x01EF;
            Foreground_T = 0x0070;
          }
          else
          {
            Background_T = 0x0FFF;
            Foreground_T = 0x0000;
          }
       }
      else
      {
        Backlight(BKLGHT_LCD_ON);                 // Backlight on
        SetContrast(GetAdcChannel(ADC_CHN_7)/28);  //Adc trim - adjust contrast
      }
    }
    // check button SW2
    if(!((m_pPioB->PIO_PDSR) & BIT25))
    {
      if(SetTime)
      {
        SetTime = 0;
        TimeLoaded = (Set_Hour*TICKS_PER_HOUR + Set_Minute*TICKS_PER_MIN + Set_Second);
        AT91F_RTTRestart(AT91C_BASE_RTTC);
      }
      else
        Backlight(BKLGHT_LCD_OFF);        // Backlight off
    }
  }
}

