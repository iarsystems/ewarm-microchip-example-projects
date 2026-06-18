/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : lcd.c
 *    Description : LCD control definitions
 *
 *    History :
 *    1. Date        : Aug 9, 2006
 *       Author      : Todor Atanasov
 *       Description : Created
 *
 *    $Revision: 10 $
**************************************************************************/

#include "ioat91sam7x256.h"
#include "lcd.h"
#include "bmp.h"
#include "font.h"
#include "RTT.h"
#include "system.h"

AT91PS_PIO    l_pPioA   = AT91C_BASE_PIOA;
AT91PS_PIO    l_pPioB   = AT91C_BASE_PIOB;
AT91PS_SPI    l_pSpi    = AT91C_BASE_SPI0;
AT91PS_PMC    l_pPMC    = AT91C_BASE_PMC;
AT91PS_PDC    l_pPDC    = AT91C_BASE_PDC_SPI0;

#define SPI_SR_TXEMPTY

#define LCD_RESET_LOW     l_pPioA->PIO_CODR   = BIT2
#define LCD_RESET_HIGH    l_pPioA->PIO_SODR   = BIT2

unsigned int i,j;
char Tempbuf[5];
unsigned char SetTime, Position;
unsigned short Background0, Foreground0, Background1, Foreground1, Background2, Foreground2;
unsigned short Background_T = 0x01EF, Foreground_T = 0x0070;

/*************************************************************************
 * Function Name: InitLCD
 * Parameters:    None
 *
 * Return:        None
 *
 * Description:   LCD driver initialization
 *
 *************************************************************************/
void InitLCD(void)
{
  // backlight pins
  //l_pPioB->PIO_CODR   = BIT20;  // Set PB20 to LOW
  l_pPioB->PIO_SODR   = BIT20;    // Set PB20 to HIGH
  l_pPioB->PIO_OER    = BIT20;    // PB20 as output

  // Reset pin
  l_pPioA->PIO_SODR   = BIT2;     // Set PA2 to HIGH
  l_pPioA->PIO_OER    = BIT2;     // PA2 as output

  // Init SPI0
  l_pPioA->PIO_PDR = BIT12 | BIT16 | BIT17 | BIT18;
  l_pPioA->PIO_ASR = BIT12 | BIT16 | BIT17 | BIT18;
  l_pPioA->PIO_BSR = 0;

  //enable the clock of SPI
  l_pPMC->PMC_PCER = 1 << AT91C_ID_SPI0;

  // Fixed mode
  l_pSpi->SPI_CR      = 0x81;         //SPI Enable, Sowtware reset
  l_pSpi->SPI_CR      = 0x01;         //SPI Enable

  l_pSpi->SPI_MR      = 0xE0019;      //Master mode, fixed select, disable decoder, FDIV=1 (MCK), PCS=1110
  l_pSpi->SPI_CSR[0]  = 0x01010F11;   //9bit, CPOL=1, ClockPhase=0, SCLK = 48Mhz/32*15 = 96kHz
}

/*************************************************************************
 * Function Name: WriteSpiCommand
 * Parameters:    Data to be sent to the LCD Driver
 *
 * Return:        None
 *
 * Description:   Sends a commands to the LCD driver
 *
 *************************************************************************/

void WriteSpiCommand(unsigned int data)
{

  data = (data & ~0x0100);

  // Wait for the transmission to complete
  while((l_pSpi->SPI_SR & AT91C_SPI_TXEMPTY) == 0);

  l_pSpi->SPI_TDR = data;
}

/*************************************************************************
 * Function Name: WriteSpiData
 * Parameters:    Data to be sent to the LCD Driver
 *
 * Return:        None
 *
 * Description:   Sends data to the LCD driver
 *
 *************************************************************************/

void WriteSpiData(unsigned int data)
{
  data = (data | 0x0100);

  // Wait for the transmission to complete
  while((l_pSpi->SPI_SR & AT91C_SPI_TXEMPTY) == 0);

  l_pSpi->SPI_TDR = data;
}

/*************************************************************************
 * Function Name: Backlight
 * Parameters:    Desired backlight state - ON or OFF
 *
 *                BKLGHT_LCD_ON
 *                BKLGHT_LCD_OFF
 *
 * Return:        None
 *
 * Description:   Switches the backlight of the LCD 'ON' or 'OFF'
 *
 *************************************************************************/

void Backlight(unsigned char state)
{

  if(state == BKLGHT_LCD_ON)
    l_pPioB->PIO_SODR   = BIT20;    // Set PB20 to HIGH
  else
    l_pPioB->PIO_CODR   = BIT20;    // Set PB20 to LOW

}
/*************************************************************************
 * Function Name: SetContrast
 * Parameters:    contrast
 *
 * Return:        None
 *
 * Description:   Increase or decrease the LCD contrast
 *
 *************************************************************************/

void SetContrast(unsigned char contrast)
{
  WriteSpiCommand(CONTRAST);
  WriteSpiData(0x20+contrast);
}

/*************************************************************************
 * Function Name: LCDSettings
 * Parameters:    None
 *
 * Return:        None
 *
 * Description:   Configuration of the LCD
 *
 *************************************************************************/

void LCDSettings(void)
{
  // Software Reset
  WriteSpiCommand(SOFTRST);
  Delay(2000);

  // Initial escape
  WriteSpiCommand(INITESC);
  Delay(2000);

  // Refresh set
  WriteSpiCommand(REFSET);
  WriteSpiData(0);

  WriteSpiCommand(DISPCTRL);
  WriteSpiData(128);     // Set the lenght of one selection term
  WriteSpiData(128);     // Set N inversion -> no N inversion
  WriteSpiData(134);     // Set frame frequence and bias rate -> 2 devision of frequency and 1/8 bias, 1/67 duty, 96x67 size
  WriteSpiData(84);      // Set duty parameter
  WriteSpiData(69);      // Set duty parameter
  WriteSpiData(82);      // Set duty parameter
  WriteSpiData(67);      // Set duty parameter

  // Grey scale 0 position set - 15 parameters
  WriteSpiCommand(GRAYSCALE0);
  WriteSpiData(1);     // GCP1  - gray lavel to be output when the RAM data is "0001"
  WriteSpiData(2);     // GCP2  - gray lavel to be output when the RAM data is "0010"
  WriteSpiData(4);     // GCP3  - gray lavel to be output when the RAM data is "0011"
  WriteSpiData(8);     // GCP4  - gray lavel to be output when the RAM data is "0100"
  WriteSpiData(16);    // GCP5  - gray lavel to be output when the RAM data is "0101"
  WriteSpiData(30);    // GCP6  - gray lavel to be output when the RAM data is "0110"
  WriteSpiData(40);    // GCP7  - gray lavel to be output when the RAM data is "0111"
  WriteSpiData(50);    // GCP8  - gray lavel to be output when the RAM data is "1000"
  WriteSpiData(60);    // GCP9  - gray lavel to be output when the RAM data is "1001"
  WriteSpiData(70);    // GCP10 - gray lavel to be output when the RAM data is "1010"
  WriteSpiData(80);    // GCP11 - gray lavel to be output when the RAM data is "1011"
  WriteSpiData(90);    // GCP12 - gray lavel to be output when the RAM data is "1100"
  WriteSpiData(100);   // GCP13 - gray lavel to be output when the RAM data is "1101"
  WriteSpiData(110);   // GCP14 - gray lavel to be output when the RAM data is "1110"
  WriteSpiData(127);   // GCP15 - gray lavel to be output when the RAM data is "1111"

  // Gamma curve set - select gray scale - GRAYSCALE 0 or GREYSCALE 1
  WriteSpiCommand(GAMMA);
  WriteSpiData(1);     // Select grey scale 0

  // Command driver output
  WriteSpiCommand(COMMONDRV);
  WriteSpiData(0);     // Set COM1-COM41 side come first, normal mode

  // Set Normal mode (my)
  WriteSpiCommand(NORMALMODE);

  // Inversion off
  // WriteSpiCommand(INVERSIONOFF);

  // Memory access controler
  WriteSpiCommand(ACCESSCTRL);
  WriteSpiData(0x40);     // horizontal
  //WriteSpiData(0x20);   // vertical

  // Power control
  WriteSpiCommand(PWRCTRL);
  WriteSpiData(4);     // Internal resistance, V1OUT -> high power mode, oscilator devision rate

  // Sleep out
  WriteSpiCommand(SLEEPOUT);

  // Voltage control - voltage control and write contrast define LCD electronic volume
  WriteSpiCommand(VOLTCTRL);
  //WriteSpiData(0x7f);    //  full voltage control
  //WriteSpiData(0x03);    //  must be "1"

  // Write contrast
  WriteSpiCommand(CONTRAST);
  WriteSpiData(0x3b);    // contrast

  Delay(2000);

  // Temperature gradient
  WriteSpiCommand(TEMPGRADIENT);
  for(i=0; i<14; i++) {
    WriteSpiData(0);
  }

  // Booster voltage ON
  WriteSpiCommand(BOOSTVON);

  // Finally - Display On
  WriteSpiCommand(DISPLAYON);
}
/*************************************************************************
 * Function Name: LCDWrite130x130bmp
 * Parameters:    Address of the data (the image which has to be displayed)
 *
 * Return:        None
 *
 * Description:   Displays an image on the whole LCD screen
 *
 *************************************************************************/

void LCDWrite130x130bmp(const unsigned char *Addr)
{
  // Display OFF
 // WriteSpiCommand(DISPLAYOFF);

  WriteSpiCommand(COLADDRSET);
  WriteSpiData(0x00);
  WriteSpiData(0x81);

  WriteSpiCommand(PAGEADDRSET);
  WriteSpiData(0x00);
  WriteSpiData(0x81);

  WriteSpiCommand(MEMWRITE);

  for(j=0; j<sizeof(bmp); j++)
  {
    WriteSpiData(Addr[j]);
  }
  // Display On
 // WriteSpiCommand(DISPLAYON);
}

/*************************************************************************
 * Function Name: LCDWriteChar
 * Parameters:    Character which should be displayed - the position in the
 *                font_digits[] table.
 *                horizontal begin pixel position
 *                horizontal end pixel position
 *                vertical begin pixel position
 *                vertical end pixel position
 *                Foreground colour - 12 bits. The most significant nibble is set to '0'
 *                Background colour - 12 bits. The most significant nibble is set to '0'
 *
 * Return:        None
 *
 * Description:   Displays a symbol on the LCD
 *
 *************************************************************************/

void LCDWriteChar(unsigned char Ascii,unsigned char xb, unsigned char xe,unsigned char yb, unsigned char ye,\
                  unsigned short FG_Colour, unsigned short BG_Colour)
{
unsigned char Byte_1,Byte_2,Byte_3, Pixel;

  WriteSpiCommand(COLADDRSET);
  WriteSpiData(xb);
  WriteSpiData(xe);

  WriteSpiCommand(PAGEADDRSET);
  WriteSpiData(yb);
  WriteSpiData(ye);

  WriteSpiCommand(MEMWRITE);

  for(j=0; j<16; j++) {
    Pixel = font_digits[j+(16*Ascii)];
    for(i=0;i<8;i++)
    {
        if(i%2)
        {
            if(Pixel&0x80)      // Pixel ON
            {
                Byte_2 &= 0xF0;
                Byte_2 |= FG_Colour >> 8;
                Byte_3 = FG_Colour&0x00FF;
            }
            else                // Pixel OFF
            {
                Byte_2 &= 0xF0;
                Byte_2 |= BG_Colour >> 8;
                Byte_3 =  BG_Colour&0x00FF;
            }
        WriteSpiData(Byte_1);
        WriteSpiData(Byte_2);
        WriteSpiData(Byte_3);
        }
        else
        {
            if(Pixel&0x80)      // Pixel ON
            {
                Byte_1 = FG_Colour >> 4;
                Byte_2 = FG_Colour << 4;
            }
            else                // Pixel OFF
            {
                Byte_1 = BG_Colour >> 4;
                Byte_2 = 0;
                Byte_2 |= BG_Colour << 4;
            }
        }
        Pixel <<= 1;
    }
  }
}

/*************************************************************************
 * Function Name: ShowTemp
 * Parameters:    None
 *
 * Return:        None
 *
 * Description:   Displays the measured temperature on the LCD
 *
 *************************************************************************/

void ShowTemp(void)
{
  LCDWriteChar('t',0x10,0x17,0x10,0x1f,Foreground_T,Background_T);
  LCDWriteChar((Tempbuf[0]),0x20,0x27,0x10,0x1f,Foreground_T,Background_T);
  LCDWriteChar((Tempbuf[1]),0x28,0x2F,0x10,0x1f,Foreground_T,Background_T);
  LCDWriteChar('.',0x30,0x37,0x10,0x1f,Foreground_T,Background_T);
  LCDWriteChar((Tempbuf[3]),0x38,0x3F,0x10,0x1f,Foreground_T,Background_T);
  LCDWriteChar(0x1F,0x43,0x4A,0x10,0x1f,Foreground_T,Background_T);
  LCDWriteChar('C',0x4B,0x52,0x10,0x1f,Foreground_T,Background_T);
}

/*************************************************************************
 * Function Name: ShowTime
 * Parameters:    None
 *
 * Return:        None
 *
 * Description:   Displays the current time on the LCD
 *
 *************************************************************************/

void ShowTime (void)
{
unsigned short BCD_Hour, BCD_Minute, BCD_Second;

  Background0 = Background_T;
  Foreground0 = Foreground_T;
  Background1 = Background_T;
  Foreground1 = Foreground_T;
  Background2 = Background_T;
  Foreground2 = Foreground_T;

  if(SetTime)
  {
    if(Position == 0)
    {
      Background0 = Foreground_T;
      Foreground0 = Background_T;
    }
    else
    {
      if(Position == 1)
      {
        Background1 = Foreground_T;
        Foreground1 = Background_T;
      }
      else
      {
        if(Position >= 2)
        {
          Background2 = Foreground_T;
          Foreground2 = Background_T;
        }
      }
    }
    BCD_Hour = ((Set_Hour / 10)<<4) | (Set_Hour % 10);
    BCD_Minute = ((Set_Minute / 10)<<4) | (Set_Minute % 10);
    BCD_Second = ((Set_Second / 10)<<4) | (Set_Second % 10);
    LCDWriteChar(((BCD_Hour >> 4) + '0'),0x10,0x17,0x20,0x2f,Foreground0,Background0);
    LCDWriteChar(((BCD_Hour&0x0F) + '0'),0x18,0x1F,0x20,0x2f,Foreground0,Background0);
    LCDWriteChar(':',0x22,0x29,0x20,0x2f,Foreground1,Background1);
    LCDWriteChar(((BCD_Minute >> 4) + '0'),0x28,0x2F,0x20,0x2f,Foreground1,Background1);
    LCDWriteChar(((BCD_Minute&0x0F) + '0'),0x30,0x37,0x20,0x2f,Foreground1,Background1);
    LCDWriteChar(':',0x3A,0x41,0x20,0x2f,Foreground1,Background1);
    LCDWriteChar(((BCD_Second >> 4) + '0'),0x40,0x47,0x20,0x2f,Foreground2,Background2);
    LCDWriteChar(((BCD_Second&0x0F) + '0'),0x48,0x4F,0x20,0x2f,Foreground2,Background2);
  }
  else
  {
    LCDWriteChar(((Hour >> 4) + '0'),0x10,0x17,0x20,0x2f,Foreground0,Background0);
    LCDWriteChar(((Hour&0x0F) + '0'),0x18,0x1F,0x20,0x2f,Foreground0,Background0);
    LCDWriteChar(':',0x22,0x29,0x20,0x2f,Foreground1,Background1);
    LCDWriteChar(((Minute >> 4) + '0'),0x28,0x2F,0x20,0x2f,Foreground1,Background1);
    LCDWriteChar(((Minute&0x0F) + '0'),0x30,0x37,0x20,0x2f,Foreground1,Background1);
    LCDWriteChar(':',0x3A,0x41,0x20,0x2f,Foreground1,Background1);
    LCDWriteChar(((Second >> 4) + '0'),0x40,0x47,0x20,0x2f,Foreground2,Background2);
    LCDWriteChar(((Second&0x0F) + '0'),0x48,0x4F,0x20,0x2f,Foreground2,Background2);
  }
}
