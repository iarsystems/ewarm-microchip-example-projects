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
 * \addtogroup screen_intro Introduction screen
 * @{
 *
 * \brief This screen show AT91SAM3S-EK picture and switch to main screen either after a delay
 * or interaction using TouchScreen, pushbuttons, potentiometer and QTouch.
 */

static uint32_t _ScrIntro_OnInitialize( SWGTScreen* pScreen )
{
    WGT_Screen_Initialize( pScreen, GUICLR_WHITE, NULL ) ;

    if ( g_demo_parameters.dwBacklight > 2 )
    {
        g_WGT_CoreData.pBE->IOCtl( DISP_BACKEND_IOCTL_SET_BACKLIGHT, (uint32_t*)g_demo_parameters.dwBacklight, NULL ) ;
    }
    else
    {
        g_WGT_CoreData.pBE->IOCtl( DISP_BACKEND_IOCTL_SET_BACKLIGHT, (uint32_t*)14, NULL ) ;
    }

    return SAMGUI_E_OK ;
}

static uint32_t _ScrIntro_OnEraseBackground( SWGTScreen* pScreen )
{
//	g_WGT_CoreData.pBE->DrawBitmap( 3, 40, 235, 236, DEMO_BMP_INTRO_SAM3S ) ;

    return SAMGUI_E_OK ;
}

static uint32_t _ScrIntro_ProcessMessage( SWGTScreen* pScreen, SWGTCoreMessage* pMsg )
{
    static uint32_t dwTime=0 ;

//    printf( "ScrIntro - msg id %x\r\n", pMsg->dwID ) ;

    switch ( pMsg->dwID )
    {
        case WGT_MSG_TIMER : // Timer triggered
            dwTime++ ;
            if ( dwTime == 5 )
            {
              // Send message to switch to main screen
              WGT_SetCurrentScreen( &ScrMain ) ;
            }
        break ;

        case WGT_MSG_POINTER_RELEASED : // cursor released
        case WGT_MSG_KEY_RELEASED : // button released
            // Send message to switch to main screen
            WGT_SetCurrentScreen( &ScrMain ) ;
        break ;

        case WGT_MSG_KEY_PRESSED : // button released
            switch ( pMsg->dwParam1 )
            {
                case WGT_KEY_VR1 :
                case WGT_KEY_S1 :
                    // Send message to switch to main screen
                    WGT_SetCurrentScreen( &ScrMain ) ;
                break ;
            }
        break ;
    }

    return SAMGUI_E_OK ;
}

extern SWGTScreen ScrIntro=
{
    .ProcessMessage=_ScrIntro_ProcessMessage,

    /* Hooks */
    .HkBeforePaint=NULL,
    .HkAfterPaint=NULL,
    .HkBeforeEraseBackground=NULL,
    .HkAfterEraseBackground=NULL,
    .HkExit=NULL,

    /* Callbacks */
    .OnInitialize=_ScrIntro_OnInitialize,
    .OnEraseBackground=_ScrIntro_OnEraseBackground,
    .OnPaint=NULL
} ;

/** @} */
/** @} */
