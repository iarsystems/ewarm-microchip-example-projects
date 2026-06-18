/**
 * \file
 *
 * \brief Main functions for Mouse example
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
#include "udd.h"
#include "ui.h"

/*_____ D E F I N I T I O N S ______________________________________________*/

#define STRING_EOL    "\r"
#define STRING_HEADER "-- USB Device HID Mouse Example "SOFTPACK_VERSION" --\r\n" \
                      "-- "BOARD_NAME" --\r\n" \
                      "-- Compiled: "__DATE__" "__TIME__" --"STRING_EOL

/*_____ V A R I A B L E S __________________________________________________*/

/** PIOs for USB OTGHS */
static const Pin pUOTGHSPins[] = {PIN_UOTGHS_VBOF, PIN_UOTGHS_ID, PIN_UOTGHS_FAULT};
static const Pin pUOTGHS_Fault = PIN_UOTGHS_FAULT;

static bool main_b_mouse_enable = false;

/*_____ F U N C T I O N S __________________________________________________*/

/**
 * Configure OTG settings for USB device
 */
static void _ConfigureUotghs(void)
{
    /* UTMI parallel mode, High/Full/Low Speed */
    /* UOTGCK not used in this configuration (High Speed) */
    PMC->PMC_SCDR = PMC_SCDR_UOTGCK;
    /* USB clock register: USB Clock Input is UTMI PLL */
    PMC->PMC_USB = PMC_USB_USBS;
    /* Enable peripheral clock for UOTGHS */
    PMC_EnablePeripheral(ID_UOTGHS);
    UOTGHS->UOTGHS_CTRL = UOTGHS_CTRL_UIMOD_Device;
    /* Enable PLL 480 MHz */
    PMC->CKGR_UCKR = CKGR_UCKR_UPLLEN | CKGR_UCKR_UPLLCOUNT(0xF);
    /* Wait that PLL is considered locked by the PMC */
    while( !(PMC->PMC_SR & PMC_SR_LOCKU) );

    /* UOTGHS pins */
    PIO_PinConfigure( pUOTGHSPins, PIO_LISTSIZE( pUOTGHSPins ) );
    if ( PIO_PinGet(&pUOTGHS_Fault) == 0 )
    {
        puts( "UOTGHS_Fault = 0 (active low  ERROR FLAG !"STRING_EOL );
        puts( "Undervoltage, Soft Start, Overcurrent, or Overtemperature"STRING_EOL );
        while(1);
    }

    /* IRQ */
    NVIC_EnableIRQ(UOTGHS_IRQn) ;
}

/*_____ E X P O R T E D   F U N C T I O N S ________________________________*/

/*! \brief Main function. Execution starts here.
 */
int main(void)
{ 
    /* Disable watchdog */
    WDT_Disable( WDT ) ;

    /* Output example information */
    puts( STRING_HEADER ) ;

    /* Initialize PIO interrupts */
    PIO_InitializeInterrupts(0);

    /* Initialize OTG clocks */
    _ConfigureUotghs();

    /* Initialize UI */
    ui_init();
    ui_powerdown();

    /* Start USB stack to authorize VBus monitoring */
    udc_start();

    if (!udc_include_vbus_monitoring()) {
        /* VBUS monitoring is not available on this product
           thereby VBUS has to be considered as present */
        main_vbus_action(true);
    }

    /* The main loop manages only the power mode
       because the USB management is done by interrupt */
    while (true) {

      #ifdef   USB_DEVICE_LOW_SPEED
        /* No USB "Keep a live" interrupt available in low speed 
           to scan mouse interface then use main loop */
        if (main_b_mouse_enable) {
            static uint16_t virtual_sof_sub = 0;
            static uint16_t virtual_sof = 0;
            if (700 == virtual_sof_sub++) {
                virtual_sof_sub = 0;
                ui_process(virtual_sof++);
            }
        }
      #endif
    }
}

void main_vbus_action(bool b_high)
{
    if (b_high) {
        /* Attach USB Device */
        udc_attach();
    } else {
        /* VBUS not present */
        udc_detach();
    }
}

void main_suspend_action(void)
{
    ui_powerdown();
}

void main_resume_action(void)
{
    ui_wakeup();
}

void main_sof_action(void)
{
    if (!main_b_mouse_enable)
        return;
    ui_process(udd_get_frame_number());
}

void main_remotewakeup_enable(void)
{
    ui_wakeup_enable();
}

void main_remotewakeup_disable(void)
{
    ui_wakeup_disable();
}

bool main_mouse_enable(void)
{
    main_b_mouse_enable = true;
    return true;
}

void main_mouse_disable(void)
{
    main_b_mouse_enable = false;
}


/**
 * \page usb_hid_mouse USB Device HID Mouse Example
 *
 * \section Purpose
 *
 * This example shows how to implement a USB Device HID Mouse
 * on ARM products with USB module.
 *
 * \section Requirements
 *
 * This package can be used with SAM3X evaluation kits.
 *
 * \section Description
 *
 * The example uses the buttons or sensors available on the board 
 * to simulate a standard mouse.
 * After loading firmware, connect hardware board (SAM3X-EK) to the USB Host.
 * When connected to a USB host system this application provides a mouse application
 * in the Unix/Mac/Windows operating systems.
 * This example uses the native HID driver for these operating systems.
 *
 * \copydoc usb_hid_mouse_ui
 *
 * \section Usage
 *
 * -# Build the program and download it inside the evaluation board. Please
 *    refer to the
 *    <a href="http://www.atmel.com/dyn/resources/prod_documents/doc6224.pdf">
 *    SAM-BA User Guide</a>, the
 *    <a href="http://www.atmel.com/dyn/resources/prod_documents/doc6310.pdf">
 *    GNU-Based Software Development</a>
 *    application note or to the
 *    <a href="ftp://ftp.iar.se/WWWfiles/arm/Guides/EWARM_UserGuide.ENU.pdf">
 *    IAR EWARM User Guide</a>,
 *    depending on your chosen solution.
 * -# On the computer, open and configure a terminal application
 *    (e.g. HyperTerminal on Microsoft Windows) with these settings:
 *   - 115200 bauds
 *   - 8 bits of data
 *   - No parity
 *   - 1 stop bit
 *   - No flow control
 * -# Start the application.
 * -# In the terminal window, the following text should appear:
 *     \code
 *     -- USB Device HID Mouse Example xxx --
 *     -- xxxxxx-xx
 *     -- Compiled: xxx xx xxxx xx:xx:xx --
 *     \endcode
 * -# When connecting USB cable to windows, the
 *    new "HID Mouse Device" appears in the
 *    hardware %device list.
 * -# Once the device is connected and configured, pressing the joystick or
 *    the configurated board buttons move the cursor.
 *
 * \section example About example
 *
 * The example uses the following module groups:
 * - Basic modules:
 *   Startup, board, clock, interrupt, power management
 * - USB Device stack and HID modules:
 *   <br>libraries/usb_device/
 *   <br>libraries/usb_device/udc/
 *   <br>libraries/usb_device/class/hid/
 *   <br>libraries/usb_device/class/hid/mouse/
 * - Specific implementation:
 *    - main.c,
 *      <br>initializes clock
 *      <br>initializes interrupt
 *      <br>\subpage power_management
 *      <br>manages UI
 *    - specific implementation:
 *       - conf_foo.h   configuration of each module
 *       - ui.c        implement of user's interface (buttons, leds)
 *
 * \section References
 * - usb_hid_mouse/main.c
 * - pio.h
 * - pio_it.h
 * - led.h
 */
