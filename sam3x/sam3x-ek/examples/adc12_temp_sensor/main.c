/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support
 * ----------------------------------------------------------------------------
 * Copyright (c) 2010, Atmel Corporation
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
 *  \page adc12_temp_sensor ADC12 Temperature Sensor Example
 *
 *  \section Purpose
 *
 *  The adc12_temp_sensor example demonstrates how to use the temperature sensor
 *  feature inside the microcontroller.
 *
 *  \section Requirements
 *
 *  This package can be used with SAM3X-EK.
 *
 *  \section Description
 *
 *  The adc12_temp_sensor is aimed to demonstrate the temperature sensor feature
 *  inside the device. To use this feature, the temperature sensor should be
 *  turned on by setting TSON bit in ADC_ACR. The channel 15 is connected to the
 *  sensor by default. With PDC support, the Interrupt Handler of ADC is designed
 *  to handle RXBUFF interrupt.
 *
 *  The temperature sensor provides an output voltage (VT) that is proportional
 *  to absolute temperature (PTAT). The relationship between measured voltage and
 *  actual temperature could be found in Electrical Characteristics part of the
 *  datasheet.
 *
 *  \section Usage
 *
 *  -# Build the program and download it inside the evaluation board. Please
 *     refer to the
 *     <a href="http://www.atmel.com/dyn/resources/prod_documents/doc6224.pdf">
 *     SAM-BA User Guide</a>, the
 *     <a href="http://www.atmel.com/dyn/resources/prod_documents/doc6310.pdf">
 *     GNU-Based Software Development</a>
 *     application note or to the
 *     <a href="ftp://ftp.iar.se/WWWfiles/arm/Guides/EWARM_UserGuide.ENU.pdf">
 *     IAR EWARM User Guide</a>,
 *     depending on your chosen solution.
 *  -# On the computer, open and configure a terminal application
 *     (e.g. HyperTerminal on Microsoft Windows) with these settings:
 *    - 115200 bauds
 *    - 8 bits of data
 *    - No parity
 *    - 1 stop bit
 *    - No flow control
 *  -# In the terminal window, the
 *     following text should appear (values depend on the board and chip used):
 *     \code
 *      -- ADC12 Temperature Sensor xxx --
 *      -- xxxxxx-xx
 *      -- Compiled: xxx xx xxxx xx:xx:xx --
 *     \endcode
 *  -# The application will output current temperature on the terminal, and
 *     user can set temperature offset by a menu.
 *
 *  \section References
 *  - adc12_temp_sensor/main.c
 *  - adc.h
 */

/** \file
 *
 *  This file contains all the specific code for the adc12_temp_sensor.
 */

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

#include "board.h"

#include <string.h>

/*----------------------------------------------------------------------------
 *        Local definitions
 *----------------------------------------------------------------------------*/

/** ADC channel for temperature */
#define ADC_CHANNEL_TEMPERATURE    ADC_CHANNEL_15

/** Size of the receive buffer and transmit buffer. */
#define BUFFER_SIZE     (10)

/** Reference voltage for ADC, in mv */
#define VOLT_REF        (3300)

/** The maximal digital value */
#define MAX_DIGITAL     (4095)

/** The end of string */
#define STRING_EOL    "\r"

/** The header of stirng */
#define STRING_HEADER "-- ADC12 Temperature Sensor Example "SOFTPACK_VERSION" --\r\n" \
                      "-- "BOARD_NAME" --\r\n" \
                      "-- Compiled: "__DATE__" "__TIME__" --"STRING_EOL
                      
/** The  degree of char*/
#define CHAR_DEGREE   (248)

/*----------------------------------------------------------------------------
 *        Local variables
 *----------------------------------------------------------------------------*/

/** ADC buffer */
static uint16_t _awAdcValue[BUFFER_SIZE] = { 0 } ;

/** ADC conversion done flag */
static uint32_t _dwConversionDone = 0 ;

/** Temperature offset value */
static int32_t _lTemperatureOffset = 0 ;

/** Timestamp */
static uint32_t _dwTimestamp = 0 ;

/*----------------------------------------------------------------------------
 *        Local functions
 *----------------------------------------------------------------------------*/

/**
 * \brief Calculate temperatrue by sample value and display it on terminal.
 */
static void _CalcTemperature( void )
{
    uint32_t dw ;
    uint32_t dwAdcValue = 0 ;
    uint32_t dwVolX10 ;
    int32_t dwTempX10 ;

    /* Calculate average sample value */
    for ( dw = 0 ; dw < BUFFER_SIZE ; dw++ )
    {
        dwAdcValue += _awAdcValue[dw] ;
    }
    dwAdcValue = dwAdcValue / BUFFER_SIZE ;

    /*
     * The VT voltage equals 0.8V at 27 ¡æ with a ¡À15% accuracy.
     * The VT output voltage linearly varies with a temperature slope dVT/dT = 2.65.
     * So the formula is: x = (vol - 800) / 2.65 + 27.
     */
    /* We ultiply voltage value by 10 for better accuracy */
    dwVolX10 = dwAdcValue * VOLT_REF * 10 / MAX_DIGITAL ;
    dwTempX10 = (int32_t)(dwVolX10 - 8000) * 100 / 265 + 270 ;
    /* Adjust temperature by offset */
    dwTempX10 += _lTemperatureOffset * 10 ;

    /* Display the temperature */
    printf( "Current temperature: %d.%01d%c    \r", (int)(dwTempX10 / 10), (int)(absv(dwTempX10) % 10), CHAR_DEGREE ) ;
}

/**
 * \brief Display the menu on the terminal.
 */
static void _DisplayMenu( void )
{
    printf( "\n\r==== Menu====\n\r"\
            "Press 's' to set offset temperature.\n\r"\
            "Press 'h' to show this menu.\n\r"\
            "The temperature offset is %d%c.\n\r", (int)_lTemperatureOffset, CHAR_DEGREE ) ;
    puts( "========"STRING_EOL ) ;
}

/*----------------------------------------------------------------------------
 *        Exported functions
 *----------------------------------------------------------------------------*/

/**
 * Systick handler, start new conversion
 */
extern void SysTick_Handler( void )
{
    _dwTimestamp++ ;

    /* Trigger adc conversion per 100ms */
    if ( _dwTimestamp % 100 == 0 )
    {
        /* Start conversion */
        ADC_StartConversion( ADC ) ;
    }
}

/**
 * \brief Interrupt handler for the ADC.
 */
extern void ADC_IrqHandler( void )
{
    uint32_t dwStatus ;

    dwStatus = ADC_GetStatus( ADC ) ;

    if ( (dwStatus & ADC_ISR_RXBUFF) == ADC_ISR_RXBUFF )
    {
        _dwConversionDone = 1 ;
    }

    ADC_DisableIt( ADC, ADC_ISR_RXBUFF ) ;
}

/**
 *  \brief adc12_temp_sensor Application entry point.
 *
 *  \return Unused (ANSI-C compatibility).
 */
extern int main( void )
{
    uint8_t ucKey ;
    int32_t dwValue ;
    uint32_t dwAdcClock ;

    /* Disable watchdog */
    WDT_Disable( WDT ) ;

    /* Output example information */
    puts( STRING_HEADER ) ;

    /* Set 1 ms timer */
    SysTick_Config( BOARD_MCK / (1000) ) ;

    /* Initialize ADC */
    ADC_Initialize( ADC, ID_ADC ) ;

    /*
     * Formula: ADCClock = MCK / ( (PRESCAL+1) * 2 )
     * For example, MCK = 84MHZ, PRESCAL = 6, then:
     *     ADCClock = 84 / ((6+1) * 2) = 6MHz;
     */
    /* Set ADC clock */
    dwAdcClock = ADC_SetClock( ADC, 6, BOARD_MCK ) ;
    printf( "Set ADC clock to %d Hz\n\r", (int)dwAdcClock ) ;

    /* Formula:
     *     Startup  Time = startup value / ADCClock
     *     Transfer Time = (TRANSFER * 2 + 3) / ADCClock
     *     Tracking Time = (TRACKTIM + 1) / ADCClock
     *     Settling Time = settling value / ADCClock
     * For example, ADC clock = 6MHz (166.7 ns)
     *     Startup time = 512 / 6MHz = 85.3 us
     *     Transfer Time = (1 * 2 + 3) / 6MHz = 833.3 ns
     *     Tracking Time = (0 + 1) / 6MHz = 166.7 ns
     *     Settling Time = 3 / 6MHz = 500 ns
     */
    /* Set ADC timing */
    ADC_SetTiming( ADC, ADC_MR_STARTUP_SUT512, ADC_MR_TRANSFER(1), ADC_MR_TRACKTIM(0), ADC_MR_SETTLING_AST17 ) ;

    /* Enable  channel for temperature sensor */
    ADC_EnableChannel( ADC, ADC_CHANNEL_TEMPERATURE ) ;

    /* Enable the temperature sensor */
    ADC_EnableTS( ADC ) ;

    /* Enable ADC interrupt */
    NVIC_EnableIRQ( ADC_IRQn ) ;

    /* Start pdc transfer */
    ADC_ReadBuffer( ADC, _awAdcValue, BUFFER_SIZE ) ;
    /* Enable PDC channel interrupt */
    ADC_EnableIt( ADC, ADC_ISR_RXBUFF ) ;
    /* Start conversion */
    ADC_StartConversion( ADC ) ;

    _DisplayMenu() ;

    while ( 1 )
    {
        /* Check if ADC conversion is done */
        if ( _dwConversionDone )
        {
            /* Calculate temperature and display it */
            _CalcTemperature() ;
            _dwConversionDone = 0 ;

            /* Start pdc transfer */
            ADC_ReadBuffer( ADC, _awAdcValue, BUFFER_SIZE ) ;
            /* Enable PDC channel interrupt */
            ADC_EnableIt( ADC, ADC_ISR_RXBUFF ) ;
        }

        /* Check if user enter a key */
        if ( UART_IsRxReady() )
        {
            ucKey = UART_GetChar() ;
            switch ( ucKey )
            {
                /* Set offset temperature */
                case 's':
                case 'S':
                    printf( "\n\rPlease input an integer for temperature offset: " ) ;

                    if ( UART_GetInteger( &dwValue ) == 1 )
                    {
                        _lTemperatureOffset = dwValue ;
                        printf( "Set the temperature offset to %d%c.\n\r", _lTemperatureOffset, CHAR_DEGREE ) ;
                    }
                break ;

                /* Show user menu */
                case 'h' :
                case 'H' :
                    _DisplayMenu() ;
                break;

                /* Do nothing */
                default :
                break ;
            }
        }
    }
}

