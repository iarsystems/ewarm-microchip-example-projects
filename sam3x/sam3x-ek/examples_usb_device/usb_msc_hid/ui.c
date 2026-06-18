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
#include "udi_msc.h"
#include "udc.h"
#include "udi_hid_mouse.h"
#include "ui.h"

/*_____ D E F I N I T I O N S ______________________________________________*/

#define  MOUSE_MOVE_RANGE  3

#define LED0    LED_GREEN
#define LED1    LED_BLUE
#define LED2    LED_AMBER
#define LED3    LED_AMBER
#define LED_On  LED_Set
#define LED_Off LED_Clear

/*_____ G L O B A L   V A R I A B L E S ____________________________________*/

/** Current I/O bytes/s for MSC */
int ms_current_io = -1;
/** Reading bytes count in a second for MSC */
int ms_cnt_read = 0;
/** Writing bytes count in a second for MSC */
int ms_cnt_write = 0;

/*_____ L O C A L   V A R I A B L E S ______________________________________*/

/* Pins for Buttons */
static const Pin pinsButtons[] = {PINS_PUSHBUTTONS};

/** MAX I/O bytes/s for MSC */
static int ms_max_io = 0;

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
    LED_Configure(LED0);
    LED_Configure(LED1);
	LED_Configure(LED2);
	LED_Configure(LED3);

    /* register button handler */
    irqflags_t flags = cpu_irq_save();
    PIO_PinConfigureIt(&pinsButtons[PUSHBUTTON_LEFT], _Button_Handlers);
    PIO_PinConfigureIt(&pinsButtons[PUSHBUTTON_RIGHT], _Button_Handlers);
    cpu_irq_restore(flags);

    LED_On(LED0);
    LED_Off(LED1);
}

void ui_powerdown(void)
{
    LED_Off(LED0);
    LED_Off(LED1);
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
    LED_On(LED0);
}

void ui_start_read(void)
{
    LED_On(LED2);
}

void ui_stop_read(void)
{
    LED_Off(LED2);
}

void ui_start_write(void)
{
    LED_On(LED3);
}

void ui_stop_write(void)
{
    LED_Off(LED3);
}

void ui_process(uint16_t framenumber)
{
    static uint8_t cpt_sof = 0;

    if ((framenumber % 1000) == 0) {
        LED_On(LED1);
    }
    if ((framenumber % 1000) == 500) {
        LED_Off(LED1);
    }
    // MSC refresh running each 1s
  #if 1
    if ((framenumber % 1000) == 0) {
        ms_current_io = ms_cnt_read + ms_cnt_write;
        ms_max_io = max(max(ms_cnt_read, ms_cnt_write), ms_max_io);
        printf("\r R %5dKB/s, W %5dKB/s, Max %5dKB/s", ms_cnt_read/2, ms_cnt_write/2, ms_max_io/2);
        ms_cnt_read = 0; ms_cnt_write = 0;
    }
    if (UART_IsRxReady())
    {
        UART_GetChar();
        ms_max_io = 0;
    }
  #endif
    // Scan process running each 2ms
    cpt_sof++;
    if (2 > cpt_sof)
        return;
    cpt_sof = 0;

    // Scan move on LEFT CLICK
    if (!PIO_PinGet(&pinsButtons[PUSHBUTTON_LEFT])) {
        udi_hid_mouse_moveX(-MOUSE_MOVE_RANGE);
    }
    // Scan move on RIGHT CLICK
    if (!PIO_PinGet(&pinsButtons[PUSHBUTTON_RIGHT])) {
        udi_hid_mouse_moveX(+MOUSE_MOVE_RANGE);
    }

}

/**
 * \defgroup usb_msc_hid_ui USB Device Composite MSC HID User Iinterface
 *
 * Human interface on SAM3X-EK :
 *
 * - PWR led is on when power present
 * - Led 0 (D4, green) is on when USB line is in IDLE mode, and off in SUSPEND mode
 * - Led 1 (D2, blue) blinks when USB Host have checked and enabled HID and MSC interfaces
 * - Led 2 & 3 (D3, amber) is on during read/write operation
 * - LEFT CLICK to move mouse left.
 * - RIGHT CLICK to move mouse right.
 */
