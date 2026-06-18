/*************************************************************************
 *
 *    Used with ARM IAR C/C++ Compiler.
 *
 *    (c) Copyright IAR Systems 2007
 *
 *    File name      : ring_buffer.h
 *    Description    : Ring buffer manager module include file
 *
 *    History :
 *    1. Date        : December 20, 2007
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 10 $
 **************************************************************************/
#include "includes.h"

#ifndef __RING_BUFFER_H
#define __RING_BUFFER_H

#ifdef RING_BUFFER_GLOBAL
#define RING_BUFFER_EXTERN
#else
#define RING_BUFFER_EXTERN extern
#endif

typedef struct _RingBuffer_t
{
  pInt8U  pPush;
  pInt8U  pPop;
  Int32U  Count;
  pInt8U  pBegin;
  pInt8U  pEnd;
  Int32U  Size;
  Boolean LockUsed;
  Boolean LockFree;
} RingBuffer_t, *pRingBuffer_t;

/*************************************************************************
 * Function Name: RingBuff_Init
 * Parameters: pRingBuffer_t pRB, pInt8U pBuffer, Int32U Size
 *
 * Return: none
 *
 * Description: Init the ring buffer structure
 *
 *************************************************************************/
void RingBuff_Init (pRingBuffer_t pRB, pInt8U pBuffer, Int32U Size);

/*************************************************************************
 * Function Name: RingBuff_GetAvableBufferSize
 * Parameters: pRingBuffer_t pRB
 *
 * Return: Int32U
 *
 * Description: Get free space in the buffer
 *
 *************************************************************************/
Int32U RingBuff_GetAvableBufferSize (pRingBuffer_t pRB);

/*************************************************************************
 * Function Name: RingBuff_Push
 * Parameters: pRingBuffer_t pRB, Int8U Data
 *
 * Return: none
 *
 * Description: Push data to a ring buffer
 * Note : Check for free space before calling
 *
 *************************************************************************/
void RingBuff_Push (pRingBuffer_t pRB, Int8U Data);

/*************************************************************************
 * Function Name: RingBuff_Pop
 * Parameters: pRingBuffer_t pRB, pInt8U pData
 *
 * Return: Boolean - FALSE - the ring buffer is empty
 *
 * Description: Pop data from a ring buffer
 *
 *************************************************************************/
Boolean RingBuff_Pop (pRingBuffer_t pRB, pInt8U pData);

/*************************************************************************
 * Function Name: RingBuff_GetFreeLinBuff
 * Parameters: pRingBuffer_t pRB, pInt32U pSize
 *
 * Return: Pointer to linear buffer or NULL 
 *
 * Description: Get a pointer to the empty linear buffer and his size 
 *
 *************************************************************************/
pInt8U RingBuff_GetFreeLinBuff (pRingBuffer_t pRB, pInt32U pSize);

/*************************************************************************
 * Function Name: RingBuff_GetUsedLinBuff
 * Parameters: pRingBuffer_t pRB, pInt32U pSize
 *
 * Return: Pointer to linear buffer or NULL 
 *
 * Description: Get a pointer to the filled linear buffer and his size 
 *
 *************************************************************************/
pInt8U RingBuff_GetUsedLinBuff (pRingBuffer_t pRB, pInt32U pSize);

/*************************************************************************
 * Function Name: RingBuff_Allocate
 * Parameters: pRingBuffer_t pRB, Int32U Size
 *
 * Return: none
 *
 * Description: Allocate "Size" bytes from ring buffer (linear)
 *
 *************************************************************************/
void RingBuff_Allocate (pRingBuffer_t pRB, Int32U Size);

/*************************************************************************
 * Function Name: RingBuff_Free
 * Parameters: pRingBuffer_t pRB, Int32U Size
 *
 * Return: none
 *
 * Description: Free "Size" bytes from ring buffer (linear)
 *
 *************************************************************************/
void RingBuff_Free (pRingBuffer_t pRB, Int32U Size);

#endif // __RING_BUFFER_H

