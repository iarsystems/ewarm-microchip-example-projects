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

/**
 * \addtogroup screens
 * @{
 * \addtogroup screen_disp_demo SAM-GUI DISP demo screen
 * @{
 *
 * \brief This screen aims at demonstrating capabilities of SAM-GUI/DISP drawing primitives.
 */

#define N 150

typedef enum
{
    DISP_DEMO_STATE_COLORS,
    DISP_DEMO_STATE_PRIMITIVES,
    DISP_DEMO_STATE_BITMAP,
    DISP_DEMO_STATE_MAX,
} eDISPState ;

typedef enum
{
    DISP_DEMO_CIRCLE,
    DISP_DEMO_CIRCLE_FILLED,
    DISP_DEMO_RECTANGLE,
    DISP_DEMO_RECTANGLE_FILLED,
    DISP_DEMO_MAX,
} eDISPType ;

#define DISP_DEMO_COLORS_DELAY         5000
#define DISP_DEMO_PRIMITIVES_DELAY     45000
//#define DISP_DEMO_BITMAP_DELAY         10000
#define DISP_DEMO_BITMAP_DELAY         1000

#define DEMO_OFFSET_X                  20
#define DEMO_OFFSET_Y                  60
#define DEMO_WIDTH                     (BOARD_LCD_WIDTH-(DEMO_OFFSET_X<<1))
#define DEMO_HEIGHT                    (BOARD_LCD_HEIGHT-DEMO_OFFSET_Y-10)

#define DEMO_DEFAULT_LUMINANCE         0.50f

#define min( a, b ) ((a<b)?a:b)
#define max( a, b ) ((a<b)?b:a)

//static __no_init SWGT_Widget g_btnBack ;
//static __no_init uint32_t _ScrDISPDemo_dwDelayOld ;
static __no_init uint32_t _ScrDISPDemo_dwState ;
static __no_init uint32_t _ScrDISPDemo_dwTimeStamp ;
static __no_init int _ScrDISPDemo_iDone ;

static uint32_t _ScrDISPDemo_GetRand( uint32_t dwMax )
{
    uint32_t dwTemp ;

    dwTemp=rand() ;
    dwTemp*=dwMax ;
    dwTemp/=65536 ;

    return dwTemp ;
}

static uint32_t _ScrDISPDemo_OnInitialize( SWGTScreen* pScreen )
{
    WGT_Screen_Initialize( pScreen, GUICLR_WHITE, NULL ) ;

//    WGT_CreateWidget( &g_btnBack, WGT_TYPE_BUTTON, 12, 252, 30, 56 ) ;
//    WGT_SetText( &g_btnBack, "btn_back" ) ;
////    WGT_SetBitmap( &g_btnBack, (void*)aucBtn_back ) ;
//    WGT_SetBkgndColor( &g_btnBack, GUICLR( 44, 200, 254 ) ) ;
//    WGT_Screen_AddWidget( pScreen, &g_btnBack ) ;

////    _ScrDISPDemo_dwDelayOld=WGT_SetTimerPeriod( 1000/12 ) ;
    _ScrDISPDemo_dwState=DISP_DEMO_STATE_COLORS ;
    srand( 4259472465 ) ;
    _ScrDISPDemo_dwTimeStamp=xTaskGetTickCount() ;
    _ScrDISPDemo_iDone=0 ;

    return SAMGUI_E_OK ;
}

static uint32_t _ScrDISPDemo_OnDrawBackground( SWGTScreen* pScreen )
{
    SGUIColor clr ;

    // Draw Atmel logo
//	g_WGT_CoreData.pBE->DrawBitmap( 4, 8, 87, 40, (void*)aucBmp_logo_atmel ) ;
    g_WGT_CoreData.pBE->DrawBitmap( 4, 8, 87, 40, DEMO_BMP_LOGO_ATMEL ) ;

    // Draw labels
    clr.u.dwRGBA=GUICLR_ATMEL_BLUE ;
    g_WGT_CoreData.pBE->DrawText( 105, 8, DEMO_LABEL"\nDISP demo", &clr, (void*)&g_Font10x14, 0 ) ;

    clr.u.dwRGBA=GUICLR_BLACK ;
    g_WGT_CoreData.pBE->DrawFilledRectangle( DEMO_OFFSET_X, DEMO_OFFSET_Y, DEMO_OFFSET_X+DEMO_WIDTH, DEMO_OFFSET_Y+DEMO_HEIGHT, NULL, &clr ) ;

    return SAMGUI_E_OK ;
}

static void _ScrDISPDemo_DrawPrimitives( void )
{
    uint32_t dwType ;
    uint32_t dw ;
    uint32_t dwMin ;
    uint32_t dwX1 ;
    uint32_t dwY1 ;
    uint32_t dwX2 ;
    uint32_t dwY2 ;
    uint32_t dwWidth ;
    uint32_t dwHeight ;
    SGUIColor clr ;
    SGUIColor clr2 ;

    for ( dw=0 ; dw < 1000 ; dw++ )
    {
        dwType=rand()&3 ; //_ScrDISPDemo_GetRand(DISP_DEMO_MAX-1) ;

        clr.u.dwRGBA = _ScrDISPDemo_GetRand( 255 )<<2 ; // Red
        clr.u.dwRGBA |= _ScrDISPDemo_GetRand( 255 )<<10 ; // Green
        clr.u.dwRGBA |= _ScrDISPDemo_GetRand( 255 )<<18 ; // Blue
        clr2.u.dwRGBA = _ScrDISPDemo_GetRand( 255 )<<2 ; // Red
        clr2.u.dwRGBA |= _ScrDISPDemo_GetRand( 255 )<<10 ; // Green
        clr2.u.dwRGBA |= _ScrDISPDemo_GetRand( 255 )<<18 ; // Blue

        // Find primitive bounding box
        dwX1=DEMO_OFFSET_X+_ScrDISPDemo_GetRand( DEMO_WIDTH-1 ) ;
        dwY1=DEMO_OFFSET_Y+_ScrDISPDemo_GetRand( DEMO_HEIGHT-1 ) ;
        dwX2=DEMO_OFFSET_X+_ScrDISPDemo_GetRand( DEMO_WIDTH-1 ) ;
        dwY2=DEMO_OFFSET_Y+_ScrDISPDemo_GetRand( DEMO_HEIGHT-1 ) ;
        dwWidth=(dwX1<dwX2)?dwX2-dwX1+1:dwX1-dwX2+1 ;
        dwHeight=(dwY1<dwY2)?dwY2-dwY1+1:dwY1-dwY2+1 ;

//        if ( (dwX1 >= DEMO_OFFSET_X) && (dwX2 <= DEMO_OFFSET_X+DEMO_WIDTH) &&
//             (dwY1 >= DEMO_OFFSET_Y) && (dwY2 <= DEMO_OFFSET_Y+DEMO_HEIGHT) )
//        {
//            printf( "bounding box %u %u %u %u\r\n", dwX1, dwY1, dwX2, dwY2 ) ;
//        }
//        else
//        {
//            printf( "++++ bounding box %u %u %u %u\r\n", dwX1, dwY1, dwX2, dwY2 ) ;
//        }

        switch ( dwType )
        {
            case DISP_DEMO_CIRCLE :
                dwMin=(dwWidth<dwHeight)?dwWidth:dwHeight ;
                g_WGT_CoreData.pBE->DrawCircle( dwX1+(dwWidth>>1), dwY1+(dwHeight>>1), dwMin>>1, &clr ) ;
            break ;

            case DISP_DEMO_CIRCLE_FILLED :
                dwMin=(dwWidth<dwHeight)?dwWidth:dwHeight ;
//                printf( "filled %u %u %u\r\n", dwX1+(dwWidth>>1), dwY1+(dwHeight>>1), dwMin ) ;
                g_WGT_CoreData.pBE->DrawFilledCircle( dwX1+(dwWidth>>1), dwY1+(dwHeight>>1), dwMin>>1, &clr2, &clr ) ;
            break ;

            case DISP_DEMO_RECTANGLE :
                g_WGT_CoreData.pBE->DrawRectangle( dwX1, dwY1, dwX2, dwY2, &clr ) ;
            break ;

            case DISP_DEMO_RECTANGLE_FILLED :
                g_WGT_CoreData.pBE->DrawFilledRectangle( dwX1, dwY1, dwX2, dwY2, &clr2, &clr ) ;
            break ;
        }
    }
}

static void _ScrDISPDemo_DrawColors( void )
{
    uint32_t dwRow ;
    uint32_t dwCol ;
    SGUIColor clr ;
    uint32_t dwOffsetX ;
    uint32_t dwOffsetY ;

    //
    // fill color picker bitmap
    //
    if ( _ScrDISPDemo_iDone == 0 )
    {
#if 0
        for ( dwRow=0 ; dwRow < DEMO_HEIGHT ; dwRow++ )
        {
            for( dwCol=0 ; dwCol < DEMO_WIDTH ; dwCol++ )
            {
                clr=_HLStoRGB( (double)dwRow/(double)DEMO_HEIGHT, DEMO_DEFAULT_LUMINANCE, (double)dwCol/(double)DEMO_WIDTH ) ;
//                clr.u.dwRGBA=_HLStoRGB( (double)dwRow/(double)DEMO_HEIGHT, currentLuminance, (double)dwCol/(double)DEMO_WIDTH ) ;
                g_WGT_CoreData.pBE->DrawPixel( DEMO_OFFSET_X+dwCol, DEMO_OFFSET_Y+dwRow, &clr ) ;
            }
        }
#else
#define DEMO_DISP_COLORS_DIVIDER 16
#define DEMO_DISP_COLORS_DIVIDER2 (DEMO_DISP_COLORS_DIVIDER/2)
        for ( dwRow=0 ; dwRow < (DEMO_HEIGHT/DEMO_DISP_COLORS_DIVIDER)/2 ; dwRow++ )
        {
            for( dwCol=0 ; dwCol < (DEMO_WIDTH/DEMO_DISP_COLORS_DIVIDER)/2 ; dwCol++ )
            {
                dwOffsetX=4+DEMO_OFFSET_X+1+(dwCol*DEMO_DISP_COLORS_DIVIDER) ;
                dwOffsetY=5+DEMO_OFFSET_Y+1+(dwRow*DEMO_DISP_COLORS_DIVIDER) ;

                clr.u.dwRGBA=GUICLR( (uint8_t)(dwRow*(256/DEMO_DISP_COLORS_DIVIDER2)), (uint8_t)0, (uint8_t)(dwCol*(256/DEMO_DISP_COLORS_DIVIDER2)) ) ;
                g_WGT_CoreData.pBE->DrawFilledRectangle( dwOffsetX, dwOffsetY,
                                                         dwOffsetX+DEMO_DISP_COLORS_DIVIDER-2, dwOffsetY+DEMO_DISP_COLORS_DIVIDER-2,
                                                         NULL, &clr ) ;

                dwOffsetX+=(DEMO_WIDTH/2)-4 ;
                clr.u.dwRGBA=GUICLR( (uint8_t)0, (uint8_t)(dwRow*(256/DEMO_DISP_COLORS_DIVIDER2)), (uint8_t)(dwCol*(256/DEMO_DISP_COLORS_DIVIDER2)) ) ;
                g_WGT_CoreData.pBE->DrawFilledRectangle( dwOffsetX, dwOffsetY,
                                                         dwOffsetX+DEMO_DISP_COLORS_DIVIDER-2, dwOffsetY+DEMO_DISP_COLORS_DIVIDER-2,
                                                         NULL, &clr ) ;

                dwOffsetY+=(DEMO_HEIGHT/2)-5 ;
                clr.u.dwRGBA=GUICLR( (uint8_t)(dwRow*(256/DEMO_DISP_COLORS_DIVIDER2)), (uint8_t)(dwCol*(256/DEMO_DISP_COLORS_DIVIDER2)), (uint8_t)0 ) ;
                g_WGT_CoreData.pBE->DrawFilledRectangle( dwOffsetX, dwOffsetY,
                                                         dwOffsetX+DEMO_DISP_COLORS_DIVIDER-2, dwOffsetY+DEMO_DISP_COLORS_DIVIDER-2,
                                                         NULL, &clr ) ;

                dwOffsetX-=(DEMO_WIDTH/2)-4 ;
//                dwOffsetY-=(DEMO_HEIGHT/2)-5 ;
                clr.u.dwRGBA=GUICLR( (uint8_t)((dwRow+dwCol)*(256/DEMO_DISP_COLORS_DIVIDER2)), (uint8_t)((dwRow+dwCol)*(256/DEMO_DISP_COLORS_DIVIDER2)), (uint8_t)((dwRow+dwCol)*(256/DEMO_DISP_COLORS_DIVIDER2)) ) ;
                g_WGT_CoreData.pBE->DrawFilledRectangle( dwOffsetX, dwOffsetY,
                                                         dwOffsetX+DEMO_DISP_COLORS_DIVIDER-2, dwOffsetY+DEMO_DISP_COLORS_DIVIDER-2,
                                                         NULL, &clr ) ;
            }
        }
#endif
        _ScrDISPDemo_iDone=1 ;
    }
}

static void _ScrDISPDemo_DrawBitmaps( void )
{
}

static void _ScrDISPDemo_Draw( void )
{
    switch ( _ScrDISPDemo_dwState )
    {
        case DISP_DEMO_STATE_COLORS :
            _ScrDISPDemo_DrawColors() ;
        break ;

        case DISP_DEMO_STATE_PRIMITIVES :
            _ScrDISPDemo_DrawPrimitives() ;
        break ;

        case DISP_DEMO_STATE_BITMAP :
            _ScrDISPDemo_DrawBitmaps() ;
        break ;
    }
}

static void _ScrDISPDemo_IncState( SWGTScreen* pScreen )
{
    _ScrDISPDemo_dwTimeStamp=xTaskGetTickCount() ;
    _ScrDISPDemo_dwState++ ;

    if ( _ScrDISPDemo_dwState == DISP_DEMO_STATE_MAX )
    {
////        WGT_SetTimerPeriod( _ScrDISPDemo_dwDelayOld ) ;
        WGT_SetCurrentScreen( &ScrMain ) ;
    }
    else
    {
        _ScrDISPDemo_OnDrawBackground( pScreen ) ;
//        printf( "ScrDISPDemo - state switch\r\n", _ScrDISPDemo_dwState ) ;
    }
}

static uint32_t _ScrDISPDemo_ProcessMessage( SWGTScreen* pScreen, SWGTCoreMessage* pMsg )
{
//    printf( "ScrDISPDemo - msg id %x\r\n", pMsg->dwID ) ;

    switch ( pMsg->dwID )
    {
        case WGT_MSG_TIMER :
            switch ( _ScrDISPDemo_dwState )
            {
                case DISP_DEMO_STATE_COLORS :
                    if ( xTaskGetTickCount()-_ScrDISPDemo_dwTimeStamp >= DISP_DEMO_COLORS_DELAY )
                    {
                        _ScrDISPDemo_IncState( pScreen ) ;
                    }
                break ;

                case DISP_DEMO_STATE_PRIMITIVES :
                    if ( xTaskGetTickCount()-_ScrDISPDemo_dwTimeStamp >= DISP_DEMO_PRIMITIVES_DELAY )
                    {
                        _ScrDISPDemo_IncState( pScreen ) ;
                    }
                break ;

                case DISP_DEMO_STATE_BITMAP :
                    if ( xTaskGetTickCount()-_ScrDISPDemo_dwTimeStamp >= DISP_DEMO_BITMAP_DELAY )
                    {
                        _ScrDISPDemo_IncState( pScreen ) ;
                    }
                break ;
            }

            _ScrDISPDemo_Draw() ;
        break ;

//        case WGT_MSG_WIDGET_SELECTED :
//            if ( pMsg->dwParam1 == (uint32_t)&g_btnBack )
//            {
//                WGT_SetTimerPeriod( _ScrDISPDemo_dwDelayOld ) ;
//                WGT_SetCurrentScreen( &ScrMain ) ;
//            }
//        break ;

        case WGT_MSG_POINTER_RELEASED : // cursor released
        case WGT_MSG_KEY_RELEASED : // button released
            _ScrDISPDemo_IncState( pScreen ) ;
        break ;
    }

    return SAMGUI_E_OK ;
}

extern SWGTScreen ScrDISPDemo=
{
    .ProcessMessage=_ScrDISPDemo_ProcessMessage,

    /* Hooks */
    .HkBeforePaint=NULL,
    .HkAfterPaint=NULL,
    .HkBeforeEraseBackground=NULL,
    .HkAfterEraseBackground=NULL,
    .HkExit=NULL,

    /* Callbacks */
    .OnInitialize=_ScrDISPDemo_OnInitialize,
    .OnEraseBackground=_ScrDISPDemo_OnDrawBackground,
    .OnPaint=NULL
} ;

/** @} */
/** @} */
