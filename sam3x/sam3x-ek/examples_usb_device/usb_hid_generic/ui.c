/**
 * \file
 *
 * \brief User Interface
 *
 * Copyright (C) 2009 Atmel Corporation. All rights reserved.
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 * Atmel AVR product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 */

/*_____  I N C L U D E S ___________________________________________________*/

#include "board.h"
#include "compiler.h"
#include "conf_usb.h"
#include "udc.h"
#include "udi_hid_generic.h"
#include "ui.h"

/*_____ D E F I N I T I O N S ______________________________________________*/

#define LED_On      LED_Set
#define LED_Off     LED_Clear
#define LED_1       LED_BLUE
#define LED_2       LED_AMBER
#define LED_3       LED_GREEN

/*_____ V A R I A B L E S __________________________________________________*/

/* Pins for Buttons */
static const Pin pinsButtons[] = {PINS_PUSHBUTTONS};

/*_____ L O C A L   F U N C T I O N S ______________________________________*/

//! Interrupt on "pin change" from switch to do wakeup on USB
//! Note:
//! This interrupt is enable when the USB host enable remotewakeup feature
//! This interrupt wakeup the CPU if this one is in idle mode
static void _Button_Handlers(const Pin * pin)
{
    pin = pin;
    // It is a wakeup then send wakeup USB
    udc_wakeup();
}

/*_____ G L O B A L   F U N C T I O N S ____________________________________*/

void ui_init(void)
{
    PIO_PinConfigure(pinsButtons, PIO_LISTSIZE(pinsButtons));
    LED_Configure(LED_1);
    LED_Off(LED_1);
    LED_Configure(LED_2);
    LED_Off(LED_2);
    LED_Configure(LED_3);
    LED_Off(LED_3);

    /* register button handler */
    irqflags_t flags = cpu_irq_save();
    PIO_PinConfigureIt(&pinsButtons[PUSHBUTTON_LEFT], _Button_Handlers);
    PIO_PinConfigureIt(&pinsButtons[PUSHBUTTON_RIGHT], _Button_Handlers);
    cpu_irq_restore(flags);
}

void ui_powerdown(void)
{
}

void ui_wakeup_enable(void)
{
    PIO_PinEnableIt(&pinsButtons[PUSHBUTTON_LEFT]);
    PIO_PinEnableIt(&pinsButtons[PUSHBUTTON_RIGHT]);
}

void ui_wakeup_disable(void)
{
    PIO_PinDisableIt(&pinsButtons[PUSHBUTTON_LEFT]);
    PIO_PinDisableIt(&pinsButtons[PUSHBUTTON_RIGHT]);
}

void ui_wakeup(void)
{
}

void ui_process(uint16_t framenumber)
{
}

void ui_led_on(uint8_t led_num)
{
	switch (led_num) {
	case 1:
		LED_On(LED_1);
		break;
	case 2:
		LED_On(LED_2);
		break;
	case 3:
		LED_On(LED_3);
		break;
	}
}

void ui_led_off(uint8_t led_num)
{
	switch (led_num) {
	case 1:
		LED_Off(LED_1);
		break;
	case 2:
		LED_Off(LED_2);
		break;
	case 3:
		LED_Off(LED_3);
		break;
	}
}

bool ui_button(uint8_t btn_num)
{
    switch (btn_num) {
    case 0: return (0 == PIO_PinGet(&pinsButtons[0]));
    case 1: return (0 == PIO_PinGet(&pinsButtons[1]));
    }
    return false;
}

/**
 * \defgroup usb_hid_generic_ui USB Device HID Generic User Interface
 *
 * Human interface on SAM3X-EK :
 * - PWR LED is always on after firmware startup
 * - Led D2 to D4 are controlled by USB Host application
 * - Push button events are reported to USB Host application
 * - Push buttons can be used to wakeup USB Host in remote wakeup mode.
 */
