/**
 * \file
 *
 * \brief Audio sample processing functions
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
#include "sample_processor.h"

/*_____ D E F I N I T I O N S ______________________________________________*/

#define AUDIO_CFG

#define I2S_MASTER_TX_SETTING(nb_bit_by_slot, nb_slot_by_frame) (\
                               SSC_TCMR_CKS_MCK |\
                               /*SSC_TCMR_CKO_CONTINUOUS*/SSC_TCMR_CKO_TRANSFER |\
                               SSC_TCMR_START_RF_FALLING |\
                               SSC_TCMR_STTDLY(0) | \
                               SSC_TCMR_PERIOD(((nb_bit_by_slot * nb_slot_by_frame) / 2) - 1) )

#define I2S_TX_FRAME_SETTING(nb_bit_by_slot, nb_slot_by_frame) (\
                             (nb_bit_by_slot - 1) |\
                             SSC_TFMR_MSBF |\
                             SSC_TFMR_DATNB(nb_slot_by_frame - 1) |\
                             SSC_TFMR_FSLEN(nb_bit_by_slot - 1) |\
                             SSC_TFMR_FSOS_NEGATIVE)

/** Master clock frequency in Hz */
#define SSC_MCK                 BOARD_MCK

/** TWI clock */
#define TWI_CLOCK               100000

/** Wav feature. */
#define SAMPLE_RATE             (UDI_AUDIO_SAMPLE_RATE)

/** Wav slot per frame */
#define SLOT_BY_FRAME           (UDI_AUDIO_SAMPLE_CHANNELS)

/** Bits per slot */
#define BITS_BY_SLOT            (UDI_AUDIO_SAMPLE_BITS)

/*_____ L O C A L   V A R I A B L E S ______________________________________*/

/** List of pins to configure. */
static const Pin pinsSSC[] = { PINS_TWI0,
                               PINS_SSC_CODEC,
                               PIN_PCK0};

/** Twi instance*/
static Twid twid;

/*_____ L O C A L   F U N C T I O N S ______________________________________*/

/**
 * \brief  Initial the TWI interface used by codec.
 */
static void _InitTWI(void)
{
    /* Enable TWI peripheral clock */
    PMC->PMC_PCER0 = 1 << ID_TWI0;
    /* Configure and enable the TWI (required for accessing the DAC) */
    TWI_ConfigureMaster(TWI0, TWI_CLOCK, SSC_MCK);

    /* Configure twi interface to WM8731 */
    TWID_Initialize(&twid, TWI0, WM8731_SLAVE_ADDRESS, 0,0,0);
}

/**
 * \brief  Initial the ssc interface.
 */
static void _InitSSC(void)
{
    /* Configure SSC*/
    SSC_Configure(SSC, SAMPLE_RATE * BITS_BY_SLOT * SLOT_BY_FRAME, SSC_MCK);

    /* Disable transmitter first */
    SSC_DisableTransmitter(SSC);

    /* Configure the ssc transmitter */
    SSC_ConfigureTransmitter(SSC,
        I2S_MASTER_TX_SETTING(BITS_BY_SLOT, SLOT_BY_FRAME),
        I2S_TX_FRAME_SETTING (BITS_BY_SLOT, SLOT_BY_FRAME));
}

/**
 * \brief  Initial the codec for audio play.
 */
static void _InitDAC(void)
{
    /* First, disable programmable clock */
    PMC_DisablePck(PMC_SCDR_PCK0);
    /* Then, configure PMC Programmable Clock: 12MHz */
    PMC_ConfigurePck(0, PMC_MCKR_CSS_MAIN_CLK, PMC_MCKR_PRES_CLK_1);
    /* Finally, enable programmable clock */
    PMC_EnablePck(PMC_SCER_PCK0);
    /* Initialize the audio DAC */
    WM8731_DAC_Init(&twid, WM8731_SLAVE_ADDRESS);

    /* digital audio interface format (R7: default 0x0A? ) */
    /* I2S, left justified */
    WM8731_Write(&twid, WM8731_SLAVE_ADDRESS, WM8731_REG_DA_INTERFACE_FORMAT, WM8731_FORMAT_I2S|(1<<4));
    /* sampling control (R8: default 0x00) */
    /* USB mode */
    WM8731_Write(&twid, WM8731_SLAVE_ADDRESS, WM8731_REG_SAMPLECTRL, WM8731_USB_MODE);
    /* power down control (R6: default 0x9F) */
    WM8731_Write(&twid, WM8731_SLAVE_ADDRESS, WM8731_REG_PWDOWN_CTRL, 0x07);
}



/*_____ G L O B A L   F U N C T I O N S ____________________________________*/

void sample_processor_init(void)
{
    PIO_PinConfigure(pinsSSC, PIO_LISTSIZE(pinsSSC));
    /* Initial the twi interface used by DAC */
    _InitTWI();
    /* Initial the DAC on board */
    _InitDAC();
    /* Initial the ssc interface */
    _InitSSC();
    /* Disable All Interrupt */
    SSC_DisableInterrupts(SSC, 0xFFFFFFFF);
}

void sample_processor_output_samples(uint16_t *samples, uint8_t count)
{
    uint8_t i;
    SSC_EnableTransmitter(SSC);
    // Process each sample in the datastream
    for (i = 0; i < count; i += 2)
    {
        // Two channels (left and right) are encoded together to form a sample pair
        int16_t sample_left  = le16_to_cpu(samples[i]);
        int16_t sample_right = le16_to_cpu(samples[i + 1]);
  
        SSC_Write(SSC, sample_left);
        SSC_Write(SSC, sample_right);
    }
    SSC_DisableTransmitter(SSC);
}
