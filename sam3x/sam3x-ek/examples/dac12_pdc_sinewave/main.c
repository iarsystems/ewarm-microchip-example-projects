/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support
 * ----------------------------------------------------------------------------
 * Copyright (c) 2009, Atmel Corporation
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
 *  \page dac12_pdc_sinewave DAC12 PDC Sinewave Example
 *
 *  \section Purpose
 *
 *  The dac12_pdc_sinewave example demonstrates how to use DACC peripheral with associated PDC.
 *
 *  \section Requirements
 *
 *  This package can only be used with sam3x-ek.
 *
 *  \section Description
 *
 *  This application is aimed to demonstrate how to use DACC in TC Output trig
 *  mode.
 *
 *  The example allows to configure the frequency and amplitude of output
 *  sinewave. The frequency could be set from 200Hz to 3KHz, and the peak amplitude
 *  could be set from 100 to 2047 in regard to 12bit resolution.
 *
 *  The output could be monitored by connecting PB16/DAC1 to one channel of an
 *  oscilloscope or heard by ear when plugging a headphone to J11 on the EK board.
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
 *      -- DAC12 PDC Sinewave Example xxx --
 *      -- xxxxxx-xx
 *      -- Compiled: xxx xx xxxx xx:xx:xx --
 *      -- Menu Choices for this example--
 *      -- 0: Set frequency(200Hz-3kHz).--
 *      -- 1: Set amplitude(100-2047).--
 *      -- i: Display present frequency and amplitude.--
 *      -- m: Display this menu.--
 *     \endcode
 *  -# Input command according to the menu.
 *
 *  \section References
 *  - dac12_pdc_sinewave/main.c
 *  - dacc.h
 */

/** \file
 *
 *  This file contains all the specific code for the dac12_sinewave.
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

/** Reference voltage for DACC,in mv*/
#define VOLT_REF   (3300)

/** The maximal digital value*/
#define MAX_DIGITAL (4095)

/** SAMPLES per cycle*/
#define SAMPLES (100)

/*----------------------------------------------------------------------------
 *        Local variables
 *----------------------------------------------------------------------------*/
/** channel 0 */
uint8_t DACC_channel_sine = DACC_CHANNEL_1;

/** current index_sample */
uint8_t index_sample = 0;
/** frequency */
uint16_t frequency = 0;
/** amplitude */
uint16_t amplitude = 0;
/** amplitude is MAX_DIGITAL/2,SAMPLES is fixed at 100 */
const int16_t sine_data[SAMPLES]=
{
    0x0,   0x080, 0x100, 0x17f, 0x1fd, 0x278, 0x2f1, 0x367, 0x3da, 0x449,
    0x4b3, 0x519, 0x579, 0x5d4, 0x629, 0x678, 0x6c0, 0x702, 0x73c, 0x76f,
    0x79b, 0x7bf, 0x7db, 0x7ef, 0x7fb, 0x7ff, 0x7fb, 0x7ef, 0x7db, 0x7bf,
    0x79b, 0x76f, 0x73c, 0x702, 0x6c0, 0x678, 0x629, 0x5d4, 0x579, 0x519,
    0x4b3, 0x449, 0x3da, 0x367, 0x2f1, 0x278, 0x1fd, 0x17f, 0x100, 0x080,

    -0x0, -0x080, -0x100, -0x17f, -0x1fd, -0x278, -0x2f1, -0x367,  -0x3da, -0x449,
    -0x4b3, -0x519, -0x579, -0x5d4, -0x629, -0x678, -0x6c0, -0x702, -0x73c, -0x76f,
    -0x79b, -0x7bf, -0x7db, -0x7ef, -0x7fb, -0x7ff, -0x7fb, -0x7ef, -0x7db, -0x7bf,
    -0x79b, -0x76f, -0x73c, -0x702, -0x6c0, -0x678, -0x629, -0x5d4, -0x579, -0x519,
    -0x4b3, -0x449, -0x3da, -0x367, -0x2f1, -0x278, -0x1fd, -0x17f, -0x100, -0x080
};
/** PDC Buffer */
uint16_t wBuffer[SAMPLES];
/*----------------------------------------------------------------------------
 *        Local functions
 *----------------------------------------------------------------------------*/

/**
 * \brief Get input from user, the biggest 4-digit decimal number is allowed
 *
 * \param lower_limit the lower limit of input
 * \param upper_limit the upper limit of input
 */
static int16_t _GetInputValue( int16_t lower_limit, int16_t upper_limit )
{
    int8_t i = 0, c ;
    int16_t value = 0 ;
    int8_t length = 0 ;
    int8_t str_temp[5] = { 0 } ;

    while ( 1 )
    {
        c = UART_GetChar() ;
        if ( c == '\n' || c == '\r' )
        {
            printf( "\n\r" ) ;
            break ;
        }

        if ( '0' <= c && '9' >= c )
        {
            printf( "%c", c ) ;
            str_temp[i++] = c ;

            if ( i >= 4 )
            {
                break ;
            }
        }
    }

    str_temp[i] = '\0' ;
    /* input string length */
    length = i ;
    value = 0 ;

    /* convert string to integer */
    for ( i = 0 ; i < 4 ; i++ )
    {
        if ( str_temp[i] != '0' )
        {
            switch ( length - i - 1 )
            {
                case 0 :
                    value += (str_temp[i] - '0') ;
                break ;

                case 1 :
                    value += (str_temp[i] - '0') * 10 ;
                break ;

                case 2 :
                    value += (str_temp[i] - '0') * 100 ;
                break ;

                case 3 :
                    value += (str_temp[i] - '0') * 1000 ;
                break ;
            }

        }
    }

    if ( value > upper_limit || value < lower_limit )
    {
        printf( "\n\r-F- Input value is invalid!\n" ) ;

        return -1 ;
    }

    return value ;
}

/** Display main menu */
static void _DisplayMenuChoices( void )
{
    printf( "-- Menu Choices for this example--\n\r" ) ;
    printf( "-- 0: Set frequency(200Hz-3kHz).--\n\r" ) ;
    printf( "-- 1: Set amplitude(100-2047).--\n\r" ) ;
    printf( "-- i: Display present frequency and amplitude.--\n\r" ) ;
    printf( "-- m: Display this menu.--\n\r" ) ;
}

/*----------------------------------------------------------------------------
 *        Exported functions
 *----------------------------------------------------------------------------*/
void DACC_IrqHandler(void)
{
    
    uint32_t sr = DACC->DACC_ISR;

    if ( sr & DACC_ISR_ENDTX )
    {
        /* End of transmission */
        DACC_WriteBuffer( DACC, wBuffer, SAMPLES );
    }
}
/*Initialize PDC buffer value and enable PDC channel transfer*/
static void DAC_PDC_Initialize(void)
{
    uint8_t i;
    for(i=0;i<SAMPLES;i++)
    {
          wBuffer[i] = sine_data[i] * amplitude / (MAX_DIGITAL/2) + MAX_DIGITAL/2;
    }
    DACC_WriteBuffer( DACC, wBuffer, SAMPLES );
}
static void Trig_WaveformConfigure(void)
{
    uint32_t dwRA, dwRC;
    /*  Set channel 1 as waveform mode*/
    REG_TC0_CMR1 = 1                                                   /* Waveform Clock Selection */
                   | TC_CMR_WAVE                                        /* Waveform mode is enabled */
                   | TC_CMR_ACPA_SET                                    /* RA Compare Effect: set */
                   | TC_CMR_ACPC_CLEAR                                  /* RC Compare Effect: clear */
                   | TC_CMR_CPCTRG;                                     /* UP mode with automatic trigger on RC Compare */
    dwRC = (BOARD_MCK / 8) / (SAMPLES*frequency);
    REG_TC0_RC1 = dwRC;
    dwRA = (100 - 50) * dwRC / 100;
    REG_TC0_RA1 = dwRA;
}

/**
 * \brief Configure TC0 channel 1 as waveform operating mode.
 */
static void Trig_WaveformInitialize(void)
{
    volatile uint32_t dwDummy;
    /* Configure the PMC to enable the Timer Counter clock for TC0 channel 1. */
    PMC_EnablePeripheral(ID_TC1);
    /*  Disable TC clock */
    REG_TC0_CCR1 = TC_CCR_CLKDIS;
    /*  Disable interrupts */
    REG_TC0_IDR1 = 0xFFFFFFFF;
    /*  Clear status register */
    dwDummy = REG_TC0_SR1;
    /* Configure waveform frequency and duty cycle */
    Trig_WaveformConfigure();
    /* Enable TC0 channel 1 */
    REG_TC0_CCR1 =  TC_CCR_CLKEN | TC_CCR_SWTRG ;
   
}
/**
 *  \brief dac12_sinewave Application entry point.
 *
 *
 *  \return Unused (ANSI-C compatibility).
 *  \callgraph
 */
extern int main( void )
{
    uint8_t c ;
    int16_t freq, amp ;

    /* Disable watchdog */
    WDT_Disable( WDT ) ;

    /* Output example information */
    printf( "-- DAC12 PDC Sinewave Example %s --\n\r", SOFTPACK_VERSION ) ;
    printf( "-- %s\n\r", BOARD_NAME ) ;
    printf( "-- Compiled: %s %s --\n\r", __DATE__, __TIME__ ) ;

    /* initialize amplitude and frequency */
    amplitude = MAX_DIGITAL / 2;
    frequency = 1000;
   
    /*Output waveform to tirg DAC conversion*/    
    Trig_WaveformInitialize();
    
     /* Initialize DACC */
    DACC_Initialize( DACC,
                    ID_DACC,
                    1, /* Hardware triggers are enabled */
                    2, /* TC Channel1 trigger */
                    0, /* Half-Word Transfer */
                    0, /* Normal Mode (not sleep mode) */
                    BOARD_MCK,
                    8, /* refresh period */
                    1, /* Channel 0 selection */
                    0, /* Tag Selection Mode disabled */
                    16 /*  value of the start up time */);
     
  /*Configure interrupt for PDC transfer*/ 
   NVIC_DisableIRQ(DACC_IRQn);
   NVIC_ClearPendingIRQ(DACC_IRQn);
   NVIC_SetPriority(DACC_IRQn, 0);
   NVIC_EnableIRQ(DACC_IRQn);
   DACC_EnableIt(DACC, DACC_IER_ENDTX);

   DAC_PDC_Initialize();
        
    /*Enable  channel for potentiometer*/
   DACC_EnableChannel( DACC, DACC_channel_sine ) ;
   
    /* main menu*/
    _DisplayMenuChoices() ;

    while( 1 )
    {
        c = UART_GetChar() ;

        switch ( c )
        {
            case '0' :
                printf( "Frequency:" ) ;
                freq = _GetInputValue( 200, 3000 ) ;
                printf( "\n\r" ) ;
                printf( "Set frequency to:%dHz\n", freq ) ;

                if ( freq > 0 )
                {
                    frequency = freq ;
              Trig_WaveformInitialize();            
                }
            break ;

            case '1' :
                printf( "Amplitude:" ) ;
                amp = _GetInputValue( 100, 2047 ) ;
                printf( "\n\r" ) ;
                printf( "Set amplitude to %d \n", amp ) ;
                if ( amp > 0 )
                {
                    amplitude = amp ;
              DAC_PDC_Initialize();            
                }
            break ;

            case 'i' :
            case 'I' :
                printf( "-I- Frequency:%d Hz Amplitude:%d\n\r", frequency, amplitude ) ;
            break ;

            case 'm' :
            case 'M' :
                _DisplayMenuChoices() ;
            break ;
        }

        printf( "Press \'m\' or \'M\' to display the main menu again!!\n\r" ) ;
    }
}

