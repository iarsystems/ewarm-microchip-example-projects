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
 * \addtogroup screen_ppt AT91SAM3S Presentation
 * @{
 *
 * \brief This screen display AT91SAM3S presentation. The delay between pages can
 * be modified using Potentiometer or QTouch slider.
 */


#define SCR_PPT_SLIDE_DELAY_MIN     1
#define SCR_PPT_SLIDE_DELAY_MAX     10
#define SCR_PPT_SLIDE_MAX           2
#define SCR_PPT_SLIDE_PATH          "/demo/slideshow/"
#define SCR_PPT_SLIDE_FOLDER        "8bpp/"
#define SCR_PPT_SLIDE_BASENAME      "Slide"
#define SCR_PPT_SLIDE_EXT           ".bmp"

static volatile __no_init uint32_t gs_dwLastSwitchTime ;
static volatile __no_init uint32_t gs_dwSwitchDelay ;
static __no_init uint32_t gs_dwSlideIndex ;
static __no_init char gs_szSlidePath[128] ;


//static FRESULT hscan_files( char* szPath, uint32_t dwLength )
//{
//    FRESULT res;
//    FILINFO fno;
//    DIR dir;
//    int i;
//    char *szName ;
//#if _USE_LFN
//    static char lfn[_MAX_LFN * (_DF1S ? 2 : 1) + 1];
//    fno.lfname = lfn;
//    fno.lfsize = sizeof(lfn);
//#endif
//
//    res=f_opendir( &dir, szPath ) ;
//
//    if (res == FR_OK)
//    {
//        i=strlen( szPath ) ;
//
//        for ( ; ; )
//        {
//            res=f_readdir( &dir, &fno ) ;
//
////            printf( "--- %s\r\n", fno.fname ) ;
//            if (res != FR_OK || fno.fname[0] == 0) break;
//            if (fno.fname[0] == '.') continue;
//#if _USE_LFN
//            szName = *fno.lfname ? fno.lfname : fno.fname;
//#else
//            szName = fno.fname;
//#endif
//            if (fno.fattrib & AM_DIR)
//            {
//                printf( "dir   %s\r\n", szName ) ;
//                strncat( szPath, "/", dwLength ) ;
//                strncat( szPath, szName, dwLength ) ;
//                res = hscan_files( szPath, dwLength ) ;
//                if (res != FR_OK) break;
//                szPath[i] = 0;
//            }
//            else
//            {
//                printf( "entry %s/%s\r\n", szPath, szName ) ;
//            }
//        }
//    }
//    else
//    {
//        printf( "scan_files - failed to opendir %s\r\n", szPath ) ;
//    }
//
//    return res;
//}

static uint32_t ScrPPT_OnInitialize( SWGTScreen* pScreen )
{
//    FIL fp ;
//    static uint8_t aucData[40] ;
//    uint32_t dwLength ;
//    static char szPath[256]="" ;

    gs_dwLastSwitchTime=0 ;
    gs_dwSwitchDelay=(SCR_PPT_SLIDE_DELAY_MAX-SCR_PPT_SLIDE_DELAY_MIN)>>1 ;
    gs_dwSlideIndex=1 ;

    WGT_Screen_Initialize( pScreen, GUICLR_WHITE, NULL ) ;

//    strcpy( szPath, "/demo/slideshow" ) ;
//    hscan_files( szPath, sizeof( szPath ) ) ;
////    hscan_files( "/demo/slideshow" ) ;
//    if ( f_open( &fp, "/demo/slides/Slide1.BMP", FA_OPEN_EXISTING|FA_READ ) == FR_OK )
//    {
//        if ( f_read( &fp, aucData, sizeof( aucData ), &dwLength ) == FR_OK )
//        {
//            printf( "read %u bytes\r\n", dwLength ) ;
//        }
//        else
//        {
//            printf( "failed to read\r\n" ) ;
//        }
//        f_close( &fp ) ;
//    }
//    else
//    {
//        printf( "failed to open\r\n" ) ;
//    }

//    scan_files( STR_ROOT_DIRECTORY ) ;
//    scan_files( STR_ROOT_DIRECTORY "demo" ) ;
//    scan_files( STR_ROOT_DIRECTORY "demo/bitmaps" ) ;
//    scan_files( STR_ROOT_DIRECTORY "demo/slideshow" ) ;

    return SAMGUI_E_OK ;
}

static uint32_t ScrPPT_OnDrawBackground( SWGTScreen* pScreen )
{
    g_WGT_CoreData.pBE->DrawBitmap( 0, 0, BOARD_LCD_WIDTH, BOARD_LCD_HEIGHT, (void*)SCR_PPT_SLIDE_PATH SCR_PPT_SLIDE_FOLDER SCR_PPT_SLIDE_BASENAME "1" SCR_PPT_SLIDE_EXT ) ;

    return SAMGUI_E_OK ;
}

static uint32_t ScrPPT_ProcessMessage( SWGTScreen* pScreen, SWGTCoreMessage* pMsg )
{
//    printf( "ScrPPT - msg id %x\r\n", pMsg->dwID ) ;
//    FIL fp ;

    switch ( pMsg->dwID )
    {
        case WGT_MSG_TIMER : // Timer triggered
            gs_dwLastSwitchTime++ ;
            if ( gs_dwLastSwitchTime >= gs_dwSwitchDelay )
            {
                gs_dwLastSwitchTime=0 ;

                // switch to next screen
                gs_dwSlideIndex++ ;

                // Process new path
                snprintf( gs_szSlidePath, sizeof( gs_szSlidePath ), "%s%s%s%u%s", SCR_PPT_SLIDE_PATH, SCR_PPT_SLIDE_FOLDER, SCR_PPT_SLIDE_BASENAME, gs_dwSlideIndex, SCR_PPT_SLIDE_EXT ) ;
//                printf( "Switching to slide %s\r\n", gs_szSlidePath ) ;

                // check if file exists
//                if ( f_open( &fp, szSlidePath, FA_OPEN_EXISTING|FA_READ ) == FR_OK )
                if ( gs_dwSlideIndex <= SCR_PPT_SLIDE_MAX )
                {
//                    f_close( &fp ) ;
                    g_WGT_CoreData.pBE->DrawBitmap( 0, 0, BOARD_LCD_WIDTH, BOARD_LCD_HEIGHT, (void*)gs_szSlidePath ) ;
//                    {
//                        SGUIColor clr ;
//
//                        clr.u.dwRGBA=GUICLR_ATMEL_BLUE ;
//                        snprintf( gs_szSlidePath, sizeof( gs_szSlidePath ), "Slide%u"SCR_PPT_SLIDE_EXT, gs_dwSlideIndex ) ;
//                        g_WGT_CoreData.pBE->DrawText( 10, 10, gs_szSlidePath, &clr, (void*)&g_Font10x14, 0 ) ;
//                    }
                }
                else
                {
//                    printf( "failed to open\r\n" ) ;
//                    // Send message to switch to main screen
//                    WGT_SetCurrentScreen( &ScrMain ) ;
                    gs_dwSlideIndex=0 ;
                }
            }
        break ;

        case WGT_MSG_POINTER_RELEASED : // cursor released
        case WGT_MSG_KEY_RELEASED : // button released
            // Send message to switch to main screen
            WGT_SetCurrentScreen( &ScrMain ) ;
        break ;

        case WGT_MSG_KEY_PRESSED : // button pressed
            switch ( pMsg->dwParam1 )
            {
                case WGT_KEY_S1 :
                case WGT_KEY_VR1 :
                    if ( pMsg->dwParam2 > 255 )
                    {
                        pMsg->dwParam2=255 ;
                    }
//                    printf( "ScrPPT - pot %u %u %u\r\n", pMsg->dwParam2, pMsg->dwParam2/7, ((pMsg->dwParam2*7)/256) ) ;
                    gs_dwSwitchDelay=SCR_PPT_SLIDE_DELAY_MIN+((pMsg->dwParam2*(SCR_PPT_SLIDE_DELAY_MAX-SCR_PPT_SLIDE_DELAY_MIN+1))/255) ;
                    printf( "pot value %u %u\r\n", pMsg->dwParam2, gs_dwSwitchDelay ) ;
                break ;
            }
        break ;
    }

    return SAMGUI_E_OK ;
}

extern SWGTScreen ScrPPT=
{
    .ProcessMessage=ScrPPT_ProcessMessage,

    /* Hooks */
    .HkBeforePaint=NULL,
    .HkAfterPaint=NULL,
    .HkBeforeEraseBackground=NULL,
    .HkAfterEraseBackground=NULL,
    .HkExit=NULL,

    /* Callbacks */
    .OnInitialize=ScrPPT_OnInitialize,
    .OnEraseBackground=ScrPPT_OnDrawBackground,
    .OnPaint=NULL
} ;

/** @} */
/** @} */
