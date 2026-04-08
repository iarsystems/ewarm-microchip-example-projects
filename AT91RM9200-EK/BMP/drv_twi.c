/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2005
 *
 *    File name   : drv_1523.c
 *    Description : Driver of TWI
 *
 *    History :
 *    1. Data        : October, 11 2005
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 10 $
**************************************************************************/
#include "drv_twi.h"

Twi_Ctrl_t Twi_Ctrl;
Int32U count = 0;
/*************************************************************************
 * Function Name: Twi_Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: TWI Interrupt handler
 *
 *************************************************************************/
void Twi_Handler (void)
{
Int32U Status = AT91C_BASE_TWI->TWI_SR;
Int8U Data;
  ++count;
  // keep only enable interrupt flags
  if (Status & AT91C_TWI_NACK)
  {
    // Slave return NAK
    Twi_Ctrl.TwiStatus = TwiNak;
  }
  if (Status & AT91C_TWI_TXRDY & AT91F_TWI_GetInterruptMaskStatus(AT91C_BASE_TWI))
  {
    // Write data to TWI
    if (--Twi_Ctrl.DataSize == 0)
    {
      // защото по-глупаво нещо не бя срещал, а съм срещал доста неща!!!
      AT91F_TWI_DisableIt(AT91C_BASE_TWI,AT91C_TWI_TXRDY);
      AT91F_TWI_EnableIt(AT91C_BASE_TWI,AT91C_TWI_TXCOMP);
      // No more data for send
      AT91C_BASE_TWI->TWI_CR   = AT91C_TWI_STOP;
    }
    // Write byte
    AT91C_BASE_TWI->TWI_THR  = *Twi_Ctrl.pMsg++;
  }
  if (Status & AT91C_TWI_RXRDY)
  {
    // Read data from TWI
    if (Twi_Ctrl.DataSize == 0)
    {
      AT91F_TWI_DisableIt(AT91C_BASE_TWI,AT91C_TWI_RXRDY);
      AT91F_TWI_EnableIt (AT91C_BASE_TWI,AT91C_TWI_TXCOMP);
      // защото по-глупаво нещо не бя срещал, а съм срещал доста неща!!!
      // No more data for receiving
      AT91C_BASE_TWI->TWI_CR   = AT91C_TWI_STOP;
    }
    Data = AT91C_BASE_TWI->TWI_RHR;
    // Read byte
    if (Twi_Ctrl.DataSize >= 0)
    {
      --Twi_Ctrl.DataSize;
      *Twi_Ctrl.pMsg++ = Data;
    }
  }
  if (Status & AT91C_TWI_TXCOMP & AT91F_TWI_GetInterruptMaskStatus(AT91C_BASE_TWI))
  {
    // Transfer completed
    Twi_Ctrl.TwiStatus = TwiOk;
    AT91F_TWI_DisableIt(AT91C_BASE_TWI,AT91C_TWI_RXRDY |
                        AT91C_TWI_TXCOMP | AT91C_TWI_TXRDY);
  }
}

/*************************************************************************
 * Function Name: Twi_Init
 * Parameters: Int32U Freq, Int8U AicPriority
 *
 * Return: Twi_Response_t
 *
 * Description: Init TWI controller
 *
 *************************************************************************/
Twi_Response_t Twi_Init (Int32U Freq, Int8U AicPriority)
{
Int32U Div,ClkDiv = 0;
  // Enable Clock of TWI
  AT91F_TWI_CfgPMC();
  // Assign PINs to TWI (Peripheral A)
  AT91F_TWI_CfgPIO();
  // Init TWI
  AT91F_TWI_Configure(AT91C_BASE_TWI);
  // Set TWI clock
  do
  {
    Div = (Fmck - 6*Freq*(1<<ClkDiv))/(2*Freq*(1<<ClkDiv));
    if(++ClkDiv>5)
    {
      return(TwiParametersError);
    }
  }
  while(Div > 256);
  AT91C_BASE_TWI->TWI_CWGR = Div | ( Div << 8) | (--ClkDiv << 16);
  // Clear Intetrups pending
  Div = AT91C_BASE_TWI->TWI_RHR;
  Div = AT91C_BASE_TWI->TWI_SR;
  // Init ACI slot
  AT91F_AIC_ConfigureIt(AT91C_BASE_AIC,AT91C_ID_TWI,AicPriority,0,(VoidFpnt_t)Twi_Handler);
  // TWI Init interrupts
  AT91F_TWI_EnableIt(AT91C_BASE_TWI, AT91C_TWI_NACK);
  // TWI interrupt enable
  AT91F_AIC_EnableIt(AT91C_BASE_AIC,AT91C_ID_TWI);
  return(TwiOk);
}

/*************************************************************************
 * Function Name: Twi_Write
 * Parameters: Int8U SlaveAdd, Int8U Add,
 *             const Int8U * pMsg, Int32U Size
 *
 * Return: Twi_Response_t
 *
 * Description: Write massive to address - Add
 *
 *************************************************************************/
Twi_Response_t Twi_Write (Int8U SlaveAdd, Int8U Add,
                          const Int8U * pMsg, Int32U Size)
{
  if((Size == 0) || (pMsg == NULL))
  {
    return(TwiParametersError);
  }

  Twi_Ctrl.TwiStatus = TwiBusy;
  // Write data
  AT91C_BASE_TWI->TWI_MMR  = AT91C_TWI_IADRSZ_1_BYTE |
                            (SlaveAdd << 16);
  AT91C_BASE_TWI->TWI_IADR = Add;
  AT91C_BASE_TWI->TWI_THR  = *pMsg++;
  Twi_Ctrl.pMsg  = (Int8U *)pMsg;
  Twi_Ctrl.DataSize = --Size;
  if (Size == 0)
  {
    AT91F_TWI_EnableIt(AT91C_BASE_TWI, AT91C_TWI_TXCOMP);
    AT91C_BASE_TWI->TWI_CR   = AT91C_TWI_START | AT91C_TWI_STOP;
  }
  else
  {
    AT91F_TWI_EnableIt(AT91C_BASE_TWI,AT91C_TWI_TXRDY);
    AT91C_BASE_TWI->TWI_CR   = AT91C_TWI_START;
  }
  // Wait status
  while(Twi_Ctrl.TwiStatus == TwiBusy);
  return(Twi_Ctrl.TwiStatus);
}

/*************************************************************************
 * Function Name: Twi_Read
 * Parameters: Int8U SlaveAdd, Int8U Add, Int8U * pMsg, Int32U Size
 *
 * Return: Twi_Response_t
 *
 * Description: Read sequential from address Add
 *
 *************************************************************************/
Twi_Response_t Twi_Read (Int8U SlaveAdd, Int8U Add,Int8U * pMsg, Int32U Size)
{
  if((Size == 0) || (pMsg == NULL))
  {
    return(TwiParametersError);
  }

  Twi_Ctrl.pMsg = pMsg;
  Twi_Ctrl.DataSize = --Size;
  // Set address and when read
  AT91C_BASE_TWI->TWI_MMR  = AT91C_TWI_IADRSZ_1_BYTE | AT91C_TWI_MREAD |
                             (SlaveAdd << 16);
  AT91C_BASE_TWI->TWI_IADR = Add;
  Twi_Ctrl.TwiStatus = TwiBusy;
  AT91F_TWI_EnableIt(AT91C_BASE_TWI, AT91C_TWI_RXRDY);
  if (Size == 0)
  {
    AT91C_BASE_TWI->TWI_CR   = AT91C_TWI_START | AT91C_TWI_STOP;
  }
  else
  {
    AT91C_BASE_TWI->TWI_CR   = AT91C_TWI_START;
  }
  // Wait status
  while(Twi_Ctrl.TwiStatus == TwiBusy);
  return(Twi_Ctrl.TwiStatus);
}
