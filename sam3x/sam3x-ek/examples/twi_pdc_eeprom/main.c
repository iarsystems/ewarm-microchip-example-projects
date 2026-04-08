/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support
 * ----------------------------------------------------------------------------
 * Copyright (c) 2009, Atmel Corporation
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
 * \page twi_pdc_eeprom TWI EEPROM Example using PDC
 *
 * \section Purpose
 *
 * This basic example program demonstrates how to use the TWI peripheral
 * to access an external serial EEPROM chip. The TWI interface could be accessed
 * by DMA, PDC or normal methods.
 *
 * \section Requirements
 *
 * This package can be used with SAM3X evaluation kits.
 *
 *
 * \section Description
 *
 * The code can be roughly broken down as follows:
 * <ul>
 * <li>Configure TWI pins.</li>
 * <li>Enable TWI peripheral clock.</li>
 * <li>Configure TWI clock.</li>
 * <li>Initialize TWI as twi master.</li>
 * <li>TWI interrupt handler.</li>
 * <li>The main function, which implements the program behavior.</li>
 * <ol>
 * <li>Get the first image file 1 from the PC and store the temp data to SDRAM. </li>
 * <li>Writes the image file 1 to the eeprom. </li>
 * <li>Get the first image file 2 from the PC and store the temp data to SDRAM. </li>
 * <li>Writes the image file 2 to the eeprom. </li>
 * <li>Read the first image file 1 from the eeprom to SDRAM.</li>
 * <li>Show the image file 1 from the SDRAM to the LCD screen. </li>
 * <li>Read the first image file 2 from the eeprom to SDRAM.</li>
 * <li>Show the image file 2 from the SDRAM to the LCD screen. </li>
 * <li>Read the image file 1 again and loop step 5 to 8 forever </li>
 * </ol>
 * </ul>
 *
 * \section Usage
 *
 * -# Build the program and download it inside the evaluation board. Please
 *    refer to the
 *    <a href="http://www.atmel.com/dyn/resources/prod_documents/doc6224.pdf">
 *    SAM-BA User Guide</a>, the
 *    <a href="http://www.atmel.com/dyn/resources/prod_documents/doc6310.pdf">
 *    GNU-Based Software Development</a> application note or to the
 *    <a href="ftp://ftp.iar.se/WWWfiles/arm/Guides/EWARM_UserGuide.ENU.pdf">
 *    IAR EWARM User Guide</a>, depending on your chosen solution.
 * -# On the computer, open and configure a terminal application
 *    (e.g. HyperTerminal on Microsoft Windows) with these settings:
 *   - 115200 bauds
 *   - 8 bits of data
 *   - No parity
 *   - 1 stop bit
 *   - No flow control
 * -# Start the application.
 * -# In the terminal window, the following text should appear:
 *    \code
 *     -- TWI PDC EEPROM Example xxx --
 *     -- xxxxxx-xx
 *     -- Compiled: xxx xx xxxx xx:xx:xx --
 *    \endcode
 * -# The following traces detail operations on the EEPROM, displaying success
 *    or error messages depending on the results of the commands.
 *
 * \section References
 * - twi_pdc_eeprom/main.c
 * - twi.c
 * - twid.c
 */

/**
 * \file
 *
 * This file contains all the specific code for the twi eeprom example.
 */

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

#include "board.h"

#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>

/*----------------------------------------------------------------------------
 *        Local definitions
 *----------------------------------------------------------------------------*/

/** TWI clock frequency in Hz. */
#define TWCK            400000
/** Slave address of AT24C chips.*/
#define AT24C_ADDRESS   0x50
/** Page size of an AT24C512 chip (in bytes)*/
#define PAGE_SIZE       128
/** Width of the picture */
#define IMAGE_WIDTH  120
/** Hight of the picture */
#define IMAGE_HIGHT  120
/** Image 1 store address in SDRAM */
#define IMAGE_1_ADDR EBI_SDRAMC_ADDR
/** Image 2 store address  in SDRAM */
#define IMAGE_2_ADDR (EBI_SDRAMC_ADDR+0x40000)
/** Image 1 store address in eeprom  */
#define IMAGE_2_EEPROM_BASE_ADDR 0x8000
/*----------------------------------------------------------------------------
 *        Local variables
 *----------------------------------------------------------------------------*/
/** Pio pins to configure. */
static const Pin pins[] = {BOARD_PINS_TWI_EEPROM};

/** Buffer of image 2 */
static uint8_t *uwImage1Buff = (uint8_t*) IMAGE_1_ADDR ;

/** Buffer of image 2 */
static uint8_t *uwImage2Buff = (uint8_t*) IMAGE_2_ADDR;

/** TWI driver instance.*/
static Twid twid;

/** Number of samples which have already been transmitted.*/
static volatile uint32_t transmittedBytes;
/** Number of samples which have already been transmitted.*/
static volatile uint32_t receivedBytes;
/** Number of samples that have not yet been transmitted.*/
static volatile uint32_t remainingBytes=0;

/*----------------------------------------------------------------------------
 *        Local functions
 *----------------------------------------------------------------------------*/
/**
 *  \brief Receive the image file from the uart and store it to the eeprom
 *
 * \param ucBuff Pointer to the image buffer.
 * \param ucIndex Index of the image. 
 */
static void _ReceiveImageAndStore(uint8_t* ucBuff, uint8_t ucIndex )
{
    uint32_t dwSize;
    volatile uint32_t dw;

    printf("-I- Please send the 120*120 16bit bmp file %d using Xmodem protocol\n\r", ucIndex);

    /* Read image through xmodem protocol */
    XMODEM_ReceiveFile((int8_t *)ucBuff);
    
    printf("\n\r-I- End receive of picture %d\n\r", ucIndex);

    remainingBytes = IMAGE_WIDTH * IMAGE_HIGHT * 2;
    transmittedBytes = 0;

    while (remainingBytes > 0)
    {
        dwSize = min(remainingBytes , PAGE_SIZE);

        /* Set twi internal address */
        if (ucIndex == 0)
            twid.iaddress = transmittedBytes ;
        else
            twid.iaddress = IMAGE_2_EEPROM_BASE_ADDR + transmittedBytes;

        TWID_Write(&twid, (uint8_t*)(&ucBuff[transmittedBytes]), dwSize,0);

        /* Wait at least 10 ms, required for eeprom */
        for (dw=0; dw < 100000; dw++);

        transmittedBytes += dwSize;
        remainingBytes -= dwSize;
    }

    printf("-I- Write picture %d to eeprom done\n\r", ucIndex);
}

/*----------------------------------------------------------------------------
 *        Global functions
 *----------------------------------------------------------------------------*/

/**
 * \brief Application entry point for TWI eeprom example.
 *
 * \return Unused (ANSI-C compatibility).
 */
extern int main( void )
{
    Async async;
    uint8_t ucSwitch = 0;
    uint32_t dwSize;

    /* Disable watchdog */
    WDT_Disable( WDT ) ;

    /* Output example information */
    printf("-- TWI PDC EEPROM Example %s --\n\r", SOFTPACK_VERSION);
    printf("-- %s\n\r", BOARD_NAME);
    printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);

    /* Configure TWI pins. */
    PIO_PinConfigure(pins, PIO_LISTSIZE(pins));

    /* Enable TWI peripheral clock */
    PMC_EnablePeripheral(ID_TWI0);

    /* Configure TWI */
    TWI_ConfigureMaster(BOARD_BASE_TWI_EEPROM, TWCK, BOARD_MCK);
    TWID_Initialize(&twid, BOARD_BASE_TWI_EEPROM, AT24C_ADDRESS, 0x0, 2, \
                TWID_TRANSFER_TYPE_PDC);

    /* IRQ configure */
    NVIC_EnableIRQ(DMAC_IRQn);

    BOARD_ConfigureSdram();

    _ReceiveImageAndStore(uwImage1Buff,0);

    _ReceiveImageAndStore(uwImage2Buff,1);

    /* Initialize LCD */
    LCD_Initialize() ;
    LCD_SetBacklight(32);
    LCD_On();
    LCD_SetColor(COLOR_WHITE);
    LCD_DrawFilledRectangle(0, 0, BOARD_LCD_WIDTH-1, BOARD_LCD_HEIGHT-1);

    /* Show the image while reading the other image  and loop forever s*/
    while (1)
    {
        remainingBytes = IMAGE_WIDTH * IMAGE_HIGHT*2;
        receivedBytes = 0;

        while (remainingBytes > 0)
        {
            dwSize = min(remainingBytes , PAGE_SIZE);

            if (ucSwitch)
            {
                /* Set the twi internal address */
                twid.iaddress = receivedBytes;
                TWID_Read(&twid, (uint8_t*) (&uwImage1Buff[receivedBytes]), dwSize,0);
            }
            else
            {
                /* Set the twi internal address */
                twid.iaddress = IMAGE_2_EEPROM_BASE_ADDR + receivedBytes;
                TWID_Read(&twid, (uint8_t*) (&uwImage2Buff[receivedBytes]), dwSize,0);
            }
            receivedBytes += dwSize;
            remainingBytes -= dwSize;
        }

        /* Test LCD_DrawPicture */
        if (ucSwitch)
        {
            LCD_DrawPicture(0, 0, IMAGE_WIDTH-1, IMAGE_HIGHT-1, (const uint16_t *)uwImage1Buff );
            ucSwitch = 0;
        }
        else
        {
            LCD_DrawPicture(0, 0, IMAGE_WIDTH-1, IMAGE_HIGHT-1, (const uint16_t *)uwImage2Buff );
            ucSwitch = 1;
        }
    }
}
