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
/// The Test power consumption core project allows to measure the consumption
/// of the core in different modes (active mode, slow clock mode, idle mode)
/// and it allows also the measure of the peripheral consumption.
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
/// It is divided into 2 parts. First part allows user to change the configuration
/// (all peripherals clock enable/disable, I cache enable/disable, MCK/PCK frequency
/// configuration). The second part allows user to enter in a mode (active mode,
/// slow clock mode, idle mode). To measure consumption, an amperemeter has to be plugged 
///  on the board instead of the VddCore jumper.
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
/// -# Upon startup, the application will output the following lines on the DBGU.
///    \code
///    -- Test Core Power Consumption Project 1.* --
///    -- AT91******-**
///    -- Compiled: *** ** **** **:**:** --
///    ===============================================================      
///    Menu: press a key to change the configuration or select a mode.    
///    ===============================================================   
///    Config:
///      p : All periph = [Enable|Disable]
///      c : I cache    = [Enable|Disable]    
///      f : Clock      = PCK=*** MCK=***         
///    Mode:        
///      A : Active Mode   
///      I : Idle Mode
///      S : Slow Clock Mode (500Hz)
///    --------------------------------------------------------------- 
///    \endcode
///
//-----------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// \unit
///
/// !Purpose
///
/// This file contains all the specific code for the Test-power-consumption-core-project.
/// It allows core consumption measure in different modes and in different configurations.
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
#include <dbgu/dbgu.h>
#include <cp15/cp15.h>
#include <slck/slck.h>
#include <utility/trace.h>
#include <utility/led.h>
#include <utility/clock.h>

#include <stdio.h>

#if defined(__ICCARM__)
#include <intrinsics.h>
#endif

//------------------------------------------------------------------------------
//         Local definitions
//------------------------------------------------------------------------------
#define TIMEOUT             10000000
#define TIME_500HZ_MODE     100 // several seconds to measure VddCore in 500Hz mode

//------------------------------------------------------------------------------
//         Internal variables
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//         Internal functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//         Imported functions
//------------------------------------------------------------------------------
extern void DHRY_testloop(int);

//-----------------------------------------------------------------------------
/// initialize the chip
//-----------------------------------------------------------------------------
void InitChip(void)
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
/// This function is used for Idle mode. DBGU ISR is used to go out Idle mode
//-----------------------------------------------------------------------------
void ISR_dbgu(void)
{
    volatile unsigned int csr = AT91C_BASE_DBGU->DBGU_CSR & AT91C_BASE_DBGU->DBGU_IMR;
    unsigned char key = DBGU_GetChar();
    printf("ISR_dbgu '%c'!\n\r", key);      
}

//------------------------------------------------------------------------------
/// Active Mode
//------------------------------------------------------------------------------
void SetActiveMode(void)
{
    printf("Enter in Active Mode\n\r");
    printf(" - do dhrystone test in loop\n\r"); 
    
    // Activity
    printf("Press a key to go out...\n\r");    
    while(!DBGU_IsRxReady()) {
        DHRY_testloop(1);        
    }
    
    DBGU_GetChar(); // empty RX buffer
    printf("Exit Active mode\n\r");     
}

//------------------------------------------------------------------------------
/// Idle Mode
//------------------------------------------------------------------------------
void SetIdleMode(void)
{ 
    printf("Enter in Idle Mode\n\r");    
    printf(" - One interrupt configure (dbgu).\n\r");
    printf(" - Idle state (PCK stopped), waiting an interrupt.\n\r");
       
    // Configure DBGU Interrupt to get one IT each time a character is entered
    IRQ_ConfigureIT(AT91C_ID_SYS, 0x7, ISR_dbgu);    
    // Enable IT
    IRQ_EnableIT(AT91C_ID_SYS);
    // Enable corresponding DBGU interrupt flags
    AT91C_BASE_DBGU->DBGU_IER = AT91C_US_RXRDY;
   
    // Go in idle mode
    printf("->PCK is stopped<-\n\r");    
    printf("Hit a key to exit...\n\r");    
    PMC_CPUInIdleMode();
    
    // Interrupt received
    printf("Interrupt received !\n\r");
    printf("Exit Idle mode\n\r");
    
    IRQ_DisableIT(AT91C_ID_SYS);     
}

//------------------------------------------------------------------------------
/// test Slow Clock Mode
//------------------------------------------------------------------------------
void SetSlowClockMode(void)
{
    unsigned int oldPll;
    unsigned int oldMck;  
    unsigned int time = 0;  
    unsigned int timeout = 0;  
    
    printf("Enter in Slow Clock Mode\n\r");    
    printf(" - ARM core at 500Hz\n\r");        
    printf(" - PLLs and Main Osc stopped\n\r");      
    printf(" - duration : several seconds\n\r");        
    
    // Save previous values for PLL A and Master Clock configuration
    oldPll = AT91C_BASE_CKGR->CKGR_PLLAR;
    oldMck = AT91C_BASE_PMC->PMC_MCKR;
     
    // Slow clock is selected for Master Clock 
    // 32kKz / 64 = 500Hz
    // PCK = 500Hz, MCK = 250 MHz
    AT91C_BASE_PMC->PMC_MCKR = AT91C_PMC_CSS_SLOW_CLK | AT91C_PMC_PRES_CLK_64 | AT91C_PMC_MDIV_2;
    timeout = 0;
    while ( !(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY) && timeout++ < TIMEOUT);
      
    // Stop PLL A
    // MULA: PLL A Multiplier 0 = The PLL A is deactivated.
    AT91C_BASE_CKGR->CKGR_PLLAR = 0x00003f00; 
   
    // Stop Main Oscillator
    AT91C_BASE_CKGR->CKGR_MOR = AT91C_BASE_CKGR->CKGR_MOR & (~AT91C_CKGR_MOSCEN);  
    
    // Wait a while. The clock is at 500Hz...
    time = 0;
    while( time++ < TIME_500HZ_MODE );
    // End !  
    
    // Restart Main Oscillator    
    AT91C_BASE_CKGR->CKGR_MOR = AT91C_BASE_CKGR->CKGR_MOR | (AT91C_CKGR_OSCOUNT & (0x32<<8) );
    AT91C_BASE_CKGR->CKGR_MOR = AT91C_BASE_CKGR->CKGR_MOR | (AT91C_CKGR_MOSCEN);

    // Restart PLL A
    AT91C_BASE_CKGR->CKGR_PLLAR = oldPll;        
    timeout = 0;
    while( !(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_LOCKA) && timeout++ < TIMEOUT);
    
    // Selection of Master Clock MCK (so Processor Clock PCK)
    AT91C_BASE_PMC->PMC_MCKR = oldMck;  
    timeout = 0;
    while( !(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY) && timeout++ < TIMEOUT);    
        
    // Reconfigure DBGU
    DBGU_Configure(DBGU_STANDARD, 115200, BOARD_MCK);                
    printf("Exit Slow Clock Mode\n\r");  
}

//-----------------------------------------------------------------------------
/// display test Core menu
//-----------------------------------------------------------------------------
void DisplayMenuCore(void)
{
    printf("\n\r");   
    printf("===============================================================\n\r");       
    printf("Menu: press a key to change the configuration or select a mode.\n\r");     
    printf("===============================================================\n\r");    
    printf("Config:\n\r");
    printf("  p : All periph = %s\n\r", PMC_IsAllPeriphEnabled()?"Enable":"Disable");
    printf("  c : I cache    = %s\n\r", CP15_IsIcacheEnabled()?"Enable":"Disable");    
    printf("  f : Clock      = PCK=%d MCK=%d\n\r", CLOCK_GetCurrPCK(), CLOCK_GetCurrMCK());              
    printf("Mode:\n\r");         
    printf("  A : Active Mode\n\r");    
    printf("  I : Idle Mode\n\r");
    printf("  S : Slow Clock Mode (500Hz)\n\r");
    printf("---------------------------------------------------------------\n\r");      
    printf("\n\r");    
}

//-----------------------------------------------------------------------------
/// test Core consumption
//-----------------------------------------------------------------------------
void TestCore(void)
{
    unsigned char key = 0; 
    
    while (1)
    {
        DisplayMenuCore();      
      
        key = DBGU_GetChar();
        switch(key)
        {
            // Configuration
            case 'p': 
                if(PMC_IsAllPeriphEnabled()) {
                    PMC_DisableAllPeripherals();
                } else {
                    PMC_EnableAllPeripherals();  
                }
                break;
            case 'c':
                if(CP15_IsIcacheEnabled()) {
                    CP15_DisableIcache();
                } else {
                    CP15_EnableIcache();  
                }
                break;            
            case 'f': CLOCK_UserChangeConfig();    break;          
            
            // Mode test
            case 'A': SetActiveMode();             break;            
            case 'I': SetIdleMode();               break;
            case 'S': SetSlowClockMode();          break;

            default :
                printf("This menu does not exist !\n\r");
                break;                        
        }//switch   
    }
}

//-----------------------------------------------------------------------------
/// main
//-----------------------------------------------------------------------------
int main(void)
{         
    // Configure the DBGU
    TRACE_CONFIGURE(DBGU_STANDARD, 119200, BOARD_MCK);
    printf("-- Test Core Power Consumption Project %s --\n\r", SOFTPACK_VERSION);
    printf("-- %s\n\r", BOARD_NAME);
    printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);    
    
    printf("The core (PCK) is running @ %dMhz and peripherals (MCK) @ %dMHz\n\r",
           BOARD_MCK*2/1000000,
           BOARD_MCK/1000000);
   
    // initialize the chip for the power consumption test
    InitChip();  
    
    TestCore();

    return 0;
}

