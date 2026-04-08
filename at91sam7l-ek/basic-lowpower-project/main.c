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
#include <utility/led.h>
#include <utility/trace.h>
#include <components/kbmatrix/kbmatrix.h>
#include <components/slcd/s7leklcd/s7leklcd.h>

#include <stdio.h>

//------------------------------------------------------------------------------
//         Local definitions
//------------------------------------------------------------------------------

/// Number of key scans to perform before switching to the default mode.
#define SCAN_TIMEOUT            20

/// 37MHz mode is currently selected.
#define MODE_37MHZ              0
/// Backup mode is currently selected.
#define MODE_BACKUP             1
/// Off mode is currently selected.
#define MODE_OFF                2
/// Backup mode with SRAM and RTC is currently selected.
#define MODE_BACKUP_SRAM_RTC    3
/// Backup mode with SRAM, RTC, BOD and LCD is currently selected.
#define MODE_BACKUP_BOD_LCD     4
/// Wait mode with flash disabled, deep mode at 1.55V.
#define MODE_WAIT               5
/// Idle mode at 500Hz, deep mode at 1.55V.
#define MODE_IDLE               6

/// Keyboard rows pin definition for wake-up registers.
#define KEYBOARD_ROWS           0x1F

//------------------------------------------------------------------------------
//         Local variables
//------------------------------------------------------------------------------

/// Pio pins to configure.
static const Pin pins[] = {PIN_USART0_TXD_POW};
static const Pin pinsRows[] = {PINS_KBMATRIX_ROWS};
static const Pin pinsCols[] = {PINS_KBMATRIX_COLS};
static const Pin pinsWakeIdle = {0x1F, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_INPUT, PIO_PULLUP};
static const Pin pinsWakeUp[] = {PINS_KBMATRIX_WAKEUP};

/// Keyboard matrix driver instance.
static KbMatrix kbMatrix;

/// Current power mode.
#ifdef __ICCARM__
#pragma location=".backup"
#else
__attribute__ ((section (".backup")))
#endif
static unsigned char currentMode = MODE_37MHZ;

/// Signals that the device should wake up.
static volatile unsigned int wakeUp = 0;

//------------------------------------------------------------------------------
//         Local functions
//------------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/// Interrupt handler for the keyboard matrix. Sets a flag to wake up the
/// device.
//-----------------------------------------------------------------------------
static void ISR_KbWakeUp(void)
{
    wakeUp = 1;
}

//-----------------------------------------------------------------------------
/// Configures an interrupt on the wake-up pins (for idle and 37MHz mode).
//-----------------------------------------------------------------------------
static void ConfigureWakeUpInterrupt(void)
{
    PIO_InitializeInterrupts(7);
    PIO_ConfigureIt(&pinsWakeIdle, (void (*)(const Pin *)) ISR_KbWakeUp);
    PIO_EnableIt(&pinsWakeIdle);
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
    switch (currentMode) {

        case MODE_37MHZ: S7LEKLCD_PutString("37 MHZ", 0); break;
        case MODE_BACKUP: S7LEKLCD_PutString("BACKUP", 0); break;
        case MODE_OFF: S7LEKLCD_PutString("OFF", 0); break;
        case MODE_BACKUP_SRAM_RTC: S7LEKLCD_PutString("BACKUP SR+RT", 0); break;
        case MODE_BACKUP_BOD_LCD: S7LEKLCD_PutString("BACKUP LCD", 0); break;
        case MODE_WAIT: S7LEKLCD_PutString("WAIT", 0); break;
        case MODE_IDLE: S7LEKLCD_PutString("IDLE", 0); break;
    }
   
    // Wait for a few seconds
    for (time=0; time < 100000; time++) {

        LED_Toggle(0);
    }

    LED_Clear(0);
    LED_Set(1);

    // Run mode
    switch (currentMode) {

        case MODE_37MHZ:
            Configure37Mhz();
            PMC_DisableMainOscillator();
            SUPC_DisableSram();
            SUPC_DisableRtc();
            SUPC_SetBodSampling(AT91C_SUPC_BODSMPL_DISABLED);
            PIO_Configure(pinsWakeUp, PIO_LISTSIZE(pinsWakeUp));
            ConfigureWakeUpInterrupt();
            Wait();
            break;

        case MODE_OFF:
            SUPC_Shutdown();
            break;

        case MODE_BACKUP:
            PIO_Configure(pinsWakeUp, PIO_LISTSIZE(pinsWakeUp));
            SLCDC_Disable();
            SUPC_DisableSlcd();
            SUPC_SetWakeUpSources(AT91C_SUPC_FWUPEN);
            SUPC_SetWakeUpInputs(KEYBOARD_ROWS);
            SUPC_DisableSram();
            SUPC_DisableRtc();
            SUPC_SetBodSampling(AT91C_SUPC_BODSMPL_DISABLED);
            SUPC_DisableVoltageRegulator();
            break;

        case MODE_BACKUP_SRAM_RTC:
            PIO_Configure(pinsWakeUp, PIO_LISTSIZE(pinsWakeUp));
            SLCDC_Disable();
            SUPC_DisableSlcd();
            SUPC_EnableSram();
            SUPC_EnableRtc();
            RTC_GetTime(0, 0, &seconds);
            seconds = (seconds + 5) % 60;
            RTC_SetTimeAlarm(0, 0, &seconds);
            AT91C_BASE_RTC->RTC_SCCR = AT91C_RTC_ALARM;
            SUPC_SetWakeUpSources(AT91C_SUPC_FWUPEN | AT91C_SUPC_RTCEN);
            SUPC_SetWakeUpInputs(KEYBOARD_ROWS);
            SUPC_SetBodSampling(AT91C_SUPC_BODSMPL_DISABLED);
            SUPC_DisableVoltageRegulator();
            break;

        case MODE_BACKUP_BOD_LCD:
            PIO_Configure(pinsWakeUp, PIO_LISTSIZE(pinsWakeUp));
            SUPC_SetWakeUpSources(AT91C_SUPC_FWUPEN);
            SUPC_SetWakeUpInputs(KEYBOARD_ROWS);
            SUPC_EnableSram();
            SUPC_EnableRtc();
            SUPC_SetBodSampling(AT91C_SUPC_BODSMPL_32_SLCK);
            SUPC_DisableVoltageRegulator();
            break;

        case MODE_WAIT:
            PIO_Configure(pinsWakeUp, PIO_LISTSIZE(pinsWakeUp));
            PMC_SetFastWakeUpInputs(KEYBOARD_ROWS);
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
            PIO_Configure(pinsWakeUp, PIO_LISTSIZE(pinsWakeUp));
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
/// Callback invoked whenever a key has been pressed or released.
//------------------------------------------------------------------------------
static void KeyStateChanged(unsigned int key, unsigned char pressed)
{
    if (pressed) {

        switch (key) {

            case 0: currentMode = MODE_37MHZ; break;
            case 1: currentMode = MODE_BACKUP; break;
            case 2: currentMode = MODE_OFF; break;
            case 3: currentMode = MODE_BACKUP_SRAM_RTC; break;
            case 4: currentMode = MODE_BACKUP_BOD_LCD; break;
            case 5: currentMode = MODE_WAIT; break;
            case 6: currentMode = MODE_IDLE; break;
        }
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

    PIO_Configure(pins, PIO_LISTSIZE(pins));
    TRACE_CONFIGURE(DBGU_STANDARD, 115200, BOARD_MCK);
    printf("-- Basic LowPower Project %s --\n\r", SOFTPACK_VERSION);
    printf("-- %s\n\r", BOARD_NAME);
    printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);

    while (1) {

        LED_Configure(0);
        LED_Configure(1);
        LED_Set(0);

        // Initialize SLCD
        SUPC_SetSlcdVoltage(0x9);
        SUPC_EnableSlcd(AT91C_SUPC_LCDMODE_INTERNAL);
        SLCDC_Disable();
        SLCDC_Configure(S7LEKLCD_NUM_COMMONS,
                        S7LEKLCD_NUM_SEGMENTS,
                        AT91C_SLCDC_BIAS_1_3,
                        AT91C_SLCDC_BUFTIME_2_Tsclk);
        SLCDC_SetFrameFreq(AT91C_SLCDC_PRESC_SCLK_32, AT91C_SLCDC_DIV_1);
        SLCDC_Enable();
        S7LEKLCD_Symbol(S7LEKLCD_ATMEL, 1);

        // Initialize keyboard
        PIO_Configure(pinsRows, PIO_LISTSIZE(pinsRows));
        PIO_Configure(pinsCols, PIO_LISTSIZE(pinsCols));
        KBMATRIX_Initialize(&kbMatrix,
                            pinsRows,
                            PIO_LISTSIZE(pinsRows),
                            pinsCols,
                            PIO_LISTSIZE(pinsCols),
                            KeyStateChanged);

        // Scan
        i = 0;
        while ((i < SCAN_TIMEOUT) && !KBMATRIX_Scan(&kbMatrix)) {

            i++;
        }

        // Run current mode
        RunCurrentMode();
    }
}

