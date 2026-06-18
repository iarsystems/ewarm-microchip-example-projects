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
/// \dir "Basic Bit Banding with AT91SAM Microcontrollers"
///
/// !!!Purpose
///
/// The basic-bitbanding project will help new users get familiar with bitbanding feature of
/// SAM3 microcontrollers.
///
/// !!!Requirements
///
/// This package can be used with all Atmel SAM3 evaluation kits.
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
/// -# In the terminal window, the following text should appear 
///     (values depend on the board and chip used):
///    \code
///     -- Basic Bit Banding Project xxx --
///     -- AT91xxxxxx-xx
///     -- Compiled: xxx xx xxxx xx:xx:xx --
///    \endcode
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// \unit
///
/// !Purpose
///
/// This file contains all the specific code for the basic-bitbanding-project.
///
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//         Headers
//------------------------------------------------------------------------------

#include <board.h>
#include <utility/trace.h>
#include <stdio.h>
#include <tc/tc.h>
#include <irq/irq.h>
#include <pmc/pmc.h>

//------------------------------------------------------------------------------
//         Local definitions
//------------------------------------------------------------------------------
#define AT91C_BITBANDING_SRAM_REGION              0x20000000
#define AT91C_BITBANDING_SRAM_ALIAS_REGION        0x22000000
#define AT91C_BITBANDING_PERIP_REGION             0x40000000
#define AT91C_BITBANDING_PERIP_ALIAS_REGION       0x42000000

#define BITBAND_TEST_SRAM_START                  AT91C_BITBANDING_SRAM_REGION // in byte
#define BITBAND_TEST_SRAM_SIZE                   0x100  // in bytes
#define BITBAND_TEST_SRAM_ALIAS_START            (AT91C_BITBANDING_SRAM_ALIAS_REGION + \
                                                     (BITBAND_TEST_SRAM_START - AT91C_BITBANDING_SRAM_REGION)*32)
#define BITBAND_TEST_SRAM_ALIAS_SIZE             (BITBAND_TEST_SRAM_SIZE * 32)  // in bytes

#define BITBAND_TEST_ITERATION                   1000

/// Marco for access memory address
#define MEM_WORD_ADDR(addr)                    *((volatile unsigned int*)(addr))
#define MEM_HWORD_ADDR(addr)                    *((volatile unsigned short*)(addr))
#define MEM_BYTE_ADDR(addr)                    *((volatile unsigned char*)(addr))
/// Marco for access memory address using bit banding feature
#define BITBAND(addr, bitnum)              ((addr & 0xf0000000) + 0x2000000 + ((addr & 0xfffff) << 5) + (bitnum << 2))

//------------------------------------------------------------------------------
//         Local variables
//------------------------------------------------------------------------------
/// clock tick count
static volatile unsigned int clockTick;

//------------------------------------------------------------------------------
//         Local functions
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/// Interrupt handler for TC0 interrupt. Toggles the state of LED\#2.
//------------------------------------------------------------------------------
void TC0_IrqHandler(void)
{
    volatile unsigned int dummy;
    // Acknowledge interrupt
    dummy = AT91C_BASE_TC0->TC_SR;

    // Increase tick
    clockTick++;
}

//------------------------------------------------------------------------------
/// Configure Timer Counter 0 to generate an interrupt every 10ms.
//------------------------------------------------------------------------------
void ConfigureTc(void)
{
    unsigned int div;
    unsigned int tcclks;

    // Enable peripheral clock
    PMC_EnablePeripheral(AT91C_ID_TC0);

    // Configure TC for a 100KHz frequency and trigger on RC compare
    TC_FindMckDivisor(100000, BOARD_MCK, &div, &tcclks);
    TC_Configure(AT91C_BASE_TC0, tcclks | AT91C_TC_CPCTRG);
    AT91C_BASE_TC0->TC_RC = (BOARD_MCK / div) / 4; // timerFreq / desiredFreq

    // Configure and enable interrupt on RC compare
    IRQ_ConfigureIT(AT91C_ID_TC0, 0x0, TC0_IrqHandler);
    AT91C_BASE_TC0->TC_IER = AT91C_TC_CPCS;
    IRQ_EnableIT(AT91C_ID_TC0);
}

//------------------------------------------------------------------------------
//         Exported functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Application entry point. 
/// \return Unused (ANSI-C compatibility).
//------------------------------------------------------------------------------
int main(void)
{
    unsigned int i, j;
    unsigned int sramAddr;
    volatile unsigned char *sram;
    volatile unsigned int *alias;
    unsigned char tmpChar, status=0;
    unsigned int tmpInt;
    unsigned int iter;

    // DBGU output configuration
    TRACE_CONFIGURE(DBGU_STANDARD, 115200, BOARD_MCK);
    printf("-- Basic Bit Banding Project %s --\n\r", SOFTPACK_VERSION);
    printf("-- %s\n\r", BOARD_NAME);
    printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);

    printf("Write SRAM alias region, read SRAM region...\n\r\n\r");

    // Clear SRAM region first
    printf("Clear SRAM region with 0x0...\n\r");
    sramAddr = BITBAND_TEST_SRAM_START;
    for(i=0; i<BITBAND_TEST_SRAM_SIZE; i++)
    {
       TRACE_DEBUG("Clear SRAM region 0x%x with 0x0...\n\r", (sramAddr+i));
       MEM_BYTE_ADDR((sramAddr+i)) = 0x0;
    }

    // Check bit-banding operation by reading SRAM alias region 
    printf("Check bit-banding operation by reading SRAM alias region...\n\r");
    sramAddr = BITBAND_TEST_SRAM_START;
    for(i=0; i<BITBAND_TEST_SRAM_SIZE; i++)
    {
        for(j=0; j<8; j++)
        {
            tmpInt = MEM_WORD_ADDR(BITBAND((sramAddr+i), j));
            TRACE_DEBUG("Memory addr 0x%x, bit %d, Bit banding addr 0x%x.\n\r",(sramAddr+i), j, BITBAND((sramAddr+i), j));
            if(tmpInt != 0)
            {
                TRACE_ERROR("Compare fail at SRAM address 0x%x, bit %d, bit banding address 0x%x.\n\r", (sramAddr+i), j, BITBAND((sramAddr+i), j));
                status = 1;
            }
        }
    }

    // Set SRAM region throught bit banding alias region
    printf("Set all the bits throuth SRAM alias region...\n\r");
    sramAddr = BITBAND_TEST_SRAM_START;
    for(i=0; i<BITBAND_TEST_SRAM_SIZE; i++)
    {
        for(j=0; j<8; j++)
        {
            TRACE_DEBUG("Memory addr 0x%x, bit %d, Bit banding addr 0x%x.\n\r",(sramAddr+i), j, BITBAND((sramAddr+i), j));
            MEM_WORD_ADDR(BITBAND((sramAddr+i), j)) = 0x1;
        }
    }

    // Check bit-banding operation by reading result from SRAM region
    printf("Check bit-banding operation by reading result from SRAM region...\n\r");
    sramAddr = BITBAND_TEST_SRAM_START;
    for(i=0; i<BITBAND_TEST_SRAM_SIZE; i++)
    {
        tmpChar = MEM_BYTE_ADDR((sramAddr+i));
        for(j=0; j<8; j++)
        {
       TRACE_DEBUG("Memory addr 0x%x, bit %d, Bit banding addr 0x%x.\n\r",(sramAddr+i), j, BITBAND((sramAddr+i), j));
            if(((tmpChar & (0x1 << j)) >> j) != 0x1)
            {
                printf("Compare fail at SRAM address 0x%x, bit %d, bit banding address 0x%x.\n\r", (sramAddr+4*i), j, BITBAND((sramAddr+4*i), j));
                status = 1;
            }
        }
    }

    if(!status)
        printf("Bit banding operation success!!!\n\r");
    else
        printf("Bit banding operation fail!!!\n\r");

    // Performance test
    printf("\n\rTest bit banding performance. \n\r");
    printf("\n\rTest iteration time %d, SRAM size 0x%x. \n\r", BITBAND_TEST_ITERATION, BITBAND_TEST_SRAM_SIZE);
    ConfigureTc();
    
    printf("Clear SRAM region with 0x0...\n\r");
    sramAddr = BITBAND_TEST_SRAM_START;
    for(i=0; i<BITBAND_TEST_SRAM_SIZE; i++)
    {
       TRACE_DEBUG("Clear SRAM region 0x%x with 0x0...\n\r", (sramAddr+i));
       MEM_BYTE_ADDR((sramAddr+i)) = 0x0;
    }

    clockTick = 0;
    

    printf("Timing: Starting write memory with bit banding timing measurement ...\n\r");
    TC_Start(AT91C_BASE_TC0);
    sramAddr = BITBAND_TEST_SRAM_START;
    iter = BITBAND_TEST_ITERATION;
    alias = (unsigned int*)BITBAND_TEST_SRAM_ALIAS_START;
    while(iter>0)
    {
      for(i=0; i<BITBAND_TEST_SRAM_ALIAS_SIZE/4; i++)
      {
        *(alias+i) = 0x1;
      }
      iter--;
    }
    TC_Stop(AT91C_BASE_TC0);
    printf("Timing: write memory with bit banding 0x%x Byte = %d us\n\r", BITBAND_TEST_SRAM_SIZE, clockTick*10);
    clockTick = 0;

    printf("Timing: Starting write memory without bit banding timing measurement ...\n\r");
    TC_Start(AT91C_BASE_TC0);
    sram = (unsigned char*)BITBAND_TEST_SRAM_START;
    iter = BITBAND_TEST_ITERATION;
    while(iter>0)
    {
      for(i=0; i<BITBAND_TEST_SRAM_SIZE; i++)
      {
          for(j=0; j<8; j++)
       	  {
              tmpChar = *(sram+i);
              tmpChar &= ~(1 << j);
              tmpChar |= (1 << j);
              *(sram+i) = tmpChar;
          }
      }
      iter--;
    }
    TC_Stop(AT91C_BASE_TC0);
    printf("Timing: write memory without bit banding 0x%x Byte = %d us\n\r", BITBAND_TEST_SRAM_SIZE, clockTick*10);
    clockTick = 0;
    return status;
}

