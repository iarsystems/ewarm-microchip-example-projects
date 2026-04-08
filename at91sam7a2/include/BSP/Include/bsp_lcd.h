/*-----------------------------------------------------------------------------
*   EUROPE TECHNOLOGIES Software Support
*------------------------------------------------------------------------------
* The software is delivered "AS IS" without warranty or condition of any
* kind, either express, implied or statutory. This includes without
* limitation any warranty or condition with respect to merchantability or
* fitness for any particular purpose, or against the infringements of
* intellectual property rights of others.
*------------------------------------------------------------------------------
*
* Processor   : easyCAN3
* File Name   : bsp_lcd.h
* Description : Definitions and Standard Typedef for Liquid Crystal Display
* Version     : 1.01
*
*       +----- (NEW | MODify | ADD | DELete)                                 
*       |                                                                    
*  No   |   when       who                  what               
*-----+---+----------+------------------+--------------------------------------
* 000  NEW  09/09/99   Patrice VILCHEZ      Creation
* 001  MOD  01/04/01   Olivier MAZUYER      Clean up
* 002  ADD  23/07/01   Frederic SAMSON      Complete LCD columns definition
*                                           Add CGRAM Patterns definition 
* 003  MOD  05/02/02   Mahmoud MESGARZADEH  Clean up
*----------------------------------------------------------------------------*/


#ifndef BSP_LCD_H
#define BSP_LCD_H



/******************************************************************************
* LCD PIO definitions
******************************************************************************/
  /* Lcd Control Signals */
  #define LCD_E            PIO8
  #define LCD_RS           PIO9
  #define LCD_RW           PIO10

  /* Lcd Data Bus Interface */
  #define LCD_DB0          PIO0
  #define LCD_DB1          PIO1
  #define LCD_DB2          PIO2
  #define LCD_DB3          PIO3
  #define LCD_DB4          PIO4
  #define LCD_DB5          PIO5
  #define LCD_DB6          PIO6
  #define LCD_DB7          PIO7
  #define LCD_BUS          (PIO0 | PIO1 | PIO2 | PIO3 | PIO4 | PIO5 | PIO6 | PIO7)

  /* Lcd Mask (Bus) */
  #define LCD_PIO_CONTROL   (LCD_RS | LCD_RW | LCD_E)

/******************************************************************************
*  LCD Definitions    
******************************************************************************/
/* LCD Row & LENGTH */
#define LCD_ROW_LENGTH        16    /* It's a 16 char length display         */
#define LCD_NO_ROWS           2     /* The LCD has 2 rows                    */

/* LCD row and column definitions */
#define LCD_COLUMN_0          0
#define LCD_COLUMN_1          1
#define LCD_COLUMN_2          2
#define LCD_COLUMN_3          3
#define LCD_COLUMN_4          4
#define LCD_COLUMN_5          5
#define LCD_COLUMN_6          6
#define LCD_COLUMN_7          7
#define LCD_COLUMN_8          8
#define LCD_COLUMN_9          9
#define LCD_COLUMN_10         10
#define LCD_COLUMN_11         11
#define LCD_COLUMN_12         12
#define LCD_COLUMN_13         13
#define LCD_COLUMN_14         14
#define LCD_COLUMN_15         15
#define LCD_ROW_0             0
#define LCD_ROW_1             1

/* LCD Char display characters */
#define ALL_DOTS              0xFF
#define BLANK_CHAR            0x20

/* LCD Commands */
#define LCD_FUNCTION_SET      0x38    /* Data length 8 bits, 2 lines, 5x7 dots  */
#define LCD_DISPLAY_ON        0x0C    /* Display on , cursor off not blinking    */
#define LCD_ENTRY_MODE        0x06    /* DDRAM increment mode , no display shift */
#define LCD_CLEAR_DISPLAY     0x01    /* Clears the display                      */

#define LCD_DDRAM_ADDRESS     0x80    /* Mask for set DDRAM address command      */
#define LCD_ROW_OFFSET        0x40
#define CG_RAM_ADDRESS        0x40 

/* Lcd Section: RS value */
#define LCD_CONTROL           0
#define LCD_DATA              1

/******************************************************************************
* BSP LCD User Patterns (CG RAM Character) Enum Definition
******************************************************************************/
typedef enum {ARROW_UP, ARROW_DOWN, NUMB_OF_CHAR_CGRAM} BSP_LCD_USER_PATTERN_E ;

extern U8_T CGRAM[NUMB_OF_CHAR_CGRAM*8] ;

/******************************************************************************
* External Functions
******************************************************************************/
extern void BSP_LCDInit(void);
extern void BSP_LCDBusy(void);
extern void BSP_LCDWrite(U8_T data, U8_T section);
extern void BSP_LCDDisplayMessage(U8_T row, U8_T column, char *message);
extern void BSP_LCDCustomCharDefinition(void);
extern void BSP_LCDClear(void);
extern void BSP_LCDDisplayChar(U8_T row, U8_T column, U8_T message);
extern U8_T BSP_LCDRead(U8_T row, U8_T column, U8_T section);
extern void BSP_LCDConvertIntASCII(char *string, U32_T data_u32);
extern void BSP_LCDConvertHexASCII(char *string, U32_T data_u32);

#endif /* BSP_LCD_H */
