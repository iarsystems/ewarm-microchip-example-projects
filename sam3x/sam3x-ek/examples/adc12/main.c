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
 *  \page adc12 ADC12 Example
 *
 *  \section Purpose
 *
 *  The adc12 example demonstrates how to use ADC peripheral with several modes.
 *
 *  \section Requirements
 *
 *  This package can be used with SAM3X-EK. To enable full scale measurement
 *  of the potentiometer, jumper JP4 has to be closed.
 *
 *  We use "LEFT CLICK" button for ADTGR, so please connect PA11 (ADTRG) to
 *  PE7 (LEFT CLICK) before running the example.
 *
 *  \section Description
 *
 *  This application shows how to use the ADC using the several modes:
 *  with/without PDC, several types of trigger (Software, ADTRG, Timer, etc.),
 *  gain and offset selection, using sequencer. User can select different mode
 *  by configuration menu in the terminal.
 *
 *  \note
 *  For early Engineering Samples, there is a known errata about adc:
 *  An ADC trigger (both software and hardware trigger) initiates a conversion
 *  sequence of one channel and not all activated channels as expected.
 *  So, we have to trigger as many as the number of activated channels.
 *  For example with ADTRG trigger, we need press "LEFT CLICK" two times to
 *  trigger two channels.
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
 *      -- ADC12 Example xxx --
 *      -- xxxxxx-xx
 *      -- Compiled: xxx xx xxxx xx:xx:xx --
 *      =========================================================
 *      Menu: press a key to change the configuration.
 *      ---------------------------------------------------------
 *      [X] 0: Set ADC trigger mode: Software.
 *      [ ] 1: Set ADC trigger mode: ADTRG.
 *      [ ] 2: Set ADC trigger mode: Timer TIOA.
 *      [ ] 3: Set ADC trigger mode: PWM Event Line.
 *      [ ] 4: Set ADC trigger mode: Free run mode.
 *      [E] T: Enable/Disable to tranfer with PDC.
 *      [D] S: Enable/Disable to use user sequence mode.
 *      [D] P: Enable/Disable ADC power save mode.
 *      [D] G: Enable/Disable to set gain=2 for potentiometer channel.
 *      [D] O: Enable/Disable offset for potentiometer channel.
 *          Q: Quit configuration and start ADC.
 *      =========================================================
 *     \endcode
 *  -# The application will output converted value to hyperterminal and display
 *     a menu for user to set different mode.
 *
 *  \section References
 *  - adc12/main.c
 *  - adc.h
 */

/** \file
 *
 *  This file contains all the specific code for the adc12 example.
 */

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

#include "board.h"

#include <string.h>

/*----------------------------------------------------------------------------
 *        Local definitions
 *----------------------------------------------------------------------------*/

/*
 * We use two ADC channel for this example:
 *    ADC_CHANNEL_1  (potentiometer)
 *    ADC_CHANNEL_15 (temperature sensor)
 */
/** Total number of ADC channels in use */
#define NUM_CHANNELS    (2)
/** ADC convention done mask */
#define ADC_DONE_MASK   ( (1<<NUM_CHANNELS) - 1 )

/** Size of the receive buffer and transmit buffer. */
#define BUFFER_SIZE     NUM_CHANNELS

/** Reference voltage for ADC, in mv */
#define VOLT_REF        (3300)

/** The maximal digital value */
#define MAX_DIGITAL     (4095)

#define STRING_EOL    "\r"
#define STRING_HEADER "-- ADC12 Example "SOFTPACK_VERSION" --\r\n" \
                      "-- "BOARD_NAME" --\r\n" \
                      "-- Compiled: "__DATE__" "__TIME__" --"STRING_EOL

/*----------------------------------------------------------------------------
 *        Local types
 *----------------------------------------------------------------------------*/

/** ADC test mode structure */
typedef struct _AdcTestMode
{
    uint8_t ucTriggerMode;
    uint8_t ucPdcEn;
    uint8_t ucSequenceEn;
    uint8_t ucGainEn;
    uint8_t ucOffsetEn;
    uint8_t ucPowerSaveEn;
} tAdcTestMode;

/** ADC trigger modes */
typedef enum _TriggerMode
{
    TRIGGER_MODE_SOFTWARE = 0,
    TRIGGER_MODE_ADTRG,
    TRIGGER_MODE_TIMER,
    TRIGGER_MODE_PWM,
    TRIGGER_MODE_FREERUN
} eTriggerMode;

/** ADC sample data */
typedef struct _AdcSampleData
{
    uint8_t  ucChNum[NUM_CHANNELS];
    uint16_t wValue[NUM_CHANNELS];
    uint16_t wDone;
} tAdcSampleData;

/*----------------------------------------------------------------------------
 *        Local variables
 *----------------------------------------------------------------------------*/

/** ADC test mode */
tAdcTestMode gAdcTestMode;

/** ADC sample data */
tAdcSampleData gAdcSampleData;

/** Definition of ADTRG pin */
Pin gPinADTRG = {PIO_PA11B_ADTRG, PIOA, ID_PIOA, PIO_PERIPH_B, PIO_DEFAULT};

/** Definition of TIOA0 pin */
Pin gPinTIOA0 = {PIO_PB25B_TIOA0, PIOB, ID_PIOB, PIO_PERIPH_B, PIO_DEFAULT};

/** Definition of PWM0 pin */
Pin gPinPwm0 = PIN_PWMC_PWMH0;

/*----------------------------------------------------------------------------
 *        Local functions
 *----------------------------------------------------------------------------*/

/**
 * \brief Display ADC configuration menu.
 */
static void _DisplayMenu( void )
{
    uint8_t ucChar;

    printf( "\n\r");
    printf( "=========================================================\n\r" );
    printf( "Menu: press a key to change the configuration.\n\r" );
    printf( "---------------------------------------------------------\n\r" );
    ucChar = (gAdcTestMode.ucTriggerMode == TRIGGER_MODE_SOFTWARE) ? 'X' : ' ';
    printf( "[%c] 0: Set ADC trigger mode: Software.\n\r", ucChar );
    ucChar = (gAdcTestMode.ucTriggerMode == TRIGGER_MODE_ADTRG) ? 'X' : ' ';
    printf( "[%c] 1: Set ADC trigger mode: ADTRG.\n\r", ucChar );
    ucChar = (gAdcTestMode.ucTriggerMode == TRIGGER_MODE_TIMER) ? 'X' : ' ';
    printf( "[%c] 2: Set ADC trigger mode: Timer TIOA.\n\r", ucChar );
    ucChar = (gAdcTestMode.ucTriggerMode == TRIGGER_MODE_PWM) ? 'X' : ' ';
    printf( "[%c] 3: Set ADC trigger mode: PWM Event Line.\n\r", ucChar );
    ucChar = (gAdcTestMode.ucTriggerMode == TRIGGER_MODE_FREERUN) ? 'X' : ' ';
    printf( "[%c] 4: Set ADC trigger mode: Free run mode.\n\r", ucChar );
    ucChar = (gAdcTestMode.ucPdcEn) ? 'E' : 'D';
    printf( "[%c] T: Enable/Disable to tranfer with PDC.\n\r", ucChar );
    ucChar = (gAdcTestMode.ucSequenceEn) ? 'E' : 'D';
    printf( "[%c] S: Enable/Disable to use user sequence mode.\n\r", ucChar );
    ucChar = (gAdcTestMode.ucPowerSaveEn) ? 'E' : 'D';
    printf( "[%c] P: Enable/Disable ADC power save mode.\n\r", ucChar );
    ucChar = (gAdcTestMode.ucGainEn) ? 'E' : 'D';
    printf( "[%c] G: Enable/Disable to set gain=2 for potentiometer channel.\n\r", ucChar );
    ucChar = (gAdcTestMode.ucOffsetEn) ? 'E' : 'D';
    printf( "[%c] O: Enable/Disable offset for potentiometer channel.\n\r", ucChar );
    printf( "    Q: Quit configuration and start ADC.\n\r" );
    printf( "=========================================================\n\r" );
}

/**
 * \brief Set ADC test mode.
 */
static void _SetAdcTestMode( void )
{
    uint8_t ucKey ;
    uint8_t ucDone = 0 ;

    while ( !ucDone )
    {
        ucKey = UART_GetChar() ;

        switch ( ucKey )
        {
            case '0' :
                gAdcTestMode.ucTriggerMode = TRIGGER_MODE_SOFTWARE ;
            break ;

            case '1' :
                gAdcTestMode.ucTriggerMode = TRIGGER_MODE_ADTRG ;
            break ;

            case '2' :
                gAdcTestMode.ucTriggerMode = TRIGGER_MODE_TIMER ;
            break ;

            case '3' :
                gAdcTestMode.ucTriggerMode = TRIGGER_MODE_PWM ;
            break ;

            case '4' :
                gAdcTestMode.ucTriggerMode = TRIGGER_MODE_FREERUN ;
            break ;

            case 't' :
            case 'T' :
                if ( gAdcTestMode.ucPdcEn )
                {
                    gAdcTestMode.ucPdcEn = 0 ;
                }
                else
                {
                    gAdcTestMode.ucPdcEn = 1 ;
                }
            break ;

            case 's' :
            case 'S' :
                if ( gAdcTestMode.ucSequenceEn )
                {
                    gAdcTestMode.ucSequenceEn = 0 ;
                }
                else
                {
                    gAdcTestMode.ucSequenceEn = 1 ;
                }
            break ;

            case 'p' :
            case 'P' :
                if ( gAdcTestMode.ucPowerSaveEn )
                {
                    gAdcTestMode.ucPowerSaveEn = 0 ;
                }
                else
                {
                    gAdcTestMode.ucPowerSaveEn = 1 ;
                }
            break ;

            case 'g' :
            case 'G' :
                if ( gAdcTestMode.ucGainEn )
                {
                    gAdcTestMode.ucGainEn = 0 ;
                }
                else
                {
                    gAdcTestMode.ucGainEn = 1 ;
                }
            break ;

            case 'o':
            case 'O':
                if ( gAdcTestMode.ucOffsetEn )
                {
                    gAdcTestMode.ucOffsetEn = 0 ;
                }
                else
                {
                    gAdcTestMode.ucOffsetEn = 1 ;
                }
            break ;

            case 'q' :
            case 'Q' :
                ucDone = 1 ;
            break ;

            default :
            break ;
        }
        _DisplayMenu() ;
    }
}

/**
 * \brief Configure to trigger ADC by TIOA output of timer.
 */
static void _ConfigureTimerTrigger( void )
{
    uint32_t dwDiv;
    uint32_t dwTcClks;

    /* PIO configuration */
    PIO_PinConfigure( &gPinTIOA0, PIO_LISTSIZE( gPinTIOA0 ) ) ;

    /* Enable peripheral clock. */
    PMC_EnablePeripheral( ID_TC0 ) ;

    /* Configure TC for a 1Hz frequency and trigger on RC compare. */
    TC_FindMckDivisor( 1, BOARD_MCK, &dwDiv, &dwTcClks, BOARD_MCK ) ;
    TC_Configure( TC0, 0, dwTcClks | TC_CMR_CPCTRG | TC_CMR_WAVE | TC_CMR_ACPA_CLEAR | TC_CMR_ACPC_SET) ;
    TC0->TC_CHANNEL[0].TC_RA = ( BOARD_MCK / dwDiv ) / 2 ;
    TC0->TC_CHANNEL[0].TC_RC = ( BOARD_MCK / dwDiv ) / 1 ;

    /* Start the Timer */
    TC_Start( TC0, 0 ) ;

    /* Set TIOA0 trigger */
    ADC_SetTrigger( ADC, 1, ADC_MR_TRGSEL_ADC_TRIG1 ) ;
}

/**
 * \brief Configure to trigger ADC by PWM Event Line.
 */
static void _ConfigurePwmTrigger( void )
{
/* PWM frequency in Hz. */
#define PWM_FREQUENCY               2
/* Maximum duty cycle value. */
#define MAX_DUTY_CYCLE              1000

    /* PIO configuration */
    PIO_PinConfigure( &gPinPwm0, PIO_LISTSIZE(gPinPwm0) );

    /* Enable PWMC peripheral clock */
    PMC_EnablePeripheral( ID_PWM ) ;

    /* Set clock A to run at PWM_FREQUENCY * MAX_DUTY_CYCLE (clock B is not used) */
    PWMC_ConfigureClocks( PWM_FREQUENCY * MAX_DUTY_CYCLE, 0, BOARD_MCK ) ;

    /* Configure PWMC for channel 0 (left-aligned) */
    PWMC_ConfigureChannel( PWM,
                           0,                   /* channel */
                           PWM_CMR_CPRE_CLKA,   /* prescaler: CLKA  */
                           0,                   /* alignment */
                           0                    /* polarity */
                          ) ;
    PWMC_SetPeriod( PWM, 0, MAX_DUTY_CYCLE ) ;
    PWMC_SetDutyCycle( PWM, 0, MAX_DUTY_CYCLE/2 ) ;

    PWMC_ConfigureComparisonUnit( PWM, 0, MAX_DUTY_CYCLE/2, PWM_CMPM_CEN ) ;
    PWMC_ConfigureEventLineMode( PWM, 0, PWM_ELMR_CSEL0 ) ;

    /* Enable PWM channel 0 */
    PWMC_EnableChannel( PWM, 0 ) ;

    /* Set PWM Event Line 0 trigger */
    ADC_SetTrigger( ADC, 1, ADC_MR_TRGSEL_ADC_TRIG4 ) ;
}

/**
 * \brief (Re)Sart ADC sample.
 * Initialize ADC, set clock and timing, set ADC to given mode.
 */
static void _StartAdc( void )
{
    uint32_t dw ;
    uint32_t dwAdcClock ;

    /* Initialize ADC */
    ADC_Initialize( ADC, ID_ADC );
    memset( (void*)&gAdcSampleData, 0, sizeof( gAdcSampleData ) );

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
    ADC_SetTiming( ADC, ADC_MR_STARTUP_SUT512, ADC_MR_TRANSFER( 1 ), ADC_MR_TRACKTIM( 0 ), ADC_MR_SETTLING_AST17 ) ;

    /* Enable channel number tag */
    ADC_SetTagEnable( ADC, 1 ) ;

    /* Enable/disable sequencer */
    if ( gAdcTestMode.ucSequenceEn )
    {
        /* Set user defined channel sequence */
        ADC_SetSequence( ADC, ADC_SEQR1_USCH1(ADC_CHANNEL_15) | ADC_SEQR1_USCH2(ADC_CHANNEL_1), 0 ) ;
        /* Enable sequencer */
        ADC_SetSequenceMode( ADC, 1 ) ;

        /* Enable channels */
        for ( dw = 0; dw < NUM_CHANNELS; dw++ )
        {
            ADC_EnableChannel( ADC, dw ) ;
        }
        /* Update channel number */
        gAdcSampleData.ucChNum[0] = ADC_CHANNEL_15 ;
        gAdcSampleData.ucChNum[1] = ADC_CHANNEL_1 ;
    }
    else
    {
        /* Disable sequencer */
        ADC_SetSequenceMode( ADC, 0 ) ;

        /* Enable  channels */
        ADC_EnableChannel( ADC, ADC_CHANNEL_1 ) ;
        ADC_EnableChannel( ADC, ADC_CHANNEL_15 ) ;

        /* Update channel number */
        gAdcSampleData.ucChNum[0] = ADC_CHANNEL_1 ;
        gAdcSampleData.ucChNum[1] = ADC_CHANNEL_15 ;
    }
    /* Enable the temperature sensor (CH15) */
    ADC_EnableTS( ADC ) ;

    /* Set gain and offset (only single ended mode used here) */
    ADC_SetAnalogChange( ADC, 0 ) ; /* Disable analog change */
    if ( gAdcTestMode.ucGainEn  )
    {
        ADC_SetAnalogChange( ADC, 1) ;
        ADC_SetChannelGain( ADC, ADC_CGR_GAIN1(2) ) ; /* gain = 2 */
    }
    else
    {
        ADC_SetChannelGain( ADC, 0 ) ; /* gain = 1 */
    }

    if ( gAdcTestMode.ucOffsetEn )
    {
        ADC_SetAnalogChange( ADC, 1 ) ;
        ADC_SetChannelOffset( ADC, ADC_COR_OFF1 ) ;
    }
    else
    {
        ADC_SetChannelOffset( ADC, 0 ) ;
    }

    /* Set power save */
    /* Note: SLEEP mode doesn't work in engineer samples. */
    if ( gAdcTestMode.ucPowerSaveEn )
    {
        ADC_SetSleepMode( ADC, 1 ) ;
    }
    else
    {
        ADC_SetSleepMode( ADC, 0 ) ;
    }

    /* Transfer with/without PDC */
    if ( gAdcTestMode.ucPdcEn )
    {
        ADC_ReadBuffer( ADC, gAdcSampleData.wValue, BUFFER_SIZE ) ;
        /* Enable PDC channel interrupt */
        ADC_EnableIt( ADC, ADC_IER_RXBUFF ) ;
    }
    else
    {
        /* Enable Data ready interrupt */
        ADC_EnableIt( ADC, ADC_IER_DRDY ) ;
    }
    /* Enable ADC interrupt */
    NVIC_EnableIRQ( ADC_IRQn ) ;

    /* Configure trigger mode and start convention */
    switch ( gAdcTestMode.ucTriggerMode )
    {
        case TRIGGER_MODE_SOFTWARE:
            ADC_SetTrigger( ADC, 0, 0 ) ; /* Disable hardware trigger */
        break ;

        case TRIGGER_MODE_ADTRG:
            PIO_PinConfigure( &gPinADTRG, 1 ) ;
            ADC_SetTrigger( ADC, 1, ADC_MR_TRGSEL_ADC_TRIG0 ) ;
        break ;

        case TRIGGER_MODE_TIMER :
            _ConfigureTimerTrigger() ;
        break ;

        case TRIGGER_MODE_PWM :
            _ConfigurePwmTrigger() ;
        break ;

        case TRIGGER_MODE_FREERUN :
            ADC_SetFreeRunMode( ADC, 1 ) ;
        break ;

        default :
        break ;
    }
}

/*----------------------------------------------------------------------------
 *        Exported functions
 *----------------------------------------------------------------------------*/

/**
 * \brief Systick handler.
 */
void SysTick_Handler( void )
{
    TimeTick_Increment();
}

/**
 * \brief Interrupt handler for the ADC.
 */
void ADC_IrqHandler(void)
{
    uint32_t i;
    uint32_t dwStatus;
    uint32_t dwTemp;
    uint8_t  ucChNum;

    dwStatus = ADC_GetStatus(ADC);

    /* With PDC transfer */
    if ( gAdcTestMode.ucPdcEn )
    {
        if ( (dwStatus & ADC_ISR_RXBUFF) == ADC_ISR_RXBUFF )
        {
            gAdcSampleData.wDone = ADC_DONE_MASK;
            ADC_ReadBuffer( ADC, gAdcSampleData.wValue, BUFFER_SIZE ) ;

            /* Only keep sample value, discard channel number */
            for (i = 0; i < NUM_CHANNELS; i ++)
            {
                gAdcSampleData.wValue[i] &= ADC_LCDR_LDATA_Msk;
            }
        }
    }

    /* Without PDC transfer */
    if ( !gAdcTestMode.ucPdcEn )
    {
        if ( (dwStatus & ADC_ISR_DRDY) == ADC_ISR_DRDY )
        {
            dwTemp = ADC_GetLastConvertedData( ADC );

            for (i = 0; i < NUM_CHANNELS; i ++)
            {
                ucChNum = (dwTemp & ADC_LCDR_CHNB_Msk) >> ADC_LCDR_CHNB_Pos;
                if (gAdcSampleData.ucChNum[i] == ucChNum)
                {
                    gAdcSampleData.wValue[i] = dwTemp & ADC_LCDR_LDATA_Msk;
                    gAdcSampleData.wDone |= 1 << i;
                }
            }
        }
    }
}

/**
 *  \brief adc12 Application entry point.
 *
 *  \return Unused (ANSI-C compatibility).
 */
int main( void )
{
    uint32_t dw ;
    uint32_t dwTickLast = 0, dwTick = 0;

    /* Disable watchdog */
    WDT_Disable( WDT ) ;

    /* Output example information */
    puts( STRING_HEADER ) ;

    /* Set 1 ms timer */
    TimeTick_Configure( BOARD_MCK );

    /* Set defaut ADC test mode */
    memset( (void *)&gAdcTestMode, 0, sizeof(gAdcTestMode) );
    gAdcTestMode.ucTriggerMode = TRIGGER_MODE_SOFTWARE;
    gAdcTestMode.ucPdcEn = 1;
    gAdcTestMode.ucSequenceEn = 0;
    gAdcTestMode.ucGainEn = 0;
    gAdcTestMode.ucOffsetEn = 0;

    _DisplayMenu();
    _StartAdc();

    printf( "Press any key to display configuration menu.\n\r\n\r" );
    while (1)
    {
        /* ADC software trigger per 1s */
        dwTick = GetTickCount();
        if ( gAdcTestMode.ucTriggerMode == TRIGGER_MODE_SOFTWARE)
        {
            if ( (dwTick - dwTickLast) > 1000 )
            {
                dwTickLast = dwTick;
                ADC_StartConversion(ADC);
            }
        }

        /* Check if user enter a key */
        if ( UART_IsRxReady() )
        {
            UART_GetChar();

            ADC_DisableIt(ADC, 0xFFFFFFFF); /* Disable all adc interrupt */
            TC_Stop( TC0, 0 ) ;             /* Stop the Timer */
            PWMC_DisableChannel( PWM, 0 );  /* Disable PWM channel 0 */
            _DisplayMenu();
            _SetAdcTestMode();
            _StartAdc();
            printf( "Press any key to display configuration menu.\n\r\n\r" );
        }

        /* Check if ADC sample is done */
        if ( gAdcSampleData.wDone == ADC_DONE_MASK )
        {
            for ( dw = 0 ; dw < NUM_CHANNELS ; dw++ )
            {
                printf( "CH%02d: %04d mv.    ",
                    (int)gAdcSampleData.ucChNum[dw],
                    (int)(gAdcSampleData.wValue[dw] * VOLT_REF / MAX_DIGITAL) );
            }
            printf("\r");
            gAdcSampleData.wDone = 0;
        }
    }
}

