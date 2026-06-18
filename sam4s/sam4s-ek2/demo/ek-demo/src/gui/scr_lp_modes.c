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
 * \addtogroup SAM3S_demo AT91SAM3S demo
 * @{
 * \addtogroup screens Screens
 * @{
 * \addtogroup lowpower_modes Low-power modes
 * @{
 * \addtogroup screen_lp_modes Low-Power modes main screen
 * @{
 *
* \brief This screens allows access to the three low-power modes : Backup, Sleep and Wait.
 */

static __no_init SWGT_Widget* g_btnBackup ;
static __no_init SWGT_Widget* g_btnWait ;
static __no_init SWGT_Widget* g_btnSleep ;
static __no_init SWGT_Widget* g_btnBack ;

static uint32_t ScrLPModes_OnInitialize( SWGTScreen* pScreen )
{
    WGT_Screen_Initialize( pScreen, GUICLR_WHITE, NULL ) ;

    g_btnBackup=WGT_CreateWidget( WGT_TYPE_BUTTON, 73+15, 78, 45, 40 ) ;
    if ( g_btnBackup )
    {
        WGT_SetBitmap( g_btnBackup, DEMO_BMP_BTN_BACKUP_MODE ) ;
        WGT_Screen_AddWidget( pScreen, g_btnBackup ) ;
    }

    g_btnWait=WGT_CreateWidget( WGT_TYPE_BUTTON, 73+15, 157, 44, 45 ) ;
    if ( g_btnWait )
    {
        WGT_SetBitmap( g_btnWait, DEMO_BMP_BTN_WAIT_MODE ) ;
        WGT_Screen_AddWidget( pScreen, g_btnWait ) ;
    }

    g_btnSleep=WGT_CreateWidget( WGT_TYPE_BUTTON, 66+15, 238, 55, 59 ) ;
    if ( g_btnSleep )
    {
        WGT_SetBitmap( g_btnSleep, DEMO_BMP_BTN_SLEEP_MODE ) ;
        WGT_Screen_AddWidget( pScreen, g_btnSleep ) ;
    }

    g_btnBack=WGT_CreateWidget( WGT_TYPE_BUTTON, 12, 252, 48, 48 ) ;
    if ( g_btnBack )
    {
        WGT_SetBitmap( g_btnBack, DEMO_BMP_BTN_BACK ) ;
        WGT_Screen_AddWidget( pScreen, g_btnBack ) ;
    }

    return SAMGUI_E_OK ;
}

static uint32_t ScrLPModes_OnDrawBackground( SWGTScreen* pScreen )
{
    SGUIColor clr ;

    // Draw Atmel logo
//	g_WGT_CoreData.pBE->DrawBitmap( 4, 8, 87, 40, (void*)aucBmp_logo_atmel ) ;
	g_WGT_CoreData.pBE->DrawBitmap( 4, 8, 87, 40, DEMO_BMP_LOGO_ATMEL ) ;

    // Draw labels
    clr.u.dwRGBA=GUICLR_ATMEL_BLUE ;
    g_WGT_CoreData.pBE->DrawText( 105, 8, DEMO_LABEL"\nLow-power\n  modes", &clr, (void*)&g_Font10x14, 0 ) ;

    g_WGT_CoreData.pBE->DrawText( 140+10,  96, "Backup", &clr, (void*)&g_Font10x14, 0 ) ;
    g_WGT_CoreData.pBE->DrawText( 140+10, 174, "Wait", &clr, (void*)&g_Font10x14, 0 ) ;
    g_WGT_CoreData.pBE->DrawText( 140+10, 256, "Sleep", &clr, (void*)&g_Font10x14, 0 ) ;

    return SAMGUI_E_OK ;
}

static uint32_t ScrLPModes_ProcessMessage( SWGTScreen* pScreen, SWGTCoreMessage* pMsg )
{
//    printf( "ScrLPModes - msg id %x\r\n", pMsg->dwID ) ;

    switch ( pMsg->dwID )
    {
        case WGT_MSG_KEY_PRESSED :
            switch ( pMsg->dwParam1 )
            {
                case WGT_KEY_PB1 :
                    // in this screen, defined as ENTER...

                break ;
            }
        break ;

        case WGT_MSG_WIDGET_SELECTED :
          if ( pMsg->dwParam1 == (uint32_t)g_btnBack )
          {
              WGT_SetCurrentScreen( &ScrMain ) ;
          }

          if ( pMsg->dwParam1 == (uint32_t)g_btnBackup )
          {
              WGT_SetCurrentScreen( &ScrLPModes_Backup ) ;
          }

          if ( pMsg->dwParam1 == (uint32_t)g_btnWait )
          {
              WGT_SetCurrentScreen( &ScrLPModes_Wait ) ;
          }

          if ( pMsg->dwParam1 == (uint32_t)g_btnSleep )
          {
              WGT_SetCurrentScreen( &ScrLPModes_Sleep ) ;
          }
        break ;
    }

    return SAMGUI_E_OK ;
}

extern SWGTScreen ScrLPModes=
{
    .ProcessMessage=ScrLPModes_ProcessMessage,

    /* Hooks */
    .HkBeforePaint=NULL,
    .HkAfterPaint=NULL,
    .HkBeforeEraseBackground=NULL,
    .HkAfterEraseBackground=NULL,
    .HkExit=NULL,

    /* Callbacks */
    .OnInitialize=ScrLPModes_OnInitialize,
    .OnEraseBackground=ScrLPModes_OnDrawBackground,
    .OnPaint=NULL
} ;

/** @}
 * @}
 * @}
 * @} */
