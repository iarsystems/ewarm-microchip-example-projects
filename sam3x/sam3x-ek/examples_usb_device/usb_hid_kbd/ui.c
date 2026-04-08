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
#include "udi_hid_kbd.h"
#include "ui.h"

/*_____ D E F I N I T I O N S ______________________________________________*/

#define LED_On      LED_Set
#define LED_Off     LED_Clear
#define LED_USB     LED_GREEN
#define LED_HID     LED_BLUE
#define LED_NUMLOCK LED_AMBER


/*_____ V A R I A B L E S __________________________________________________*/

/* Pins for Buttons */
static const Pin pinsButtons[] = {PINS_PUSHBUTTONS};

static bool btn_last_states[] = {false, false};

static struct{
bool b_modifier;
bool b_down;
uint8_t u8_value;
} ui_sequence[] = {
    // Display windows menu
    {true,true,HID_MODIFIER_LEFT_UI},
        // Launch Windows Command line
        {false,true,HID_R},
        {false,false,HID_R},
    {true,false,HID_MODIFIER_LEFT_UI},
    // Tape sequence "notepad" + return
    {false,true,HID_N},
    {false,false,HID_N},
    {false,true,HID_O},
    {false,false,HID_O},
    {false,true,HID_T},
    {false,false,HID_T},
    {false,true,HID_E},
    {false,false,HID_E},
    {false,true,HID_P},
    {false,false,HID_P},
    {false,true,HID_A},
    {false,false,HID_A},
    {false,true,HID_D},
    {false,false,HID_D},
    {false,true,HID_ENTER},
    {false,false,HID_ENTER},
    // Display "Atmel "
    {true,true,HID_MODIFIER_RIGHT_SHIFT},   // Enable Maj
    {false,true,HID_A},
    {false,false,HID_A},
    {true,false,HID_MODIFIER_RIGHT_SHIFT},  // Disable Maj
    {false,true,HID_T},
    {false,false,HID_T},
    {false,true,HID_M},
    {false,false,HID_M},
    {false,true,HID_E},
    {false,false,HID_E},
    {false,true,HID_L},
    {false,false,HID_L},
    {false,true,HID_SPACEBAR},
    {false,false,HID_SPACEBAR},
    // Display "ARM "
    {false,true,HID_CAPS_LOCK},         // Enable caps lock
    {false,false,HID_CAPS_LOCK},
    {false,true,HID_A},
    {false,false,HID_A},
    {false,true,HID_R},
    {false,false,HID_R},
    {false,true,HID_M},
    {false,false,HID_M},
    {false,true,HID_CAPS_LOCK},         // Disable caps lock
    {false,false,HID_CAPS_LOCK},
};

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
    LED_Configure(LED_USB);
    LED_Configure(LED_HID);
    LED_Configure(LED_NUMLOCK);

    /* register button handler */
    irqflags_t flags = cpu_irq_save();
    PIO_PinConfigureIt(&pinsButtons[PUSHBUTTON_LEFT], _Button_Handlers);
    PIO_PinConfigureIt(&pinsButtons[PUSHBUTTON_RIGHT], _Button_Handlers);
    cpu_irq_restore(flags);

    ui_powerdown();
}

void ui_powerdown(void)
{
    LED_Off(LED_USB);
    LED_Off(LED_HID);
    LED_Off(LED_NUMLOCK);
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
    LED_On(LED_USB);
}

void ui_process(uint16_t framenumber)
{
    bool b_btn_states[2], sucess;
    static bool sequence_running = false;
    static uint8_t u8_sequence_pos = 0;
    uint8_t u8_value;
    static uint16_t cpt_sof = 0;

    if ((framenumber % 1000) == 0) {
        LED_On(LED_HID);
    }
    if ((framenumber % 1000) == 500) {
        LED_Off(LED_HID);
    }

    // Scan process running each 2ms
    cpt_sof++;
    if ((cpt_sof%2)==0)
        return;

    // Scan buttons on switch 0 to send keys sequence
    b_btn_states[PUSHBUTTON_LEFT] = (0 == PIO_PinGet(&pinsButtons[PUSHBUTTON_LEFT])) ? true : false;
    if (b_btn_states[PUSHBUTTON_LEFT] != btn_last_states[PUSHBUTTON_LEFT]) {
        btn_last_states[PUSHBUTTON_LEFT] = b_btn_states[PUSHBUTTON_LEFT];
        sequence_running = true;
    }

    // Sequence process running each 150ms
    if (150 > cpt_sof)
        return;
    cpt_sof = 0;

    if (sequence_running) {
        // Send next key
        u8_value = ui_sequence[u8_sequence_pos].u8_value;
        if (ui_sequence[u8_sequence_pos].b_modifier) {
            if (ui_sequence[u8_sequence_pos].b_down) {
                sucess = udi_hid_kbd_modifier_down(u8_value);
            }else{
                sucess = udi_hid_kbd_modifier_up(u8_value);
            }
        }else{
            if (ui_sequence[u8_sequence_pos].b_down) {
                sucess = udi_hid_kbd_down(u8_value);
            }else{
                sucess = udi_hid_kbd_up(u8_value);
            }
        }
        if (!sucess)
            return; // Retry it on next schedule
  
        // Valid sequence position
        u8_sequence_pos++;
        if (u8_sequence_pos>=sizeof(ui_sequence)/sizeof(ui_sequence[0])) {
            u8_sequence_pos=0;
            sequence_running=false;
        }
    }
    else {
        b_btn_states[PUSHBUTTON_RIGHT] = (0 == PIO_PinGet(&pinsButtons[PUSHBUTTON_RIGHT]));
        if (b_btn_states[PUSHBUTTON_RIGHT] != btn_last_states[PUSHBUTTON_RIGHT]) {
            btn_last_states[PUSHBUTTON_RIGHT] = b_btn_states[PUSHBUTTON_RIGHT];
            if (b_btn_states[PUSHBUTTON_RIGHT])
                udi_hid_kbd_down(HID_CAPS_LOCK);
            else
                udi_hid_kbd_up(HID_CAPS_LOCK);
        }
    }
}

void ui_kbd_led(uint8_t value)
{
    if (value & HID_LED_CAPS_LOCK) {
        LED_On(LED_NUMLOCK);
    }else{
        LED_Off(LED_NUMLOCK);
    }
}

/**
 * \defgroup usb_hid_kbd_ui USB Device HID Keyboard User Interface
 *
 * Human interface on SAM3X-EK :
 * - PWR LED is always on after firmware startup
 * - Led GREEN (D4) is on when USB line is in IDLE mode, and off in SUSPEND mode
 * - Led BLUE (D2) blinks when USB Host have checked and enabled HID Keyboard interface
 * - The led AMBER (D3) displays caps lock status.
 * - The push button "LEFT CLICK" open a note pad and send key sequence "Atmel ARM"
 * - The push button "RIGHT CLICK" switches caps lock status.
 */
