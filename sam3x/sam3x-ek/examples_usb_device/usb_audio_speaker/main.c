/**
 * \file
 *
 * \brief Main functions for Audio example
 *
 * Copyright (C) 2011 Atmel Corporation. All rights reserved.
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
#include "udd.h"
#include "udc.h"
#include "udi_audio.h"
#include "ui.h"
#include "sample_processor.h"

/*_____ D E F I N I T I O N S ______________________________________________*/

#define STRING_EOL    "\r"
#define STRING_HEADER "-- USB Device Audio Speaker Example "SOFTPACK_VERSION" --\r\n" \
                      "-- "BOARD_NAME" --\r\n" \
                      "-- Compiled: "__DATE__" "__TIME__" --"STRING_EOL

/*_____ V A R I A B L E S __________________________________________________*/

/** PIOs for USB OTGHS */
static const Pin pUOTGHSPins[] = {PIN_UOTGHS_VBOF, PIN_UOTGHS_ID, PIN_UOTGHS_FAULT};
static const Pin pUOTGHS_Fault = PIN_UOTGHS_FAULT;

static volatile bool main_b_audio_enable = false;

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
    /* Set 4 WS for Embedded Flash Access for 84 MHz */
    EFC0->EEFC_FMR = EEFC_FMR_FWS( 4 );
    EFC1->EEFC_FMR = EEFC_FMR_FWS( 4 );

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
    
    // Initialize the sample processor module ready to receive samples
    sample_processor_init();

    // Start USB stack to authorize VBus monitoring
    udc_start();

    if (!udc_include_vbus_monitoring()) {
        // VBUS monitoring is not available on this product
        // thereby VBUS has to be considered as present
        main_vbus_action(true);
    }

    // The main loop manages only the power mode
    // because the USB management is done by interrupt
    while (true) {
        if (main_b_audio_enable
        && udi_audio_stream_get_feature_cur(AUDIO_FEATURE_MUTE) == false) {
            udi_audio_process();
        }
    }
}

void main_vbus_action(bool b_high)
{
    if (b_high) {
        // Attach USB Device
        udc_attach();
    } else {
        // VBUS not present
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
    if (!main_b_audio_enable)
        return;
    ui_process(udd_get_frame_number());
}

bool main_audio_enable(void)
{
    main_b_audio_enable = true;
    ui_stream_enabled();
    return true;
}

void main_audio_disable(void)
{
    main_b_audio_enable = false;
    ui_stream_disabled();
}

void main_audio_out_process(uint8_t *data, uint16_t count)
{
    // Samples are encoded as 16-bit signed values in the datastream
    uint16_t *samples = (uint16_t*)data;
    
    // Since each sample is 16-bit, need to halve length to get sample count
    uint8_t   sample_count = count / 2;

    // Hand off received samples to the board sample processor
    sample_processor_output_samples(samples, sample_count); 
}

/**
 * \page usb_audio_speaker USB Device Audio Speaker Example
 *
 * \section Purpose
 * This example shows how to implement a USB Device Audio 1.0
 * on ARM products with USB module.
 *
 * \section Requirements
 *
 * This package can be used with SAM3X evaluation kits.
 *
 * \section Description
 *
 * The example uses the speaker and amplifier available on the board.
 * After loading firmware, connect hardware board to the USB Host.
 * When connected to a USB host system this application provides an audio output
 * device in the Unix/Mac/Windows operating systems.
 *
 * \note
 * This example uses the native Audio driver on Unix/Mac/Windows OS.
 *
 * \copydoc usb_hid_generic_ui
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
 *     -- USB Device Audio Speaker Example xxx --
 *     -- xxxxxx-xx
 *     -- Compiled: xxx xx xxxx xx:xx:xx --
 *     \endcode
 *  -# When connecting USB cable to windowss, the host
 *     reports a new USB device attachment (if it's the first time you connect
 *     an audio speaker demo board to your host). You can find new
 *     "USB Composite Device" and "USB Audio Device" appear in the hardware
 *     device list.
 *  -# You can play sound in host side through the USB Audio Device (Audio 1.0
 *     Device), and it can be heard from the earphone connected to the EK.
 *
 * \section example About example
 *
 * The example uses the following module groups:
 * - Basic modules:
 *   Startup, board, clock, interrupt, power management
 * - USB Device stack and Audio modules:
 *   <br>libraries/usb_device/
 *   <br>libraries/usb_device/udc/
 *   <br>libraries/usb_device/class/audio/
 * - Specific implementation:
 *    - main.c,
 *      <br>initializes clock
 *      <br>initializes interrupt
 *      <br>\subpage power_management
 *      <br>manages UI
 *    - specific implementation:
 *       - conf_foo.h   configuration of each module
 *       - ui.c        implement of user's interface (leds)
 *
 * \section References
 * - usb_audio_speaker/main.c
 * - pio.h
 * - pio_it.h
 * - led.h
 */
