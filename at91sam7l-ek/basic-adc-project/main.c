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
/// \dir "basic adc project"
///
/// !!!Purpose
///
/// This project demonstrates the ADC (Analog-to-Digital Converter).If Evaluation
/// board has ADC12Bit, it will use ADC12Bit, otherwise ADC10bit.
///
/// !!!Requirements
///
/// The following evaluation kits are supported by this project:
/// - AT91SAM7A3-EK
/// - AT91SAM7L-EK
/// - AT91SAM7S-EK
/// - AT91SAM7SE-EK
/// - AT91SAM7X-EK
/// - AT91SAM7XC-EK
/// - AT91SAM9260-EK
/// - AT91SAM9G20-EK
/// - AT91SAM9XE-EK
/// - AT91CAP9-DK
/// - AT91CAP9-STK
/// - AT91SAM3U-EK
///
/// !!!Description
///
/// After launching the program, the device will display the voltage on four
/// of his ADC pins. Somes boards have more than four accessible pins, so the 
/// software can be easily update for that. BoardS like AT91SAM3U-EK has only 
/// one ADC12 and ADC10 pin connected to the potentimeter, so if you adjust the
/// potentimeter, in this example, only one selected PIN of ADC12Bit will change.
///
///
/// !!!Usage
/// -# For AT91SAM7A3-EK board:
/// The measured pins are J6 pins 1 and 2, and J9 pins B15 and B16
///
/// -# For AT91SAM7L-EK board:
/// The measured pins are J205 pins 1, 3, 5 and 7
///
/// -# For AT91SAM7S-EK board:
/// The measured pins are J7, J8, J9 and J10
/// 
/// -# For AT91SAM7SE-EK board:
/// The measured pins are J17 pins 29, 30, 31 and 32
///
/// -# For AT91SAM7X-EK board:
/// The measured pins are J16 pins C29 and C30, and J26 pins 1 and 2
/// 
/// -# For AT91SAM7XC-EK board:
/// The measured pins are J16 pins C29 and C30, and J26 pins 1 and 2
///
/// -# For AT91SAM9260-EK board:
/// The measured pins are J24 pins 1, 2, 3 and 4
///
/// -# For AT91SAM9G20-EK board:
/// The measured pins are J24 pins 1, 2, 3 and 4
///
/// -# For AT91SAM9XE-EK board:
/// The measured pins are J24 pins 1, 2, 3 and 4
///
/// -# For AT91CAP9-DK board:
/// The ADC jumpers are on position 1 (J3)
///
/// -# For AT91CAP9-STK board:
/// The ADC jumpers are on position 3 (J53-3, J54-3, J55-3, J56-3)
///
/// -# For AT91SAM3U-EK board:
/// ADC represents ADC10bit, ADC12B represents ADC12bit.
/// The ADC VREF jumper is set on right side '+3V3' of JP20(ADC12B) or JP5(ADC)
/// for default 3.3v, left side 'VREF0/VREF1' for 2.5v. J14(PB4 pin) and BNC(CN1) are 
/// connected to ADC12B_AD3 channel of ADC12BIT if R82 is connected; J14(PB5 pin) and 
/// BNC(CN2) are connected to ADC_AD1 channel of ADC10BIT if R84 is connected. 
/// The potentimeter acts on AD3 channel of ADC12B, AD1 channel of ADC.
/// ADC12B_AD2,ADC12B_AD6, ADC12B_AD7 are connected to the the accelerometer, and they don't
/// response to the adjustment of the potentimeter.
/// 
/// -# Compile the application.
/// -# Connect the DBGU port of the evaluation board to the computer and open
/// it in a terminal.
///    - Settings: 115200 bauds, 8 bits, 1 stop bit, no parity, no flow control.
/// -# Download the program inside the evaluation board and run it. Please refer to
///    <a href="http://www.atmel.com/dyn/resources/prod_documents/doc6132.pdf">
///    the SAM-BA User Guide,
///    <a href="http://www.atmel.com/dyn/resources/prod_documents/doc6310.pdf">
///    the GNU-Based Software Development</a> application note or to the
///    <a href="http://www.iar.se/website1/1.0.1.0/78/1/index.php?">
///    IAR EWARM User Guide</a>,
///    depending on your chosen solution.
/// -# Upon startup, the application will output the following line on the DBGU:
/// -# 12 bit adc menu and information only shown if ek board has the 12 bit ADC
///    \code
///    -- Basic ADC Project xxx --
///    -- AT91xxxxxx-xx
///    -- Compiled: xxx xx xxxx xx:xx:xx --
///	xxxxxxxxxxxxxxxxxxx ADC 12 bit configuration Menu: xxxxxxxxxx
///	g--- config 12 bit ADC Gain
///	c--- Config 12 bit ADC bias Current
///	d--- Config 12 bit ADC Differential Mode
///	o--- Config 12 bit ADC offset
///	m--- Show this menu!
///
///	Current setting:[GAIN: x], [IBCTL: x], [DIFF: x], [OFFSET: x]
///
///	Select to config 12 bit ADC analog control!
///	-I- Press any key to perform a measurement on the ADC ...
///    \endcode
/// -# And then, after press a key:
///    \code
///    Channel 2 : 3125 mV
///    Channel 3 : 245 mV
///    Channel 6 : 3149 mV
///    Channel 7 : 3132 mV
///    \endcode
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
//         Headers
//------------------------------------------------------------------------------

#include <board.h>
#include <pio/pio.h>
#include <dbgu/dbgu.h>
#include <irq/irq.h>
#include <utility/trace.h>

#if defined(at91sam3u4)
#include <adc/adc12.h>
#else
#include <adc/adc.h>
#endif

#include <stdio.h>

//------------------------------------------------------------------------------
//         Local definitions
//------------------------------------------------------------------------------

#define BOARD_ADC_FREQ 5000000
#define ADC_VREF       3300  // 3.3 * 1000

typedef struct _GainMap {
  unsigned int diffmode;
  char *gainstring[4];
} GainMap;

//------------------------------------------------------------------------------
//         Local variables
//------------------------------------------------------------------------------

/// Pio pins to configure.
#ifdef PINS_ADC
static const Pin pinsADC[] = {PINS_ADC};
#endif

#if defined(at91sam7a3)
#define AT91C_ID_ADC AT91C_ID_ADC0
#define AT91C_BASE_ADC AT91C_BASE_ADC0
#endif

//remap SAM3U4 adc 10 bit to be compatible with definition name of others
#if defined(at91sam3u4)
#if defined(AT91C_ID_ADC)
#undef AT91C_ID_ADC
#endif
#define AT91C_ID_ADC AT91C_ID_ADC12B
#define AT91C_BASE_ADC AT91C_BASE_ADC12B
#endif


/// Indicates that the conversion is finished.
static volatile unsigned char conversionDone;

#if defined(at91sam7s16) || defined(at91sam7s161) || defined(at91sam7s32)\
    || defined(at91sam7s321) || defined(at91sam7s64) || defined(at91sam7s128)\
    || defined(at91sam7s256) || defined(at91sam7s512)
#define ADC_NUM_1  ADC_CHANNEL_0
#define ADC_NUM_2  ADC_CHANNEL_1
#define ADC_NUM_3  ADC_CHANNEL_4
#define ADC_NUM_4  ADC_CHANNEL_5
#endif
#if defined(at91sam7x128) || defined(at91sam7x256) || defined(at91sam7x512)\
   || defined(at91sam7xc128) || defined(at91sam7xc256) || defined(at91sam7xc512)
#define ADC_NUM_1  ADC_CHANNEL_4
#define ADC_NUM_2  ADC_CHANNEL_5
#define ADC_NUM_3  ADC_CHANNEL_6
#define ADC_NUM_4  ADC_CHANNEL_7
#endif
#if defined(at91sam7a3)\
    || defined(at91sam7l64) || defined(at91sam7l128)\
    || defined(at91sam9260)\
    || defined(at91sam9g20)\
    || defined(at91sam9rl64)\
    || defined(at91sam9xe128) || defined(at91sam9xe256) || defined(at91sam9xe512) \
    || defined(at91cap9)
#define ADC_NUM_1  ADC_CHANNEL_0
#define ADC_NUM_2  ADC_CHANNEL_1
#define ADC_NUM_3  ADC_CHANNEL_2
#define ADC_NUM_4  ADC_CHANNEL_3
#endif

#if defined(at91sam7se32) || defined(at91sam7se256) ||defined(at91sam7se512)
#define ADC_NUM_1  ADC_CHANNEL_4
#define ADC_NUM_2  ADC_CHANNEL_5
#define ADC_NUM_3  ADC_CHANNEL_6
#define ADC_NUM_4  ADC_CHANNEL_7
#endif

#if defined(at91sam3u4)
#define ADC_NUM_1  ADC12_CHANNEL_2
#define ADC_NUM_2  ADC12_CHANNEL_3
#define ADC_NUM_3  ADC12_CHANNEL_6
#define ADC_NUM_4  ADC12_CHANNEL_7
#endif

static unsigned int chns[] = {ADC_NUM_1, ADC_NUM_2, ADC_NUM_3, ADC_NUM_4};
//------------------------------------------------------------------------------
//         Local functions
//------------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/// Convert a digital value in milivolt
/// \param valueToconvert Value to convert in milivolt
//-----------------------------------------------------------------------------
static unsigned int ConvHex2mV( unsigned int valueToConvert )
{
    unsigned int mask;

//for ADC10Bit, example use 10bit only; for ADC12Bit, use 12bit only
#if defined(at91sam3u4)
    mask = 0xFFF;
#else
    mask = 0x3FF;
#endif
    
    return( (ADC_VREF * valueToConvert)/mask);
}

//------------------------------------------------------------------------------
/// Interrupt handler for the ADC. Signals that the conversion is finished by
/// setting a flag variable.
//------------------------------------------------------------------------------
void ADCC0_IrqHandler(void)
{
    unsigned int status, i;

#if defined(at91sam3u4)
    status = ADC12_GetStatus(AT91C_BASE_ADC);
#else
    status = ADC_GetStatus(AT91C_BASE_ADC);
#endif
    
    TRACE_DEBUG("status =0x%X\n\r", status);
    //TRACE_DEBUG("adc_imr=0x%X\n\r", ADC_GetInterruptMaskStatus());

    for(i=0;i<4;i++) {
      
#if defined(at91sam3u4)
        if (ADC12_IsChannelInterruptStatusSet(status, chns[i])) {
#else
        if (ADC_IsChannelInterruptStatusSet(status, chns[i])) {
#endif

            TRACE_DEBUG("channel %d\n\r", chns[i]);
            
#if defined(at91sam3u4)
            ADC12_DisableIt(AT91C_BASE_ADC, 1<<chns[i]);//disable EOCx interrupt
#else
            ADC_DisableIt(AT91C_BASE_ADC, 1<<chns[i]);//disable EOCx interrupt
#endif
            
            conversionDone |= 1<<chns[i];
        }
    }
}

#if defined(at91sam3u4)
static void ShowADC12ConfigMenu()
{
    unsigned int adc_acr;
    unsigned int gain, currentctrl, diffmode, offset;
    
    GainMap gainmap[2] = {
      {0, "1", "1", "2", "4"},
      {1, "0.5", "1", "2", "2"}
    };
    
    char *inputmode[2] =  {
      "Single Ended",
      "Full Differential"
    };

    adc_acr = ADC12_GetAnalogCtrlReg(AT91C_BASE_ADC);

    gain = adc_acr & 0x3;
    currentctrl = (adc_acr &0xc0)>>6;
    diffmode = (adc_acr & 0x10000)>>16;
    offset = (adc_acr & 0x20000)>>17;

    printf("\n\r ************ ADC 12 bit configuration Menu: ***********");
    printf("\n\r g--- config 12 bit ADC Gain");
    printf("\n\r c--- Config 12 bit ADC bias Current");
    printf("\n\r d--- Config 12 bit ADC Differential Mode");
    printf("\n\r o--- Config 12 bit ADC Offset");
    printf("\n\r m--- Show this menu!\n\r");
    printf("\n\r Current setting:[GAIN:%u, gain=%s], [IBCTL: %u], [DIFF: %u, %s], [OFFSET: %u]\n\r", \
      gain, gainmap[diffmode].gainstring[gain], currentctrl, diffmode, inputmode[diffmode],offset);
    printf("\n\r Select to config 12 bit ADC analog control!");
    printf("\n\r Or any other key to perform a measurement on the ADC!\n\r");
}
#endif


//------------------------------------------------------------------------------
//         Global functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Performs measurements on ADC channel 0 and displays the result on the DBGU.
//------------------------------------------------------------------------------
int main(void)
{
    unsigned int id_channel, advalue;	
    char key;

    TRACE_CONFIGURE(DBGU_STANDARD, 115200, BOARD_MCK);
    printf("-- Basic ADC Project %s --\n\r", SOFTPACK_VERSION);
    printf("-- %s\n\r", BOARD_NAME);
    printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);

#ifdef PINS_ADC
    PIO_Configure(pinsADC, PIO_LISTSIZE(pinsADC));
#endif

#if defined(at91sam3u4)
    /////
    //AT91C_BASE_ADC,mapped to AT91C_BASE_ADC12B
    //AT91C_ID_ADC,mapped to AT91C_ID_ADC12B
    /////
    
    ADC12_Initialize( AT91C_BASE_ADC,//mapped to AT91C_BASE_ADC12B
                    AT91C_ID_ADC,//mapped to AT91C_ID_ADC12B
                    AT91C_ADC12B_MR_TRGEN_DIS,
                    0,
                    AT91C_ADC12B_MR_SLEEP_NORMAL,
                    AT91C_ADC12B_MR_LOWRES_12_BIT,
                    BOARD_MCK,
                    BOARD_ADC_FREQ,
                    10,
                    1200);

    ADC12_EnableChannel(AT91C_BASE_ADC, ADC_NUM_1);
    ADC12_EnableChannel(AT91C_BASE_ADC, ADC_NUM_2);
    ADC12_EnableChannel(AT91C_BASE_ADC, ADC_NUM_3);
    ADC12_EnableChannel(AT91C_BASE_ADC, ADC_NUM_4);
#else
    ADC_Initialize( AT91C_BASE_ADC,
                    AT91C_ID_ADC,
                    AT91C_ADC_TRGEN_DIS,
                    0,
                    AT91C_ADC_SLEEP_NORMAL_MODE,
                    AT91C_ADC_LOWRES_10_BIT,
                    BOARD_MCK,
                    BOARD_ADC_FREQ,
                    10,
                    1200);

    ADC_EnableChannel(AT91C_BASE_ADC, ADC_NUM_1);
    ADC_EnableChannel(AT91C_BASE_ADC, ADC_NUM_2);
    ADC_EnableChannel(AT91C_BASE_ADC, ADC_NUM_3);
    ADC_EnableChannel(AT91C_BASE_ADC, ADC_NUM_4);
#endif

    IRQ_ConfigureIT(AT91C_ID_ADC, 0, ADCC0_IrqHandler);
    IRQ_EnableIT(AT91C_ID_ADC);

#if defined(at91sam3u4)
    ShowADC12ConfigMenu();	
#else
    printf("-I- Press any key to perform a measurement on the ADC ...\n\r");
#endif


    // Infinite loop
    while (1) {

        // Wait for user input
        key = DBGU_GetChar();

#if defined(at91sam3u4)
        //set analog control register
        //analog control register format: 
        // <31...18><OFFSET:17><DIFF:16><15...8><IBCTL:7...6><5...2><GAIN:1...0>
        
	//set Gain
	if(key == 'g' || key == 'G') {
		printf("\n\r select Gain option to set GAIN<0:1>:"); 
		printf("\n\r 1--- 00b [Single Ended Mode:gain=1, Full Differential Mode:gain=0.5]");
		printf("\n\r 2--- 01b [Single Ended Mode:gain=1, Full Differential Mode:gain=1]");
		printf("\n\r 3--- 10b [Single Ended Mode:gain=2, Full Differential Mode:gain=2]");
		printf("\n\r 4--- 11b [Single Ended Mode:gain=4, Full Differential Mode:gain=2]");
                printf("\n\r Note: No differential inputs for ADC12Bit on Sam3u-ek by default.");

		key = DBGU_GetChar();

		unsigned int gain = 0xff;
		switch(key) {
			case '1':
				gain = 0;
				break;

			case '2':
				gain = 0x1;
				break;

			case '3':
				gain = 0x2;
				break;

			case '4':
				gain = 0x3;				
				break;

			default:
				printf("\n\r Wrong selection!\n\r");
				break;
		}

		if(gain!=0xff) {
			//Change GAIN field of ADC_ACR of ADC 12 bit
			gain |= (((unsigned int)ADC12_GetAnalogCtrlReg(AT91C_BASE_ADC)) & 0x300c0);
			ADC12_CfgAnalogCtrlReg(AT91C_BASE_ADC, gain);
		}

		ShowADC12ConfigMenu();
		continue;
	}

	//set bias current control
	if(key == 'c' || key == 'C') {
		printf("\n\r select Current option to set IBCTL<0:1>:");
		printf("\n\r 1--- 00b [typ - 20%%]");
		printf("\n\r 2--- 01b [typ]");
		printf("\n\r 3--- 10b [typ + 20%%]");
		printf("\n\r 4--- 11b [typ + 40%%]");

		key = DBGU_GetChar();

		unsigned int cur = 0xff;
		switch(key) {
			case '1':
				cur = 0;
				break;

			case '2':
				cur = 0x40;
				break;

			case '3':
				cur = 0x80;
				break;

			case '4':
				cur = 0xC0;
				break;

			default:
				printf("\n\r Wrong selection!\n\r");
				break;
		}

		if(cur!=0xff) {
			//Change GAIN field of ADC_ACR of ADC 12 bit
			cur |= (((unsigned int)ADC12_GetAnalogCtrlReg(AT91C_BASE_ADC)) & 0x30003);
			ADC12_CfgAnalogCtrlReg(AT91C_BASE_ADC, cur);
		}

		ShowADC12ConfigMenu();
		continue;
	}
	
	//set Differential Mode
	//note: in SAM3UE-EK REV 1, only single end is connected. No available input on EK board for Fully Differential Mode 
	if(key == 'd' || key == 'D') {
		printf("\n\r select Differential mode to set DIFF bit field:");
                printf("\n\r 1--- DIFF:0b Single Ended Mode");
                printf("\n\r 2--- DIFF:1b Fully Differential Mode");
                printf("\n\r NOTE: No differential input on AT91SAM3U-EK, ");
                printf(" Select it may output wrong result\n\r");

		key = DBGU_GetChar();

		unsigned int diff = 0xff;
		switch(key) {
			case '1':
				diff = 0;
				break;

			case '2':
				diff = 0x10000;
				break;

			default:
				printf("\n\r Wrong selection!\n\r");
				break;
		}

		if(diff!=0xff) {
			//Change GAIN field of ADC_ACR of ADC 12 bit
			diff |= (((unsigned int)ADC12_GetAnalogCtrlReg(AT91C_BASE_ADC)) & 0x200c3);
			ADC12_CfgAnalogCtrlReg(AT91C_BASE_ADC, diff);
		}

		ShowADC12ConfigMenu();
		continue;
	}
        
        //set offset
        if(key == 'o' || key == 'O') {
                printf("\n\r Select input OFFSET to set OFFSET bit field:");
                printf("\n\r 1--- 0 [DIFF:0, Vrefin/4][DIFF:1, Vrefin/2]");
                printf("\n\r 2--- 1 [Vrefin/2]");
                printf("\n\r");
                
                key = DBGU_GetChar();
                
		unsigned int offset = 0xff;
		switch(key) {
			case '1':
				offset = 0;
				break;

			case '2':
				offset = 0x20000;
				break;

			default:
				printf("\n\r Wrong selection!\n\r");
				break;
		}

		if(offset!=0xff) {
			//Change GAIN field of ADC_ACR of ADC 12 bit
			offset |= (((unsigned int)ADC12_GetAnalogCtrlReg(AT91C_BASE_ADC)) & 0x100c3);
			ADC12_CfgAnalogCtrlReg(AT91C_BASE_ADC, offset);
		}

		ShowADC12ConfigMenu();
		continue;
        }
        
        //show menu
        if(key == 'm' || key == 'M') {
                ShowADC12ConfigMenu();
                continue;
        }
          
#endif

        conversionDone = 0;

#if defined(at91sam3u4)
        ADC12_EnableIt(AT91C_BASE_ADC, 1<<ADC_NUM_1);
        ADC12_EnableIt(AT91C_BASE_ADC, 1<<ADC_NUM_2);
        ADC12_EnableIt(AT91C_BASE_ADC, 1<<ADC_NUM_3);
        ADC12_EnableIt(AT91C_BASE_ADC, 1<<ADC_NUM_4);
        
        // Start measurement
        ADC12_StartConversion(AT91C_BASE_ADC);
#else
        ADC_EnableIt(AT91C_BASE_ADC, 1<<ADC_NUM_1);
        ADC_EnableIt(AT91C_BASE_ADC, 1<<ADC_NUM_2);
        ADC_EnableIt(AT91C_BASE_ADC, 1<<ADC_NUM_3);
        ADC_EnableIt(AT91C_BASE_ADC, 1<<ADC_NUM_4);

        // Start measurement
        ADC_StartConversion(AT91C_BASE_ADC);
#endif

        while( conversionDone != ((1<<ADC_NUM_1)|(1<<ADC_NUM_2)|(1<<ADC_NUM_3)|(1<<ADC_NUM_4)) );

        for(id_channel=0;id_channel<4;id_channel++) {

#if defined(at91sam3u4)
          advalue = ADC12_GetConvertedData(AT91C_BASE_ADC, chns[id_channel]);
#else
          advalue = ADC_GetConvertedData(AT91C_BASE_ADC, chns[id_channel]);
#endif
            printf("Channel %u : %u mV\n\r", chns[id_channel],ConvHex2mV(advalue));
        }

        printf("\n\r");
    }
}

