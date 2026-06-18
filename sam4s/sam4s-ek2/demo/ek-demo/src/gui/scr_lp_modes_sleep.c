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
 * \addtogroup screen_sleep Sleep mode screen
 * @{
 */

/** Pins used to wake-up */
static const uint32_t g_sleepWakeUpPin = PIN_TSC_IRQ_WUP_ID;
extern Pin _WFE_ADS7843_pinPenIRQ=PIN_TSC_IRQ ;
static __no_init uint32_t g_irq[2];

static uint32_t ScrLPModes_Sleep_OnInitialize( SWGTScreen* pScreen )
{
    WGT_Screen_Initialize( pScreen, GUICLR_WHITE, NULL ) ;

    return SAMGUI_E_OK ;
}

static uint32_t ScrLPModes_Sleep_OnDrawBackground( SWGTScreen* pScreen )
{
    SGUIColor clr ;
    clr.u.dwRGBA=GUICLR_ATMEL_BLUE ;

    // Draw Atmel logo and label
    g_WGT_CoreData.pBE->DrawBitmap( 4, 8, 87, 40, DEMO_BMP_LOGO_ATMEL ) ;
    g_WGT_CoreData.pBE->DrawText( 105, 8, DEMO_LABEL"\nLow-power\n  modes", &clr, (void*)&g_Font10x14, 0 ) ;

    // Draw Backup bitmap and Title
    g_WGT_CoreData.pBE->DrawBitmap( 82, 64, 75, 80, DEMO_BMP_SLEEP_MODE ) ;
    g_WGT_CoreData.pBE->DrawText( 60, 150, "Sleep mode", &clr, (void*)&g_Font10x14, 0 ) ;
    //g_WGT_CoreData.pBE->DrawText( 12, 176, "Power consumption:\n  app dependent", &clr, (void*)&g_Font10x14, 0 ) ;

    // Draw labels
    g_WGT_CoreData.pBE->DrawText( 42, 240, "Init mode ...", &clr, (void*)&g_Font10x14, 0 ) ;

    return SAMGUI_E_OK ;
}

static uint32_t ScrLPModes_Sleep_ProcessMessage( SWGTScreen* pScreen, SWGTCoreMessage* pMsg )
{
    SGUIColor clr ;

//    printf( "ScrLPModes_Sleep - msg id %x\r\n", pMsg->dwID ) ;

    switch ( pMsg->dwID )
    {
        case WGT_MSG_TIMER :
            clr.u.dwRGBA = GUICLR_WHITE;
//            g_WGT_CoreData.pBE->DrawFilledRectangle( 42, 240, BOARD_LCD_WIDTH, 120, NULL, &clr ) ;
            g_WGT_CoreData.pBE->DrawText( 42, 240, "Init mode ...", &clr, (void*)&g_Font10x14, 0 ) ;

            clr.u.dwRGBA = GUICLR_ATMEL_BLUE;
            g_WGT_CoreData.pBE->DrawText( 25, 240, "Touch the screen\n   to wake-up", &clr, (void*)&g_Font10x14, 0 ) ;

            // Disable all ITs
            taskENTER_CRITICAL();
            g_irq[0] = NVIC->ISER[0];
            g_irq[1] = NVIC->ISER[1];
            NVIC->ICER[0] = 0xFFFFFFFF;
            NVIC->ICER[1] = 0xFFFFFFFF;

            // Configure PIOA with IT priority > basepri  (lower has higher priority on Cortex M3)
            NVIC_SetPriority(PIOA_IRQn, configMAX_SYSCALL_INTERRUPT_PRIORITY - 16);
            /* Enable the interrupt for PIOA */
            NVIC_EnableIRQ(PIOA_IRQn);

            // Enable the PIO for wake-up
            SUPC->SUPC_WUIR = (g_sleepWakeUpPin << 16) | g_sleepWakeUpPin;
            // WFI or WFE mode can wake-up
            //PMC->PMC_FSMR &= (unsigned int)~PMC_FSMR_LPM;
            // Clear Deep Sleep
            SCB->SCR &= (unsigned int)~(1 << 2);

            // Enter Sleep Mode
            __WFI();

            // Re configure PIOA with IT priority > basepri  (lower has higher priority on Cortex M3)
            NVIC_SetPriority(PIOA_IRQn, configMAX_SYSCALL_INTERRUPT_PRIORITY + 16);
            // Re-enable all ITs
            NVIC->ISER[0] = g_irq[0];
            NVIC->ISER[1] = g_irq[1];
            taskEXIT_CRITICAL();

            // Go back to previous screen
            WGT_SetCurrentScreen( &ScrLPModes );
        break;
    }

    return SAMGUI_E_OK ;
}

extern SWGTScreen ScrLPModes_Sleep=
{
    .ProcessMessage=ScrLPModes_Sleep_ProcessMessage,

    /* Hooks */
    .HkBeforePaint=NULL,
    .HkAfterPaint=NULL,
    .HkBeforeEraseBackground=NULL,
    .HkAfterEraseBackground=NULL,
    .HkExit=NULL,

    /* Callbacks */
    .OnInitialize=ScrLPModes_Sleep_OnInitialize,
    .OnEraseBackground=ScrLPModes_Sleep_OnDrawBackground,
    .OnPaint=NULL
} ;

/** @} */
/** @} */
