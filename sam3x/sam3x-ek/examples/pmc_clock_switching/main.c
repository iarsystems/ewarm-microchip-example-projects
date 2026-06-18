/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support
 * ----------------------------------------------------------------------------
 * Copyright (c) 2010, Atmel Corporation
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
 * \page pmc_clock_switching PMC Clock Switching Example.
 *
 * \section Purpose
 * This example shows how to switch from a clock to another (PLLA, UPLL, SLCK, MAINCK)
 * or change divider.
 *
 * \section Requirements
 *
 * This package can be used with SAM3X evaluation kits.
 *
 * \section Description
 *
 * Upon startup, the program configure pios for UART,PCK and botton. The baud rate of
 * UART is configured as 2400 bps. The application does some printf with the current
 * configuration (except 32KHz slow clock ) and waits for button pressed to switch to
 * next configuration. PCK Outputs can be selected from the clocks provided by the
 * clock (PLLA, UPLL, SLCK, MAINCK) and driven on the pin PCK0 on PB22.
 * After the clock switchs, the PCK output signal can be measured by scope compare with
 * the clock configuration.
 *
 * <ul>
 * <li> The Clock Generator integrates a 32,768 Hz low-power oscillator.
 * In order to use this oscillator, the XIN32 and XOUT32 pins must be connected
 * to a 32,768 Hz crystal. The user can select the crystal oscillator to be the source of
 * the slow clock, as it provides a more accurate frequency. The command is made by
 * function SUPC_SelectExtCrystal32K().</li>
 * <li> MAINCK is the output of the Main Clock Oscillator selection: either the Crystal
 * Oscillator or 4/8/12 MHz Fast RC Oscillator. The user can select the output frequency
 * of the Fast RC Oscillator: either 4 MHz, 8 MHz or 12 MHz are available. It can be done
 * through function PMC_EnableIntRC4_8_12MHz(). The user can also select the 3 to 20 MHz Crystal
 * oscillator to be the source of MAINCK, as it provides a more accurate frequency.
 * The function PMC_EnableExtOsc12MHz() show how to enable the oscillator. </li>
 * <li> The PMC features PLL Blocks that permit a wide range of frequencies to be
 * selected on either the master clock, the processor clock or the programmable clock outputs.
 * Function PMC_ConfigureMckWithPlla() descripts the deatil how to configure PLLA.</li>
 * </ul>
 * \section Usage
 *
 * -# Build the program and download it inside the evaluation board. Please
 *    refer to the
 *    <a href="http://www.atmel.com/dyn/resources/prod_documents/doc6224.pdf">
 *    SAM-BA User Guide</a>, the
 *    <a href="http://www.atmel.com/dyn/resources/prod_documents/doc6310.pdf">
 *    GNU-Based Software Development</a> application note or to the
 *    <a href="ftp://ftp.iar.se/WWWfiles/arm/Guides/EWARM_UserGuide.ENU.pdf">
 *    IAR EWARM User Guide</a>, depending on your chosen solution.
 * -# On the computer, open and configure a terminal application
 *    (e.g. HyperTerminal on Microsoft Windows) with these settings:
 *   - 2400 bauds
 *   - 8 bits of data
 *   - No parity
 *   - 1 stop bit
 *   - No flow control
 * -# Start the application.
 * -# In the terminal window, the following text should appear:
 *    \code
 *     -- Pmc clock Switching Example xxx --
 *     -- xxxxxx-xx
 *     -- Compiled: xxx xx xxxx xx:xx:xx --
 *    \endcode
 * -# Press the left push button to perform the corresponding action.
 *
 * \section References
 * - pmc_clock_switching/main.c
 */

/**
 * \file
 *
 * This file contains all the specific code for the pmc clock switching example.
 */

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

#include "board.h"

#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>

/*----------------------------------------------------------------------------
 *        Local definitions
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 *        Local variables
 *----------------------------------------------------------------------------*/
/** Pin PCK0 */
const Pin pinPCK = PIN_PCK0 ;

/** Pushbutton pin instance. */
const Pin pinPB = PIN_PB_LEFT_CLICK ;

/** Mutual semaphore. */
static volatile uint8_t _ucWaitButton = 0 ;

/*----------------------------------------------------------------------------
 *        Local functions
 *----------------------------------------------------------------------------*/
/**
 *  \brief Handler for button rising edge interrupt.
 */
static void _Button_Handler( const Pin *pin )
{
    if ( pin == &pinPB )
    {
        _ucWaitButton = 0 ;
    }
}

/**
 *  \brief Configure the Pushbutton
 *
 *  Configure the PIO as inputs and generate corresponding interrupt when
 *  pressed or released.
 */
static void _ConfigureButton( void )
{
    /* Configure pio as inputs. */
    PIO_PinConfigure( &pinPB, 1 ) ;
    /* Adjust pio debounce filter patameters, uses 10 Hz filter. */
    PIO_PinSetDebounceFilter( &pinPB, 10 ) ;
    /* Initialize pios interrupt handlers, see PIO definition in board.h. */
    PIO_PinConfigureIt( &pinPB, _Button_Handler ) ; /* Interrupt on rising edge  */
    /* Enable PIO controller IRQs. */
    NVIC_EnableIRQ( (IRQn_Type)pinPB.id ) ;
    /* Enable PIO line interrupts. */
    PIO_PinEnableIt( &pinPB ) ;
}

/**
 * \brief Configure UART with given master clock, and Configure PCK with given
 *  divider source of master clock and prescaler.
 *
 * \param css  The master clock divider source.
 * \param pres Master Clock prescaler.
 * \param masterClk frequency of the master clock (in Hz).
 */
static void _ConfigureUartAndPck( uint32_t dwClockSource, uint32_t dwPrescaler, uint32_t dwMasterClock )
{
    /* Configures an UART Baudrate 2400 bps (except slow clock)*/
    if ( dwMasterClock > 32768 )
    {
        UART_Configure( 2400, dwMasterClock ) ;
    }

    /* First, disable programmable clock */
    PMC_DisablePck(PMC_SCDR_PCK0);
    /* Then, configure PMC Programmable Clock */
    PMC_ConfigurePck(0, dwClockSource, dwPrescaler);
    /* Finally, enable programmable clock */
    PMC_EnablePck(PMC_SCER_PCK0);
}

/*----------------------------------------------------------------------------
 *         Global functions
 *----------------------------------------------------------------------------*/
/**
 * \brief Application entry point for pmc_clock switch example.
 *
 * \return Unused (ANSI-C compatibility).
 */
extern int main( void )
{
    /* Disable watchdog */
    WDT_Disable( WDT ) ;

    /* Configure UART baud rate to 2400bps */
    UART_Configure( 2400, BOARD_MCK ) ;

    /* Output example information */
    printf( "-- Pmc Clock Switching example %s --\n\r", SOFTPACK_VERSION ) ;
    printf( "-- %s\n\r", BOARD_NAME ) ;
    printf( "-- Compiled: %s %s --\n\r", __DATE__, __TIME__ ) ;

    /* Configure PCK as peripheral */
    PIO_PinConfigure( &pinPCK, 1 ) ;
    /* PIO configuration for button. */
    PIO_InitializeInterrupts( 0 ) ;
    /* Configure button debouncing */
    _ConfigureButton() ;

    _ConfigureUartAndPck( PMC_PCK_CSS_MCK, PMC_PCK_PRES_CLK_1, BOARD_MCK ) ;
    printf( "\n\rThe master clock can be measured on PCK0 pin(PB22) by a scope.\n\r" ) ;

    printf( "-I- The current MCK is configured to PLLA(%dMHz) in board lowlevel functions.\n\r", (int)(BOARD_MCK/1000000) ) ;
    printf( "-I- Press button LEFT CLICK to continue.\n\r" ) ;
    for ( _ucWaitButton=1 ; _ucWaitButton ; ) ;

    /* =========================================================================
     * Switch MCK from PLLA to Slow clock.
     */
    printf( "\n\r-I- Switch MCK from PLLA(%dMHz) to Slow clock.\n\r", (int)(BOARD_MCK/1000000) ) ;
    printf( "-I- Please measure the clock on PCK to make sure it is 32KHz.\n\r" ) ;
    printf( "-I- Press button LEFT CLICK to continue.\n\r" ) ;

    /* Enable external 32K for more accurate frequency (optional) */
    SUPC_SelectExtCrystal32K();
    /* Switch MCK to Slow clock  */
    PMC_SetMckSelection(PMC_MCKR_CSS_SLOW_CLK, PMC_PCK_PRES_CLK_1);
    /* Disable unused clock to save power (optional) */
    PMC_SetPllaClock(0, 0);
    PMC_DisableIntRC4_8_12MHz();
    PMC_DisableExtOsc12MHz();

    _ConfigureUartAndPck( PMC_PCK_CSS_MCK, PMC_PCK_PRES_CLK_1, 32768 ) ;
    for( _ucWaitButton=1 ; _ucWaitButton ; ) ;

    /* =========================================================================
     * Switch MCK from Slow clock to Fast RC
     */
    /* First, enable Fast RC */
    PMC_EnableIntRC4_8_12MHz(FAST_RC_4MHZ);
    /* Then, Switch MCK to main clock  */
    PMC_SetMckSelection(PMC_MCKR_CSS_MAIN_CLK, PMC_PCK_PRES_CLK_1);
    /* Disable unused clock to save power (optional) */
    PMC_DisableExtOsc12MHz();

    _ConfigureUartAndPck( PMC_PCK_CSS_MCK, PMC_PCK_PRES_CLK_1, 4000000 ) ;
    printf( "\n\r-I- Switch MCK from Slow clock to Fast RC(4MHz).\n\r" ) ;
    printf( "-I- The internal Fast RC is the source for main clock.\n\r" ) ;
    printf( "-I- Please measure the clock on PCK to make sure it is 4MHz.\n\r" ) ;
    printf( "-I- Press button LEFT CLICK to continue.\n\r" ) ;
    for( _ucWaitButton=1 ; _ucWaitButton ; ) ;

    /* =========================================================================
     * Switch MCK from Fast RC to Main XTAL oscillator
     */
    printf( "\n\r-I- Switch MCK from Fast RC to Main Osc(12MHz).\n\r" ) ;
    printf( "-I- The external Main Osc(12MHz) is the source for main clock.\n\r" ) ;
    printf( "-I- Please measure the clock on PCK to make sure it is 12MHz.\n\r" ) ;
    printf( "-I- Press button LEFT CLICK to continue.\n\r" ) ;

    /* First, switch MCK to Slow clock  */
    PMC_SetMckSelection(PMC_MCKR_CSS_SLOW_CLK, PMC_PCK_PRES_CLK_1);
    /* Then, enable Main XTAL Osc */
    PMC_EnableExtOsc12MHz();
    /* Then, Switch MCK to main clock  */
    PMC_SetMckSelection(PMC_MCKR_CSS_MAIN_CLK, PMC_PCK_PRES_CLK_1);
    /* Disable unused clock to save power (optional) */
    PMC_DisableIntRC4_8_12MHz();

    _ConfigureUartAndPck( PMC_PCK_CSS_MCK, PMC_PCK_PRES_CLK_1, 12000000 ) ;
    for( _ucWaitButton=1 ; _ucWaitButton ; ) ;

    /* =========================================================================
     * Switch MCK from Main XTAL oscillator to Fast RC
     */
    printf( "\n\r-I- Switch MCK from Main Osc(12MHz) to Fast RC(4MHz).\n\r" ) ;
    printf( "-I- The internal Fast RC is the source for main clock.\n\r" ) ;
    printf( "-I- Please measure the clock on PCK to make sure it is 4MHz.\n\r" ) ;
    printf( "-I- Press button LEFT CLICK to continue.\n\r" ) ;

    /* First, switch MCK to Slow clock  */
    PMC_SetMckSelection(PMC_MCKR_CSS_SLOW_CLK, PMC_PCK_PRES_CLK_1);
    /* Then, enable Fast RC */
    PMC_EnableIntRC4_8_12MHz(FAST_RC_4MHZ);
    /* Then, Switch MCK to main clock  */
    PMC_SetMckSelection(PMC_MCKR_CSS_MAIN_CLK, PMC_PCK_PRES_CLK_1);
    /* Disable unused clock to save power (optional) */
    PMC_DisableExtOsc12MHz();

    _ConfigureUartAndPck( PMC_PCK_CSS_MCK, PMC_PCK_PRES_CLK_1, 4000000 ) ;
    for( _ucWaitButton=1 ; _ucWaitButton ; ) ;

    /* =========================================================================
     * Switch MCK from Fast RC to Slow clock.
     */
    printf( "\n\r-I- Switch MCK from Fast RC(4MHz) to Slow clock(32K).\n\r" ) ;
    printf( "-I- Please measure the clock on PCK to make sure it is 32KHz.\n\r" ) ;
    printf( "-I- Press button LEFT CLICK to continue.\n\r" ) ;
    /* Switch MCK to Slow clock  */
    PMC_SetMckSelection(PMC_MCKR_CSS_SLOW_CLK, PMC_PCK_PRES_CLK_1);
    /* Disable unused clock to save power (optional) */
    PMC_SetPllaClock(0, 0);
    PMC_DisableIntRC4_8_12MHz();
    PMC_DisableExtOsc12MHz();

    _ConfigureUartAndPck( PMC_PCK_CSS_MCK, PMC_PCK_PRES_CLK_1, 32768 ) ;
    for( _ucWaitButton=1 ; _ucWaitButton ; ) ;

    /* =========================================================================
     * Switch MCK from Slow clock to Main oscillator.
     */
    /* First, enable Main XTAL Osc */
    PMC_EnableExtOsc12MHz();
    /* Then, switch MCK to main clock */
    PMC_SetMckSelection(PMC_MCKR_CSS_MAIN_CLK, PMC_PCK_PRES_CLK_1);
    /* Disable unused clock to save power (optional) */
    PMC_DisableIntRC4_8_12MHz();

    _ConfigureUartAndPck( PMC_PCK_CSS_MCK, PMC_PCK_PRES_CLK_1, 12000000 ) ;
    printf( "\n\r-I- Switch MCK from Slow clock(32K) to Main Osc.\n\r" ) ;
    printf( "-I- Please measure the clock on PCK to make sure it is 12MHz.\n\r" ) ;
    printf( "-I- Press button LEFT CLICK to continue.\n\r" ) ;
    for( _ucWaitButton=1 ; _ucWaitButton ; ) ;

    /* =========================================================================
     * Switch MCK from Main oscillator to UPLL.
     */
    printf( "\n\r-I- Switch MCK from Main Osc to UPLL.\n\r" ) ;
    printf( "-I- Please measure the clock on PCK to make sure it is 60MHz.\n\r" ) ;
    printf( "-I- Press button LEFT CLICK to continue.\n\r" ) ;

    /* First, switch MCK to main clock */
    PMC_SetMckSelection(PMC_MCKR_CSS_MAIN_CLK, PMC_PCK_PRES_CLK_1);
    /* Then, enable UPLL clock */
    PMC_EnableUpllClock();
    /* Then, switch MCK to UPLL */
    PMC_SetMckSelection(PMC_MCKR_CSS_UPLL_CLK, PMC_PCK_PRES_CLK_4);
    /* Disable unused clock to save power (optional) */
    PMC_DisableIntRC4_8_12MHz();

    _ConfigureUartAndPck( PMC_PCK_CSS_MCK, PMC_PCK_PRES_CLK_1, 480000000/2/4 ) ;
    for( _ucWaitButton=1 ; _ucWaitButton ; ) ;

    /* =========================================================================
     * Switch MCK from UPLL to PLLA.
     */
    printf( "\n\r-I- Switch MCK from UPLL to PLLA.\n\r" ) ;
    printf( "-I- Please measure the clock on PCK to make sure it is 24MHz.\n\r" ) ;
    printf( "-I- Press button LEFT CLICK to continue.\n\r" ) ;

    /* First, switch MCK to main clock */
    PMC_SetMckSelection(PMC_MCKR_CSS_MAIN_CLK, PMC_PCK_PRES_CLK_1);
    /* Then, cofigure PLLA and switch clock */
    PMC_ConfigureMckWithPlla(16, 1, PMC_MCKR_PRES_CLK_8); /* MCK = 12MHz * 16 / 1 / 8 = 24MHz */
    /* Disable unused clock to save power (optional) */
    PMC_DisableUpllClock();

    _ConfigureUartAndPck( PMC_PCK_CSS_MCK, PMC_PCK_PRES_CLK_1, 24000000 ) ;
    for( _ucWaitButton=1 ; _ucWaitButton ; ) ;

    /* =========================================================================
     * Switch MCK from PLLA to Fast RC.
     */
    printf( "\n\r-I- Switch MCK from PLLA to Fast RC(4MHz).\n\r" ) ;
    printf( "-I- The internal Fast RC is the source for main clock.\n\r" ) ;
    printf( "-I- Please measure the clock on PCK to make sure it is 4MHz.\n\r" ) ;
    printf( "-I- Press button LEFT CLICK to continue.\n\r" ) ;

    /* First, switch MCK to Slow clock */
    PMC_SetMckSelection(PMC_MCKR_CSS_SLOW_CLK, PMC_PCK_PRES_CLK_1);
    /* Then, enable Fast RC */
    PMC_EnableIntRC4_8_12MHz(FAST_RC_4MHZ);
    /* Then, switch MCK to main clock */
    PMC_SetMckSelection(PMC_MCKR_CSS_MAIN_CLK, PMC_PCK_PRES_CLK_1);
    /* Disable unused clock to save power (optional) */
    PMC_DisableExtOsc12MHz();
    PMC_SetPllaClock(0, 0);

    _ConfigureUartAndPck( PMC_PCK_CSS_MCK, PMC_PCK_PRES_CLK_1, 4000000 ) ;
    for( _ucWaitButton=1 ; _ucWaitButton ; ) ;

    /* =========================================================================
     * Switch MCK from Fast RC to PLLA.
     */
    printf( "\n\r-I- Switch MCK from Fast RC to PLLA.\n\r" ) ;
    printf( "-I- The external Main Osc(12MHz) is the source for main clock.\n\r" ) ;
    printf( "-I- Please measure the clock on PCK to make sure it is 48MHz.\n\r" ) ;
    printf( "-I- Press button LEFT CLICK to continue.\n\r" ) ;

    /* First, switch MCK to Slow clock */
    PMC_SetMckSelection(PMC_MCKR_CSS_SLOW_CLK, PMC_PCK_PRES_CLK_1);
    /* Then, enable Main XTAL oscillator */
    PMC_EnableExtOsc12MHz();
    /* Then, cofigure PLLA and switch clock */
    PMC_ConfigureMckWithPlla(16, 1, PMC_MCKR_PRES_CLK_4); /* MCK = 12MHz * 16 / 1 / 4 = 48MHz */
    /* Disable unused clock to save power (optional) */
    PMC_DisableIntRC4_8_12MHz();

    _ConfigureUartAndPck( PMC_PCK_CSS_MCK, PMC_PCK_PRES_CLK_1, 48000000 ) ;
    for( _ucWaitButton=1 ; _ucWaitButton ; ) ;

    /* =========================================================================
     * Switch MCK from PLLA to Main XTAL Osc.
     */
    printf( "\n\r-I- Switch MCK from PLLA to Main Osc.\n\r" ) ;
    printf( "-I- The external Main Osc(12MHz) is the source for main clock.\n\r" ) ;
    printf( "-I- Please measure the clock on PCK to make sure it is 12MHz.\n\r" ) ;

    /* Switch MCK to main clock only (here, Main Osc is already enabled before) */
    PMC_SetMckSelection(PMC_MCKR_CSS_MAIN_CLK, PMC_PCK_PRES_CLK_1);
    /* Disable unused clock to save power (optional) */
    PMC_SetPllaClock(0, 0);

    _ConfigureUartAndPck( PMC_PCK_CSS_MCK, PMC_PCK_PRES_CLK_1, 12000000 ) ;

    printf( "\n\rDone.\n\r" ) ;
    while( 1 ) ;
}

