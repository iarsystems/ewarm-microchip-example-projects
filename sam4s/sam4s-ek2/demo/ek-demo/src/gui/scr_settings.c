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
 * \addtogroup settings Settings
 * @{
 * \addtogroup screen_settings Settings main screen
 * @{
 */

static __no_init SWGT_Widget* g_btnBack ;
static __no_init SWGT_Widget* g_btnDate ;
static __no_init SWGT_Widget* g_btnTime ;
static __no_init SWGT_Widget* g_btnBacklight ;
static __no_init SWGT_Widget* g_btnTSD ;

static uint32_t ScrSettings_OnInitialize( SWGTScreen* pScreen )
{
    WGT_Screen_Initialize( pScreen, GUICLR_WHITE, NULL ) ;

    g_btnTime=WGT_CreateWidget( WGT_TYPE_BUTTON, 16, 81, 62, 66 ) ;
    if ( g_btnTime )
    {
        WGT_SetBitmap( g_btnTime, DEMO_BMP_BTN_TIME_SETUP ) ;
        WGT_Screen_AddWidget( pScreen, g_btnTime ) ;
    }

    g_btnDate=WGT_CreateWidget( WGT_TYPE_BUTTON, 98, 90, 56, 56 ) ;
    if ( g_btnDate )
    {
        WGT_SetBitmap( g_btnDate, DEMO_BMP_BTN_DATE_SETUP ) ;
        WGT_Screen_AddWidget( pScreen, g_btnDate ) ;
    }

    g_btnBacklight=WGT_CreateWidget( WGT_TYPE_BUTTON, 165, 86, 66, 64 ) ;
    if ( g_btnBacklight )
    {
        WGT_SetBitmap( g_btnBacklight, DEMO_BMP_BTN_BACKLIGHT_SETUP ) ;
        WGT_Screen_AddWidget( pScreen, g_btnBacklight ) ;
    }

    g_btnTSD=WGT_CreateWidget( WGT_TYPE_BUTTON, /*22*/100, 180, 48, 48 ) ;
    if ( g_btnTSD )
    {
        WGT_SetBitmap( g_btnTSD, DEMO_BMP_BTN_TSD_SETUP ) ;
        WGT_Screen_AddWidget( pScreen, g_btnTSD ) ;
    }

    g_btnBack=WGT_CreateWidget( WGT_TYPE_BUTTON, 12, 252, 48, 48 ) ;
    if ( g_btnBack )
    {
        WGT_SetBitmap( g_btnBack, DEMO_BMP_BTN_BACK ) ;
        WGT_Screen_AddWidget( pScreen, g_btnBack ) ;
    }

    return SAMGUI_E_OK ;
}

static uint32_t ScrSettings_OnDrawBackground( SWGTScreen* pScreen )
{
    SGUIColor clr ;

    // Draw Atmel logo
//	g_WGT_CoreData.pBE->DrawBitmap( 4, 8, 87, 40, (void*)aucBmp_logo_atmel ) ;
	g_WGT_CoreData.pBE->DrawBitmap( 4, 8, 87, 40, DEMO_BMP_LOGO_ATMEL ) ;

    // Draw labels
    clr.u.dwRGBA=GUICLR_ATMEL_BLUE ;
    g_WGT_CoreData.pBE->DrawText( 105, 8, DEMO_LABEL"\nSettings", &clr, (void*)&g_Font10x14, 0 ) ;

    return SAMGUI_E_OK ;
}

static uint32_t ScrSettings_ProcessMessage( SWGTScreen* pScreen, SWGTCoreMessage* pMsg )
{
//    printf( "ScrSettings_ProcessMessage - msg id %x\r\n", pMsg->dwID ) ;

    switch ( pMsg->dwID )
    {
        case WGT_MSG_WIDGET_SELECTED :
          if ( pMsg->dwParam1 == (uint32_t)g_btnBack )
          {
              WGT_SetCurrentScreen( &ScrMain ) ;
          }

          if ( pMsg->dwParam1 == (uint32_t)g_btnDate )
          {
              WGT_SetCurrentScreen( &ScrSettings_Date ) ;
          }

          if ( pMsg->dwParam1 == (uint32_t)g_btnTime )
          {
              WGT_SetCurrentScreen( &ScrSettings_Time ) ;
          }

          if ( pMsg->dwParam1 == (uint32_t)g_btnBacklight )
          {
              WGT_SetCurrentScreen( &ScrSettings_Backlight ) ;
          }

          if ( pMsg->dwParam1 == (uint32_t)g_btnTSD )
          {
              WGT_SetCurrentScreen( &ScrTSDCalibration ) ;
          }
        break ;

        case WGT_MSG_TIMER :
        break ;
    }

    return SAMGUI_E_OK ;
}

extern SWGTScreen ScrSettings=
{
    .ProcessMessage=ScrSettings_ProcessMessage,

    /* Hooks */
    .HkBeforePaint=NULL,
    .HkAfterPaint=NULL,
    .HkBeforeEraseBackground=NULL,
    .HkAfterEraseBackground=NULL,
    .HkExit=NULL,

    /* Callbacks */
    .OnInitialize=ScrSettings_OnInitialize,
    .OnEraseBackground=ScrSettings_OnDrawBackground,
    .OnPaint=NULL
} ;

/** @} */
/** @} */
/** @} */
