/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2007
 *
 *    File name   : RF.c
 *    Description :
 *
 *    History :
 *    1. Date        : 07.1.2008
 *       Author      : Stoyan Choynev
 *       Description : Initial Version
 *
 *    $Revision: 10 $
 **************************************************************************/

/** include files **/
#include "nrf_drv.h"
#include "RF.h"

Int8U RefCntr;
Int8U TrnCntr;

/** local definitions **/


Int32U GardTxTimer;

struct
{
  BYTE Start;
  BYTE End;
  BYTE  Data[32];
} RfRxBuffer;

/** default settings **/

/** external functions **/

/** external data **/

/** internal functions **/

/** public data **/

RfModeTypeDef RfMode;
/** private data **/
#if  defined(NODEA)
#warning  Defined NODEA
static const BYTE TxAddress[] = { 0x01, 0x00, 0xEF, 0xEF, 0xEF};
static const BYTE RxAddress[] = { 0x02, 0x00, 0xEF, 0xEF, 0xEF};
#elif defined(NODEB)
#warning  Defined NODEB
static const BYTE TxAddress[] = { 0x02, 0x00, 0xEF, 0xEF, 0xEF};
static const BYTE RxAddress[] = { 0x01, 0x00, 0xEF, 0xEF, 0xEF};
#else
#error Define NODEA or NODEB
#endif

Int32U TxFifoCntr = 0;

/** public functions **/

/*************************************************************************
 * Function Name: RfInit
 * Parameters: None
 *
 * Return: None
 *
 * Description: Inits RF modul. Transceiver stays in Power Down mode
 *
 *************************************************************************/
void RfInit(void)
{

  nrf24l_init();

  //Power Down Mode
  NrfPower(POWER_DOWN);
  //Enable CRC
  NrfCrcEn(CRC_ENABLE);
  NrfCrc(CRC16);
  //Enable Pipe 1 Auto Ack  (all the others are disabled)
  NrfDisAutoAck( NRF_PIPE2M | NRF_PIPE3M
              | NRF_PIPE4M | NRF_PIPE5M);
  NrfEnAutoAck(NRF_PIPE0M | NRF_PIPE1M );

  //Enable Pipe 0 and Pipe 1 Rx (all the others are disabled)
  NrfDisRxAddr( NRF_PIPE2M | NRF_PIPE3M
              | NRF_PIPE4M | NRF_PIPE5M);
  NrfEnRxAddr( NRF_PIPE1M | NRF_PIPE0M);
  //5 bytes Address Width
  NrfAddrWidth(AW_5BYTES);
  //Auto retransmit 3 times, 500us between retransmissions
  NrfAutoRetCount(3);
  NrfAutoRetDelay(ARD_500us);
  //Channel 2 selected
  NrfChannel(2);
  //0 dBm output Power, 2Mbps, LNA High Current
  NrfOutPower(RF_PWR_0dBm);
  NrfDataRate(NRF_2MBPS);
  NrfLNA(LNA_HCURR);

  //Clear Interrupts
  NrfClearInt(MAX_RT | TX_DS | RX_DR);
  //Disable interrupt
  NrfMaskInt(MAX_RT | TX_DS | RX_DR);

  NrfSetRxAddr(NRF_PIPE0,TxAddress,5);
  NrfSetRxAddr(NRF_PIPE1,RxAddress,5);
  NrfSetTxAddr(TxAddress,5);

  NrfEnDynPay(NRF_PIPE0M | NRF_PIPE1M);
  NrfDynPay(DPL_ENABLE);

}

/*************************************************************************
 * Function Name: RxPowerUp
 * Parameters: None
 *
 * Return: None
 *
 * Description: Puts Transceiver in Power Up mode. Clears all
 *              Receive and Transmit Buffers
 *************************************************************************/
void RfPowerUp(void)
{
  RF24L_CE_L();

  NrfClearRxBuffer();
  RfRxBuffer.Start = RfRxBuffer.End = 0;
  
  NrfClearTxBuffer();

  NrfPower(POWER_UP);
}
/*************************************************************************
 * Function Name: RxPowerDown
 * Parameters: None
 *
 * Return: None
 *
 * Description: Puts Transceiver in Power Down mode. Clears all
 *              Receive and Transmit Buffers
 *************************************************************************/
void RfPowerDown(void)
{
  RF24L_CE_L();

  NrfClearRxBuffer();

  NrfClearTxBuffer();

  NrfPower(POWER_DOWN);
}

/*************************************************************************
 * Function Name: StartRx
 * Parameters:None
 *
 * Return:None
 *
 * Description:Sets Transceiver in RX Mode
 *
 *************************************************************************/
void StartRx(void)
{
  RF24L_CE_L();
  
  NrfMode(RX_MODE);

  RF24L_CE_H();
}

/*************************************************************************
 * Function Name: StartRx
 * Parameters:None
 *
 * Return:None
 *
 * Description:Copies Data from RfTxBuffer to NRF TX Fifo and
 *                    Sets Transceiver in TX Mode
 *************************************************************************/
void StartTx(void)
{
  RF24L_CE_L();

  NrfMode(TX_MODE);

  NrfClearInt(MAX_RT | TX_DS);
  NrfUnmaskInt(MAX_RT | TX_DS);

  RF24L_CE_H();
  for(volatile BYTE j = NRF_TX_PULSE_DLY; j; j--);
  RF24L_CE_L();
}

/*************************************************************************
 * Function Name: DecGardTxTimer
 * Parameters:None
 *
 * Return: None
 *
 * Description:Decrements GardtxTimer if it is not 0
 *
 *************************************************************************/
void DecGardTxTimer(void)
{
  if(GardTxTimer) --GardTxTimer;
}
/*************************************************************************
 * Function Name: GetGardTxTimer
 * Parameters:None
 *
 * Return: None
 *
 * Description:Returns the GardTxTimer Value
 *
 *************************************************************************/
int GetGardTxTimer(void)
{
  return GardTxTimer;
}

void StartGardTxTimer(Int32U Miliseconds)
{
  GardTxTimer = Miliseconds;
}
/*************************************************************************
 * Function Name: RF_IRQ_Handler
 * Parameters:None
 *
 * Return: None
 *
 * Description: Handler of Transceiver RX, TX and MAX RT interrupts
 *
 *************************************************************************/
void RF_IRQ_Handler(void)
{
BYTE RfIntId;

    RfIntId = NrfGetIntID();
  
    NrfClearInt(RfIntId);
    
    if(RfIntId & (MAX_RT | TX_DS))
    {
      if(RfIntId & (TX_DS))
      {
        assert(TxFifoCntr);
        
        if(!(--TxFifoCntr))
        {
          RfMode = RF_RX_MODE;
          StartGardTxTimer(RAND_GARD_TIME);
          StartRx();
        }
        else
        {
          StartTx();
        }
      }
      else
      {
        RfMode = RF_RX_MODE;
        StartGardTxTimer(RAND_GARD_TIME);
        StartRx();
      }
    }
}

/*************************************************************************
 * Function Name: RfGetData
 * Parameters:pInt32U pointer to the Dest Buffer
 *            Int32U Available buffer size
 * Return:    Int32U size of the transferred data
 *
 * Description: Read data From Nrf FIFO and puts it to Dest Buffer until
 *              Fifo gets empty or Buffer gets Full
 *************************************************************************/
Int32U RfGetData(pInt8U pBuffer, Int32U BuffSize)
{
Int32U DataCount = 0;
Int32U Size;

  while(1)
  {
      Size = MIN(BuffSize, (RfRxBuffer.End - RfRxBuffer.Start));  
            
      for(Int32U i = 0; i < Size; i++)
      {
        *pBuffer++ = RfRxBuffer.Data[RfRxBuffer.Start++];
      }
      
      DataCount += Size;
      BuffSize -= Size;
      
      if(!BuffSize) return DataCount;
      
      if((RfRxBuffer.End - RfRxBuffer.Start)) return DataCount;
      
      if(NrfGetFifoStatus() & RX_FIFO_EMPTY) return DataCount;
                  
      RF24L_CE_L();

      StartGardTxTimer(RAND_GARD_TIME);
      
      RfRxBuffer.Start = 0;
      
      RfRxBuffer.End =  NrfGetRxSize();
      
      NrfReadFifo(RfRxBuffer.Data,RfRxBuffer.End);
      
      NrfClearInt(RX_DR);

      RF24L_CE_H();      
      
  }
}
/*************************************************************************
 * Function Name: RfPutData
 * Parameters:pInt32U pointer to the Source Buffer
 *            Int32U Data size
 * Return:    Int32U size of the transferred data
 *
 * Description: Fills Nrf Fifo with data from source Buffer
 *              until all data is sent of Fifo gets full
 *************************************************************************/
Int32U RfPutData(pInt8U pBuffer, Int32U DataSize)
{
Int32U Size;
Int32U DataCount = 0;

  while(DataSize && (2 > TxFifoCntr))
  {
    Size = MIN(32, DataSize);
    
    NrfWriteFifo(pBuffer+DataCount,Size);
    DataCount += Size;          
    DataSize -= Size;
    TxFifoCntr++;
  }
  
  return DataCount; 
}
