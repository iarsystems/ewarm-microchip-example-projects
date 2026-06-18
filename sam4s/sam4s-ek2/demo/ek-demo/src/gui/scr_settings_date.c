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

/**
 * \addtogroup screens
 * @{
 * \addtogroup settings Settings
 * @{
 * \addtogroup screen_settings_date Date settings screen
 * @{
 *
 * \brief This screen allows configuration of system date.
 */

static __no_init SWGT_Widget* g_btnUpDay ;
static __no_init SWGT_Widget* g_btnDownDay ;
static __no_init SWGT_Widget* g_wgt_Day ;

static __no_init SWGT_Widget* g_btnUpMonth ;
static __no_init SWGT_Widget* g_btnDownMonth ;
static __no_init SWGT_Widget* g_wgt_Month ;

static __no_init SWGT_Widget* g_btnUpYear ;
static __no_init SWGT_Widget* g_btnDownYear ;
static __no_init SWGT_Widget* g_wgt_Year ;

static __no_init SWGT_Widget* g_btnBack ;

static __no_init char gs_szDay[3] ;
static __no_init char gs_szMonth[3] ;
static __no_init char gs_szYear[3] ;
static __no_init uint32_t gs_dwDay ;
static __no_init uint32_t gs_dwMonth ;
static __no_init uint32_t gs_dwYear ;

static void _ScrSettings_Date_SetDay( void )
{
    // Set Day text
    snprintf( gs_szDay, sizeof( gs_szDay ), "%02u", gs_dwDay ) ;
    WGT_SetText( g_wgt_Day, gs_szDay ) ;
//    RTC_SetDate( g_demo_parameters.dwYear, g_demo_parameters.dwMonth, g_demo_parameters.dwDay, 0 ) ;
}

static void _ScrSettings_Date_SetMonth( void )
{
    // Set Month text
    snprintf( gs_szMonth, sizeof( gs_szMonth ), "%02u", gs_dwMonth ) ;
    WGT_SetText( g_wgt_Month, gs_szMonth ) ;
//    RTC_SetDate( g_demo_parameters.dwYear, g_demo_parameters.dwMonth, g_demo_parameters.dwDay, 0 ) ;
}

static void _ScrSettings_Date_SetYear( void )
{
    // Set Year text
    snprintf( gs_szYear, sizeof( gs_szYear ), "%02u", gs_dwYear-2000 ) ;
    WGT_SetText( g_wgt_Year, gs_szYear ) ;
//    RTC_SetDate( g_demo_parameters.dwYear, g_demo_parameters.dwMonth, g_demo_parameters.dwDay, 0 ) ;
}

static uint32_t _ScrSettings_Date_OnInitialize( SWGTScreen* pScreen )
{
    WGT_Screen_Initialize( pScreen, GUICLR_WHITE, NULL ) ;

    g_btnUpDay=WGT_CreateWidget( WGT_TYPE_BUTTON, 43,70, 48, 48 ) ;
    if ( g_btnUpDay )
    {
        WGT_SetBitmap( g_btnUpDay, DEMO_BMP_BTN_UP ) ;
        WGT_Screen_AddWidget( pScreen, g_btnUpDay ) ;
    }

    g_wgt_Day=WGT_CreateWidget( WGT_TYPE_TEXT, 43+12, 135, 22, 14 ) ;
    if ( g_wgt_Day )
    {
        WGT_SetText( g_wgt_Day, "DD" ) ;
        WGT_SetBkgndColor( g_wgt_Day, GUICLR_WHITE ) ;
        WGT_SetTextColor( g_wgt_Day, GUICLR_ATMEL_BLUE ) ;
        WGT_Screen_AddWidget( pScreen, g_wgt_Day ) ;
    }

    g_btnDownDay=WGT_CreateWidget( WGT_TYPE_BUTTON, 43, 164, 48, 48 ) ;
    if ( g_btnDownDay )
    {
        WGT_SetBitmap( g_btnDownDay, DEMO_BMP_BTN_DOWN ) ;
        WGT_Screen_AddWidget( pScreen, g_btnDownDay ) ;
    }



    g_btnUpMonth=WGT_CreateWidget( WGT_TYPE_BUTTON, 104,70, 48, 48 ) ;
    if ( g_btnUpMonth )
    {
        WGT_SetBitmap( g_btnUpMonth, DEMO_BMP_BTN_UP ) ;
        WGT_Screen_AddWidget( pScreen, g_btnUpMonth ) ;
    }

    g_wgt_Month=WGT_CreateWidget( WGT_TYPE_TEXT, 104+12, 135, 22, 14 ) ;
    if ( g_wgt_Month )
    {
        WGT_SetText( g_wgt_Month, "MM" ) ;
        WGT_SetBkgndColor( g_wgt_Month, GUICLR_WHITE ) ;
        WGT_SetTextColor( g_wgt_Month, GUICLR_ATMEL_BLUE ) ;
        WGT_Screen_AddWidget( pScreen, g_wgt_Month ) ;
    }

    g_btnDownMonth=WGT_CreateWidget( WGT_TYPE_BUTTON, 104, 164, 48, 48 ) ;
    if ( g_btnDownMonth )
    {
        WGT_SetBitmap( g_btnDownMonth, DEMO_BMP_BTN_DOWN ) ;
        WGT_Screen_AddWidget( pScreen, g_btnDownMonth ) ;
    }



    g_btnUpYear=WGT_CreateWidget( WGT_TYPE_BUTTON, 162, 70, 48, 48 ) ;
    if ( g_btnUpYear )
    {
        WGT_SetBitmap( g_btnUpYear, DEMO_BMP_BTN_UP ) ;
        WGT_Screen_AddWidget( pScreen, g_btnUpYear ) ;
    }

    g_wgt_Year=WGT_CreateWidget( WGT_TYPE_TEXT, 162+12, 135, 22, 14 ) ;
    if ( g_wgt_Year )
    {
        WGT_SetText( g_wgt_Year, "YY" ) ;
        WGT_SetBkgndColor( g_wgt_Year, GUICLR_WHITE ) ;
        WGT_SetTextColor( g_wgt_Year, GUICLR_ATMEL_BLUE ) ;
        WGT_Screen_AddWidget( pScreen, g_wgt_Year ) ;
    }

    g_btnDownYear=WGT_CreateWidget( WGT_TYPE_BUTTON, 162, 164, 48, 48 ) ;
    if ( g_btnDownYear )
    {
        WGT_SetBitmap( g_btnDownYear, DEMO_BMP_BTN_DOWN ) ;
        WGT_Screen_AddWidget( pScreen, g_btnDownYear ) ;
    }



    g_btnBack=WGT_CreateWidget( WGT_TYPE_BUTTON, 12, 252, 48, 48 ) ;
    if ( g_btnBack )
    {
        WGT_SetBitmap( g_btnBack, DEMO_BMP_BTN_BACK ) ;
        WGT_Screen_AddWidget( pScreen, g_btnBack ) ;
    }

    gs_dwDay=g_demo_parameters.dwDay ;
    gs_dwMonth=g_demo_parameters.dwMonth ;
    gs_dwYear=g_demo_parameters.dwYear ;

    // Initialize text widgets with parameters data
    _ScrSettings_Date_SetDay() ;
    _ScrSettings_Date_SetMonth() ;
    _ScrSettings_Date_SetYear() ;

    return SAMGUI_E_OK ;
}

static uint32_t _ScrSettings_Date_OnDrawBackground( SWGTScreen* pScreen )
{
    SGUIColor clr ;

    // Draw Atmel logo
	g_WGT_CoreData.pBE->DrawBitmap( 4, 8, 87, 40, DEMO_BMP_LOGO_ATMEL ) ;

    // Draw labels
    clr.u.dwRGBA=GUICLR_ATMEL_BLUE ;
    g_WGT_CoreData.pBE->DrawText( 105, 8, DEMO_LABEL"\nDate setup", &clr, (void*)&g_Font10x14, 0 ) ;
    g_WGT_CoreData.pBE->DrawText( 47, 215, "Day", &clr, (void*)&g_Font10x14, 0 ) ;
    g_WGT_CoreData.pBE->DrawText( 102, 215, "Month", &clr, (void*)&g_Font10x14, 0 ) ;
    g_WGT_CoreData.pBE->DrawText( 166, 215, "Year", &clr, (void*)&g_Font10x14, 0 ) ;

    return SAMGUI_E_OK ;
}

static uint32_t _ScrSettings_Date_ProcessMessage( SWGTScreen* pScreen, SWGTCoreMessage* pMsg )
{
//    printf( "ScrSettings_Date - msg id %x\r\n", pMsg->dwID ) ;

    switch ( pMsg->dwID )
    {
        case WGT_MSG_WIDGET_SELECTED :
            /*
             * Handle Back button
             */
            if ( pMsg->dwParam1 == (uint32_t)g_btnBack )
            {
                g_demo_parameters.dwDay=gs_dwDay ;
                g_demo_parameters.dwMonth=gs_dwMonth ;
                g_demo_parameters.dwYear=gs_dwYear ;
                Demo_Parameters_CommitChanges() ;

                WGT_SetCurrentScreen( &ScrSettings ) ;
            }

            /*
             * Handle UP/Day button
             */
            if ( pMsg->dwParam1 == (uint32_t)g_btnUpDay )
            {
                if ( gs_dwDay == 31 )
                {
                    gs_dwDay=1 ;
                }
                else
                {
                    gs_dwDay++ ;
                }

                _ScrSettings_Date_SetDay() ;
                WGT_Draw( g_wgt_Day, g_WGT_CoreData.pBE ) ;
            }

            /*
             * Handle DOWN/Day button
             */
            if ( pMsg->dwParam1 == (uint32_t)g_btnDownDay )
            {
                if ( gs_dwDay == 1 )
                {
                    gs_dwDay=31 ;
                }
                else
                {
                    gs_dwDay-- ;
                }

                _ScrSettings_Date_SetDay() ;
                WGT_Draw( g_wgt_Day, g_WGT_CoreData.pBE ) ;
            }

            /*
             * Handle UP/Month button
             */
            if ( pMsg->dwParam1 == (uint32_t)g_btnUpMonth )
            {
                if ( gs_dwMonth == 12 )
                {
                    gs_dwMonth=1 ;
                }
                else
                {
                    gs_dwMonth++ ;
                }

                _ScrSettings_Date_SetMonth() ;
                WGT_Draw( g_wgt_Month, g_WGT_CoreData.pBE ) ;
            }

            /*
             * Handle DOWN/Month button
             */
            if ( pMsg->dwParam1 == (uint32_t)g_btnDownMonth )
            {
                if ( gs_dwMonth == 1 )
                {
                    gs_dwMonth=12 ;
                }
                else
                {
                    gs_dwMonth-- ;
                }

                _ScrSettings_Date_SetMonth() ;
                WGT_Draw( g_wgt_Month, g_WGT_CoreData.pBE ) ;
            }

            /*
             * Handle UP/Year button
             */
            if ( pMsg->dwParam1 == (uint32_t)g_btnUpYear )
            {
                if ( gs_dwYear == 2020 )
                {
                    gs_dwYear=2000 ;
                }
                else
                {
                    gs_dwYear++ ;
                }

                _ScrSettings_Date_SetYear() ;
                WGT_Draw( g_wgt_Year, g_WGT_CoreData.pBE ) ;
            }

            /*
             * Handle DOWN/Year button
             */
            if ( pMsg->dwParam1 == (uint32_t)g_btnDownYear )
            {
                if ( gs_dwYear == 2000 )
                {
                    gs_dwYear=2020 ;
                }
                else
                {
                    gs_dwYear-- ;
                }

                _ScrSettings_Date_SetYear() ;
                WGT_Draw( g_wgt_Year, g_WGT_CoreData.pBE ) ;
            }
        break ;

        case WGT_MSG_TIMER :
        break ;
    }

    return SAMGUI_E_OK ;
}

static uint32_t _ScrSettings_Date_HkExit( SWGTScreen* pScreen )
{
    RTC_SetDate( RTC, g_demo_parameters.dwYear, g_demo_parameters.dwMonth, g_demo_parameters.dwDay, 1 ) ;

    return SAMGUI_E_OK ;
}

extern SWGTScreen ScrSettings_Date=
{
    .ProcessMessage=_ScrSettings_Date_ProcessMessage,

    /* Hooks */
    .HkBeforePaint=NULL,
    .HkAfterPaint=NULL,
    .HkBeforeEraseBackground=NULL,
    .HkAfterEraseBackground=NULL,
    .HkExit=_ScrSettings_Date_HkExit,

    /* Callbacks */
    .OnInitialize=_ScrSettings_Date_OnInitialize,
    .OnEraseBackground=_ScrSettings_Date_OnDrawBackground,
    .OnPaint=NULL
} ;

/** @} */
/** @} */
/** @} */
