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

// Atmel library includes.
#include "board.h"

// Scheduler includes.
#include "libfreertos.h"
#include "demo_parameters.h"

// Atmel UI buttons definitions
#include "libsam_gui.h"

#include <string.h>
#include <assert.h>

/*-----------------------------------------------------------*/

// The LCD task uses the sprintf function so requires a little more stack too.
#define mainLCD_TASK_STACK_SIZE				( configMINIMAL_STACK_SIZE * 2 )
#define mainFAT_TASK_STACK_SIZE				( configMINIMAL_STACK_SIZE * 2 )

/*-----------------------------------------------------------*/

/**
 * \brief Interrupt handler for the RTC. Sends WGT_MSG_TIMER message.
 */
extern void RTC_IrqHandler( void )
{
    uint32_t dwStatus=RTC->RTC_SR ;

    // Second increment interrupt
    if ( (dwStatus & RTC_SR_SEC) == RTC_SR_SEC )
    {
        // Disable RTC interrupt
        RTC_DisableIt( RTC, RTC_IDR_SECDIS ) ;

        LED_Toggle( LED_RED ) ;

        // Post Message
//        WGT_SendMessageISR( WGT_MSG_TIMER, 0, 0 ) ;

        RTC->RTC_SCCR=RTC_SCCR_SECCLR ;

        RTC_EnableIt( RTC, RTC_IER_SECEN ) ;
    }
}

