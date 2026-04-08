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

#ifndef MPDDRC_HEADER_
#define MPDDRC_HEADER_

#include <stdint.h>

enum _ram_type {
#ifdef CONFIG_HAVE_MPDDRC_SDRAM
	MPDDRC_TYPE_SDRAM,
#endif
#ifdef CONFIG_HAVE_MPDDRC_LPDDR
	MPDDRC_TYPE_LPDDR,
#endif
#ifdef CONFIG_HAVE_MPDDRC_DDR2
	MPDDRC_TYPE_DDR2,
#endif
#ifdef CONFIG_HAVE_MPDDRC_LPDDR2
	MPDDRC_TYPE_LPDDR2,
#endif
#ifdef CONFIG_HAVE_MPDDRC_DDR3
	MPDDRC_TYPE_DDR3,
#endif
#ifdef CONFIG_HAVE_MPDDRC_LPDDR3
	MPDDRC_TYPE_LPDDR3,
#endif
};

struct _mpddrc_desc {
	enum _ram_type type;
#ifdef CONFIG_HAVE_MPDDRC_IO_CALIBRATION
	uint32_t io_calibr;
#endif
#ifdef CONFIG_HAVE_MPDDRC_DATA_PATH
	uint32_t data_path;
#endif
	uint32_t mode;
	uint32_t control;

	struct {
		uint8_t tmrd;   /**< Load Mode Register Command to Activate or Refresh Command */
		uint8_t twtr;   /**< Internal Write to Read Delay */
		uint8_t trrd;   /**< Active BankA to Active BankB */
		uint8_t trp;    /**< Row Precharge Delay */
		uint8_t trc;    /**< Row Cycle Delay */
		uint8_t twr;    /**< Write Recovery Delay */
		uint8_t trcd;   /**< Row to Column Delay */
		uint8_t tras;   /**< Active to Precharge Delay */
		uint8_t txp;    /**< Exit Powerdown Delay to First Command */
		uint8_t txsrd;  /**< Exit Self-refresh Delay to Read Command */
		uint8_t txsnr;  /**< Exit Self-refresh Delay to Non-Read Command */
		uint8_t trfc;   /**< Row Cycle Delay */
		uint8_t tfaw;   /**< Four Active Windows */
		uint8_t trtp;   /**< Read to Precharge */
		uint8_t trpa;   /**< Row Precharge All Delay */
		uint8_t txards; /**< Exit Active Powerdown Delay to Read Command in Mode "Slow Exit" */
		uint8_t txard;  /**< Exit Active Powerdown Delay to Read Command in Mode "Fast Exit" */
	} timings;

	uint32_t refresh_window; /* in ms */
	uint32_t refresh_cycles;
};

#if defined MPDDRC_CR_NC_DDR9_MDDR8_COL_BITS

#define MPDDRC_CR_NC_DDR_9_COL_BITS MPDDRC_CR_NC_DDR9_MDDR8_COL_BITS
#define MPDDRC_CR_NC_DDR_10_COL_BITS MPDDRC_CR_NC_DDR10_MDDR9_COL_BITS
#define MPDDRC_CR_NC_DDR_11_COL_BITS MPDDRC_CR_NC_DDR11_MDDR10_COL_BITS
#define MPDDRC_CR_NC_DDR_12_COL_BITS MPDDRC_CR_NC_DDR12_MDDR11_COL_BITS


#if defined MPDDRC_CR_DIC_DS_DDR2_WEAKSTRENGTH_DDR3_RZQ7
#define MPDDRC_CR_DIC_DS_DDR2_WEAKSTRENGTH MPDDRC_CR_DIC_DS_DDR2_WEAKSTRENGTH_DDR3_RZQ7
#endif

#define MPDDRC_CR_DIC_DS_DDR3_RZQ6 (0x0u << 8)
#define MPDDRC_CR_DIC_DS_DDR3_RZQ7 (0x1u << 8)

#define   MPDDRC_MR_MODE_CALIB_CMD (0x6u << 0)
#define   MPDDRC_MR_MODE_LPDDR2_CMD (0x7u << 0)
#define   MPDDRC_MR_MODE_LPDDR3_CMD (0x7u << 0)
#endif

#if defined MPDDRC_CR_NC_9_COL_BITS
#define MPDDRC_CR_NC_DDR_9_COL_BITS MPDDRC_CR_NC_9_COL_BITS
#define MPDDRC_CR_NC_DDR_10_COL_BITS MPDDRC_CR_NC_10_COL_BITS
#define MPDDRC_CR_NC_DDR_11_COL_BITS MPDDRC_CR_NC_11_COL_BITS
#define MPDDRC_CR_NC_DDR_12_COL_BITS MPDDRC_CR_NC_12_COL_BITS
#define MPDDRC_MD_DBW_DBW_16_BITS (0x1u << 4)
#endif

#if defined CONFIG_SOC_SAM9XX5
#define Mpddrc Ddrsdrc
#define MPDDRC_MR DDRSDRC_MR
#define MPDDRC_RTR DDRSDRC_RTR
#define MPDDRC_CR DDRSDRC_CR
#define MPDDRC_TPR0 DDRSDRC_TPR0
#define MPDDRC_TPR1 DDRSDRC_TPR1
#define MPDDRC_TPR2 DDRSDRC_TPR2
#define MPDDRC_LPR DDRSDRC_LPR
#define MPDDRC_MD DDRSDRC_MD
#define MPDDRC_DLL DDRSDRC_DLL
#define MPDDRC_HS DDRSDRC_HS

#define MPDDRC_MD_MD_SDR_SDRAM DDRSDRC_MD_MD(0)
#define MPDDRC_MD_MD_LPSDR_SDRAM DDRSDRC_MD_MD(1)
#define MPDDRC_MD_MD_LPDDR_SDRAM DDRSDRC_MD_MD(0)
#define MPDDRC_MD_MD_DDR2_SDRAM DDRSDRC_MD_MD(6)

#define MPDDRC_MD_DBW DDRSDRC_MD_DBW
#define MPDDRC_MD_DBW_DBW_32_BITS 0
#define MPDDRC_MD_DBW_DBW_16_BITS DDRSDRC_MD_DBW

#define MPDDRC_CR_NC_DDR_9_COL_BITS (0x0u << 0)
#define MPDDRC_CR_NC_DDR_10_COL_BITS (0x1u << 0)
#define MPDDRC_CR_NC_DDR_11_COL_BITS (0x2u << 0)
#define MPDDRC_CR_NC_DDR_12_COL_BITS (0x3u << 0)
#define MPDDRC_CR_NC_SDR_8_COL_BITS (0x0u << 0)
#define MPDDRC_CR_NC_SDR_9_COL_BITS (0x1u << 0)
#define MPDDRC_CR_NC_SDR_10_COL_BITS (0x2u << 0)
#define MPDDRC_CR_NC_SDR_11_COL_BITS (0x3u << 0)

#define MPDDRC_CR_NR_11_ROW_BITS (0x0u << 2)
#define MPDDRC_CR_NR_12_ROW_BITS (0x1u << 2)
#define MPDDRC_CR_NR_13_ROW_BITS (0x2u << 2) 
#define MPDDRC_CR_NR_14_ROW_BITS (0x3u << 2)

#define MPDDRC_CR_NC_Pos DDRSDRC_CR_NC_Pos
#define MPDDRC_CR_NC_Msk DDRSDRC_CR_NC_Msk
#define MPDDRC_CR_NC(value) DDRSDRC_CR_NC(value)

#define MPDDRC_CR_NR_Pos DDRSDRC_CR_NR_Pos
#define MPDDRC_CR_NR_Msk DDRSDRC_CR_NR_Msk
#define MPDDRC_CR_NR(value) DDRSDRC_CR_NR(value)

#define MPDDRC_CR_CAS_DDR_CAS3 (0x3u << 4)
#define MPDDRC_CR_CAS_SDR_CAS2 (0x2u << 4)
#define MPDDRC_CR_CAS_SDR_CAS3 (0x3u << 4)

#define MPDDRC_CR_NB_4_BANKS (0x0u << 20)
#define MPDDRC_CR_NB_8_BANKS (0x1u << 20)
#define MPDDRC_CR_DECOD_SEQUENTIAL (0x0u << 22)
#define MPDDRC_CR_DECOD_INTERLEAVED (0x1u << 22)

#define MPDDRC_CR_OCD_Pos DDRSDRC_CR_OCD_Pos
#define MPDDRC_CR_OCD_Msk DDRSDRC_CR_OCD_Msk
#define MPDDRC_CR_OCD(value) DDRSDRC_CR_OCD(value)

#define MPDDRC_TPR0_TRAS_Pos		DDRSDRC_TPR0_TRAS_Pos
#define MPDDRC_TPR0_TRAS_Msk		DDRSDRC_TPR0_TRAS_Msk
#define MPDDRC_TPR0_TRAS(value)		DDRSDRC_TPR0_TRAS(value)
#define MPDDRC_TPR0_TRCD_Pos		DDRSDRC_TPR0_TRCD_Pos
#define MPDDRC_TPR0_TRCD_Msk		DDRSDRC_TPR0_TRCD_Msk
#define MPDDRC_TPR0_TRCD(value)		DDRSDRC_TPR0_TRCD(value)
#define MPDDRC_TPR0_TWR_Pos			DDRSDRC_TPR0_TWR_Pos
#define MPDDRC_TPR0_TWR_Msk			DDRSDRC_TPR0_TWR_Msk
#define MPDDRC_TPR0_TWR(value)		DDRSDRC_TPR0_TWR(value)
#define MPDDRC_TPR0_TRC_Pos			DDRSDRC_TPR0_TRC_Pos
#define MPDDRC_TPR0_TRC_Msk			DDRSDRC_TPR0_TRC_Msk
#define MPDDRC_TPR0_TRC(value)		DDRSDRC_TPR0_TRC(value)
#define MPDDRC_TPR0_TRP_Pos			DDRSDRC_TPR0_TRP_Pos
#define MPDDRC_TPR0_TRP_Msk			DDRSDRC_TPR0_TRP_Msk
#define MPDDRC_TPR0_TRP(value)		DDRSDRC_TPR0_TRP(value)
#define MPDDRC_TPR0_TRRD_Pos		DDRSDRC_TPR0_TRRD_Pos
#define MPDDRC_TPR0_TRRD_Msk		DDRSDRC_TPR0_TRRD_Msk
#define MPDDRC_TPR0_TRRD(value)		DDRSDRC_TPR0_TRRD(value)
#define MPDDRC_TPR0_TWTR_Pos		DDRSDRC_TPR0_TWTR_Pos
#define MPDDRC_TPR0_TWTR_Msk		DDRSDRC_TPR0_TWTR_Msk
#define MPDDRC_TPR0_TWTR(value)		DDRSDRC_TPR0_TWTR(value)
#define MPDDRC_TPR0_RDC_WRRD		DDRSDRC_TPR0_RDC_WRRD
#define MPDDRC_TPR0_TMRD_Pos		DDRSDRC_TPR0_TMRD_Pos
#define MPDDRC_TPR0_TMRD_Msk		DDRSDRC_TPR0_TMRD_Msk
#define MPDDRC_TPR0_TMRD(value)		DDRSDRC_TPR0_TMRD(value)
#define MPDDRC_TPR1_TRFC_Pos 		DDRSDRC_TPR1_TRFC_Pos
#define MPDDRC_TPR1_TRFC_Msk		DDRSDRC_TPR1_TRFC_Msk
#define MPDDRC_TPR1_TRFC(value)		DDRSDRC_TPR1_TRFC(value)
#define MPDDRC_TPR1_TXSNR_Pos		DDRSDRC_TPR1_TXSNR_Pos
#define MPDDRC_TPR1_TXSNR_Msk		DDRSDRC_TPR1_TXSNR_Msk
#define MPDDRC_TPR1_TXSNR(value)	DDRSDRC_TPR1_TXSNR(value)
#define MPDDRC_TPR1_TXSRD_Pos		DDRSDRC_TPR1_TXSRD_Pos
#define MPDDRC_TPR1_TXSRD_Msk		DDRSDRC_TPR1_TXSRD_Msk
#define MPDDRC_TPR1_TXSRD(value)	DDRSDRC_TPR1_TXSRD(value)
#define MPDDRC_TPR1_TXP_Pos			DDRSDRC_TPR1_TXP_Pos
#define MPDDRC_TPR1_TXP_Msk			DDRSDRC_TPR1_TXP_Msk
#define MPDDRC_TPR1_TXP(value)		DDRSDRC_TPR1_TXP(value)
#define MPDDRC_TPR2_TXARD_Pos		DDRSDRC_TPR2_TXARD_Pos
#define MPDDRC_TPR2_TXARD_Msk		DDRSDRC_TPR2_TXARD_Msk
#define MPDDRC_TPR2_TXARD(value)	DDRSDRC_TPR2_TXARD(value)
#define MPDDRC_TPR2_TXARDS_Pos		DDRSDRC_TPR2_TXARDS_Pos
#define MPDDRC_TPR2_TXARDS_Msk		DDRSDRC_TPR2_TXARDS_Msk
#define MPDDRC_TPR2_TXARDS(value)	DDRSDRC_TPR2_TXARDS(value)
#define MPDDRC_TPR2_TRPA_Pos		DDRSDRC_TPR2_TRPA_Pos
#define MPDDRC_TPR2_TRPA_Msk		DDRSDRC_TPR2_TRPA_Msk
#define MPDDRC_TPR2_TRPA(value)		DDRSDRC_TPR2_TRPA(value)
#define MPDDRC_TPR2_TRTP_Pos		DDRSDRC_TPR2_TRTP_Pos
#define MPDDRC_TPR2_TRTP_Msk		DDRSDRC_TPR2_TRTP_Msk
#define MPDDRC_TPR2_TRTP(value)		DDRSDRC_TPR2_TRTP(value)
#define MPDDRC_TPR2_TFAW_Pos		DDRSDRC_TPR2_TFAW_Pos
#define MPDDRC_TPR2_TFAW_Msk		DDRSDRC_TPR2_TFAW_Msk
#define MPDDRC_TPR2_TFAW(value)		DDRSDRC_TPR2_TFAW(value)
#define MPDDRC_LPR_LPCB_Pos			DDRSDRC_LPR_LPCB_Pos
#define MPDDRC_LPR_LPCB_Msk			DDRSDRC_LPR_LPCB_Msk

#define MPDDRC_MR_MODE_Pos			DDRSDRC_MR_MODE_Pos
#define MPDDRC_MR_MODE_Msk			DDRSDRC_MR_MODE_Msk
#define MPDDRC_MR_MODE(value)		DDRSDRC_MR_MODE(value)
#define MPDDRC_MR_MODE_NORMAL_CMD	(0x0u << 0)
#define MPDDRC_MR_MODE_NOP_CMD		(0x1u << 0)
#define MPDDRC_MR_MODE_PRCGALL_CMD	(0x2u << 0)
#define MPDDRC_MR_MODE_LMR_CMD		(0x3u << 0)
#define MPDDRC_MR_MODE_RFSH_CMD		(0x4u << 0)
#define MPDDRC_MR_MODE_EXT_LMR_CMD 	(0x5u << 0)
#define MPDDRC_MR_MODE_DEEP_CMD		(0x6u << 0)

#define MPDDRC_RTR_COUNT_Pos DDRSDRC_RTR_COUNT_Pos
#define MPDDRC_RTR_COUNT_Msk DDRSDRC_RTR_COUNT_Msk
#define MPDDRC_RTR_COUNT(value) DDRSDRC_RTR_COUNT(value)

#define MPDDRC_CR_DLL_RESET_DISABLED (0x0u << 7)
#define MPDDRC_CR_DLL_RESET_ENABLED (0x1u << 7)
#define MPDDRC_LPR_LPCB_SELFREFRESH (0x1u << 0)
#define MPDDRC_LPR_LPCB_DISABLED (0x0u << 0)
#define MPDDRC_CR_OCD_DDR2_DEFAULT_CALIB (0x7u << 12)
#endif

extern void mpddrc_configure(struct _mpddrc_desc* desc);

/**
 * \brief Issue a Low-Power Command to the DDR-SDRAM device.
 *
 * \param cmd the Low-Power Command to send (one of the MPDDRC_LPR_LPCB_*
 * constants)
 */
extern void mpddrc_issue_low_power_command(uint32_t cmd);

void ddr_self_refresh(void);

void check_ddr_ready(void);

#endif
