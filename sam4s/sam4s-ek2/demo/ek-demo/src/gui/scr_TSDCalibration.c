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

#include "gui/screens.h"
#include "libsam_gui.h"
#include "demo_parameters.h"
#include "board.h"

#include <stdio.h>
#include <string.h>

/**
 * \addtogroup screens
 * @{
 * \addtogroup settings Settings
 * @{
 * \addtogroup screen_tsd_calibration TouchScreen Calibration screen
 * @{
 *
 * \brief This screens aims at calibrating the TouchScreen input interface.
 */

/** Size in pixels of calibration points. */
#define POINTS_SIZE         4
/** Maximum difference in pixels between the test point and the measured point. */
#define POINTS_MAX_ERROR    32


//static SWGT_Widget g_wgt_Text1 ;

typedef enum _EScrTSDCalibrationState
{
    CALIB_DOT1,
    CALIB_DOT2,
    CALIB_DOT3,
    CALIB_DOT4,
    CALIB_TEST,
    CALIB_END,
    CALIB_MAX
} EScrTSDCalibrationState ;

typedef struct _Point
{
    uint32_t dwX ;
    uint32_t dwY ;
} Point ;

static const Point aCalibrationPoints[CALIB_MAX]=
{
    { .dwX=(BOARD_LCD_WIDTH/10), .dwY=(BOARD_LCD_HEIGHT/10) },
    { .dwX=(BOARD_LCD_WIDTH)-(BOARD_LCD_WIDTH/10), .dwY=(BOARD_LCD_HEIGHT/10) },
    { .dwX=(BOARD_LCD_WIDTH)-(BOARD_LCD_WIDTH/10), .dwY=(BOARD_LCD_HEIGHT)-(BOARD_LCD_HEIGHT/10) },
    { .dwX=(BOARD_LCD_WIDTH/10), .dwY=(BOARD_LCD_HEIGHT)-(BOARD_LCD_HEIGHT/10) },
    { .dwX=(BOARD_LCD_WIDTH>>1), .dwY=(BOARD_LCD_HEIGHT>>1) }
} ;
static Point aCalibrationMeasures[CALIB_MAX] ;

static uint32_t g_dwScrTSDCalibration_state=CALIB_DOT1 ;


static Ads7843CalibParameters gs_TsdCalibParameters ;


static uint32_t _ScrTSDCalibration_OnInitialize( SWGTScreen* pScreen )
{
    uint32_t dwLength = 0;

    WGT_Screen_Initialize( pScreen, GUICLR_WHITE, NULL ) ;

    g_dwScrTSDCalibration_state = CALIB_DOT1;

    g_WGT_CoreData.pBE->IOCtl( DISP_BACKEND_IOCTL_SET_BACKLIGHT, (uint32_t*)14, NULL ) ;

    memset( &gs_TsdCalibParameters, 0, sizeof( Ads7843CalibParameters ) ) ;
    dwLength=sizeof( Ads7843CalibParameters ) ;
    g_pFrontend_TSD->IOCtl( WGT_FRONTEND_IOCTL_SET_DATA, (uint32_t *)(&gs_TsdCalibParameters), &dwLength ) ;

    return SAMGUI_E_OK ;
}

static uint32_t _ScrTSDCalibration_OnDrawBackground( SWGTScreen* pScreen )
{
    SGUIColor clr ;

    // Draw Atmel logo
    g_WGT_CoreData.pBE->DrawBitmap( 76, (BOARD_LCD_HEIGHT>>2), 87, 40, DEMO_BMP_LOGO_ATMEL ) ;

    // Draw labels
    clr.u.dwRGBA=GUICLR_ATMEL_BLUE ;
    g_WGT_CoreData.pBE->DrawText( 1+30, 3*(BOARD_LCD_HEIGHT>>2), "TSD Calibration", &clr, (void*)&g_Font10x14, 0 ) ;

    return SAMGUI_E_OK ;
}

static uint32_t _ScrTSDCalibration_ProcessSlopes( void )
{
    int32_t lSlope1 ;
    int32_t lSlope2 ;
    uint32_t dwLength ;

    /**
     * Calculate slopes using the calibration data
     * Theory behind those calculations:
     *   - We suppose the touchscreen measurements are linear, so the following equations are true (simple
     *     linear regression) for any two 'a' and 'b' points of the screen:
     *       dx = (a.data[0] - b.data[0]) / (a.x - b.x)
     *       dy = (a.data[1] - b.data[1]) / (a.y - b.y)
     *
     *   - We calculate dx and dy (called xslope and yslope here) using the calibration points.
     *
     *   - We can then use dx and dy to infer the position of a point 'p' given the measurements performed
     *     by the touchscreen ('c' is any of the calibration points):
     *       dx = (p.data[0] - c.data[0]) / (p.x - c.x)
     *       dy = (p.data[1] - c.data[1]) / (p.y - c.y)
     *     Thus:
     *       p.x = c.x - (p.data[0] - c.data[0]) / dx
     *       p.y = c.y - (p.data[1] - c.data[1]) / dy
     *
     *   - Since there are four calibration points, dx and dy can be calculated twice, so we average
     *     the two values.
     */
    lSlope1 = ((int32_t)aCalibrationMeasures[0].dwX) - ((int32_t)aCalibrationMeasures[1].dwX ) ;
    lSlope1 *= 1024 ;
    lSlope1 /= ((int32_t)aCalibrationPoints[0].dwX) - ((int32_t)aCalibrationPoints[1].dwX ) ;
    lSlope2 = ((int32_t)aCalibrationMeasures[2].dwX) - ((int32_t)aCalibrationMeasures[3].dwX ) ;
    lSlope2 *= 1024 ;
    lSlope2 /= ((int32_t)aCalibrationPoints[2].dwX) - ((int32_t)aCalibrationPoints[3].dwX ) ;
    gs_TsdCalibParameters.lSlopeX=(lSlope1+lSlope2)/2 ;


    lSlope1 = ((int32_t)aCalibrationMeasures[0].dwY) - ((int32_t)aCalibrationMeasures[2].dwY ) ;
    lSlope1 *= 1024 ;
    lSlope1 /= ((int32_t)aCalibrationPoints[0].dwY) - ((int32_t)aCalibrationPoints[2].dwY);
    lSlope2 = ((int32_t)aCalibrationMeasures[1].dwY) - ((int32_t)aCalibrationMeasures[3].dwY ) ;
    lSlope2 *= 1024 ;
    lSlope2 /= ((int32_t)aCalibrationPoints[1].dwY) - ((int32_t)aCalibrationPoints[3].dwY ) ;
    gs_TsdCalibParameters.lSlopeY=(lSlope1+lSlope2)/2 ;


    gs_TsdCalibParameters.dwPointX=aCalibrationPoints[0].dwX ;
    gs_TsdCalibParameters.dwPointY=aCalibrationPoints[0].dwY ;
    gs_TsdCalibParameters.dwMeasureX=aCalibrationMeasures[0].dwX ;
    gs_TsdCalibParameters.dwMeasureY=aCalibrationMeasures[0].dwY ;

    dwLength=sizeof( gs_TsdCalibParameters ) ;
    g_pFrontend_TSD->IOCtl( WGT_FRONTEND_IOCTL_SET_DATA, (uint32_t *)(&(gs_TsdCalibParameters)), &dwLength ) ;

    return SAMGUI_E_OK ;
}

static uint32_t _ScrTSDCalibration_ProcessMessage( SWGTScreen* pScreen, SWGTCoreMessage* pMsg )
{
    SGUIColor clrPoint={ .u.dwRGBA=GUICLR_RED } ;
    SGUIColor clrBkgnd={ .u.dwRGBA=ScrTSDCalibration.dwClrBackground } ;
    static uint32_t dwTime=0 ;
    static uint32_t dwLength=0 ;
    int iDiffX ;
    int iDiffY ;
//    Point IntPoint ;

//    printf( "ScrTSDCalibration - msg id %x\r\n", pMsg->dwID ) ;

    switch ( pMsg->dwID )
    {
        case WGT_MSG_PAINT :
            // Draw first calibration dot (top/left)
            g_WGT_CoreData.pBE->DrawFilledCircle( aCalibrationPoints[CALIB_DOT1].dwX, aCalibrationPoints[CALIB_DOT1].dwY, POINTS_SIZE>>1, NULL, &clrPoint ) ;
        break ;

        case WGT_MSG_POINTER_RAW :
            // Debounce
            if ( xTaskGetTickCount() > dwTime+500 )
            {
                dwTime=xTaskGetTickCount() ;
            }
            else
            {
                // Debounce

                break ;
            }

//            printf( "Received dot at (%u, %u)\r\n", pMsg->dwParam1, pMsg->dwParam2 ) ;
            aCalibrationMeasures[g_dwScrTSDCalibration_state].dwX=pMsg->dwParam1 ;
            aCalibrationMeasures[g_dwScrTSDCalibration_state].dwY=pMsg->dwParam2 ;

            switch ( g_dwScrTSDCalibration_state )
            {
                case CALIB_DOT1 :
                    // Clear first calibration dot (top/left)
                    g_WGT_CoreData.pBE->DrawFilledCircle( aCalibrationPoints[CALIB_DOT1].dwX, aCalibrationPoints[CALIB_DOT1].dwY, POINTS_SIZE>>1, NULL, &clrBkgnd ) ;
                    printf( "Received dot1 (%u, %u)\r\n", aCalibrationMeasures[CALIB_DOT1].dwX, aCalibrationMeasures[CALIB_DOT1].dwY ) ;

                    g_dwScrTSDCalibration_state=CALIB_DOT2 ;
                    // Draw second calibration dot (top/right)
                    g_WGT_CoreData.pBE->DrawFilledCircle( aCalibrationPoints[CALIB_DOT2].dwX, aCalibrationPoints[CALIB_DOT2].dwY, POINTS_SIZE>>1, NULL, &clrPoint ) ;
                break ;

                case CALIB_DOT2 :
                    // Clear second calibration dot (top/right)
                    g_WGT_CoreData.pBE->DrawFilledCircle( aCalibrationPoints[CALIB_DOT2].dwX, aCalibrationPoints[CALIB_DOT2].dwY, POINTS_SIZE>>1, NULL, &clrBkgnd ) ;
                    printf( "Received dot2 (%u, %u)\r\n", aCalibrationMeasures[CALIB_DOT2].dwX, aCalibrationMeasures[CALIB_DOT2].dwY ) ;

                    g_dwScrTSDCalibration_state=CALIB_DOT3 ;
                    // Draw third calibration dot (bottom/right)
                    g_WGT_CoreData.pBE->DrawFilledCircle( aCalibrationPoints[CALIB_DOT3].dwX, aCalibrationPoints[CALIB_DOT3].dwY, POINTS_SIZE>>1, NULL, &clrPoint ) ;
                break ;

                case CALIB_DOT3 :
                    // Clear third calibration dot (bottom/right)
                    g_WGT_CoreData.pBE->DrawFilledCircle( aCalibrationPoints[CALIB_DOT3].dwX, aCalibrationPoints[CALIB_DOT3].dwY, POINTS_SIZE>>1, NULL, &clrBkgnd ) ;
                    printf( "Received dot3 (%u, %u)\r\n", aCalibrationMeasures[CALIB_DOT3].dwX, aCalibrationMeasures[CALIB_DOT3].dwY ) ;

                    g_dwScrTSDCalibration_state=CALIB_DOT4 ;
                    // Draw fourth calibration dot (bottom/left)
                    g_WGT_CoreData.pBE->DrawFilledCircle( aCalibrationPoints[CALIB_DOT4].dwX, aCalibrationPoints[CALIB_DOT4].dwY, POINTS_SIZE>>1, NULL, &clrPoint ) ;
                break ;

                case CALIB_DOT4 :
                    // Clear fourth calibration dot (bottom/left)
                    g_WGT_CoreData.pBE->DrawFilledCircle( aCalibrationPoints[CALIB_DOT4].dwX, aCalibrationPoints[CALIB_DOT4].dwY, POINTS_SIZE>>1, NULL, &clrBkgnd ) ;
                    printf( "Received dot4 (%u, %u)\r\n", aCalibrationMeasures[CALIB_DOT4].dwX, aCalibrationMeasures[CALIB_DOT4].dwY ) ;

                    // Process measured data
                    _ScrTSDCalibration_ProcessSlopes() ;

                    g_dwScrTSDCalibration_state=CALIB_TEST ;
                    // Draw test calibration dot (center/center)
                    g_WGT_CoreData.pBE->DrawFilledCircle( aCalibrationPoints[CALIB_TEST].dwX, aCalibrationPoints[CALIB_TEST].dwY, POINTS_SIZE>>1, NULL, &clrPoint ) ;
                    g_WGT_CoreData.pBE->DrawCircle( aCalibrationPoints[CALIB_TEST].dwX, aCalibrationPoints[CALIB_TEST].dwY, POINTS_MAX_ERROR>>1, &clrPoint ) ;

                break ;

            }
        break ;

        case WGT_MSG_POINTER_PRESSED :
            if ( g_dwScrTSDCalibration_state == CALIB_TEST )
            {
                printf( "Received Point Pressed (%u, %u)\r\n", pMsg->dwParam1, pMsg->dwParam2 ) ;

                iDiffX=(int32_t)(pMsg->dwParam1)-(int32_t)(aCalibrationPoints[CALIB_TEST].dwX) ;
                iDiffY=(int32_t)(pMsg->dwParam2)-(int32_t)(aCalibrationPoints[CALIB_TEST].dwY) ;

                if ( (iDiffX >= -POINTS_MAX_ERROR) && (iDiffX <= POINTS_MAX_ERROR) &&
                     (iDiffY >= -POINTS_MAX_ERROR) && (iDiffY <= POINTS_MAX_ERROR) )
                {
                    g_dwScrTSDCalibration_state=CALIB_END ;
                    memcpy( &(g_demo_parameters.sCalibration), &gs_TsdCalibParameters, sizeof( Ads7843CalibParameters ) ) ;
                    Demo_Parameters_CommitChanges() ;
//                    WGT_SetCurrentScreen( &ScrMain ) ;
                }
                else
                {
                    /* Reset calibration parameters */
                    memset( &gs_TsdCalibParameters, 0, sizeof( gs_TsdCalibParameters ) ) ;
                    dwLength=sizeof( Ads7843CalibParameters ) ;
                    g_pFrontend_TSD->IOCtl( WGT_FRONTEND_IOCTL_SET_DATA, (uint32_t *)(&(gs_TsdCalibParameters)), &dwLength ) ;

                    /* Return to first calibration state */
                    SAMGUI_TaskDelay( 500 ) ;
                    WGT_SetCurrentScreen( pScreen ) ;
                }
            }
        break ;

        case WGT_MSG_POINTER_RELEASED :
            if ( g_dwScrTSDCalibration_state == CALIB_END )
            {
                WGT_SetCurrentScreen( &ScrMain ) ;
            }
        break;

        case WGT_MSG_KEY_PRESSED : // button pressed

            dwLength=sizeof( Ads7843CalibParameters ) ;
            g_pFrontend_TSD->IOCtl( WGT_FRONTEND_IOCTL_SET_DATA, (uint32_t *)(&(g_demo_parameters.sCalibration)), &dwLength ) ;
            // Send message to switch to main screen
            WGT_SetCurrentScreen( &ScrMain ) ;
        break ;
    }

    return SAMGUI_E_OK ;
}

extern SWGTScreen ScrTSDCalibration=
{
    .ProcessMessage=_ScrTSDCalibration_ProcessMessage,

    /* Hooks */
    .HkBeforePaint=NULL,
    .HkAfterPaint=NULL,
    .HkBeforeEraseBackground=NULL,
    .HkAfterEraseBackground=NULL,
    .HkExit=NULL,

    /* Callbacks */
    .OnInitialize=_ScrTSDCalibration_OnInitialize,
    .OnEraseBackground=_ScrTSDCalibration_OnDrawBackground,
    .OnPaint=NULL
} ;

/** @} */
/** @} */
/** @} */
