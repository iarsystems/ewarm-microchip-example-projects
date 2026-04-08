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

//------------------------------------------------------------------------------
/// \dir "Basic ISI Project with AT91SAM Microcontrollers"
///
/// !!!Purpose
///
/// This example demonstrates the ISI (Image Sensor Interface) on AT91
/// microcontrollers.
///
/// !!!Requirements
///
/// This example can be used on AT91SAM9260-EK, AT91SAM9263-EK, AT91SAM9G20-EK,
/// AT91SAM9XE-EK.
///
/// !!!Description
///
/// The provided program uses the Image Sensor Interface to connects a CMOS-type
/// image sensor to the processor and provides image capture in various formats.
///
/// It does data conversion, if necessary, before the storage in memory 
/// through DMA. The ISI supports color CMOS image sensor and grayscale image 
/// sensors with a reduced set of functionalities.
///
/// !!!Usage
///
/// -# Connect the Omnivision captor to the ISI interface.
/// -# On AT91SAM9260-EK, replace R44 and R45 with 2,2KO.
/// -# On AT91SAM9G20-EK, replace R155 and R156 with 2,2KO.
/// -# Build the program and download it inside the evaluation board. Please
///    refer to the <a href="http://www.atmel.com/dyn/resources/prod_documents/doc6224.pdf">SAM-BA User Guide</a>,
///    the <a href="http://www.atmel.com/dyn/resources/prod_documents/doc6310.pdf">GNU-Based Software Development</a>
///    application note or to the <a href="ftp://ftp.iar.se/WWWfiles/arm/Guides/EWARM_UserGuide.ENU.pdf">IAR EWARM User Guide</a>,
///    depending on your chosen solution.
/// -# Connect a serial cable to the DEBUG port on the evaluation kit. It will
///    most often be labeled "SERIAL DEBUG port".
/// -# Connect the captor to the ISI connector:
/// Typical connection of Omnivision captor:\n
/// <table>
/// <tr><td> ISI pin  </td><td>OV9655 pin  </td><td> ISI pin  </td><td> OV9655 pin</td></tr>
/// <tr><td> 1 3V3    </td><td>DOVDD       </td><td> 14 HSYNC </td><td> HREF      </td></tr>
/// <tr><td> 2 GND    </td><td>DGND        </td><td> 15 NC    </td><td> NC        </td></tr>
/// <tr><td> 3 VddISI </td><td>AVDD(+diode)</td><td> 16 PCK   </td><td> PCLK      </td></tr> 
/// <tr><td> 4 GND    </td><td>AGND        </td><td> 17 NC    </td><td> NC        </td></tr>
/// <tr><td> 5 CTRL1  </td><td>NC          </td><td> 18 DATA0 </td><td> D2        </td></tr>
/// <tr><td> 6 CTRL2  </td><td>NC          </td><td> 19 DATA1 </td><td> D3        </td></tr>
/// <tr><td> 7 SCL    </td><td>SIO_C       </td><td> 20 DATA2 </td><td> D4        </td></tr>
/// <tr><td> 8 SDA    </td><td>SIO_D       </td><td> 21 DATA3 </td><td> D5        </td></tr>
/// <tr><td> 9 NC     </td><td>NC          </td><td> 22 DATA4 </td><td> D6        </td></tr>
/// <tr><td> 10 MCK   </td><td>XCLK        </td><td> 23 DATA5 </td><td> D7        </td></tr>
/// <tr><td> 11 NC    </td><td>NC          </td><td> 24 DATA6 </td><td> D8        </td></tr>
/// <tr><td> 12 VSYNC </td><td>VSYNC       </td><td> 25 DATA7 </td><td> D9        </td></tr>
/// <tr><td> 13 NC    </td><td>NC          </td></tr> 
/// </table>
/// For the captor: the pin RESET should be high, the pin PWND should be low.
/// The pin DVDD should be to 1,8V. You can add a regulator with the use of 
/// the pin DOVDD (3,3V).
/// Add 2 pullup to pins SIO_C and SIO_D.
/// 
/// -# On the computer, open and configure a terminal application (e.g.
///    HyperTerminal on Microsoft Windows) with these settings:
///       - 115200 bauds
///       - 8 data bits
///       - No parity
///       - 1 stop bit
///       - Hardware flow control (RTS/CTS)
/// -# Start the application. The following traces shall appear on the terminal:
///    \code
/// -- Basic ISI Project xxx --
/// -- AT91xxxxxx-xx
/// -- Compiled: xxx xx xxxx xx:xx:xx --
/// Source Size = [640,480] YUV
/// -- ====================================
/// --           TEST ISI
/// -- ====================================
/// -- 1: Sensor QVGA  [320x240]
/// -- 2: Sensor CIF   [352x288]
/// -- 3: Sensor VGA   [640x480]
/// -- 4: Sensor SXGA [1280x1024]
/// -- 5: Take a BMP photo in RGB
/// -- 6: Take a BMP photo in YCbCr
/// -- 7: end video
/// -- Enter:
///    \endcode
/// 
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// \unit
///
/// !Purpose
///
/// This file contains all the specific code for the basic-isi-project.
///
/// !Contents
/// The code can be roughly broken down as follows:
///    - Configuration functions
///    - Interrupt handlers
///    - Utility functions
///    - The main() function, which implements the program behavior
///    - Different functions are only available in debug mode, for test the LCD
///
/// !Peripherals
///    - aic
///    - dbgu
///    - isi
///    - lcd
///    - pio
///    - twi
///    - usart
///
/// !Components
///    - omnivision
///
/// Please refer to the list of functions in the #Overview# tab of this unit
/// for more detailed information.
//------------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//         Headers
//-----------------------------------------------------------------------------

#include <board.h>
#include <string.h>
#include <stdio.h>
#include <pio/pio.h>
#include <pio/pio_it.h>
#include <irq/irq.h>
#include <dbgu/dbgu.h>
#include <usart/usart.h>
#include <utility/trace.h>
#include <utility/assert.h>
#include <utility/video.h>
#include <isi/isi.h>
#include <utility/bmp.h>
#include <drivers/twi/twid.h>
#include <twi/twi.h>
#include <components/omnivision/omnivision.h>
#include <components/omnivision/ov9655/ov9655.h>

#include "2008_RGB_ref.h"

#if defined (AT91C_ID_LCDC)
#include <lcd/lcd.h>
#endif

//-----------------------------------------------------------------------------
//         Local Define
//-----------------------------------------------------------------------------

/// Number of preview buffer
#define AT91C_ISI_MAX_PREV_BUFFER    1

/// TWI clock frequency in Hz (400KHz)
#define TWCK            400000

/// List of ISI pins that must be configured for use by the application.
#define PINS_ISI BOARD_ISI_PIO_CTRL1,\
                 BOARD_ISI_PIO_CTRL2,\
                 BOARD_ISI_TWCK,\
                 BOARD_ISI_TWD,\
                 BOARD_ISI_MCK,\
                 BOARD_ISI_VSYNC,\
                 BOARD_ISI_HSYNC,\
                 BOARD_ISI_PCK,\
                 BOARD_ISI_PINS_DATA

/// TWI peripheral redefinition if needed
#if !defined(AT91C_BASE_TWI) && defined(AT91C_BASE_TWI0)
    #define AT91C_BASE_TWI      AT91C_BASE_TWI0
    #define AT91C_ID_TWI        AT91C_ID_TWI0
    #define PINS_TWI            PINS_TWI0
#endif

//-----------------------------------------------------------------------------
//         Local variables
//-----------------------------------------------------------------------------

/// Video driver instance
AT91S_VIDEO S_Video;

/// TWI driver instance.
static Twid twid;

#if defined(at91sam9m10)
#define BASE_ADRESSE_BUFFER AT91C_DDR2
#else
#define BASE_ADRESSE_BUFFER AT91C_EBI_SDRAM
#endif

/// Base adresse in SDRAM used for ISI and LCD
unsigned char *BuffCam    = (unsigned char *) (BASE_ADRESSE_BUFFER + 0x00400100);
/// Base adresse in SDRAM used for BMP header corresponding at BuffCam
unsigned char *BuffCamBMP = (unsigned char *) (BASE_ADRESSE_BUFFER + 0x00400000);
/// Base adresse in DRAM used for transformation
unsigned char *BuffTrans    = (unsigned char *) (BASE_ADRESSE_BUFFER + 0x00600100);
/// Base addresse in SDRAM used for BMP header corresponding at BuffTrans
unsigned char *BuffTransBMP = (unsigned char *) (BASE_ADRESSE_BUFFER + 0x00600000);

/// List of pins that must be configured for use by the application.
#if defined (AT91C_ID_LCDC)
static const Pin pins[] = {PINS_TWI, PINS_ISI, PINS_LCD};
#else
static const Pin pins[] = {PINS_TWI, PINS_ISI};
#endif

/// Frame Buffer Descriptors
ISI_FrameBufferDescriptors  FbList[AT91C_ISI_MAX_PREV_BUFFER+1];
ISI_Descriptors IsiDescriptors;

//------------------------------------------------------------------------------
//         Local functions
//------------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/// Frame Buffer Descriptors (FBD)
/// \param pVideo: codec vsize, codec hsize and lcd_nbpp
//-----------------------------------------------------------------------------
void AllocateFBD(AT91PS_VIDEO pVideo)
{
    unsigned int i;
    unsigned int Fb_offset;

    Fb_offset = ((pVideo->codec_vsize)*(pVideo->codec_hsize)*(pVideo->lcd_nbpp))/8;

    for(i = 0; i <= AT91C_ISI_MAX_PREV_BUFFER; i++) {
        FbList[i].Current = (unsigned int)BuffCam + (i*Fb_offset);
#if defined (BOARD_ISI_V200)
        FbList[i].Control = AT91C_ISI_P_FETCH_ENABLE;
#endif
        FbList[i].Next    = (int)&FbList[i+1];
    }
    // Wrapping to first FBD
    FbList[i-1].Next = (int)&FbList[0];

    TRACE_DEBUG("FbList[0].Current = 0x%X\n\r", FbList[0].Current);
    TRACE_DEBUG("FbList[0].Next = 0x%X\n\r", FbList[0].Next);
    TRACE_DEBUG("FbList[1].Current = 0x%X\n\r", FbList[1].Current);
    TRACE_DEBUG("FbList[1].Next = 0x%X\n\r", FbList[1].Next);
}

//-----------------------------------------------------------------------------
/// ISI interrupt handler
/// On "Fifo Preview Empty" interrupt, update LCD Frame Buffer Address
/// Generate statistics for "Fifo Codec Empty", "Fifo Codec Overflow" and
/// Fifo Preview Overflow
//-----------------------------------------------------------------------------
#if !defined (BOARD_ISI_V200)
void ISR_IsiHandler(void)
{
    unsigned int status = ISI_StatusRegister();

    //TRACE_DEBUG("I:0x%X", status);
    // Fifo Preview Empty
    if(status & AT91C_ISI_FO_P_EMP) {
        if( S_Video.IsiPrevBuffIndex < AT91C_ISI_MAX_PREV_BUFFER) {
            S_Video.lcd_fb_addr = FbList[S_Video.IsiPrevBuffIndex].Current;
            S_Video.IsiPrevBuffIndex++;
        }
        else {
            S_Video.lcd_fb_addr = FbList[0].Current;
            S_Video.IsiPrevBuffIndex = 0;
            IsiDescriptors.CurrentLcdIndex = 0;
        }
    }
    // Fifo Codec Empty
    if(status & AT91C_ISI_FO_C_EMP) {
        TRACE_DEBUG("Fifo Codec Empty\n\r");
        IsiDescriptors.DisplayCodec = 1;
    }
    // Fifo Codec Overflow
    if(status & AT91C_ISI_FO_C_OVF) {
        TRACE_DEBUG("Fifo Codec Overflow\n\r");
        IsiDescriptors.nb_codec_ovf++;
    }
    // Fifo Preview Overflow
    if(status & AT91C_ISI_FO_P_OVF) {
        TRACE_DEBUG("Fifo Preview Overflow\n\r");
        IsiDescriptors.nb_prev_ovf++;
    }
}
#else
void ISR_IsiHandler(void)
{
    unsigned int status = ISI_StatusRegister();

    //TRACE_DEBUG("I");
    //TRACE_DEBUG("I:0x%X ", status);

    // Fifo Preview Empty
    if(status & AT91C_ISI_PXFR_DONE_1) {
        if( S_Video.IsiPrevBuffIndex < AT91C_ISI_MAX_PREV_BUFFER) {
            S_Video.lcd_fb_addr = FbList[S_Video.IsiPrevBuffIndex].Current;
            S_Video.IsiPrevBuffIndex++;
        }
        else {
            S_Video.lcd_fb_addr = FbList[0].Current;
            S_Video.IsiPrevBuffIndex = 0;
            IsiDescriptors.CurrentLcdIndex = 0;
        }
    }
    // Fifo Codec Empty
    if(status & AT91C_ISI_CXFR_DONE_1) {
        TRACE_DEBUG("Fifo Codec Empty\n\r");
        IsiDescriptors.DisplayCodec = 1;
    }
    // Fifo Codec Overflow
    if(status & AT91C_ISI_C_OVR_1) {
        TRACE_DEBUG("Fifo Codec Overflow\n\r");
        IsiDescriptors.nb_codec_ovf++;
    }
    // Fifo Preview Overflow
    if(status & AT91C_ISI_P_OVR_1) {
        TRACE_DEBUG("Fifo Preview Overflow\n\r");
        IsiDescriptors.nb_prev_ovf++;
    }
    // CRC synchronization error
    if(status & AT91C_ISI_CRC_ERR_1) {
        TRACE_DEBUG("CRC synchronization error\n\r");
    }
    // Frame rate overrun
    if(status & AT91C_ISI_FR_OVR_1) {
        TRACE_DEBUG("Frame rate overrun\n\r");
    }
}
#endif

//-----------------------------------------------------------------------------
/// LCD Interrupt handler
/// Upadte the frameBase address at the end of the frame
//-----------------------------------------------------------------------------
#if defined (AT91C_ID_LCDC)
void ISR_LcdHandler(void)
{
    unsigned int status;

    //TRACE_DEBUG("I:0x%X", status);
    status = AT91C_BASE_LCDC->LCDC_ISR;
    // Update the frameBase address at the end of the frame
    if(status & AT91C_LCDC_EOFI) {    // End of Frame Interrupt 
        AT91C_BASE_LCDC->LCDC_ICR = AT91C_LCDC_EOFI;
        if(IsiDescriptors.CurrentLcdIndex != S_Video.IsiPrevBuffIndex) {

            LCD_SetFrameBufferAddress((void*)S_Video.lcd_fb_addr);

            IsiDescriptors.CurrentLcdIndex = S_Video.IsiPrevBuffIndex;
        }
    }
    else if(status & AT91C_LCDC_UFLWI) {   // (LCDC) FIFO Underflow Interrupt
        TRACE_ERROR("FIFO Underflow Interrupt\n\r");
        AT91C_BASE_LCDC->LCDC_ICR = AT91C_LCDC_UFLWI;
    }
    else if(status & AT91C_LCDC_OWRI) {    // (LCDC) Over Write Interrupt
        TRACE_ERROR("Over Write Interrupt\n\r");
        AT91C_BASE_LCDC->LCDC_ICR = AT91C_LCDC_OWRI;
    }
    else if(status & AT91C_LCDC_MERI) {    // (LCDC) Memory Error  Interrupt
        TRACE_ERROR("Memory Error  Interrupt\n\r");
        AT91C_BASE_LCDC->LCDC_ICR = AT91C_LCDC_MERI;
    }
    else {
        TRACE_ERROR("IT LCD not defined: 0x%X\n\r", status);
    }
}
#endif

//------------------------------------------------------------------------------
/// TWI interrupt handler. Forwards the interrupt to the TWI driver handler
//------------------------------------------------------------------------------
void ISR_Twi(void)
{
    TWID_Handler(&twid);
}

//------------------------------------------------------------------------------
/// Erase buffer used for LCD
//------------------------------------------------------------------------------
void EraseBufferLCD(void)
{
    #if defined (AT91C_ID_LCDC)
    unsigned int i;

    for(i=0; i<(BOARD_LCD_WIDTH*BOARD_LCD_HEIGHT*3); i++ ) {
        BuffCam[i] = 0;
    }
    #endif
}

//------------------------------------------------------------------------------
/// Initializes the LCD controller with the board parameters
//------------------------------------------------------------------------------
#if defined (AT91C_ID_LCDC)
void InitializeLcd( void )
{
    // Disable the LCD and the DMA
    TRACE_DEBUG("InitializeLcd\n\r");
    LCD_Disable(0);
    LCD_DisableDma();

    // Configure the LCD controller
    LCD_SetPixelClock(BOARD_MCK, BOARD_LCD_PIXELCLOCK);
    LCD_SetDisplayType(BOARD_LCD_DISPLAYTYPE);
    LCD_SetScanMode(AT91C_LCDC_SCANMOD_SINGLESCAN);
    LCD_SetBitsPerPixel(BOARD_LCD_BPP);
    LCD_SetPolarities(BOARD_LCD_POLARITY_INVVD,
                      BOARD_LCD_POLARITY_INVFRAME,
                      BOARD_LCD_POLARITY_INVLINE,
                      BOARD_LCD_POLARITY_INVCLK,
                      BOARD_LCD_POLARITY_INVDVAL);
    LCD_SetClockMode(BOARD_LCD_CLOCKMODE);
    LCD_SetMemoryFormat((unsigned int) AT91C_LCDC_MEMOR_LITTLEIND);
    LCD_SetSize(BOARD_LCD_WIDTH, BOARD_LCD_HEIGHT);

    // Configure timings
    LCD_SetVerticalTimings(BOARD_LCD_TIMING_VFP,
                           BOARD_LCD_TIMING_VBP,
                           BOARD_LCD_TIMING_VPW,
                           BOARD_LCD_TIMING_VHDLY);
    LCD_SetHorizontalTimings(BOARD_LCD_TIMING_HBP,
                             BOARD_LCD_TIMING_HPW,
                             BOARD_LCD_TIMING_HFP);

    // Configure contrast (TODO functions)
    LCD_SetContrastPrescaler(AT91C_LCDC_PS_NOTDIVIDED);
    LCD_SetContrastPolarity(AT91C_LCDC_POL_POSITIVEPULSE);
    LCD_SetContrastValue(0x80);
    LCD_EnableContrast();

    // Configure DMA
    LCD_SetFrameSize(BOARD_LCD_FRAMESIZE);
    LCD_SetBurstLength(4);

    EraseBufferLCD();

    // Set frame buffer
    LCD_SetFrameBufferAddress((void*)BuffCam);

    // Enable DMA and LCD
    LCD_EnableDma();
    LCD_Enable(0x0C);
}
#else
void InitializeLcd( void )
{
}
#endif


#ifdef BOARD_LCD_RGB565
//------------------------------------------------------------------------------
/// Test LCD with 240 x 320 pixels 16bpp
/// RGB 565
/// R4R3R2R1 R0G5G4G3 G2G1G0B4 B3B2B1B0
/// \param pBufferLCD pointer to image to be modified
//------------------------------------------------------------------------------
void TestLCD_RGB565_240_320_16bpp(unsigned char *pBufferLCD )
{
    unsigned int i;
    unsigned int j;

    j=0;
    while( j<(240*2*320) ) {
        // 16 bpp
        i=0;
        while(i<160) { // Red in RGB565
            pBufferLCD[j+i] = 0x00;
            i++;
            pBufferLCD[j+i] = 0xF8;
            i++;
        }
        j+=160;
        i=0;
        while(i<160) {  // Green in RGB565
            pBufferLCD[j+i] = 0x00;
            i++;
            pBufferLCD[j+i] = 0x07;
            i++;
        }
        j+=160;
        i=0;
        while(i<160) {  // Blue in RGB565
            pBufferLCD[j+i] = 0x1F;
            i++;
            pBufferLCD[j+i] = 0x00;
            i++;
        }
        j+=160;
    }

}
#else
//------------------------------------------------------------------------------
/// Test LCD with 240 x 320 pixels 16bpp
/// BGR 555
/// -B4B3B2 B1B0G4G3 G2G1G0R4 R3R2R1R0
/// \param pBufferLCD pointer to image to be modified
//------------------------------------------------------------------------------
void TestLCD_BGR555_240_320_16bpp(unsigned char *pBufferLCD )
{
    unsigned int i;
    unsigned int j;

    j=0;
    while( j<(240*2*320) ) {
        // 16 bpp
        i=0;
        while(i<160) { // Red in BGR
            pBufferLCD[j+i] = 0x1F;
            i++;
            pBufferLCD[j+i] = 0x80; // Intensity bit set to 1
            i++;
        }
        j+=160;
        i=0;
        while(i<160) {  // Green in BGR555
            pBufferLCD[j+i] = 0xE0;
            i++;
            pBufferLCD[j+i] = 0x83; // Intensity bit set to 1
            i++;
        }
        j+=160;
        i=0;
        while(i<160) {  // Blue in BGR555
            pBufferLCD[j+i] = 0x00;
            i++;
            pBufferLCD[j+i] = 0xFC; // Intensity bit set to 1
            i++;
        }
        j+=160;
    }

}
#endif

#ifdef BOARD_LCD_RGB565
//------------------------------------------------------------------------------
/// Test LCD with 240 x 320 pixels 24bpp
/// \param pBufferLCD pointer to image to be modified
//------------------------------------------------------------------------------
void TestLCD_RGB_240_320_24bpp(unsigned char *pBufferLCD )
{
    unsigned int i;
    unsigned int j;

    j=0;
    while( j<(240*3*320) ) {
        // 24 bpp
        i=0;
        while(i<240) { // Red in RGB
            pBufferLCD[j+i] = 0x00;
            i++;
            pBufferLCD[j+i] = 0x00;
            i++;
            pBufferLCD[j+i] = 0xFF;
            i++;
        }
        j+=240;
        i=0;
        while(i<240) {  // Green
            pBufferLCD[j+i] = 0x00;
            i++;
            pBufferLCD[j+i] = 0xFF;
            i++;
            pBufferLCD[j+i] = 0x00;
            i++;
        }
        j+=240;
        i=0;
        while(i<240) {  // Blue
            pBufferLCD[j+i] = 0xFF;
            i++;
            pBufferLCD[j+i] = 0x00;
            i++;
            pBufferLCD[j+i] = 0x00;
            i++;
        }
        j+=240;
    }

}
#else
//------------------------------------------------------------------------------
/// Test LCD with 240 x 320 pixels 24bpp
/// \param pBufferLCD pointer to image to be modified
//------------------------------------------------------------------------------
void TestLCD_BGR_240_320_24bpp(unsigned char *pBufferLCD )
{
    unsigned int i;
    unsigned int j;

    j=0;
    while( j<(240*3*320) ) {
        // 24 bpp
        i=0;
        while(i<240) { // Red in BGR
            pBufferLCD[j+i] = 0xFF;
            i++;
            pBufferLCD[j+i] = 0x00;
            i++;
            pBufferLCD[j+i] = 0x00;
            i++;
        }
        j+=240;
        i=0;
        while(i<240) {  // Green
            pBufferLCD[j+i] = 0x00;
            i++;
            pBufferLCD[j+i] = 0xFF;
            i++;
            pBufferLCD[j+i] = 0x00;
            i++;
        }
        j+=240;
        i=0;
        while(i<240) {  // Blue
            pBufferLCD[j+i] = 0x00;
            i++;
            pBufferLCD[j+i] = 0x00;
            i++;
            pBufferLCD[j+i] = 0xFF;
            i++;
        }
        j+=240;
    }

}
#endif

//------------------------------------------------------------------------------
/// Generic initialisation of test
/// \param pVideo video instance
//-----------------------------------------------------------------------------
void main_Test(AT91PS_VIDEO pVideo)
{
    TRACE_DEBUG("main_Test\n\r");
    S_Video.Hblank = 0;
    S_Video.Vblank = 0;
    S_Video.lcd_fb_addr = (unsigned int)BuffCam;
    // base address of the descriptor list for preview DMA
    S_Video.Isi_fbd_base = (int)&FbList[0];
    S_Video.codec_fb_addr = 0;
    S_Video.rgb_or_yuv = YUV;

    #if defined (AT91C_ID_LCDC)
    if( BOARD_LCD_WIDTH < S_Video.codec_hsize ) {
        S_Video.lcd_hsize = BOARD_LCD_WIDTH;
        S_Video.lcd_vsize = S_Video.codec_vsize*BOARD_LCD_WIDTH/S_Video.codec_hsize;
    }
    else {
        if(S_Video.codec_vsize > BOARD_LCD_HEIGHT) {
            S_Video.lcd_vsize = BOARD_LCD_HEIGHT;
        }
        else {
            S_Video.lcd_vsize = S_Video.codec_vsize;
        }                        
        S_Video.lcd_hsize = S_Video.codec_hsize*S_Video.lcd_vsize/S_Video.codec_vsize;
    }
    #endif

#if !defined (BOARD_ISI_V200)
    ISI_DisableInterrupt( AT91C_ISI_FO_P_EMP | AT91C_ISI_FO_C_EMP
                        | AT91C_ISI_FO_C_OVF | AT91C_ISI_FO_P_OVF);
    ISI_Init(&S_Video);

    ISI_EnableInterrupt( AT91C_ISI_FO_P_EMP | AT91C_ISI_FO_C_EMP
                       | AT91C_ISI_FO_C_OVF | AT91C_ISI_FO_P_OVF);
#else
    ISI_DisableInterrupt( AT91C_ISI_PXFR_DONE_1 | AT91C_ISI_CXFR_DONE_1
                        | AT91C_ISI_C_OVR_1     | AT91C_ISI_P_OVR_1
                        | AT91C_ISI_CRC_ERR_1   | AT91C_ISI_FR_OVR_1);
    ISI_Init(&S_Video);

    ISI_EnableInterrupt( AT91C_ISI_PXFR_DONE_1 | AT91C_ISI_CXFR_DONE_1
                       | AT91C_ISI_C_OVR_1     | AT91C_ISI_P_OVR_1
                       | AT91C_ISI_CRC_ERR_1   | AT91C_ISI_FR_OVR_1);
#endif

    ISI_Reset();
    ISI_Enable();

    ov965x_configure(&twid, S_Video.codec_hsize, S_Video.codec_vsize);
   // ov95x_DumpRegisters(&twid); 

#if defined (AT91C_ID_LCDC)
    InitializeLcd();

    AT91C_BASE_LCDC->LCDC_IDR = AT91C_LCDC_EOFI;

    LCD_Disable(0);
    LCD_DisableDma();
    LCD_DMAReset();
    LCD_SetBitsPerPixel(AT91C_LCDC_PIXELSIZE_SIXTEENBITSPERPIXEL);
    LCD_SetSize(S_Video.lcd_hsize, S_Video.lcd_vsize);
    LCD_SetFrameSize((S_Video.lcd_hsize * S_Video.lcd_vsize * S_Video.lcd_nbpp)/32);
    LCD_SetBurstLength(4);
    LCD_SetFrameBufferAddress(BuffCam);

    LCD_EnableDma();
    LCD_Enable(8);
    LCD_EnableInterrupts(AT91C_LCDC_EOFI  | // (LCDC) End of Frame Interrupt
                         AT91C_LCDC_UFLWI | // (LCDC) FIFO Underflow Interrupt
                         AT91C_LCDC_OWRI  | // (LCDC) Over Write Interrupt
                         AT91C_LCDC_MERI);  // (LCDC) Memory Error  Interrupt
#endif
}

//------------------------------------------------------------------------------
//         Global functions
//------------------------------------------------------------------------------
//-----------------------------------------------------------------------------
/// Application entry point. 
/// Configures the DBGU, TWI, ISI, LCD if present, display test to be launch
/// \return Unused (ANSI-C compatibility)
//-----------------------------------------------------------------------------
int main(void)
{
    unsigned char CharReceive=0;
    unsigned char command=0;
    unsigned char nbByte_Pixels=0;
    unsigned char toggle_swapcc=0;
    unsigned int  reg_cr2=0;
    unsigned char toggle_HSYNC=0;
    unsigned char toggle_VSYNC=0;
    unsigned char toggle_POL=0;
    unsigned char toggle_SYNC=0;
    unsigned char toggle_BAR=0;

    /// Erase buffer used for LCD and ISI preview
    EraseBufferLCD();

    PIO_Configure(pins, PIO_LISTSIZE(pins));
    PIO_InitializeInterrupts(0);
    TRACE_CONFIGURE(DBGU_STANDARD, 115200, BOARD_MCK);
    printf("-- Basic ISI Project %s --\n\r", SOFTPACK_VERSION);
    printf("-- %s\n\r", BOARD_NAME);
    printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);

    S_Video.codec_hsize = 640;
    S_Video.codec_vsize = 480;
    S_Video.lcd_nbpp = 16; // 16-bit format of the LCD controller.
    // Allocate the frame buffers for the preview datapath
    AllocateFBD((AT91PS_VIDEO)&S_Video);

    IsiDescriptors.CurrentLcdIndex = 0;
    IsiDescriptors.DisplayCodec = 0;
    IsiDescriptors.nb_codec_ovf = 0;
    IsiDescriptors.nb_prev_ovf = 0;

    // Configure TWI
    // In IRQ mode: to avoid problems, the priority of the TWI IRQ must be max.
    // In polling mode: try to disable all IRQs if possible.
    // (in this example it does not matter, there is only the TWI IRQ active)
    AT91C_BASE_PMC->PMC_PCER = 1 << AT91C_ID_TWI;
    AT91C_BASE_PMC->PMC_PCER = 1 << AT91C_ID_ISI;

#if defined (AT91C_ID_LCDC)
    // LCD Enable peripheral clock
    AT91C_BASE_PMC->PMC_PCER = 1 << AT91C_ID_LCDC;

#if defined(at91sam9261)
    AT91C_BASE_PMC->PMC_SCER = AT91C_PMC_HCK1;
#endif
#endif

#if defined(at91sam9263)
    // ISI_MCK is mapped in PCK3: 24Mhz Sensor clock
    AT91C_BASE_PMC->PMC_PCKR[3] = AT91C_PMC_CSS_PLLA_CLK | AT91C_PMC_PRES_CLK_8;
    AT91C_BASE_PMC->PMC_SCER = AT91C_PMC_PCK3;
    while ((AT91C_BASE_PMC->PMC_SR & AT91C_PMC_PCK3RDY) == 0);
#elif defined(at91sam9m10)
    // ISI_MCK is mapped in PCK1: 24Mhz Sensor clock
    AT91C_BASE_PMC->PMC_PCKR[1] = AT91C_PMC_CSS_PLLA_CLK | AT91C_PMC_PRES_CLK_16;
    AT91C_BASE_PMC->PMC_SCER = AT91C_PMC_PCK1;
    while ((AT91C_BASE_PMC->PMC_SR & AT91C_PMC_PCK1RDY) == 0);
#else
    // ISI_MCK is mapped in PCK1: 24Mhz Sensor clock
    AT91C_BASE_PMC->PMC_PCKR[1] = AT91C_PMC_CSS_PLLA_CLK | AT91C_PMC_PRES_CLK_8;
    AT91C_BASE_PMC->PMC_SCER = AT91C_PMC_PCK1;
    while ((AT91C_BASE_PMC->PMC_SR & AT91C_PMC_PCK1RDY) == 0);
#endif

    TWI_ConfigureMaster(AT91C_BASE_TWI, TWCK, BOARD_MCK);
    TWID_Initialize(&twid, AT91C_BASE_TWI);
    IRQ_ConfigureIT(AT91C_ID_TWI, 0, ISR_Twi);
    IRQ_EnableIT(AT91C_ID_TWI);

    S_Video.Hblank = 0;
    S_Video.Vblank = 0;
    #if defined (AT91C_ID_LCDC)
    S_Video.lcd_hsize = BOARD_LCD_WIDTH;
    S_Video.lcd_vsize = 180;
    S_Video.lcd_fb_addr = (unsigned int)BuffCam;
    #endif
    // base address of the descriptor list for preview DMA
    S_Video.Isi_fbd_base = (int)&FbList[0];
    S_Video.codec_fb_addr = 0;
    S_Video.rgb_or_yuv = YUV;
    ISI_Init(&S_Video);

    S_Video.IsiPrevBuffIndex = 0;

    IRQ_ConfigureIT(AT91C_ID_ISI, 0x7, ISR_IsiHandler);
    IRQ_EnableIT(AT91C_ID_ISI);

#if defined (AT91C_ID_LCDC)
    IRQ_ConfigureIT(AT91C_ID_LCDC, 0x7, ISR_LcdHandler);
    IRQ_EnableIT(AT91C_ID_LCDC);
#endif

    while( ov965x_init(&twid) == 0 ) {
        printf("-I- Retry init\n\r");
    }
    printf("-I- Init passed\n\r");

    nbByte_Pixels = ISI_BytesForOnePixel(S_Video.rgb_or_yuv);


#if defined (AT91C_ID_LCDC)
    InitializeLcd();
    // LCD power off
    LCD_Disable(0);
    // DMA disable
    LCD_DisableDma();
    // DMA reset
    LCD_DMAReset();
    LCD_SetBitsPerPixel(AT91C_LCDC_PIXELSIZE_SIXTEENBITSPERPIXEL);
    LCD_SetFrameSize((S_Video.lcd_hsize * S_Video.lcd_vsize * S_Video.lcd_nbpp)/32);
    LCD_SetBurstLength(4);
    LCD_SetFrameBufferAddress(BuffCam);
    LCD_EnableDma();
    LCD_Enable(8);
    LCD_EnableInterrupts(AT91C_LCDC_EOFI  | // (LCDC) End of Frame Interrupt
                         AT91C_LCDC_UFLWI | // (LCDC) FIFO Underflow Interrupt
                         AT91C_LCDC_OWRI  | // (LCDC) Over Write Interrupt
                         AT91C_LCDC_MERI);  // (LCDC) Memory Error Interrupt
#endif

    printf("Source Size = [%d,%d]", S_Video.codec_hsize, S_Video.codec_vsize);
    if ( S_Video.rgb_or_yuv == RGB) {
        printf(" RGB\n\r");
    }
    else {
        printf(" YUV\n\r");
    }
    while(1)
    {
        printf("-- ====================================\n\r");
        printf("--           TEST ISI\n\r");
        printf("-- ====================================\n\r");
        TRACE_DEBUG("IP TWI version: 0x%X\n\r", *(unsigned int*)(((unsigned int)AT91C_BASE_TWI)+0xFC));
        TRACE_DEBUG("IP ISI version: 0x%X\n\r", *(unsigned int*)(((unsigned int)AT91C_BASE_ISI)+0xFC));
        #if defined (AT91C_ID_LCDC)
        TRACE_DEBUG("IP LCD version: 0x%X\n\r", *(unsigned int*)(((unsigned int)AT91C_BASE_LCDC)+0x8FC));
        #endif
        printf("-- 1: Sensor QVGA  [320x240]\n\r");
        printf("-- 2: Sensor CIF   [352x288]\n\r");
        printf("-- 3: Sensor VGA   [640x480]\n\r");
        printf("-- 4: Sensor SXGA [1280x1024]\n\r");
        printf("-- 5: Take a BMP photo in RGB\n\r");
        printf("-- 6: Take a BMP photo in YCbCr\n\r");
        printf("-- 7: end video\n\r");
        // Other cases are only for debug purpose
        #if defined (AT91C_ID_LCDC)
        TRACE_DEBUG("-- a: Test LCD: 24bpp\n\r");
        TRACE_DEBUG("-- b: Test LCD: 16bpp, mire, with bgr correction\n\r");
        TRACE_DEBUG("-- c: Test LCD mire:\n\r");
        #endif
        TRACE_DEBUG("-- d: Debug statistics ISI\n\r");
        TRACE_DEBUG("-- e: Select Mode [0:YUV, 1:RGB]\n\r");
        TRACE_DEBUG("-- f: Enable CODEC Path\n\r");
        TRACE_DEBUG("-- g: Debug toggle YCC_SWAP: Defines the YCC image data\n\r");
        TRACE_DEBUG("-- h: Debug toggle HSYNC\n\r");
        TRACE_DEBUG("-- i: Debug toggle VSYNC\n\r");
        TRACE_DEBUG("-- j: Debug toggle PIXCLK_POL\n\r");
        TRACE_DEBUG("-- k: Debug toggle EMB_SYNC\n\r");
        TRACE_DEBUG("-- l: Debug toggle Bar Color\n\r");
        printf("-- Enter: ");
        if( !USART_IsDataAvailable((AT91S_USART *)AT91C_BASE_DBGU) );
        command = DBGU_GetChar();
        printf(".\n\r");

        switch(command) {

            case '1': // QVGA
                S_Video.codec_hsize = 320;
                S_Video.codec_vsize = 240;
                main_Test(&S_Video);
                printf("-- Sensor QVGA    [%dx%d], LCD[%dx%d]\n\r", S_Video.codec_hsize,
                    S_Video.codec_vsize, S_Video.lcd_hsize, S_Video.lcd_vsize);
            break;

            case '2': // CIF
                S_Video.codec_hsize = 352;
                S_Video.codec_vsize = 288;
                main_Test(&S_Video);
                printf("-- Sensor VGA    [%dx%d], LCD[%dx%d]\n\r", S_Video.codec_hsize,
                    S_Video.codec_vsize, S_Video.lcd_hsize, S_Video.lcd_vsize);
            break;

            case '3': // VGA
                S_Video.codec_hsize = 640;
                S_Video.codec_vsize = 480;
                main_Test(&S_Video);
                printf("-- Sensor VGA    [%dx%d], LCD[%dx%d]\n\r", S_Video.codec_hsize,
                    S_Video.codec_vsize, S_Video.lcd_hsize, S_Video.lcd_vsize);
            break;

            case '4': // SXGA
                S_Video.codec_hsize = 1280;
                S_Video.codec_vsize = 1024;
                main_Test(&S_Video);
                printf("-- Sensor SXGA    [%dx%d], LCD[%dx%d]\n\r", S_Video.codec_hsize,
                    S_Video.codec_vsize, S_Video.lcd_hsize, S_Video.lcd_vsize);
            break;

            case '5':
                printf("-- Take a BMP photo in RGB\n\r");
                ISI_Disable();
                // convert ISI captor (YCrCb) to LCD (RGB)
                VIDEO_Ycc2Rgb((unsigned char *)BuffCam,
                              (unsigned short *)BuffTrans,
                              (S_Video.codec_hsize*S_Video.codec_vsize*S_Video.lcd_nbpp)/8);
                S_Video.rgb_or_yuv = RGB;
                WriteBMPheader((unsigned int*)BuffTransBMP,
                               S_Video.lcd_hsize,
                               S_Video.lcd_vsize,
                               S_Video.rgb_or_yuv,
                               nbByte_Pixels);
                #if defined (AT91C_ID_LCDC)
                LCD_Disable(0);
                LCD_DisableDma();
                LCD_DMAReset();
                LCD_SetSize(S_Video.lcd_hsize, S_Video.lcd_vsize);
                LCD_SetFrameSize((S_Video.lcd_hsize * S_Video.lcd_vsize * S_Video.lcd_nbpp)/32);
                LCD_SetFrameBufferAddress((void*)BuffTrans);
                LCD_EnableDma();
                LCD_Enable(8);
                #endif
                printf("BMP image at address 0x%8X, length:%d\n\r",
                                            (unsigned int)BuffTransBMP,
                                            (S_Video.lcd_hsize * S_Video.lcd_vsize * S_Video.lcd_nbpp)/32);
            break;

            case '6':
                printf("-- Take a BMP photo in YCbCr\n\r");
                ISI_Disable();
                S_Video.rgb_or_yuv = YUV;
                WriteBMPheader((unsigned int*)BuffCamBMP,
                               S_Video.lcd_hsize,
                               S_Video.lcd_vsize,
                               S_Video.rgb_or_yuv,
                               nbByte_Pixels);
                #if defined (AT91C_ID_LCDC)
                LCD_Disable(0);
                LCD_DisableDma();
                LCD_DMAReset();
                LCD_SetSize(S_Video.lcd_hsize, S_Video.lcd_vsize);
                LCD_SetFrameSize((S_Video.lcd_hsize * S_Video.lcd_vsize * S_Video.lcd_nbpp)/32);
                LCD_SetFrameBufferAddress((void*)BuffCam);
                LCD_EnableDma();
                LCD_Enable(8);
                #endif
                printf("BMP image at address 0x%8X, length:%d\n\r",
                                            (unsigned int)BuffCamBMP,
                                            (S_Video.lcd_hsize * S_Video.lcd_vsize * S_Video.lcd_nbpp)/32);
            break;

            case '7':
                printf("-- end video\n\r");
                #if !defined (BOARD_ISI_V200)
                    ISI_DisableInterrupt(AT91C_ISI_FO_P_EMP | AT91C_ISI_FO_C_EMP
                                       | AT91C_ISI_FO_C_OVF | AT91C_ISI_FO_P_OVF);
                #else
                    ISI_DisableInterrupt( AT91C_ISI_PXFR_DONE_1 | AT91C_ISI_CXFR_DONE_1
                                        | AT91C_ISI_C_OVR_1     | AT91C_ISI_P_OVR_1
                                        | AT91C_ISI_CRC_ERR_1   | AT91C_ISI_FR_OVR_1);
                #endif

                EraseBufferLCD();
                #if defined (AT91C_ID_LCDC)
                LCD_SetFrameBufferAddress((void*)BuffCam);
                #endif
                ISI_Reset();
            break;


            //------------------------------------------------------------------
            // Other cases are only for debug purpose
            //------------------------------------------------------------------
            #if defined (AT91C_ID_LCDC)
            case 97:  // 'a'
                printf("-- Test LCD: 24bpp\n\r");
                InitializeLcd();
                // LCD power off
                LCD_Disable(0);
                // DMA disable
                LCD_DisableDma();
                // DMA reset
                LCD_DMAReset();

                EraseBufferLCD();
                LCD_SetFrameSize((BOARD_LCD_WIDTH * BOARD_LCD_HEIGHT * 24)/32);
                LCD_SetFrameBufferAddress((void*)BuffCam);

                // DMA enable.
                LCD_EnableDma();
                // LCD power on
                LCD_Enable(8);

                #ifdef BOARD_LCD_RGB565
                    TestLCD_RGB_240_320_24bpp(BuffCam);
                #else
                    TestLCD_BGR_240_320_24bpp(BuffCam);
                #endif
            break;

            case 98: // 'b'
                printf("-- Test LCD: 16bpp, mire, with bgr correction\n\r");
                InitializeLcd();
                // LCD power off
                LCD_Disable(0);
                // DMA disable
                LCD_DisableDma();
                // DMA reset
                LCD_DMAReset();

                EraseBufferLCD();
                LCD_SetBitsPerPixel(AT91C_LCDC_PIXELSIZE_SIXTEENBITSPERPIXEL);
                LCD_SetFrameSize((BOARD_LCD_WIDTH * BOARD_LCD_HEIGHT * 16)/32);
                LCD_SetFrameBufferAddress((void*)BuffCam);

                // DMA enable.
                LCD_EnableDma();
                // LCD power on
                LCD_Enable(8);

                #ifdef BOARD_LCD_RGB565
                    TestLCD_RGB565_240_320_16bpp(BuffCam);
                #else
                    TestLCD_BGR555_240_320_16bpp(BuffCam);
                #endif
            break;

            case 99: // 'c'
                printf("-- Test LCD mire:\n\r");
                InitializeLcd();
                // LCD power off
                LCD_Disable(0);
                // DMA disable
                LCD_DisableDma();
                // DMA reset
                LCD_DMAReset();

                EraseBufferLCD();

                LCD_SetBitsPerPixel(AT91C_LCDC_PIXELSIZE_SIXTEENBITSPERPIXEL);
                LCD_SetFrameSize((BOARD_LCD_WIDTH * BOARD_LCD_HEIGHT * 16)/32);
                LCD_SetBurstLength(4);

                // this image should be see with the adaptator on the lcd
                LCD_DecodeRGB( (unsigned char*)image_2008,
                               (unsigned char*)BuffCam,
                               320,
                               240,
                               16);
                LCD_SetFrameBufferAddress((void*)BuffCam);

                // DMA enable.
                LCD_EnableDma();
                // LCD power on
                LCD_Enable(8);
            break;
            #endif

            case 100: // 'd'
                printf("-- statistics ISI\n\r");
                printf("IsiDescriptors.DisplayCodec = %d\n\r", IsiDescriptors.DisplayCodec);
                printf("IsiDescriptors.Fifo Codec Overflow   = %d\n\r", IsiDescriptors.nb_codec_ovf);
                printf("IsiDescriptors.Fifo Preview Overflow = %d\n\r", IsiDescriptors.nb_prev_ovf);
                printf("S_Video.codec_hsize: %d\n\r", S_Video.codec_hsize);
                printf("S_Video.codec_vsize: %d\n\r", S_Video.codec_vsize);
                printf("S_Video.lcd_hsize: %d\n\r", S_Video.lcd_hsize);
                printf("S_Video.lcd_vsize: %d\n\r", S_Video.lcd_vsize);

                #if !defined (BOARD_ISI_V200)
                    printf("ISI_CR1 0x%X\n\r", AT91C_BASE_ISI->ISI_CR1);
                    printf("ISI_CR2 0x%X\n\r", AT91C_BASE_ISI->ISI_CR2);
                    printf("ISI_SR  0x%X\n\r", AT91C_BASE_ISI->ISI_SR);
                    printf("ISI_PPFBD 0x%X\n\r", AT91C_BASE_ISI->ISI_PPFBD);
                    printf("ISI_PDECF %d\n\r", AT91C_BASE_ISI->ISI_PDECF);
                    printf("ISI_PSIZE 0x%X\n\r", AT91C_BASE_ISI->ISI_PSIZE);
                    printf("ISI_Y2RSET0 0x%X\n\r", AT91C_BASE_ISI->ISI_Y2RSET0);
                    printf("ISI_Y2RSET1 0x%X\n\r", AT91C_BASE_ISI->ISI_Y2RSET1);
                    printf("ISI_R2YSET0 0x%X\n\r", AT91C_BASE_ISI->ISI_R2YSET0);
                    printf("ISI_R2YSET1 0x%X\n\r", AT91C_BASE_ISI->ISI_R2YSET1);
                    printf("ISI_R2YSET2 0x%X\n\r", AT91C_BASE_ISI->ISI_R2YSET2);
                #else
                    printf("ISI_CFG1 0x%X\n\r", AT91C_BASE_ISI->ISI_CFG1);
                    printf("ISI_CFG2 0x%X\n\r", AT91C_BASE_ISI->ISI_CFG2);
                    printf("ISI_PSIZE 0x%X\n\r", AT91C_BASE_ISI->ISI_PSIZE);
                    printf("ISI_PDECF %d\n\r", AT91C_BASE_ISI->ISI_PDECF);
                    printf("ISI_Y2RSET0 0x%X\n\r", AT91C_BASE_ISI->ISI_Y2RSET0);
                    printf("ISI_Y2RSET1 0x%X\n\r", AT91C_BASE_ISI->ISI_Y2RSET1);
                    printf("ISI_R2YSET0 0x%X\n\r", AT91C_BASE_ISI->ISI_R2YSET0);
                    printf("ISI_R2YSET1 0x%X\n\r", AT91C_BASE_ISI->ISI_R2YSET1);
                    printf("ISI_R2YSET2 0x%X\n\r", AT91C_BASE_ISI->ISI_R2YSET2);
                    printf("ISI_CTRL 0x%X\n\r", AT91C_BASE_ISI->ISI_CTRL);
                    printf("ISI_SR   0x%X\n\r", AT91C_BASE_ISI->ISI_SR);
                    printf("ISI_IER  0x%X\n\r", AT91C_BASE_ISI->ISI_IER);
                    printf("ISI_IDR  0x%X\n\r", AT91C_BASE_ISI->ISI_IDR);
                    printf("ISI_IMR  0x%X\n\r", AT91C_BASE_ISI->ISI_IMR);

                    printf("ISI_DMACHER  0x%X\n\r", AT91C_BASE_ISI->ISI_DMACHER);
                    printf("ISI_DMACHDR  0x%X\n\r", AT91C_BASE_ISI->ISI_DMACHDR);
                    printf("ISI_DMACHSR  0x%X\n\r", AT91C_BASE_ISI->ISI_DMACHSR);
                    printf("ISI_DMAPADDR  0x%X\n\r", AT91C_BASE_ISI->ISI_DMAPADDR);
                    printf("ISI_DMAPCTRL  0x%X\n\r", AT91C_BASE_ISI->ISI_DMAPCTRL);
                    printf("ISI_DMAPDSCR  0x%X\n\r", AT91C_BASE_ISI->ISI_DMAPDSCR);
                    printf("ISI_DMACADDR  0x%X\n\r", AT91C_BASE_ISI->ISI_DMACADDR);
                    printf("ISI_DMACCTRL  0x%X\n\r", AT91C_BASE_ISI->ISI_DMACCTRL);
                    printf("ISI_DMACDSCR  0x%X\n\r", AT91C_BASE_ISI->ISI_DMACDSCR);

                    printf("ISI_WPCR  0x%X\n\r", AT91C_BASE_ISI->ISI_WPCR);
                    printf("ISI_WPSR  0x%X\n\r", AT91C_BASE_ISI->ISI_WPSR);
                #endif

                #if defined (AT91C_ID_LCDC)
                printf("LCDC_BA1   0x%X\n\r", AT91C_BASE_LCDC->LCDC_BA1);
                printf("LCDC_BA2   0x%X\n\r", AT91C_BASE_LCDC->LCDC_BA2);
                printf("LCDC_FRMP1 0x%X\n\r", AT91C_BASE_LCDC->LCDC_FRMP1);
                printf("LCDC_FRMP2 0x%X\n\r", AT91C_BASE_LCDC->LCDC_FRMP2);
                printf("LCDC_FRMA1 0x%X\n\r", AT91C_BASE_LCDC->LCDC_FRMA1);
                printf("LCDC_FRMA2 0x%X\n\r", AT91C_BASE_LCDC->LCDC_FRMA2);
                printf("LCDC_FRMCFG 0x%X\n\r", AT91C_BASE_LCDC->LCDC_FRMCFG);
                printf("LCDC_DMACON 0x%X\n\r", AT91C_BASE_LCDC->LCDC_DMACON);
                printf("LCDC_DMA2DCFG 0x%X\n\r", AT91C_BASE_LCDC->LCDC_DMA2DCFG);
                printf("LCDC_LCDCON1 0x%X\n\r", AT91C_BASE_LCDC->LCDC_LCDCON1&0x001FF001);
                printf("LCDC_LCDCON2 0x%X\n\r", AT91C_BASE_LCDC->LCDC_LCDCON2);
                printf("LCDC_TIM1 0x%X\n\r", AT91C_BASE_LCDC->LCDC_TIM1);
                printf("LCDC_TIM2 0x%X\n\r", AT91C_BASE_LCDC->LCDC_TIM2);
                printf("LCDC_LCDFRCFG 0x%X\n\r", AT91C_BASE_LCDC->LCDC_LCDFRCFG);
                printf("LCDC_MVAL     0x%X\n\r", AT91C_BASE_LCDC->LCDC_MVAL);
                printf("LCDC_PWRCON   0x%X\n\r", AT91C_BASE_LCDC->LCDC_PWRCON);
                printf("LCDC_CTRSTCON 0x%X\n\r", AT91C_BASE_LCDC->LCDC_CTRSTCON);
                printf("LCDC_CTRSTVAL 0x%X\n\r", AT91C_BASE_LCDC->LCDC_CTRSTVAL);
                printf("LCDC_ISR 0x%X\n\r", AT91C_BASE_LCDC->LCDC_ISR);
                printf("LCDC_GPR 0x%X\n\r", AT91C_BASE_LCDC->LCDC_GPR);
                #endif
           break;

            case 101: // 'e'
                printf("-- Select Mode [0:YUV, 1:RGB]: ");
                if ( !USART_IsDataAvailable((AT91S_USART *)AT91C_BASE_DBGU) ) {}
                CharReceive = DBGU_GetChar();
                printf("%c\n\r", CharReceive);
                if( ( CharReceive == 0x30 ) || ( CharReceive == 0x31 ) ) {
                    #if !defined (BOARD_ISI_V200)
                        ISI_DisableInterrupt(AT91C_ISI_FO_P_EMP);
                    #else
                        ISI_DisableInterrupt(AT91C_ISI_PXFR_DONE_1);
                    #endif
                    ISI_Reset();
                    S_Video.rgb_or_yuv = CharReceive-0x30;
                    if( S_Video.rgb_or_yuv == RGB ) {
                        // Values for RGB565
                        // 0x12 0x03
                        ov965x_write_reg(&twid, 0x12, 0x03);
                        // 0x3A 0xC0
                        ov965x_write_reg(&twid, 0x3A, 0xC0);
                        // 0x40 0xd0
                        ov965x_write_reg(&twid, 0x40, 0xD0);
                        // 0xC7 0x80
                        ov965x_write_reg(&twid, 0xC7, 0x80);
                    }
                    else {
                        // 0x12 0x62
                        ov965x_write_reg(&twid, 0x12, 0x62);
                        // 0x3a 0x80
                        ov965x_write_reg(&twid, 0x3A, 0x80);
                        // 0x40 0xc0
                        ov965x_write_reg(&twid, 0x40, 0xC0);
                        // 0xc7 0x80
                        ov965x_write_reg(&twid, 0xC7, 0x80);
                    }
                    ISI_Init(&S_Video);
                    #if !defined (BOARD_ISI_V200)
                        ISI_EnableInterrupt(AT91C_ISI_FO_P_EMP | AT91C_ISI_FO_P_OVF);
                    #else
                        ISI_DisableInterrupt(AT91C_ISI_PXFR_DONE_1 | AT91C_ISI_P_OVR_1);
                    #endif
                    ISI_Reset();
                    ISI_Enable();
                }
                else {
                    TRACE_ERROR("Mode not supported\n\r");
                    printf("-- Press a key\n\r");
                    if ( !USART_IsDataAvailable((AT91S_USART *)AT91C_BASE_DBGU) ) {}
                }
            break;

            case 102: // 'f'
                printf("-- Enable CODEC Path\n\r");
                ISI_Reset();
                ISI_CodecPathFull();
                ISI_Enable();
            break;

            case 103: // 'g'
                printf("-- toggle swapp CC\n\r");
                #if !defined (BOARD_ISI_V200)
                    reg_cr2 = AT91C_BASE_ISI->ISI_CR2;
                #else
                    reg_cr2 = AT91C_BASE_ISI->ISI_CFG2;
                #endif
                reg_cr2 &= ~AT91C_ISI_YCC_SWAP;
                if(toggle_swapcc == 0) {
                    printf("AT91C_ISI_YCC_SWAP_YCC_DEFAULT\n\r");
                    toggle_swapcc = 1;
                }
                else if(toggle_swapcc == 1) {
                    printf("AT91C_ISI_YCC_SWAP_YCC_MODE1\n\r");
                    reg_cr2 |= AT91C_ISI_YCC_SWAP_YCC_MODE1;
                    toggle_swapcc = 2;
                }
                else if(toggle_swapcc == 2) {
                    printf("AT91C_ISI_YCC_SWAP_YCC_MODE2\n\r");
                    reg_cr2 |= AT91C_ISI_YCC_SWAP_YCC_MODE2;
                    toggle_swapcc = 3;
                }
                else {
                    printf("AT91C_ISI_YCC_SWAP_YCC_MODE3\n\r");
                    reg_cr2 |= AT91C_ISI_YCC_SWAP_YCC_MODE3;
                    toggle_swapcc = 0;
                }
                #if !defined (BOARD_ISI_V200)
                    AT91C_BASE_ISI->ISI_CR2 = reg_cr2;
                #else
                    AT91C_BASE_ISI->ISI_CFG2 = reg_cr2;
                #endif
            break;

            case 104: // 'h'
                printf("-- toggle HSYNC\n\r");
                if(toggle_HSYNC == 0) {
                    printf("HSYNC active high\n\r");
                    #if !defined (BOARD_ISI_V200)
                        AT91C_BASE_ISI->ISI_CR1 &= ~AT91C_ISI_HSYNC_POL;
                    #else
                        AT91C_BASE_ISI->ISI_CFG1 &= ~AT91C_ISI_HSYNC_POL;
                    #endif
                    toggle_HSYNC = 1;
                }
                else if(toggle_HSYNC == 1) {
                    printf("HSYNC active low\n\r");
                    #if !defined (BOARD_ISI_V200)
                        AT91C_BASE_ISI->ISI_CR1 |= AT91C_ISI_HSYNC_POL;
                    #else
                        AT91C_BASE_ISI->ISI_CFG1 |= AT91C_ISI_HSYNC_POL;
                    #endif
                    toggle_HSYNC = 0;
                }
            break;

            case 105: // 'i'
                printf("-- toggle VSYNC\n\r");
                if(toggle_VSYNC == 0) {
                    printf("VSYNC active high\n\r");
                    #if !defined (BOARD_ISI_V200)
                        AT91C_BASE_ISI->ISI_CR1 &= ~AT91C_ISI_VSYNC_POL;
                    #else
                        AT91C_BASE_ISI->ISI_CFG1 &= ~AT91C_ISI_VSYNC_POL;
                    #endif
                    toggle_VSYNC = 1;
                }
                else if(toggle_VSYNC == 1) {
                    printf("VSYNC active low\n\r");
                    #if !defined (BOARD_ISI_V200)
                        AT91C_BASE_ISI->ISI_CR1 |= AT91C_ISI_VSYNC_POL;
                    #else
                        AT91C_BASE_ISI->ISI_CFG1 |= AT91C_ISI_VSYNC_POL;
                    #endif
                    toggle_VSYNC = 0;
                }
            break;

            case 106: // 'j'
                printf("-- toggle PIXCLK_POL\n\r");
                if(toggle_POL == 0) {
                    printf("PIXCLK rising edge\n\r");
                    #if !defined (BOARD_ISI_V200)
                        AT91C_BASE_ISI->ISI_CR1 &= ~AT91C_ISI_PIXCLK_POL;
                    #else
                        AT91C_BASE_ISI->ISI_CFG1 &= ~AT91C_ISI_PIXCLK_POL;
                    #endif
                    toggle_POL = 1;
                }
                else if(toggle_POL == 1) {
                    printf("PIXCLK falling edge\n\r");
                    #if !defined (BOARD_ISI_V200)
                        AT91C_BASE_ISI->ISI_CR1 |= AT91C_ISI_PIXCLK_POL;
                    #else
                        AT91C_BASE_ISI->ISI_CFG1 |= AT91C_ISI_PIXCLK_POL;
                    #endif
                    toggle_POL = 0;
                }
            break;

            case 107: // 'k'
                printf("-- toggle EMB_SYNC\n\r");
                if(toggle_SYNC == 0) {
                    printf("Synchronization by HSYNC, VSYNC\n\r");
                    #if !defined (BOARD_ISI_V200)
                        AT91C_BASE_ISI->ISI_CR1 &= ~AT91C_ISI_EMB_SYNC;
                    #else
                        AT91C_BASE_ISI->ISI_CFG1 &= ~AT91C_ISI_EMB_SYNC;
                    #endif
                    toggle_SYNC = 1;
                }
                else if(toggle_SYNC == 1) {
                    printf("Synchronization by embedded synchronization sequence SAV/EAV\n\r");
                    #if !defined (BOARD_ISI_V200)
                        AT91C_BASE_ISI->ISI_CR1 |= AT91C_ISI_EMB_SYNC;
                    #else
                        AT91C_BASE_ISI->ISI_CFG1 |= AT91C_ISI_EMB_SYNC;
                    #endif
                    toggle_SYNC = 0;
                }
            break;

            case 108: // 'l'
                printf("-- Debug Toggle Bar Color\n\r");
                if(toggle_BAR == 0) {
                    printf("Set Bar Color\n\r");
                    // Set register 8D bit 4 high (10hex)
                    ov965x_write_reg(&twid, 0x8D, 0x10);
                    toggle_BAR = 1;
                }
                else if(toggle_BAR == 1) {
                    printf("Remove Bar color\n\r");
                    ov965x_write_reg(&twid, 0x8D, 0x00);
                    toggle_BAR = 0;
                }
            break;

            default:
                printf("-%c ", command);
                printf("-%d\n\r", command);
                command = 0;
            break;
        }
    }
}

