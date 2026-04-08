/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : lcd.h
 *    Description : LCD control definitions
 *
 *    History :
 *    1. Date        : Aug 9, 2006
 *       Author      : Todor Atanasov
 *       Description : Created
 *
 *    $Revision: 10 $
**************************************************************************/

#ifndef  __LCD_H
#define  __LCD_H

#define BKLGHT_LCD_ON        1
#define BKLGHT_LCD_OFF       2


// COMMANDS
#define NOP           0x00     // nop
#define SOFTRST       0x01     // software reset
#define BOOSTVOFF     0x02     // booster voltage OFF
#define BOOSTVON      0x03     // booster voltage ON
#define TESTMODE1     0x04     // test mode
#define DISPSTATUS    0x09     // display status
#define SLEEPIN       0x10     // sleep in
#define SLEEPOUT      0x11     // sleep out
#define PARTIAL       0x12     // partial display mode
#define NORMALMODE    0x13     // display normal mode
#define INVERSIONOFF  0x20     // inversion OFF
#define INVERSIONON   0x21     // inversion ON
#define ALLPIXELOFF   0x22     // all pixel OFF
#define ALLPIXELON    0x23     // all pixel ON
#define CONTRAST      0x25     // write contrast
#define DISPLAYOFF    0x28     // display OFF
#define DISPLAYON     0x29     // display ON
#define COLADDRSET    0x2A     // column address set
#define PAGEADDRSET   0x2B     // page address set
#define MEMWRITE      0x2C     // memory write
#define COLORSET      0x2D     // colour set
#define READRAMDATA   0x2E     // RAM data read
#define PARTIALAREA   0x30     // partial area
#define VERTSCROLL    0x33     // vertical scrolling definition
#define TESTMODE2     0x34     // test mode
#define TESTMODE3     0x35     // test mode
#define ACCESSCTRL    0x36     // memory access control
#define VSCRLSADDR    0x37     // vertical scrolling start address
#define IDLEOFF       0x38     // idle mode OFF
#define IDLEON        0x39     // idle mode ON
#define PIXELFORMAT   0x3A     // interface pixel format
#define TESTMODE4     0xDE     // test mode
#define NOP2          0xAA     // nop
#define INITESC       0xC6     // initial escape
#define TESTMODE5     0xDA     // test mode
#define TESTMODE6     0xDB     // test mode
#define TESTMODE7     0xDC     // test mode
#define TESTMODE8     0xB2     // test mode
#define GRAYSCALE0    0xB3     // gray scale position set 0
#define GRAYSCALE1    0xB4     // gray scale position set 1
#define GAMMA         0xB5     // gamma curve set
#define DISPCTRL      0xB6     // display control
#define TEMPGRADIENT  0xB7     // temp gradient set
#define TESTMODE9     0xB8     // test mode
#define REFSET        0xB9     // refresh set
#define VOLTCTRL      0xBA     // voltage control
#define COMMONDRV     0xBD     // common driver output select
#define PWRCTRL       0xBE     // power control

extern char Tempbuf[5];
extern unsigned char SetTime, Position;
extern unsigned short Background_T, Foreground_T;


void InitLCD(void);
void Backlight(unsigned char state);

void WriteSpiCommand(unsigned int data);
void WriteSpiData(unsigned int data);

void LCDSettings(void);
void LCDWrite130x130bmp(const unsigned char *Addr);

void SetContrast(unsigned char contrast);
void ShowTemp(void);
void ShowTime (void);

void LCDWriteChar(unsigned char Ascii,unsigned char xb, unsigned char xe,unsigned char yb, unsigned char ye, unsigned short FG_Colour, unsigned short BG_Colour);

#endif  /* __LCD_H */

