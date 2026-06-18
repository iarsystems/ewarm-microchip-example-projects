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

/// \unit
///
/// !Purpose
///
/// This file contains 3d engine library structures and apis.
/// 
/// !!!Description
///  
/// Structures and apis defined in this header provide a basis for 3d rotation.
///
/// !!Principles 
/// Rotating with axis(a,b,c) for teta could be expressed in a matrix multiplication.
/// If the orgininal position is (x,y,z) and after rotation the position will be (x',y',z')
/// The relationship of these two coordinate in a specific 3d space is expressed in 
/// below equation:
/// \image rotation3d.png "Rotation in 3D space"
/// And the transofmation matrix is calculated with following matrix equation:
/// \image trasformatrix.png "Transformation matrix computation"
///
/// !!Structures and APIs description
///
///
/// !Stucture
///  OBJ3D_C8_FFIX is the structure to represent a 3d object. 
///  
///  For example, a cube is composed of six faces. Each face could be taken as a square and 
///  a image in it which may seem like following picture.
///  \image logo_s.png "Character image face"  
///  To represent a square, the vertices and links between the neighbor two 
///  vertices should be defined.
///  
///  - A square could be defined like following:
///  \pre
/// const OBJ3D_C8_FFIX of = {.zoom=1.0, .ob=(signed char*)obof1,
/// .lien=(unsigned char*)lkof, .zbuffer=(double*)NULL, 
/// .nbpoints=4, .nbliens=4}
///  \endpre
///  'obof1' is the 4 points for each square and 'lkof' is the 4 links between the neighbor points.
///  \pre
///const char obof1[]={(char)-34,       (char)-34,       +34,     +34,
///                          +34,       (char)-34, (char)-34,     +34,
///                          +34,             +34,       +34,     +34 };
///const unsigned char lkof[]={0,1,1,2,2,3,3,0};
///  \endpre
///  - An image inside the square could be defined like following:
///  \pre
///const OBJ3D_C8_FFIX logosam_S   = {.zoom=1.0, .ob=(signed char*)oblogosam_S, 
///.lien=(unsigned char*)NULL, .zbuffer=(double*)NULL, 
///.nbpoints=1087, .nbliens=1087};
///  \endpre
///  where 'oblogosam_S' is an array to accommodate all the points in 3 dimensions and .lien member is used 
///  to define color for each point.Due to the monochrome mode of the charcater image, 'lien' is empty.
/// 
/// !APIs
///  mrot_C8_FULL_FFIX is used to get the transformation matrix. 
///
///  transforme_C8_FFIX is used to transform the 3d object to new postion.
///  
///  cprodf_FFIX is used to transform a 3 dimensional point to new positon.
///    
///  !Limitations
///  
///  - The absolute value of element in transformation matrix should not be bigger than 255,which is limited 
///  by routine "cprodf_FFIX". The SHIFT_BITS used in the algorithm determine this limitation.
///
///  - The absolute upper limit of rotation axis length should not exceed 0x4000. This is to avoid overflowing during 
///  the computation of tranformation matrix.
/// 
///
///
//------------------------------------------------------------------------------
#ifndef _3DENGINE_H
#define _3DENGINE_H

/// 3D Object Definition
typedef struct {
   float zoom;               //!< zoom factor
   signed char *ob;          //!< points coordinates
   unsigned char *lien;      //!< colors or links for points
   double* zbuffer;          //!< depth in z axis
   unsigned int nbpoints;    //!< point number
   unsigned int nbliens;     //!< color or links number
} OBJ3D_C8_FFIX;


/// Cosine Table Fixed Point
extern const int COSTABLE_STEP_0_050000_QUADPOS[];

/// Sine Table Fixed Point
extern const int SINTABLE_STEP_0_050000_QUADPOS[];


//------------------------------------------------------------------------------
/// Compute tranformaton matrix, a rotation is defined as an angle and an axis.
/// \param m      computed transformation matrix pointer
/// \param teta   rotation angle
/// \param a      x of rotation axis
/// \param b      y of rotation axis
/// \param c      z of rotation axis
//------------------------------------------------------------------------------
extern void mrot_C8_FULL_FFIX(int *m,int teta,int a,int b,int c);

/// Transform each point in *ob to the new position which is stored in *ob2 by  transformation matrix *tsf
/// \param ob   pointer of 3d object which hold the original positions of points
/// \param ob2  pointer of 3d object which should hold the tranformed positons from
///             the original ones 
/// \param tsf  pointer of transformation matrix
extern void transforme_C8_FFIX(OBJ3D_C8_FFIX *ob,OBJ3D_C8_FFIX *ob2,int *tsf);


//------------------------------------------------------------------------------
/// Compute the transformation matrix from the previous rotation axis and angles.
/// The actual rotation angle should be integral multiple of 90 for rotation aixs
/// parallel with coordinate or integral multiple of 180 for rotation axis not parallel
/// with coordinates.
/// \param m    return pointer of transformation matrix  
/// \param teta previous rotation angle
/// \param a    x component of rotation axis
/// \param b    y component of rotation axis 
/// \param c    z component of rotation axis
//------------------------------------------------------------------------------
void Static_Matrix(int *m,int teta,int a,int b,int c);

/// Multiplication of Matrices, should ensure that c1=l2
/// \param m1   matrix 1
/// \param l1   rows in matrix 1
/// \param c1   columns in matrix 1
/// \param m2   matrix 2
/// \param l2   rows in matrix 2
/// \param c2   coulumns in matrix 2
/// \param m3   result matrix of multipication of m1 and m2
///             it's a l1 x c2 matrix 
int cprodf_FFIX(int *m1,int l1,int c1,int *m2,int l2,int c2,int *m3);

#endif //_3DENGINE_H

