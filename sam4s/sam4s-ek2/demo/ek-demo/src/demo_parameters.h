/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support
 * ----------------------------------------------------------------------------
 * Copyright (c) 2012, Atmel Corporation
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

#ifndef _DEMO_PARAMETERS_
#define _DEMO_PARAMETERS_

#include "board.h"
#include "libsam_gui.h"
#include "frontend_ADS7843.h"

/**
 * \addtogroup SAM4S_demo AT91SAM4S demo
 * @{
 * \addtogroup parameters Demo Parameters
 * @{
 * Demo parameters
 */

/** Demo version */
#define DEMO_VERSION "1.2"

/** Demo board */
#define DEMO_LABEL   "SAM4S-EK2"

/** Switch to enable/disable QTouch library use */
#define DEMO_USE_QTOUCH
//#undef DEMO_USE_QTOUCH

/** Switch to enable/disable USB library use */
#define _DEMO_USE_USB_
//#undef _DEMO_USE_USB_

/** Parameters magic number */
#define DEMO_PARAMETERS_MAGIC 'LMTA'

/**
 * Switch to enable/disable QTouch library use
 * - If defined, the external 32kHz xtal is used as Slow Clock
 * - If undefined, we keep the internal 32kHz internal RC as slow clock
 */
#define DEMO_ENABLE_32KHZ_EXT

/** This structure is used to save application parameters in Internal Flash */
typedef struct _SDemoparameters
{
    uint32_t dwMagic ;
    uint32_t dwPPTDelay ;
    Ads7843CalibParameters sCalibration ;
    uint32_t dwHour ;
    uint32_t dwMin ;
    uint32_t dwSec ;
    uint32_t dwDay ;
    uint32_t dwMonth ;
    uint32_t dwYear ;
    uint32_t dwBacklight ;
} SDemoparameters ;

extern SDemoparameters g_demo_parameters ;

extern uint32_t Demo_Parameters_Initialize( void ) ;
extern uint32_t Demo_Parameters_CommitChanges( void ) ;

/** @} */
/** @} */

#endif // _DEMO_PARAMETERS_
