/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2008
 *
 *    File name   : nrf_drv_low.c
 *    Description : nRF24L01 low level driver include file
 *
 *    History :
 *    1. Date        : April 17 2008
 *       Author      : Stanimir Bonev
 *       Description : Initial Revision
 *    $Revision: 10 $
 **************************************************************************/

#include "includes.h"

#ifndef NRF_DRV_LOW_H
#define NRF_DRV_LOW_H

#ifdef RNF_DRV_LOW_GLOBAL
#define RNF_DRV_LOW_EXTER
#else
#define RNF_DRV_LOW_EXTER  extern
#endif

#ifndef BYTE
#define BYTE    Int8U
#endif

#ifndef WORD
#define WORD    Int16U
#endif

#ifndef DWORD
#define DWORD   Int32U
#endif

#ifndef LONG
#define LONG    Int32U
#endif

#ifndef ULONG
#define ULONG   Int32U
#endif

#ifndef BOOL
#define BOOL    Boolean
#endif

#define NRF24L_INTR_PRIORITY  3

#define NRF_TX_PULSE_DLY      40  // >10us

#define RF24L_GET_CE()        ((AT91C_BASE_PIOA->PIO_PDSR & NRF24L_CE_MASK) != 0)
#define RF24L_CE_L()          (AT91C_BASE_PIOA->PIO_CODR = NRF24L_CE_MASK)
#define RF24L_CE_H()	        (AT91C_BASE_PIOA->PIO_SODR = NRF24L_CE_MASK)

#define RF24L_CS_L()          (AT91C_BASE_PIOA->PIO_CODR = NRF24L_CS_MASK)
#define RF24L_CS_H()          (AT91C_BASE_PIOA->PIO_SODR = NRF24L_CS_MASK)

#define NRF_TRANSF_DATA(Data) Spi0TransferByte(Data)

RNF_DRV_LOW_EXTER Int32U Save;

BYTE SpiNrfSendData (BYTE Byte);
void nrf24l_init(void);
void nrf24l_ClrPendingIntr(void);

#endif // NRF_DRV_LOW_H
