/*************************************************************************
 *
 *    Used with ARM IAR C/C++ Compiler.
 *
 *    (c) Copyright IAR Systems 2008
 *
 *    File name      : ring_buffer.c
 *    Description    : Ring buffer manager module
 *
 *    History :
 *    1. Date        : December 20, 2007
 *       Author      : Stanimir Bonev
 *       Description : Create
 *    2. Date        : Aprim 17, 2008
 *       Author      : Stanimir Bonev
 *       Description : Add ARM7,ARM9 and Cortex support
 *
 *    $Revision: 10 $
 **************************************************************************/

#define RING_BUFFER_GLOBAL
#include "ring_buffer.h"

/*************************************************************************
 * Function Name: RingBuff_Init
 * Parameters: pRingBuffer_t pRB, pInt8U pBuffer, Int32U Size
 *
 * Return: none
 *
 * Description: Init the ring buffer structure
 *
 *************************************************************************/
void RingBuff_Init (pRingBuffer_t pRB, pInt8U pBuffer, Int32U Size)
{
#if __CORE__ < 7
Int32U Save;
#endif // __CORE__ < 7
  assert(pRB);                    // parameters check
  assert(pBuffer);
  assert(Size);
#if __CORE__ < 7
  ENTR_CRT_SECTION(Save);
#else
  ENTR_CRT_SECTION();
#endif // __CORE__ < 7
  pRB->pBegin   = \
  pRB->pPop     = \
  pRB->pPush    = pBuffer;
  // calculate buffer boundary
  pRB->pEnd     = pBuffer + Size;
  pRB->Size     = Size;           // hold buffer size
  pRB->Count    = 0;              // number of used bytes from the buffer
  pRB->LockUsed = \
  pRB->LockFree = FALSE;          // unlock ring buffer
#if __CORE__ < 7
  EXT_CRT_SECTION(Save);
#else
  EXT_CRT_SECTION();
#endif // __CORE__ < 7
}

/*************************************************************************
 * Function Name: RingBuff_GetAvableBufferSize
 * Parameters: pRingBuffer_t pRB
 *
 * Return: Int32U
 *
 * Description: Get free space in the buffer
 *
 *************************************************************************/
Int32U RingBuff_GetAvableBufferSize (pRingBuffer_t pRB)
{
  assert(pRB);  // parameters check
  return((pRB->LockFree)?0:(pRB->Size - pRB->Count));
}

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
void RingBuff_Push (pRingBuffer_t pRB, Int8U Data)
{
#if __CORE__ < 7
Int32U Save;
#endif // __CORE__ < 7
  assert(pRB);                    // parameters check
  assert(pRB->Size - pRB->Count); // Access right check
  assert(pRB->LockFree);
#if __CORE__ < 7
  ENTR_CRT_SECTION(Save);
#else
  ENTR_CRT_SECTION();
#endif // __CORE__ < 7
  ++pRB->Count;                   // update number of used byte from the buffer
#if __CORE__ < 7
  EXT_CRT_SECTION(Save);
#else
  EXT_CRT_SECTION();
#endif // __CORE__ < 7
  *pRB->pPush = Data;             // store data
  if (++pRB->pPush >= pRB->pEnd)  // update push pointer
  {
    pRB->pPush = pRB->pBegin;
  }
}

/*************************************************************************
 * Function Name: RingBuff_Pop
 * Parameters: pRingBuffer_t pRB, pInt8U pData
 *
 * Return: Boolean - FALSE - the ring buffer is empty
 *
 * Description: Pop data from a ring buffer
 *
 *************************************************************************/
Boolean RingBuff_Pop (pRingBuffer_t pRB, pInt8U pData)
{
#if __CORE__ < 7
Int32U Save;
#endif // __CORE__ < 7
  assert(pRB);                  // parameters check
  assert(pData);
  if (   !pRB->Count
      ||  pRB->LockUsed)
  {
    return(FALSE);              // buffer empty or locked
  }
#if __CORE__ < 7
  ENTR_CRT_SECTION(Save);
#else
  ENTR_CRT_SECTION();
#endif // __CORE__ < 7
  --pRB->Count;                 // update counter of used bytes from the buffer
#if __CORE__ < 7
  EXT_CRT_SECTION(Save);
#else
  EXT_CRT_SECTION();
#endif // __CORE__ < 7
  *pData = *pRB->pPop;          // get data
  if (++pRB->pPop >= pRB->pEnd) // update pop pointer
  {
    pRB->pPop = pRB->pBegin;
  }
  return(TRUE);
}

/*************************************************************************
 * Function Name: RingBuff_GetFreeLinBuff
 * Parameters: pRingBuffer_t pRB, pInt32U pSize
 *
 * Return: Pointer to linear buffer or NULL
 *
 * Description: Get a pointer to the empty linear buffer and his size
 *
 *************************************************************************/
pInt8U RingBuff_GetFreeLinBuff (pRingBuffer_t pRB, pInt32U pSize)
{
#if __CORE__ < 7
Int32U Save;
#endif // __CORE__ < 7
Int32U Pop;
  assert(pRB);        // parameters check
  assert(pSize);
  assert(!pRB->LockFree);
#if __CORE__ < 7
  ENTR_CRT_SECTION(Save);
#else
  ENTR_CRT_SECTION();
#endif // __CORE__ < 7
  if (pRB->Count == pRB->Size)
  {
    // buffer full
  #if __CORE__ < 7
    EXT_CRT_SECTION(Save);
  #else
    EXT_CRT_SECTION();
  #endif // __CORE__ < 7
    *pSize = 0;
    return(NULL);
  }
  pRB->LockFree = TRUE; // lock buffer for pushing
  Pop = (Int32U)pRB->pPop;
#if __CORE__ < 7
  EXT_CRT_SECTION(Save);
#else
  EXT_CRT_SECTION();
#endif // __CORE__ < 7

  // calculate available free linear buffer
  if ((Int32U)pRB->pPush >= Pop)
  {
    *pSize = (Int32U)pRB->pEnd - (Int32U)pRB->pPush;
  }
  else
  {
    *pSize = Pop - (Int32U)pRB->pPush;
  }
  return(pRB->pPush);
}

/*************************************************************************
 * Function Name: RingBuff_GetUsedLinBuff
 * Parameters: pRingBuffer_t pRB, pInt32U pSize
 *
 * Return: Pointer to linear buffer or NULL
 *
 * Description: Get a pointer to the filled linear buffer and his size
 *
 *************************************************************************/
pInt8U RingBuff_GetUsedLinBuff (pRingBuffer_t pRB, pInt32U pSize)
{
#if __CORE__ < 7
Int32U Save;
#endif // __CORE__ < 7
Int32U Push;
  assert(pRB);          // parameters check
  assert(pSize);
  assert(!pRB->LockUsed);
#if __CORE__ < 7
  ENTR_CRT_SECTION(Save);
#else
  ENTR_CRT_SECTION();
#endif // __CORE__ < 7
  if (pRB->Count == 0)
  {
    // buffer empty
  #if __CORE__ < 7
    EXT_CRT_SECTION(Save);
  #else
    EXT_CRT_SECTION();
  #endif // __CORE__ < 7
    *pSize = 0;
    return(NULL);
  }
  pRB->LockUsed = TRUE; // lock buffer for popping
  Push = (Int32U)pRB->pPush;
#if __CORE__ < 7
  EXT_CRT_SECTION(Save);
#else
  EXT_CRT_SECTION();
#endif // __CORE__ < 7

  // calculate available filled linear buffer
  if ((Int32U)pRB->pPop >= Push)
  {
    *pSize = (Int32U)pRB->pEnd - (Int32U)pRB->pPop;
  }
  else
  {
    *pSize = Push - (Int32U)pRB->pPop;
  }
  return(pRB->pPop);
}

/*************************************************************************
 * Function Name: RingBuff_Allocate
 * Parameters: pRingBuffer_t pRB, Int32U Size
 *
 * Return: none
 *
 * Description: Allocate "Size" bytes from ring buffer (linear)
 *
 *************************************************************************/
void RingBuff_Allocate (pRingBuffer_t pRB, Int32U Size)
{
#if __CORE__ < 7
Int32U Save;
#endif // __CORE__ < 7
  assert(pRB);            // parameters check
  assert(pRB->LockFree);

#if __CORE__ < 7
  ENTR_CRT_SECTION(Save);
#else
  ENTR_CRT_SECTION();
#endif // __CORE__ < 7
  // Update filled counter
  pRB->Count += Size;

  // Update push pointer
  pRB->pPush += Size;
  if (pRB->pPush >= pRB->pEnd)
  {
    pRB->pPush = pRB->pBegin;
  }
#if __CORE__ < 7
  EXT_CRT_SECTION(Save);
#else
  EXT_CRT_SECTION();
#endif // __CORE__ < 7

  // Unlock buffer
  pRB->LockFree = FALSE;
}

/*************************************************************************
 * Function Name: RingBuff_Free
 * Parameters: pRingBuffer_t pRB, Int32U Size
 *
 * Return: none
 *
 * Description: Free "Size" bytes from ring buffer (linear)
 *
 *************************************************************************/
void RingBuff_Free (pRingBuffer_t pRB, Int32U Size)
{
#if __CORE__ < 7
Int32U Save;
#endif // __CORE__ < 7
  assert(pRB);            // parameters check
  assert(pRB->LockUsed);

#if __CORE__ < 7
  ENTR_CRT_SECTION(Save);
#else
  ENTR_CRT_SECTION();
#endif // __CORE__ < 7
  // Update filled counter
  pRB->Count -= Size;
  // Update pop pointer
  pRB->pPop += Size;

  if (pRB->pPop == pRB->pEnd)
  {
    pRB->pPop = pRB->pBegin;
  }
#if __CORE__ < 7
  EXT_CRT_SECTION(Save);
#else
  EXT_CRT_SECTION();
#endif // __CORE__ < 7

  // Unlock buffer
  pRB->LockUsed = FALSE;
}

