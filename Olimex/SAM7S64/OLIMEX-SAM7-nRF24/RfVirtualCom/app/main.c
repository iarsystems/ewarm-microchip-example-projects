/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2008
 *
 *    File name   : main.c
 *    Description : Main module
 *
 *    History :
 *    1. Date        : 17, April 2008
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *   This example project shows how to use the IAR Embedded Workbench
 *  for ARM to develop code for the Olimex SAM7-nRF24 board.
 *   It implements wireless point to point USB CDC (Communication Device Class)
 *  device and install it like a Virtual COM port.
 *   The connection realize between device with Node A configuration and
 *  Node B configuration.
 *
 *    $Revision: 10 $
 **************************************************************************/

#include "includes.h"

#define RING_BUFFER_SIZE  150

Int32U CriticalSecCntr;

RingBuffer_t RfRxRingBuffer;
RingBuffer_t RfTxRingBuffer;
Int8U RxBuff[RING_BUFFER_SIZE];
Int8U TxBuff[RING_BUFFER_SIZE];

extern void RfInit(void);
extern void StartRx(void);

/*************************************************************************
 * Function Name: main
 * Parameters: none
 *
 * Return: none
 *
 * Description: main
 *
 *************************************************************************/
void main(void)
{
Int32U Size;
pInt8U pBuffer;
Boolean bComm;

  __disable_interrupt();

  // RF init
  RfInit();

  // Init LED Output
  AT91F_PIO_CfgOutput(AT91C_BASE_PIOA,AT91C_PIO_PA18);

  // CDC USB
  UsbCdcInit();

  // Soft connection enable
  USB_ConnectRes(TRUE);

  // Init Ring Buffers
  RingBuff_Init(&RfRxRingBuffer,RxBuff,RING_BUFFER_SIZE);
  RingBuff_Init(&RfTxRingBuffer,TxBuff,RING_BUFFER_SIZE);

  AT91F_AIC_EnableIt(AT91C_BASE_AIC,AT91C_ID_SYS);
  __enable_interrupt();

#ifdef NRF24_SELF_TEST
  Boolean nRF24_TestFault = FALSE;
  Int8U channel_tmp, channel;
  RF24L_CE_L();
  channel_tmp = NrfGetChannel();
  channel = (channel_tmp + 1) & 0x7F;
  NrfChannel(channel);
  if(channel != NrfGetChannel())
  {
    nRF24_TestFault = TRUE;
  }
  NrfChannel(channel_tmp);
#endif // NRF24_SELF_TEST

  while(1)
  {
    if (IsUsbCdcConfigure())
    {
      bComm = FALSE;
      // Get free size of the buffer
      pBuffer = RingBuff_GetFreeLinBuff(&RfTxRingBuffer,&Size);
      if(Size)
      {
        // Read data from USB
        Size = UsbCdcRead(pBuffer,Size);
        if(Size)
        {
        #ifdef NRF24_SELF_TEST
          if(nRF24_TestFault)
          {
            nRF24_TestFault = FALSE;
            // Send data from radio
            LED_ON();
            bComm = TRUE;
            UsbCdcWrite("Communication with nRF24L01 fault\n",
                        sizeof("Communication with nRF24L01 fault\n"));
          }
        #endif // NRF24_SELF_TEST
          LED_ON();
          bComm = TRUE;
        }
        RingBuff_Allocate(&RfTxRingBuffer,Size);
      }

      // Get filled size of the buffer
      pBuffer = RingBuff_GetUsedLinBuff(&RfRxRingBuffer,&Size);
      if(Size)
      {
        // Send data from radio
        LED_ON();
        bComm = TRUE;
        UsbCdcWrite(pBuffer,Size);
        RingBuff_Free(&RfRxRingBuffer,Size);
      }

      if (!bComm)
      {
        LED_OFF();
      }
    }
    else
    {
      LED_OFF();
    }
  }
}
