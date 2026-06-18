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
 * \page sdramc SDRAMC Example
 *
 * \section Purpose
 *
 * This example shows how to configure the SDRAMC,
 * it will do write and read operations to check the configuration.
 *
 * \section Requirements
 *
 * This package can be used with SAM3X evaluation kits.
 *
 * \section Description
 *
 * An accurate one-to-one comparison is processed after the complete SDRAM configuration.
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
 *     -- SDRAMC Example --
 *     -- xxxxxx-xx
 *     -- Compiled: xxx xx xxxx xx:xx:xx --
 *
 *    \endcode
 *
 * \section References
 * - sdramc/main.c
 */

/**
 * \file
 *
 * This file contains all the specific code for the sdramc example.
 */

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

#include "board.h"

/* These headers were introduced in C99 by working group ISO/IEC JTC1/SC22/WG14. */
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>

/*----------------------------------------------------------------------------
 *        Local Definitions
 *----------------------------------------------------------------------------*/
/** Sdram benchmark test size */
#define TEST_BUFF_SIZE  (30*1024)

/*----------------------------------------------------------------------------
 *        Local Variables
 *----------------------------------------------------------------------------*/
/** SDRAM benchmark buffer */
static uint16_t pMemBuff[TEST_BUFF_SIZE];

/*----------------------------------------------------------------------------
 *        Local functions
 *----------------------------------------------------------------------------*/

/**
 * \brief Systick handler to manage timestamping and delay
 */
void SysTick_Handler( void )
{
    TimeTick_Increment() ;
}

/**
 * \brief Get Delayed number of tick
 * \param startTick Start tick point.
 * \param endTick   End tick point.
 */
static uint32_t _GetDelayInTicks(uint32_t dwStartTick, uint32_t dwEndTick)
{
    if (dwEndTick > dwStartTick) return (dwEndTick - dwStartTick);
    return (dwEndTick + (0xFFFFFFFF - dwStartTick));
}

/**
 * \brief Go/No-Go test of the first 10K-Bytes of external SDRAM access.
   \return 0 if test is failed else 1.
 */
static uint8_t Sdram_AccessTest( void )
{
    uint32_t i;
    uint32_t *pdw = (uint32_t*) EBI_SDRAMC_ADDR ;

    for ( i=0 ; i < 10 * 1024 ; ++i )
    {
        if ( i & 1 )
        {
            pdw[i] = 0x55AA55AA | (1 << i) ;
        }
        else
        {
            pdw[i] = 0xAA55AA55 | (1 << i) ;
        }
    }

    for ( i=0 ; i < 10 * 1024 ; ++i )
    {
        if ( i & 1 )
        {
            if ( pdw[i] != (0x55AA55AA | (1 << i)) )
            {
                return 0 ;
            }
        }
        else
        {
            if ( pdw[i] != (0xAA55AA55 | (1 << i)) )
            {
                return 0 ;
            }
        }
    }

    return 1 ;
}

/**
 * \brief Sdram benchmark test.
 * \return 1 for success and 0 for error.
 */
static uint32_t Sdram_Benchmarks( void )
{
    uint32_t dwCount ;
    uint16_t *pdw = (uint16_t*) EBI_SDRAMC_ADDR ;
    uint32_t dwTickStart, dwTickEnd, dwTicks, dwRWSpeed;

    printf( "-I- Start SDRAMC benchmarks \n\r" ) ;

    for (dwCount = 0; dwCount < TEST_BUFF_SIZE; dwCount ++)
    {
        pMemBuff[dwCount] = (uint16_t)(dwCount&0xffff) ;
    }

    /* Start SDRAM write test */
    dwTickStart = GetTickCount() ;
    for (dwCount = 0; dwCount < TEST_BUFF_SIZE; dwCount ++)
    {
        pdw[dwCount] = pMemBuff[dwCount] ;
    }
    dwTickEnd = GetTickCount();
    dwTicks = _GetDelayInTicks(dwTickStart, dwTickEnd);
    dwRWSpeed = TEST_BUFF_SIZE * sizeof(uint16_t) / dwTicks;
    printf("-I- SDRAMC write speed: %uK/s\n\r", (uint32_t)dwRWSpeed);

    /* Start SDRAM read test */
    dwTickStart = GetTickCount() ;
    for (dwCount = 0; dwCount < TEST_BUFF_SIZE; dwCount ++)
    {
        pMemBuff[dwCount] = pdw[dwCount];

    }
    dwTickEnd = GetTickCount();
    dwTicks = _GetDelayInTicks(dwTickStart, dwTickEnd);
    dwRWSpeed = TEST_BUFF_SIZE * sizeof(uint16_t) / dwTicks;
    printf("-I- SDRAMC read speed: %uK/s\n\r", (uint32_t)dwRWSpeed);

    for (dwCount = 0; dwCount < TEST_BUFF_SIZE; dwCount ++)
    {
        if ( pMemBuff[dwCount] != (dwCount&0xffff))
        {
            printf("    SDRAMC read data error\n\r");
            return 0;
        }
    }

    return 1 ;
}

/*----------------------------------------------------------------------------
 *         Global functions
 *----------------------------------------------------------------------------*/

/**
 * \brief Application entry point for smc_sdramc example.
 *
 * \return Unused (ANSI-C compatibility).
 */
int main( void )
{
    /* Disable watchdog */
    WDT_Disable( WDT ) ;

    /* Output example information */
    printf( "-- SDRAMC Example %s --\n\r", SOFTPACK_VERSION ) ;
    printf( "-- %s\n\r", BOARD_NAME ) ;
    printf( "-- Compiled: %s %s --\n\r", __DATE__, __TIME__ ) ;

    TimeTick_Configure( BOARD_MCK ) ;

    /* complete SDRAM configuration.*/
    BOARD_ConfigureSdram() ;

    /* Test external SDRAM access */
    printf( "-I- Test external SDRAM access. \n\r" ) ;

    if ( Sdram_AccessTest() != 0 )
    {
        printf( "-I- Access SDRAM successful.\n\r" ) ;
    }
    else
    {
        printf( "-I- Access SDRAM failed.\n\r" ) ;
    }

    if ( Sdram_Benchmarks() )
    {
      printf("-I- SDRAM test pass.\n\r" ) ;
    }

    return 0 ;
}
