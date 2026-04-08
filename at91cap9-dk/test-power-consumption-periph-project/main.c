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
/// \dir "Test-power-consumption-core-project"
///
/// !!!Purpose
///
/// The Test power consumption periph project allows to measure the consumption
/// of the peripherals in static mode.
///
/// !!!See
///
/// !!!Requirements
///
/// This package can be used with all Atmel evaluation kits.
/// The package runs only at at SRAM.
///
/// !!!Description
///
/// At startup, the program configures all the PIOs as input to avoid
/// parasite consumption (except the DBGU PIOs). Then a menu is displayed.
/// It displays the list of all the peripherals and their state (ON/Off).
/// User can enable or disable a peripheral by writing the corresponding number.
/// The PCK and MCK can also be reconfigurated.
/// To measure consumption, an amperemeter has to be plugged on the board
/// instead of the VddCore jumper.
///
/// !!!Usage
///
/// -# Build the program and download it inside the evaluation board. Please
///    refer to the <a href="http://www.atmel.com/dyn/resources/prod_documents/doc6224.pdf">SAM-BA User Guide</a>,
///    the <a href="http://www.atmel.com/dyn/resources/prod_documents/doc6310.pdf">GNU-Based Software Development</a>
///    application note or to the <a href="ftp://ftp.iar.se/WWWfiles/arm/Guides/EWARM_UserGuide.ENU.pdf">IAR EWARM User Guide</a>,
///    depending on your chosen solution.
/// -# On the computer, open and configure a terminal application
///    (e.g. HyperTerminal on Microsoft Windows) with these settings:
///   - 115200 bauds
///   - 8 bits of data
///   - No parity
///   - 1 stop bit
///   - No flow control
/// -# Start the application.
/// -# Upon startup, the application will output the following lines on the DBGU 
///   (example for AT91CAP9)
///    \code
///    -- Test Periph Power Consumption Project 1.* --
///    -- AT91******-**
///    -- Compiled: *** ** **** **:**:** --
///    "The core (PCK) is running @ ***Mhz and peripherals (MCK) @ ***MHz
///    Menu:
///       02 PIO          off
///       08 US0          off
///       09 US1          off
///       10 US2          off
///       11 MCI0         off
///       12 MCI1         off
///       13 CAN          off
///       14 TWI          off
///       15 SPI0         off
///       16 SPI1         off
///       17 SSC0         off
///       18 SSC1         off
///       19 AC97C        off
///       20 TC012        off
///       21 PWMC         off
///       22 EMAC         off
///       24 ADC          off
///       25 HISI         off
///       26 LCDC         off
///       27 HDMA         off
///       28 UDPHS        off
///       29 UHP          off
///       00 -> disable all periph clocks
///       99 -> enable all periph clocks
///       ff -> Clock = PCK=200 MCK=100
///    
///    Enter 2 digits :
///    \endcode
///
//-----------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// \unit
///
/// !Purpose
///
/// This file contains all the specific code for the Test-power-consumption-periph-project.
/// It allows peripheral consumption measure in different MCK/PCK configurations.
/// 
/// !Contents
/// 
/// !See also
///
/// Please refer to the list of functions in the #Overview# tab of this unit
/// for more detailed information.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//         Headers
//------------------------------------------------------------------------------

#include <board.h>
#include <board_memories.h>
#include <pio/pio.h>
#include <pio/pio_it.h>
#include <irq/irq.h>
#include <rtt/rtt.h>
#include <pmc/pmc.h>
#include <slck/slck.h>
#include <dbgu/dbgu.h>
#include <cp15/cp15.h>
#include <utility/trace.h>
#include <utility/clock.h>

#include <stdio.h>

#if defined(__ICCARM__)
#include <intrinsics.h>
#endif

//------------------------------------------------------------------------------
//         Local definitions
//------------------------------------------------------------------------------
typedef struct{
    int  id;
    char str[6];
} t_periph;

//------------------------------------------------------------------------------
//         Internal variables
//------------------------------------------------------------------------------

/// List of button pins that must be configured for use by the application.
const Pin pPins[] = {PINS_DBGU};

#if defined(at91cap9)
t_periph list_periph[]=
{
    { AT91C_ID_PIOABCD, "PIO  " },
    { AT91C_ID_US0,     "US0  " },
    { AT91C_ID_US1,     "US1  " },
    { AT91C_ID_US2,     "US2  " },
    { AT91C_ID_MCI0,    "MCI0 " },
    { AT91C_ID_MCI1,    "MCI1 " },
    { AT91C_ID_CAN,     "CAN  " },
    { AT91C_ID_TWI,     "TWI  " },
    { AT91C_ID_SPI0,    "SPI0 " },
    { AT91C_ID_SPI1,    "SPI1 " },
    { AT91C_ID_SSC0,    "SSC0 " },
    { AT91C_ID_SSC1,    "SSC1 " },
    { AT91C_ID_AC97C,   "AC97C" },
    { AT91C_ID_TC012,   "TC012" },
    { AT91C_ID_PWMC,    "PWMC " },
    { AT91C_ID_EMAC,    "EMAC " },
    { AT91C_ID_ADC,     "ADC  " },
    { AT91C_ID_ISI,     "HISI " },
    { AT91C_ID_LCDC,    "LCDC " },
    { AT91C_ID_HDMA,    "HDMA " },
    { AT91C_ID_UDPHS,   "UDPHS" },
    { AT91C_ID_UHP ,    "UHP  " }
};
/*
#elif defined(at91sam9261)
t_periph list_periph[]=
{
    { AT91C_ID_LCDC,    "LCDC " },
    { AT91C_ID_UHP,     "UHP  " },
    { AT91C_ID_UDP,     "UDP  " },
    { AT91C_ID_SSC0,    "SSC0 " },
    { AT91C_ID_SSC1,    "SSC1 " },
    { AT91C_ID_SSC2,    "SSC2 " },
    { AT91C_ID_TC0,     "TC0  " },
    { AT91C_ID_TC1,     "TC1  " },
    { AT91C_ID_TC2,     "TC2  " },
    { AT91C_ID_TWI,     "TWI  " },
    { AT91C_ID_SPI0,    "SPI0 " },
    { AT91C_ID_SPI1,    "SPI1 " },
    { AT91C_ID_US0,     "US0  " },
    { AT91C_ID_US1,     "US1  " },
    { AT91C_ID_US2,     "US2  " },
    { AT91C_ID_MCI,     "MCI  " },
    { AT91C_ID_PIOA,    "PIOA " },
    { AT91C_ID_PIOB,    "PIOB " },
    { AT91C_ID_PIOC,    "PIOC " }
}
*/
#elif
#error Chip not yet supported
#endif

#define NBPERIPH ( sizeof(list_periph) / sizeof(t_periph))

//------------------------------------------------------------------------------
//         Internal functions
//------------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/// initialize the chip
//-----------------------------------------------------------------------------
static void InitChip(void)
{
    // Disable all the peripheral clocks
    PMC_DisableAllPeripherals();             

#if defined(at91cap9)    
    
    // use 32k slow clock instead of RC    
    printf("Select 32kHz quartz for Slow Clock. Wait several seconds...\n\r");
    SLCK_RCto32k();      
    
    // Disable UHP Port Clock (PMC_SCDR) 
    AT91C_BASE_PMC->PMC_SCDR = AT91C_PMC_UHP;
        
    // Stop PLL B (MULB=0 -> The PLL B is deactivated)
    AT91C_BASE_CKGR->CKGR_PLLBR = 0x00003f00;   
    
    // Configure all PIO as output
    // Set PIO as Output
    AT91C_BASE_PIOA->PIO_OER = 0xFFFFFFFF;
    AT91C_BASE_PIOB->PIO_OER = 0xFFFFFFFF;
    AT91C_BASE_PIOC->PIO_OER = 0xFFFFFFFF;
    AT91C_BASE_PIOD->PIO_OER = 0xFFFFFFFF;
    // Enable PIO    
    AT91C_BASE_PIOA->PIO_PER = 0xFFFFFFFF;     
    AT91C_BASE_PIOB->PIO_PER = 0xFFFFFFFF;  
    AT91C_BASE_PIOC->PIO_PER = 0xFFFFFFFF;  
    AT91C_BASE_PIOD->PIO_PER = 0xFFFFFFFF;      
    // reconfigure PIO for DBGU
    TRACE_CONFIGURE(DBGU_STANDARD, 119200, BOARD_MCK);    
    
/*    
#elif defined(at91sam9261)    

    // Disable HClocks LCD and UHP
    AT91C_BASE_PMC->PMC_SCDR = (1 << 0) << 16;    
    AT91C_BASE_PMC->PMC_SCDR = (1 << 1) << 16;   
    // Disable UDP and UHP Port Clock (PMC_SCDR) 
    AT91C_BASE_PMC->PMC_SCDR = AT91C_PMC_UDP;
    AT91C_BASE_PMC->PMC_SCDR = AT91C_PMC_UHP;    
    
#elif defined(at91sam9262)
    
    // Disable UDP, UHP and OTG Port Clock (PMC_SCDR) 
    AT91C_BASE_PMC->PMC_SCDR = AT91C_PMC_UDP;
    AT91C_BASE_PMC->PMC_SCDR = AT91C_PMC_UHP;
    AT91C_BASE_PMC->PMC_SCDR = AT91C_PMC_OTG;   
    
*/
#else
#error Chip not yet supported    
#endif

    
}

//-----------------------------------------------------------------------------
/// display menu
//-----------------------------------------------------------------------------
static void DisplayMenu(void)
{
    int i;

    printf("\n\rMenu:\n\r");
    for(i=0;i<NBPERIPH;i++)
    {
        printf("   %02d %s",list_periph[i].id, list_periph[i].str);
        if (PMC_IsPeriphEnabled(list_periph[i].id)) {
            printf("    ON\n\r");
        } else {
            printf("        off\n\r");
        }
    }
    printf("   00 -> disable all periph clocks\n\r");
    printf("   99 -> enable all periph clocks\n\r");
    printf("   ff -> Clock = PCK=%d MCK=%d\n\r", CLOCK_GetCurrPCK(), CLOCK_GetCurrMCK());
}

//-----------------------------------------------------------------------------
/// periph
//-----------------------------------------------------------------------------
static void TestPeriph(void)
{
    int i;
    int numkey;
    unsigned  char key = 0;
    unsigned char key2 = 0;

    while (1){
        DisplayMenu();      
      
        printf("\n\rEnter 2 digits : ");
        key = DBGU_GetChar();
        printf("%c", key);
        key2 = DBGU_GetChar();
        printf("%c", key2);
        printf("\n\r");
        
        numkey = (key - '0')*10 + (key2 - '0');

        if(numkey > 0 && numkey < 32) {

            // enable/disable periph clock
            for(i=0;i<NBPERIPH;i++) {
              
                if (list_periph[i].id == numkey) {
                  
                    if (PMC_IsPeriphEnabled(list_periph[i].id)) {
                      
                        PMC_DisablePeripheral(list_periph[i].id);                        
                    }
                    else {
                      
                        PMC_EnablePeripheral(list_periph[i].id);
                    }
                    break;
                }
            }
            if(i==NBPERIPH) {
              
                printf("Unknown menu !\n\r");              
            }
        }
        else if(numkey == 0) {

            PMC_DisableAllPeripherals();
        }
        else if(numkey == 99) {

            PMC_EnableAllPeripherals();
        }
        else if(key == 'f' && key2 == 'f') {

            CLOCK_UserChangeConfig();
        }
        else {

            printf("Unknown menu !\n\r");
        }
    }
}

//-----------------------------------------------------------------------------
/// main
//-----------------------------------------------------------------------------
int main(void)
{
    // Configure the DBGU
    TRACE_CONFIGURE(DBGU_STANDARD, 119200, BOARD_MCK);
    printf("-- Test Periph Power Consumption Project %s --\n\r", SOFTPACK_VERSION);
    printf("-- %s\n\r", BOARD_NAME);
    printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);  

    printf("The core (PCK) is running @ %dMhz and peripherals (MCK) @ %dMHz\n\r",
           BOARD_MCK*2/1000000,
           BOARD_MCK/1000000);

    // initialize the chip for the power consumption test
    InitChip();

    TestPeriph();

    return 0;
}

