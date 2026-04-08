/* ---------------------------------------------------------------------------- */
/*                Microchip Microcontroller Software Support                    */
/*                       SAM Software Package License                           */
/* ---------------------------------------------------------------------------- */
/* Copyright (c) 2019, Microchip Technology Inc.                                */
/*                                                                              */
/* All rights reserved.                                                         */
/*                                                                              */
/* Redistribution and use in source and binary forms, with or without           */
/* modification, are permitted provided that the following condition is met:    */
/*                                                                              */
/* - Redistributions of source code must retain the above copyright notice,     */
/* this list of conditions and the disclaimer below.                            */
/*                                                                              */
/* Microchip's name may not be used to endorse or promote products derived from */
/* this software without specific prior written permission.                     */
/*                                                                              */
/* DISCLAIMER:  THIS SOFTWARE IS PROVIDED BY MICROCHIP "AS IS" AND ANY EXPRESS  */
/* OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES */
/* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT    */
/* ARE DISCLAIMED. IN NO EVENT SHALL MICROCHIP BE LIABLE FOR ANY DIRECT,        */
/* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES           */
/* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; */
/* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND  */
/* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT   */
/* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF     */
/* THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.            */
/* ---------------------------------------------------------------------------- */

/**
 * \file
 *
 * Implementation of memories configuration on board.
 *
 */

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

#include "board.h"
#include "board_timer.h"
#include "timer.h"
#include "trace.h"

#include "irq/irq.h"
#include "gpio/pio.h"
#include "peripherals/pmc.h"
#include "extram/smc.h"
#include "peripherals/wdt.h"
#ifdef BOARD_DDRAM_TYPE
	#include "extram/ddram.h"
#endif
#ifdef BOARD_SDRAM_TYPE
	#include "extram/sdram.h"
#endif
#include "arm/mmu_cp15.h"
#include "mm/l1cache.h"

#include "board_support.h"
#include "string.h"
/*----------------------------------------------------------------------------
 *        Local constants
 *----------------------------------------------------------------------------*/
#define PLLA_DIV 1
#define PLLA_COUNT 0x3f
#define PLLA_LOOP_FILTER 0

#define PLLA_CFG(_mul, _fracr)			\
	{									\
		.mul = (_mul),					\
		.div = PLLA_DIV,				\
		.count = PLLA_COUNT,			\
		.fracr = (_fracr),				\
		.loop_filter = PLLA_LOOP_FILTER,\
	}

#define PLLA_FRACR(_p, _q) ((uint32_t)((((uint64_t)(_p)) << 22) / (_q)))

/*----------------------------------------------------------------------------
 *        Local variables
 *----------------------------------------------------------------------------*/
static const char* board_name = BOARD_NAME;

SECTION(".region_ddr") ALIGNED(16384) static uint32_t tlb[4096];

/*----------------------------------------------------------------------------
 *        Exported functions
 *----------------------------------------------------------------------------*/

const char* get_board_name(void)
{
	return board_name;
}

void board_cfg_clocks_for_preset(uint32_t preset)
{
	struct _pmc_plla_cfg plla_config;

#define MAX_PRESET 2 /* PCK/MCK (MHz) = { 600/200, 500/167 } */

	static const struct _pmc_plla_cfg plla_configs[][MAX_PRESET] = {
		/* MAINCK = 12MHz */
		{ PLLA_CFG(99, 0), PLLA_CFG(82, PLLA_FRACR(1, 3)) },

		/* MAINCK = 16MHz */
		{ PLLA_CFG(74, 0), PLLA_CFG(61, PLLA_FRACR(1, 2)) },

		/* MAINCK = 24MHz */
#ifdef CONFIG_HAVE_CLASSD
		{ PLLA_CFG(48, 0x9ba5e), PLLA_CFG(40, PLLA_FRACR(2, 3)) },
#else
		{ PLLA_CFG(49, 0), PLLA_CFG(40, PLLA_FRACR(2, 3)) },
#endif
		/* MAINCK = 48MHz */
		{ PLLA_CFG(24, 0), PLLA_CFG(19, PLLA_FRACR(5, 6)) },
	};
	uint32_t row;

	switch (pmc_get_main_oscillator_freq()) {
	default:
	case 12000000:
		row = 0;
		break;
	case 16000000:
		row = 1;
		break;
	case 24000000:
		row = 2;
		break;
	case 48000000:
		row = 3;
		break;
	}

	if (preset >= MAX_PRESET)
		trace_fatal("Invalid preset for board: %lu (max %d).\r\n", preset, MAX_PRESET);
	memcpy(&plla_config, &plla_configs[row][preset], sizeof(plla_config));

	pmc_switch_mck_to_main();
	pmc_set_mck_prescaler(PMC_MCKR_PRES_CLOCK);
	pmc_select_external_osc(false);
	pmc_configure_plla(&plla_config);
	pmc_set_mck_divider(PMC_MCKR_MDIV_PCK_DIV3);
	pmc_set_mck_prescaler(PMC_MCKR_PRES_CLOCK);
	pmc_switch_mck_to_pll();
}

void board_cfg_clocks(void)
{
	board_cfg_clocks_for_preset(0);
}

void board_cfg_lowlevel(bool clocks, bool ddram, bool mmu)
{
	/* Disable Watchdog */
	wdt_disable();

	/* Disable all PIO interrupts */
	pio_reset_all_it();

	/* Set the external oscillator frequency */
	pmc_set_main_oscillator_freq(BOARD_MAIN_CLOCK_EXT_OSC);

	if (clocks) {
		/* Configure system clocks */
		board_cfg_clocks();
	}

	/* Setup default interrupt handlers */
	irq_initialize();

	/* Configure system timer */
	board_cfg_timer();

	if (ddram) {
		/* Configure DDRAM */
		board_cfg_ddram();
	}

	if (mmu) {
		/* Setup MMU */
		board_cfg_mmu();
	}
}

void board_restore_pio_reset_state(void)
{
	int i;

	/* all pins */
	struct _pin pins[] = {
		{ PIO_GROUP_A, 0xFFFFFFFF, PIO_INPUT, PIO_PULLUP },
		{ PIO_GROUP_B, 0xFFFFFFFF, PIO_INPUT, PIO_PULLUP },
		{ PIO_GROUP_C, 0xFFFFFFFF, PIO_INPUT, PIO_PULLUP },
		{ PIO_GROUP_D, 0xFFFFFFFF, PIO_INPUT, PIO_PULLUP },
	};

	pio_configure(pins, ARRAY_SIZE(pins));
	for (i = 0; i < ARRAY_SIZE(pins); i++)
		pio_clear(&pins[i]);
}

void board_save_misc_power(void)
{
	int i;
	int tc_id = get_tc_id_from_addr(BOARD_TIMER_TC, BOARD_TIMER_CHANNEL);

	/* disable USB clock */
	pmc_disable_upll_clock();

	/* disable system clocks */
#ifdef VARIANT_SRAM
	pmc_disable_system_clock(PMC_SYSTEM_CLOCK_DDR);
#endif
	pmc_disable_system_clock(PMC_SYSTEM_CLOCK_UHP);
	pmc_disable_system_clock(PMC_SYSTEM_CLOCK_PCK0);
	pmc_disable_system_clock(PMC_SYSTEM_CLOCK_PCK1);
	pmc_disable_system_clock(PMC_SYSTEM_CLOCK_QSPI);

	/* disable all peripheral clocks except PIOA for JTAG, serial debug port */
	for (i = ID_PIT; i < ID_PERIPH_COUNT; i++) {
		if (i == ID_PIOA)
			continue;
		if (i == tc_id)
			continue;
#ifdef VARIANT_DDRAM
		if (i == ID_MPDDRC)
			continue;
#endif
		pmc_disable_peripheral(i);
	}
}

void board_cfg_mmu(void)
{
	uint32_t addr;

	if (mmu_is_enabled())
		return;

	/* TODO: some peripherals are configured TTB_SECT_STRONGLY_ORDERED
	   instead of TTB_SECT_SHAREABLE_DEVICE because their drivers have to
	   be verified for correct operation when write-back is enabled */

	/* Reset table entries */
	for (addr = 0; addr < 4096; addr++)
		tlb[addr] = 0;

	/* 0x00000000: SRAM (Remapped) */
	tlb[0x000] = TTB_SECT_ADDR(0x00000000)
	           | TTB_SECT_AP_FULL_ACCESS
	           | TTB_SECT_DOMAIN(0xf)
	           | TTB_SECT_CACHEABLE_WB
	           | TTB_SECT_SBO
	           | TTB_TYPE_SECT;

	/* 0x00100000: ROM */
	tlb[0x001] = TTB_SECT_ADDR(0x00100000)
	           | TTB_SECT_AP_FULL_ACCESS
	           | TTB_SECT_DOMAIN(0xf)
	           | TTB_SECT_CACHEABLE_WB
	           | TTB_SECT_SBO
	           | TTB_TYPE_SECT;

	/* 0x00300000: SRAM0 */
	tlb[0x003] = TTB_SECT_ADDR(0x00300000)
	           | TTB_SECT_AP_FULL_ACCESS
	           | TTB_SECT_DOMAIN(0xf)
	           | TTB_SECT_CACHEABLE_WB
	           | TTB_SECT_SBO
	           | TTB_TYPE_SECT;

	/* 0x00400000: SRAM1 */
	tlb[0x004] = TTB_SECT_ADDR(0x00400000)
	           | TTB_SECT_AP_FULL_ACCESS
	           | TTB_SECT_DOMAIN(0xf)
	           | TTB_SECT_SHAREABLE_DEVICE
	           | TTB_SECT_SBO
	           | TTB_TYPE_SECT;

#ifdef CONFIG_HAVE_UDPHS
	/* 0x00500000: UDPHS RAM */
	tlb[0x005] = TTB_SECT_ADDR(0x00500000)
	           | TTB_SECT_AP_FULL_ACCESS
	           | TTB_SECT_DOMAIN(0xf)
	           | TTB_SECT_SHAREABLE_DEVICE
	           | TTB_SECT_SBO
	           | TTB_TYPE_SECT;

	/* 0x00600000: UHP (OHCI) */
	tlb[0x006] = TTB_SECT_ADDR(0x00600000)
	           | TTB_SECT_AP_FULL_ACCESS
	           | TTB_SECT_DOMAIN(0xf)
	           | TTB_SECT_SHAREABLE_DEVICE
	           | TTB_SECT_SBO
	           | TTB_TYPE_SECT;

	/* 0x00700000: UHP (EHCI) */
	tlb[0x007] = TTB_SECT_ADDR(0x00700000)
	           | TTB_SECT_AP_FULL_ACCESS
	           | TTB_SECT_DOMAIN(0xf)
	           | TTB_SECT_SHAREABLE_DEVICE
	           | TTB_SECT_SBO
	           | TTB_TYPE_SECT;
#endif /* CONFIG_HAVE_UDPHS */

	/* 0x10000000: EBI Chip Select 0 */
	for (addr = 0x100; addr < 0x200; addr++)
		tlb[addr] = TTB_SECT_ADDR(addr << 20)
	                  | TTB_SECT_AP_FULL_ACCESS
	                  | TTB_SECT_DOMAIN(0xf)
	                  | TTB_SECT_STRONGLY_ORDERED
	                  | TTB_SECT_SBO
	                  | TTB_TYPE_SECT;

	/* 0x20000000: EBI Chip Select 1 / DDR CS */
	/* (64MB cacheable, 192MB strongly ordered) */
	for (addr = 0x200; addr < 0x240; addr++)
		tlb[addr] = TTB_SECT_ADDR(addr << 20)
	                  | TTB_SECT_AP_FULL_ACCESS
	                  | TTB_SECT_DOMAIN(0xf)
	                  | TTB_SECT_CACHEABLE_WB
	                  | TTB_SECT_SBO
	                  | TTB_TYPE_SECT;
	for (addr = 0x240; addr < 0x300; addr++)
		tlb[addr] = TTB_SECT_ADDR(addr << 20)
	                  | TTB_SECT_AP_FULL_ACCESS
	                  | TTB_SECT_DOMAIN(0xf)
	                  | TTB_SECT_STRONGLY_ORDERED
	                  | TTB_SECT_SBO
	                  | TTB_TYPE_SECT;

	/* 0x30000000: EBI Chip Select 2 */
	for (addr = 0x300; addr < 0x400; addr++)
		tlb[addr] = TTB_SECT_ADDR(addr << 20)
	                  | TTB_SECT_AP_FULL_ACCESS
	                  | TTB_SECT_DOMAIN(0xf)
	                  | TTB_SECT_STRONGLY_ORDERED
	                  | TTB_SECT_SBO
	                  | TTB_TYPE_SECT;

	/* 0x40000000: EBI Chip Select 3 */
	for (addr = 0x400; addr < 0x500; addr++)
		tlb[addr] = TTB_SECT_ADDR(addr << 20)
	                  | TTB_SECT_AP_FULL_ACCESS
	                  | TTB_SECT_DOMAIN(0xf)
	                  | TTB_SECT_STRONGLY_ORDERED
	                  | TTB_SECT_SBO
	                  | TTB_TYPE_SECT;

	/* 0x50000000: EBI Chip Select 4 */
	for (addr = 0x500; addr < 0x600; addr++)
		tlb[addr] = TTB_SECT_ADDR(addr << 20)
	                  | TTB_SECT_AP_FULL_ACCESS
	                  | TTB_SECT_DOMAIN(0xf)
	                  | TTB_SECT_STRONGLY_ORDERED
	                  | TTB_SECT_SBO
	                  | TTB_TYPE_SECT;

	/* 0x60000000: EBI Chip Select 5 */
	for (addr = 0x600; addr < 0x700; addr++)
		tlb[addr] = TTB_SECT_ADDR(addr << 20)
	                  | TTB_SECT_AP_FULL_ACCESS
	                  | TTB_SECT_DOMAIN(0xf)
	                  | TTB_SECT_STRONGLY_ORDERED
	                  | TTB_SECT_SBO
	                  | TTB_TYPE_SECT;

	/* 0x70000000: QSPI0/1 AESB MEM */
	for (addr = 0x700; addr < 0x800; addr++)
		tlb[addr] = TTB_SECT_ADDR(addr << 20)
	                  | TTB_SECT_AP_FULL_ACCESS
	                  | TTB_SECT_DOMAIN(0xf)
#if defined(VARIANT_QSPI0)
	                  | TTB_SECT_CACHEABLE_WB
#else
	                  | TTB_SECT_STRONGLY_ORDERED
#endif
	                  | TTB_TYPE_SECT;

	/* 0x80000000: SDMMC0 */
	for (addr = 0x800; addr < 0x900; addr++)
		tlb[addr] = TTB_SECT_ADDR(addr << 20)
	                  | TTB_SECT_AP_FULL_ACCESS
	                  | TTB_SECT_DOMAIN(0xf)
	                  | TTB_SECT_STRONGLY_ORDERED
	                  | TTB_TYPE_SECT;

	/* 0x90000000: SDMMC1 */
	for (addr = 0x900; addr < 0xa00; addr++)
		tlb[addr] = TTB_SECT_ADDR(addr << 20)
	                  | TTB_SECT_AP_FULL_ACCESS
	                  | TTB_SECT_DOMAIN(0xf)
	                  | TTB_SECT_STRONGLY_ORDERED
	                  | TTB_TYPE_SECT;

	/* 0xeff00000: OTPC */
	tlb[0xeff] = TTB_SECT_ADDR(0xeff00000)
	           | TTB_SECT_AP_FULL_ACCESS
	           | TTB_SECT_DOMAIN(0xf)
	           | TTB_SECT_STRONGLY_ORDERED
	           | TTB_SECT_SBO
	           | TTB_TYPE_SECT;

	/* 0xf0000000: Peripherals */
	tlb[0xf00] = TTB_SECT_ADDR(0xf0000000)
	           | TTB_SECT_AP_FULL_ACCESS
	           | TTB_SECT_DOMAIN(0xf)
	           | TTB_SECT_STRONGLY_ORDERED
	           | TTB_SECT_SBO
	           | TTB_TYPE_SECT;

	/* 0xf8000000: Peripherals */
	tlb[0xf80] = TTB_SECT_ADDR(0xf8000000)
	           | TTB_SECT_AP_FULL_ACCESS
	           | TTB_SECT_DOMAIN(0xf)
	           | TTB_SECT_STRONGLY_ORDERED
	           | TTB_SECT_SBO
	           | TTB_TYPE_SECT;

	/* 0xfff0000: System Controller */
	tlb[0xfff] = TTB_SECT_ADDR(0xfff00000)
	           | TTB_SECT_AP_FULL_ACCESS
	           | TTB_SECT_DOMAIN(0xf)
	           | TTB_SECT_STRONGLY_ORDERED
	           | TTB_SECT_SBO
	           | TTB_TYPE_SECT;

	/* Enable MMU, I-Cache and D-Cache */
	mmu_configure(tlb);
	icache_enable();
	mmu_enable();
	dcache_enable();
}


#define VDDIOM_1V8_OUT_Z_CALN_TYP 4
#define VDDIOM_1V8_OUT_Z_CALP_TYP 10

void board_cfg_matrix_for_ddr(void)
{
	SFR->SFR_CCFG_EBICSA |= SFR_CCFG_EBICSA_EBI_CS1A;
}


void board_cfg_ddram(void)
{
	board_cfg_matrix_for_ddr();
#ifdef BOARD_DDRAM_TYPE
	struct _mpddrc_desc desc;
	ddram_init_descriptor(&desc, BOARD_DDRAM_TYPE);
	ddram_configure(&desc);
#endif
#ifdef BOARD_SDRAM_TYPE
	struct _sdramc_desc desc;
	sdram_init_descriptor(&desc, BOARD_DDRAM_TYPE);
	sdram_configure(&desc);
#endif
}
