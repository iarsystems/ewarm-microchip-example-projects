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


//-----------------------------------------------------------------------------
/// \dir "Basic Keyboard Matrix Project"
///
/// !!!Purpose
///
/// How to handle the keyboard matrix of the AT91SAM7L-EK
///
/// !!!Requirements
///
/// This package can be used with AT91SAM7L-EK Atmel evaluation kit
///
/// !!!Description
///
/// Management of a matrix keyboard, in our case, the keyboard used in 
/// AT91SAM7L-EK
///
/// !!!Usage
///
/// -# Build the program and flash it inside the evaluation board. Please
///    refer to the
///    <a href="http://www.atmel.com/dyn/resources/prod_documents/doc6224.pdf">
///    SAM-BA User Guide</a>, the
///    <a href="http://www.atmel.com/dyn/resources/prod_documents/doc6310.pdf">
///    GNU-Based Software Development</a> application note or to the
///    <a href="ftp://ftp.iar.se/WWWfiles/arm/Guides/EWARM_UserGuide.ENU.pdf">
///    IAR EWARM User Guide</a>, depending on your chosen solution.
/// -# On the computer, open and configure a terminal application
///    (e.g. HyperTerminal on Microsoft Windows) with these settings:
///   - 115200 bauds
///   - 8 bits of data
///   - No parity
///   - 1 stop bit
///   - No flow control
/// -# Start the application by pressing the wakeup key
/// -# In the terminal window, the following text should appear:
///     \code
///     -- Basic Keyboard Matrix Project xxx --
///     -- AT91SAMxx-xx
///     -- Compiled: xxx xx xxxx xx:xx:xx --
///     -I- Configuring PIOs
///     -I- Configuring TC0
///     -I- Configuring keyboard matrix driver
///     -I- Press any key on the board
///     \endcode
/// -# Pressing any key outputs the corresponding character on the DBGU. After
/// a set inactivity period, the program will output a message to notify it has
/// disabled the TC. Pressing any key resumes the detection process. 
/// Here is an example output trace:
///     \code
///     0
///     .
///     -I- No activity, shutting down timer ...
///     -I- Activity detected, restarting timer ...
///     8
///     9
///     \endcode
//-----------------------------------------------------------------------------

//------------------------------------------------------------------------------
//         Headers
//------------------------------------------------------------------------------

#include <board.h>
#include <pio/pio.h>
#include <pio/pio_it.h>
#include <irq/irq.h>
#include <dbgu/dbgu.h>
#include <tc/tc.h>
#include <utility/assert.h>
#include <utility/led.h>
#include <kbmatrix/kbmatrix.h>
#include <kbmatrix/s7lekkbm/s7lekkbm.h>
#include <utility/trace.h>

//------------------------------------------------------------------------------
//         Local definitions
//------------------------------------------------------------------------------

/// Keyboard matrix scan sample rate (in Hz).
#define SAMPLE_RATE         200

/// Number of timer ticks without activity before the timer is stopped (and PIO
/// interrupt activated to resume it).
#define TIMEOUT             (SAMPLE_RATE * 5)

//------------------------------------------------------------------------------
//         Local variables
//------------------------------------------------------------------------------

/// List of keyboard matrix row pins.
static const Pin pinsRow[] = {PINS_KBMATRIX_ROWS};
/// List of keyboard matrix column pins.
static const Pin pinsCol[] = {PINS_KBMATRIX_COLS};

/// Keyboard matrix driver instance.
static KbMatrix kbMatrix;

/// Indicates if 2nd function is currently active or not.
static volatile unsigned char secondFunction = 0;
/// Indicates if 2nd function lock is currently active or not.
static volatile unsigned char secondFunctionLock = 0;

/// String to output for each special key.
static const unsigned char pSpecialKeys[][12] = {

    "SquareRoot",
    "%",
    "+/-",
    "1/x",
    "x^y",
    "mode",
    "2ndF",
    "/",
    "exp",
    "hex/dec/bin",
    "2ndFLock",
    "x",
    "log",
    "up",
    "pause",
    "esc",
    "-",
    "left",
    "ok",
    "play",
    "right",
    ".",
    "=",
    "+",
    "ln",
    "down",
    "stop",
    "del"
};

//------------------------------------------------------------------------------
//         Local functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// PIO interrupt handler. Re-enables the timer interrupt.
/// \param pPin  Pointer to the pin triggering the interrupt.
//------------------------------------------------------------------------------
static void ISR_Pio(const Pin *pPin)
{
    unsigned int i;

    printf("-I- Activity detected, restarting timer ...\n\r");

    // Acknowledge & disable PIO interrupt
    for (i=0; i < PIO_LISTSIZE(pinsRow); i++) {

        PIO_Set(&(pinsRow[i]));
    }
    for (i=0; i < PIO_LISTSIZE(pinsCol); i++) {

        PIO_DisableIt(&(pinsCol[i]));
    }

    // Restart timer
    IRQ_EnableIT(AT91C_ID_TC0);
    TC_Start(AT91C_BASE_TC0);
}

//------------------------------------------------------------------------------
/// Timer Counter 0 interrupt handler. Forwards the interrupt to the keyboard
/// matrix driver.
//------------------------------------------------------------------------------
static void ISR_Tc0(void)
{
    static unsigned int timeout = 0;
    unsigned int i;

    // Acknowledge interrupt
    AT91C_BASE_TC0->TC_SR;

    // Invoke driver timer handler
    if (KBMATRIX_Scan(&kbMatrix)) {

        timeout = 0;
    }
    else {

        timeout++;
    }

    // Check if timeout has expired
    if (timeout == TIMEOUT) {

        printf("-I- No activity, shutting down timer ...\n\r");
        timeout = 0;

        // Disable timer interrupt
        IRQ_DisableIT(AT91C_ID_TC0);
        TC_Stop(AT91C_BASE_TC0);

        // Enable PIO interrupt
        for (i=0; i < PIO_LISTSIZE(pinsRow); i++) {

            PIO_Clear(&(pinsRow[i]));
        }
        for (i=0; i < PIO_LISTSIZE(pinsCol); i++) {

            PIO_EnableIt(&(pinsCol[i]));
        }
    }
}

//------------------------------------------------------------------------------
/// Callback invoked when a key is pressed or released.
/// \param key  Key index.
/// \param pressed  Indicates if the key has been pressed or released.
//------------------------------------------------------------------------------
static void KeyStateChanged(unsigned int key, unsigned char pressed)
{
    unsigned char code;

    // Retrieve key code (depends on 2ndf and 2ndfLock keys state)
    if ((!secondFunction && !secondFunctionLock)
        || (secondFunction && secondFunctionLock)) {

        code = gpKeyboardMatrix[key];
    }
    else {

        code = gpKeyboardMatrixAlt[key];
    }

    // Treat key code
    if (pressed) {

        if (S7LEKKBM_SPECIALKEY(code)) {

            switch (code) {

                case S7LEKKBM_2NDF: secondFunction = 1; break;
                case S7LEKKBM_2NDF_LOCK:
                    secondFunctionLock = !secondFunctionLock;
                    LED_Toggle(0);
                    break;
                default: printf("%s\n\r", pSpecialKeys[code - 128]);
            }
        }
        else {
        
            printf("%c\n\r", code);
        }
    }
    else {

        if (code == S7LEKKBM_2NDF) {

            secondFunction = 0;
        }
    }
}

//------------------------------------------------------------------------------
//         Global functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Main function
//------------------------------------------------------------------------------
int main()
{
    unsigned int div, tcclks;
    unsigned int i;

    // Configure DBGU
    TRACE_CONFIGURE(DBGU_STANDARD, 115200, BOARD_MCK);
    printf("-- Basic Keyboard Matrix Project %s --\n\r", SOFTPACK_VERSION);
    printf("-- %s\n\r", BOARD_NAME);
    printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);

    // Configure keyboard matrix pins
    printf("-I- Configuring PIOs\n\r");
    PIO_Configure(pinsRow, PIO_LISTSIZE(pinsRow));
    PIO_Configure(pinsCol, PIO_LISTSIZE(pinsCol));

    PIO_InitializeInterrupts(0x0);
    for (i=0; i < PIO_LISTSIZE(pinsCol); i++) {

        PIO_ConfigureIt(&(pinsCol[i]), ISR_Pio);
    }

    // Configure LED0 to display 2ndF lock status
    LED_Configure(0);
    LED_Clear(0);

    // Configure TC0
    printf("-I- Configuring TC0\n\r");
    AT91C_BASE_PMC->PMC_PCER = 1 << AT91C_ID_TC0;
    ASSERT(TC_FindMckDivisor(SAMPLE_RATE, BOARD_MCK, &div, &tcclks),
           "-F- Could not generate the desired timer frequency\n\r");
    TC_Configure(AT91C_BASE_TC0, tcclks | AT91C_TC_WAVE | AT91C_TC_WAVESEL_UP_AUTO);
    AT91C_BASE_TC0->TC_RC = BOARD_MCK / (SAMPLE_RATE * div);

     // Configure TC0 interrupt
    IRQ_DisableIT(AT91C_ID_TC0);
    IRQ_ConfigureIT(AT91C_ID_TC0, 0, ISR_Tc0);
    AT91C_BASE_TC0->TC_IER = AT91C_TC_CPCS;
    IRQ_EnableIT(AT91C_ID_TC0);

    // Initialize keyboard
    printf("-I- Configuring keyboard matrix driver\n\r");
    KBMATRIX_Initialize(&kbMatrix,
                        pinsRow,
                        PIO_LISTSIZE(pinsRow),
                        pinsCol,
                        PIO_LISTSIZE(pinsCol),
                        KeyStateChanged);

    // Start timer
    TC_Start(AT91C_BASE_TC0);

    printf("-I- Press any key on the board\n\r");

    // Infinite loop (key presses are treated in interrupt routines
    while (1);
}

