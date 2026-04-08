/*******************************************************************************
 * (c) Copyright 2008 Actel Corporation.  All rights reserved.
 *
 * Platform definitions.
 *
 * SVN $Revision: 5 $
 * SVN $Date: 2013-12-23 13:52:47 +0100 (mån, 23 dec 2013) $
 */
#ifndef __PLATFORM_H_
#define __PLATFORM_H_

/*******************************************************************************
 * RAM and NVM base addresses
 */
#ifndef RAM_DEBUG
  #define EXT_FLASH_BASE_ADDR		0x00000000UL
  #define EXT_SRAM_BASE_ADDR		0x10000000UL
#else
  #define EXT_SRAM_BASE_ADDR		0x00000000UL
  #define EXT_FLASH_BASE_ADDR		0x10000000UL
#endif	

#define COREAHBSRAM_BASE_ADDR		0x20000000UL
#define COREAHBNVM_BASE_ADDR		0x30000000UL

/*******************************************************************************
 * Peripherals base addresses
 */
#define CORETIMER0_BASE_ADDR        0xC0000000UL
#define COREINTERRUPT_BASE_ADDR     0xC1000000UL
#define COREGPIO_BASE_ADDR          0xC2000000UL
#define COREUARTAPB0_BASE_ADDR      0xC3000000UL
#define CORETIMER1_BASE_ADDR        0xC4000000UL
#define COREUARTAPB1_BASE_ADDR      0xC5000000UL
#define COREAI_BASE_ADDR            0xC6000000UL
#define COREREMAP_BASE_ADDR         0xCF000000UL

/*******************************************************************************
 * Interrupt numbers
 */
#define TIMER0_IRQ_NB           0
#define UART0_TXRDY_IRQ_NB      1
#define UART0_RXRDY_IRQ_NB      2
#define ANALOG_IRQ_NB           3
#define TIMER1_IRQ_NB           4
#define UART1_TXRDY_IRQ_NB      5
#define UART1_RXRDY_IRQ_NB      6

#endif /*__PLATFORM_H_*/
