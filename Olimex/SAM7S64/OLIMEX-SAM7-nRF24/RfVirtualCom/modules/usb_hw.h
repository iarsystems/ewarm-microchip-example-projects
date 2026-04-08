/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2007
 *
 *    File name   : usb_hw.h
 *    Description : usb module include file
 *
 *    History :
 *    1. Date        : Octomber 5, 2007
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 10 $
 **************************************************************************/

#include "includes.h"

#ifndef __USB_HW_H
#define __USB_HW_H

#ifdef USB_HW_GLOBAL
#define USB_HW_EXTERN
#else
#define USB_HW_EXTERN  extern
#endif

#define IS_EP_DOUBLE_BUFF(EP)  (  (EP == ENP1_OUT)\
                                ||(EP == ENP1_IN) \
                                ||(EP == ENP2_OUT)\
                                ||(EP == ENP2_IN))

#define USB_REMOTE_WAKE_UP_DLY()  dly100us(30);     //  delay 3 ms (from 1 to 10 ms)

#if SELF_POWERED != 0
#define IS_DEVICE_ATTCHED() (AT91F_PIO_IsInputSet(AT91C_BASE_PIOA,USB_D_MASK))
#endif // SELF_POWERED != 0

#define USB_EP_VALID(pEP) (pEP->pEpCtrl != NULL)

#define USB_EP_CTRL_MAX_SETUP_DLY 100

typedef enum _USB_PacketType_t
{
	// Packet type parameters
  UsbSetupPacket = 0,UsbDataOutPacket,UsbDataInPacket,UsbDmaPacket,
} USB_PacketType_t;

typedef enum _USB_ErrorCodes_t
{
  USB_OK = 0,USB_PLL_ERROR, USB_INTR_ERROR,
  USB_EP_OCCUPIER, USB_MEMORY_FULL, USB_BUF_OVERFLOW,
  USB_EP_NOT_VALID, UB_EP_SETUP_UNDERRUN, USB_EP_STALLED,
  UB_EP_SETUP_OVERWRITE, USB_EP_FATAL_ERROR,
} USB_ErrorCodes_t;

typedef enum _EpType_t
{
  EP_CTRL = 0, EP_ISO, EP_BULK, EP_INTERRUPT,
} EpType_t;

typedef enum _EpState_t
{
  EP_DISABLED = 0, EP_STALL, EP_NAK, EP_VALID
} EpState_t;

typedef enum _USB_Endpoint_t
{
  CTRL_ENP_OUT=0, CTRL_ENP_IN,
  ENP1_OUT      , ENP1_IN    ,
  ENP2_OUT      , ENP2_IN    ,
  ENP3_OUT      , ENP3_IN    ,
  ENP_MAX_NUMB
} USB_Endpoint_t;

typedef enum _USB_DevStatusReqType_t
{
  USB_DevConnectStatus = 0, USB_AddrStateStatus, USB_CnfgStateStatus
} USB_DevStatusReqType_t;

typedef enum _UsbResumeEvent_t
{
  USB_RESUME_SOF_EVENT = 0, USB_RESUME_SOFT_EVENT,
  USB_RESUME_WAKE_UP_EVENT,
} UsbResumeEvent_t, *pUsbResumeEvent_t;

typedef enum _USB_IO_Status_t
{
  NOT_READY = 0, NO_SERVICED, BEGIN_SERVICED, COMPLETE, BUFFER_UNDERRUN, BUFFER_OVERRUN,
  SETUP_OVERWRITE, STALLED, NOT_VALID
} USB_IO_Status_t;

typedef struct _UsbEP_ExtData_t
{
  Boolean    DoubleBuff;
} UsbEP_ExtData_t, *pUsbEP_ExtData_t;

typedef struct _EpCnfg_t
{
  volatile pInt32U    pEpCtrl;
  Int32U              Mask;
  Int32U              MaxSize;
  UsbEpTransferType_t EpType;
  void *              pFn;
  Int32U              Offset;
  Int32U              Size;
  USB_IO_Status_t     Status;
  pInt8U              pBuffer;
  Int8U               AvbBuff;
  union
  {
    Int8U Flags;
    struct
    {
      Int8U bDoubleBuffered     : 1;
      Int8U bZeroPacket         : 1;
      Int8U bZeroPacketPossible : 1;
      Int8U bBuffer             : 1;
      Int8U bBufferCntrUpdate   : 1;
    };
  };
} EpCnfg_t, *pEpCnfg_t;

typedef Int32U UsbDefStatus_t;
typedef void * (* UserFunc_t)(void * Arg);

USB_HW_EXTERN EpCnfg_t EpCnfg[ENP_MAX_NUMB];

/*************************************************************************
 * Function Name: USB_HwInit
 * Parameters: none
 *
 * Return: none
 *
 * Description: Init USB
 *
 *************************************************************************/
void USB_HwInit(void);

/*************************************************************************
 * Function Name: USB_HwReset
 * Parameters: none
 *
 * Return: none
 *
 * Description: Reset USB engine
 *
 *************************************************************************/
void USB_HwReset (void);

/*************************************************************************
 * Function Name: USB_RealizeEp
 * Parameters: const UsbStandardEpDescriptor_t * pEP_Desc,
 *             const UsbEP_ExtData_t * pUsbEP_ExtData, Boolean Enable
 *
 * Return: USB_ErrorCodes_t
 *
 * Description: Enable or disable an endpoint
 *
 *************************************************************************/
USB_ErrorCodes_t USB_RealizeEp(const UsbStandardEpDescriptor_t * pEP_Desc,
                               const UsbEP_ExtData_t * pUsbEP_ExtData,
                               Boolean Enable);

/*************************************************************************
 * Function Name: USB_SetAdd
 * Parameters: Int32U DevAdd - device address between 0 - 127
 *
 * Return: none
 *
 * Description: Set device address
 *
 *************************************************************************/
void USB_SetAdd(Int32U DevAdd);
#define USB_SetDefAdd() USB_SetAdd(0)

/*************************************************************************
 * Function Name: USB_ConnectRes
 * Parameters: Boolean Conn
 *
 * Return: none
 *
 * Description: Enable Pull-Up resistor
 *
 *************************************************************************/
void USB_ConnectRes (Boolean Conn);

/*************************************************************************
 * Function Name: USB_Configure
 * Parameters: Boolean Configure
 *
 * Return: none
 *
 * Description: Configure device
 *  When Configure != 0 enable all Realize Ep
 *
 *************************************************************************/
void USB_Configure (Boolean Configure);

#if USB_REMOTE_WAKEUP != 0
/*************************************************************************
 * Function Name: USB_WakeUp
 * Parameters: none
 *
 * Return: none
 *
 * Description: Wake up USB
 *
 *************************************************************************/
void USB_WakeUp (void);
#endif // USB_REMOTE_WAKEUP != 0

/*************************************************************************
 * Function Name: USB_GetDevStatus
 * Parameters: USB_DevStatusReqType_t Type
 *
 * Return: Boolean
 *
 * Description: Return USB device status
 *
 *************************************************************************/
Boolean USB_GetDevStatus (USB_DevStatusReqType_t Type);

/*************************************************************************
 * Function Name: USB_EpLogToPhysAdd
 * Parameters: Int8U EpLogAdd
 *
 * Return: USB_Endpoint_t
 *
 * Description: Convert the logical to physical address
 *
 *************************************************************************/
USB_Endpoint_t USB_EpLogToPhysAdd (Int8U EpLogAdd);

#if USB_SOF_EVENT > 0
/*************************************************************************
 * Function Name: USB_GetFrameNumb
 * Parameters: none
 *
 * Return: Int32U
 *
 * Description: Return current value of SOF number
 *
 *************************************************************************/
Int32U USB_GetFrameNumb (void);
#endif // USB_SOF_EVENT > 0

/*************************************************************************
 * Function Name: USB_StatusPhase
 * Parameters: Boolean In
 *
 * Return: none
 *
 * Description: Prepare status phase
 *
 *************************************************************************/
void USB_StatusPhase (Boolean In);

/*************************************************************************
 * Function Name: USB_ISR
 * Parameters: none
 *
 * Return: none
 *
 * Description: USB interrupt subroutine
 *
 *************************************************************************/
void USB_ISR (void);

/*************************************************************************
 * Function Name: USB_SetStallEP
 * Parameters: USB_Endpoint_t EndPoint, Boolean Stall
 *
 * Return: USB_ErrorCodes_t
 *
 * Description: The endpoint stall/unstall
 *
 *************************************************************************/
USB_ErrorCodes_t USB_SetStallEP (USB_Endpoint_t EndPoint, Boolean Stall);

/*************************************************************************
 * Function Name: USB_GetStallEP
 * Parameters: USB_Endpoint_t EndPoint, pBoolean pStall
 *
 * Return: USB_ErrorCodes_t
 *
 * Description: Get stall state of the endpoint
 *
 *************************************************************************/
USB_ErrorCodes_t USB_GetStallEP (USB_Endpoint_t EndPoint, pBoolean pStall);

/*************************************************************************
 * Function Name: USB_EP_IO
 * Parameters: USB_Endpoint_t EndPoint
 *
 * Return: none
 *
 * Description: Endpoints IO
 *
 *************************************************************************/
void USB_EP_IO(USB_Endpoint_t EP);

#endif //__USB_HW_H
