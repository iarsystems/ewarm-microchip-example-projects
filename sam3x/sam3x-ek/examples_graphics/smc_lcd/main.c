/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support
 * ----------------------------------------------------------------------------
 * Copyright (c) 2011, Atmel Corporation
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Atmel's name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ----------------------------------------------------------------------------
 */

/**
 * \page smc_lcd SMC LCD Example
 *
 * \section Purpose
 *
 * This example demonstrates how to configure the Static Memory Controller (SMC)
 * to use the LCD on the board and also shows how to use SPI to control
 * touchscreen controller (ADS7843).
 *
 * \section Requirements
 *
 * This package can be used with SAM3X evaluation kits.
 *
 * \section Description
 *
 * This example is divided into two part:
 * - LCD test: It first configure SMC for access the LCD controller,
 * then initialize the LCD, finally draw some text, image, basic shapes (line,
 * rectangle, circle) on LCD.
 * - Touchscreen test: It then initialize touchscreen controller, then let
 * user do calibration for the touchscreen. After calibration is done, user may
 * touch the LCD and the pen position will be output on terminal.
 *
 * \section Usage
 *
 * -# Build the program and download it inside the evaluation board. Please
 *    refer to the
 *    <a href="http://www.atmel.com/dyn/resources/prod_documents/doc6224.pdf">
 *    SAM-BA User Guide</a>, the
 *    <a href="http://www.atmel.com/dyn/resources/prod_documents/doc6310.pdf">
 *    GNU-Based Software Development</a> application note or to the
 *    <a href="ftp://ftp.iar.se/WWWfiles/arm/Guides/EWARM_UserGuide.ENU.pdf">
 *    IAR EWARM User Guide</a>, depending on your chosen solution.
 * -# On the computer, open and configure a terminal application
 *    (e.g. HyperTerminal on Microsoft Windows) with these settings:
 *   - 115200 bauds
 *   - 8 bits of data
 *   - No parity
 *   - 1 stop bit
 *   - No flow control
 * -# Start the application.
 * -# In the terminal window, the following text should appear:
 *    \code
 *     -- SMC_LCD Example xxx --
 *     -- xxxxxx-xx
 *     -- Compiled: xxx xx xxxx xx:xx:xx --
 *     Press any key to continue touchscreen test.
 *    \endcode
 * -# Some text, image and basic shapes should be displayed on the LCD.
 * -# Press any key in the terminal application to continue touchscreen test.
 * -# Touch the dots on the LCD to calibrate the touchscreen, the calibration
 *    results will output on the terminal and LCD.
 * -# Touch the LCD, the pen position will output on the terminal
 *    if touchscreen is calibrated ok.
 *
 * \section References
 * - smc_lcd/main.c
 * - lcd_color.h
 * - lcdd.c
 * - lcd_font.c
 * - hx8347.c
 */

/**
 * \file
 *
 * This file contains all the specific code for the smc_lcd example.
 *
 */

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/
#include "board.h"
#include "image.h"

#include <stdint.h>
#include <stdio.h>

/*----------------------------------------------------------------------------
 *        Local definitions
 *----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------
 *        Local variables
 *----------------------------------------------------------------------------*/

/** Image buffer (16-bits color). */
const uint16_t gImageBuffer[DEMO_IMAGE_HEIGHT * DEMO_IMAGE_WIDTH] = DEMO_IMAGE;

/*----------------------------------------------------------------------------
 *         Global functions
 *----------------------------------------------------------------------------*/

/**
 * \brief Handler for SysTick interrupt. Increments the timestamp counter.
 */
void SysTick_Handler( void )
{
    TimeTick_Increment() ;


    /* Call TSD_TimerHandler per 10ms */
    if ( (GetTickCount() % 10) == 0 )
    {
        TSD_TimerHandler() ;
    }
}

/**
 * \brief Callback called when the pen is pressed on the touchscreen.
 *
 * \param dwX horizontal position (in pixel if ts calibrated).
 * \param dwY vertical position (in pixel if ts calibrated).
 */
void TSD_PenPressed( uint32_t dwX, uint32_t dwY )
{
    printf( "Pen pressed at  (%03u, %03u)\n\r", (unsigned int)dwX, (unsigned int)dwY ) ;
}

/**
 * \brief Callback called when the pen is moved on the touchscreen.
 *
 * \param dwX horizontal position (in pixel if ts calibrated).
 * \param dwY vertical position (in pixel if ts calibrated).
 */
void TSD_PenMoved( uint32_t dwX, uint32_t dwY )
{
    printf("Pen moved at    (%03u, %03u)\n\r", (unsigned int)dwX, (unsigned int)dwY);
}

/**
 * \brief Callback called when the touchscreen is released on the touchscreen.
 *
 * \param dwX horizontal position (in pixel if ts calibrated).
 * \param dwY vertical position (in pixel if ts calibrated).
 */
void TSD_PenReleased( uint32_t dwX, uint32_t dwY )
{
    printf("Pen released at (%03u, %03u)\n\r", (unsigned int)dwX, (unsigned int)dwY);
}

/**
 * \brief Application entry point.
 *
 * \return Unused (ANSI-C compatibility).
 */
extern int main( void )
{
    uint8_t ucResult ;

    /* Disable watchdog */
    WDT_Disable( WDT ) ;

    /* Output example information */
    printf("-- SMC_LCD Example %s --\n\r", SOFTPACK_VERSION);
    printf("-- %s\n\r", BOARD_NAME);
    printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);

    /* Configure systick for 1 ms. */
    if ( TimeTick_Configure( BOARD_MCK ) != 0 )
    {
        printf( "-F- Systick configuration error\n\r" );
    }

    /*=================== Test LCD ===================*/

    /* Initialize LCD */
    LCD_Initialize() ;

    LCD_SetBacklight(32);

    LCD_On();

    /* Test basic color space translation and LCD_DrawFilledRectangle */
    LCD_SetColor(COLOR_WHITE);
    LCD_DrawFilledRectangle(0, 0, BOARD_LCD_WIDTH-1, BOARD_LCD_HEIGHT-1);

    LCD_SetColor(COLOR_BLACK);
    LCD_DrawFilledRectangle(BOARD_LCD_WIDTH-5, BOARD_LCD_HEIGHT-5, 4, 4);

    LCD_SetColor(COLOR_BLUE);
    LCD_DrawFilledRectangle(8, 8, BOARD_LCD_WIDTH-9, BOARD_LCD_HEIGHT-9);

    LCD_SetColor(COLOR_RED);
    LCD_DrawFilledRectangle(12, 12, BOARD_LCD_WIDTH-13, BOARD_LCD_HEIGHT-13);

    LCD_SetColor(COLOR_GREEN);
    LCD_DrawFilledRectangle(16, 14, BOARD_LCD_WIDTH-17, BOARD_LCD_HEIGHT-17);

    LCD_SetColor(COLOR_RED);

    /* Test horizontal/vertial LCD_drawLine  */
    LCD_DrawLine(0, BOARD_LCD_HEIGHT/2, BOARD_LCD_WIDTH-1, BOARD_LCD_HEIGHT/2);
    LCD_DrawLine(BOARD_LCD_WIDTH/2, 0, BOARD_LCD_WIDTH/2, BOARD_LCD_HEIGHT-1);

    /* Test Bresenham LCD_drawLine  */
    LCD_DrawLine(0, 0, BOARD_LCD_WIDTH-1, BOARD_LCD_HEIGHT-1);
    LCD_DrawLine(0, BOARD_LCD_HEIGHT-1, BOARD_LCD_WIDTH-1, 0);

    /* Test LCD_DrawRectangle */
    LCD_DrawRectangle(BOARD_LCD_WIDTH/4, BOARD_LCD_HEIGHT/4, BOARD_LCD_WIDTH*3/4, BOARD_LCD_HEIGHT*3/4);
    LCD_DrawRectangle(BOARD_LCD_WIDTH*2/3, BOARD_LCD_HEIGHT*2/3, BOARD_LCD_WIDTH/3, BOARD_LCD_HEIGHT/3);

    /* Test LCD_DrawFilledCircle */
    LCD_SetColor(COLOR_BLUE);
    LCD_DrawFilledCircle(BOARD_LCD_WIDTH*3/4, BOARD_LCD_HEIGHT*3/4, BOARD_LCD_WIDTH/4);
    LCD_DrawCircle(BOARD_LCD_WIDTH/4, BOARD_LCD_HEIGHT/4, BOARD_LCD_HEIGHT/4);
    LCD_SetColor(COLOR_YELLOW);
    LCD_DrawFilledCircle(BOARD_LCD_WIDTH/4, BOARD_LCD_HEIGHT*3/4, BOARD_LCD_HEIGHT/4);
    LCD_DrawCircle(BOARD_LCD_WIDTH*3/4, BOARD_LCD_HEIGHT/4, BOARD_LCD_WIDTH/4);

    /* Test LCD_DrawPicture */
    LCD_DrawPicture(0, 0, DEMO_IMAGE_WIDTH-1, DEMO_IMAGE_HEIGHT-1, gImageBuffer );

    /*=================== Test Touchscreen ===================*/

    printf( "Press any key to continue touchscreen test.\n\r" ) ;
    UART_GetChar() ;

    /* Initializes the PIO interrupt management for touchscreen driver */
    PIO_InitializeInterrupts( 0 ) ;

    /* Initialize touchscreen without calibration */
    TSD_Initialize( 0 ) ;

    /* Touchscreen calibration */
    while ( 1 )
    {
        printf( "-I- Touchscreen calibration...\n\r" ) ;
        ucResult = TSD_Calibrate() ;
        if ( ucResult )
        {
            printf( "-I- Calibration successful !\n\r" ) ;
            break ;
        }
        else
        {
            printf( "-E- Error too big ! Retry...\n\r" ) ;
        }
    }

    /* Infinite loop */
    while ( 1 ) ;
}

