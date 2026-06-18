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
#include "rand.h"
#include "board.h"
#include <stdio.h>

/**
 * \addtogroup screens
 * @{
 * \addtogroup screen_fs_explorer File system explorer screen
 * @{
 *
 * \brief This screen aims at browsing files.
 */

#define DEMO_OFFSET_X                  20
#define DEMO_OFFSET_Y                  60
#define DEMO_WIDTH                     (BOARD_LCD_WIDTH-(DEMO_OFFSET_X<<1))
#define DEMO_HEIGHT                    (BOARD_LCD_HEIGHT-DEMO_OFFSET_Y-10)


#define min( a, b ) ((a<b)?a:b)
#define max( a, b ) ((a<b)?b:a)

SWGT_Widget g_btnBack ;

static uint32_t _ScrExplorer_OnInitialize( SWGTScreen* pScreen )
{

    WGT_Screen_Initialize( pScreen, GUICLR_WHITE, NULL ) ;

    WGT_CreateWidget( WGT_TYPE_BUTTON, 12, 252, 48, 48 ) ;
    WGT_SetBitmap( &g_btnBack, DEMO_BMP_BTN_BACK ) ;
    WGT_Screen_AddWidget( pScreen, &g_btnBack ) ;

    return SAMGUI_E_OK ;
}

static uint32_t _ScrExplorer_OnDrawBackground( SWGTScreen* pScreen )
{
    SGUIColor clr ;

    // Draw Atmel logo
    g_WGT_CoreData.pBE->DrawBitmap( 4, 8, 87, 40, DEMO_BMP_LOGO_ATMEL ) ;

    // Draw labels
    clr.u.dwRGBA=GUICLR_ATMEL_BLUE ;
    g_WGT_CoreData.pBE->DrawText( 105, 8, DEMO_LABEL"\nExplorer", &clr, (void*)&g_Font10x14, 0 ) ;

    clr.u.dwRGBA=GUICLR_BLACK ;
    g_WGT_CoreData.pBE->DrawFilledRectangle( DEMO_OFFSET_X, DEMO_OFFSET_Y, DEMO_OFFSET_X+DEMO_WIDTH, DEMO_OFFSET_Y+DEMO_HEIGHT, NULL, &clr ) ;

    return SAMGUI_E_OK ;
}

static uint32_t _ScrExplorer_ProcessMessage( SWGTScreen* pScreen, SWGTCoreMessage* pMsg )
{
//    printf( "ScrDISPDemo - msg id %x\r\n", pMsg->dwID ) ;

    switch ( pMsg->dwID )
    {
        case WGT_MSG_POINTER_RELEASED : // cursor released
        case WGT_MSG_KEY_RELEASED : // button released
        break ;

        case WGT_MSG_WIDGET_SELECTED :
          if ( pMsg->dwParam1 == (uint32_t)&g_btnBack )
          {
              WGT_SetCurrentScreen( &ScrMain ) ;
          }
        break ;
    }

    return SAMGUI_E_OK ;
}

extern SWGTScreen ScrExplorer=
{
    .ProcessMessage=_ScrExplorer_ProcessMessage,

    /* Hooks */
    .HkBeforePaint=NULL,
    .HkAfterPaint=NULL,
    .HkBeforeEraseBackground=NULL,
    .HkAfterEraseBackground=NULL,
    .HkExit=NULL,

    /* Callbacks */
    .OnInitialize=_ScrExplorer_OnInitialize,
    .OnEraseBackground=_ScrExplorer_OnDrawBackground,
    .OnPaint=NULL
} ;

/** @} */
/** @} */
