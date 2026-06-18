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

#include "demo_parameters.h"
#include "libsam_gui.h"
#include "board.h"

#include <string.h>
#include <stdio.h>

extern int __offset_parameters__ ;

SDemoparameters g_demo_parameters=
{
    .dwMagic=DEMO_PARAMETERS_MAGIC,
    .dwPPTDelay=5000,
    .sCalibration={
#if 0
    .dwPointX=24,
    .dwPointY=32,
    .dwMeasureX=262,
    .dwMeasureY=269,
    .lSlopeX=7837,
    .lSlopeY=5588,
#else
    .dwPointX=0,
    .dwPointY=0,
    .dwMeasureX=0,
    .dwMeasureY=0,
    .lSlopeX=0,
    .lSlopeY=0,
#endif
    },
    .dwHour=12,
    .dwMin=0,
    .dwSec=0,
    .dwDay=22,
    .dwMonth=9,
    .dwYear=2011,
    .dwBacklight=14
} ;

extern uint32_t Demo_Parameters_Initialize( void )
{
    SDemoparameters* pParameters ;

    pParameters=(SDemoparameters*)&__offset_parameters__ ;

    if ( pParameters->dwMagic == DEMO_PARAMETERS_MAGIC )
    {
        memcpy( &g_demo_parameters, pParameters, sizeof( SDemoparameters ) ) ;
    }
    else
    {
        return Demo_Parameters_CommitChanges() ;
    }

    printf( "Demo_Parameters_Initialize\r\n" ) ;
    printf( "    PPTDelay  %u\r\n", g_demo_parameters.dwPPTDelay ) ;
    printf( "    CalPtX    %u\r\n", g_demo_parameters.sCalibration.dwPointX ) ;
    printf( "    CalPtY    %u\r\n", g_demo_parameters.sCalibration.dwPointY ) ;
    printf( "    CalMesX   %u\r\n", g_demo_parameters.sCalibration.dwMeasureX ) ;
    printf( "    CalMesY   %u\r\n", g_demo_parameters.sCalibration.dwMeasureY ) ;
    printf( "    SlopeX    %d\r\n", g_demo_parameters.sCalibration.lSlopeX ) ;
    printf( "    SlopeY    %d\r\n", g_demo_parameters.sCalibration.lSlopeY ) ;
    printf( "    Hour      %u\r\n", g_demo_parameters.dwHour ) ;
    printf( "    Min       %u\r\n", g_demo_parameters.dwMin ) ;
    printf( "    Sec       %u\r\n", g_demo_parameters.dwSec ) ;
    printf( "    Day       %u\r\n", g_demo_parameters.dwDay ) ;
    printf( "    Month     %u\r\n", g_demo_parameters.dwMonth ) ;
    printf( "    Year      %u\r\n", g_demo_parameters.dwYear ) ;
    printf( "    Backlight %u\r\n", g_demo_parameters.dwBacklight ) ;

    return SAMGUI_E_OK ;
}

extern uint32_t Demo_Parameters_CommitChanges( void )
{
    uint32_t dwRet ;

    taskENTER_CRITICAL() ;
#if 1
    dwRet = FLASHD_EraseSector( (uint32_t)(&__offset_parameters__));
    dwRet = FLASHD_Write( (uint32_t)(&__offset_parameters__), (void*)&g_demo_parameters, sizeof( SDemoparameters ) ) ;
#else
    dwRet=FLASHD_Write( (uint32_t)(&__offset_parameters__), (void*)&g_demo_parameters, sizeof( SDemoparameters ) ) ;
#endif
    taskEXIT_CRITICAL() ;

    if ( dwRet != 0 )
    {
        return SAMGUI_E_OK ;
    }

    return SAMGUI_E_OK ;
}


