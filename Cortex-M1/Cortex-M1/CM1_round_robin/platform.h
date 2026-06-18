/*******************************************************************************
 * Copyright Actel Corporation.
 *
 *  This header file contains the base address of all the peripherals in the
 *  system.
 */

#ifndef PLATFORM_HEADER
#define PLATFORM_HEADER

/*------------------------------------------------------------------------------
 * Peripherals memory map:
 */
#ifndef DEBUG_IN_RAM
#define EXT_SRAM_BASE_ADDR		0x10000000UL
#else
#define EXT_SRAM_BASE_ADDR		0x00000000UL
#endif

#define EXT_FLASH_BASE_ADDR		0x20000000UL
#define CORE_AHB_NVM_BASE_ADDR	0x40000000UL
#define CORE_AHB_SRAM_BASE_ADDR	0x30000000UL

#define CORE_INT_BASE_ADDR		0xC0000000UL
#define COREAI_BASE_ADDR		0xC1000000UL
#define GPIO_BASE_ADDR      	0xC2000000UL
#define UART_BASE_ADDR			0xC3000000UL
#define TIMER_BASE_ADDR			0xC4000000UL
#define A3P_UART_BASE_ADDR		0xC5000000UL
#define CORE_PWM_BASE_ADDR		0xC6000000UL
#define SMBUS_BASE_ADDR			0xC7000000UL

/* GPIO blocks used to program external SRAM FPGA: */
#define SELECTMAP_CTRL_ADDR		0xC8000000UL
/*#define SELECTMAP_CTRL_ADDR		0xCA000000*/
#define SELECTMAP_DATA_ADDR		0xC9000000UL

/* GPIO peripheral used to bit-bang JTAG for programming A3P250: */
#define DIRECTC_BASE_ADDR		0xCA000000UL
/*#define DIRECTC_BASE_ADDR		0xC8000000*/

/*------------------------------------------------------------------------------
 * IRQ assignments:
 */
#define TX_RDY_IRQ_NB		0
#define RX_FULL_IRQ_NB		1
/*#define SMB1_IRQ_SOURCE		2*/
#define TIMER_IRQ_SOURCE	2
 
/*------------------------------------------------------------------------------
 * NVM locations usage:
 */
#define NVM_MARKER_SIZE			4
#define LOG_START_ADDRESS		CORE_AHB_NVM_BASE_ADDR
#define EVENT_LOG_SIZE			0x100
#define CAL_START_ADDRESS		(LOG_START_ADDRESS + EVENT_LOG_SIZE)
#define CALIBRATION_DATA_SIZE	128
#define CLEI_CODE_ADDRESS		(CAL_START_ADDRESS + CALIBRATION_DATA_SIZE + NVM_MARKER_SIZE)
#define CLEI_NVM_SIZE			8

/*------------------------------------------------------------------------------
 * SRAM reserved for transfering FPGA configuration image: Allow 1MB for 
 * stack (and debug image)
 */
/*#define FPGA_IMAGE_BASE_ADDR	EXT_SRAM_BASE_ADDR + 0x100000*/
/*#define FPGA_IMAGE_SRAM_SIZE	0x100000*/
#define FPGA_IMAGE_BUFF_ADDR	EXT_SRAM_BASE_ADDR + 0x100000
#define FPGA_IMAGE_BUFF_SIZE	0x100000

#endif /* PLATFORM_HEADER */
