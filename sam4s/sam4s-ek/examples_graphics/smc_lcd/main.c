/* ----------------------------------------------------------------------------
 *         SAM Software Package License
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
 * This package can be used with SAM4S-EK.
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
 *    <a href="http://www.atmel.com/dyn/resources/prod_documents/6421B.pdf">
 *    SAM-BA User Guide</a>, the
 *    <a href="http://www.atmel.com/dyn/resources/prod_documents/doc6310.pdf">
 *    GNU-Based Software Development</a> application note or to the
 *    <a href="http://www.iar.com/website1/1.0.1.0/78/1/">
 *    IAR EWARM User and reference guides</a>, depending on your chosen solution.
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
 * - lcd_draw.c
 * - ili9325.c
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

#include <stdint.h>
#include <stdio.h>

/*----------------------------------------------------------------------------
 *        Local definitions
 *----------------------------------------------------------------------------*/

/** Test image height. */
#define IMAGE_HEIGHT    50
/** Test image width. */
#define IMAGE_WIDTH     100
/** Number of color block in horizontal direction of test image */
#define N_BLK_HOR       5
/** Number of color block in vertical direction of test image */
#define N_BLK_VERT      4

/*----------------------------------------------------------------------------
 *        Local variables
 *----------------------------------------------------------------------------*/

/** Image buffer. */
uint8_t gImageBuffer[IMAGE_HEIGHT * IMAGE_WIDTH * 3];

/** Color pattern for make image. */
const uint32_t gColorPattern[N_BLK_HOR*N_BLK_VERT] = {
    COLOR_BLACK,    COLOR_YELLOW, COLOR_RED,         COLOR_GREEN,  COLOR_BLUE,
    COLOR_CYAN,     COLOR_WHITE,  COLOR_INDIGO,      COLOR_OLIVE,  COLOR_BROWN,
    COLOR_GRAY,     COLOR_SIENNA, COLOR_GREENYELLOW, COLOR_SILVER, COLOR_VIOLET,
    COLOR_DARKBLUE, COLOR_ORANGE, COLOR_DARKGREEN,   COLOR_TOMATO, COLOR_GOLD,
};

/*----------------------------------------------------------------------------
 *        Local functions
 *----------------------------------------------------------------------------*/

/**
 * \brief Make a test Image with special pattern.
 *
 * \param pBuffer Image buffer.
 *
 * \note The image is a raw data of RGB 24-bit format.
 */
static void MakeTestImage( uint8_t *pBuffer )
{
    uint32_t v_max  = IMAGE_HEIGHT;
    uint32_t h_max  = IMAGE_WIDTH;
    uint32_t v_step = (v_max + N_BLK_VERT - 1) / N_BLK_VERT;
    uint32_t h_step = (h_max + N_BLK_HOR  - 1) / N_BLK_HOR;
    uint32_t v, h;
    uint8_t  *pImage = (uint8_t *)pBuffer;
    uint8_t ix ;
    uint8_t iy ;

    for ( v = 0; v < v_max; v++ )
    {
        iy = v / v_step ;

        for ( h = 0 ; h < h_max ; h++ )
        {
            ix = N_BLK_HOR * iy + (h / h_step);
            *pImage++ = (gColorPattern[ix] & 0x0000FF);
            *pImage++ = (gColorPattern[ix] & 0x00FF00) >> 8;
            *pImage++ = (gColorPattern[ix] & 0xFF0000) >> 16;
        }
    }
}

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
        printf( "-F- Systick configuration error\n\r" ) ;
    }

    /*=================== Test LCD ===================*/

    /* Initialize LCD */
    LCDD_Initialize() ;
    LCDD_Fill( COLOR_WHITE ) ;

    /* Turn on LCD */
    LCDD_On() ;

    /* Draw text, image and basic shapes on the LCD */
    LCDD_DrawString( 30, 20, (uint8_t *)"smc_lcd example", COLOR_BLACK ) ;

    MakeTestImage(gImageBuffer);
    LCDD_DrawImage(60, 60, (const uint8_t *)gImageBuffer, IMAGE_WIDTH, IMAGE_HEIGHT);

    LCDD_DrawCircle(60,  160, 40, COLOR_RED);
    LCDD_DrawCircle(120, 160, 40, COLOR_GREEN);
    LCDD_DrawCircle(180, 160, 40, COLOR_BLUE);

    LCDD_DrawRectangle(20, 220, 200, 80, COLOR_VIOLET);
    LCDD_DrawLine(10,  260, 220, DIRECTION_HLINE, COLOR_CYAN);
    LCDD_DrawLine(120, 210, 100, DIRECTION_VLINE, COLOR_ORANGE);

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

