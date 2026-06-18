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

#include "gui/screens.h"
#include "libsam_gui.h"
#include "demo_parameters.h"
#include "board.h"

//#include <stdio.h>
//#include <stdint.h>
//#include <math.h>

/**
 * \addtogroup screens
 * @{
 * \addtogroup screen_disp_demo SAM-GUI Mandelbrot screen
 * @{
 *
 * \brief This screen aims at playing with fractals.
 */

typedef enum
{
    FRACTAL_MANDELBROT,
    FRACTAL_JULIA,
} eFractalType ;

#define DEMO_OFFSET_X                  20
#define DEMO_OFFSET_Y                  60
#define DEMO_WIDTH                     (BOARD_LCD_WIDTH-(DEMO_OFFSET_X<<1))
#define DEMO_HEIGHT                    180 //(BOARD_LCD_HEIGHT-DEMO_OFFSET_Y-10)

#define MANDELBROT_X_MIN               -2.0f
#define MANDELBROT_X_MAX               +0.8f
#define MANDELBROT_Y_MIN               -1.4f
#define MANDELBROT_Y_MAX               +1.4f

#define JULIA_X_MIN                    -2.0f
#define JULIA_X_MAX                    +2.0f
#define JULIA_Y_MIN                    -2.0f
#define JULIA_Y_MAX                    +2.0f

#define DEFAULT_ITER_MAX               256

//#define min( a, b ) ((a<b)?a:b)
//#define max( a, b ) ((a<b)?b:a)

static __no_init uint32_t _ScrMandelbrot_dwState ;
static __no_init SWGT_Widget* g_btnBack ;
static __no_init SWGT_Widget* g_btnJulia ;

/**
 *  Fractal Specific definitions
 */

#define REAL float
//#define REAL fixed

typedef struct _SViewWindow
{
    uint32_t dwLeft ;
    uint32_t dwRight ;
    uint32_t dwTop ;
    uint32_t dwBottom ;
    uint32_t dwWidth ;
    uint32_t dwHeight ;

    REAL fXMin ;
    REAL fXMax ;
    REAL fYMin ;
    REAL fYMax ;
    REAL fDeltaX ;
    REAL fDeltaY ;

    REAL fCX ;
    REAL fCY ;
} SViewWindow ;

//static const uint8_t aucLUT_GIMP_GOLD
static SViewWindow _sMandelbrot_Window ;
static SViewWindow _sJulia_Window ;

static void _ScrMandelbrot_InitializeMandelbrotWindow( void )
{
    _sMandelbrot_Window.dwLeft=DEMO_OFFSET_X ;
    _sMandelbrot_Window.dwRight=DEMO_OFFSET_X+DEMO_WIDTH-1 ;
    _sMandelbrot_Window.dwTop=DEMO_OFFSET_Y ;
    _sMandelbrot_Window.dwBottom=DEMO_OFFSET_Y+DEMO_HEIGHT-1 ;
    _sMandelbrot_Window.dwWidth=DEMO_WIDTH ;
    _sMandelbrot_Window.dwHeight=DEMO_HEIGHT ;

    _sMandelbrot_Window.fXMin=MANDELBROT_X_MIN ;
    _sMandelbrot_Window.fXMax=MANDELBROT_X_MAX ;
    _sMandelbrot_Window.fYMin=MANDELBROT_Y_MIN ;
    _sMandelbrot_Window.fYMax=MANDELBROT_Y_MAX ;
    _sMandelbrot_Window.fDeltaX=(_sMandelbrot_Window.fXMax-_sMandelbrot_Window.fXMin)/_sMandelbrot_Window.dwWidth ;
    _sMandelbrot_Window.fDeltaY=(_sMandelbrot_Window.fYMax-_sMandelbrot_Window.fYMin)/_sMandelbrot_Window.dwHeight ;
}

static void _ScrMandelbrot_InitializeJuliaWindow( REAL fCX, REAL fCY )
{
    _sJulia_Window.dwLeft=DEMO_OFFSET_X ;
    _sJulia_Window.dwRight=DEMO_OFFSET_X+DEMO_WIDTH-1 ;
    _sJulia_Window.dwTop=DEMO_OFFSET_Y ;
    _sJulia_Window.dwBottom=DEMO_OFFSET_Y+DEMO_HEIGHT-1 ;
    _sJulia_Window.dwWidth=DEMO_WIDTH ;
    _sJulia_Window.dwHeight=DEMO_HEIGHT ;

    _sJulia_Window.fXMin=JULIA_X_MIN ;
    _sJulia_Window.fXMax=JULIA_X_MAX ;
    _sJulia_Window.fYMin=JULIA_Y_MIN ;
    _sJulia_Window.fYMax=JULIA_Y_MAX ;
    _sJulia_Window.fDeltaX=(_sJulia_Window.fXMax-_sJulia_Window.fXMin)/_sJulia_Window.dwWidth ;
    _sJulia_Window.fDeltaY=(_sJulia_Window.fYMax-_sJulia_Window.fYMin)/_sJulia_Window.dwHeight ;

    _sJulia_Window.fCX=fCX ;
    _sJulia_Window.fCY=fCY ;
}

/**
 *  Screen Specific definitions
 */

static uint32_t _ScrMandelbrot_OnInitialize( SWGTScreen* pScreen )
{
    WGT_Screen_Initialize( pScreen, GUICLR_WHITE, NULL ) ;

    g_btnBack=WGT_CreateWidget( WGT_TYPE_BUTTON, 12, 252, 48, 48 ) ;
    if ( g_btnBack )
    {
        WGT_SetBitmap( g_btnBack, DEMO_BMP_BTN_BACK ) ;
        WGT_Screen_AddWidget( pScreen, g_btnBack ) ;
    }

    g_btnJulia=WGT_CreateWidget( WGT_TYPE_BUTTON, 80, 260, 32, 32 ) ;
    if ( g_btnJulia )
    {
        WGT_SetBitmap( g_btnJulia, NULL ) ;
        WGT_Screen_AddWidget( pScreen, g_btnJulia ) ;
    }

    _ScrMandelbrot_dwState=FRACTAL_MANDELBROT ;

    _ScrMandelbrot_InitializeMandelbrotWindow() ;

    return SAMGUI_E_OK ;
}

static uint32_t _ScrMandelbrot_OnDrawBackground( SWGTScreen* pScreen )
{
    SGUIColor clr ;

    // Draw Atmel logo
//	g_WGT_CoreData.pBE->DrawBitmap( 4, 8, 87, 40, (void*)aucBmp_logo_atmel ) ;
    g_WGT_CoreData.pBE->DrawBitmap( 4, 8, 87, 40, DEMO_BMP_LOGO_ATMEL ) ;

    // Draw labels
    clr.u.dwRGBA=GUICLR_ATMEL_BLUE ;
    g_WGT_CoreData.pBE->DrawText( 105, 8, DEMO_LABEL"\nFractals", &clr, (void*)&g_Font10x14, 0 ) ;

    clr.u.dwRGBA=GUICLR_DARKRED ;
    g_WGT_CoreData.pBE->DrawFilledRectangle( DEMO_OFFSET_X, DEMO_OFFSET_Y, DEMO_OFFSET_X+DEMO_WIDTH, DEMO_OFFSET_Y+DEMO_HEIGHT, NULL, &clr ) ;

    return SAMGUI_E_OK ;
}

static void _ScrMandelbrot_DrawMandelbrot( SViewWindow* pWindow, uint32_t dwIterationMax )
{
    uint32_t dwX ;
    uint32_t dwY ;
    REAL rCx ;
    REAL rCy ;
    REAL rX ;
    REAL rY ;
    REAL rX2 ;
    REAL rY2 ;
    REAL r2xy ;
    uint32_t dwIteration ;
    SGUIColor clr ;

    for ( dwY=0 ; dwY < pWindow->dwHeight ; dwY++ )
    {
        for ( dwX=0 ; dwX < pWindow->dwWidth ; dwX++ )
        {
            rCx = pWindow->fXMin + dwX * pWindow->fDeltaX ;
            rCy = pWindow->fYMax - dwY * pWindow->fDeltaY ;
            /* Z = X+I*Y */
            rX = 0.0 ;
            rY = 0.0 ;

            for ( dwIteration=0 ; dwIteration < dwIterationMax ; dwIteration++ )
            {
                rX2 = rX*rX ;
                rY2 = rY*rY ;

                /* Stop iterations when |Z| > 2 */
                if ( rX2 + rY2 > 4.0 )
                {
                    break ;
                }

                r2xy=2.0*rX*rY ;
                /* Z = Z^2 + C */
                rX = rX2 - rY2 + rCx ;
                rY =  r2xy + rCy ;
            }

            /* Here IT is in 0..ITMAX, get the corresponding COLOR */
            clr.u.dwRGBA=GUICLR( 0, 0, dwIteration<<2 ) ;

            /* Store the color in the A array */
            g_WGT_CoreData.pBE->DrawPixel( pWindow->dwLeft+dwX, pWindow->dwTop+dwY, &clr ) ;
        }
    }
}

static void _ScrMandelbrot_DrawJulia( SViewWindow* pWindow, uint32_t dwIterationMax )
{
    uint32_t dwX ;
    uint32_t dwY ;
    REAL rX ;
    REAL rY ;
    REAL rX2 ;
    REAL rY2 ;
    REAL r2xy ;
    uint32_t dwIteration ;
    SGUIColor clr ;
    REAL rCx ;
    REAL rCy ;

    rCx=_sJulia_Window.fCX ;
    rCy=_sJulia_Window.fCY ;

    for ( dwY=0 ; dwY < pWindow->dwHeight ; dwY++ )
    {
        for ( dwX=0 ; dwX < pWindow->dwWidth ; dwX++ )
        {
//            rCx = pWindow->fXMin + dwX * pWindow->fDeltaX ;
//            rCy = pWindow->fYMax - dwY * pWindow->fDeltaY ;
            /* Z = X+I*Y */
            rX = 0.0 ;
            rY = 0.0 ;

            for ( dwIteration=0 ; dwIteration < dwIterationMax ; dwIteration++ )
            {
                rX2 = rX*rX ;
                rY2 = rY*rY ;

                /* Stop iterations when |Z| > 2 */
                if ( rX2 + rY2 > 4.0 )
                {
                    break ;
                }

                r2xy=2.0*rX*rY ;
                /* Z = Z^2 + C */
                rX = rX2 - rY2 + rCx ;
                rY =  r2xy + rCy ;
            }

            /* Here IT is in 0..ITMAX, get the corresponding COLOR */
            clr.u.dwRGBA=GUICLR( 0, 0, dwIteration<<2 ) ;

            /* Store the color in the A array */
            g_WGT_CoreData.pBE->DrawPixel( pWindow->dwLeft+dwX, pWindow->dwTop+dwY, &clr ) ;
        }
    }
}

static uint32_t _ScrMandelbrot_OnDraw( SWGTScreen* pScreen )
{
    switch ( _ScrMandelbrot_dwState )
    {
        case FRACTAL_MANDELBROT :
            _ScrMandelbrot_DrawMandelbrot( &_sMandelbrot_Window, DEFAULT_ITER_MAX ) ;
        break ;

        case FRACTAL_JULIA :
            _ScrMandelbrot_DrawJulia( &_sJulia_Window, DEFAULT_ITER_MAX ) ;
        break ;
    }

    return SAMGUI_E_OK ;
}

static uint32_t _ScrMandelbrot_ProcessMessage( SWGTScreen* pScreen, SWGTCoreMessage* pMsg )
{
//    printf( "ScrMandelbrot - msg id %x\r\n", pMsg->dwID ) ;

    switch ( pMsg->dwID )
    {
        case WGT_MSG_POINTER_RELEASED : // cursor released
            switch ( _ScrMandelbrot_dwState )
            {
                case FRACTAL_JULIA:
                break ;

                case FRACTAL_MANDELBROT:
                    if ( (pMsg->dwParam1 >= _sMandelbrot_Window.dwLeft) && (pMsg->dwParam1 <= _sMandelbrot_Window.dwRight) &&
                         (pMsg->dwParam1 >= _sMandelbrot_Window.dwTop) && (pMsg->dwParam1 <= _sMandelbrot_Window.dwBottom) )
                    {
                        REAL cx ;
                        REAL cy ;

                        cx=_sMandelbrot_Window.fXMin+_sMandelbrot_Window.fDeltaX*(pMsg->dwParam1-_sMandelbrot_Window.dwLeft) ;
                        cy=_sMandelbrot_Window.fYMin+_sMandelbrot_Window.fDeltaY*(pMsg->dwParam2-_sMandelbrot_Window.dwTop) ;

                        _ScrMandelbrot_InitializeJuliaWindow( cx, cy ) ;
                        WGT_PostMessage( WGT_MSG_PAINT, 0, 0 ) ;
                        _ScrMandelbrot_dwState=FRACTAL_JULIA ;
                    }
                break ;
            }
        break ;

        case WGT_MSG_KEY_RELEASED : // button released
        break ;

        case WGT_MSG_WIDGET_SELECTED :
            if ( pMsg->dwParam1 == (uint32_t)g_btnBack )
            {
                WGT_SetCurrentScreen( &ScrMain ) ;
            }

            if ( pMsg->dwParam1 == (uint32_t)g_btnJulia )
            {
                _ScrMandelbrot_dwState=FRACTAL_JULIA ;
            }
        break ;
    }

    return SAMGUI_E_OK ;
}

extern SWGTScreen ScrMandelbrot=
{
    .ProcessMessage=_ScrMandelbrot_ProcessMessage,

    /* Hooks */
    .HkBeforePaint=NULL,
    .HkAfterPaint=NULL,
    .HkBeforeEraseBackground=NULL,
    .HkAfterEraseBackground=NULL,
    .HkExit=NULL,

    /* Callbacks */
    .OnInitialize=_ScrMandelbrot_OnInitialize,
    .OnEraseBackground=_ScrMandelbrot_OnDrawBackground,
    .OnPaint=_ScrMandelbrot_OnDraw
} ;

/** @} */
/** @} */
