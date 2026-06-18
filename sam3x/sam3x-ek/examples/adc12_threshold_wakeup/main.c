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
 *  \page adc12_threshold_wakeup ADC12 Threshold Wakeup Example
 *
 *  \section Purpose
 *
 *  The adc12_threshold_wakeup example demonstrates how to use ADC with
 *  threshold wakeup.
 *
 *  \section Requirements
 *
 *  This package can be used with SAM3X-EK. To enable full scale measurement
 *  of the potentiometer by default configuration. Close jumper JP4 on 1 and 2,
 *  the outer pins of it.
 *
 *  \section Description
 *  This example uses TIOA0 as external trigger instead of software trigger for
 *  ADC conversion. The TIOA0 is a 1ms period square wave. The rising edge
 *  during each period would trigger the ADC to start a conversion on given channel
 *  which is connected to the potentiometer. This example shows a menu as below
 *  upon running,
 *  \code
 *  -- Menu Choices for this example--
 *  -- 0: Display voltage on potentiometer.--
 *  -- 1: Display thresholds.--
 *  -- 2: Modify low threshold.--
 *  -- 3: Modify high threshold.--
 *  -- 4: Choose comparison mode.--
 *  -- i: Display ADC information.--
 *  -- m: Display this main menu.--
 *  -- s: Enter sleep mode.--
 *  \endcode
 *  With the user interface, comparison window and mode could be set. The ADC
 *  supports 4 kinds of comparison events as following:
 *
 *  - lower than the lower threshold.
 *  - higher than the high threshold.
 *  - in the comparison window.
 *  - out of the comparison window.
 *
 *  If the target got an 'S' or 's' from user's input,the core fell in sleep
 *  by __WFI.Tuning the potentiometer to enable the ADC input fall into the
 *  comparison window and then generate a trigger event. The comparison event
 *  will wake the system up.
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
 *      -- ADC12 Threshold Wakeup Example xxx --
 *      -- xxxxxx-xx
 *      -- Compiled: xxx xx xxxx xx:xx:xx --
 *      -- Menu Choices for this example--
 *      -- 0: Display voltage on potentiometer.--
 *      -- 1: Display thresholds.--
 *      -- 2: Modify low threshold.--
 *      -- 3: Modify high threshold.--
 *      -- 4: Choose comparison mode.--
 *      -- i: Display ADC information.--
 *      -- m: Display this main menu.--
 *      -- s: Enter sleep mode.--
 *     \endcode
 *  -# Input the command according to the menu.
 *
 *  \section References
 *  - adc12_threshold_wakeup/main.c
 *  - adc.h
 *  - tc.h
 */

/** \file
 *
 *  This file contains all the specific code for the adc12_threshold_wakeup.
 */

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

#include "board.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>

/*----------------------------------------------------------------------------
 *        Local definitions
 *----------------------------------------------------------------------------*/

/** ADC channel for potentiometer */
#define ADC_CHANNEL_POTENTIOMETER    ADC_CHANNEL_1

/** Reference voltage for ADC,in mv*/
#define VOLT_REF   (3300)

/** The maximal digital value*/
#define MAX_DIGITAL (4095)

/*----------------------------------------------------------------------------
 *        Local variables
 *----------------------------------------------------------------------------*/

/** low threshold */
uint16_t gwLowThreshold = 0;
/** high threshold */
uint16_t gwHighThreshold = 0;

/*----------------------------------------------------------------------------
 *        Local functions
 *----------------------------------------------------------------------------*/

/**
 *  \brief TC0 configuration
 *
 * Configures Timer Counter 0 (TC0) to generate an interrupt every second.
 */

static void _ConfigureTc0( void )
{
    /* Enable TC0 peripheral clock*/
    PMC_EnablePeripheral( ID_TC0 ) ;

    /* Configure TC for a 1s (= 1Hz) tick*/
    TC_Configure( TC0, 0, 0x4 | TC_CMR_ACPC_SET | TC_CMR_WAVE | TC_CMR_ACPA_CLEAR | (0x2 << 13) ) ;

    /* 50% duty, 1s frequency */
    TC0->TC_CHANNEL[0].TC_RA = 16384 ;
    TC0->TC_CHANNEL[0].TC_RC = 32768 ;
}

/**
 * \brief Display main menu
 */
static void _DisplayMenuChoices( void )
{
    printf("-- Menu Choices for this example--\n\r");
    printf("-- 0: Display voltage on potentiometer.--\n\r");
    printf("-- 1: Display thresholds.--\n\r");
    printf("-- 2: Modify low threshold.--\n\r");
    printf("-- 3: Modify high threshold.--\n\r");
    printf("-- 4: Choose comparison mode.--\n\r");
    printf("-- i: Display ADC information.--\n\r");
    printf("-- m: Display this main menu.--\n\r");
    printf("-- s: Enter sleep mode.--\n\r");
}

/**
 * \brief Display current information,including voltage on potentiometer,
 * thresholds and comparison mode.
 */
static void _DisplayInfo( void )
{
    uint32_t dwAdcValue = ADC_GetConvertedData( ADC, ADC_CHANNEL_POTENTIOMETER ) ;

    printf( "-I- Thresholds: %d mv - %d mv.\n\r", gwLowThreshold * VOLT_REF / MAX_DIGITAL, gwHighThreshold * VOLT_REF / MAX_DIGITAL ) ;
    printf( "-I- Voltage on potentiometer: %u mv.\n\r", (unsigned int)(dwAdcValue * VOLT_REF / MAX_DIGITAL) ) ;
    printf( "-I- Comparison mode is %u.\n\r", (unsigned int)(ADC->ADC_EMR & ADC_EMR_CMPMODE_Msk) ) ;
}

/** 
 * \breif Fall asleep by __WFI
 * Enable interrupt first, and disable it after wake up
 */
static void _Fallasleep( void )
{
    while ( 1 )
    {
        printf( "The device is going to fall in sleep!\n\r" ) ;
        /* clear status register*/
        ADC_GetStatus( ADC ) ;

        /* Enable Compare Interrupt*/
        ADC_EnableIt( ADC, ADC_IER_COMPE ) ;

        __WFI() ;

        /* every time waked-up ,break out of the loop*/
        break ;
    }
}

/**
 * Get comparison mode
 */
static uint8_t GetComparisonMode( void )
{
    uint8_t dwMode = (ADC->ADC_EMR & ADC_EMR_CMPMODE_Msk) ;
    uint8_t ucKey ;

    while ( 1 )
    {
        ucKey = UART_GetChar() ;
        switch ( ucKey )
        {
            case 'a':
            case 'A':
                dwMode = 0x0;
                break;
            case 'b':
            case 'B':
                dwMode = 0x1;
                break;
            case 'c':
            case 'C':
                dwMode = 0x2;
                break;
            case 'd':
            case 'D':
                dwMode = 0x3;
                break;
            case 'q':
            case 'Q':
                break;
            default:
                continue;
        }
        return dwMode ;
    }
}

/*----------------------------------------------------------------------------
 *        Exported functions
 *----------------------------------------------------------------------------*/

/**
 * \brief Interrupt handler for the ADC.
 */
void ADC_IrqHandler( void )
{
    uint32_t dwStatus, dwMode ;
    uint32_t dwAdcValue ;

    /* Disable Compare Interrupt*/
    ADC_DisableIt( ADC, ADC_IDR_COMPE ) ;

    dwStatus = ADC_GetStatus( ADC ) ;

    if ( (dwStatus & ADC_ISR_COMPE) == ADC_ISR_COMPE )
    {
        dwMode = ADC_GetCompareMode( ADC ) ;
        dwAdcValue = ADC_GetConvertedData( ADC, ADC_CHANNEL_POTENTIOMETER ) ;

        switch ( dwMode )
        {
            case 0:
                printf("-ISR-:Potentiometer voltage %d mv is below the low "
                       "threshold:%d mv!\n\r", (int)(dwAdcValue * VOLT_REF / MAX_DIGITAL),
                       gwLowThreshold * VOLT_REF / MAX_DIGITAL);
            break;

            case 1:
                printf("-ISR-:Potentiometer voltage %d mv is above the high "
                       "threshold:%d mv!\n\r", (int)(dwAdcValue * VOLT_REF / MAX_DIGITAL),
                       gwHighThreshold * VOLT_REF / MAX_DIGITAL);
            break;

            case 2:
                printf("-ISR-:Potentiometer voltage %d mv is in the comparison "
                       "window:%d mv-%d mv!\n\r", (int)(dwAdcValue * VOLT_REF / MAX_DIGITAL),
                      gwLowThreshold * VOLT_REF / MAX_DIGITAL, gwHighThreshold * VOLT_REF / MAX_DIGITAL);
            break;

            case 3:
                printf("-ISR-:Potentiometer voltage %d mv is out of the comparison"
                       " window:%d mv-%d mv!\n\r", (int)(dwAdcValue * VOLT_REF / MAX_DIGITAL),
                       gwLowThreshold * VOLT_REF / MAX_DIGITAL, gwHighThreshold * VOLT_REF / MAX_DIGITAL);
            break;
        }
    }
}

/**
 *  \brief adc12_threshold_wakeup Application entry point.
 *
 *  Initialize adc to 12bit,enable given channel
 *  , hardware trigger with TIOA0 every second
 *  and start conversion
 *
 *  \return Unused (ANSI-C compatibility).
 *  \callgraph
 */
int main( void )
{
    uint8_t ucKey;
    uint32_t dwAdcValue;
    uint32_t dwThreshold = 0;
    uint32_t dwAdcClock;

    /* Disable watchdog */
    WDT_Disable( WDT ) ;

    /* Output example information */
    printf( "-- ADC12 Threshold Wakeup Example %s --\n\r", SOFTPACK_VERSION ) ;
    printf( "-- %s\n\r", BOARD_NAME ) ;
    printf( "-- Compiled: %s %s --\n\r", __DATE__, __TIME__ ) ;

    /* Initialize threshold */
    gwLowThreshold = MAX_DIGITAL / 10;         /* 10% of Max. value */
    gwHighThreshold = MAX_DIGITAL * 9 / 10;    /* 90% of Max. value */

    /* Initialize ADC */
    ADC_Initialize( ADC, ID_ADC );

    /*
     * Formula: ADCClock = MCK / ( (PRESCAL+1) * 2 )
     * For example, MCK = 84MHZ, PRESCAL = 6, then:
     *     ADCClock = 84 / ((6+1) * 2) = 6MHz;
     */
    /* Set ADC clock */
    dwAdcClock = ADC_SetClock( ADC, 6, BOARD_MCK );
    printf("Set ADC clock to %d Hz\n\r", (int)dwAdcClock);

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
    ADC_SetTiming( ADC, ADC_MR_STARTUP_SUT512, ADC_MR_TRANSFER(1),
                        ADC_MR_TRACKTIM(0), ADC_MR_SETTLING_AST17 );

    /* Set hardware trigger on TIOA0 */
    ADC_SetTrigger( ADC, 1, ADC_MR_TRGSEL_ADC_TRIG1);

    /* Enable channels for potentiometer */
    ADC_EnableChannel( ADC, ADC_CHANNEL_POTENTIOMETER ) ;

    /* Configure TC*/
    _ConfigureTc0() ;

    /* Set compare Channel */
    ADC_SetCompareChannel( ADC, ADC_CHANNEL_POTENTIOMETER ) ;
    /* Set compare mode, in the window */
    ADC_SetCompareMode( ADC, ADC_EMR_CMPMODE_IN ) ;

    /* Setup Threshold*/
    ADC_SetComparisonWindow( ADC, ((gwHighThreshold<<16) | gwLowThreshold) ) ;

    /* enable adc interrupt*/
    NVIC_EnableIRQ( ADC_IRQn ) ;

    /* Disable Compare Interrupt*/
    ADC_DisableIt( ADC, ADC_IDR_COMPE ) ;

    /* Start TC0 and hardware trigger*/
    TC_Start( TC0, 0 ) ;

    /* Display main menu */
    _DisplayMenuChoices() ;

    while( 1 )
    {
        ucKey = UART_GetChar() ;
        printf( "%c\r\n", ucKey ) ;

        switch( ucKey )
        {
            case '0':
                dwAdcValue = ADC_GetConvertedData( ADC, ADC_CHANNEL_POTENTIOMETER ) ;
                printf( "-I- Current voltage is %d mv, %d%% of ADVREF\n\r", (int)(dwAdcValue*VOLT_REF/MAX_DIGITAL), (int)(dwAdcValue*100/MAX_DIGITAL) ) ;
            break ;

            case '1':
                printf( "-I- Thresholds are 0x%x(%d%%) and 0x%x(%d%%).\n\r", gwLowThreshold, gwLowThreshold*100/MAX_DIGITAL, gwHighThreshold, gwHighThreshold*100/MAX_DIGITAL ) ;
            break ;

            case '2':
                printf( "Please input low threshold (0~%d mv):", (int)VOLT_REF) ;
                if ( UART_GetIntegerMinMax((int32_t *)&dwThreshold, 0, VOLT_REF) == 1)
                {
                    dwAdcValue = dwThreshold*MAX_DIGITAL/VOLT_REF ;
                    ADC_SetComparisonWindow( ADC, dwAdcValue | (gwHighThreshold<<16) ) ;

                    /* renew low threshold */
                    gwLowThreshold = dwAdcValue ;
                    printf( "Low threshold is set to 0x%x(%d%%)\n\r", gwLowThreshold, gwLowThreshold*100/MAX_DIGITAL ) ;
                }
                else
                {
                    printf("*** Fail: Wrong input.\n\r");
                }
            break ;

            case '3':
                printf( "Please input high threshold (0~%d mv):", (int)VOLT_REF) ;

                if ( UART_GetIntegerMinMax((int32_t *)&dwThreshold, 0, VOLT_REF) == 1)
                {
                    dwAdcValue = dwThreshold*MAX_DIGITAL/VOLT_REF ;
                    ADC_SetComparisonWindow( ADC, gwLowThreshold | (dwAdcValue<<16) ) ;

                    /* renew high threshold */
                    gwHighThreshold = dwAdcValue ;
                    printf( "High threshold is set to 0x%x(%d%%)\n\r", gwHighThreshold, gwHighThreshold*100/MAX_DIGITAL ) ;
                }
                else
                {
                    printf("*** Fail: Wrong input.\n\r");
                }
            break ;
            case '4' :
                printf( "-a. Below low threshold.\n\r" \
                        "-b. Above high threshold.\n\r" \
                        "-c. In the comparison window.\n\r" \
                        "-d. Out of the comparison window.\n\r"\
                        "-q. Quit the setting.\n\r" ) ;
                ucKey = GetComparisonMode() ;
                ADC_SetCompareMode( ADC, ucKey ) ;
                printf( "Comparison mode is %c.\n\r", 'a'+ucKey ) ;
            break ;

            case 'm' :
            case 'M' :
                _DisplayMenuChoices() ;
            break ;

            case 'i' :
            case 'I' :
                _DisplayInfo() ;
            break ;

            case 's' :
            case 'S' :
                _Fallasleep() ;
            break ;
      }
      printf( "Press \'m\' or \'M\' to display the main menu again!!\n\r" ) ;
    }
}

