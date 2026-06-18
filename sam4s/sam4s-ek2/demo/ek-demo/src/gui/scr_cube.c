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
#include "sam-gui/common/sam_gui_errors.h"
#include "sam-gui/common/sam_gui_colors.h"
#include "sam-gui/porting/sam_gui_porting.h"
#include "cube/3dengine.h"
#include "cube/raster.h"
#include "cube/conf_3Dcube.h"
#include "cube/picture_objects.raw"
#include <stdio.h>
#include "drivers/utility/rand.h"

//------------------------------------------------------------------------------
//         External Variables
//------------------------------------------------------------------------------


#define psram ((BUFFER_TYPE*)0x60000000)
#define byte_addr ((char*)0x60000000)

extern int rotation_axes_cpt;               //!< Rotation Axes Speed Reference
extern int Face_to_prints[3];               //!< Faces Reference
extern int Number_Face_to_print;            //!< Number of Face to Display

extern int top_face;                        //!< The face show on the top
//------------------------------------------------------------------------------
//         Variables
//------------------------------------------------------------------------------

static int Ball_speed=5 ;                          //!< Ball Speed
static volatile unsigned int timestamp=0 ;
/// rotation axis ,updated every time after new measurement of tilt direction
static int rotation_axes_cpt = 10;
static BUFFER_TYPE RASTER[CANVAS_WIDTH*CANVAS_HEIGHT] ;           //!< LCD Display Screen Content
//static unsigned char mycolor=WHITE ;                //!< Current Color

static int i=0 ;
static int teta=0 ;
static int MYTSFMATR[9] ;
static int previous_axis ;

//	screen	Slide_01
//	bitmap	bmp_intro_sam3s_ek	3	40	235	236

static SWGT_Widget g_wgtCube ;
static SWGT_Widget g_btnBack ;

static uint32_t ScrCube_OnInitialize( SWGTScreen* pScreen )
{
    WGT_Screen_Initialize( pScreen, GUICLR_WHITE, NULL ) ;

//    WGT_CreateWidget( &g_wgtCube, WGT_TYPE_BITMAP, 5, 40, 235, 236 ) ;
//    WGT_SetText( &g_wgtCube, "bmp_intro_sam3s_ek" ) ;
//    WGT_SetBitmap( &g_wgtCube, (void*)RASTER ) ;
//    WGT_SetBkgndColor( &g_wgtCube, GUICLR( 248, 222, 84 ) ) ;
//    WGT_Screen_AddWidget( &ScrCube, &g_wgtCube ) ;

    WGT_CreateWidget( &g_btnBack, WGT_TYPE_BUTTON, 12, 252, 30, 56 ) ;
    WGT_SetText( &g_btnBack, "btn_back" ) ;
//    WGT_SetBitmap( &g_btnBack, (void*)aucBtn_back ) ;
    WGT_SetBkgndColor( &g_btnBack, GUICLR( 44, 200, 254 ) ) ;
    WGT_Screen_AddWidget( &ScrCube, &g_btnBack ) ;

    previous_axis=rotation_axes_cpt ;
    reset_Raster( RASTER ) ;
    // First init of the rotation matrix
    mrot_C8_FULL_FFIX(MYTSFMATR, teta, 0x2D42, 0x2D42, 0);

    srand( xTaskGetTickCount() ) ;

    return SAMGUI_E_OK ;
}

static uint32_t ScrCube_OnDrawBackground( SWGTScreen* pScreen )
{
    SGUIColor clr ;

    // Draw Atmel logo
	g_WGT_CoreData.pBE->DrawBitmap( 4, 8, 87, 40, (void*)aucBmp_logo_atmel ) ;

    // Draw labels
    clr.u.dwRGBA=GUICLR_ATMEL_BLUE ;
    g_WGT_CoreData.pBE->DrawText( 105, 8, "SAM3S-EK\n3D demo", &clr, (void*)&g_Font10x14, 0 ) ;

    return SAMGUI_E_OK ;
}

//------------------------------------------------------------------------------
/// Normalize to avoid overflow
/// \param m   matrix or array pointer
/// \param size  size of matrix or array
/// \param norm  the absolute maximum for data in matrix *m
//------------------------------------------------------------------------------
static void Normalize( int *m, char size, int norm )
{
  int i=0;

  for(i=0;i<size && size > 0;i++)
  {
    if(m[i] > norm || m[i] < -norm)
    {
      m[i] = ((m[i]<0)?-norm:norm);
    }
  }
}

 //------------------------------------------------------------------------------
/// Get the transoformation matrix with regard to the axis and angle
/// \param rotate_axis current rotation axis from the status of the accelerometer
/// \param m   matrix or array pointer
/// \param teta  rotation angle rotated
//------------------------------------------------------------------------------
static void MatrixTransform( unsigned int rotate_axis, int* m, int teta )
{
   switch(rotate_axis)
  {
      case 0:
    // m[0] = 0xFF;m[1] = 0;m[2] = 0;
    // m[3] = 0;m[4] = 0xFF;m[5] = 0;
    // m[6] = 0;m[7] = 0;m[8] =0xFF;
      break;
    case 1://y
      mrot_C8_FULL_FFIX(m, teta, 0, 0x4000,0 );
      break;
    case 2://-y
      mrot_C8_FULL_FFIX(m, teta, 0, -0x4000,0 );
      break;
    case 3://x
      mrot_C8_FULL_FFIX(m, teta, 0x4000, 0,0 );
      break;
    case 4://-x
      mrot_C8_FULL_FFIX(m, teta, -0x4000,0,0 );
      break;
    case 5://x,y
      mrot_C8_FULL_FFIX(m, teta, 0x2D42, 0x2D42,0 );
      break;
    case 6://x,-y
      mrot_C8_FULL_FFIX(m, teta, 0x2D42,-0x2D42,0 );
      break;
    case 7://-x,y
      mrot_C8_FULL_FFIX(m, teta,-0x2D42,0x2D42,0 );
      break;
    case 8://-x,-y
      mrot_C8_FULL_FFIX(m, teta,-0x2D42,-0x2D42,0 );
      break;
  }
}

//------------------------------------------------------------------------------
/// make the transition and rotation smoother with regard to previous rotation
/// direction and current status of the accelerometer,
/// \param cur_axis current rotation axis from the status of the accelerometer
/// \param m   matrix or array pointer
/// \param teta  rotation angle rotated
/// \param prev_axis rotation axis before
//------------------------------------------------------------------------------
static void SmoothTransform( int cur_axis, int *m, int teta, int previous_axis )
{
  // The rotation should be stopped
  if(cur_axis == 0)
  {
    //if  it tilts to both x and y ,the cube will stop after 0,180,or 360 making
    // its top or bottom up
    if(teta %120 == 0 && previous_axis >= 5 && previous_axis <= 8)
    {
      //stop at 180 or 360
      Ball_speed = 0;
    }
    //if it tilts to either x or y,the cube will stop after 0,90,180,270 or 360 making
    // its left,right,top or bottom up
    else if(teta % 60 == 0 && previous_axis >= 1 && previous_axis <=4)
    {
      //stop at 0,90,180,360
      Ball_speed = 0;
    }
    //leave the rotation axis unchanged
    rotation_axes_cpt = previous_axis;
  }
  // the rotation should be continued
  else if(cur_axis >=5 && cur_axis <= 8)
  {
    //rotation axis transition happened, smoother when the new rotation starts after
    //its pervious rotation stop at positions where exact a face is on top
    if(previous_axis >= 1 && previous_axis <=4)
    {
      //previous rotating around 1 axis
      if(teta % 120 != 0)  // after rotation 0,180 or 360
      {
        //continue to rotate with previous axis until to position 0,180 or 360a
         rotation_axes_cpt = previous_axis;
      }

    }
  }
  // Get associated transformation matrix
  MatrixTransform(rotation_axes_cpt,m,teta);
}

static void ScrCube_ProcessMove( void )
{
    rotation_axes_cpt=(rand()*10)/65536 ;

    // Ball speed
    Ball_speed=5 ;

    // controlling logic for rotation according to current and previous rotation axis
    SmoothTransform( rotation_axes_cpt, MYTSFMATR, teta, previous_axis ) ;

    // normalize the matrix
    Normalize( MYTSFMATR, 9, 255 ) ;
    previous_axis=rotation_axes_cpt ;

    // Teta Update: Update step between every rotation axes
    // Bigger is the step and Faster is the rotation
    teta+=Ball_speed ;

    if ( teta >= 240 )
    {
      teta=0 ;
    }
    // Display Screen
    SEND_RASTER_BUF( NULL, RASTER ) ;

    // Clear Display Buffer for next compute step
    reset_Raster( RASTER ) ;

    // Compute 3D Transformation with last MATRIX value
    // Input buffer : centers / Output buffer : tmpcenters
    transforme_C8_FFIX( (OBJ3D_C8_FFIX *)&centers, (OBJ3D_C8_FFIX *)&tmpcenters, (int*)MYTSFMATR ) ;

    // Compute zBuffer Transformation in order to optimize display
    update_faces() ;

    for ( i = 0; i < Number_Face_to_print ; i++ )
    {
      // Test if it is a face to display so a Picture
      if ( cube_logosam[Face_to_prints[i]] )
      {
        transforme_C8_FFIX( cube_logosam[Face_to_prints[i]], tmpcube_logosam[Face_to_prints[i]], MYTSFMATR ) ;
        draw_image_raster_bmp( tmpcube_logosam[Face_to_prints[i]], COLOR_DOT, RASTER ) ;
      }

      // Else Display Cube Transformation
      transforme_C8_FFIX( (OBJ3D_C8_FFIX *)&of[Face_to_prints[i]], (OBJ3D_C8_FFIX *)&tmpof[Face_to_prints[i]], (int *)MYTSFMATR ) ;
      draw_image_raster( &tmpof[Face_to_prints[i]], COLOR_LINE, RASTER ) ;
    }
}

static uint32_t ScrCube_ProcessMessage( SWGTScreen* pScreen, SWGTCoreMessage* pMsg )
{
    static uint32_t dwTime=0 ;

//    printf( "ScrCube - msg id %x\r\n", pMsg->dwID ) ;

    switch ( pMsg->dwID )
    {
        case WGT_MSG_INIT :
          WGT_SetTimerPeriod( 1000/25 ) ;
        break ;

        case WGT_MSG_WIDGET_SELECTED :
          if ( pMsg->dwParam1 == (uint32_t)&g_btnBack )
          {
              WGT_SetCurrentScreen( &ScrMain ) ;
              WGT_SetTimerPeriod( 1000 ) ;
          }
        break ;

        case WGT_MSG_TIMER : // Timer triggered
            dwTime++ ;

//            printf( "timer\r\n" ) ;
            ScrCube_ProcessMove() ;
        break ;

        case WGT_MSG_POINTER : // cursor moved
        case WGT_MSG_POINTER_RAW : // cursor moved
        case WGT_MSG_KEY_PRESSED : // button pressed
        case WGT_MSG_KEY_RELEASED : // button released
        break ;
    }

    return SAMGUI_E_OK ;
}

extern SWGTScreen ScrCube=
{
    .ProcessMessage=ScrCube_ProcessMessage,

    .OnInitialize=ScrCube_OnInitialize,
    .OnEraseBackground=ScrCube_OnDrawBackground,
    .OnPaint=NULL
} ;
