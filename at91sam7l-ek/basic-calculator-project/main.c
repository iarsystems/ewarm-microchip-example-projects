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
/// \dir "Basic Calculator Project"
///
/// !!!Purpose
///
/// This examples implements a very simple calculator to demonstrate  
/// how the backup modes of the  microcontroller family can be used in an actual application. 
///
///
/// !!!Description
///
/// The project implements a simple calculator. It has the following particularities: 
///   - Integer operations only (add, subtract, multiply, divide). 
///   - Clear using the DEL key. 
///
/// The point of the example is not the calculator itself, but its use of two backup
/// modes. The first backup mode is used between each keypress. In this mode, the SLCD is still powered, 
/// which makes it completely transparent to the user while reducing power consumption. 
/// The device instantly wakes-up whenever any key is pressed. 
///
/// !!!Usage
///
/// -# Build the program and download it inside the evaluation board. Please
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
/// -# Start the application.
/// -# The SLCD should display the Atmel logo and the 0 value. 
/// -# Use the device like a standard calculator. Traces on the DBGU show 
///      the transition between the three different operating modes (i.e. normal, backup, deep backup). 
///
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/// \unit
///
/// !Purpose
///
/// This file contains all the specific code for the
/// basic-calculator-project
///
/// !Contents
///
/// The code can be roughly broken down as follows:
///    - Initialize SLCD
///    - Initial display
///    - Initialize keyboard
///    - Process key code calculating
///    - Print out the result
///    - Enter backup mode 
///
/// Please refer to the list of functions in the #Overview# tab of this unit
/// for more detailed information.
//-----------------------------------------------------------------------------

//------------------------------------------------------------------------------
//         Headers
//------------------------------------------------------------------------------

#include <board.h>
#include <pio/pio.h>
#include <dbgu/dbgu.h>
#include <slcdc/slcdc.h>
#include <rtc/rtc.h>
#include <supc/supc.h>
#include <components/kbmatrix/kbmatrix.h>
#include <components/kbmatrix/s7lekkbm/s7lekkbm.h>
#include <components/slcd/s7leklcd/s7leklcd.h>
#include <utility/trace.h>

#include <stdio.h>

//------------------------------------------------------------------------------
//         Local definitions
//------------------------------------------------------------------------------

/// The calculator is waiting for the first operand to be input by the user.
#define MODE_FIRST_OPERAND          1
/// The calculator is waiting for the user to choose an operator.
#define MODE_OPERATOR               2
/// The caculator is waiting for the second operand to be input.
#define MODE_SECOND_OPERAND         3
/// Result of operation is being computed.
#define MODE_COMPUTE_RESULT         4
/// The calculation result is being displayed.
#define MODE_DISPLAY_RESULT         5
/// The operation has caused an overflow.
#define MODE_OVERFLOW               6
/// A division by 0 has been attempted.
#define MODE_DIV_BY_ZERO            7

/// No operator selected.
#define OPERATOR_NONE               0
/// '+' operator.
#define OPERATOR_PLUS               1
/// '-' operator.
#define OPERATOR_MINUS              2
/// '*' operator.
#define OPERATOR_MULTIPLY           3
/// '/' operator.
#define OPERATOR_DIVIDE             4

/// Maximum number of digits in operand.
#define MAX_OPERAND_SIZE            9

/// Number of unsuccessful KBMATRIX_Scan() calls before entering backup mode.
#define KBD_TIMEOUT                 20

//------------------------------------------------------------------------------
//         Local variables
//------------------------------------------------------------------------------

/// Pio pins to configure.
static const Pin pins[] = {PIN_USART0_TXD_POW};
static const Pin pinsRows[] = {PINS_KBMATRIX_ROWS};
static const Pin pinsCols[] = {PINS_KBMATRIX_COLS};
static const Pin pinsWakeUp[] = {PINS_KBMATRIX_WAKEUP};

/// Keyboard matrix driver instance.
static KbMatrix kbMatrix;

// Backuped variables
#ifdef __ICCARM__
#pragma location=".backup"
#else
__attribute__ ((section (".backup")))
#endif
/// Current state of the calculator
static unsigned char calculatorState = MODE_FIRST_OPERAND;

#ifdef __ICCARM__
#pragma location=".backup"
#else
__attribute__ ((section (".backup")))
#endif
/// First operand.
static signed int firstOperand = 0;

#ifdef __ICCARM__
#pragma location=".backup"
#else
__attribute__ ((section (".backup")))
#endif
/// Second operand.
static signed int secondOperand = 0;

#ifdef __ICCARM__
#pragma location=".backup"
#else
__attribute__ ((section (".backup")))
#endif
/// Operator.
static unsigned char operator = 0;

#ifdef __ICCARM__
#pragma location=".backup"
#else
__attribute__ ((section (".backup")))
#endif
/// Result of last operation.
static signed int result = 0;

#ifdef __ICCARM__
#pragma location=".backup"
#else
__attribute__ ((section (".backup")))
#endif
/// Indicates if the device was in backup mode.
static unsigned char backuped = 0;

//------------------------------------------------------------------------------
//         Local functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Updates the SLCD display according to the current state of the calculator.
//------------------------------------------------------------------------------
static void RefreshDisplay(void)
{
    char pString[20];
    unsigned char num;

    switch (calculatorState) {

    case MODE_FIRST_OPERAND:
    case MODE_OPERATOR:
        // Display first operand on SLCD
        num = sprintf(pString, "%d", firstOperand);
        break;

    case MODE_SECOND_OPERAND:
        // Display second operand on SLCD
        num = sprintf(pString, "%d", secondOperand);
        break;

    case MODE_DISPLAY_RESULT:
        // Display result
        num = sprintf(pString, "%d", result);
        break;

    case MODE_OVERFLOW:
        // Display overflow error
        num = sprintf(pString, "--OVERFLOW--");
        break;
    
    case MODE_DIV_BY_ZERO:
        // Display div by 0 error
        num = sprintf(pString, "-DIV. BY 0-");
        break;
    }

    S7LEKLCD_PutString(pString, S7LEKLCD_MAX_CHARS - num);
}

//------------------------------------------------------------------------------
/// Processes the given key code, performing the associated action.
/// \param code  Code of key that was pressed.
//------------------------------------------------------------------------------
static void ProcessKeyCode(unsigned char code)
{
    char pString[20];
    unsigned char num;

    // Delete key
    if (code == S7LEKKBM_DELETE) {

        firstOperand = 0;
        secondOperand = 0;
        result = 0;
        operator = OPERATOR_NONE;
        calculatorState = MODE_FIRST_OPERAND;
    }

    // Display result
    if (calculatorState == MODE_DISPLAY_RESULT) {

        // Operator
        if ((code == S7LEKKBM_PLUS) || (code == S7LEKKBM_MINUS)
            || (code == S7LEKKBM_MULTIPLY) || (code == S7LEKKBM_DIVIDE)) {

            firstOperand = result;
            secondOperand = 0;
            calculatorState = MODE_OPERATOR;
        }
        // Number
        else if ((code >= '0') && (code <= '9')) {

            firstOperand = 0;
            operator = OPERATOR_NONE;
            secondOperand = 0;
            calculatorState = MODE_FIRST_OPERAND;
        }
        // Sign
        else if (code == S7LEKKBM_SIGN) {

            firstOperand = result;
            operator = OPERATOR_NONE;
            secondOperand = 0;
            calculatorState = MODE_FIRST_OPERAND;
        }
        // Equal
        else if (code == S7LEKKBM_EQUAL) {

            firstOperand = result;
            calculatorState = MODE_COMPUTE_RESULT;
        }
    }

    // First operand input
    if (calculatorState == MODE_FIRST_OPERAND) {

        // Compute current size of operand
        num = sprintf(pString, "%d", firstOperand);

        // Number
        if ((code >= '0') && (code <= '9')) {
            if (num < MAX_OPERAND_SIZE) {

                if (firstOperand >= 0) {

                    firstOperand = firstOperand * 10 + code - '0';
                }
                else {

                    firstOperand = firstOperand * 10 - code + '0';
                }
            }
        }
        // Sign
        else if (code == S7LEKKBM_SIGN) {

            firstOperand = -firstOperand;
        }
        // Operator
        else if ((code == S7LEKKBM_PLUS) || (code == S7LEKKBM_MINUS)
                 || (code == S7LEKKBM_MULTIPLY) || (code == S7LEKKBM_DIVIDE)) {

            calculatorState = MODE_OPERATOR;
        }
        // Result
        else if (code == S7LEKKBM_EQUAL) {

            calculatorState = MODE_COMPUTE_RESULT;
        }
    }

    // Select operator
    if (calculatorState == MODE_OPERATOR) {

        // Plus
        if (code == S7LEKKBM_PLUS) {

            operator = OPERATOR_PLUS;
        }
        // Minus
        else if (code == S7LEKKBM_MINUS) {

            operator = OPERATOR_MINUS;
        }
        // Multiply
        else if (code == S7LEKKBM_MULTIPLY) {

            operator = OPERATOR_MULTIPLY;
        }
        // Divide
        else if (code == S7LEKKBM_DIVIDE) {

            operator = OPERATOR_DIVIDE;
        }
        // Number
        if ((code >= '0') && (code <= '9')) {
            
            calculatorState = MODE_SECOND_OPERAND;
        }
    }

    // Second operand input
    if (calculatorState == MODE_SECOND_OPERAND) {

        // Compute current size of operand
        num = sprintf(pString, "%d", secondOperand);

        // Number
        if ((code >= '0') && (code <= '9')) {
            if (num < MAX_OPERAND_SIZE) {

                if (secondOperand >= 0) {
                    
                    secondOperand = secondOperand * 10 + code - '0';
                }
                else {

                    secondOperand = secondOperand * 10 - code + '0';
                }
            }
        }
        // Sign
        else if (code == S7LEKKBM_SIGN) {

            secondOperand = -secondOperand;
        }
        // Result
        else if (code == S7LEKKBM_EQUAL) {

            calculatorState = MODE_COMPUTE_RESULT;
        }
    }

    // Compute result
    if (calculatorState == MODE_COMPUTE_RESULT) {

        switch (operator) {

            case OPERATOR_NONE:
                result = firstOperand;
                calculatorState = MODE_DISPLAY_RESULT;
                break;

            case OPERATOR_PLUS:
                result = firstOperand + secondOperand;
                num = sprintf(pString, "%d", result);
                if (num > MAX_OPERAND_SIZE) {

                    calculatorState = MODE_OVERFLOW;
                }
                else {

                    calculatorState = MODE_DISPLAY_RESULT;
                }
                break;

            case OPERATOR_MINUS:
                result = firstOperand - secondOperand;
                num = sprintf(pString, "%d", result);
                if (num > MAX_OPERAND_SIZE) {

                    calculatorState = MODE_OVERFLOW;
                }
                else {

                    calculatorState = MODE_DISPLAY_RESULT;
                }
                break;

            case OPERATOR_MULTIPLY:
                num = sprintf(pString, "%d", firstOperand * secondOperand);
                if (num > MAX_OPERAND_SIZE) {

                    calculatorState = MODE_OVERFLOW;
                }
                else {

                    result = firstOperand * secondOperand;
                    calculatorState = MODE_DISPLAY_RESULT;
                }
                break;

            case OPERATOR_DIVIDE:

                if (secondOperand == 0) {

                    calculatorState = MODE_DIV_BY_ZERO;
                }
                else {

                    result = firstOperand / secondOperand;
                    calculatorState = MODE_DISPLAY_RESULT;
                }
                break;
        }
    }

    // Refresh display
    RefreshDisplay();
}

//------------------------------------------------------------------------------
/// Callback invoked whenever a key has been pressed or released.
//------------------------------------------------------------------------------
static void KeyStateChanged(unsigned int key, unsigned char pressed)
{
    unsigned char code = gpKeyboardMatrix[key];

    if (pressed) {

        ProcessKeyCode(code);
    }
}

//------------------------------------------------------------------------------
//         Global functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// State machine for the calculator.
//------------------------------------------------------------------------------
int main(void)
{
    unsigned int i;
    unsigned char seconds;

    PIO_Configure(pins, PIO_LISTSIZE(pins));
    TRACE_CONFIGURE(DBGU_STANDARD, 115200, BOARD_MCK);

    // Wake-up from the RTC -> enter deep backup
    if ((AT91C_BASE_RTC->RTC_SR & AT91C_RTC_ALARM) == AT91C_RTC_ALARM) {

        printf("-I- Deep backup\n\r");
        SUPC_SetWakeUpSources(AT91C_SUPC_FWUPEN);
        SLCDC_Disable();
        SUPC_DisableSlcd();
        SUPC_EnableSram();
        SUPC_DisableRtc();
        SUPC_DisableVoltageRegulator();
    }

    // Device was in backup mode
    if (backuped) {

        printf("-I- Wake up\n\r");
    }
    // Fresh start
    else {
        printf("-- Basic Calculator Project %s --\n\r", SOFTPACK_VERSION);
        printf("-- %s\n\r", BOARD_NAME);
        printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);
    }

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
    SLCDC_Clear();
    S7LEKLCD_Symbol(S7LEKLCD_ATMEL, 1);
    S7LEKLCD_Symbol(S7LEKLCD_CALC, 1);

    // Initial display
    RefreshDisplay();

    // Initialize keyboard
    PIO_Configure(pinsRows, PIO_LISTSIZE(pinsRows));
    PIO_Configure(pinsCols, PIO_LISTSIZE(pinsCols));
    KBMATRIX_Initialize(&kbMatrix,
                        pinsRows,
                        PIO_LISTSIZE(pinsRows),
                        pinsCols,
                        PIO_LISTSIZE(pinsCols),
                        KeyStateChanged);

    // Infinite loop
    while (1) {

        // Scan for key presses until timeout
        i = 0;
        while (i < KBD_TIMEOUT) {
            
            if (KBMATRIX_Scan(&kbMatrix)) {

                i = 0;
            }
            else {

                i++;
            }
        }

        // Enter backup mode
        printf("-I- Backup\n\r");
        backuped = 1;
        SUPC_EnableSram();
        SUPC_EnableRtc();
        RTC_GetTime(0, 0, &seconds);
        seconds = (seconds + 10) % 60;
        RTC_SetTimeAlarm(0, 0, &seconds);
        PIO_Configure(pinsWakeUp, PIO_LISTSIZE(pinsWakeUp));
        SUPC_SetWakeUpInputs(0x1F);
        SUPC_SetWakeUpSources(AT91C_SUPC_RTCEN);
        SUPC_DisableVoltageRegulator();
    }
}

