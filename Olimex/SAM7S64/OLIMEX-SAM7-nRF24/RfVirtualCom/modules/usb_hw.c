/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2007
 *
 *    File name   : usb_hw.c
 *    Description : usb module (HAL)
 *
 *    History :
 *    1. Date        : Octomber 5, 2007
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 10 $
 **************************************************************************/

#define USB_HW_GLOBAL
#include "usb_hw.h"

Int32U DlyCnt;

static const UsbStandardEpDescriptor_t USB_CtrlEpDescr0 =
{
  sizeof(UsbStandardEpDescriptor_t),
  UsbDescriptorEp,
  UsbEpOut(CTRL_ENP_OUT>>1),
  {(Int8U)UsbEpTransferControl | (Int8U)UsbEpSynchNoSynchronization | (Int8U)UsbEpUsageData},
  Ep0MaxSize,
  0
};

static const UsbEP_ExtData_t USB_CtrlEpExt0 =
{
  IS_EP_DOUBLE_BUFF(CTRL_ENP_OUT)
};

static const UsbStandardEpDescriptor_t USB_CtrlEpDescr1 =
{
  sizeof(UsbStandardEpDescriptor_t),
  UsbDescriptorEp,
  UsbEpIn(CTRL_ENP_IN>>1),
  {(Int8U)UsbEpTransferControl | (Int8U)UsbEpSynchNoSynchronization | (Int8U)UsbEpUsageData},
  Ep0MaxSize,
  0
};

static const UsbEP_ExtData_t USB_CtrlEpExt1 =
{
  IS_EP_DOUBLE_BUFF(CTRL_ENP_IN)
};

/*************************************************************************
 * Function Name: USB_HwInit
 * Parameters: none
 *
 * Return: none
 *
 * Description: Init USB
 *
 *************************************************************************/
void USB_HwInit(void)
{
Int32U reg;

  // Enable GPIO controller
  AT91F_PIOA_CfgPMC();
  // Init Pull up resistor ctrl
  // PIO assign to PIO controller
  AT91F_PIO_Enable(AT91C_BASE_PIOA,USB_PUP_MASK | USB_PUN_MASK);
  // USB disconnect
  USB_ConnectRes(FALSE);
  // disable wake up
  *((USB_PUP_POLARITY)?
    &AT91C_BASE_PIOA->PIO_SODR:
    &AT91C_BASE_PIOA->PIO_CODR) = USB_PUN_MASK;
  // set to output
  AT91F_PIO_OutputEnable(AT91C_BASE_PIOA,USB_PUP_MASK | USB_PUN_MASK);


#if SELF_POWERED != 0
  // Init USB_V detection
  // PIO assign to PIO controller
  AT91F_PIO_Enable(AT91C_BASE_PIOA,USB_D_MASK);
  // Enable glitch filter
  AT91F_PIO_InputFilterEnable(AT91C_BASE_PIOA,USB_D_MASK);
  // disable internal pull-up
  reg = ~USB_D_MASK & AT91F_PIO_GetCfgPullup(AT91C_BASE_PIOA);
  AT91F_PIO_CfgPullup(AT91C_BASE_PIOA,reg);
  // set to input
  AT91F_PIO_OutputDisable(AT91C_BASE_PIOA,USB_D_MASK);
  // USB V detect interrupt init
  AT91F_AIC_ConfigureIt(AT91C_BASE_AIC,
                        AT91C_ID_PIOA,
                        UsbVIntrPriority,
                        AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL,
                        USB_V_ISR);
  // clear beginning interrupt
  volatile Int32U Dummy = AT91F_PIO_GetInterruptStatus(AT91C_BASE_PIOA);
  // Enable interrupt
  AT91F_PIO_InterruptEnable(AT91C_BASE_PIOA,USB_D_MASK);
#endif // SELF_POWERED != 0

  // Enable USB mclk
  AT91F_UDP_CfgPMC();
  // Set USBDIV
  reg = (AT91F_CKGR_GetPLLReg(AT91C_BASE_CKGR) & ~AT91C_CKGR_USBDIV) | USB_DIV;
  AT91F_CKGR_CfgPLLReg(AT91C_BASE_CKGR,reg);
  // Enable 48MHz clock
  AT91F_PMC_CfgSysClkEnableReg(AT91C_BASE_PMC,AT91C_PMC_UDP);
  // Enable transmitter
  AT91F_UDP_EnableTransceiver(AT91C_BASE_UDP);

  AT91C_BASE_UDP->UDP_FADDR = 0;
  AT91C_BASE_UDP->UDP_GLBSTATE = 0;

  // Disable ALL interrupts
  AT91F_UDP_DisableIt(AT91C_BASE_UDP,
                      AT91C_UDP_EPINT0 | AT91C_UDP_EPINT1 | AT91C_UDP_EPINT2 | AT91C_UDP_EPINT3 |
                      AT91C_UDP_RXSUSP | AT91C_UDP_RXRSM  | AT91C_UDP_SOFINT |
                      AT91C_UDP_WAKEUP);

  // Clear All pending interrupt
  AT91F_UDP_InterruptClearRegister(AT91C_BASE_UDP,
                                   AT91C_UDP_RXSUSP | AT91C_UDP_RXRSM  | AT91C_UDP_SOFINT |
                                   AT91C_UDP_WAKEUP);

  // Init controls endpoints
  USB_HwReset();

  // USB interrupt init
  AT91F_AIC_ConfigureIt(AT91C_BASE_AIC,
                        AT91C_ID_UDP,
                        USB_INTR_PRIORITY,
                        AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL,
                        USB_ISR);
#if SELF_POWERED != 0
  if(IS_DEVICE_ATTCHED())
  {
    UsbSetDevState(UsbDevStatusAttached);
    AT91F_AIC_EnableIt(AT91C_BASE_AIC,AT91C_ID_UDP);
  }
  else
  {
    UsbSetDevState(UsbDevStatusUnknow);
    AT91F_AIC_DisableIt(AT91C_BASE_AIC,AT91C_ID_UDP);
  }

  // Enable USB interrupt
  AT91F_AIC_EnableIt(AT91C_BASE_AIC,AT91C_ID_PIOA);
#else
  UsbSetDevState(UsbDevStatusUnknow);
  AT91F_AIC_EnableIt(AT91C_BASE_AIC,AT91C_ID_UDP);
#endif // SELF_POWERED != 0
}

/*************************************************************************
 * Function Name: USB_HwReset
 * Parameters: none
 *
 * Return: none
 *
 * Description: Reset USB engine
 *
 *************************************************************************/
void USB_HwReset (void)
{
Int32U Count;

  // Clear realized EP flag
  for(Count = 0; Count < ENP_MAX_NUMB; Count++)
  {
    EpCnfg[Count].pEpCtrl = NULL;
  }

  // Disable all endpoints
  for(Int32U Count = 0; Count < ENP_MAX_NUMB/2; Count++)
  {
    AT91C_BASE_UDP->UDP_CSR[Count] = 0;
  }
  DLY_EP();
  // Reset
  AT91F_UDP_ResetEp(AT91C_BASE_UDP,0xF);

  // for every case
  AT91F_UDP_CfgPMC();
  AT91F_PMC_CfgSysClkEnableReg( AT91C_BASE_PMC, AT91C_PMC_UDP );
  AT91F_UDP_EnableTransceiver(AT91C_BASE_UDP);

  // USB_Configure
  USB_Configure(FALSE);

  // Control EP Init
  USB_RealizeEp(&USB_CtrlEpDescr0,&USB_CtrlEpExt0,TRUE);
  USB_RealizeEp(&USB_CtrlEpDescr1,&USB_CtrlEpExt1,TRUE);

  // Set address 0
  USB_SetDefAdd();

  // Clear All pending interrupt
  AT91F_UDP_InterruptClearRegister(AT91C_BASE_UDP,
                                   AT91C_UDP_WAKEUP | AT91C_UDP_RXRSM |
                                   AT91C_UDP_RXSUSP);

  // Enable Device interrupts
  AT91F_UDP_EnableIt( AT91C_BASE_UDP,
                      AT91C_UDP_EPINT0 | AT91C_UDP_RXSUSP | AT91C_UDP_RXRSM | AT91C_UDP_WAKEUP |
                     (USB_SOF_EVENT     ?AT91C_UDP_SOFINT:0));
}

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
                               Boolean Enable)
{
Int32U reg;
USB_Endpoint_t EP;
pEpCnfg_t pEP;
volatile pInt32U pEpCtrlBaseAddr;

  assert(pEP_Desc);

  EP = (USB_Endpoint_t)USB_EpLogToPhysAdd(pEP_Desc->bEndpointAddress);
  pEP = &EpCnfg[EP];
  // Disable EP Interrupts
  AT91F_UDP_DisableIt(AT91C_BASE_UDP, pEP->Mask);

  if (Enable)
  {
    assert(pUsbEP_ExtData);

    // Set EP status
    pEP->Status  = NOT_READY;
    // Init EP flags
    pEP->Flags = 0;
    // Set Extra data
    pEP->bDoubleBuffered = pUsbEP_ExtData->DoubleBuff;
    if (EP & 1)
    {
      pEP->AvbBuff = pEP->bDoubleBuffered + 1;
    }
    else
    {
      pEP->AvbBuff = 0;
    }

    pEpCtrlBaseAddr  = (pInt32U)&AT91C_BASE_UDP->UDP_CSR[0];
    pEpCtrlBaseAddr +=  (EP>>1);

    // Set endpoint type
    pEP->EpType = (UsbEpTransferType_t)pEP_Desc->bmAttributes.TransferType;
    pEP->pEpCtrl = pEpCtrlBaseAddr;
    // Init EP max packet size
    pEP->MaxSize = pEP_Desc->wMaxPacketSize;
    // EP mask
    pEP->Mask = 1 << (EP>>1);

    // Disable EP
    *pEP->pEpCtrl = 0;
    // Reset EP
    AT91F_UDP_ResetEp(AT91C_BASE_UDP,pEP->Mask);

    switch (pEP->EpType)
    {
    case UsbEpTransferIsochronous:
      reg = (EP & 1)?(0x4 | EP_ISO) << 8:(EP_ISO) << 8;
      break;
    case UsbEpTransferControl:
      break;
    case UsbEpTransferBulk:
      reg = (EP & 1)?(0x4 | EP_BULK) << 8:(EP_BULK) << 8;
      break;
    case  UsbEpTransferInterrupt:
      reg = (EP & 1)?(0x4 | EP_INTERRUPT) << 8:(EP_INTERRUPT) << 8;
      break;
    default:  // reserved EP type
      assert(0);
    }
    *pEpCtrlBaseAddr = reg;
    DLY_EP();

    // Enable EP
    AT91F_UDP_EnableEp(AT91C_BASE_UDP,EP>>1);
  }
  else
  {
    if (pEP->pEpCtrl)
    {
      pEP->Status   = NOT_READY;
      // Disable EP
      *pEP->pEpCtrl = 0;
      pEP->pEpCtrl  = NULL;
    }
  }
  return(USB_OK);
}

/*************************************************************************
 * Function Name: USB_SetAdd
 * Parameters: Int32U DevAdd - device address between 0 - 127
 *
 * Return: none
 *
 * Description: Set device address
 *
 *************************************************************************/
void USB_SetAdd(Int32U DevAdd)
{
  AT91F_UDP_SetAddress(AT91C_BASE_UDP,DevAdd);
  if(DevAdd)
  {
    AT91C_BASE_UDP->UDP_GLBSTATE |=  AT91C_UDP_FADDEN;
  }
  else
  {
    AT91C_BASE_UDP->UDP_GLBSTATE &= ~AT91C_UDP_FADDEN;
  }
}

/*************************************************************************
 * Function Name: USB_ConnectRes
 * Parameters: Boolean Conn
 *
 * Return: none
 *
 * Description: Enable Pull-Up resistor
 *
 *************************************************************************/
void USB_ConnectRes (Boolean Conn)
{
  *(((Conn == TRUE) ^ USB_PUP_POLARITY)?
    &AT91C_BASE_PIOA->PIO_SODR:
    &AT91C_BASE_PIOA->PIO_CODR) = USB_PUP_MASK;
}

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
void USB_Configure (Boolean Configure)
{
  if (Configure)
  {
    AT91C_BASE_UDP->UDP_GLBSTATE |=  AT91C_UDP_CONFG;
  }
  else
  {
    AT91C_BASE_UDP->UDP_GLBSTATE &= ~AT91C_UDP_CONFG;
  }
}

#if USB_REMOTE_WAKEUP != 0
/*************************************************************************
 * Function Name: USB_WakeUp
 * Parameters: none
 *
 * Return: none
 *
 * Description: Wake up Usb
 *
 *************************************************************************/
void USB_WakeUp (void)
{
  *((1 ^ USB_PUP_POLARITY)?
    &AT91C_BASE_PIOA->PIO_SODR:
    &AT91C_BASE_PIOA->PIO_CODR) = USB_PUN_MASK;
  USB_ConnectRes(FALSE);

  USB_REMOTE_WAKE_UP_DLY();

  USB_ConnectRes(TRUE);
  *((USB_PUP_POLARITY)?
    &AT91C_BASE_PIOA->PIO_SODR:
    &AT91C_BASE_PIOA->PIO_CODR) = USB_PUN_MASK;
}
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
Boolean USB_GetDevStatus (USB_DevStatusReqType_t Type)
{
  switch (Type)
  {
  case USB_DevConnectStatus:
    return(TRUE);
  case USB_AddrStateStatus:
    return((AT91C_BASE_UDP->UDP_GLBSTATE & AT91C_UDP_FADDEN) != 0);
  case USB_CnfgStateStatus:
    return((AT91C_BASE_UDP->UDP_GLBSTATE & AT91C_UDP_CONFG) != 0);
  }
  return(FALSE);
}

/*************************************************************************
 * Function Name: USB_SetStallEP
 * Parameters: USB_Endpoint_t EP, Boolean Stall
 *
 * Return: USB_ErrorCodes_t
 *
 * Description: The endpoint stall/unstall
 *
 *************************************************************************/
USB_ErrorCodes_t USB_SetStallEP (USB_Endpoint_t EP, Boolean Stall)
{
pEpCnfg_t pEP = &EpCnfg[EP];
Int32U reg;
  if (EpCnfg->pEpCtrl == NULL)
  {
    return(USB_EP_NOT_VALID);
  }
  reg = *pEP->pEpCtrl;
  if(Stall)
  {
    pEP->Status = STALLED;
    reg |=  AT91C_UDP_FORCESTALL | USB_EP_CLR_ONLY_MASK;
    reg &=~(USB_EP_SET_ONLY_MASK);
    *pEP->pEpCtrl = reg;
    DLY_EP();
    AT91F_UDP_EnableIt(AT91C_BASE_UDP,pEP->Mask);
  }
  else
  {
    pEP->Status = NOT_READY;
    if(EP & 1)
    {
      pEP->AvbBuff = pEP->bDoubleBuffered+1;
      pEP->bBuffer = 0;
    }
    else
    {
      pEP->AvbBuff = 0;
    }
    if ((EP != CTRL_ENP_OUT) || (EP != CTRL_ENP_IN))
    {
      AT91F_UDP_DisableIt(AT91C_BASE_UDP, pEP->Mask);
    }
    AT91F_UDP_ResetEp(AT91C_BASE_UDP,pEP->Mask);
    reg |=  USB_EP_CLR_ONLY_MASK;
    reg &= ~(AT91C_UDP_FORCESTALL | USB_EP_SET_ONLY_MASK | AT91C_UDP_ISOERROR);
    *pEP->pEpCtrl = reg;
    DLY_EP();
  }
  return(USB_OK);
}

/*************************************************************************
 * Function Name: USB_StallCtrlEP
 * Parameters: none
 *
 * Return: none
 *
 * Description: Stall both direction of the CTRL EP
 *
 *************************************************************************/
void USB_StallCtrlEP (void)
{
Int32U reg;
  EpCnfg[CTRL_ENP_IN].Status = STALLED;
  EpCnfg[CTRL_ENP_OUT].Status = STALLED;
  reg = *EpCnfg[CTRL_ENP_OUT].pEpCtrl;
  reg |=  AT91C_UDP_FORCESTALL | USB_EP_CLR_ONLY_MASK;
  reg &=~(USB_EP_SET_ONLY_MASK);
  *EpCnfg[CTRL_ENP_OUT].pEpCtrl = reg;
  DLY_EP();
}

/*************************************************************************
 * Function Name: USB_GetStallEP
 * Parameters: USB_Endpoint_t EP, pBoolean pStall
 *
 * Return: USB_ErrorCodes_t
 *
 * Description: Get stall state of the endpoint
 *
 *************************************************************************/
USB_ErrorCodes_t USB_GetStallEP (USB_Endpoint_t EP, pBoolean pStall)
{
Int32U reg;
  if(EpCnfg[EP].pEpCtrl == NULL)
  {
    return(USB_EP_NOT_VALID);
  }
  reg = *EpCnfg[EP].pEpCtrl;
  *pStall = (reg & AT91C_UDP_FORCESTALL) || (!(reg & AT91C_UDP_EPEDS));

  return (USB_OK);
}

/*************************************************************************
 * Function Name: USB_EP_IO
 * Parameters: USB_Endpoint_t EndPoint
 *
 * Return: none
 *
 * Description: Endpoints IO
 *
 *************************************************************************/
void USB_EP_IO(USB_Endpoint_t EP)
{
Int32U reg, Count;
pInt32U pDst;
pInt32U pSrc;
pEpCnfg_t pEP = &EpCnfg[EP];

  assert(pEP->pEpCtrl);

  if(pEP->Status != BEGIN_SERVICED &&
     pEP->Status != NO_SERVICED)
  {
    // This code may be executed only when IO function
    // has been called from interrupt
    // Only for IN EPs
    if (     (EP & 1)
        &&   (!pEP->bDoubleBuffered
          || (pEP->bDoubleBuffered
           &&(pEP->AvbBuff == 2))))
    {
      reg = *pEP->pEpCtrl;
      assert(reg & AT91C_UDP_TXCOMP);
      reg |=  USB_EP_CLR_ONLY_MASK;
      reg &= ~(USB_EP_SET_ONLY_MASK | AT91C_UDP_TXCOMP);
      *pEP->pEpCtrl = reg;
      DLY_EP();
      if (EP != CTRL_ENP_IN)
      {
        AT91F_UDP_DisableIt(AT91C_BASE_UDP, EpCnfg[EP].Mask);
      }
    }
    return;
  }
  if(EP & 1)
  {
    // IN
    Count = pEP->Size - pEP->Offset;
    while(pEP->AvbBuff)
    {
      if(Count == 0 && !pEP->bZeroPacket)
      {
        break;
      }
      // Set Status
      pEP->Status = BEGIN_SERVICED;
      // Get data size
      if(pEP->bZeroPacketPossible && Count == pEP->MaxSize)
      {
        pEP->bZeroPacketPossible = 0;
        pEP->bZeroPacket = 1;
      }

      Count = MIN(Count,pEP->MaxSize);
      if(Count == 0)
      {
        pEP->bZeroPacket = 0;
      }

      Int32U Offset = pEP->Offset;
      pEP->Offset += Count;
      // Write data to SIE buffer
      pDst = (pInt32U)&AT91C_BASE_UDP->UDP_FDR[EP>>1];

      while (Count--)
      {
        *pDst = *(pEP->pBuffer+Offset++);
      }

      if (  !pEP->bDoubleBuffered
          ||(pEP->bDoubleBuffered && pEP->AvbBuff == 2))
      {
        reg = *pEP->pEpCtrl;
        if(reg & AT91C_UDP_TXCOMP)
        {
          if(!pEP->bBufferCntrUpdate)
          {
            ++pEP->AvbBuff;
          }
          reg &= ~USB_EP_SET_ONLY_MASK;
          reg |=  USB_EP_CLR_ONLY_MASK | AT91C_UDP_TXPKTRDY;
          reg &= ~AT91C_UDP_TXCOMP;
          *pEP->pEpCtrl = reg;
          DLY_EP();
        }
        else
        {
          reg &= ~USB_EP_SET_ONLY_MASK;
          reg |=  USB_EP_CLR_ONLY_MASK | AT91C_UDP_TXPKTRDY;
          *pEP->pEpCtrl = reg;
          reg = *pEP->pEpCtrl ;
          DLY_EP();
        }
      }

      pEP->bBufferCntrUpdate = FALSE;
      --pEP->AvbBuff;
      Count = pEP->Size - pEP->Offset;
    }

    pEP->bBufferCntrUpdate = FALSE;
    if(!pEP->bZeroPacket && !Count)
    {
      //call callback function
      pEP->Status = COMPLETE;
      //call callback function
      if(pEP->pFn)
      {
        ((void(*)(USB_Endpoint_t))pEP->pFn)(EP);
      }
    }
    if (!(AT91F_UDP_GetInterruptMaskStatus(AT91C_BASE_UDP) & pEP->Mask))
    {
      AT91F_UDP_EnableIt(AT91C_BASE_UDP,pEP->Mask);
    }
  }
  else
  {
    // OUT
    while(pEP->AvbBuff)
    {
      // Get data size and buffer begin address
      reg = *pEP->pEpCtrl;
      // Get received bytes number
      Count = (reg>>16) & 0x7FF;
      // Get address of the USB packet buffer for corresponding EP
      pSrc = (pInt32U)&AT91C_BASE_UDP->UDP_FDR[EP>>1];

      if(Count > (pEP->Size - pEP->Offset))
      {
        pEP->Status = BUFFER_OVERRUN;
        pEP->Size = pEP->Offset;
        break;
      }
      else if (Count < pEP->MaxSize)
      {
        pEP->Status = BUFFER_UNDERRUN;
        pEP->Size = pEP->Offset + Count;
      }
      else
      {
        pEP->Status = BEGIN_SERVICED;
      }

      Int32U Offset = pEP->Offset;
      pEP->Offset += Count;

      // Read data from SIE buffer
      while (Count--)
      {
        *(pEP->pBuffer+Offset++) = *pSrc;
      }

      reg |=  USB_EP_CLR_ONLY_MASK;
      reg &=~(USB_EP_SET_ONLY_MASK | (pEP->bBuffer?AT91C_UDP_RX_DATA_BK1:AT91C_UDP_RX_DATA_BK0));
      *pEP->pEpCtrl = reg;
      DLY_EP();
      pEP->bBuffer ^= pEP->bDoubleBuffered;
      --pEP->AvbBuff;

      if(*pEP->pEpCtrl & AT91C_UDP_RXSETUP)
      {
        pEP->Status = SETUP_OVERWRITE;
        return;
      }
      if (!(Count = (pEP->Size - pEP->Offset)))
      {
        pEP->Status = COMPLETE;
        break;
      }
    }

    if (   pEP->Status != BEGIN_SERVICED
        && pEP->Status != NO_SERVICED)
    {
      //call callback function
      if (EP != CTRL_ENP_OUT)
      {
        AT91F_UDP_DisableIt(AT91C_BASE_UDP, pEP->Mask);
      }
      if(pEP->pFn)
      {
        ((void(*)(USB_Endpoint_t))pEP->pFn)(EP);
      }
    }
    else if (!(AT91F_UDP_GetInterruptMaskStatus(AT91C_BASE_UDP) & pEP->Mask))
    {
      AT91F_UDP_EnableIt(AT91C_BASE_UDP,pEP->Mask);
    }
  }
}

/*************************************************************************
 * Function Name: USB_EpLogToPhysAdd
 * Parameters: Int8U EpLogAdd
 *
 * Return: USB_Endpoint_t
 *
 * Description: Convert the logical to physical address
 *
 *************************************************************************/
USB_Endpoint_t USB_EpLogToPhysAdd (Int8U EpLogAdd)
{
USB_Endpoint_t Address = (USB_Endpoint_t)((EpLogAdd & 0x0F)<<1);
  if(EpLogAdd & 0x80)
  {
    ++Address;
  }
  return(Address);
}

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
Int32U USB_GetFrameNumb (void)
{
  return(AT91C_BASE_UDP->UDP_NUM & 0x7FF);
}
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
void USB_StatusPhase (Boolean In)
{
  if(In)
  {
    USB_IO_Data(CTRL_ENP_IN,NULL,0,NULL);
  }
}

#if SELF_POWERED != 0
/*************************************************************************
 * Function Name: USB_V_ISR
 * Parameters: none
 *
 * Return: none
 *
 * Description: USB voltage detect interrupt handler
 *
 *************************************************************************/
static
void USB_V_ISR (void)
{
volatile Int32U Status = AT91F_PIO_GetInterruptStatus(AT91C_BASE_PIOA);
  if(Status & USB_D_MASK)
  {
    if(IS_DEVICE_ATTCHED())
    {
      // Enable USB interrupts when device is attached
      UsbDevConnectCallback(TRUE);
      AT91F_AIC_EnableIt(AT91C_BASE_AIC,AT91C_ID_UDP);
    }
    else
    {
      // Disable USB interrupts when device is deattached
      AT91F_AIC_DisableIt(AT91C_BASE_AIC,AT91C_ID_UDP);
      UsbDevConnectCallback(FALSE);
    }
  }
}
#endif // SELF_POWERED != 0

/*************************************************************************
 * Function Name: USB_ISR
 * Parameters: none
 *
 * Return: none
 *
 * Description: USB interrupt subroutine
 *
 *************************************************************************/
void USB_ISR (void)
{
static Int32U IntrState;
static Int32U reg;
  USB_INTR_ENTRY_HOOK();
  // Get masked interrupt flags
  IntrState  = AT91F_UDP_InterruptStatusRegister(AT91C_BASE_UDP);
  IntrState &= AT91C_UDP_ENDBUSRES | AT91F_UDP_GetInterruptMaskStatus(AT91C_BASE_UDP);

  // Device reset
  if(IntrState & AT91C_UDP_ENDBUSRES)
  {
    AT91F_UDP_InterruptClearRegister(AT91C_BASE_UDP, AT91C_UDP_ENDBUSRES);
    USB_HwReset();
    UsbDevSuspendCallback(FALSE);
    UsbDevResetCallback();
    USB_INTR_EXIT_HOOK();
    return;
  }

  // Wake-up interrupt
  if(IntrState & (AT91C_UDP_RXRSM | AT91C_UDP_WAKEUP))
  {
    AT91F_UDP_CfgPMC();
    AT91F_PMC_CfgSysClkEnableReg( AT91C_BASE_PMC, AT91C_PMC_UDP );
    AT91F_UDP_EnableTransceiver(AT91C_BASE_UDP);
    UsbDevSuspendCallback(FALSE);
    AT91F_UDP_InterruptClearRegister(AT91C_BASE_UDP,AT91C_UDP_RXRSM | AT91C_UDP_WAKEUP);
    USB_INTR_EXIT_HOOK();
    return;
  }

  assert(AT91F_UDP_GetInterruptMaskStatus(AT91C_BASE_UDP) & AT91C_UDP_EPINT0);

  // Device suspend
  if(IntrState & AT91C_UDP_RXSUSP)
  {
    // disable of suspend interrupt after event isn't reasonably because
    // when cable remove the USB generate series of suspend-resume events
    AT91F_UDP_InterruptClearRegister(AT91C_BASE_UDP,AT91C_UDP_RXSUSP);
    AT91F_UDP_DisableTransceiver(AT91C_BASE_UDP);
    AT91F_PMC_CfgSysClkDisableReg( AT91C_BASE_PMC, AT91C_PMC_UDP );
    AT91F_PMC_DisablePeriphClock( AT91C_BASE_PMC, (1 << AT91C_ID_UDP) );
    UsbDevSuspendCallback(TRUE);
    USB_INTR_EXIT_HOOK();
    return;
  }

#if USB_SOF_EVENT > 0
  // Frame interrupt
  if(IntrState & AT91C_UDP_SOFINT)
  {
    AT91F_UDP_InterruptClearRegister(AT91C_BASE_UDP,AT91C_UDP_SOFINT);
  #if USB_SOF_FRAME_NUMB > 0
    USB_FRAME_HOOK(USB_GetFrameNumb());
  #else
    USB_FRAME_HOOK(0);
  #endif
  }
#endif

  // EP interrupt
  Int32U Mask = 1;
  Int32U i = 0;
  USB_Endpoint_t EP;
  //Int32U reg;
  Int32U Temp;
  volatile Int32U *pReg = AT91C_BASE_UDP->UDP_CSR;

  while(IntrState & 0xF)
  {
    if(IntrState & Mask)
    {
      reg = *pReg;
      // Determinate direction
      if (reg & (AT91C_UDP_RXSETUP | AT91C_UDP_RX_DATA_BK0 | AT91C_UDP_RX_DATA_BK1))
      {
        EP = (USB_Endpoint_t)(i << 1);
        if ((reg & (AT91C_UDP_RX_DATA_BK0 | AT91C_UDP_RX_DATA_BK1))
                == (AT91C_UDP_RX_DATA_BK0 | AT91C_UDP_RX_DATA_BK1))
        {
          EpCnfg[EP].AvbBuff = 2;
        }
        else
        {
          EpCnfg[EP].AvbBuff = 1;
        }
      }
      else if (reg & AT91C_UDP_TXCOMP)
      {
        EP = (USB_Endpoint_t)((i << 1) + 1);
        ++EpCnfg[EP].AvbBuff;
        if (   EpCnfg[EP].bDoubleBuffered
            && (EpCnfg[EP].AvbBuff == 1))
        {
          Temp = reg;
          Temp &= ~USB_EP_SET_ONLY_MASK;
          Temp |=  USB_EP_CLR_ONLY_MASK | AT91C_UDP_TXPKTRDY;
          Temp &= ~AT91C_UDP_TXCOMP;
          *pReg = Temp;
          DLY_EP();
          EpCnfg[EP].bBufferCntrUpdate = FALSE;
        }
        EpCnfg[EP].bBufferCntrUpdate = TRUE;
      }
      else if ((reg & AT91C_UDP_ISOERROR))
      {
        Temp = reg;
        Temp |=  USB_EP_CLR_ONLY_MASK;
        Temp &=~(AT91C_UDP_FORCESTALL | AT91C_UDP_ISOERROR | USB_EP_SET_ONLY_MASK);
        *pReg = Temp;
        DLY_EP();
        break;

      }
      else
      {
        break;
      }

      if(reg & AT91C_UDP_RXSETUP)
      {
        // setup packet
        // only ctrl EP can receive setup packets
        EpCnfg[CTRL_ENP_IN].AvbBuff  = 1;

        // init IO to receive Setup packet
        USB_IO_Data(CTRL_ENP_IN,NULL,(Int32U)-1,NULL);
        USB_IO_Data(CTRL_ENP_OUT,UsbEp0SetupPacket.Data,sizeof(UsbSetupPacket_t),NULL);

        Temp = reg;
        if(UsbEp0SetupPacket.mRequestType.Dir == UsbDevice2Host)
        {
          Temp |=  USB_EP_CLR_ONLY_MASK | AT91C_UDP_DIR;
          Temp &= ~( AT91C_UDP_RX_DATA_BK0
                   | AT91C_UDP_TXCOMP
                   | AT91C_UDP_FORCESTALL
                   | AT91C_UDP_ISOERROR
                   | USB_EP_SET_ONLY_MASK);
        }
        else
        {
          Temp |=  USB_EP_CLR_ONLY_MASK;
          Temp &= ~( AT91C_UDP_RX_DATA_BK0
                   | AT91C_UDP_DIR
                   | AT91C_UDP_TXCOMP
                   | AT91C_UDP_FORCESTALL
                   | AT91C_UDP_ISOERROR
                   | USB_EP_SET_ONLY_MASK);
        }

        Temp &= ~AT91C_UDP_RXSETUP;
        *pReg = Temp;
        DLY_EP();

        // Enable status phase
        USB_SetupHandler();

        if(EpCnfg[CTRL_ENP_OUT].Status == STALLED)
        {
          USB_StallCtrlEP();
        }
        else
        {
          if (UsbEp0SetupPacket.mRequestType.Dir == UsbDevice2Host)
          {
            USB_StatusHandler(CTRL_ENP_OUT);
          }
        }
      }
      else
      {
        if(EP == CTRL_ENP_OUT)
        {
          if(UsbEp0SetupPacket.mRequestType.Dir == UsbDevice2Host &&
             EpCnfg[CTRL_ENP_OUT].pFn)
          {
            Temp = reg;
            Temp |=  USB_EP_CLR_ONLY_MASK;
            Temp &=~(USB_EP_SET_ONLY_MASK | AT91C_UDP_RX_DATA_BK0);
            *pReg = Temp;
            DLY_EP();
            EpCnfg[CTRL_ENP_OUT].AvbBuff = 0;
            USB_IO_Data(CTRL_ENP_IN,NULL,(Int32U)-1,NULL);
            ((void(*)(USB_Endpoint_t))EpCnfg[CTRL_ENP_OUT].pFn)(CTRL_ENP_OUT);
            break;
          }
        }
        else if(EP == CTRL_ENP_IN)
        {
          if(UsbEp0SetupPacket.mRequestType.Dir == UsbHost2Device &&
             EpCnfg[CTRL_ENP_IN].pFn)
          {
            Temp = reg;
            Temp |=  USB_EP_CLR_ONLY_MASK;
            Temp &=~(USB_EP_SET_ONLY_MASK | AT91C_UDP_TXCOMP);
            *pReg = Temp;
            DLY_EP();
            EpCnfg[CTRL_ENP_IN].AvbBuff = 1;
            ((void(*)(USB_Endpoint_t))EpCnfg[CTRL_ENP_IN].pFn)(CTRL_ENP_IN);
            break;
          }
        }
        assert(EpCnfg[EP].AvbBuff <= (EpCnfg[EP].bDoubleBuffered + 1));
        USB_EP_IO(EP);
      }
      break;
    }
    ++i; Mask <<= 1; ++pReg;
  }
  USB_INTR_EXIT_HOOK();
}
