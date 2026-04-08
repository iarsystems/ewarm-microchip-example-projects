/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2007
 *
 *    File name   : nrv_drv.c
 *    Description : Nordic nRF24L01 functions
 *
 *    History :
 *    1. Date        : September 2007
 *       Author      : Stanimir Bonev
 *       Description : Initial Revision
 *    2. Date        : 20.12.2007
 *       Author      : Stoyan Choynev
 *       Description : Ported for Olimex STM32F103-STK
 *
 *    $Revision: 10 $
 **************************************************************************/
#define RNF_DRV_GLOBAL

#include "nrf_drv.h"

//---------------------------------------------------------------------------
// WriteReg
//---------------------------------------------------------------------------
static
BYTE WriteReg(BYTE Addr, const BYTE *pData, BYTE Size)
{
BYTE Status;
	RF24L_CS_L();
	Status = SpiNrfSendData(WRITE_REG | Addr);
	for(BYTE i = 0; i < Size; ++i)
	{
		SpiNrfSendData(*pData++);
	}
	RF24L_CS_H();
	return(Status);
}

//---------------------------------------------------------------------------
// ReadReg
//---------------------------------------------------------------------------
static
BYTE ReadReg(BYTE Addr, BYTE *pData, BYTE Size)
{
BYTE Status;
	RF24L_CS_L();
	Status = SpiNrfSendData(Addr);
	for(BYTE i = 0; i < Size; ++i)
	{
		*pData++ = SpiNrfSendData(NOP);
	}
	RF24L_CS_H();
	return(Status);
}

//---------------------------------------------------------------------------
// GetStatus
//---------------------------------------------------------------------------
static
BYTE GetStatus(void)
{
BYTE Status;
  RF24L_CS_L();
  Status = SpiNrfSendData(NOP);
  RF24L_CS_H();		
	return(Status);
}

//---------------------------------------------------------------------------
// FlushTx
//---------------------------------------------------------------------------
static
void FlushTx(void)
{
  RF24L_CS_L();
  SpiNrfSendData(FLUSH_TX);
  RF24L_CS_H();
}

//---------------------------------------------------------------------------
// FlushRx
//---------------------------------------------------------------------------
static
void FlushRx(void)
{
  RF24L_CS_L();
  SpiNrfSendData(FLUSH_RX);
  RF24L_CS_H();
}

//---------------------------------------------------------------------------
// WriteActReg
//---------------------------------------------------------------------------
static
void SendActComm(void)
{
	RF24L_CS_L();
	SpiNrfSendData(ACTIVE);
	SpiNrfSendData(ACTIVE_DATA);
	RF24L_CS_H();
}

static
void WriteActReg(BYTE Addr, BYTE Data)
{
  SendActComm();

  WriteReg(Addr,&Data,sizeof(Data));

  SendActComm();
}

//---------------------------------------------------------------------------
// WriteActReg
//---------------------------------------------------------------------------
static
void ReadActReg(BYTE Addr, BYTE * pData)
{
  SendActComm();

  ReadReg(Addr,pData,1);

  SendActComm();
}

/*************************************************************************
 * Function Name: NrfMode
 * Parameters: Mode Type RX_MODE Or TX_MODE
 *
 * Return: None
 *
 * Description: Sets the Nrf24L01 Rx Tx Mode
 *
 *************************************************************************/
void NrfMode(NrfModeType Mode)
{
BYTE Config;


  ReadReg(CONFIG,&Config,1);  //Read Nrf Config Register

  if( RX_MODE == Mode)
  {//Rx Mode
    Config |= PRIM_RX;        //Set mode Bit
  }
  else
  {//Tx Mode
    Config &= ~PRIM_RX;       //Clear Mode Bit
  }

  WriteReg(CONFIG,&Config,1); //Write Nrf Config Register	
}
/*************************************************************************
 * Function Name: NrfPower
 * Parameters: Power Mode POWER_DOWN or POWER_UP
 *
 * Return: None
 *
 * Description: Sets the Nrf24L01 Power Mode
 *
 *************************************************************************/
void NrfPower(NrfPowerType Power)
{
BYTE Config;

  ReadReg(CONFIG,&Config,1);    //Read Nrf Config Register

  if( POWER_UP == Power)
  {//Power Up
    Config |= PWR_UP;           //Set Power Up Bit
  }
  else
  {//Power Down
    Config &= ~PWR_UP;          //Clear Power Up Bit
  }

  WriteReg(CONFIG,&Config,1);   //Write Nrf Config Register	
}	

/*************************************************************************
 * Function Name: NrfCrc
 * Parameters: CRC width CRC16 or CRC8
 *
 * Return: None
 *
 * Description: Sets the Nrf24L01 Power Mode
 *
 *************************************************************************/
void NrfCrc( NrfCrcType Crc)
{
BYTE Config;

  ReadReg(CONFIG,&Config,1);    //Read Nrf Config Register

  if( CRC16 == Crc)
  {//2 Bytes CRC
    Config |= CRCO;             //Set CRCO Bit
  }
  else
  {//One Byte CRC
    Config &= ~CRCO;            //Clear CRCO Bit
  }

  WriteReg(CONFIG,&Config,1);   //Write Nrf Config Register	
}
/*************************************************************************
 * Function Name: NrfCrcEn
 * Parameters: CRC_ENABLE or CRC_DISABLE
 *
 * Return: None
 *
 * Description: Enables or Disables Nrf CRC
 *
 *************************************************************************/
void NrfCrcEn( NrfCrcEnType Enable)
{
BYTE Config;

  ReadReg(CONFIG,&Config,1);      //Read Nrf Config Register

  if( CRC_ENABLE == Enable)
  {//Enable CRC
    Config |= EN_CRC;             //Set CRC Enable Bit
  }
  else
  {//Disable CRC
    Config &= ~EN_CRC;            //Clear CRC Enable Bit
  }

  WriteReg(CONFIG,&Config,1);     //Write Nrf Config Register	
}

/*************************************************************************
 * Function Name: NrfMaskInt
 * Parameters: Interrupt Mask ( TX_DS, RX_DR, MAX_RT)
 *
 * Return: None
 *
 * Description: Mask Nrf Interrupt
 *
 *************************************************************************/
void NrfMaskInt(BYTE Interrupt)
{
BYTE Config;

  ReadReg(CONFIG,&Config,1);  //Read Config Register

  Config |= (Interrupt & (MAX_RT | TX_DS | RX_DR));  //Set mask Bits

  WriteReg(CONFIG,&Config,1);                        //Write Nrf Config Register
}

/*************************************************************************
 * Function Name: NrfMaskInt
 * Parameters: Interrupt ( TX_DS, RX_DR, MAX_RT)
 *
 * Return: None
 *
 * Description: UnMask Nrf Interrupt
 *
 *************************************************************************/
void NrfUnmaskInt(BYTE Interrupt)
{
BYTE Config;

  ReadReg(CONFIG,&Config,1);  //Read Config Register

  Config &= ~(Interrupt & (TX_DS | RX_DR | MAX_RT));  //Clear mask Bits

  WriteReg(CONFIG,&Config,1);                         //Write Nrf Config Register
}

/*************************************************************************
 * Function Name: NrfEnAutoAck
 * Parameters: Pipe Mask to be Enabled
 * (NRF_PIPE0M, NRF_PIPE1M, NRF_PIPE2M , NRF_PIPE3M, NRF_PIPE4M, NRF_PIPE5M)
 *
 * Return: None
 *
 * Description: Enable Pipe Auto Ack
 *
 *************************************************************************/
void NrfEnAutoAck(BYTE Pipe)
{
BYTE AutoAck;

  ReadReg(EN_AA,&AutoAck,1);  //Read EN_AA Register

  AutoAck |= (Pipe & (NRF_PIPE0M | NRF_PIPE1M | NRF_PIPE2M
                    | NRF_PIPE3M | NRF_PIPE4M | NRF_PIPE5M));  //Set Enable Bits

  WriteReg(EN_AA,&AutoAck,1);                     //Write Nrf EN_AA Register
}
/*************************************************************************
 * Function Name: NrfDisAutoAck
 * Parameters: Pipe to be Disabled (ENAA_P0, ENAA_P1, ENAA_P2 , ENAA_P3, ENAA_P4, ENAA_P5)
 *
 * Return: None
 *
 * Description: Disables Pipe Auto Ack
 *
 *************************************************************************/
void NrfDisAutoAck(BYTE Pipe)
{
BYTE AutoAck;

  ReadReg(EN_AA,&AutoAck,1);  //Read EN_AA Register

  AutoAck &= ~(Pipe & (NRF_PIPE0M | NRF_PIPE1M | NRF_PIPE2M
                     | NRF_PIPE3M | NRF_PIPE4M | NRF_PIPE5M));  //Clear Enable Bits
  WriteReg(EN_AA,&AutoAck,1);                     //Write Nrf EN_AA Register
}
/*************************************************************************
 * Function Name: NrfEnRxAddr
 * Parameters: Pipe to be Enabled (ERX_P0 , ERX_P1 , ERX_P2 , ERX_P3 , ERX_P4 , ERX_P5)
 *
 * Return: None
 *
 * Description: Enable RX Address
 *
 *************************************************************************/
void NrfEnRxAddr(BYTE Pipe)
{
BYTE RxAddr;

  ReadReg(EN_RXADDR,&RxAddr,1);   //Read EN_RXADDR Register

  RxAddr |= (Pipe & (NRF_PIPE0M | NRF_PIPE1M | NRF_PIPE2M
                    | NRF_PIPE3M | NRF_PIPE4M | NRF_PIPE5M));  //Set Enable Bits

  WriteReg(EN_RXADDR,&RxAddr,1);                     //Write Nrf EN_AA Register
}
/*************************************************************************
 * Function Name: NrfDisRxAddr
 * Parameters: Pipe to be Enabled (ERX_P0 , ERX_P1 , ERX_P2 , ERX_P3 , ERX_P4 , ERX_P5)
 *
 * Return: None
 *
 * Description: Enable RX Address
 *
 *************************************************************************/
void NrfDisRxAddr(BYTE Pipe)
{
BYTE RxAddr;

  ReadReg(EN_RXADDR,&RxAddr,1);   //Read EN_RXADDR Register

  RxAddr &= ~(Pipe & (NRF_PIPE0M | NRF_PIPE1M | NRF_PIPE2M
                    | NRF_PIPE3M | NRF_PIPE4M | NRF_PIPE5M));  //Set Enable Bits

  WriteReg(EN_RXADDR,&RxAddr,1);                     //Write Nrf EN_AA Register
}


/*************************************************************************
 * Function Name: NrfAddrWidth
 * Parameters: Address width (AW_3BYTES,AW_4BYTES,AW_5BYTES)
 *
 * Return: None
 *
 * Description: Sets Nrf Address Width
 *
 *************************************************************************/
void NrfAddrWidth(NrfAddrWidthType AddrWidth)
{
  AddrWidth &= AW_5BYTES;               //Set Address Width Bits

  assert(AddrWidth);                     //

  WriteReg(SETUP_AW,(BYTE *)&AddrWidth,1);      //Write Nrf SETUP_AW Register
}

/*************************************************************************
 * Function Name: NrfAutoRetDelay
 * Parameters: Auto Retransmit Delay (ARD_250us, ARD_500us.....ARD_4000us)
 *
 * Return: None
 *
 * Description: Sets Nrf Auto Retransmit Delay
 *
 *************************************************************************/
void NrfAutoRetDelay (NrfAutoDelayType Delay)
{
BYTE SetupRetr;

  ReadReg(SETUP_RETR, &SetupRetr, 1);

  SetupRetr &= ~(ARD_4000us);
  SetupRetr |= ( Delay & ARD_4000us);

  WriteReg(SETUP_RETR, &SetupRetr, 1);
}
/*************************************************************************
 * Function Name: NrfAutoRetCount
 * Parameters: Byte ( 0 .. 15 ), How many times the frame should be
 *  retransmitted
 *
 * Return: None
 *
 * Description: Sets Auto Retransmit Count
 *
 *************************************************************************/
void NrfAutoRetCount(BYTE Count)
{
BYTE SetupRetr;

  ReadReg(SETUP_RETR, &SetupRetr, 1);

  SetupRetr &= ~15;
  SetupRetr |= (Count & 15);

  WriteReg(SETUP_RETR, &SetupRetr, 1);
}

/*************************************************************************
 * Function Name: NrfChannel
 * Parameters: Channel
 *
 * Return: None
 *
 * Description: Sets RF Nrf Channel
 *
 *************************************************************************/
void NrfChannel(BYTE Channel)
{
  Channel &= ~128;
  WriteReg(RF_CH, &Channel, 1);
}

/*************************************************************************
 * Function Name: NrfGetChannel
 * Parameters: none
 *
 * Return: Byte
 *
 * Description: Gets RF Nrf Channel
 *
 *************************************************************************/
BYTE NrfGetChannel(void)
{
BYTE  Channel;
  ReadReg(RF_CH, &Channel, 1);
  return(Channel);
}

/*************************************************************************
 * Function Name: NrfLNA
 * Parameters: LNA Current (LNA_LCURR,LNA_HCURR)
 *
 * Return: None
 *
 * Description: Sets Auto LNA Current
 *
 *************************************************************************/
void NrfLNA (NrfLnaType Current)
{
BYTE RfSetup;

  ReadReg(RF_SETUP, &RfSetup, 1);

  if( LNA_HCURR == Current )
  {
    RfSetup |= RF_LNA;
  }
  else
  {
    RfSetup &= ~RF_LNA;
  }

  WriteReg(RF_SETUP, &RfSetup, 1);
}
/*************************************************************************
 * Function Name: NrfOutPower
 * Parameters: Output Power(RF_PWR_0dBm,RF_PWR_6dBm,RF_PWR_12dBm,RF_PWR_18dBm)
 *
 * Return: None
 *
 * Description: Sets Nrf Output Power
 *
 *************************************************************************/
void NrfOutPower(NrfOutPowerType Power)
{
BYTE RfSetup;

  ReadReg(RF_SETUP, &RfSetup, 1);

  RfSetup &= ~RF_PWR_0dBm ;

  RfSetup |= (Power & RF_PWR_0dBm);

  WriteReg(RF_SETUP, &RfSetup, 1);
}

/*************************************************************************
 * Function Name: NrfDataRate
 * Parameters: Data Rate (NRF_1MBPS, NRF_2MBPS)
 *
 * Return: None
 *
 * Description: Sets Nrf Air Data Rate
 *
 *************************************************************************/
void NrfDataRate(NrfDataRateType Rate)
{
BYTE RfSetup;

  ReadReg(RF_SETUP, &RfSetup, 1);

  if(NRF_2MBPS == Rate)
  {
    RfSetup |= RF_DR;
  }
  else
  {
    RfSetup &= ~RF_DR;
  }

  WriteReg(RF_SETUP, &RfSetup, 1);

}
/*************************************************************************
 * Function Name: NrfClearInt
 * Parameters: Interrupt (MAX_RT, TX_DS, RX_DR)
 *
 * Return: None
 *
 * Description: Sets Nrf Air Data Rate
 *
 *************************************************************************/
void NrfClearInt(BYTE Interrupt )
{

  Interrupt &= (MAX_RT | TX_DS | RX_DR);            //Set mask Bits

  WriteReg(STATUS,(BYTE *)&Interrupt,1);                    //Write Nrf Status Register

}

/*************************************************************************
 * Function Name: NrfSetRxAddr
 * Parameters: Pipe, pointer to Addr array, Address Size
 *
 * Return:
 *
 * Description: Sets Pipe Rx Address
 *
 *************************************************************************/
void NrfSetRxAddr(NRF_Pipes_t Pipe, const BYTE * Addr, BYTE Size)
{
  assert(!(Size > 5 || Pipe > NRF_PIPE5));
  assert(!((Size > 1) && (Pipe > NRF_PIPE1)));

  WriteReg(RX_ADDR_P0+Pipe, Addr, Size);
}

/*************************************************************************
 * Function Name: NrfSetTxAddr
 * Parameters: pointer to Addr array, Address Size
 *
 * Return:
 *
 * Description: Sets Tx Address
 *
 *************************************************************************/
void NrfSetTxAddr(const BYTE * Addr, BYTE Size)
{
  WriteReg(TX_ADDR, Addr, Size);
}

/*************************************************************************
 * Function Name: NrfPeyWidth
 * Parameters: Pipe, Payload Width
 *
 * Return:
 *
 * Description: Sets Pipe Payload Width
 *
 *************************************************************************/
void NrfRxPayWidth(NRF_Pipes_t Pipe, BYTE PayWidth)
{
  assert(!(Pipe > NRF_PIPE5));
  assert(!(PayWidth > 32));

  WriteReg(RX_PW_P0+Pipe, &PayWidth, 1);
}

/*************************************************************************
 * Function Name: NrfEnDynPay
 * Parameters: Pipe to Enable
 *
 * Return:
 *
 * Description: Enables Pipe Dynamic Payload length
 *
 *************************************************************************/
void NrfEnDynPay(BYTE Pipe)
{
BYTE DynPay;

  ReadActReg(DYNPD,&DynPay);   //Read DYNPD Register

  DynPay |= (Pipe & (NRF_PIPE0M | NRF_PIPE1M | NRF_PIPE2M
                    | NRF_PIPE3M | NRF_PIPE4M | NRF_PIPE5M));  //Set Enable Bits

  WriteActReg(DYNPD,DynPay);                     //Write Nrf DYNPD Register
}

/*************************************************************************
 * Function Name: NrfDisDynPay
 * Parameters: Pipe to Disable
 *
 * Return:
 *
 * Description: Disable Pipe Dynamic Payload length
 *
 *************************************************************************/
void NrfDisDynPay(BYTE Pipe)
{
BYTE Dynpd;

  ReadActReg(DYNPD,&Dynpd);   //Read DYNPD Register

  Dynpd &= ~(Pipe & (NRF_PIPE0M | NRF_PIPE1M | NRF_PIPE2M
                    | NRF_PIPE3M | NRF_PIPE4M | NRF_PIPE5M));  //Set Enable Bits

  WriteActReg(DYNPD,Dynpd);                     //Write Nrf DYNPD Register
}

/*************************************************************************
 * Function Name: NrfDynPya
 * Parameters: Enable Disable (DPL_ENABLE, DPL_DISABLE)
 *
 * Return:
 *
 * Description: Enables/Disables Dynamic Payload length
 *
 *************************************************************************/
void NrfDynPay(NrfDynPayType DynPay )
{
BYTE Feature;

  ReadActReg(FEATURE, &Feature);

  if( DPL_ENABLE == DynPay )
  {
    Feature |= EN_DPL;
  }
  else
  {
    Feature &= ~EN_DPL;
  }

  WriteActReg(FEATURE, Feature);
}

/*************************************************************************
 * Function Name: NrfDynAck
 * Parameters: Enable Disable (DYN_ACK_ENABLE, DYN_ACK_DISABLE)
 *
 * Return:
 *
 * Description: Enables/Disables Dynamic Payload length
 *
 *************************************************************************/
void NrfDynAck(NrfDynAckType DynAck)
{
BYTE Feature;

  ReadActReg(FEATURE, &Feature);

  if( DYN_ACK_ENABLE == DynAck )
  {
    Feature |= EN_DYN_ACK;
  }
  else
  {
    Feature &= ~EN_DYN_ACK;
  }

  WriteActReg(FEATURE, Feature);
}

/*************************************************************************
 * Function Name: NrfDynAck
 * Parameters: Enable Disable (DYN_ACK_ENABLE, DYN_ACK_DISABLE)
 *
 * Return:
 *
 * Description: Enables/Disables Dynamic Payload length
 *
 *************************************************************************/
void NrfAckPay(NrfAckPayType AckPay)
{
BYTE Feature;

  ReadActReg(FEATURE, &Feature);

  if( ACK_PAY_ENABLE == AckPay )
  {
    Feature |= EN_ACK_PAY;
  }
  else
  {
    Feature &= ~EN_ACK_PAY;
  }

  WriteActReg(FEATURE, Feature);
}

/*************************************************************************
 * Function Name: NrfClearRxBuffer
 * Parameters: None
 *
 * Return: None
 *
 * Description: Clear Nrf24L01 Rx FIFO
 *
 *************************************************************************/
void NrfClearRxBuffer(void)
{
  FlushRx();
}
/*************************************************************************
 * Function Name: NrfClearTxBuffer
 * Parameters: None
 *
 * Return: None
 *
 * Description: Clear Nrf24L01 Tx FIFO
 *
 *************************************************************************/
void NrfClearTxBuffer(void)
{
  FlushTx();
}

/*************************************************************************
 * Function Name: NrfGetRxSize
 * Parameters: None
 *
 * Return: BYTE Size of the RX FIFO
 *
 * Description: Reads the size of the Data in the RX FIFO
 *
 *************************************************************************/
BYTE NrfGetRxSize(void)
{
BYTE Size;
  ReadReg(RD_RX_PLOAD_WID, &Size,1);
  return(Size);
}

/*************************************************************************
 * Function Name: NrfGetRxSize
 * Parameters: None
 *
 * Return: BYTE
 *
 * Description: Gets the Pipe number for the payload available for
 * reading from RX_FIFO
 *
 *************************************************************************/
BYTE NrfGetPipeAvb(void)
{
BYTE Status;
  Status = GetStatus();
  return((Status>>1)&0x07);
}

/*************************************************************************
 * Function Name: NrfGetState
 * Parameters: None
 *
 * Return: NrfState
 *
 * Description: Returns the State of the Nrf24L01
 *
 *************************************************************************/
NrfStateType NrfGetState(void)
{
BYTE Reg;
  ReadReg(CONFIG, &Reg,1);
  if(!(Reg & PWR_UP)) return(ST_PWR_DOWN);
  if( Reg & PRIM_RX)
  {
    if(RF24L_GET_CE()) return(ST_RX);
    return(ST_STAND_I);
  }

  ReadReg(FIFO_STATUS,&Reg,1);

  if(!(Reg & TX_FIFO_EMPTY)) return(ST_TX);

  if(RF24L_GET_CE()) return(ST_STAND_II);
  return(ST_STAND_I);
}

BYTE NrfGetFifoStatus(void)
{
BYTE FifoStatus;
  ReadReg(FIFO_STATUS,&FifoStatus,1);
  return FifoStatus;
}

BYTE NrfGetIntID(void)
{
BYTE Status;
BYTE Mask;
  ReadReg(STATUS,&Status,1);
  ReadReg(CONFIG,&Mask,1);
  Mask |= ~(MAX_RT | TX_DS | RX_DR);
  return((BYTE)(Status & ~Mask));
}

void NrfReadFifo(BYTE* pData, BYTE Size)
{
  ReadReg(RD_RX_PLOAD, pData, Size);
}

void NrfWriteFifo(BYTE* pData, BYTE Size)
{
  WriteReg(WR_TX_PLOAD,pData, Size);
}



