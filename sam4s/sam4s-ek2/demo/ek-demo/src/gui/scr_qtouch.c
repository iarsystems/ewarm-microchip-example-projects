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

#include <stdio.h>

/**
 * \addtogroup screens
 * @{
 * \addtogroup screen_qtouch Qtouch screen
 * @{
 *
 * \brief This screen aims at demonstrate QTouch capabilities.
 */

static __no_init SWGT_Widget* g_btnK1 ;
static __no_init SWGT_Widget* g_btnK2 ;
static __no_init SWGT_Widget* g_btnK3 ;
static __no_init SWGT_Widget* g_btnK4 ;
static __no_init SWGT_Widget* g_btnK5 ;
static __no_init SWGT_Widget* g_btnBack ;

static __no_init SGUIColor gs_ProgressInside ;
static __no_init SGUIColor gs_ProgressBorder ;

#define QTOUCH_BMP_SIZE                48
#define QTOUCH_X                       140
#define QTOUCH_Y                       120
#define QTOUCH_WIDTH                   (48+10)
#define QTOUCH_PROGRESS_WIDTH          150

static void _ScrQTouch_DrawProgress( uint32_t dwX, uint32_t dwY, uint32_t dwWidth, uint32_t dwHeight,
                                     uint32_t dwValue, uint32_t dwValueMax, SGUIColor* pColorInside, SGUIColor* pColorBorder )
{
    SGUIColor clrWhite={ .u.dwRGBA=GUICLR_WHITE } ;

    g_WGT_CoreData.pBE->DrawFilledRectangle( dwX, dwY, dwX+dwWidth, dwY+dwHeight-1, NULL, &clrWhite ) ;
    g_WGT_CoreData.pBE->DrawFilledRectangle( dwX, dwY, dwX+(dwValue*dwWidth)/dwValueMax, dwY+dwHeight-1, NULL, pColorInside ) ;
    g_WGT_CoreData.pBE->DrawRectangle( dwX, dwY, dwX+dwWidth, dwY+dwHeight-1, pColorBorder ) ;
}

static uint32_t _ScrQTouch_OnInitialize( SWGTScreen* pScreen )
{
    WGT_Screen_Initialize( pScreen, GUICLR_WHITE, NULL ) ;

    // K1
    g_btnK1=WGT_CreateWidget( WGT_TYPE_BUTTON, QTOUCH_X-(QTOUCH_BMP_SIZE>>1), QTOUCH_Y-QTOUCH_WIDTH, 48, 48 ) ;
    if ( g_btnK1 )
    {
        WGT_SetBitmap( g_btnK1, DEMO_BMP_BTN_QT_UP ) ;
        WGT_Screen_AddWidget( pScreen, g_btnK1 ) ;
    }

    // K2
    g_btnK2=WGT_CreateWidget( WGT_TYPE_BUTTON, QTOUCH_X-(QTOUCH_BMP_SIZE>>1)-QTOUCH_WIDTH, QTOUCH_Y, 48, 48 ) ;
    if ( g_btnK2 )
    {
        WGT_SetBitmap( g_btnK2, DEMO_BMP_BTN_QT_PREV ) ;
        WGT_Screen_AddWidget( pScreen, g_btnK2 ) ;
    }

    // K3
    g_btnK3=WGT_CreateWidget( WGT_TYPE_BUTTON, QTOUCH_X-(QTOUCH_BMP_SIZE>>1), QTOUCH_Y, 48, 48 ) ;
    if ( g_btnK3 )
    {
        WGT_SetBitmap( g_btnK3, DEMO_BMP_BTN_QT_ENTER ) ;
        WGT_Screen_AddWidget( pScreen, g_btnK3 ) ;
    }

    // K4
    g_btnK4=WGT_CreateWidget( WGT_TYPE_BUTTON, QTOUCH_X-(QTOUCH_BMP_SIZE>>1)+QTOUCH_WIDTH, QTOUCH_Y, 48, 48 ) ;
    if ( g_btnK4 )
    {
        WGT_SetBitmap( g_btnK4, DEMO_BMP_BTN_QT_NEXT ) ;
        WGT_Screen_AddWidget( pScreen, g_btnK4 ) ;
    }

    // K5
    g_btnK5=WGT_CreateWidget( WGT_TYPE_BUTTON, QTOUCH_X-(QTOUCH_BMP_SIZE>>1), QTOUCH_Y+QTOUCH_WIDTH, 48, 48 ) ;
    if ( g_btnK5 )
    {
        WGT_SetBitmap( g_btnK5, DEMO_BMP_BTN_QT_DOWN ) ;
        WGT_Screen_AddWidget( pScreen, g_btnK5 ) ;
    }

    // Back
    g_btnBack=WGT_CreateWidget( WGT_TYPE_BUTTON, 12, 252, 48, 48 ) ;
    if ( g_btnBack )
    {
        WGT_SetBitmap( g_btnBack, DEMO_BMP_BTN_BACK ) ;
        WGT_Screen_AddWidget( pScreen, g_btnBack ) ;
    }

    gs_ProgressInside.u.dwRGBA=GUICLR_ATMEL_BLUE ;
    gs_ProgressBorder.u.dwRGBA=GUICLR_BLACK ;

    return SAMGUI_E_OK ;
}

static uint32_t _ScrQTouch_OnDrawBackground( SWGTScreen* pScreen )
{
    SGUIColor clr ;

    // Draw Atmel logo
	g_WGT_CoreData.pBE->DrawBitmap( 4, 8, 87, 40, DEMO_BMP_LOGO_ATMEL ) ;

    // Draw labels
    clr.u.dwRGBA=GUICLR_ATMEL_BLUE ;
    g_WGT_CoreData.pBE->DrawText( 105, 8, DEMO_LABEL"\nQTouch", &clr, (void*)&g_Font10x14, 0 ) ;

    _ScrQTouch_DrawProgress( QTOUCH_X-(QTOUCH_PROGRESS_WIDTH>>1), QTOUCH_Y+(QTOUCH_WIDTH<<1),
                             QTOUCH_PROGRESS_WIDTH, QTOUCH_BMP_SIZE, 0, 255,
                             &gs_ProgressInside, &gs_ProgressBorder ) ;

    return SAMGUI_E_OK ;
}

static uint32_t _ScrQTouch_ProcessMessage( SWGTScreen* pScreen, SWGTCoreMessage* pMsg )
{
//    static uint32_t dwTime=0 ;

//    printf( "ScrQTouch - msg id %x\r\n", pMsg->dwID ) ;

    switch ( pMsg->dwID )
    {
        case WGT_MSG_WIDGET_SELECTED :
          if ( pMsg->dwParam1 == (uint32_t)g_btnBack )
          {
              WGT_SetCurrentScreen( &ScrMain ) ;
          }

          if ( pMsg->dwParam1 == (uint32_t)g_btnK1 )
          {
          }

          if ( pMsg->dwParam1 == (uint32_t)g_btnK2 )
          {
          }

          if ( pMsg->dwParam1 == (uint32_t)g_btnK3 )
          {
          }

          if ( pMsg->dwParam1 == (uint32_t)g_btnK4 )
          {
          }

          if ( pMsg->dwParam1 == (uint32_t)g_btnK5 )
          {
          }
        break ;

        case WGT_MSG_KEY_PRESSED : // button pressed
            switch ( pMsg->dwParam1 )
            {
                case WGT_KEY_K1 :
                    // Draw K1
                    WGT_SetBitmap( g_btnK1, DEMO_BMP_BTN_QT_UP_PRESSED ) ;
                    WGT_Draw( g_btnK1, g_WGT_CoreData.pBE ) ;
                break ;

                case WGT_KEY_K2 :
                    // Draw K2
                    WGT_SetBitmap( g_btnK2, DEMO_BMP_BTN_QT_PREV_PRESSED ) ;
                    WGT_Draw( g_btnK2, g_WGT_CoreData.pBE ) ;
                break ;

                case WGT_KEY_PB2 :
                case WGT_KEY_K3 :
                    // Draw K3
                    WGT_SetBitmap( g_btnK3, DEMO_BMP_BTN_QT_ENTER_PRESSED ) ;
                    WGT_Draw( g_btnK3, g_WGT_CoreData.pBE ) ;
                break ;

                case WGT_KEY_PB1 :
                case WGT_KEY_K4 :
                    // Draw K4
                    WGT_SetBitmap( g_btnK4, DEMO_BMP_BTN_QT_NEXT_PRESSED ) ;
                    WGT_Draw( g_btnK4, g_WGT_CoreData.pBE ) ;
                break ;

                case WGT_KEY_K5 :
                    // Draw K5
                    WGT_SetBitmap( g_btnK5, DEMO_BMP_BTN_QT_DOWN_PRESSED ) ;
                    WGT_Draw( g_btnK5, g_WGT_CoreData.pBE ) ;
                break ;

                case WGT_KEY_VR1 :
                case WGT_KEY_S1 :
//                    printf( "progress value : %u\r\n", pMsg->dwParam2 ) ;
                    if ( pMsg->dwParam2 > 255 )
                    {
                        pMsg->dwParam2=255 ;
                    }

                    // Draw ProgressBar
                    _ScrQTouch_DrawProgress( QTOUCH_X-(QTOUCH_PROGRESS_WIDTH>>1), QTOUCH_Y+(QTOUCH_WIDTH<<1),
                                             QTOUCH_PROGRESS_WIDTH, QTOUCH_BMP_SIZE, pMsg->dwParam2, 255,
                                             &gs_ProgressInside, &gs_ProgressBorder ) ;
                break ;
            }
        break ;

        case WGT_MSG_KEY_RELEASED : // button released
            switch ( pMsg->dwParam1 )
            {
                case WGT_KEY_K1 :
                    // Draw K1
                    WGT_SetBitmap( g_btnK1, DEMO_BMP_BTN_QT_UP ) ;
                    WGT_Draw( g_btnK1, g_WGT_CoreData.pBE ) ;
                break ;

                case WGT_KEY_K2 :
                    // Draw K2
                    WGT_SetBitmap( g_btnK2, DEMO_BMP_BTN_QT_PREV ) ;
                    WGT_Draw( g_btnK2, g_WGT_CoreData.pBE ) ;
                break ;

                case WGT_KEY_PB2 :
                case WGT_KEY_K3 :
                    // Draw K3
                    WGT_SetBitmap( g_btnK3, DEMO_BMP_BTN_QT_ENTER ) ;
                    WGT_Draw( g_btnK3, g_WGT_CoreData.pBE ) ;
                break ;

                case WGT_KEY_PB1 :
                case WGT_KEY_K4 :
                    // Draw K4
                    WGT_SetBitmap( g_btnK4, DEMO_BMP_BTN_QT_NEXT ) ;
                    WGT_Draw( g_btnK4, g_WGT_CoreData.pBE ) ;
                break ;

                case WGT_KEY_K5 :
                    // Draw K5
                    WGT_SetBitmap( g_btnK5, DEMO_BMP_BTN_QT_DOWN ) ;
                    WGT_Draw( g_btnK5, g_WGT_CoreData.pBE ) ;
                break ;
            }
        break ;
    }

    return SAMGUI_E_OK ;
}

extern SWGTScreen ScrQTouch=
{
    .ProcessMessage=_ScrQTouch_ProcessMessage,

    /* Hooks */
    .HkBeforePaint=NULL,
    .HkAfterPaint=NULL,
    .HkBeforeEraseBackground=NULL,
    .HkAfterEraseBackground=NULL,
    .HkExit=NULL,

    /* Callbacks */
    .OnInitialize=_ScrQTouch_OnInitialize,
    .OnEraseBackground=_ScrQTouch_OnDrawBackground,
    .OnPaint=NULL
} ;

/** @} */
/** @} */
