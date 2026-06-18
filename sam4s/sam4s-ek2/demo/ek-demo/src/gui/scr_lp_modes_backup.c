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

#include "board.h"

#include <stdio.h>

/**
 * \addtogroup lowpower_modes
 * @{
 * \addtogroup screen_backup Backup mode screen
 * @{
 */

/** Pins used to wake-up */
static const uint32_t g_backupWakeUpPin = PIN_TSC_IRQ_WUP_ID;

static uint32_t ScrLPModes_Backup_OnInitialize( SWGTScreen* pScreen )
{
    WGT_Screen_Initialize( pScreen, GUICLR_WHITE, NULL ) ;

    return SAMGUI_E_OK ;
}

static uint32_t ScrLPModes_Backup_OnDrawBackground( SWGTScreen* pScreen )
{
    SGUIColor clr ;
    clr.u.dwRGBA=GUICLR_ATMEL_BLUE ;

    // Draw Atmel logo and label
    g_WGT_CoreData.pBE->DrawBitmap( 4, 8, 87, 40, DEMO_BMP_LOGO_ATMEL ) ;
    g_WGT_CoreData.pBE->DrawText( 105, 8, DEMO_LABEL"\nLow-power\n  modes", &clr, (void*)&g_Font10x14, 0 ) ;

    // Draw Backup bitmap and Title
    g_WGT_CoreData.pBE->DrawBitmap( 93, 66, 53, 49, DEMO_BMP_BACKUP_MODE ) ;
    g_WGT_CoreData.pBE->DrawText( 54, 144, "Backup mode", &clr, (void*)&g_Font10x14, 0 ) ;
    //g_WGT_CoreData.pBE->DrawText( 12, 176, "Power consumption:\n  typically 3 uA", &clr, (void*)&g_Font10x14, 0 ) ;

    // Draw labels
    g_WGT_CoreData.pBE->DrawText( 42, 240, "Init mode ...", &clr, (void*)&g_Font10x14, 0 ) ;

    return SAMGUI_E_OK ;
}

static uint32_t ScrLPModes_Backup_ProcessMessage( SWGTScreen* pScreen, SWGTCoreMessage* pMsg )
{
    SGUIColor clr ;

//    printf( "ScrLPModes_Backup - msg id %x\r\n", pMsg->dwID ) ;

    switch ( pMsg->dwID )
    {
        case WGT_MSG_TIMER :
            clr.u.dwRGBA = GUICLR_WHITE;
//            g_WGT_CoreData.pBE->DrawFilledRectangle( 42, 240, BOARD_LCD_WIDTH, 120, NULL, &clr ) ;
            g_WGT_CoreData.pBE->DrawText( 42, 240, "Init mode ...", &clr, (void*)&g_Font10x14, 0 ) ;

            clr.u.dwRGBA = GUICLR_ATMEL_BLUE;
            g_WGT_CoreData.pBE->DrawText( 25, 240, "Touch the screen\n   to wake-up\n  and reboot", &clr, (void*)&g_Font10x14, 0 ) ;

            /* Enable the PIO for wake-up */
            SUPC->SUPC_WUIR = g_backupWakeUpPin;

            /* Enter Backup Mode */
            SCB->SCR |= 1 << 2;
            __WFE();

		SUPC->SUPC_CR = (0xa5<<24) | (1<<2);
        break;
    }

    return SAMGUI_E_OK ;
}

extern SWGTScreen ScrLPModes_Backup=
{
    .ProcessMessage=ScrLPModes_Backup_ProcessMessage,

    /* Hooks */
    .HkBeforePaint=NULL,
    .HkAfterPaint=NULL,
    .HkBeforeEraseBackground=NULL,
    .HkAfterEraseBackground=NULL,
    .HkExit=NULL,

    /* Callbacks */
    .OnInitialize=ScrLPModes_Backup_OnInitialize,
    .OnEraseBackground=ScrLPModes_Backup_OnDrawBackground,
    .OnPaint=NULL
} ;

/** @} */
/** @} */
