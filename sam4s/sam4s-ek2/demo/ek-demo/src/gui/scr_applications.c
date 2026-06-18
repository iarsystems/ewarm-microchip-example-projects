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

#include "gui/screens.h"
#include "libsam_gui.h"
#include "demo_parameters.h"
#include "board.h"

#include <stdio.h>
#include <string.h>

/**
 * \addtogroup screens
 * @{
 * \addtogroup screen_main Main screen
 * @{
 *
 * \brief This screen is the main screen with access to all functions.
 * Buttons can be browsed using TouchScreen, pushbuttons, potentiometer and QTouch.
 */

static SWGT_Widget* g_btnAudio ;
static SWGT_Widget* g_btnVideo ;
static SWGT_Widget* g_btnSlideShow ;
#if defined DEMO_USE_QTOUCH
static SWGT_Widget* g_btnQTouch ;
#endif // defined DEMO_USE_QTOUCH
static SWGT_Widget* g_btnLPModes ;

static uint32_t _ScrApplications_OnInitialize( SWGTScreen* pScreen )
{
    WGT_Screen_Initialize( pScreen, GUICLR_WHITE, NULL ) ;

    g_btnAudio=WGT_CreateWidget( WGT_TYPE_BUTTON, 14, 62, 64, 64 ) ;
    if ( g_btnAudio )
    {
        WGT_SetBitmap( g_btnAudio, DEMO_BMP_BTN_AUDIO ) ;
        WGT_Screen_AddWidget( pScreen, g_btnAudio ) ;
    }

    g_btnVideo=WGT_CreateWidget( WGT_TYPE_BUTTON, 92, 60, 64, 64 ) ;
    if ( g_btnVideo )
    {
        WGT_SetBitmap( g_btnVideo, DEMO_BMP_BTN_VIDEO ) ;
        WGT_Screen_AddWidget( pScreen, g_btnVideo ) ;
    }

    g_btnSlideShow=WGT_CreateWidget( WGT_TYPE_BUTTON, 166, 61, 70, 63 ) ;
    if ( g_btnSlideShow )
    {
        WGT_SetBitmap( g_btnSlideShow, DEMO_BMP_BTN_SLIDESHOW ) ;
        WGT_Screen_AddWidget( pScreen, g_btnSlideShow ) ;
    }

#if defined DEMO_USE_QTOUCH
    g_btnQTouch=WGT_CreateWidget( WGT_TYPE_BUTTON, 95, 165-26, 69, 69 ) ;
    if ( g_btnQTouch )
    {
        WGT_SetBitmap( g_btnQTouch, DEMO_BMP_BTN_QTOUCH ) ;
        WGT_Screen_AddWidget( pScreen, g_btnQTouch ) ;
    }

    g_btnLPModes=WGT_CreateWidget( WGT_TYPE_BUTTON, 184, 140, 47, 67 ) ;
    if ( g_btnLPModes )
    {
        WGT_SetBitmap( g_btnLPModes, DEMO_BMP_BTN_LOW_POWER_MODES ) ;
        WGT_Screen_AddWidget( pScreen, g_btnLPModes ) ;
    }
#else
    
    g_btnLPModes=WGT_CreateWidget( WGT_TYPE_BUTTON, 98, 140, 47, 67 ) ;
    if ( g_btnLPModes )
    {
    WGT_SetBitmap( g_btnLPModes, DEMO_BMP_BTN_LOW_POWER_MODES ) ;
    WGT_Screen_AddWidget( pScreen, g_btnLPModes ) ;
    }
#endif // defined DEMO_USE_QTOUCH

//    g_wgt_Status=WGT_CreateWidget( WGT_TYPE_TEXT, 0, 300, BOARD_LCD_WIDTH, 20 ) ;
//    if ( g_wgt_Status )
//    {
//        WGT_SetText( g_wgt_Status, NULL ) ;
//        WGT_SetBkgndColor( g_wgt_Status, GUICLR_WHITE ) ;
//        WGT_SetTextColor( g_wgt_Status, GUICLR_ATMEL_BLUE ) ;
//        WGT_Screen_AddWidget( pScreen, g_wgt_Status ) ;
//    }

// Call ACC hands-on init
    return SAMGUI_E_OK ;
}

static uint32_t _ScrApplications_HkDisableRTC( SWGTScreen* pScreen )
{
//    NVIC_DisableIRQ( RTC_IRQn ) ;
    RTC_DisableIt( RTC, RTC_IER_SECEN ) ;

    return SAMGUI_E_OK ;
}

static uint32_t _ScrApplications_HkEnableRTC( SWGTScreen* pScreen )
{
//    NVIC_EnableIRQ( RTC_IRQn ) ;
    RTC_EnableIt( RTC, RTC_IER_SECEN ) ;

    return SAMGUI_E_OK ;
}

//static uint32_t _ScrApplications_HkExit( SWGTScreen* pScreen )
//{
//    NVIC_DisableIRQ( RTC_IRQn ) ;
//
//    return SAMGUI_E_OK ;
//}

static uint32_t _ScrApplications_OnEraseBackground( SWGTScreen* pScreen )
{
     SGUIColor clr ;

    /* Draw Atmel logo */
//	g_WGT_CoreData.pBE->DrawBitmap( 4, 8, 87, 40, DEMO_BMP_LOGO_ATMEL ) ;
	g_WGT_CoreData.pBE->DrawBitmap( 0, 0, 240, 320, DEMO_BMP_BACKGROUND ) ;

    /* Draw labels */
    clr.u.dwRGBA=GUICLR_ATMEL_BLUE ;
    g_WGT_CoreData.pBE->DrawText( 105, 8, DEMO_LABEL"\nDemo home", &clr, (void*)&g_Font10x14, 0 ) ;

    return SAMGUI_E_OK ;
}

static uint32_t _ScrApplications_ProcessMessage( SWGTScreen* pScreen, SWGTCoreMessage* pMsg )
{
//    static uint32_t dwTimeUpdate=0 ;
    SWGT_Widget* pWidget ;

//    if ( pMsg->dwID != WGT_MSG_TIMER ) printf( "ScrMain - msg id %x\r\n", pMsg->dwID ) ;

    switch ( pMsg->dwID )
    {
//        case WGT_MSG_TIMER :
//            _ScrApplications_DrawStatus() ;
//
//            // Increment timestamp
//            dwTimeUpdate++ ;
//            if ( dwTimeUpdate == 60000 )
//            {
//                // Write parameters every 60 seconds
//                Demo_Parameters_CommitChanges() ;
//                dwTimeUpdate=0 ;
//            }
//        break ;

        case WGT_MSG_PAINT :
#if defined DEMO_USE_QTOUCH
            WGT_Screen_SetSelectedWidget( pScreen, g_btnQTouch ) ;
#else
            WGT_Screen_SetSelectedWidget( pScreen, g_btnAudio ) ;
#endif // defined DEMO_USE_QTOUCH

//            _ScrMain_DrawStatus() ;
        break ;

        case WGT_MSG_KEY_RELEASED : // button released
//        case WGT_MSG_KEY_PRESSED : // button pressed
            switch ( pMsg->dwParam1 )
            {
                // Select next widget
                case WGT_KEY_PB1 :
                case WGT_KEY_K4 :
                    if ( WGT_Screen_GetSelectedWidget( pScreen, &pWidget ) == SAMGUI_E_OK )
                    {
                        if ( pWidget )
                        {
                            if ( pWidget == g_btnAudio )
                            {
                                WGT_Screen_SetSelectedWidget( pScreen, g_btnVideo ) ;
                            }

                            if ( pWidget == g_btnVideo )
                            {
                                WGT_Screen_SetSelectedWidget( pScreen, g_btnSlideShow ) ;
                            }

#if defined DEMO_USE_QTOUCH
                            if ( pWidget == g_btnSlideShow )
                            {
                                WGT_Screen_SetSelectedWidget( pScreen, g_btnQTouch ) ;
                            }

                            if ( pWidget == g_btnQTouch )
                            {
                                WGT_Screen_SetSelectedWidget( pScreen, g_btnLPModes ) ;
                            }
#else
                            if ( pWidget == g_btnSlideShow )
                            {
                                WGT_Screen_SetSelectedWidget( pScreen, g_btnLPModes ) ;
                            }

#endif // defined DEMO_USE_QTOUCH

                            if ( pWidget == g_btnLPModes )
                            {
                                WGT_Screen_SetSelectedWidget( pScreen, g_btnAudio ) ;
                            }
                        }
                    }
                break ;

#if defined DEMO_USE_QTOUCH
                // Select previous widget
                case WGT_KEY_K2 :
                    if ( WGT_Screen_GetSelectedWidget( pScreen, &pWidget ) == SAMGUI_E_OK )
                    {
                        if ( pWidget )
                        {
                            if ( pWidget == g_btnAudio )
                            {
                                WGT_Screen_SetSelectedWidget( pScreen, g_btnLPModes ) ;
                            }

                            if ( pWidget == g_btnVideo )
                            {
                                WGT_Screen_SetSelectedWidget( pScreen, g_btnAudio ) ;
                            }

                            if ( pWidget == g_btnSlideShow )
                            {
                                WGT_Screen_SetSelectedWidget( pScreen, g_btnVideo ) ;
                            }

                            if ( pWidget == g_btnQTouch )
                            {
                                WGT_Screen_SetSelectedWidget( pScreen, g_btnSlideShow ) ;
                            }

                            if ( pWidget == g_btnLPModes )
                            {
                                WGT_Screen_SetSelectedWidget( pScreen, g_btnQTouch ) ;
                            }
                        }
                    }
                break ;
#endif //defined DEMO_USE_QTOUCH

#if defined DEMO_USE_QTOUCH
                // Select up widget
                case WGT_KEY_K1 :
                    if ( WGT_Screen_GetSelectedWidget( pScreen, &pWidget ) == SAMGUI_E_OK )
                    {
                        if ( pWidget )
                        {
                            if ( pWidget == g_btnAudio )
                            {
                            }

                            if ( pWidget == g_btnVideo )
                            {
                            }

                            if ( pWidget == g_btnSlideShow )
                            {
                            }

                            if ( pWidget == g_btnQTouch )
                            {
                                WGT_Screen_SetSelectedWidget( pScreen, g_btnVideo ) ;
                            }

                            if ( pWidget == g_btnLPModes )
                            {
                                WGT_Screen_SetSelectedWidget( pScreen, g_btnSlideShow ) ;
                            }
                        }
                    }
                break ;
#endif // defined DEMO_USE_QTOUCH

#if defined DEMO_USE_QTOUCH
                // Select down widget
                case WGT_KEY_K5 :
                    if ( WGT_Screen_GetSelectedWidget( pScreen, &pWidget ) == SAMGUI_E_OK )
                    {
                        if ( pWidget )
                        {
                            if ( pWidget == g_btnAudio )
                            {
                            }

                            if ( pWidget == g_btnVideo )
                            {
                                WGT_Screen_SetSelectedWidget( pScreen, g_btnQTouch ) ;
                            }

                            if ( pWidget == g_btnSlideShow )
                            {
                                WGT_Screen_SetSelectedWidget( pScreen, g_btnLPModes ) ;
                            }

                            if ( pWidget == g_btnQTouch )
                            {
                            }

                            if ( pWidget == g_btnLPModes )
                            {
                            }
                        }
                    }
                break ;
#endif // defined DEMO_USE_QTOUCH

                // Enter
                case WGT_KEY_PB2 :
                case WGT_KEY_K3 :
                    if ( WGT_Screen_GetSelectedWidget( pScreen, &pWidget ) == SAMGUI_E_OK )
                    {
                        if ( pWidget != NULL )
                        {
//                            printf( "ScrMain - Sending selected\r\n" ) ;
                            WGT_PostMessage( WGT_MSG_WIDGET_SELECTED, (uint32_t)pWidget, 0 ) ;
                        }
                    }
                break ;
            }
        break ;

        case WGT_MSG_KEY_PRESSED : // button pressed
            switch ( pMsg->dwParam1 )
            {
                case WGT_KEY_S1 :
                case WGT_KEY_VR1 :
//                    printf( "ScrMain - pot %u %u %u\r\n", pMsg->dwParam2, pMsg->dwParam2/7, ((pMsg->dwParam2*7)/256) ) ;

#if defined DEMO_USE_QTOUCH
                    switch ( ((pMsg->dwParam2*7)/256) )
#else
                    switch ( ((pMsg->dwParam2*6)/256) )
#endif // defined DEMO_USE_QTOUCH
                    {
                        case 0 :
                            WGT_Screen_SetSelectedWidget( pScreen, g_btnAudio ) ;
                        break ;

                        case 1 :
                            WGT_Screen_SetSelectedWidget( pScreen, g_btnVideo ) ;
                        break ;

                        case 2 :
                            WGT_Screen_SetSelectedWidget( pScreen, g_btnSlideShow ) ;
                        break ;

#if defined DEMO_USE_QTOUCH
                        case 4 :
                            WGT_Screen_SetSelectedWidget( pScreen, g_btnQTouch ) ;
                        break ;

                        case 5 :
                            WGT_Screen_SetSelectedWidget( pScreen, g_btnLPModes ) ;
                        break ;
#else
                        case 4 :
                            WGT_Screen_SetSelectedWidget( pScreen, g_btnLPModes ) ;
                        break ;
#endif // defined DEMO_USE_QTOUCH
                    }
                break ;
            }
        break ;

        case WGT_MSG_WIDGET_SELECTED :
//            printf( "ScrMain - switching screen\r\n" ) ;
            if ( pMsg->dwParam1 == (uint32_t)g_btnVideo )
            {
//                WGT_SetCurrentScreen( &ScrCube ) ;
//                WGT_SetCurrentScreen( &ScrStarfield ) ;
                WGT_SetCurrentScreen( &ScrDISPDemo ) ;
            }

            if ( pMsg->dwParam1 == (uint32_t)g_btnSlideShow )
            {
                WGT_SetCurrentScreen( &ScrPPT ) ;
            }

            if ( pMsg->dwParam1 == (uint32_t)g_btnAudio )
            {
                WGT_SetCurrentScreen( &ScrAudio ) ;
            }

#if defined DEMO_USE_QTOUCH
            if ( pMsg->dwParam1 == (uint32_t)g_btnQTouch )
            {
                WGT_SetCurrentScreen( &ScrQTouch ) ;
            }
#endif // defined DEMO_USE_QTOUCH

            if ( pMsg->dwParam1 == (uint32_t)g_btnLPModes )
            {
                WGT_SetCurrentScreen( &ScrLPModes ) ;
            }
        break ;
    }

    return SAMGUI_E_OK ;
}

SWGTScreen ScrApplications=
{
    .ProcessMessage=_ScrApplications_ProcessMessage,

    /* Hooks */
    .HkBeforePaint=_ScrApplications_HkDisableRTC,
    .HkAfterPaint=_ScrApplications_HkEnableRTC,
    .HkBeforeEraseBackground=_ScrApplications_HkDisableRTC,
    .HkAfterEraseBackground=_ScrApplications_HkEnableRTC,
    .HkExit=_ScrApplications_HkDisableRTC, //_ScrMain_HkExit,

    /* Callbacks */
    .OnInitialize=_ScrApplications_OnInitialize,
    .OnEraseBackground=_ScrApplications_OnEraseBackground,
    .OnPaint=NULL
} ;

/** @} */
/** @} */
