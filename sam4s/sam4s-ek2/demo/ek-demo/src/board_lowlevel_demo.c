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

/**
 * \addtogroup SAM3S_demo AT91SAM3S demo
 * @{
 * \addtogroup group_bsp BSP
 * @{
 *
 * \section Purpose
 *
 * Provides the low-level initialization function that gets called on chip startup.
 */

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

#include "board.h"
#include "demo_parameters.h"
#include "board_lowlevel_demo.h"

/*----------------------------------------------------------------------------
 *        Local definitions
 *----------------------------------------------------------------------------*/
/** Define clock timeout */
#define CLOCK_TIMEOUT         5000


/*----------------------------------------------------------------------------
 *        Local functions
 *----------------------------------------------------------------------------*/
static void _BOARD_ConfigurePmc( void )
{

// Settings at 64 MHz for MCK


#define BOARD_OSCOUNT   (CKGR_MOR_MOSCXTST(0x8))
#define BOARD_PLLBR     (CKGR_PLLBR_MULB(24) \
                       | CKGR_PLLBR_PLLBCOUNT(0x1) \
                       | CKGR_PLLBR_DIVB(0x3))
#define BOARD_MCKR      (PMC_MCKR_PRES_CLK_1 | PMC_MCKR_CSS_PLLB_CLK)


// Define clock timeout
#undef CLOCK_TIMEOUT
#define CLOCK_TIMEOUT           0xFFFFFFFF

    uint32_t timeout = 0;

    /* Enable NRST reset
     ************************************/
    //AT91C_BASE_RSTC->RSTC_RMR |= AT91C_RSTC_URSTEN;

#ifdef DEMO_ENABLE_32KHZ_EXT
    /* Select external slow clock
     ****************************/
    if ((SUPC->SUPC_SR & SUPC_SR_OSCSEL) != SUPC_SR_OSCSEL_CRYST) {
        SUPC->SUPC_CR = SUPC_CR_XTALSEL_CRYSTAL_SEL | ((uint32_t)0xA5 << 24);
        timeout = 0;

        // Light ON green LED during polling
        REG_PIOA_SODR = 0x00100000;
        REG_PIOA_OER  = 0x00100000;
        REG_PIOA_PER  = 0x00100000;
        REG_PIOA_CODR = 0x00100000;

        while (!(SUPC->SUPC_SR & SUPC_SR_OSCSEL_CRYST) );

        // Light OFF green LED
        REG_PIOA_SODR = 0x00100000;
    }
#endif

    /* Initialize main oscillator
     ****************************/
    if(!(PMC->CKGR_MOR & CKGR_MOR_MOSCSEL))
    {

        PMC->CKGR_MOR = CKGR_MOR_KEY_PASSWD | BOARD_OSCOUNT | CKGR_MOR_MOSCRCEN | CKGR_MOR_MOSCXTEN;
        timeout = 0;
        while (!(PMC->PMC_SR & PMC_SR_MOSCXTS) && (timeout++ < CLOCK_TIMEOUT));

    }

    /* Switch to 3-20MHz Xtal oscillator */
    PMC->CKGR_MOR = CKGR_MOR_KEY_PASSWD | BOARD_OSCOUNT | CKGR_MOR_MOSCRCEN | CKGR_MOR_MOSCXTEN | CKGR_MOR_MOSCSEL;
    timeout = 0;
    while (!(PMC->PMC_SR & PMC_SR_MOSCSELS) && (timeout++ < CLOCK_TIMEOUT));
    PMC->PMC_MCKR = (PMC->PMC_MCKR & ~(uint32_t)PMC_MCKR_CSS_Msk) | PMC_MCKR_CSS_MAIN_CLK;
    timeout = 0;
    while (!(PMC->PMC_SR & PMC_SR_MCKRDY) && (timeout++ < CLOCK_TIMEOUT));

    /** Set 3 WS for Embedded Flash Access */
    EFC0->EEFC_FMR = (6 << 8);

    /* Initialize PLLA */
//    PMC->CKGR_PLLAR = BOARD_PLLAR;
//    timeout = 0;
//    while (!(PMC->PMC_SR & PMC_SR_LOCKA) && (timeout++ < CLOCK_TIMEOUT));

    /* Initialize PLLB */
    PMC->CKGR_PLLBR = BOARD_PLLBR;
    timeout = 0;
    while (!(PMC->PMC_SR & PMC_SR_LOCKB) && (timeout++ < CLOCK_TIMEOUT));

    // Set USB clock on PLLB
//    REG_PMC_USB = PMC_USB_USBS | PMC_USB_USBDIV(1);

    /* Enable PLLA for USB */
    PMC->CKGR_PLLAR = (1<<29)
                    | CKGR_PLLAR_DIVA(1)
                    | CKGR_PLLAR_MULA(7)
                    | CKGR_PLLAR_PLLACOUNT_Msk;
    while(!(PMC->PMC_SR & PMC_SR_LOCKA) && (timeout++ < CLOCK_TIMEOUT));
    /* USB Clock uses PLLA */
    PMC->PMC_USB = PMC_USB_USBDIV(1);       /* /2   */


    /* Switch to fast clock
     **********************/
    PMC->PMC_MCKR = (BOARD_MCKR & ~PMC_MCKR_CSS_Msk) | PMC_MCKR_CSS_MAIN_CLK;
    timeout = 0;
    while (!(PMC->PMC_SR & PMC_SR_MCKRDY) && (timeout++ < CLOCK_TIMEOUT));

    PMC->PMC_MCKR = BOARD_MCKR;
    timeout = 0;
    while (!(PMC->PMC_SR & PMC_SR_MCKRDY) && (timeout++ < CLOCK_TIMEOUT));

}

/*----------------------------------------------------------------------------
 *        Exported functions
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/**
 * \brief Performs the low-level initialization of the chip. This includes EFC,
 * master clock and watchdog configuration.
 */
/*----------------------------------------------------------------------------*/
extern void LowLevelInit( void )
{
    /** Configure PMC */
    _BOARD_ConfigurePmc() ;
}

/** @}
 * @} */
