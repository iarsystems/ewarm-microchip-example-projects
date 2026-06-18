/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2007
 *
 *    File name   : nrf_drv.h
 *    Description : nRF24L01 driver include file
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

#include <assert.h>
#include "nrf_drv_low.h"
#include "nrf2401l.h"

#ifndef NRF_DRV_DEF
#define NRF_DRV_DEF

#ifdef RNF_DRV_GLOBAL
#define RNF_DRV_EXTER
#else
#define RNF_DRV_EXTER  extern
#endif

#ifndef NDEBUG
# ifdef NRF24L_DISABLE_DBG_MESSAGES
#define NRF24L_DBG_MESS(...)
#define NRF24L_DBG(...)
# else
#define NRF24L_DBG_MESS(Data)\
{\
pInt8U pData = (pInt8U)Data;\
  while(*pData) DBG_TxPush(*pData++);\
}

#define NRF24L_DBG(Data) DBG_TxPush(Data)
# endif

# ifdef NRF24L_DISABLE_ERROR_MESSAGES
#define NRF24L_ERROR_MESS(...)
#define NRF24L_ERROR(...)

# else
#define NRF24L_ERROR_MESS(Data)\
{\
BYTE * pData = (BYTE *)Data;\
  while(*pData) DBG_TxPush(*pData++);\
}

#define NRF24L_ERROR(Data) DBG_TxPush(Data)
# endif
#else
#define NRF24L_DBG_MESS(...)
#define NRF24L_DBG(...)
#define NRF24L_ERROR_MESS(...)
#define NRF24L_ERROR(...)
#endif


#define FLAG_SEQUENCE         0x7E

#define NRF_PUSH_CHAR_TO               5      // 5 ms
#define NRF_MAX_TX_PACKET_TO           5      // 5 ms
#define NRF_SUPERVISION_MASTER_TO     64      // 64 ms
#define NRF_SUPERVISION_SLAVE_TO     378      // 378 ms

typedef enum _RF_DevTypes_t
{
  RF_JTAG_ID = 0,
} RF_DevTypes_t;

typedef enum _RF_Command_t
{
  NRF_USER_DATA = 0, NRF_SUPERVISION, NRF_CHANGE_CHANNEL, NRF_ECHO_SEND, NRF_ECHO_RETURN,
  NRF_FLOW_CTRL, NRF_DATA_LOST,
  NRF_FLOW_CTRL_PL = 0x80,
} RF_Command_t;

typedef enum _RF_TestStatus_t
{
  NRF_TEST_PASS = 0, NRF_TEST_PROCESS, NRF_TEST_DATA_MISSCOMP, NRF_TEST_FAULT, NRF_TEST_REJECT
} RF_TestStatus_t;

typedef struct _RF_Test_t
{
  BYTE Size;
  BYTE Loop;
  RF_TestStatus_t Status;
  LONG Delay;
  void (* pFn)(struct _RF_Test_t *);
  WORD RepeatedPackets;
  BYTE * pBuffer;
} RF_Test_t, *pRF_Test_t;

typedef enum {TX_MODE = 0, RX_MODE = !TX_MODE} NrfModeType ;

typedef enum {POWER_DOWN = 0, POWER_UP = !POWER_DOWN} NrfPowerType;

typedef enum {CRC8 = 0, CRC16 = !CRC8} NrfCrcType;

typedef enum {CRC_DISABLE = 0, CRC_ENABLE = !CRC_DISABLE} NrfCrcEnType;

#define MAX_RT  (BYTE)(1<<4)
#define TX_DS   (BYTE)(1<<5)
#define RX_DR   (BYTE)(1<<6)

#define NRF_PIPE0M (BYTE)(1<<0)
#define NRF_PIPE1M (BYTE)(1<<1)
#define NRF_PIPE2M (BYTE)(1<<2)
#define NRF_PIPE3M (BYTE)(1<<3)
#define NRF_PIPE4M (BYTE)(1<<4)
#define NRF_PIPE5M (BYTE)(1<<5)

typedef enum {AW_3BYTES = 1, AW_4BYTES, AW_5BYTES} NrfAddrWidthType;

typedef enum {ARD_250us  = (0<<4), ARD_500us  = (1<<4), ARD_750us  = (2<<4),
              ARD_1000us = (3<<4), ARD_1250us = (4<<4), ARD_1500us = (5<<4),
              ARD_1750us = (6<<4), ARD_2000us = (7<<4), ARD_2250us = (8<<4),
              ARD_2500us = (9<<4), ARD_2750us = (10<<4),ARD_3000us = (11<<4),
              ARD_3250us = (12<<4), ARD_3500us = (13<<4), ARD_3750us = (14<<4),
              ARD_4000us = (15<<4)} NrfAutoDelayType;

typedef enum {LNA_LCURR = 0, LNA_HCURR = !LNA_LCURR} NrfLnaType;

typedef enum {RF_PWR_18dBm = (0<<1), RF_PWR_12dBm = (1<<1), RF_PWR_6dBm  = (2<<1), RF_PWR_0dBm  = (3<<1)} NrfOutPowerType;

typedef enum {NRF_1MBPS = 0, NRF_2MBPS} NrfDataRateType;

typedef enum {DPL_DISABLE = 0, DPL_ENABLE = !DPL_DISABLE} NrfDynPayType;

typedef enum {DYN_ACK_DISABLE = 0, DYN_ACK_ENABLE = !DYN_ACK_DISABLE} NrfDynAckType;

typedef enum {ACK_PAY_DISABLE = 0, ACK_PAY_ENABLE = !ACK_PAY_DISABLE} NrfAckPayType;

typedef enum { ST_PWR_DOWN = 0 , ST_STAND_I, ST_STAND_II, ST_RX, ST_TX} NrfStateType;

typedef void (* pFnUserHookRfTest)(pRF_Test_t *);

#pragma pack(1)
typedef union _RF_FlowCtrl_t
{
  BYTE Data[2];
  struct
  {
    WORD        : 8;
    WORD nRTS   : 1;
    WORD        : 7;
  };
} RF_FlowCtrl_t;

#pragma pack()

RNF_DRV_EXTER LONG nRFTickCntr;

static BYTE WriteReg(BYTE Addr, const BYTE *pData, BYTE Size);
static BYTE ReadReg(BYTE Addr, BYTE *pData, BYTE Size);
static void FlushTx(void);
static void FlushRx(void);
static void WriteActReg(BYTE Addr, BYTE Data);
static void ReadActReg(BYTE Addr, BYTE * pData);

void NrfMode(NrfModeType);
void NrfPower(NrfPowerType);
void NrfCrc( NrfCrcType);
void NrfCrcEn( NrfCrcEnType);
void NrfMaskInt(BYTE);
void NrfUnmaskInt(BYTE);
void NrfEnAutoAck(BYTE);
void NrfDisAutoAck(BYTE);
void NrfEnRxAddr(BYTE);
void NrfDisRxAddr(BYTE);
void NrfAddrWidth(NrfAddrWidthType);
void NrfAutoRetDelay (NrfAutoDelayType);
void NrfAutoRetCount(BYTE);
void NrfChannel(BYTE);
BYTE NrfGetChannel(void);
void NrfLNA(NrfLnaType);
void NrfOutPower(NrfOutPowerType);
void NrfDataRate(NrfDataRateType);
void NrfClearInt(BYTE);
void NrfSetRxAddr(NRF_Pipes_t, const BYTE *, BYTE);
void NrfSetTxAddr(const BYTE *, BYTE);
void NrfRxPayWidth(NRF_Pipes_t, BYTE);
void NrfEnDynPay(BYTE);
void NrfDisDynPay(BYTE);
void NrfDynPay(NrfDynPayType);
void NrfDynAck(NrfDynAckType);
void NrfAckPay(NrfAckPayType);
void NrfClearRxBuffer(void);
void NrfClearTxBuffer(void);
BYTE NrfGetRxSize(void);
BYTE NrfGetPipeAvb(void);
NrfStateType NrfGetState(void);
BYTE NrfGetFifoStatus(void);
BYTE NrfGetIntID(void);
void NrfReadFifo(BYTE*, BYTE);
void NrfWriteFifo(BYTE*, BYTE);

#endif // NRF_DRV_DEF
