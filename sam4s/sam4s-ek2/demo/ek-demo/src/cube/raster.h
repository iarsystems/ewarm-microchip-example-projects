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
//------------------------------------------------------------------------------
///
/// \unit
///
/// !Purpose
///
/// APIs defined in this header provide a basis for projecting 3d object to 2d buffer.
///
/// !!Descriptions:
///
/// Projecting a 3d object to 2d plane, especially when the plane overlap or is parallel
/// with x-y plane,  just need to discard the z component for each point of the object.
///
/// !APIs
///  update_faces is used to optimize display by filtering hidden faces.
///
///  draw_image_raster_bmp is used to draw the image inside the face.
///
///  draw_image_raster is used to draw the outside frame of a face .
///
///  SEND_RASTER_BUF is a platform dependent implementation for lcd display.
///
///

#ifndef RASTER_H
#define RASTER_H

#define BUFFER_WIDTH 8
#if BUFFER_WIDTH == 8
#define BUFFER_TYPE char
#else
#define BUFFER_TYPE short
#endif

//------------------------------------------------------------------------------
/// The top faces,z component of which is bigger than zero, should be visible.
/// Here the original point of used coordinate system overlaps with the center of
/// a cube.
//------------------------------------------------------------------------------
extern void update_faces( void ) ;

//------------------------------------------------------------------------------
/// Draw the image inside a face
/// \param myobj  the 3d object
/// \param color  the color in monochrome mode
/// \param RASTER the 2D buffer corresponding to lcd screen
//------------------------------------------------------------------------------
extern void draw_image_raster_bmp( OBJ3D_C8_FFIX *myobj, int color, BUFFER_TYPE* RASTER ) ;

//------------------------------------------------------------------------------
/// Draw the side frame of a face.
/// \param myobj  the 3d object, the square of a face in a cube
/// \param color  the color in monochrome mode
/// \param RASTER the 2D buffer corresponding to lcd screen
//------------------------------------------------------------------------------
extern void draw_image_raster( OBJ3D_C8_FFIX *myobj, int color, BUFFER_TYPE* RASTER ) ;

//------------------------------------------------------------------------------
/// Flush the buffer to display on lcd
/// \param pBuffer  the pointer refered to LCD
/// \param RASTER the 2D buffer corresponding to lcd screen
//------------------------------------------------------------------------------
extern void SEND_RASTER_BUF( void* pBuffer,BUFFER_TYPE* RASTER ) ;

//------------------------------------------------------------------------------
/// Reset the buffer prepared for next frame.
/// \param RASTER the 2D buffer corresponding to lcd screen
//------------------------------------------------------------------------------
extern void reset_Raster( BUFFER_TYPE* RASTER ) ;

#endif //RASTER_H
