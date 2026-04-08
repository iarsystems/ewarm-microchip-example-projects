/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support 
 * ----------------------------------------------------------------------------
 * Copyright (c) 2008, Atmel Corporation
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

//------------------------------------------------------------------------------
//         Headers
//------------------------------------------------------------------------------

#include <board.h>
#include <pio/pio.h>
#include <pio/pio_it.h>
#include <rtc/rtc.h>
#include <dbgu/dbgu.h>
#include <supc/supc.h>
#include <pmc/pmc.h>
#include <slcdc/slcdc.h>
#include <components/slcd/s7lstklcd/s7lstklcd.h>
#include <utility/trace.h>

#include <stdio.h>

//------------------------------------------------------------------------------
//         Local definitions
//------------------------------------------------------------------------------

/// Backup mode is currently selected.
#define MODE_BACKUP             0
/// Backup mode with SRAM and RTC is currently selected.
#define MODE_BACKUP_SRAM_RTC    1
/// Backup mode with SRAM, RTC, BOD and LCD is currently selected.
#define MODE_BACKUP_BOD_LCD     2
/// Wait mode with flash disabled, deep mode at 1.55V.
#define MODE_WAIT               3
/// Idle mode at 500Hz, deep mode at 1.55V.
#define MODE_IDLE               4
/// 37MHz mode is currently selected.
#define MODE_37MHZ              5
/// Off mode is currently selected.
#define MODE_OFF                6

/// Wake-up pins definition for wake-up registers.
#define WAKEUP_PINS             0x1F

//------------------------------------------------------------------------------
//         Local variables
//------------------------------------------------------------------------------

/// Pio pins to configure.
static const Pin pPinsButtons[] = {PINS_PUSHBUTTONS};

/// Current power mode.
#ifdef __ICCARM__
#pragma location=".backup"
#else
__attribute__ ((section (".backup")))
#endif
static volatile unsigned char currentMode = MODE_37MHZ;

/// Signals that the device should wake up.
static volatile unsigned int wakeUp = 0;

//------------------------------------------------------------------------------
//         Local functions
//------------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/// Interrupt handler for the keyboard matrix. Sets a flag to wake up the
/// device.
//-----------------------------------------------------------------------------
static void ISR_WakeUp(void)
{
    wakeUp = 1;
}

//-----------------------------------------------------------------------------
/// Configures an interrupt on the wake-up pins (for idle and 37MHz mode).
//-----------------------------------------------------------------------------
static void ConfigureWakeUpInterrupt(void)
{
    unsigned int i;

    PIO_InitializeInterrupts(7);
    for (i=0; i < PIO_LISTSIZE(pPinsButtons); i++) {

        PIO_ConfigureIt(&pPinsButtons[i], (void (*)(const Pin *)) ISR_WakeUp);
        PIO_EnableIt(&pPinsButtons[i]);
    }
}

//-----------------------------------------------------------------------------
/// Waits for the wakeUp variable to be set.
//-----------------------------------------------------------------------------
static void Wait(void)
{
    wakeUp = 0;
    while (wakeUp == 0);
}

//-----------------------------------------------------------------------------
/// Switches the master clock to PLL@37MHz.
//-----------------------------------------------------------------------------
void Configure37Mhz(void)
{
    // Set 3 wait states
    AT91C_BASE_MC->MC_FMR = AT91C_MC_FWS_3FWS;

    // Switch to slow clock to change PLL settings
    AT91C_BASE_PMC->PMC_MCKR = AT91C_PMC_CSS_SLOW_CLK;
    while ((AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY) != AT91C_PMC_MCKRDY);

    // Configure PLL at 37MHz
    AT91C_BASE_PMC->PMC_PLLR = 0x0469BF01;
    while ((AT91C_BASE_PMC->PMC_SR & AT91C_PMC_LOCK) == 0);

    // Switch to PLL
    AT91C_BASE_PMC->PMC_MCKR = AT91C_PMC_CSS_PLL_CLK;
    while ((AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY) == 0);

    DBGU_Configure(DBGU_STANDARD, 115200, 37000000);
}

//-----------------------------------------------------------------------------
/// Switches the master clock to the internal 2MHz RC oscillator.
//-----------------------------------------------------------------------------
static void Configure2Mhz(void)
{
    // Switch to 2 MHz RC oscillator
    AT91C_BASE_PMC->PMC_MOR = AT91C_PMC_MAINSELS | (0x37 << 16); 
    while ((AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MAINSELS) != AT91C_PMC_MAINSELS);
    AT91C_BASE_PMC->PMC_MCKR = AT91C_PMC_CSS_MAIN_CLK;
    while ((AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY) != AT91C_PMC_MCKRDY);

    // Set 0 wait states
    AT91C_BASE_MC->MC_FMR = AT91C_MC_FWS_0FWS;
}

//-----------------------------------------------------------------------------
/// Switches the master clock to SlowClock@500Hz.
//-----------------------------------------------------------------------------
void Configure500Hz(void)
{
    // Set slow clock
    AT91C_BASE_PMC->PMC_MCKR = AT91C_PMC_CSS_SLOW_CLK;
    while ((AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY) != AT91C_PMC_MCKRDY);
    AT91C_BASE_PMC->PMC_MCKR = AT91C_PMC_CSS_SLOW_CLK | AT91C_PMC_PRES_CLK_64;
    while ((AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY) != AT91C_PMC_MCKRDY);

    // Set 0 wait state
    AT91C_BASE_MC->MC_FMR = AT91C_MC_FWS_0FWS;
}

//-----------------------------------------------------------------------------
/// Disables the PLL.
//-----------------------------------------------------------------------------
static void DisablePll(void)
{
    AT91C_BASE_PMC->PMC_PLLR = 0x2 << 14;
}

#if defined(__ICCARM__)
static __ramfunc // IAR
#else
__attribute__ ((section (".ramfunc"))) static // GCC
#endif
//-----------------------------------------------------------------------------
/// Disables the internal flash, then the internal 2 MHz RC oscillator. Upon
/// wakeup, re-enables the internal flash.
//-----------------------------------------------------------------------------
void EnterWaitMode(void)
{
    // Disable Flash
    SUPC_DisableFlash();

    // Disable main oscillator
    PMC_DisableMainOscillatorForWaitMode();

    // Re-enable flash
    SUPC_EnableFlash(0xFF);
}

//------------------------------------------------------------------------------
/// Displays the currently selected mode and runs it.
//------------------------------------------------------------------------------
static void RunCurrentMode(void)
{
    volatile unsigned int time;
    unsigned char seconds;

    printf("-I- Run(%d)\n\r", currentMode);

    // Display string on SLCD
    SLCDC_Clear();
    switch (currentMode) {

        case MODE_37MHZ: S7LSTKLCD_PutString("37 MHZ"); break;
        case MODE_BACKUP: S7LSTKLCD_PutString("BACK#1"); break;
        case MODE_OFF: S7LSTKLCD_PutString("OFF"); break;
        case MODE_BACKUP_SRAM_RTC: S7LSTKLCD_PutString("BACK#2"); break;
        case MODE_BACKUP_BOD_LCD: S7LSTKLCD_PutString("BACK#3"); break;
        case MODE_WAIT: S7LSTKLCD_PutString("WAIT"); break;
        case MODE_IDLE: S7LSTKLCD_PutString("IDLE"); break;
    }
   
    // Wait for a few seconds
    for (time=0; time < 1000000; time++);

    // Run mode
    switch (currentMode) {

        case MODE_37MHZ:
            Configure37Mhz();
            PMC_DisableMainOscillator();
            SUPC_DisableSram();
            SUPC_DisableRtc();
            SUPC_SetBodSampling(AT91C_SUPC_BODSMPL_DISABLED);
            ConfigureWakeUpInterrupt();
            Wait();
            break;

        case MODE_OFF:
            SUPC_Shutdown();
            break;

        case MODE_BACKUP:
            SLCDC_Disable();
            SUPC_DisableSlcd();
            SUPC_SetWakeUpSources(AT91C_SUPC_FWUPEN);
            SUPC_SetWakeUpInputs(WAKEUP_PINS);
            SUPC_DisableSram();
            SUPC_DisableRtc();
            SUPC_SetBodSampling(AT91C_SUPC_BODSMPL_DISABLED);
            SUPC_DisableVoltageRegulator();
            break;

        case MODE_BACKUP_SRAM_RTC:
            SLCDC_Disable();
            SUPC_DisableSlcd();
            SUPC_EnableSram();
            SUPC_EnableRtc();
            RTC_GetTime(0, 0, &seconds);
            seconds = (seconds + 5) % 60;
            RTC_SetTimeAlarm(0, 0, &seconds);
            AT91C_BASE_RTC->RTC_SCCR = AT91C_RTC_ALARM;
            SUPC_SetWakeUpSources(AT91C_SUPC_FWUPEN | AT91C_SUPC_RTCEN);
            SUPC_SetWakeUpInputs(WAKEUP_PINS);
            SUPC_SetBodSampling(AT91C_SUPC_BODSMPL_DISABLED);
            SUPC_DisableVoltageRegulator();
            break;

        case MODE_BACKUP_BOD_LCD:
            SUPC_SetWakeUpSources(AT91C_SUPC_FWUPEN);
            SUPC_SetWakeUpInputs(WAKEUP_PINS);
            SUPC_EnableSram();
            SUPC_EnableRtc();
            SUPC_SetBodSampling(AT91C_SUPC_BODSMPL_32_SLCK);
            SUPC_DisableVoltageRegulator();
            break;

        case MODE_WAIT:
            PMC_SetFastWakeUpInputs(WAKEUP_PINS);
            SLCDC_Disable();
            SUPC_DisableSlcd();
            SUPC_DisableSram();
            SUPC_DisableRtc();
            SUPC_SetBodSampling(AT91C_SUPC_BODSMPL_DISABLED);
            Configure2Mhz();
            DisablePll();
            SUPC_SetVoltageOutput(0x2 << 9);
            SUPC_EnableDeepMode();
            EnterWaitMode();
            SUPC_DisableDeepMode();
            SUPC_SetVoltageOutput(0x5 << 9);
            Configure37Mhz();
            break;

        case MODE_IDLE:
            ConfigureWakeUpInterrupt();
            Configure500Hz();
            DisablePll();
            PMC_DisableMainOscillator();
            SLCDC_Disable();
            SUPC_DisableSlcd();
            SUPC_SetBodSampling(AT91C_SUPC_BODSMPL_DISABLED);
            SUPC_DisableSram();
            SUPC_DisableRtc();
            SUPC_SetVoltageOutput(0x2 << 9);
            SUPC_EnableDeepMode();
            PMC_DisableProcessorClock();
            SUPC_DisableDeepMode();
            SUPC_SetVoltageOutput(0x5 << 9);
            Configure37Mhz();
            break;
    }
}

//------------------------------------------------------------------------------
//         Global functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Initializes the SLCD & the keyboard, and starts in the default power mode.
//------------------------------------------------------------------------------
int main(void)
{
    unsigned int i;

    TRACE_CONFIGURE(DBGU_STANDARD, 115200, BOARD_MCK);

    // Display message only at startup
    if (currentMode == MODE_37MHZ) {
        
        printf("-- Basic LowPower stk Project %s --\n\r", SOFTPACK_VERSION);
        printf("-- %s\n\r", BOARD_NAME);
        printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);
    }

    while (1) {

        // Initialize SLCD
        SUPC_SetSlcdVoltage(0x8);
        SUPC_EnableSlcd(AT91C_SUPC_LCDMODE_INTERNAL);
        SLCDC_Disable();
        SLCDC_Configure(S7LSTKLCD_NUM_COMMONS,
                        S7LSTKLCD_NUM_SEGMENTS,
                        AT91C_SLCDC_BIAS_1_4,
                        AT91C_SLCDC_BUFTIME_0_percent);
        SLCDC_SetFrameFreq(AT91C_SLCDC_PRESC_SCLK_32, AT91C_SLCDC_DIV_1);
        SLCDC_Enable();
        SLCDC_Clear();

        // Initialize pushbuttons
        PIO_Configure(pPinsButtons, PIO_LISTSIZE(pPinsButtons));

        // Scan buttons
        for (i=0; i < PIO_LISTSIZE(pPinsButtons); i++) {

            if (!PIO_Get(&pPinsButtons[i])) {

                currentMode = i;
                break;
            }
        }

        // Run current mode
        RunCurrentMode();
    }
}

