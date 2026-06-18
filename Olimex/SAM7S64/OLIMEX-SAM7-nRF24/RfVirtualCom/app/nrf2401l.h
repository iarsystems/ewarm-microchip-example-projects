/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2007
 *
 *    File name   : nrf2401l.h
 *    Description : nRF24L01 registers definitions.
 *
 *    History :
 *    1. Date        : 09.20.2007
 *       Author      : Stanimir Bonev
 *       Description : initial version
 *
 *    $Revision: 10 $
 **************************************************************************/

#ifndef __NRF2401L_H
#define __NRF2401L_H

//********************************************************************************************************************//
// SPI(nRF24L01) commands
#define READ_REG            0x00  // Define read command to register
#define WRITE_REG           0x20  // Define write command to register
#define RD_RX_PLOAD         0x61  // Define RX payload register address
#define WR_TX_PLOAD         0xA0  // Define TX payload register address
#define FLUSH_TX            0xE1  // Define flush TX register command
#define FLUSH_RX            0xE2  // Define flush RX register command
#define REUSE_TX_PL         0xE3  // Define reuse TX payload register command
#define NOP                 0xFF  // Define No Operation, might be used to read status register
#define ACTIVE              0x50  //
#define ACTIVE_DATA         0x73  //
#define RD_RX_PLOAD_WID     0x60  //
#define WR_ACK_PAYLOAD      0xA8  //
#define WR_TX_PAYLOAD_NACK  0xB0  //

//********************************************************************************************************************//
// SPI(nRF24L01) registers(addresses)
#define CONFIG          0x00  // 'Config' register address
#define PRIM_RX         (1<<0)
#define PWR_UP          (1<<1)
#define CRCO            (1<<2)
#define EN_CRC          (1<<3)
#define EN_AA           0x01  // 'Enable Auto Acknowledgment' register address
#define EN_RXADDR       0x02  // 'Enabled RX addresses' register address
#define SETUP_AW        0x03  // 'Setup address width' register address
#define SETUP_RETR      0x04  // 'Setup Auto. Retrans' register address
#define RF_CH           0x05  // 'RF channel' register address
#define RF_SETUP        0x06  // 'RF setup' register address
#define RF_LNA         (1<<0)
#define RF_DR          (1<<3)
#define STATUS          0x07  // 'Status' register address
#define TX_FULL         (1<<0)
#define RX_P_NO         (7<<1)
#define OBSERVE_TX      0x08  // 'Observe TX' register address
#define PLOS_CNT        0xF0
#define ARC_CNT         0x0F
#define CD              0x09  // 'Carrier Detect' register address
#define _CD             (1<<0)
#define RX_ADDR_P0      0x0A  // 'RX address pipe0' register address
#define RX_ADDR_P1      0x0B  // 'RX address pipe1' register address
#define RX_ADDR_P2      0x0C  // 'RX address pipe2' register address
#define RX_ADDR_P3      0x0D  // 'RX address pipe3' register address
#define RX_ADDR_P4      0x0E  // 'RX address pipe4' register address
#define RX_ADDR_P5      0x0F  // 'RX address pipe5' register address
#define TX_ADDR         0x10  // 'TX address' register address
#define RX_PW_P0        0x11  // 'RX payload width, pipe0' register address
#define RX_PW_P1        0x12  // 'RX payload width, pipe1' register address
#define RX_PW_P2        0x13  // 'RX payload width, pipe2' register address
#define RX_PW_P3        0x14  // 'RX payload width, pipe3' register address
#define RX_PW_P4        0x15  // 'RX payload width, pipe4' register address
#define RX_PW_P5        0x16  // 'RX payload width, pipe5' register address
#define FIFO_STATUS     0x17  // 'FIFO Status Register' register address
#define RX_FIFO_EMPTY   (1<<0)
#define RX_FIFO_FULL    (1<<1)
#define TX_FIFO_EMPTY   (1<<4)
#define TX_FIFO_FULL    (1<<5)

#define DYNPD           0x1C  // Enable dynamic payload length
#define DPL_P0          (1<<0)
#define DPL_P1          (1<<1)
#define DPL_P2          (1<<2)
#define DPL_P3          (1<<3)
#define DPL_P4          (1<<4)
#define DPL_P5          (1<<5)

#define FEATURE         0x1D  // Feature Register
#define EN_DYN_ACK      (1<<0)
#define EN_ACK_PAY      (1<<1)
#define EN_DPL          (1<<2)

typedef enum _NRF_Pipes_t
{
  NRF_PIPE0 = 0, NRF_PIPE1, NRF_PIPE2, NRF_PIPE3, NRF_PIPE4, NRF_PIPE5
} NRF_Pipes_t;

#endif // __NRF2401L_H
