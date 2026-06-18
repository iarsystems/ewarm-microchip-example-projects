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

#include "demo_parameters.h"
#include "gui/screens.h"
#include "libsam_gui.h"
#include "board.h"

#include <stdio.h>

/**
 * \addtogroup screens
 * @{
 * \addtogroup screen_audio Audio screen
 * @{
 *
 * \brief This screens aims at recording and playing a sound sample.
 */

/*----------------------------------------------------------------------------
 *         Definitions
 *----------------------------------------------------------------------------*/

//#define SCR_AUDIO_ENABLE
#undef SCR_AUDIO_ENABLE

#if defined SCR_AUDIO_ENABLE
/**  Number of available audio buffers. */
#define BUFFER_NUMBER       8
/**  Number of available microphone buffers. */
#define BUFFER_NUMBER_MIC   7
/**  Size of one buffer in bytes. */
#define BUFFER_SIZE         1024//(_BYTESPERFRAME + _BYTESPERSUBFRAME)

/**  Buffer adjust level */
#define BUFFER_DELAY        (3)

/**  Delay in ms for starting the DAC transmission
     after a frame has been received. */
#define DAC_DELAY           BUFFER_DELAY

/**  Delay in ms for starting the ADC transmission
     after a frame has been requested. */
#define ADC_DELAY           (6)

/** Audio output channel RIGHT */
#define SPEAKER_CHANNEL_R       DACC_CHANNEL_0
/** Audio output channel LEFT */
#define SPEAKER_CHANNEL_L       DACC_CHANNEL_1

/** Audio input channel RIGHT */
#define MICROPHONE_CHANNEL      ADC_CHANNEL_4

#define MID_VALUE_16BITS  (0x7FFF)
#define MASK_12BITS       (0xFFF)
#define DACC_REFRESH      (8)
#define DACC_STARTUP      DACC_MR_STARTUP_960

/** Sample rate in Hz. */
#define _SAMPLERATE        48000
/** Number of channels in audio stream. */
#define _NUMCHANNELS       2
/** Number of bytes in one sample. */
#define _BYTESPERSAMPLE    2
/** Number of bits in one sample. */
#define _BITSPERSAMPLE     (_BYTESPERSAMPLE * 8)
/** Number of samples in one USB subframe. */
#define _BYTESPERSUBFRAME  (_NUMCHANNELS * _BYTESPERSAMPLE)
/** Number of samples in one USB frame. */
#define _SAMPLESPERFRAME   (_SAMPLERATE / 1000 * _NUMCHANNELS)
/** Number of bytes in one USB frame. */
#define _BYTESPERFRAME     (_SAMPLESPERFRAME * _BYTESPERSAMPLE)
/** Master channel. */
#define _MASTERCHANNEL     0
/** Front left channel. */
#define _LEFTCHANNEL       1
/** Front right channel. */
#define _RIGHTCHANNEL      2
/*----------------------------------------------------------------------------
 *         Internal variables
 *----------------------------------------------------------------------------*/

/**  Data buffers for receiving audio frames from the USB host. */
static __no_init uint8_t buffers[BUFFER_NUMBER][BUFFER_SIZE];
/**  Number of samples stored in each data buffer. */
static __no_init uint32_t bufferSizes[BUFFER_NUMBER];
/**  Next buffer in which USB data can be stored. */
static uint32_t inBufferIndex = 0;
/**  Next buffer which should be sent to the DAC. */
static uint32_t outBufferIndex = 0;
/**  Number of buffers that can be sent to the DAC. */
static volatile uint32_t numBuffersToSend = 0;
/**  Data buffers for mic */
static __no_init uint8_t micBuffers[BUFFER_NUMBER_MIC][BUFFER_SIZE];
/**  Number of samples in each buffer */
static __no_init uint8_t micSizes[BUFFER_NUMBER_MIC];
/**  Next buffer in which ADC data can be stored. */
static uint32_t micInIndex = 0;
/**  Next buffer which should be sent to USB. */
static uint32_t micOutIndex = 0;
/**  Number of buffers that has be sampled by ADC. */
static volatile int32_t micNumSampled = 0;

/**  Current state of the USB OUT transmission. */
static volatile uint8_t isPlyActive = 0;
/**  Current state of the DAC transmission. */
static volatile uint8_t isDacActive = 0;
/**  Current state of the USB IN transmission. */
static volatile uint8_t isMicActive = 0;
/**  Current state of the ADC transmission. */
static volatile uint8_t isAdcActive = 0;
/**  Number of buffers to wait for before the DAC starts to transmit data. */
static __no_init volatile uint8_t dacDelay;
/**  Number of buffers to wait for before the ADC starts to transmit data. */
static __no_init volatile uint8_t adcDelay;

/**  Default TC frequency setting backup: RA */
static __no_init uint32_t defaultRA;
#endif // SCR_AUDIO_ENABLE

/**  Widgets */
static __no_init SWGT_Widget* g_btnRecord ;
static __no_init SWGT_Widget* g_btnPlayPause ;
static __no_init SWGT_Widget* g_btnStop ;
static __no_init SWGT_Widget* g_btnVolume ;
static __no_init SWGT_Widget* g_btnMicrophone ;
static __no_init SWGT_Widget* g_btnBack ;

#if defined SCR_AUDIO_ENABLE
/**
 * Enable/Disable audio speaker channels
 */
static void _AudioSpeakerEnable( uint8_t ucEnable )
{
    if ( ucEnable == 1 )
    {
        TC_Start( TC0, 0 ) ;
        DACC_EnableChannel( DACC, SPEAKER_CHANNEL_R ) ;
        DACC_EnableChannel( DACC, SPEAKER_CHANNEL_L ) ;
    }
    else
    {
        if ( ucEnable == 0 )
        {
            DACC_DisableChannel( DACC, SPEAKER_CHANNEL_R ) ;
            DACC_DisableChannel( DACC, SPEAKER_CHANNEL_L ) ;

            if ( !isAdcActive )
            {
                TC_Stop( TC0, 0 ) ;
            }
        }
    }
}

/**
 * Enable/Disable audio microphone channels
 */
static void _AudioMicrophoneEnable( uint8_t ucEnable )
{
    if ( ucEnable == 1 )
    {
        TC_Start( TC0, 0 ) ;
        ADC_EnableChannel( ADC, MICROPHONE_CHANNEL ) ;
    }
    else
    {
        if ( ucEnable == 0 )
        {
            ADC_DisableChannel( ADC, MICROPHONE_CHANNEL ) ;
            if ( !isDacActive )
            {
                TC_Stop( TC0, 0 ) ;
            }
        }
    }
}

/**
 * Configure the TC0 and DACC, ADC for audio playback/record.
 * \param sampleRate Audio sample rate.
 * \param nbChannels Number of audio channels.
 * \param mck        MCK frequence.
 */
static void _ConfigureAudio( uint32_t dwSampleRate, uint8_t ucChannels, uint32_t dwMCK )
{
    uint32_t div = 2;
    uint32_t tcclks = TC_CMR0_TCCLKS_TIMER_CLOCK1 ;
    uint32_t freq = dwSampleRate * ucChannels ;
    double   ra, rc ;
    static uint32_t divs[5] = { 2, 8, 32, 128, BOARD_MCK / 32768 } ;
    uint8_t i = 0 ;

    /* Enable TC0 Peripheral */
    PMC_EnablePeripheral( ID_TC0 ) ;
    {
        divs[4] = dwMCK/32768 ;

        /* Minimize DIV & Maximize RC for better waveform */
        while ( freq < ((dwMCK / divs[i]) / 65536) )
        {
            ++ i ;
            if ( i == 5 )
            {
                TRACE_FATAL( "Cann't find TC0 divisor!\n\r" ) ;
            }
        }
        div = divs[i] ;
        tcclks = i ;
    }
    /* Configure TC for tioa output: 48M -> 48K*2 */
    TC_Configure( TC0, 0, tcclks /*MCK/2*/
                        | TC_CMR0_ACPC_SET
                        | TC_CMR0_WAVE
                        | TC_CMR0_ACPA_CLEAR
                        | TC_CMR0_CPCTRG ) ;
    /* 50% duty ,freq frequency*/
    ra = (((double)dwMCK/div)/(freq*2) + 0.5 );
    rc = (((double)dwMCK/div)/(freq)   + 0.99);
    TC0->TC_CHANNEL[0].TC_RA = (uint32_t)ra;
    TC0->TC_CHANNEL[0].TC_RC = (uint32_t)ra*2;
    defaultRA = (uint32_t)ra ;

    printf( "-I- MCK %dKHz, Div %d(%x), RA: %d*.01(%x), RC: %d*.01(%x)\n\r",
            (int)dwMCK/1000, (int)div, div,
            (int)(ra*100), (int)ra, (int)(rc*100), (int)rc);

    /* Initialize DACC with HW as trigger */
    DACC_Initialize( DACC, ID_DACC,
                     DACC_MR_TRGEN_EN,
                     1,                 /* TIO Output of the Timer Counter Channel 0 */
                     DACC_MR_WORD_HALF,
                     0,                 /* Normal Mode (not DACC_MR_SLEEP) */,
                     BOARD_MCK,
                     DACC_REFRESH,
                     SPEAKER_CHANNEL_R,
                     DACC_MR_TAG_EN,
                     DACC_STARTUP ) ;
    /* channel number is in the first 4 significant bits*/
    DACC->DACC_MR |= DACC_MR_TAG ;
    /* Mute */
    _AudioSpeakerEnable( 0 ) ;
    /* Enable DACC ISR */
    NVIC_EnableIRQ( DACC_IRQn ) ;

    /* Initialize ADC with HW as trigger */
    ADC_Initialize( ADC, ID_ADC,
                    ADC_MR_TRGEN_EN,
                    ADC_MR_TRGSEL_ADC_TRIG1,
                    ADC_MR_SLEEP_NORMAL,
                    ADC_MR_LOWRES_12_BIT,
                    BOARD_MCK,
                    12000000,
                    ADC_MR_STARTUP_512 >> 16,
                    128 ) ;

    /* channel number is in the first 4 significant bits */
    ADC->ADC_EMR |= ADC_EMR_TAG ;

    /* Mute */
    _AudioMicrophoneEnable( 0 ) ;

    /* Enable ADC ISR */
    NVIC_EnableIRQ( ADC_IRQn ) ;
}

/**
 * Buffer translation for PCM format to DAC values
 * \param pBuffer    Pointer for source and destination buffer
 * \param size       Buffer size in number of samples
 * \param nbChannels Channel numbers for source data
 */
static void _PCM2DACTranslate( int16_t *psBuffer, uint32_t dwSize, uint8_t ucChannels )
{
    uint32_t dw = 0 ;

    while ( dw < dwSize )
    {
        if ( ucChannels > 1 )
        {
            /* channle0-0,channel1-1 */
            psBuffer[dw] = ( ( (psBuffer[dw] + MID_VALUE_16BITS) >> 4 ) & MASK_12BITS ) |
                           ((uint16_t)(dw%2) << 12) ;
        }
        else
        {
            if ( ucChannels == 1 )
            {
                /* one channel data stored continously */
                psBuffer[dw] = ( (psBuffer[dw] + MID_VALUE_16BITS) >> 4 ) & MASK_12BITS ;
            }
        }
        dw++ ;
    }
}

/**
 * Play PCM data through DAC
 * \param pBuffer    Pointer to data buffer (16-Bit aligned)
 * \param nbSamples  Number of audio data samples
 * \param nbChannels Number of audio channels
 */
static void _DAC_PlayBuffer( uint8_t* pucBuffer, uint32_t dwSamples, uint8_t ucChannels )
{
    _PCM2DACTranslate( (int16_t*)pucBuffer, dwSamples, ucChannels ) ;
    DACC_WriteBuffer( DACC, (int16_t*)pucBuffer, dwSamples ) ;
}

/**
 * Sample audio data through ADC
 * \param pBuffer    Pointer to data buffer (16-Bit aligned)
 * \param nbSamples  Number of audio data samples
 * \param nbChannels Number of audio channels
 */
static inline int8_t _ADC_SampleBuffer( uint8_t* pucBuffer, uint32_t dwSamples, uint8_t ucChannels )
{
    return ADC_ReadBuffer( ADC, (int16_t*)pucBuffer, dwSamples ) ;
}

/**
 * Buffer translation for N channel ADC to PCM format,
 * source is N channel ADC data.
 * \param pBuffer           Pointer for source and destination buffer
 * \param size              Buffer size in number of samples
 * \param ovwrChannels      Expand first channel to over write others.
 */
static void _ADC2PCM_Translate( int16_t *psBuffer, uint32_t dwSize, uint8_t ucExpandChannels )
{
    while ( dwSize )
    {
        psBuffer[0] = ((psBuffer[0] & MASK_12BITS) << 4) - MID_VALUE_16BITS ;

        /* Expand 1 or more channel */
        if ( ucExpandChannels )
        {
            uint8_t dw ;
            for ( dw = 1 ; dw <= ucExpandChannels ; dw++ )
            {
                psBuffer[dw] = psBuffer[0] ;
            }
            psBuffer = &psBuffer[dw] ;
            dwSize -= dw ;
        }
        /* Process next sample */
        else
        {
            psBuffer++ ;
            dwSize-- ;
        }
    }
}

/**
 * Handles interrupts coming from the DACC.
 */
void DAC_IrqHandler( void )
{
    Dacc *pDac = DACC ;
    uint32_t sr = pDac->DACC_ISR ;

    if ( sr & DACC_ISR_TXBUFE )
    {
        /* End of transmission */
        _AudioSpeakerEnable( 0 ) ;
        DACC_DisableIt( pDac, DACC_IDR_TXBUFE | DACC_IDR_ENDTX ) ;
        pDac->DACC_PTCR = DACC_PTCR_TXTDIS ;
        isDacActive = 0 ;
        /* Reset playback index */
        inBufferIndex = outBufferIndex = 0 ;
    }
    else
        if ( sr & DACC_ISR_ENDTX )
    {
        if ( numBuffersToSend )
        {
            /* Check the number of available buffers */
            if ( numBuffersToSend > BUFFER_DELAY )
            {
                //printf("%d+", numBuffersToSend);
                /* Speed up DAC freq (TC) */
                {
                    uint32_t newRA = defaultRA - (numBuffersToSend-DAC_DELAY) ;

                    if (TC0->TC_CHANNEL[0].TC_RA != newRA)
                    {
                        TC0->TC_CHANNEL[0].TC_RA =  newRA;
                        TC0->TC_CHANNEL[0].TC_RC =  newRA*2;
                    }
                }
            }
            else
                if (numBuffersToSend < BUFFER_DELAY)
                {
                //printf("%d-", numBuffersToSend);
                /* Slow down DAC (TC) */
                {
                    uint32_t newRA = defaultRA + (DAC_DELAY-numBuffersToSend) ;

                    if ( TC0->TC_CHANNEL[0].TC_RA != newRA )
                    {
                        TC0->TC_CHANNEL[0].TC_RA = newRA ;
                        TC0->TC_CHANNEL[0].TC_RC = newRA*2 ;
                    }
                }
            }
            else
            {
                /* Keep DAC (TC) */
                if ( TC0->TC_CHANNEL[0].TC_RA != defaultRA )
                {
                    TC0->TC_CHANNEL[0].TC_RA = defaultRA ;
                    TC0->TC_CHANNEL[0].TC_RC = defaultRA*2 ;
                }
            }

            /* Load next buffer */
            _DAC_PlayBuffer( buffers[outBufferIndex], bufferSizes[outBufferIndex], 1 ) ;
            outBufferIndex = (outBufferIndex + 1) % BUFFER_NUMBER ;
            numBuffersToSend --;
        }
        else
        {
            /* Last frame remaining */
            DACC_DisableIt( pDac, DACC_IDR_ENDTX ) ;
        }
    }
}

/**
 * Handles interrupts coming from the ADC.
 */
void ADC_IrqHandler(void)
{
    Adc *pAdc = ADC;
    uint32_t isr = pAdc->ADC_ISR;

    micInIndex = (micInIndex + 1) % BUFFER_NUMBER_MIC ;
    micNumSampled ++ ;

    if ( isr & ADC_ISR_RXBUFF )
    {
        /* End of transmission */
        _AudioMicrophoneEnable( 0 ) ;
        ADC_DisableIt( pAdc, ADC_IDR_RXBUFF | ADC_IDR_ENDRX ) ;
        pAdc->ADC_PTCR = ADC_PTCR_RXTDIS ;
        isAdcActive = 0 ;

        /* Reset Microphone status if stream OFF */
        micNumSampled = 0 ;
        micInIndex = micOutIndex = 0 ;
    }
    else
    {
        if ( isr & ADC_ISR_ENDRX )
        {
            uint32_t sampleSize = _SAMPLESPERFRAME ;

            if ( micNumSampled < BUFFER_DELAY )
            {
                /* Shorter */
                sampleSize -= _BYTESPERSUBFRAME / _BYTESPERSAMPLE ;
                //printf("%d ", sampleSize);
                //printf("%d- ", micNumSampled);
                //printf("%d,%d ", micInIndex, micOutIndex);
            }
            else
            {
                if (micNumSampled > BUFFER_DELAY)
                {
                    /* Longer */
                    sampleSize += _BYTESPERSUBFRAME
                                    / _BYTESPERSAMPLE;
                    //printf("%d ", sampleSize);
                    //printf("%d+ ", micNumSampled);
                }
            }

            micSizes[micInIndex] = sampleSize ;
            _ADC_SampleBuffer( micBuffers[micInIndex], sampleSize, _NUMCHANNELS ) ;

            if ( !isMicActive )
            {
                /* Last frame */
                ADC_DisableIt( pAdc, ADC_IDR_ENDRX ) ;
            }
        }
    }
}
#endif // SCR_AUDIO_ENABLE

static uint32_t _ScrAudio_OnInitialize( SWGTScreen* pScreen )
{
    WGT_Screen_Initialize( pScreen, GUICLR_WHITE, NULL ) ;

    g_btnRecord=WGT_CreateWidget( WGT_TYPE_BUTTON, 14, 62, 48, 48 ) ;
    if ( g_btnRecord )
    {
        WGT_SetBitmap( g_btnRecord, DEMO_BMP_BTN_RECORD ) ;
        WGT_Screen_AddWidget( pScreen, g_btnRecord ) ;
    }

    g_btnMicrophone=WGT_CreateWidget( WGT_TYPE_BUTTON, 174, 62, 48, 48 ) ;
    if ( g_btnMicrophone )
    {
        WGT_SetBitmap( g_btnMicrophone, DEMO_BMP_BTN_MICROPHONE_DISABLED ) ;
        WGT_Screen_AddWidget( pScreen, g_btnMicrophone ) ;
    }

    g_btnPlayPause=WGT_CreateWidget( WGT_TYPE_BUTTON, 14, 140, 48, 48 ) ;
    if ( g_btnPlayPause )
    {
        WGT_SetBitmap( g_btnPlayPause, DEMO_BMP_BTN_PLAY_PAUSE_DISABLED ) ;
        WGT_Screen_AddWidget( pScreen, g_btnPlayPause ) ;
    }

    g_btnStop=WGT_CreateWidget( WGT_TYPE_BUTTON, 94, 140, 48, 48 ) ;
    if ( g_btnStop )
    {
        WGT_SetBitmap( g_btnStop, DEMO_BMP_BTN_STOP_DISABLED ) ;
        WGT_Screen_AddWidget( pScreen, g_btnStop ) ;
    }

    g_btnVolume=WGT_CreateWidget( WGT_TYPE_BUTTON, 174, 140, 48, 48 ) ;
    if ( g_btnVolume )
    {
        WGT_SetBitmap( g_btnVolume, DEMO_BMP_BTN_VOLUME_DISABLED ) ;
        WGT_Screen_AddWidget( pScreen, g_btnVolume ) ;
    }

    g_btnBack=WGT_CreateWidget( WGT_TYPE_BUTTON, 12, 252, 48, 48 ) ;
    if ( g_btnBack )
    {
        WGT_SetBitmap( g_btnBack, DEMO_BMP_BTN_BACK ) ;
        WGT_Screen_AddWidget( pScreen, g_btnBack ) ;
    }

    return SAMGUI_E_OK ;
}

static uint32_t _ScrAudio_OnDrawBackground( SWGTScreen* pScreen )
{
    SGUIColor clr ;

    // Draw Atmel logo
    g_WGT_CoreData.pBE->DrawBitmap( 4, 8, 87, 40, DEMO_BMP_LOGO_ATMEL ) ;

    // Draw labels
    clr.u.dwRGBA=GUICLR_ATMEL_BLUE ;
    g_WGT_CoreData.pBE->DrawText( 105, 8, DEMO_LABEL"\nAudio", &clr, (void*)&g_Font10x14, 0 ) ;

    clr.u.dwRGBA=GUICLR_RED ;
    g_WGT_CoreData.pBE->DrawText( 20, 200, "Under construction\n Touch to return", &clr, (void*)&g_Font10x14, 0 ) ;

    return SAMGUI_E_OK ;
}

static uint32_t _ScrAudio_ProcessMessage( SWGTScreen* pScreen, SWGTCoreMessage* pMsg )
{
//    printf( "ScrAudio - msg id %x\r\n", pMsg->dwID ) ;

    switch ( pMsg->dwID )
    {
        case WGT_MSG_POINTER_RELEASED : // cursor released
        case WGT_MSG_KEY_RELEASED : // button released
            // Send message to switch to main screen
            WGT_SetCurrentScreen( &ScrMain ) ;
        break ;
    }

    return SAMGUI_E_OK ;
}

SWGTScreen ScrAudio=
{
    .ProcessMessage=_ScrAudio_ProcessMessage,

    /* Hooks */
    .HkBeforePaint=NULL,
    .HkAfterPaint=NULL,
    .HkBeforeEraseBackground=NULL,
    .HkAfterEraseBackground=NULL,
    .HkExit=NULL,

    /* Callbacks */
    .OnInitialize=_ScrAudio_OnInitialize,
    .OnEraseBackground=_ScrAudio_OnDrawBackground,
    .OnPaint=NULL
} ;

/** @} */
/** @} */
