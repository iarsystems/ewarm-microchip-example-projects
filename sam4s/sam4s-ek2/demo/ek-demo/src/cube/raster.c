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
/// \unit
///
/// !Summary
///
/// This file contains algorithms to project 3d object to 2d buffer.
//------------------------------------------------------------------------------
//         Headers
//------------------------------------------------------------------------------
#include "board.h"
#include "conf_3Dcube.h"
#include "3dengine.h"
#include "cube/raster.h"
#include "sam-gui/wgt/core/wgt_core.h"
#include <string.h>

//------------------------------------------------------------------------------
//         Variables
//------------------------------------------------------------------------------
int Face_to_prints[3];                    //!< Printed Face Declaration
int Number_Face_to_print = 0 ;            //!< Number of Face to Display
extern unsigned char mycolor;             //!< Current mycolor variable
extern OBJ3D_C8_FFIX tmpcenters;          //!< Barycenter current pointer

int top_face = 0;


extern void update_faces( void )
{
  // Here we considere on z coordinate of tempcenters object.
  #define zbuffer ((signed char *)&tmpcenters.ob[FACE_NUMBER*2 + 0])

  int Faces[6] = {0, 1, 2, 3, 4, 5};

  int swap, temp, i;
  do
  {
    swap = 0;
    for (i = 0; i < 5; i++)
      // If Next Buffer to display has an z value higher than current one, swap
      //current z with new z face value and stop
      if (zbuffer[Faces[i]] > zbuffer[Faces[i+1]])
      {
        temp = Faces[i];
        Faces[i] = Faces[i+1];
        Faces[i+1] = temp;
        swap = 1;
      }
  }
  while (swap);

  Number_Face_to_print = 0;
  // Display the 3 first one faces
  for (i = 0; i < 3; i++)
  {
    Face_to_prints[i] = Faces[5-i];
    if (zbuffer[Face_to_prints[i]]) Number_Face_to_print++;
    else break;
  }

}



extern void draw_image_raster_bmp( OBJ3D_C8_FFIX *myobj, int color, BUFFER_TYPE* RASTER )
{
    int i ;

    if ( myobj->lien == NULL )
    {
        for ( i=0 ; i < myobj->nbliens ; i++ )
        {
            RASTER[(CANVAS_WIDTH/2-myobj->ob[i]) + (CANVAS_HEIGHT/2-myobj->ob[i+myobj->nbpoints])*CANVAS_WIDTH] = color;
        }
    }
    else
    {
        for(i=0; i<myobj->nbliens;i++)
        {
            RASTER[(CANVAS_WIDTH/2-myobj->ob[i]) + (CANVAS_HEIGHT/2-myobj->ob[i+myobj->nbpoints])*CANVAS_WIDTH] = myobj->lien[i] ;
        }
    }
}

static void my_put_line_raster( int x0, int y0, int x1, int y1, unsigned char color, BUFFER_TYPE* RASTER )
{
    int dy = y1 - y0;
    int dx = x1 - x0;
    int stepx, stepy;

    if (dy < 0) { dy = -dy;  stepy = -1; } else { stepy = 1; }
    if (dx < 0) { dx = -dx;  stepx = -1; } else { stepx = 1; }
    dy <<= 1;
    dx <<= 1;

    if( (x0)+(y0)*CANVAS_HEIGHT < 128*128)
    {
      RASTER[(x0)+(y0)*CANVAS_HEIGHT]=color ;
    }

    if (dx > dy)
    {
        int fraction = dy - (dx >> 1);

        while (x0 != x1)
        {
            if (fraction >= 0)
            {
                y0 += stepy;
                fraction -= dx;
            }

            x0 += stepx;
            fraction += dy;

            if ( (x0)+(y0)*CANVAS_HEIGHT < 128*128 )
            {
                RASTER[(x0)+(y0)*CANVAS_HEIGHT]=color;
            }
        }
    }
    else
    {
        int fraction = dx - (dy >> 1);

        while ( y0 != y1 )
        {
            if ( fraction >= 0 )
            {
                x0 += stepx;
                fraction -= dy;
            }
            y0 += stepy;
            fraction += dx;

            if( (x0)+(y0)*CANVAS_HEIGHT < 128*128 )
            {
                RASTER[(x0)+(y0)*CANVAS_HEIGHT]=color;
            }
        }
    }
}

void draw_image_raster(OBJ3D_C8_FFIX *myobj, int color, BUFFER_TYPE* RASTER)
{
  int i ;
  int x1, y1, x2, y2 ;

  for ( i=0 ; i < myobj->nbliens ; i++ )
  {
    x1=(unsigned char)(CANVAS_WIDTH/2-((int)(myobj->ob[myobj->lien[2*i]])));
    y1=(unsigned char)(CANVAS_HEIGHT/2-((int)(myobj->ob[myobj->lien[2*i]+ myobj->nbpoints])));
    x2=(unsigned char)(CANVAS_WIDTH/2-((int)(myobj->ob[myobj->lien[2*i+1]])));
    y2=(unsigned char)(CANVAS_HEIGHT/2-((int)(myobj->ob[myobj->lien[2*i+1]+myobj->nbpoints])));
    my_put_line_raster(x1, y1, x2, y2, color, RASTER);
  }
}


void SEND_RASTER_BUF( void *pBuffer, BUFFER_TYPE* RASTER )
{
   int i ;
   unsigned short color ;
#if 0
   unsigned char red_bits = 0x3;
   unsigned char green_bits = 0x3;
   unsigned char blue_bits = 0x2;
   unsigned char red_mask = 0xE0;
   unsigned char green_mask = 0x1C;
   unsigned char blue_mask = 0x03;
#else
#    define red_bits         0x3
#    define green_bits       0x3
#    define blue_bits        0x2
#    define red_mask         0xe0
#    define green_mask       0x1c
#    define blue_mask        0x03
#endif
   unsigned short red = 0x0;
   unsigned short green = 0x0;
   unsigned short blue = 0x0;
   SGUIColor clr ;
   int iRow ;
   int iCol ;
   int iOffsetX ;
   int iOffsetY ;


#ifdef BOARD_LCD_HX8347
   LCD_SetWindow(pBuffer, 54,94,185,225);
   LCD_WriteRAM_Prepare(pBuffer);
#endif

#if 0
   for( i=0 ; i < CANVAS_WIDTH*CANVAS_HEIGHT ; i++ )
   {
   //rrrgggbb to rrrrrggggggbbbbb
#if  BUFFER_WIDTH == 8
     red = (RASTER[i]&red_mask)>>5; //RED
     green = (RASTER[i]&green_mask)>>2; //GREEN
     blue = (RASTER[i]&blue_mask); //BLUE
#endif
     red = (red << 5)>>red_bits; //new red component
     green = (green << 6)>>green_bits;
     blue = (blue << 5)>> blue_bits;
     color = (((red << 11)&0xF800) |((green<<5)&0x07E0)|((blue)&0x001F));
#ifdef BOARD_LCD_HX8347
     LCD_WriteRAM(pBuffer, color);
#endif
   }
#else
    for ( iRow=0 ; iRow < CANVAS_HEIGHT ; iRow++ )
    {
        for ( iCol=0 ; iCol < CANVAS_WIDTH ; iCol++ )
        {
            i=(iRow*CANVAS_WIDTH)+iCol ;

            // rrrgggbb to rrrrrggggggbbbbb
#if  BUFFER_WIDTH == 8
            red = (RASTER[i]&red_mask)>>5; //RED
            green = (RASTER[i]&green_mask)>>2; //GREEN
            blue = (RASTER[i]&blue_mask); //BLUE
#endif
            red = (red << 5)>>red_bits; //new red component
            green = (green << 6)>>green_bits;
            blue = (blue << 5)>> blue_bits;
            color = (((red << 11)&0xF800) |((green<<5)&0x07E0)|((blue)&0x001F));

            clr.u.dwRGBA=color ;
            iOffsetX=(BOARD_LCD_WIDTH-CANVAS_WIDTH)>>1 ;
            iOffsetY=(BOARD_LCD_HEIGHT-CANVAS_HEIGHT)>>1 ;

            g_WGT_CoreData.pBE->DrawPixel( iOffsetX+iCol, iOffsetY+iRow, &clr ) ;
        }
   }
#endif
}

extern void reset_Raster( BUFFER_TYPE *RASTER )
{
    if ( sizeof( BUFFER_TYPE ) == 2 )
    {
        int i ;
        for ( i=0 ; i < CANVAS_WIDTH*CANVAS_HEIGHT ; i++ )
        {
            RASTER[i]=0xffff ;
        }
    }
    else
    {
        if ( sizeof( BUFFER_TYPE )== 1 )
        {
            int i ;

            for ( i=0 ; i < CANVAS_WIDTH*CANVAS_HEIGHT ; i++ )
            {
                RASTER[i]=0xff ;
            }
        }
    }
}
