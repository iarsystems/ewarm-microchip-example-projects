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
#include "usbd.h"

#include <stdio.h>


/**
 * \addtogroup lowpower_modes
 * @{
 * \addtogroup screen_wait_mode Wait Mode screen
 * @{
 */

/** Pins used to wake-up */
static const uint32_t g_waitWakeUpPin = PIN_TSC_IRQ_WUP_ID;

static uint32_t g_PllarValBackup = 0;
static uint32_t g_PllbrValBackup = 0;
static uint32_t g_MckrValBackup  = 0;
static uint32_t g_irq[2];

/**-----------------------------------------------------------------------------
 * Switch master clock to On-Chip Fast RC Oscillator.
 * -----------------------------------------------------------------------------
 */

static void switchMckFastRC(void)
{
    // Enable Fast RC oscillator
    PMC->CKGR_MOR = CKGR_MOR_MOSCSEL | (0x37 << 16) | CKGR_MOR_MOSCXTEN | CKGR_MOR_MOSCRCEN;
    // Wait the Fast RC to stabilize
    while (!(PMC->PMC_SR & PMC_SR_MOSCRCS));

    // Switch to main clock
    PMC->PMC_MCKR = (PMC->PMC_MCKR & (unsigned int)~PMC_MCKR_CSS_Msk) | PMC_MCKR_CSS_MAIN_CLK;
    while (!(PMC->PMC_SR & PMC_SR_MCKRDY));

    PMC->PMC_MCKR = (PMC->PMC_MCKR & (unsigned int)~PMC_MCKR_PRES_Msk);
    while (!(PMC->PMC_SR & PMC_SR_MCKRDY));

    // Switch from Main Xtal osc to Fast RC
    PMC->CKGR_MOR = (0x37 << 16) | CKGR_MOR_MOSCRCEN | CKGR_MOR_MOSCXTEN;
    // Wait for Main Oscillator Selection Status bit MOSCSELS
    while (!(PMC->PMC_SR & PMC_SR_MOSCSELS));

    // Disable Main XTAL Oscillator
    PMC->CKGR_MOR = (0x37 << 16) | CKGR_MOR_MOSCRCEN;

    // Change frequency on 4MHz RC oscillator
    PMC->CKGR_MOR = (0x37 << 16) | PMC->CKGR_MOR;
    // Wait the Fast RC to stabilize
    while (!(PMC->PMC_SR & PMC_SR_MOSCRCS));

    // Stop PLLA
    PMC->CKGR_PLLAR = CKGR_PLLAR_ONE | CKGR_PLLAR_MULA(0);

    // Stop PLLB
    PMC->CKGR_PLLBR = CKGR_PLLBR_MULB(0);
}


static void restoreClock(uint32_t PllarVal, uint32_t PllbrVal, uint32_t MckrVal)
{
    // switch to slow clock first
    PMC->PMC_MCKR = (PMC->PMC_MCKR & (unsigned int)~PMC_MCKR_CSS_Msk) | PMC_MCKR_CSS_SLOW_CLK;
    while (!(PMC->PMC_SR & PMC_SR_MCKRDY));

    PMC->PMC_MCKR = (PMC->PMC_MCKR & (unsigned int)~PMC_MCKR_PRES_Msk) | PMC_MCKR_PRES_CLK_1;

    // Restart Main Oscillator
    PMC->CKGR_MOR = (0x37 << 16) | (0x3F<<8) | CKGR_MOR_MOSCRCEN | CKGR_MOR_MOSCXTEN;
    while (!(PMC->PMC_SR & PMC_SR_MOSCXTS));
    // Switch to moscsel
    PMC->CKGR_MOR = (0x37 << 16) | (0x3F<<8) | CKGR_MOR_MOSCRCEN | CKGR_MOR_MOSCXTEN | CKGR_MOR_MOSCSEL;
    while (!(PMC->PMC_SR & PMC_SR_MOSCSELS));

    // Switch to main oscillator
    PMC->PMC_MCKR = (PMC->PMC_MCKR & (unsigned int)~PMC_MCKR_CSS_Msk) | PMC_MCKR_CSS_MAIN_CLK;
    while (!(PMC->PMC_SR & PMC_SR_MCKRDY));

    PMC->PMC_MCKR = (PMC->PMC_MCKR & (unsigned int)~PMC_MCKR_PRES_Msk) | PMC_MCKR_PRES_CLK_1;
    while (!(PMC->PMC_SR & PMC_SR_MCKRDY));

    // Restart PLL A
    if ((PllarVal & CKGR_PLLAR_MULA_Msk) != 0) {
        PMC->CKGR_PLLAR = PllarVal;
        while(!(PMC->PMC_SR & PMC_SR_LOCKA));
    }

    // Restart PLL B
    if ((PllbrVal & CKGR_PLLBR_MULB_Msk) != 0) {
        PMC->CKGR_PLLBR = PllbrVal;
        while(!(PMC->PMC_SR & PMC_SR_LOCKB));
    }

    // Switch to fast clock
    PMC->PMC_MCKR = (MckrVal & (unsigned int)~PMC_MCKR_CSS_Msk) | PMC_MCKR_CSS_MAIN_CLK;
    while (!(PMC->PMC_SR & PMC_SR_MCKRDY));

    PMC->PMC_MCKR = MckrVal;
    while (!(PMC->PMC_SR & PMC_SR_MCKRDY));
}



static uint32_t ScrLPModes_Wait_OnInitialize( SWGTScreen* pScreen )
{
    WGT_Screen_Initialize( pScreen, GUICLR_WHITE, NULL ) ;

    return SAMGUI_E_OK ;
}

static uint32_t ScrLPModes_Wait_OnDrawBackground( SWGTScreen* pScreen )
{
    SGUIColor clr ;
    clr.u.dwRGBA=GUICLR_ATMEL_BLUE ;

    // Draw Atmel logo and label
    g_WGT_CoreData.pBE->DrawBitmap( 4, 8, 87, 40, DEMO_BMP_LOGO_ATMEL ) ;
    g_WGT_CoreData.pBE->DrawText( 105, 8, DEMO_LABEL"\nLow-power\n  modes", &clr, (void*)&g_Font10x14, 0 ) ;

    // Draw Backup bitmap and Title
    g_WGT_CoreData.pBE->DrawBitmap( 91, 66, 58, 59, DEMO_BMP_WAIT_MODE ) ;
    g_WGT_CoreData.pBE->DrawText( 66, 148, "Wait mode", &clr, (void*)&g_Font10x14, 0 ) ;
    //g_WGT_CoreData.pBE->DrawText( 12, 176, "Power consumption:\n  typically 15 uA", &clr, (void*)&g_Font10x14, 0 ) ;

    // Draw labels
    g_WGT_CoreData.pBE->DrawText( 42, 240, "Init mode ...", &clr, (void*)&g_Font10x14, 0 ) ;

    return SAMGUI_E_OK ;
}

static uint32_t ScrLPModes_Wait_ProcessMessage( SWGTScreen* pScreen, SWGTCoreMessage* pMsg )
{
    SGUIColor clr ;
    uint32_t i = 0;

//    printf( "ScrLPModes_Wait - msg id %x\r\n", pMsg->dwID ) ;

    switch ( pMsg->dwID )
    {
        case WGT_MSG_TIMER :

            clr.u.dwRGBA = GUICLR_WHITE;
            g_WGT_CoreData.pBE->DrawText( 42, 240, "Init mode ...", &clr, (void*)&g_Font10x14, 0 ) ;

            clr.u.dwRGBA = GUICLR_ATMEL_BLUE;
            g_WGT_CoreData.pBE->DrawText( 25, 240, "Touch the screen\n   to wake-up", &clr, (void*)&g_Font10x14, 0 ) ;

            // Backup clock settings
            g_PllarValBackup = PMC->CKGR_PLLAR;
            g_PllbrValBackup = PMC->CKGR_PLLBR;
            g_MckrValBackup  = PMC->PMC_MCKR;

            // Disable all ITs
            taskENTER_CRITICAL();
            g_irq[0] = NVIC->ISER[0];
            g_irq[1] = NVIC->ISER[1];
            NVIC->ICER[0] = 0xFFFFFFFF;
            NVIC->ICER[1] = 0xFFFFFFFF;

#if defined _DEMO_USE_USB_
            // Disable USB
            USBD_Disconnect();
            // Disable Transceiver
            UDP->UDP_TXVC |= UDP_TXVC_TXVDIS;
            // Disable USB Peripheral clock
            PMC_DisablePeripheral(ID_UDP);
            // Disable USB Clock
            REG_PMC_SCDR = PMC_SCER_UDP;
#endif // _DEMO_USE_USB_

            // Switch to 4MHz RC clock
            switchMckFastRC();

            // Wait the Fast RC to stabilize
            while (!(PMC->PMC_SR & PMC_SR_MOSCRCS));

            /* Disable unused peripherals */

            NVIC_DisableIRQ( RTC_IRQn ) ;
            NVIC_ClearPendingIRQ( RTC_IRQn ) ;
            NVIC_DisableIRQ( RTC_IRQn ) ;
            RTC_DisableIt( RTC, RTC_IER_SECEN ) ;

            // Set wakeup input for fast startup
            PMC->PMC_FSMR &= (unsigned int)~0x7FFFF;
            PMC->PMC_FSMR |= g_waitWakeUpPin;

            //PMC->PMC_FSMR |= PMC_FSMR_LPM;
            PMC->PMC_FSMR |= PMC_FSMR_FLPM_FLASH_STANDBY;
            SCB->SCR &= (unsigned int)~(1 << 2);

            /* Set WS to 0 */
            EFC0->EEFC_FMR = (0 << 8);
            PMC->CKGR_MOR = CKGR_MOR_KEY_PASSWD |(1<<2);
            while (!(PMC->PMC_SR & PMC_SR_MCKRDY));
            // Enter Wait Mode
            __WFE();

            // Waiting for MOSCRCEN bit is cleared is strongly recommended
            // to ensure that the core will not execute undesired instructions
            for (i = 0; i < 500; i++)
            {
                __NOP();
            }
            while (!(PMC->CKGR_MOR & CKGR_MOR_MOSCRCEN));
            /*Restore WS */
            EFC0->EEFC_FMR = (3 << 8);
            restoreClock(g_PllarValBackup, g_PllbrValBackup, g_MckrValBackup);

            LED_Configure( 2 ) ;
            LED_Set( 2 ) ;
            NVIC_DisableIRQ( RTC_IRQn ) ;
            NVIC_ClearPendingIRQ( RTC_IRQn ) ;
            NVIC_EnableIRQ( RTC_IRQn ) ;
            RTC_EnableIt( RTC, RTC_IER_SECEN ) ;
            /* Re-start peripherals */

            // Re-enable all ITs
            NVIC->ISER[0] = g_irq[0];
            NVIC->ISER[1] = g_irq[1];
            taskEXIT_CRITICAL();

#if defined _DEMO_USE_USB_
            // Enable USB Clock
            REG_PMC_SCER = PMC_SCER_UDP;
            // Enable USB Peripheral clock
            PMC_EnablePeripheral(ID_UDP);
            // Enable Transceiver
            UDP->UDP_TXVC &= ~UDP_TXVC_TXVDIS;
            // Disable USB
            USBD_Connect();
#endif // _DEMO_USE_USB_

            // Reset wakeup inputs
            PMC->PMC_FSMR = 0;
            PMC->PMC_FSPR = 0;

            // Go back to previous screen
            WGT_SetCurrentScreen( &ScrLPModes );
        break;
    }

    return SAMGUI_E_OK ;
}

extern SWGTScreen ScrLPModes_Wait=
{
    .ProcessMessage=ScrLPModes_Wait_ProcessMessage,

    /* Hooks */
    .HkBeforePaint=NULL,
    .HkAfterPaint=NULL,
    .HkBeforeEraseBackground=NULL,
    .HkAfterEraseBackground=NULL,
    .HkExit=NULL,

    /* Callbacks */
    .OnInitialize=ScrLPModes_Wait_OnInitialize,
    .OnEraseBackground=ScrLPModes_Wait_OnDrawBackground,
    .OnPaint=NULL
} ;

/** @} */
/** @} */
