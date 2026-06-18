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
#include <math.h>

/**
 * \addtogroup screens
 * @{
 * \addtogroup screen_main Main screen
 * @{
 *
 * \brief This screen is the main screen with access to all functions.
 * Buttons can be browsed using TouchScreen, pushbuttons, potentiometer and QTouch.
 */

static __no_init SWGT_Widget* g_btnAudio ;
static __no_init SWGT_Widget* g_btnVideo ;
static __no_init SWGT_Widget* g_btnSlideShow ;
#if defined DEMO_USE_QTOUCH
static __no_init SWGT_Widget* g_btnQTouch ;
#endif // defined DEMO_USE_QTOUCH
static __no_init SWGT_Widget* g_btnLPModes ;
static __no_init SWGT_Widget* g_btnSettings ;
static __no_init SWGT_Widget* g_btnInfos ;
static __no_init SWGT_Widget* g_wgt_Status ;

static __no_init char szStatusText[20] ;

#define M_PI                3.14159265358979323846
//#define M_TWOPI             (M_PI * 2.0)

//#define CLOCK_X              (225-21)
//#define CLOCK_Y              (305-21)
#define CLOCK_X              (240-32-5)
#define CLOCK_Y              (320-32-5)
#define CLOCK_RADIUS         16
#define CLOCK_RADIUS_HOUR    (CLOCK_RADIUS>>1)
#define CLOCK_RADIUS_MINUTE  ((CLOCK_RADIUS>>1)+(CLOCK_RADIUS>>2))
#define CLOCK_RADIUS_SECOND  (CLOCK_RADIUS-3)

static float sinx(float x)
{
   float result=x,temp=x;
   float den=x,fac=1;
   int n=1,sign=1;
   while((temp>1e-5)||(temp<-1e-5))        
   {
       n++,fac*=n,den*=x;
       n++,fac*=n,den*=x;
       temp=den/fac;sign=-sign;
       result=sign>0?result+temp:result-temp;
   }
   return result;
}

static float cosx(float x)
{
   x=1.57079-x;
   return sinx(x);
}   

static void _ScrMain_DrawStatus( void )
{
    uint8_t ucHour, ucMinute, ucSecond ;
#if 0
    uint16_t wYear ;
    uint8_t ucMonth, ucDay ;
#endif // 0

    /* Retrieve date and time */
    RTC_GetTime( RTC, &ucHour, &ucMinute, &ucSecond ) ;

#if 0
    RTC_GetDate( RTC, &wYear, &ucMonth, &ucDay, NULL ) ;

    snprintf( szStatusText, sizeof( szStatusText ), "%02u/%02u/%02u %02u:%02u:%02u",
              ucMonth, ucDay, wYear-2000, ucHour, ucMinute, ucSecond ) ;

    /* Display */
    WGT_SetText( &g_wgt_Status, szStatusText ) ;
    WGT_Draw( &g_wgt_Status, g_WGT_CoreData.pBE ) ;
#else
    {
        SGUIColor clr ;
        int32_t dwX ;
        int32_t dwY ;
        float fAngle ;

        /* Display digital clock */
        snprintf( szStatusText, sizeof( szStatusText ), "%02u:%02u:%02u",
                  ucHour, ucMinute, ucSecond ) ;
        WGT_SetText( g_wgt_Status, szStatusText ) ;
        WGT_Draw( g_wgt_Status, g_WGT_CoreData.pBE ) ;
#if 0
        /* Display analog clock */
        g_WGT_CoreData.pBE->DrawBitmap( CLOCK_X, CLOCK_Y, 32, 32, DEMO_BMP_CLOCK ) ;
        /* Draw hour */
        clr.u.dwRGBA=GUICLR_BLACK ;
        if ( ucHour > 12 )
        {
            ucHour-=12 ;
        }
        fAngle=((ucHour*5)+(ucMinute/12)-15)*M_PI/30.0f ;
        dwX=(int32_t)( CLOCK_RADIUS_HOUR*cosx( fAngle ) ) ;
        dwY=(int32_t)( CLOCK_RADIUS_HOUR*sinx( fAngle ) ) ;
        g_WGT_CoreData.pBE->DrawLine( CLOCK_X+CLOCK_RADIUS, CLOCK_Y+CLOCK_RADIUS, CLOCK_X+CLOCK_RADIUS+dwX, CLOCK_Y+CLOCK_RADIUS+dwY, &clr ) ;

        /* Draw minutes */
        clr.u.dwRGBA=GUICLR_BLACK ;
        fAngle=(ucMinute-15)*M_PI/30.0f ;
        dwX=(int32_t)( CLOCK_RADIUS_MINUTE*cosx( fAngle ) ) ;
        dwY=(int32_t)( CLOCK_RADIUS_MINUTE*sinx( fAngle ) ) ;
        g_WGT_CoreData.pBE->DrawLine( CLOCK_X+CLOCK_RADIUS, CLOCK_Y+CLOCK_RADIUS, CLOCK_X+CLOCK_RADIUS+dwX, CLOCK_Y+CLOCK_RADIUS+dwY, &clr ) ;

        /* Draw seconds */
        clr.u.dwRGBA=GUICLR_BLACK ;
        fAngle=(ucSecond-15)*M_PI/30.0f ;
        dwX=(int32_t)( CLOCK_RADIUS_SECOND*cosx( fAngle ) ) ;
        dwY=(int32_t)( CLOCK_RADIUS_SECOND*sinx( fAngle ) ) ;
        g_WGT_CoreData.pBE->DrawLine( CLOCK_X+CLOCK_RADIUS, CLOCK_Y+CLOCK_RADIUS, CLOCK_X+CLOCK_RADIUS+dwX, CLOCK_Y+CLOCK_RADIUS+dwY, &clr ) ;

#endif
    }
#endif
}

static uint32_t _ScrMain_OnInitialize( SWGTScreen* pScreen )
{
    WGT_Screen_Initialize( pScreen, GUICLR_WHITE, NULL ) ;

    g_btnAudio=WGT_CreateWidget( WGT_TYPE_BUTTON, 14, 60, 64, 64 ) ;
    if ( g_btnAudio )
    {
        WGT_SetBitmap( g_btnAudio, DEMO_BMP_BTN_AUDIO ) ;
        WGT_Screen_AddWidget( pScreen, g_btnAudio ) ;
    }

    g_btnVideo=WGT_CreateWidget( WGT_TYPE_BUTTON, 88, 60, 64, 64 ) ;
    if ( g_btnVideo )
    {
//        WGT_SetBitmap( &g_btnVideo, DEMO_BMP_BTN_VIDEO ) ;
        WGT_SetBitmap( g_btnVideo, DEMO_BMP_BTN_GFX_DEMO ) ;
        WGT_Screen_AddWidget( pScreen, g_btnVideo ) ;
    }

    g_btnSlideShow=WGT_CreateWidget( WGT_TYPE_BUTTON, 162, 60, 64, 64 ) ;
    if ( g_btnSlideShow )
    {
        WGT_SetBitmap( g_btnSlideShow, DEMO_BMP_BTN_SLIDESHOW ) ;
        WGT_Screen_AddWidget( pScreen, g_btnSlideShow ) ;
    }

    g_btnSettings=WGT_CreateWidget( WGT_TYPE_BUTTON, 14, 140, 64, 64 ) ;
    if ( g_btnSettings )
    {
        WGT_SetBitmap( g_btnSettings, DEMO_BMP_BTN_SETTINGS ) ;
        WGT_Screen_AddWidget( pScreen, g_btnSettings ) ;
    }

#if defined DEMO_USE_QTOUCH
    g_btnQTouch=WGT_CreateWidget( WGT_TYPE_BUTTON, 88, 140, 64, 64 ) ;
    if ( g_btnQTouch )
    {
        WGT_SetBitmap( g_btnQTouch, DEMO_BMP_BTN_QTOUCH ) ;
        WGT_Screen_AddWidget( pScreen, g_btnQTouch ) ;
    }

    g_btnLPModes=WGT_CreateWidget( WGT_TYPE_BUTTON, 162, 140, 64, 64 ) ;
    if ( g_btnLPModes )
    {
        WGT_SetBitmap( g_btnLPModes, DEMO_BMP_BTN_LOW_POWER_MODES ) ;
        WGT_Screen_AddWidget( pScreen, g_btnLPModes ) ;
    }

    g_btnInfos=WGT_CreateWidget( WGT_TYPE_BUTTON, 88, 220, 64, 64 ) ;
    if ( g_btnInfos )
    {
        WGT_SetBitmap( g_btnInfos, DEMO_BMP_BTN_INFOS ) ;
        WGT_Screen_AddWidget( pScreen, g_btnInfos ) ;
    }
#else
    g_btnLPModes=WGT_CreateWidget( WGT_TYPE_BUTTON, 88, 140, 64, 64 ) ;
    if ( g_btnLPModes )
    {
        WGT_SetBitmap( g_btnLPModes, DEMO_BMP_BTN_LOW_POWER_MODES ) ;
        WGT_Screen_AddWidget( pScreen, g_btnLPModes ) ;
    }

    g_btnInfos=WGT_CreateWidget( WGT_TYPE_BUTTON, 162, 140, 64, 64 ) ;
    if ( g_btnInfos )
    {
        WGT_SetBitmap( g_btnInfos, DEMO_BMP_BTN_INFOS ) ;
        WGT_Screen_AddWidget( pScreen, g_btnInfos ) ;
    }
#endif // defined DEMO_USE_QTOUCH

    g_wgt_Status=WGT_CreateWidget( WGT_TYPE_TEXT, 0, 300, BOARD_LCD_WIDTH-50, 20 ) ;
    if ( g_wgt_Status )
    {
        WGT_SetText( g_wgt_Status, NULL ) ;
        WGT_SetBkgndColor( g_wgt_Status, GUICLR_WHITE ) ;
        WGT_SetTextColor( g_wgt_Status, GUICLR_ATMEL_BLUE ) ;
        WGT_Screen_AddWidget( pScreen, g_wgt_Status ) ;
    }

// Call ACC hands-on init
    return SAMGUI_E_OK ;
}

static uint32_t _ScrMain_OnEraseBackground( SWGTScreen* pScreen )
{
     SGUIColor clr ;
//     SGUIColor clrBlack ;

    /* Draw Atmel logo */
	g_WGT_CoreData.pBE->DrawBitmap( 4, 8, 87, 40, DEMO_BMP_LOGO_ATMEL ) ;
//	g_WGT_CoreData.pBE->DrawBitmap( 0, 0, 240, 320, DEMO_BMP_BACKGROUND ) ;

    /* Draw labels */
    clr.u.dwRGBA=GUICLR_ATMEL_BLUE ;
//    clr.u.dwRGBA=GUICLR_WHITE ;
    g_WGT_CoreData.pBE->DrawText( 105, 8, DEMO_LABEL"\nDemo", &clr, (void*)&g_Font10x14, 0 ) ;

//    clr.u.dwRGBA=GUICLR_ATMEL_BLUE ;
//    clrBlack.u.dwRGBA=GUICLR_BLACK ;
//	g_WGT_CoreData.pBE->DrawFilledCircle( CLOCK_X, CLOCK_Y, CLOCK_RADIUS, &clrBlack, &clr ) ;
//	g_WGT_CoreData.pBE->DrawBitmap( CLOCK_X, CLOCK_Y, 32, 32, DEMO_BMP_CLOCK ) ;

    return SAMGUI_E_OK ;
}

static uint32_t _ScrMain_ProcessMessage( SWGTScreen* pScreen, SWGTCoreMessage* pMsg )
{
    static uint32_t dwTimeUpdate=0 ;
    SWGT_Widget* pWidget ;

    switch ( pMsg->dwID )
    {
        case WGT_MSG_TIMER :
            _ScrMain_DrawStatus() ;

            // Increment timestamp
            dwTimeUpdate++ ;
            if ( dwTimeUpdate == 60000 )
            {
                // Write parameters every 60 seconds
                Demo_Parameters_CommitChanges() ;
                dwTimeUpdate=0 ;
            }
        break ;

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

                            if ( pWidget == g_btnSlideShow )
                            {
                                WGT_Screen_SetSelectedWidget( pScreen, g_btnSettings ) ;
                            }

#if defined DEMO_USE_QTOUCH
                            if ( pWidget == g_btnSettings )
                            {
                                WGT_Screen_SetSelectedWidget( pScreen, g_btnQTouch ) ;
                            }

                            if ( pWidget == g_btnQTouch )
                            {
                                WGT_Screen_SetSelectedWidget( pScreen, g_btnLPModes ) ;
                            }
#else
                            if ( pWidget == g_btnSettings )
                            {
                                WGT_Screen_SetSelectedWidget( pScreen, g_btnLPModes ) ;
                            }
#endif // defined DEMO_USE_QTOUCH

                            if ( pWidget == g_btnLPModes )
                            {
                                WGT_Screen_SetSelectedWidget( pScreen, g_btnInfos ) ;
                            }

                            if ( pWidget == g_btnInfos )
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
                                WGT_Screen_SetSelectedWidget( pScreen, g_btnInfos ) ;
                            }

                            if ( pWidget == g_btnVideo )
                            {
                                WGT_Screen_SetSelectedWidget( pScreen, g_btnAudio ) ;
                            }

                            if ( pWidget == g_btnSlideShow )
                            {
                                WGT_Screen_SetSelectedWidget( pScreen, g_btnVideo ) ;
                            }

                            if ( pWidget == g_btnSettings )
                            {
                                WGT_Screen_SetSelectedWidget( pScreen, g_btnSlideShow ) ;
                            }

                            if ( pWidget == g_btnQTouch )
                            {
                                WGT_Screen_SetSelectedWidget( pScreen, g_btnSettings ) ;
                            }

                            if ( pWidget == g_btnLPModes )
                            {
                                WGT_Screen_SetSelectedWidget( pScreen, g_btnQTouch ) ;
                            }

                            if ( pWidget == g_btnInfos )
                            {
                                WGT_Screen_SetSelectedWidget( pScreen, g_btnLPModes ) ;
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
                                WGT_Screen_SetSelectedWidget( pScreen, g_btnInfos ) ;
                            }

                            if ( pWidget == g_btnVideo )
                            {
                                WGT_Screen_SetSelectedWidget( pScreen, g_btnInfos ) ;
                            }

                            if ( pWidget == g_btnSlideShow )
                            {
                                WGT_Screen_SetSelectedWidget( pScreen, g_btnInfos ) ;
                            }

                            if ( pWidget == g_btnSettings )
                            {
                                WGT_Screen_SetSelectedWidget( pScreen, g_btnAudio ) ;
                            }

                            if ( pWidget == g_btnQTouch )
                            {
                                WGT_Screen_SetSelectedWidget( pScreen, g_btnVideo ) ;
                            }

                            if ( pWidget == g_btnLPModes )
                            {
                                WGT_Screen_SetSelectedWidget( pScreen, g_btnSlideShow ) ;
                            }

                            if ( pWidget == g_btnInfos )
                            {
                                WGT_Screen_SetSelectedWidget( pScreen, g_btnQTouch ) ;
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
                                WGT_Screen_SetSelectedWidget( pScreen, g_btnSettings ) ;
                            }

                            if ( pWidget == g_btnVideo )
                            {
                                WGT_Screen_SetSelectedWidget( pScreen, g_btnQTouch ) ;
                            }

                            if ( pWidget == g_btnSlideShow )
                            {
                                WGT_Screen_SetSelectedWidget( pScreen, g_btnLPModes ) ;
                            }

                            if ( pWidget == g_btnSettings )
                            {
                                WGT_Screen_SetSelectedWidget( pScreen, g_btnInfos ) ;
                            }

                            if ( pWidget == g_btnQTouch )
                            {
                                WGT_Screen_SetSelectedWidget( pScreen, g_btnInfos ) ;
                            }

                            if ( pWidget == g_btnLPModes )
                            {
                                WGT_Screen_SetSelectedWidget( pScreen, g_btnInfos ) ;
                            }

                            if ( pWidget == g_btnInfos )
                            {
                                WGT_Screen_SetSelectedWidget( pScreen, g_btnVideo ) ;
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

                        case 3 :
                            WGT_Screen_SetSelectedWidget( pScreen, g_btnSettings ) ;
                        break ;

#if defined DEMO_USE_QTOUCH
                        case 4 :
                            WGT_Screen_SetSelectedWidget( pScreen, g_btnQTouch ) ;
                        break ;

                        case 5 :
                            WGT_Screen_SetSelectedWidget( pScreen, g_btnLPModes ) ;
                        break ;

                        case 6 :
                            WGT_Screen_SetSelectedWidget( pScreen, g_btnInfos ) ;
                        break ;
#else
                        case 4 :
                            WGT_Screen_SetSelectedWidget( pScreen, g_btnLPModes ) ;
                        break ;

                        case 5 :
                            WGT_Screen_SetSelectedWidget( pScreen, g_btnInfos ) ;
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
//                WGT_SetCurrentScreen( &ScrMandelbrot ) ;
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

            if ( pMsg->dwParam1 == (uint32_t)g_btnSettings )
            {
                WGT_SetCurrentScreen( &ScrSettings ) ;
            }

            if ( pMsg->dwParam1 == (uint32_t)g_btnInfos )
            {
                WGT_SetCurrentScreen( &ScrInfos ) ;
            }
        break ;
    }

    return SAMGUI_E_OK ;
}

SWGTScreen ScrMain=
{
    .ProcessMessage=_ScrMain_ProcessMessage,

    /* Hooks */
    .HkBeforePaint=NULL,
    .HkAfterPaint=NULL,
    .HkBeforeEraseBackground=NULL,
    .HkAfterEraseBackground=NULL,
    .HkExit=NULL,

    /* Callbacks */
    .OnInitialize=_ScrMain_OnInitialize,
    .OnEraseBackground=_ScrMain_OnEraseBackground,
    .OnPaint=NULL
} ;

/** @} */
/** @} */
