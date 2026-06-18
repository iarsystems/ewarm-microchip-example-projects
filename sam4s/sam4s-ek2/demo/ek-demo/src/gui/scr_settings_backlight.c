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

#include <stdio.h>

/**
 * \addtogroup screens
 * @{
 * \addtogroup settings Settings
 * @{
 * \addtogroup screen_settings_backlight Backlight settings screen
 * @{
 *
 * \brief This screen allows configuration of board LCD backlight.
 */

static __no_init SWGT_Widget* g_btnBack ;
static __no_init SWGT_Widget* g_btnUp ;
static __no_init SWGT_Widget* g_btnDown ;

static uint32_t ScrSettings_Backlight_OnInitialize( SWGTScreen* pScreen )
{
    WGT_Screen_Initialize( pScreen, GUICLR_WHITE, NULL ) ;

    g_btnUp=WGT_CreateWidget( WGT_TYPE_BUTTON, 98, 80, 48, 48 ) ;
    if ( g_btnUp )
    {
        WGT_SetBitmap( g_btnUp, DEMO_BMP_BTN_UP ) ;
        WGT_Screen_AddWidget( pScreen, g_btnUp ) ;
    }

    g_btnDown=WGT_CreateWidget( WGT_TYPE_BUTTON, 98, 217, 48, 48 ) ;
    if ( g_btnDown )
    {
        WGT_SetBitmap( g_btnDown, DEMO_BMP_BTN_DOWN ) ;
        WGT_Screen_AddWidget( pScreen, g_btnDown ) ;
    }

    g_btnBack=WGT_CreateWidget( WGT_TYPE_BUTTON, 12, 252, 48, 48 ) ;
    if ( g_btnBack )
    {
        WGT_SetBitmap( g_btnBack, DEMO_BMP_BTN_BACK ) ;
        WGT_Screen_AddWidget( pScreen, g_btnBack ) ;
    }

    return SAMGUI_E_OK ;
}

static uint32_t ScrSettings_Backlight_OnDrawBackground( SWGTScreen* pScreen )
{
    SGUIColor clr ;

    // Draw Atmel logo
//	g_WGT_CoreData.pBE->DrawBitmap( 4, 8, 87, 40, (void*)aucBmp_logo_atmel ) ;
	g_WGT_CoreData.pBE->DrawBitmap( 4, 8, 87, 40, DEMO_BMP_LOGO_ATMEL ) ;

	g_WGT_CoreData.pBE->DrawBitmap( 86, 129, 66, 64, DEMO_BMP_CONFIG_BACKLIGHT ) ;

    // Draw labels
    clr.u.dwRGBA=GUICLR_ATMEL_BLUE ;
    g_WGT_CoreData.pBE->DrawText( 105, 8, DEMO_LABEL"\nLCD\nbacklight", &clr, (void*)&g_Font10x14, 0 ) ;

    return SAMGUI_E_OK ;
}

static uint32_t ScrSettings_Backlight_ProcessMessage( SWGTScreen* pScreen, SWGTCoreMessage* pMsg )
{
//    printf( "ScrSettings_Backlight - msg id %x\r\n", pMsg->dwID ) ;

    switch ( pMsg->dwID )
    {
        case WGT_MSG_WIDGET_SELECTED :
          if ( pMsg->dwParam1 == (uint32_t)g_btnBack )
          {
              Demo_Parameters_CommitChanges() ;
              WGT_SetCurrentScreen( &ScrSettings ) ;
          }

          if ( pMsg->dwParam1 == (uint32_t)g_btnUp )
          {
              if ( g_demo_parameters.dwBacklight == 16 )
              {
                  g_demo_parameters.dwBacklight=16 ;
              }
              else
              {
                  g_demo_parameters.dwBacklight++ ;
              }

              g_WGT_CoreData.pBE->IOCtl( DISP_BACKEND_IOCTL_SET_BACKLIGHT, (uint32_t*)g_demo_parameters.dwBacklight, NULL ) ;
          }

          if ( pMsg->dwParam1 == (uint32_t)g_btnDown )
          {
              if ( g_demo_parameters.dwBacklight == 3 )
              {
                  g_demo_parameters.dwBacklight=3 ;
              }
              else
              {
                  g_demo_parameters.dwBacklight-- ;
              }

              g_WGT_CoreData.pBE->IOCtl( DISP_BACKEND_IOCTL_SET_BACKLIGHT, (uint32_t*)g_demo_parameters.dwBacklight, NULL ) ;
          }
        break ;

        case WGT_MSG_TIMER :
        break ;
    }

    return SAMGUI_E_OK ;
}

extern SWGTScreen ScrSettings_Backlight=
{
    .ProcessMessage=ScrSettings_Backlight_ProcessMessage,

    /* Hooks */
    .HkBeforePaint=NULL,
    .HkAfterPaint=NULL,
    .HkBeforeEraseBackground=NULL,
    .HkAfterEraseBackground=NULL,
    .HkExit=NULL,

    /* Callbacks */
    .OnInitialize=ScrSettings_Backlight_OnInitialize,
    .OnEraseBackground=ScrSettings_Backlight_OnDrawBackground,
    .OnPaint=NULL
} ;

/** @} */
/** @} */
/** @} */
