/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2005
 *
 *    File name   : bmp.h
 *    Description : Driver of ISC1523 Definitions
 *
 *    History :
 *    1. Data        : October, 13 2005
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 10 $
**************************************************************************/
#ifndef __BMP_H
#define __BMP_H

#pragma pack(1)
typedef struct {
  int          NumEntries;
  char         HasTrans;
  const char * pPalEntries;
} GUI_LOGPALETTE;

typedef struct {
  unsigned short XSize;
  unsigned short YSize;
  unsigned short BytesPerLine;
  unsigned short  BitsPerPixel;
  const char * pData;
  const GUI_LOGPALETTE* pPal;
} GUI_BITMAP;
#pragma pack()

extern const GUI_BITMAP bm0;
extern const GUI_BITMAP bm1;
extern const GUI_BITMAP bm2;
extern const GUI_BITMAP bm3;
extern const GUI_BITMAP bm4;
extern const GUI_BITMAP bm6;
extern const GUI_BITMAP bm7;

#endif //__BMP_H
