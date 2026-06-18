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
* Processor   : easyCAN
* File Name   : bsp_lcd.c
* Description : Function declarations for LCD for the BSP
* Version     : 1.01
*
*       +----- (NEW | MODify | ADD | DELete)                                 
*       |                                                                    
*  No   |   when       who                what               
*-----+---+----------+------------------+--------------------------------------
* 000  NEW  06/12/99   Patrice VILCHEZ    Creation
* 001  MOD  01/04/01   Olivier MAZUYER    Clean up
* 002  MOD  23/07/01   Frederic SAMSON    - Add CGRAM Pattern definition
*                                         - Create BSP_LCDCustomCharDefinition() 
*                                         - Modify BSP_LCDDisplayChar()
*                                         - Add BSP_LCDCustomCharDefinition() 
*                                           to BSP_LCDInit()
* 003  ADD             Olivier MAZUYER    Add BSP_LCDConvertIntASCII function
* 004  MOD  07/08/2001 Frederic SAMSON    Corrections in BSP_LCDConvertIntASCII
*                                         to take account of 0
* 005  MOD  31/01/02   Mahmoud Mesgarzadeh   Clean Up
*----------------------------------------------------------------------------*/


/******************************************************************************
* Includes 
******************************************************************************/
#include "csp.h"
#include "bsp.h"

/* Define to select the VDD_PERIPH voltage :                                 */
/* VDD_PERIPH_3V ==> VDD_PERIPH = 3.3V                                       */
/* VDD_PERIPH_5V ==> VDD_PERIPH = 5.0V                                       */
/* This define is used by the contrast voltage generator (see BSP_LCDInit()  */
#define VDD_PERIPH_3V
//#define VDD_PERIPH_5V

/******************************************************************************
* Custom Characters Definition 
******************************************************************************/
U8_T CGRAM[NUMB_OF_CHAR_CGRAM*8]=
{
   /* ARROW UP */
   /* Line 0, Line 1, Line 2, Line 3, Line 4, Line 5, Line 6, Line 7*/
   0x04, 0x0E, 0x15, 0x04, 0x04, 0x04, 0x04, 0x04, 
   /* ARROW DOWN */
   0x04, 0x04, 0x04, 0x04, 0x04, 0x15, 0x0E, 0x04   
};


/******************************************************************************
Function          : BSP_LCDInit
Description       : This function initializes the LCD mode: it enables the UPIO 
                    module clock, defines UPIO pins connected to the LCD as 
                    output and then write the LCD sequence initialization 
- data length 8 bits, 
- 2 lines, 5x7 dots 
- set the display on and the cursor off not blinking 
- clears the display 
- set the entry mode: DDRAM increment mode and no display shift). 
- User patterns are defined in the LCD CGRAM memory.
Input             : None
Functions called  : 创BSP_LCDWrite创, 创BSP_LCDCustomCharDefinition创, 创CSP_PIO_SET_ECR创
                    创CSP_PIO_SET_OER创
Returns           : None
******************************************************************************/
void BSP_LCDInit(void)
{

   /* Set the contrast voltage with the PWM channel 1 */
   /* frequency = 32768 Hz, dutycycle=23/256 = 9% */
#ifdef VDD_PERIPH_3V
   BSP_LCDSetContrast( 32768, 23);
#endif

#ifdef VDD_PERIPH_5V
   BSP_LCDSetContrast( 32768, 3); 
#endif

   /* Enable the PIO clock */
   CSP_PIO_SET_ECR(UPIO, PIO);

   /* Define Lcd pio as output (Default) */
   CSP_PIO_SET_OER(UPIO, (LCD_RW | LCD_E | LCD_RS | LCD_BUS));
   
   /* Write Lcd sequence initialization */
   BSP_LCDWrite(LCD_FUNCTION_SET,  LCD_CONTROL);
   BSP_LCDWrite(LCD_DISPLAY_ON, LCD_CONTROL);
   BSP_LCDWrite(LCD_CLEAR_DISPLAY, LCD_CONTROL);
   BSP_LCDWrite(LCD_ENTRY_MODE, LCD_CONTROL);

   /* Define User Pattern in CGRAM */
   BSP_LCDCustomCharDefinition();
}


/******************************************************************************
Function          : BSP_LCDClear
Description       : This function clears the LCD display
Input             : None
Functions called  : 创BSP_LCDWrite创
Returns           : None
******************************************************************************/
void BSP_LCDClear(void)
{
    BSP_LCDWrite(LCD_CLEAR_DISPLAY, LCD_CONTROL);
    BSP_LCDWrite(LCD_ENTRY_MODE, LCD_CONTROL);
}


/******************************************************************************
Function          : BSP_LCDBusy
Description       : This function polls the busy bit of the LCD. 
                    While the LCD is busy, the function reads the busy 
                    flag which indicating internal operation performed. 
                    It is a waiting function.
Input             : None
Functions called  : 创CSP_PIO_SET_ODR创, 创CSP_PIOClear创, 创CSP_PIOSet创, 
                    创CSP_PIOGetStatus创
Returns           : None
******************************************************************************/
void BSP_LCDBusy(void)
{
  /* Local Variable */
  U32_T data = 0x80;
  
  /* Set LCD Bus as input */
     CSP_PIO_SET_ODR(UPIO, LCD_BUS);

  /* Clear RS */
     CSP_PIOClear(UPIO, LCD_RS);
     
     while( (data & 0x80) == 0x80 )
     {
     /* Set RW to 1 (Read) */
     CSP_PIOSet(UPIO, LCD_RW);
        

     /********************************************************************/  
     /* Be careful if internal chip clock is faster than 40MHz:          */
     /* The delay between rising edge for RW and rising edge for enable  */
     /* need to be at least of 100ns (4 assembler instructions at 40MHz) */
     /********************************************************************/  

     /* Enable LCD */
        CSP_PIOSet(UPIO, LCD_E);
  
     /* Read Data */
        data = CSP_PIOGetStatus(UPIO);
  
     /* Disable LCD */
        CSP_PIOClear(UPIO, LCD_E);
     }
}


/******************************************************************************
Function          : BSP_LCDRead
Description       : Read data on LCD 
Input             : 
- row : row (from 0 to 1)
- column :	(from 0 to 15)
- section : RS value (control or data)
Functions called  : 创BSP_LCDWrite创, 创BSP_LCDBusy创, 创CSP_PIO_SET_ODR创,
                    创CSP_PIO_SET_SODR创, 创CSP_PIO_SET_CODR创, 创CSP_PIOGetStatus创
Returns           : data read
******************************************************************************/
U8_T BSP_LCDRead(U8_T row, U8_T column, U8_T section)
{
  /* Local Variable */
     U32_T pio_val;
     U8_T  lcd_parameter;

  /* Calculate LCD start address for string. */
  /* Start of first row is address 0x00, start of second row is 0x40 */
     if(row == 0)
       lcd_parameter = 0x80 + column;
     else
       lcd_parameter = 0xC0 + column;
   
  /* Set DDRAM address in LCD */
     BSP_LCDWrite(lcd_parameter, LCD_CONTROL); 

  /* Wait Lcd Busy */
     BSP_LCDBusy();

  /* Set LCD Bus as input */
     CSP_PIO_SET_ODR(UPIO, LCD_BUS);

  /* Set RS (Control or Data section) */
     if( section == LCD_DATA)
       CSP_PIO_SET_SODR(UPIO, LCD_RS);
     else
       CSP_PIO_SET_CODR(UPIO, LCD_RS);

  /* Set RW to 1 (Read) */
     CSP_PIO_SET_SODR(UPIO, LCD_RW);
  
  /* Enable LCD */
     CSP_PIO_SET_SODR(UPIO, LCD_E);
  
  /* Read Data */
     pio_val = CSP_PIOGetStatus(UPIO);
  
  /* Disable LCD */
     CSP_PIO_SET_CODR(UPIO, LCD_E);
  
  /* Return Value */
     return(pio_val);
}


/******************************************************************************
Function          : BSP_LCDWrite
Description       : Write a data on LCD 
Input             : 
- data : data 8 bits to be written
- section (control or data)
Functions called  : 创BSP_LCDBusy创, 创CSP_PIO_SET_OER创, 创CSP_PIO_SET_SODR创,
                    创CSP_PIO_SET_CODR创, 
Returns           : None
******************************************************************************/
void  BSP_LCDWrite(U8_T data, U8_T section)
{
  /* Wait Lcd Not Busy */
     BSP_LCDBusy();
  
  /* Set LCD Bus as output */
     CSP_PIO_SET_OER(UPIO, LCD_BUS);
  
  /* Check Section to be Written */
     if(section == LCD_DATA)
       CSP_PIO_SET_SODR(UPIO, LCD_RS);
     else
     CSP_PIO_SET_CODR(UPIO, LCD_RS);
  
  /* Set LCD_RW Low Level 0 (Write) */
     CSP_PIO_SET_CODR(UPIO, LCD_RW);

  /* Set Lcd Enable */
     CSP_PIO_SET_SODR(UPIO, LCD_E); 
  
  /* Write Data */
     CSP_PIO_SET_SODR(UPIO, data); 
  
  /* Clear LCD_E */
     CSP_PIO_SET_CODR(UPIO, LCD_E);
  
  /* Set LCD_RW Hight Level 1 (Read) */
     CSP_PIO_SET_SODR(UPIO, LCD_RW);
  
  /* Clear the data */
     CSP_PIO_SET_CODR(UPIO, (LCD_BUS));
} 


/******************************************************************************
Function          : BSP_LCDDisplayMessage
Description       : Displays a message on the LCD at the given position.
Input             : 
- row : row number for start of message
- column : column number for start of message
- message : pointer to character string to be displayed
Functions called  : 创BSP_LCDWrite创
Returns           : None
******************************************************************************/
void BSP_LCDDisplayMessage(U8_T row, U8_T column, char *message)
{
  U8_T lcd_parameter;

  /* Calculate LCD start address for string. */
  /* Start of first row is address 0x00, start of second row is 0x40 */
     if(row == 0)
       lcd_parameter = 0x80 + column;
     else
       lcd_parameter = 0xC0 + column;
  
  /* Set DDRAM address in LCD */
     BSP_LCDWrite(lcd_parameter, LCD_CONTROL); 

  /* Write string to LCD */
     while( *message != '\0' )
     {    
     /* Display the current character */
        BSP_LCDWrite((U8_T)*message, LCD_DATA);

     /* Get Next Char */
        message++;
     }
}

/******************************************************************************
Function          : BSP_LCDDisplayChar
Description       : Displays a 8 bit character on the LCD at the given position.
Input             : 
- row : row number for start of message
- column : column number for start of message
- message : Character Code in DDRAM (DDRAM data)
Functions called  : 创BSP_LCDWrite创
Returns           : None
******************************************************************************/
void BSP_LCDDisplayChar(U8_T row, U8_T column, U8_T message)
{
  U8_T lcd_parameter;

   /* Calculate LCD start address for string. */
   /* Start of first row is address 0x00, start of second row is 0x40 */
   lcd_parameter = (U8_T) (row * LCD_ROW_OFFSET) ;
   lcd_parameter += column;
  
   /* Set Display Address */
   lcd_parameter |= LCD_DDRAM_ADDRESS ;
  
   /* Set DDRAM address in LCD */
   BSP_LCDWrite(lcd_parameter, LCD_CONTROL); 

   /* Display the current character */
   BSP_LCDWrite(message, LCD_DATA);

}



/******************************************************************************
Function          : BSP_LCDCustomCharDefinition
Description       : Init CGRAM data with User Patterns
Input             : None
Functions called  : 创BSP_LCDWrite创
Returns           : None
******************************************************************************/
void BSP_LCDCustomCharDefinition(void)
{
   U8_T lcd_parameter;
   U8_T i_u8;
  
   /* Set Display Address */
   lcd_parameter = CG_RAM_ADDRESS ;

   /* Set CGRAM address in LCD */
   BSP_LCDWrite(lcd_parameter, LCD_CONTROL); 

   /* Write string to CGRAM */
   for (i_u8=0; i_u8 < (NUMB_OF_CHAR_CGRAM*8); i_u8++)
   {    
      /* Display the current character */
      BSP_LCDWrite(CGRAM[i_u8], LCD_DATA);
   }
}


/******************************************************************************
Function          : BSP_LCDConvertIntASCII
Description       : Convert Integer into a string in decimal format
Input             : 
- *string : pointer on the string receiving the ASCII conversion
- data_u32 : integer to be converted
Functions called  : None
Returns           : None
******************************************************************************/
void BSP_LCDConvertIntASCII(char *string, U32_T data_u32)
{
   /* Local variable */
   char buff[16] ;
   char offset = 0;
   char length = 0;

   /* Loop to convert integer into a sring coded in ASCII */
   do 
   {
      /* Convert the last digit of integer freq to ASCII value */
      buff[length++] = (char)((data_u32 %10) + '0');
   } while ((data_u32/=10) > 0);

   
   /* Loop to replace the string in order */   
   do 
   {
      length-- ;
      /* The first character of string receive the last character of buffer */
      string[offset++] =buff[length] ;
   } while(length>0);

   string[offset] = '\0';   
}

/******************************************************************************
Function          : BSP_LCDConvertHexASCII
Description       : Convert integer into a string in hexadecimal format
Input             : 
- *string : pointer on the string receiving the ASCII conversion
- data_u32 : integer to be converted
Functions called  : None
Returns           : None
******************************************************************************/
void BSP_LCDConvertHexASCII(char *string, U32_T data_u32)
{
   /* Local variable */
   char buff[16] ;
   char offset = 0;
   char length = 0;
   U32_T val   = 0 ;

   /* Loop to convert integer into a sring coded in ASCII */
   do 
   {
      val = (data_u32 %16) ; 
      /* Convert the last digit of integer freq to ASCII value */
      if (val>9)
      {
         buff[length++] =(char)('A' + (val-10)) ;
      }
      else 
      { 
         buff[length++] =(char)( val + '0');
      }
   } while ((data_u32/=16) > 0);

   
   /* Loop to replace the string in order */   
   do 
   {
      length-- ;
      /* The first character of string receive the last character of buffer */
      string[offset++] = buff[length] ;
   } while(length>0);

   string[offset] = '\0';   
}


/*******************************************************************************
Function    : BSP_LCDSetContrast
Description : Enable the PWM channel1 to control the LCD contrast thanks to
              a pump charge which generates a negative voltage
Input       : 
- Frequency_u32 = PWM channel 1 frequency (Hz)
- DutyCycle_u16 = dutycycle (0 to 256).             
Returns     : None
Fct called  : 创BSP_PWM4CSetOuput创
*******************************************************************************/
void BSP_LCDSetContrast( U32_T Frequency_u32, U16_T DutyCycle_u16)
{
   BSP_PWM4CSetOuput(PWM0, 1, Frequency_u32*256, DutyCycle_u16);
}
