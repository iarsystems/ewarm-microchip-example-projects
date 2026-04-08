/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support
 * ----------------------------------------------------------------------------
 * Copyright (c) 2010, Atmel Corporation
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


/**
 *  \page periph_protect Peripheral Protect Example
 *
 *  \section Purpose
 *
 *  The peripheral_protect example demonstrates how to prevent a program from
 *  corrupting a PIO controller behavior.
 *
 *  \section Requirements
 *
 *  This package can only be used with SAM3X-EK.
 *
 *  \section Description
 *
 *  The application shows the protective mechanism of the PIO controller.
 *  The example chooses a pin connected with a led (D2, PA12). This pin is
 *  multiplexed with the PWM0 of the peripheral PWM. In the example, the control
 *  of the pin could be switched by pressing the pushbutton 1. When the pin is
 *  controlled by PIOC, the connected LED is turned on. When the pin is used as
 *  PWM0, the connected LED is keeping flashing.
 *  The switching is implemented by writing ABCD Select Registers. This
 *  register can only be written if the WPEN bit is cleared in PIO Write
 *  Protect Mode Register. The WPEN bit can be set/clear by pressing the
 *  pushbutton 2. If the write protection is enable, switching the peripheral
 *  funtions will be unsuccessful. A warning is generated as well. The source
 *  of violation will be reported to the console by reading the register,
 *  pio write protect status register.
 *
 *  \section Usage
 *
 *  -# Build the program and download it inside the evaluation board. Please
 *     refer to the
 *     <a href="http://www.atmel.com/dyn/resources/prod_documents/doc6224.pdf">
 *     SAM-BA User Guide</a>, the
 *     <a href="http://www.atmel.com/dyn/resources/prod_documents/doc6310.pdf">
 *     GNU-Based Software Development</a>
 *     application note or to the
 *     <a href="ftp://ftp.iar.se/WWWfiles/arm/Guides/EWARM_UserGuide.ENU.pdf">
 *     IAR EWARM User Guide</a>,
 *     depending on your chosen solution.
 *  -# On the computer, open and configure a terminal application
 *     (e.g. HyperTerminal on Microsoft Windows) with these settings:
 *    - 115200 bauds
 *    - 8 bits of data
 *    - No parity
 *    - 1 stop bit
 *    - No flow control
 *  -# Start application.
 *  -# In the terminal window, the
 *     following text should appear (values depend on the board and chip used):
 *     \code
 *      -- PIO Alternate Function Example xxx --
 *      -- xxxxxx-xx
 *      -- Compiled: xxx xx xxxx xx:xx:xx --
 *     \endcode
 *  -# Perform erase operation following the application prompt.
 *
 *  \section References
 *  - periph_protect/main.c
 *  - pio.h, pio_it.h
 */

/** \file
 *
 *  This file contains all the specific code for the periph_protect.
 */

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

#include "board.h"

/* These headers were introduced in C99 by working group ISO/IEC JTC1/SC22/WG14. */
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>


/*----------------------------------------------------------------------------
 *        Local definitions
 *----------------------------------------------------------------------------*/

/** PWM frequency in Hz. */
#define PWM_FREQUENCY               10

/** IRQ priority for PIO (The lower the value, the greater the priority) */
#define IRQ_PRIOR_PIO    0

/** Pushbutton \#1 pin instance.
 * switching peripheral functions.
 */
const Pin pinPB1 = PIN_PB_LEFT_CLICK;

/** Pushbutton \#2 pin instance
 * enable/disable write protection.
 */
const Pin pinPB2 = PIN_PB_RIGHT_CLICK;

/** LED \#1 pin instance */
Pin pinLED1 = PIN_USER_LED3;

/** Pushbutton \#1 pin event flag.
 *  switching the peripheral functions
 */
volatile bool button1Evt = false;

/** Pushbutton \#2 pin event flag.
 *  enable/disable the write protection
 */
volatile bool button2Evt       = false;

/** pin control switching flag */
volatile bool blinkFlag        = true;

/** Write protect flag */
volatile bool writeProtectFlag = true;


/**
 *  \brief Handler for Button 1 (left) rising edge interrupt.
 *
 *  Set button1 event flag (button1Evt).
 */
static void _Button1_Handler( const Pin *pin )
{
    if ( pin->mask == pinPB1.mask && pin->id == pinPB1.id )
    {
        button1Evt = true;
    }
    else
    {
        button1Evt = false;
    }
}

/**
 *  \brief Handler for Button 2 (right) falling edge interrupt.
 *
 *  Set button2 event flag (button2Evt).
 */
static void _Button2_Handler( const Pin *pin )
{
    if ( pin->mask == pinPB2.mask && pin->id == pinPB2.id )
    {
        button2Evt = true;
    }
    else
    {
        button2Evt = false;
    }
}

void PWM_IrqHandler(void)
{
    /* Reading PWM_ISR automatically clears CHIDx flags. */

    /* Interrupt on channle 0 */
    if ((PWM->PWM_ISR1 & (0x01 << CHANNEL_PWM_LED1)) == (0x01 << CHANNEL_PWM_LED1))
    {
    /* dummy statement */
    }
}
/**
 *  \brief Configure the Pushbutton
 *
 *  Configure the PIO as inputs and generate corresponding interrupt when
 *  pressed or released.
 */
static void _ConfigureButton( void )
{
    /* Configure pios as inputs. */
    PIO_PinConfigure( &pinPB1, 1 );
    PIO_PinConfigure( &pinPB2, 1 );


    /* Adjust pio debounce filter parameters, uses 10 Hz filter. */
    PIO_PinSetDebounceFilter( &pinPB1, 10 );
    PIO_PinSetDebounceFilter( &pinPB2, 10 );

    /* Initialize pios interrupt handlers, see PIO definition in board.h. */
    PIO_PinConfigureIt( &pinPB1, _Button1_Handler ); /* Interrupt on rising edge  */
    PIO_PinConfigureIt( &pinPB2, _Button2_Handler ); /* Interrupt on falling edge */

    /* Enable PIO controller IRQs. */
    NVIC_EnableIRQ( (IRQn_Type)pinPB1.id ) ;
    NVIC_EnableIRQ( (IRQn_Type)pinPB2.id ) ;

    /* Enable PIO line interrupts. */
    PIO_PinEnableIt( &pinPB1 );
    PIO_PinEnableIt( &pinPB2 );
}

static void _ConfigurePWM( void )
{
     /* Enable PWMC peripheral clock */
    PMC_EnablePeripheral( ID_PWM );

    /* Set clock A to run at PWM_FREQUENCY * 50 (clock B is not used) */
    PWMC_ConfigureClocks( PWM_FREQUENCY * 50, 0, BOARD_MCK );

    /* Configure PWMC channel for LED0 (left-aligned) */
    PWMC_ConfigureChannel( PWM,CHANNEL_PWM_LED1, PWM_CMR_CPRE_CLKA, 0, 0 );

    /* Configure PWMC period and duty cycle */
    PWMC_SetPeriod( PWM,CHANNEL_PWM_LED1, 50 );
    PWMC_SetDutyCycle( PWM,CHANNEL_PWM_LED1, 10);

    /* Configure interrupt */
    NVIC_EnableIRQ(PWM_IRQn);
    PWMC_EnableChannelIt(PWM,CHANNEL_PWM_LED1);

    /* Enable channels for LEDs */
    PWMC_EnableChannel(PWM,CHANNEL_PWM_LED1);
}
/*----------------------------------------------------------------------------
 *        Exported functions
 *----------------------------------------------------------------------------*/

/**
 *  \brief peripheral protect application entry point.
 *
 *  Users can switch the pin control between PIO controller and embedded peripheral
 *  pressing left pushbutton is to switch the pin control
 *  pressing right pushbutton is to enable/disable write protect
 *  reading write protect status register to get violation information.
 *
 *  \return Unused (ANSI-C compatibility).
 *  \callgraph
 */

int main(void)
{
    /* Write protect violation information */
    uint32_t violationInfo   = 0x00;

    /* Write protect violation status */
    uint8_t  violationStatus = 0x00;

    /* Write protect violation source */
    uint32_t violationSource = 0x00;

    /* Disable watchdog */
    WDT_Disable( WDT ) ;

    /* Output example information */
    printf("-- Peripheral Protect Example %s --\n\r", SOFTPACK_VERSION);
    printf("-- %s\n\r", BOARD_NAME);
    printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);

     /* PIO configuration for LEDs and Buttons. */
    PIO_InitializeInterrupts( IRQ_PRIOR_PIO );

    /* Configure pushbuttons*/
    printf( "-I- Configure Pushbuttons.\n\r" );
    _ConfigureButton() ;

    /* Configure PWM*/
    printf( "-I- Configure PWM channel 0. \n\r" );
    _ConfigurePWM();


    printf( "Press USR-LEFT to change the blue LED D1 blinking.\n\r" ) ;
    printf( "Press USR-RIGHT to enable/disable write protection.\n\r" ) ;

    while ( 1 )
    {

      /* Pushbutton left is pressed
         * switching the peripheral functions
         */
        if ( button1Evt )
        {
            if ( blinkFlag )
            {
                /* Set pin type as a output line */
                PIO_PinSetPinType( &pinLED1, PIO_OUTPUT_0);

                /* Configure pin LED1 */
                PIO_PinConfigure( &pinLED1, 1 );

                if( writeProtectFlag )
                {
                    printf( "-I- pin is used as a general purpose I/O line.\n\r" );
                }
                else
                {
                    printf( "-I- switching is not successful! \n\r" );
                }

                blinkFlag = false;
            }
            else
            {
                /* Set pin type as controlled by peripheral PWM */
                PIO_PinSetPinType( &pinLED1, PIO_PERIPH_B);

                /* Configure pin LED1 */
                PIO_PinConfigure( &pinLED1, 1 );

                if( writeProtectFlag )
                {
                    printf( "-I- pin is controlled by on-chip peripheral PWM.\n\r" );
                }
                else
                {
                    printf( "-I- switching is not successful! \n\r" );
                }

                blinkFlag = true;
            }
            button1Evt = false;
        }

        /* pushbutton right is pressed
         * enable/disable the write protect
         */
        if ( button2Evt )
        {
            if ( writeProtectFlag )
            {
                /* Enable write protect */
                PIO_PinEnableWriteProtect( &pinLED1 );
                printf( "-I- Write protect is enabled.\n\r" );
                writeProtectFlag = false;
            }
            else
            {
                /* Disable write protect */
                PIO_PinDisableWriteProtect( &pinLED1 );
                printf( "-I- Write protect is disabled.\n\r" );
                writeProtectFlag = true;
            }

            button2Evt = false;
        }

        /* read PIO Write Protect Status Register (PIO_WPSR)
         * Reading PIO_WPSR automatically clears all fields
         */
        violationInfo   = PIO_PinGetWriteProtectViolationInfo( &pinLED1 );

        /* Get Write Protect Violation Status */
        violationStatus = violationInfo & PIO_WPSR_WPVS;

        /* Get Write Protect Violation Source through address offset */
        violationSource = violationInfo & PIO_WPSR_WPVSRC_Msk;


        if ( violationStatus )
        {
            /* Clear the violation status */
            violationStatus = 0x00;

            /* Right shift 8 bits to get address offset */
            violationSource = violationSource >> 8;


            switch ( violationSource )
            {
                case 0x00:
                   printf( "-V- A write access to PIO Enable Register.\n\r" );
                   break;

                case 0x04:
                   printf( "-V- A write access to PIO Disable Register.\n\r" );
                   break;

                default:
                   printf( "-V- Violation source is 0x%x.\n\r", (unsigned int)violationSource );
                   break;
            }
        }
    }
}

