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
 * \addtogroup screen_infos Informations screen
 * @{
 *
 * \brief This screen shows informations about this demo application.
 */

/** Demo version string */
static __no_init char szVersionText[20] ;


static uint32_t ScrInfos_OnInitialize( SWGTScreen* pScreen )
{
    WGT_Screen_Initialize( pScreen, GUICLR_WHITE, NULL ) ;

    return SAMGUI_E_OK ;
}

static uint32_t ScrInfos_OnDrawBackground( SWGTScreen* pScreen )
{
    SGUIColor clr ;

    // Draw Atmel logo
    g_WGT_CoreData.pBE->DrawBitmap( 4, 8, 87, 40, DEMO_BMP_LOGO_ATMEL ) ;

    // Draw labels
    clr.u.dwRGBA=GUICLR_ATMEL_BLUE ;
    g_WGT_CoreData.pBE->DrawText( 105, 8, DEMO_LABEL"\nInformation", &clr, (void*)&g_Font10x14, 0 ) ;

    // Draw info bitmap
//    g_WGT_CoreData.pBE->DrawBitmap( 95, 82, 49, 49, DEMO_BMP_INFO ) ;

    // Draw text
    g_WGT_CoreData.pBE->DrawText( 18, 159, "  SAM4S-EK2 Demo", &clr, (void*)&g_Font10x14, 0 ) ;
    snprintf( szVersionText, sizeof( szVersionText ), "    Version %s", DEMO_VERSION);
    g_WGT_CoreData.pBE->DrawText( 0, 191, szVersionText, &clr, (void*)&g_Font10x14, 0 ) ;

    return SAMGUI_E_OK ;
}

static uint32_t ScrInfos_ProcessMessage( SWGTScreen* pScreen, SWGTCoreMessage* pMsg )
{
//    printf( "ScrInfos - msg id %x\r\n", pMsg->dwID ) ;

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

extern SWGTScreen ScrInfos=
{
    .ProcessMessage=ScrInfos_ProcessMessage,

    /* Hooks */
    .HkBeforePaint=NULL,
    .HkAfterPaint=NULL,
    .HkBeforeEraseBackground=NULL,
    .HkAfterEraseBackground=NULL,
    .HkExit=NULL,

    /* Callbacks */
    .OnInitialize=ScrInfos_OnInitialize,
    .OnEraseBackground=ScrInfos_OnDrawBackground,
    .OnPaint=NULL
} ;

/** @} */
/** @} */
