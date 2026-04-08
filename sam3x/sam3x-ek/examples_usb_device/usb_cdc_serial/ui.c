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
#include "ui.h"

/*_____ D E F I N I T I O N S ______________________________________________*/

#define LED_On          LED_Set
#define LED_Off         LED_Clear
#define LED_CDC         LED_GREEN
#define LED_CDCXFR      LED_BLUE
#define LED_ERROR       LED_AMBER

/*_____ V A R I A B L E S __________________________________________________*/

/*_____ L O C A L   F U N C T I O N S ______________________________________*/

/*_____ G L O B A L   F U N C T I O N S ____________________________________*/

void ui_init(void)
{
    LED_Configure(LED_CDC);
    LED_Configure(LED_CDCXFR);
    LED_Configure(LED_ERROR);

    ui_powerdown();
}

void ui_powerdown(void)
{
    LED_Off(LED_CDC);
    LED_Off(LED_CDCXFR);
    LED_Off(LED_ERROR);
}

void ui_wakeup(void)
{
}

void ui_com_open(void)
{
    LED_On(LED_CDC);
}

void ui_com_close(void)
{
    LED_Off(LED_CDC);
    LED_Off(LED_CDCXFR);
    LED_Off(LED_ERROR);
}

void ui_com_rx_start(void)
{
    LED_On(LED_CDCXFR);
}

void ui_com_rx_stop(void)
{
    LED_Off(LED_CDCXFR);
}

void ui_com_tx_start(void)
{
    LED_On(LED_CDCXFR);
}

void ui_com_tx_stop(void)
{
    LED_Off(LED_CDCXFR);
}

void ui_com_error(void)
{
    printf("com err\n\r");
    LED_On(LED_ERROR);
}

void ui_com_overflow(void)
{
    printf("ovf\n\r");
    LED_On(LED_ERROR);
}


void ui_process(uint16_t framenumber)
{
    framenumber = framenumber;
}


/**
 * \defgroup usb_cdc_serial_ui USB Device CDC Serial User Interface
 *
 * Human interface on SAM3X-EK:
 * - PWR led is always on after firmware startup
 * - Led D4 green is on when USB Host checked and enabled CDC COM.
 * - Led D2 blue is on during data transfer between USART and CDC
 * - Led D3 amber means an error (frame error on USART or overflow)
 */
