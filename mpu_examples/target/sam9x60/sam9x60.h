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

#ifndef _SAM9X60_
#define _SAM9X60_

/** \addtogroup SAM9X60_definitions SAM9X60 definitions
  This file defines all structures and symbols for SAM9X60:
    - registers and bitfields
    - peripheral base address
    - peripheral ID
    - PIO definitions
*/
/*@{*/

#ifdef __cplusplus
 extern "C" {
#endif

#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#include <stdint.h>
#ifdef __cplusplus
  #define __I  volatile       /**< Defines 'read-only'  permissions */
#else
  #define __I  volatile const /**< Defines 'read-only'  permissions */
#endif
#define   __O  volatile       /**< Defines 'write-only' permissions */
#define   __IO volatile       /**< Defines 'read/write' permissions */
#endif

typedef enum IRQn
{
  EXT_FIQ_IRQn         =  0, /**<  0 Advanced Interrupt Controller (EXT_FIQ) */
  SYSC_IRQn            =  1, /**<  1 logical-OR interrupt of SYSC, PMC, DWDT, SHDWC, PIT, RTC (SYSC) */
  PIOA_IRQn            =  2, /**<  2 Parallel I/O Controller A (PIOA) */
  PIOB_IRQn            =  3, /**<  3 Parallel I/O Controller B (PIOB) */
  PIOC_IRQn            =  4, /**<  4 Parallel I/O Controller C (PIOC) */
  FLEXCOM0_IRQn        =  5, /**<  5 /!\\ was USART 0 - fGCLK < fMCK0 / 3 - False Path (FLEXCOM0) */
  FLEXCOM1_IRQn        =  6, /**<  6 /!\\ was USART 1 - fGCLK < fMCK0 / 3 - False Path (FLEXCOM1) */
  FLEXCOM2_IRQn        =  7, /**<  7 /!\\ was USART 2 - fGCLK < fMCK0 / 3 - False Path (FLEXCOM2) */
  FLEXCOM3_IRQn        =  8, /**<  8 /!\\ was USART 3 - fGCLK < fMCK0 / 3 - False Path (FLEXCOM3) */
  FLEXCOM6_IRQn        =  9, /**<  9 /!\\ was TWI 0 - fGCLK < fMCK0 / 3 - False Path (FLEXCOM6) */
  FLEXCOM7_IRQn        = 10, /**< 10 /!\\ was TWI 1 - fGCLK < fMCK0 / 3 - False Path (FLEXCOM7) */
  FLEXCOM8_IRQn        = 11, /**< 11 /!\\ was TWI 2 - fGCLK < fMCK0 / 3 - False Path (FLEXCOM8) */
  SDMMC0_IRQn          = 12, /**< 12 Secure Data Memory Card Controller 0 - There is a divider by two and we need 52MHz on the I/F clock. Miniminum requirement is 104MHz. (SDMMC0) */
  FLEXCOM4_IRQn        = 13, /**< 13 /!\\ was SPI 0 - fGCLK < fMCK0 / 3 - False Path (FLEXCOM4) */
  FLEXCOM5_IRQn        = 14, /**< 14 /!\\ was SPI 1 - fGCLK < fMCK0 / 3 - False Path (FLEXCOM5) */
  FLEXCOM9_IRQn        = 15, /**< 15 /!\\ was UART 0 - fGCLK < fMCK0 / 3 - False Path (FLEXCOM9) */
  FLEXCOM10_IRQn       = 16, /**< 16 /!\\ was UART 1 - fGCLK < fMCK0 / 3 - False Path (FLEXCOM10) */
  TC0_IRQn             = 17, /**< 17 TC 0,1,2 - fGCLK < fMCK0 / 3 - False Path (TC0) */
  PWM_IRQn             = 18, /**< 18 Pulse Width Modulation Controller (PWM) */
  ADC_IRQn             = 19, /**< 19 ADC Controller - fGCLK < fMCK0 / 3 - False Path (ADC) */
  XDMAC_IRQn           = 20, /**< 20 Extended DMA Controller (XDMAC) */
  MATRIX_IRQn          = 21, /**< 21 Matrix (MATRIX) */
  UHPHS_IRQn           = 22, /**< 22 USB Host High Speed (UHPHS) */
  UDPHS_IRQn           = 23, /**< 23 USB Device High Speed (UDPHS) */
  EMAC0_IRQn           = 24, /**< 24 Ethernet MAC 0 (EMAC0) */
  LCDC_IRQn            = 25, /**< 25 LCD Controller (LCDC) */
  SDMMC1_IRQn          = 26, /**< 26 Secure Data Memory Card Controller 1 - There is a divider by two and we need 52MHz on the I/F clock. Miniminum requirement is 104MHz. (SDMMC1) */
  EMAC1_IRQn           = 27, /**< 27 Ethernet MAC 1 (EMAC1) */
  SSC_IRQn             = 28, /**< 28 Synchronous Serial Controller (SSC) */
  CAN0_IRQn            = 29, /**< 29 CAN controller 0 (CAN0) */
  CAN1_IRQn            = 30, /**< 30 CAN controller 1 (CAN1) */
  EXT_IRQ_IRQn         = 31, /**< 31 Advanced Interrupt Controller (EXT_IRQ) */
  FLEXCOM11_IRQn       = 32, /**< 32 /!\\ new - UART addition - fGCLK < fMCK0 / 3 - False Path (FLEXCOM11) */
  FLEXCOM12_IRQn       = 33, /**< 33 /!\\ new - UART addition - fGCLK < fMCK0 / 3 - False Path (FLEXCOM12) */
  I2SMCC_IRQn          = 34, /**< 34 I2S Multi Channel Controller (I2SMCC) */
  QSPI_IRQn            = 35, /**< 35 Quad I/O SPI Controller (QSPI) */
  GFX2D_IRQn           = 36, /**< 36 2D Graphic Controller (GFX2D) */
  PIT64B_IRQn          = 37, /**< 37 64-b Timer - fGCLK < fMCK0 / 3 - False Path (PIT64B) */
  TRNG_IRQn            = 38, /**< 38 True Random Number Generator (TRNG) */
  AES_IRQn             = 39, /**< 39 Advanced Encryption Standard (AES) */
  TDES_IRQn            = 40, /**< 40 Triple Data Encryption Standard (TDES) */
  SHA_IRQn             = 41, /**< 41 Secure Hash Algorithm (SHA) */
  CLASSD_IRQn          = 42, /**< 42 CLASS D Controller (CLASSD) */
  ISI_IRQn             = 43, /**< 43 Image Sensor Interface (ISI) */
  PIOD_IRQn            = 44, /**< 44 Parallel I/O Controller D (PIOD) */
  TC1_IRQn             = 45, /**< 45 TC 3,4,5 - fGCLK < fMCK0 / 3 - False Path (TC1) */
  OTPC_IRQn            = 46, /**< 46 OTP Controller (OTPC) */
  DBGU_IRQn            = 47, /**< 47 DBGU - fGCLK < fMCK0 / 3 - False Path (DBGU) */
  PMECC_IRQn           = 48, /**< 48 logical-OR interrupt of PMECC and PMERRLOC (PMECC) */
  SDRAMC_IRQn          = 49, /**< 49 logical-OR interrupt of SDRAMC and MPDDRC and HSMC (SDRAMC) */
  UTMI_IRQn            = 50, /**< 50 UTMI (UTMI) */

  PERIPH_COUNT_IRQn    = 51  /**< Number of peripheral IDs */
} IRQn_Type;

/* ************************************************************************** */
/**  SOFTWARE PERIPHERAL API DEFINITION FOR SAM9X60 */
/* ************************************************************************** */
/** \addtogroup SAM9X60_api Peripheral Software API */
/*@{*/

#include "component/component_adc.h"
#include "component/component_aes.h"
#include "component/component_aic.h"
#include "component/component_bsc.h"
#include "component/component_can.h"
#include "component/component_classd.h"
#include "component/component_dbgu.h"
#include "component/component_emac.h"
#include "component/component_flexcom.h"
#include "component/component_gfx2d.h"
#include "component/component_gpbr.h"
#include "component/component_i2smcc.h"
#include "component/component_isi.h"
#include "component/component_lcdc.h"
#include "component/component_matrix.h"
#include "component/component_mpddrc.h"
#include "component/component_otpc.h"
#include "component/component_pio.h"
#include "component/component_pit.h"
#include "component/component_pit64b.h"
#include "component/component_pmc.h"
#include "component/component_pmecc.h"
#include "component/component_pmerrloc.h"
#include "component/component_pwm.h"
#include "component/component_qspi.h"
#include "component/component_rstc.h"
#include "component/component_rtc.h"
#include "component/component_rtt.h"
#include "component/component_sckc.h"
#include "component/component_sdmmc.h"
#include "component/component_sdramc.h"
#include "component/component_sfr.h"
#include "component/component_sha.h"
#include "component/component_shdwc.h"
#include "component/component_smc.h"
#include "component/component_ssc.h"
#include "component/component_syscwp.h"
#include "component/component_tc.h"
#include "component/component_tdes.h"
#include "component/component_trng.h"
#include "component/component_udphs.h"
#include "component/component_uhpfs.h"
#include "component/component_uhphs.h"
#include "component/component_wdt.h"
#include "component/component_xdmac.h"
/*@}*/

/* ************************************************************************** */
/*   REGISTER ACCESS DEFINITIONS FOR SAM9X60 */
/* ************************************************************************** */
/** \addtogroup SAM9X60_reg Registers Access Definitions */
/*@{*/

#include "instance/instance_uhphs_ohci.h"
#include "instance/instance_uhphs_ehci.h"
#include "instance/instance_sdmmc0.h"
#include "instance/instance_sdmmc1.h"
#include "instance/instance_otpc.h"
#include "instance/instance_flexcom4.h"
#include "instance/instance_flexcom5.h"
#include "instance/instance_xdmac.h"
#include "instance/instance_ssc.h"
#include "instance/instance_qspi.h"
#include "instance/instance_gfx2d.h"
#include "instance/instance_i2smcc.h"
#include "instance/instance_flexcom11.h"
#include "instance/instance_flexcom12.h"
#include "instance/instance_pit64b.h"
#include "instance/instance_sha.h"
#include "instance/instance_trng.h"
#include "instance/instance_aes.h"
#include "instance/instance_tdes.h"
#include "instance/instance_classd.h"
#include "instance/instance_can0.h"
#include "instance/instance_can1.h"
#include "instance/instance_tc0.h"
#include "instance/instance_tc1.h"
#include "instance/instance_flexcom6.h"
#include "instance/instance_flexcom7.h"
#include "instance/instance_flexcom8.h"
#include "instance/instance_flexcom0.h"
#include "instance/instance_flexcom1.h"
#include "instance/instance_flexcom2.h"
#include "instance/instance_flexcom3.h"
#include "instance/instance_emac0.h"
#include "instance/instance_emac1.h"
#include "instance/instance_pwm.h"
#include "instance/instance_lcdc.h"
#include "instance/instance_udphs.h"
#include "instance/instance_flexcom9.h"
#include "instance/instance_flexcom10.h"
#include "instance/instance_isi.h"
#include "instance/instance_adc.h"
#include "instance/instance_sfr.h"
#include "instance/instance_matrix.h"
#include "instance/instance_pmecc.h"
#include "instance/instance_pmerrloc.h"
#include "instance/instance_mpddrc.h"
#include "instance/instance_smc.h"
#include "instance/instance_sdramc.h"
#include "instance/instance_aic.h"
#include "instance/instance_dbgu.h"
#include "instance/instance_pioa.h"
#include "instance/instance_piob.h"
#include "instance/instance_pioc.h"
#include "instance/instance_piod.h"
#include "instance/instance_pmc.h"
#include "instance/instance_rstc.h"
#include "instance/instance_shdwc.h"
#include "instance/instance_rtt.h"
#include "instance/instance_pit.h"
#include "instance/instance_sckc.h"
#include "instance/instance_bsc.h"
#include "instance/instance_gpbr.h"
#include "instance/instance_rtc.h"
#include "instance/instance_syscwp.h"
#include "instance/instance_wdt.h"

#include "spi/_spi.h"
#include "serial/_usart.h"
#include "peripherals/_twi.h"
/*@}*/

/* ************************************************************************** */
/*   PERIPHERAL ID DEFINITIONS FOR SAM9X60 */
/* ************************************************************************** */
/** \addtogroup SAM9X60_id Peripheral Ids Definitions */
/*@{*/

#define ID_EXT_FIQ   ( 0) /**< \brief Advanced Interrupt Controller (EXT_FIQ) */
#define ID_SYSC      ( 1) /**< \brief logical-OR interrupt of SYSC, PMC, DWDT, SHDWC, PIT, RTC (SYSC) */
#define ID_PIT       ( 1) /**< \brief Periodic Interval Timer (PIT) */
#define ID_RSTC      ( 1) /**< \brief Reset Controller (RSTC) */
#define ID_RTC       ( 1) /**< \brief Real-time Clock (RTC) */
#define ID_RTT       ( 1) /**< \brief Real-time Timer (RTT) */
#define ID_WDT       ( 1) /**< \brief Watchdog Timer (WDT) */
#define ID_PIOA      ( 2) /**< \brief Parallel I/O Controller A (PIOA) */
#define ID_PIOB      ( 3) /**< \brief Parallel I/O Controller B (PIOB) */
#define ID_PIOC      ( 4) /**< \brief Parallel I/O Controller C (PIOC) */
#define ID_FLEXCOM0  ( 5) /**< \brief /!\\ was USART 0 - fGCLK < fMCK0 / 3 - False Path (FLEXCOM0) */
#define ID_FLEXCOM1  ( 6) /**< \brief /!\\ was USART 1 - fGCLK < fMCK0 / 3 - False Path (FLEXCOM1) */
#define ID_FLEXCOM2  ( 7) /**< \brief /!\\ was USART 2 - fGCLK < fMCK0 / 3 - False Path (FLEXCOM2) */
#define ID_FLEXCOM3  ( 8) /**< \brief /!\\ was USART 3 - fGCLK < fMCK0 / 3 - False Path (FLEXCOM3) */
#define ID_FLEXCOM6  ( 9) /**< \brief /!\\ was TWI 0 - fGCLK < fMCK0 / 3 - False Path (FLEXCOM6) */
#define ID_FLEXCOM7  (10) /**< \brief /!\\ was TWI 1 - fGCLK < fMCK0 / 3 - False Path (FLEXCOM7) */
#define ID_FLEXCOM8  (11) /**< \brief /!\\ was TWI 2 - fGCLK < fMCK0 / 3 - False Path (FLEXCOM8) */
#define ID_SDMMC0    (12) /**< \brief Secure Data Memory Card Controller 0 - There is a divider by two and we need 52MHz on the I/F clock. Miniminum requirement is 104MHz. (SDMMC0) */
#define ID_FLEXCOM4  (13) /**< \brief /!\\ was SPI 0 - fGCLK < fMCK0 / 3 - False Path (FLEXCOM4) */
#define ID_FLEXCOM5  (14) /**< \brief /!\\ was SPI 1 - fGCLK < fMCK0 / 3 - False Path (FLEXCOM5) */
#define ID_FLEXCOM9  (15) /**< \brief /!\\ was UART 0 - fGCLK < fMCK0 / 3 - False Path (FLEXCOM9) */
#define ID_FLEXCOM10 (16) /**< \brief /!\\ was UART 1 - fGCLK < fMCK0 / 3 - False Path (FLEXCOM10) */
#define ID_TC0       (17) /**< \brief TC 0,1,2 - fGCLK < fMCK0 / 3 - False Path (TC0) */
#define ID_PWM0      (18) /**< \brief Pulse Width Modulation Controller (PWM) */
#define ID_ADC       (19) /**< \brief ADC Controller - fGCLK < fMCK0 / 3 - False Path (ADC) */
#define ID_XDMAC0    (20) /**< \brief Extended DMA Controller (XDMAC) */
#define ID_MATRIX    (21) /**< \brief Matrix (MATRIX) */
#define ID_UHPHS     (22) /**< \brief USB Host High Speed (UHPHS) */
#define ID_UDPHS     (23) /**< \brief USB Device High Speed (UDPHS) */
#define ID_EMAC0     (24) /**< \brief Ethernet MAC 0 (EMAC0) */
#define ID_LCDC      (25) /**< \brief LCD Controller (LCDC) */
#define ID_SDMMC1    (26) /**< \brief Secure Data Memory Card Controller 1 - There is a divider by two and we need 52MHz on the I/F clock. Miniminum requirement is 104MHz. (SDMMC1) */
#define ID_EMAC1     (27) /**< \brief Ethernet MAC 1 (EMAC1) */
#define ID_SSC       (28) /**< \brief Synchronous Serial Controller (SSC) */
#define ID_CAN0      (29) /**< \brief CAN controller 0 (CAN0) */
#define ID_CAN1      (30) /**< \brief CAN controller 1 (CAN1) */
#define ID_EXT_IRQ   (31) /**< \brief Advanced Interrupt Controller (EXT_IRQ) */
#define ID_FLEXCOM11 (32) /**< \brief /!\\ new - UART addition - fGCLK < fMCK0 / 3 - False Path (FLEXCOM11) */
#define ID_FLEXCOM12 (33) /**< \brief /!\\ new - UART addition - fGCLK < fMCK0 / 3 - False Path (FLEXCOM12) */
#define ID_I2SMCC    (34) /**< \brief I2S Multi Channel Controller (I2SMCC) */
#define ID_QSPI0     (35) /**< \brief Quad I/O SPI Controller (QSPI) */
#define ID_GFX2D     (36) /**< \brief 2D Graphic Controller (GFX2D) */
#define ID_PIT64B    (37) /**< \brief 64-b Timer - fGCLK < fMCK0 / 3 - False Path (PIT64B) */
#define ID_TRNG      (38) /**< \brief True Random Number Generator (TRNG) */
#define ID_AES       (39) /**< \brief Advanced Encryption Standard (AES) */
#define ID_TDES      (40) /**< \brief Triple Data Encryption Standard (TDES) */
#define ID_SHA       (41) /**< \brief Secure Hash Algorithm (SHA) */
#define ID_CLASSD0   (42) /**< \brief CLASS D Controller (CLASSD) */
#define ID_ISI       (43) /**< \brief Image Sensor Interface (ISI) */
#define ID_PIOD      (44) /**< \brief Parallel I/O Controller D (PIOD) */
#define ID_TC1       (45) /**< \brief TC 3,4,5 - fGCLK < fMCK0 / 3 - False Path (TC1) */
#define ID_OTPC      (46) /**< \brief OTP Controller (OTPC) */
#define ID_DBGU      (47) /**< \brief DBGU - fGCLK < fMCK0 / 3 - False Path (DBGU) */
#define ID_PMECC     (48) /**< \brief logical-OR interrupt of PMECC and PMERRLOC (PMECC) */
#define ID_MPDDRC    (49) /**< \brief logical-OR interrupt of SDRAMC and MPDDRC and HSMC (SDRAMC) */
#define ID_UTMI      (50) /**< \brief UTMI (UTMI) */

#define ID_PERIPH_COUNT (51) /**< \brief Number of peripheral IDs */
/*@}*/

/* ************************************************************************** */
/*   BASE ADDRESS DEFINITIONS FOR SAM9X60 */
/* ************************************************************************** */
/** \addtogroup SAM9X60_base Peripheral Base Address Definitions */
/*@{*/

#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define UHPHS_OHCI (0x00600000U) /**< \brief (UHPHS_OHCI) Base Address */
#define UHPHS_EHCI (0x00700000U) /**< \brief (UHPHS_EHCI) Base Address */
#define SDMMC0     (0x80000000U) /**< \brief (SDMMC0    ) Base Address */
#define SDMMC1     (0x90000000U) /**< \brief (SDMMC1    ) Base Address */
#define OTPC       (0xEFF00000U) /**< \brief (OTPC      ) Base Address */
#define FLEXCOM4   (0xF0000000U) /**< \brief (FLEXCOM4  ) Base Address */
#define FLEXCOM5   (0xF0004000U) /**< \brief (FLEXCOM5  ) Base Address */
#define XDMAC0     (0xF0008000U) /**< \brief (XDMAC     ) Base Address */
#define SSC        (0xF0010000U) /**< \brief (SSC       ) Base Address */
#define QSPI0      (0xF0014000U) /**< \brief (QSPI      ) Base Address */
#define GFX2D      (0xF0018000U) /**< \brief (GFX2D     ) Base Address */
#define I2SMCC     (0xF001C000U) /**< \brief (I2SMCC    ) Base Address */
#define FLEXCOM11  (0xF0020000U) /**< \brief (FLEXCOM11 ) Base Address */
#define FLEXCOM12  (0xF0024000U) /**< \brief (FLEXCOM12 ) Base Address */
#define PIT64B     (0xF0028000U) /**< \brief (PIT64B    ) Base Address */
#define SHA        (0xF002C000U) /**< \brief (SHA       ) Base Address */
#define TRNG       (0xF0030000U) /**< \brief (TRNG      ) Base Address */
#define AES        (0xF0034000U) /**< \brief (AES       ) Base Address */
#define TDES       (0xF0038000U) /**< \brief (TDES      ) Base Address */
#define CLASSD     (0xF003C000U) /**< \brief (CLASSD    ) Base Address */
#define CAN0       (0xF8000000U) /**< \brief (CAN0      ) Base Address */
#define CAN1       (0xF8004000U) /**< \brief (CAN1      ) Base Address */
#define TC0        (0xF8008000U) /**< \brief (TC0       ) Base Address */
#define TC1        (0xF800C000U) /**< \brief (TC1       ) Base Address */
#define FLEXCOM6   (0xF8010000U) /**< \brief (FLEXCOM6  ) Base Address */
#define FLEXCOM7   (0xF8014000U) /**< \brief (FLEXCOM7  ) Base Address */
#define FLEXCOM8   (0xF8018000U) /**< \brief (FLEXCOM8  ) Base Address */
#define FLEXCOM0   (0xF801C000U) /**< \brief (FLEXCOM0  ) Base Address */
#define FLEXCOM1   (0xF8020000U) /**< \brief (FLEXCOM1  ) Base Address */
#define FLEXCOM2   (0xF8024000U) /**< \brief (FLEXCOM2  ) Base Address */
#define FLEXCOM3   (0xF8028000U) /**< \brief (FLEXCOM3  ) Base Address */
#define EMAC0      (0xF802C000U) /**< \brief (EMAC0     ) Base Address */
#define EMAC1      (0xF8030000U) /**< \brief (EMAC1     ) Base Address */
#define PWM        (0xF8034000U) /**< \brief (PWM       ) Base Address */
#define LCDC       (0xF8038000U) /**< \brief (LCDC      ) Base Address */
#define UDPHS      (0xF803C000U) /**< \brief (UDPHS     ) Base Address */
#define FLEXCOM9   (0xF8040000U) /**< \brief (FLEXCOM9  ) Base Address */
#define FLEXCOM10  (0xF8044000U) /**< \brief (FLEXCOM10 ) Base Address */
#define ISI        (0xF8048000U) /**< \brief (ISI       ) Base Address */
#define ADC        (0xF804C000U) /**< \brief (ADC       ) Base Address */
#define SFR        (0xF8050000U) /**< \brief (SFR       ) Base Address */
#define MATRIX     (0xFFFFDE00U) /**< \brief (MATRIX    ) Base Address */
#define PMECC      (0xFFFFE000U) /**< \brief (PMECC     ) Base Address */
#define PMERRLOC   (0xFFFFE600U) /**< \brief (PMERRLOC  ) Base Address */
#define MPDDRC     (0xFFFFE800U) /**< \brief (MPDDRC    ) Base Address */
#define SMC        (0xFFFFEA00U) /**< \brief (SMC       ) Base Address */
#define SDRAMC     (0xFFFFEC00U) /**< \brief (SDRAMC    ) Base Address */
#define AIC        (0xFFFFF100U) /**< \brief (AIC       ) Base Address */
#define DBGU       (0xFFFFF200U) /**< \brief (DBGU      ) Base Address */
#define PIOA       (0xFFFFF400U) /**< \brief (PIOA      ) Base Address */
#define PIOB       (0xFFFFF600U) /**< \brief (PIOB      ) Base Address */
#define PIOC       (0xFFFFF800U) /**< \brief (PIOC      ) Base Address */
#define PIOD       (0xFFFFFA00U) /**< \brief (PIOD      ) Base Address */
#define PMC        (0xFFFFFC00U) /**< \brief (PMC       ) Base Address */
#define RSTC       (0xFFFFFE00U) /**< \brief (RSTC      ) Base Address */
#define SHDWC      (0xFFFFFE10U) /**< \brief (SHDWC     ) Base Address */
#define RTT        (0xFFFFFE20U) /**< \brief (RTT       ) Base Address */
#define PIT        (0xFFFFFE40U) /**< \brief (PIT       ) Base Address */
#define SCKC       (0xFFFFFE50U) /**< \brief (SCKC      ) Base Address */
#define BSC        (0xFFFFFE54U) /**< \brief (BSC       ) Base Address */
#define GPBR       (0xFFFFFE60U) /**< \brief (GPBR      ) Base Address */
#define RTC        (0xFFFFFEA8U) /**< \brief (RTC       ) Base Address */
#define SYSCWP     (0xFFFFFEDCU) /**< \brief (SYSCWP    ) Base Address */
#define WDT        (0xFFFFFF80U) /**< \brief (WDT       ) Base Address */
#else
#define UHPHS_OHCI ((Uhpfs      *)0x00600000U) /**< \brief (UHPHS_OHCI) Base Address */
#define UHPHS_EHCI ((Uhphs      *)0x00700000U) /**< \brief (UHPHS_EHCI) Base Address */
#define SDMMC0     ((Sdmmc      *)0x80000000U) /**< \brief (SDMMC0    ) Base Address */
#define SDMMC1     ((Sdmmc      *)0x90000000U) /**< \brief (SDMMC1    ) Base Address */
#define OTPC       ((Otpc       *)0xEFF00000U) /**< \brief (OTPC      ) Base Address */
#define FLEXCOM4   ((Flexcom    *)0xF0000000U) /**< \brief (FLEXCOM4  ) Base Address */
#define FLEXUSART4 ((Usart      *)0xF0000200U) /**< \brief (FLEXUSART4) Base Address */
#define FLEXSPI4   ((Spi        *)0xF0000400U) /**< \brief (FLEXSPI4  ) Base Address */
#define FLEXTWI4   ((Twi        *)0xF0000600U) /**< \brief (FLEXTWI4  ) Base Address */
#define FLEXCOM5   ((Flexcom    *)0xF0004000U) /**< \brief (FLEXCOM5  ) Base Address */
#define FLEXUSART5 ((Usart      *)0xF0004200U) /**< \brief (FLEXUSART5) Base Address */
#define FLEXSPI5   ((Spi        *)0xF0004400U) /**< \brief (FLEXSPI5  ) Base Address */
#define FLEXTWI5   ((Twi        *)0xF0004600U) /**< \brief (FLEXTWI5  ) Base Address */
#define XDMAC0     ((Xdmac      *)0xF0008000U) /**< \brief (XDMAC     ) Base Address */
#define SSC        ((Ssc        *)0xF0010000U) /**< \brief (SSC       ) Base Address */
#define QSPI0      ((Qspi       *)0xF0014000U) /**< \brief (QSPI      ) Base Address */
#define GFX2D      ((Gfx2d      *)0xF0018000U) /**< \brief (GFX2D     ) Base Address */
#define I2SMCC     ((I2smcc     *)0xF001C000U) /**< \brief (I2SMCC    ) Base Address */
#define FLEXCOM11  ((Flexcom    *)0xF0020000U) /**< \brief (FLEXCOM11 ) Base Address */
#define FLEXUSART11 ((Usart     *)0xF0020200U) /**< \brief (FLEXUSART11) Base Address */
#define FLEXTWI11   ((Twi       *)0xF0020600U) /**< \brief (FLEXTWI11  ) Base Address */
#define FLEXCOM12  ((Flexcom    *)0xF0024000U) /**< \brief (FLEXCOM12 ) Base Address */
#define FLEXUSART12 ((Usart     *)0xF0024200U) /**< \brief (FLEXUSART12) Base Address */
#define FLEXTWI12   ((Twi       *)0xF0024600U) /**< \brief (FLEXTWI12  ) Base Address */
#define PIT64B     ((Pit64b     *)0xF0028000U) /**< \brief (PIT64B    ) Base Address */
#define SHA        ((Sha        *)0xF002C000U) /**< \brief (SHA       ) Base Address */
#define TRNG       ((Trng       *)0xF0030000U) /**< \brief (TRNG      ) Base Address */
#define AES        ((Aes        *)0xF0034000U) /**< \brief (AES       ) Base Address */
#define TDES       ((Tdes       *)0xF0038000U) /**< \brief (TDES      ) Base Address */
#define CLASSD0    ((Classd     *)0xF003C000U) /**< \brief (CLASSD    ) Base Address */
#define CAN0       ((Can        *)0xF8000000U) /**< \brief (CAN0      ) Base Address */
#define CAN1       ((Can        *)0xF8004000U) /**< \brief (CAN1      ) Base Address */
#define TC0        ((Tc         *)0xF8008000U) /**< \brief (TC0       ) Base Address */
#define TC1        ((Tc         *)0xF800C000U) /**< \brief (TC1       ) Base Address */
#define FLEXCOM6   ((Flexcom    *)0xF8010000U) /**< \brief (FLEXCOM6  ) Base Address */
#define FLEXUSART6 ((Usart      *)0xF8010200U) /**< \brief (FLEXUSART6) Base Address */
#define FLEXTWI6   ((Twi        *)0xF8010600U) /**< \brief (FLEXTWI6  ) Base Address */
#define FLEXCOM7   ((Flexcom    *)0xF8014000U) /**< \brief (FLEXCOM7  ) Base Address */
#define FLEXUSART7 ((Usart      *)0xF8014200U) /**< \brief (FLEXUSART7) Base Address */
#define FLEXTWI7   ((Twi        *)0xF8014600U) /**< \brief (FLEXTWI7  ) Base Address */
#define FLEXCOM8   ((Flexcom    *)0xF8018000U) /**< \brief (FLEXCOM8  ) Base Address */
#define FLEXUSART8 ((Usart      *)0xF8018200U) /**< \brief (FLEXUSART8) Base Address */
#define FLEXTWI8   ((Twi        *)0xF8018600U) /**< \brief (FLEXTWI8  ) Base Address */
#define FLEXCOM0   ((Flexcom    *)0xF801C000U) /**< \brief (FLEXCOM0  ) Base Address */
#define FLEXUSART0 ((Usart      *)0xF801C200U) /**< \brief (FLEXUSART0) Base Address */
#define FLEXSPI0   ((Spi        *)0xF801C400U) /**< \brief (FLEXSPI0  ) Base Address */
#define FLEXTWI0   ((Twi        *)0xF801C600U) /**< \brief (FLEXTWI0  ) Base Address */
#define FLEXCOM1   ((Flexcom    *)0xF8020000U) /**< \brief (FLEXCOM1  ) Base Address */
#define FLEXUSART1 ((Usart      *)0xF8020200U) /**< \brief (FLEXUSART1) Base Address */
#define FLEXSPI1   ((Spi        *)0xF8020400U) /**< \brief (FLEXSPI1  ) Base Address */
#define FLEXTWI1   ((Twi        *)0xF8020600U) /**< \brief (FLEXTWI1  ) Base Address */
#define FLEXCOM2   ((Flexcom    *)0xF8024000U) /**< \brief (FLEXCOM2  ) Base Address */
#define FLEXUSART2 ((Usart      *)0xF8024200U) /**< \brief (FLEXUSART2) Base Address */
#define FLEXSPI2   ((Spi        *)0xF8024400U) /**< \brief (FLEXSPI2  ) Base Address */
#define FLEXTWI2   ((Twi        *)0xF8024600U) /**< \brief (FLEXTWI2  ) Base Address */
#define FLEXCOM3   ((Flexcom    *)0xF8028000U) /**< \brief (FLEXCOM3  ) Base Address */
#define FLEXUSART3 ((Usart      *)0xF8028200U) /**< \brief (FLEXUSART3) Base Address */
#define FLEXSPI3   ((Spi        *)0xF8028400U) /**< \brief (FLEXSPI3  ) Base Address */
#define FLEXTWI3   ((Twi        *)0xF8028600U) /**< \brief (FLEXTWI3  ) Base Address */
#define EMAC0      ((Emac       *)0xF802C000U) /**< \brief (EMAC0     ) Base Address */
#define EMAC1      ((Emac       *)0xF8030000U) /**< \brief (EMAC1     ) Base Address */
#define PWM0       ((Pwm        *)0xF8034000U) /**< \brief (PWM0      ) Base Address */
#define LCDC       ((Lcdc       *)0xF8038000U) /**< \brief (LCDC      ) Base Address */
#define UDPHS      ((Udphs      *)0xF803C000U) /**< \brief (UDPHS     ) Base Address */
#define FLEXCOM9   ((Flexcom    *)0xF8040000U) /**< \brief (FLEXCOM9  ) Base Address */
#define FLEXUSART9 ((Usart      *)0xF8040200U) /**< \brief (FLEXUSART9) Base Address */
#define FLEXTWI9   ((Twi        *)0xF8040600U) /**< \brief (FLEXTWI9  ) Base Address */
#define FLEXCOM10  ((Flexcom    *)0xF8044000U) /**< \brief (FLEXCOM10 ) Base Address */
#define FLEXUSART10 ((Usart     *)0xF8044200U) /**< \brief (FLEXUSART10) Base Address */
#define FLEXTWI10   ((Twi       *)0xF8044600U) /**< \brief (FLEXTWI10  ) Base Address */
#define ISI        ((Isi        *)0xF8048000U) /**< \brief (ISI       ) Base Address */
#define ADC        ((Adc        *)0xF804C000U) /**< \brief (ADC       ) Base Address */
#define SFR        ((Sfr        *)0xF8050000U) /**< \brief (SFR       ) Base Address */
#define MATRIX     ((Matrix     *)0xFFFFDE00U) /**< \brief (MATRIX    ) Base Address */
#define PMECC      ((Pmecc      *)0xFFFFE000U) /**< \brief (PMECC     ) Base Address */
#define PMERRLOC   ((Pmerrloc   *)0xFFFFE600U) /**< \brief (PMERRLOC  ) Base Address */
#define MPDDRC     ((Mpddrc     *)0xFFFFE800U) /**< \brief (MPDDRC    ) Base Address */
#define SMC        ((Smc        *)0xFFFFEA00U) /**< \brief (SMC       ) Base Address */
#define SDRAMC     ((Sdramc     *)0xFFFFEC00U) /**< \brief (SDRAMC    ) Base Address */
#define AIC        ((Aic        *)0xFFFFF100U) /**< \brief (AIC       ) Base Address */
#define DBGU       ((Dbgu       *)0xFFFFF200U) /**< \brief (DBGU      ) Base Address */
#define PIOA       ((Pio        *)0xFFFFF400U) /**< \brief (PIOA      ) Base Address */
#define PIOB       ((Pio        *)0xFFFFF600U) /**< \brief (PIOB      ) Base Address */
#define PIOC       ((Pio        *)0xFFFFF800U) /**< \brief (PIOC      ) Base Address */
#define PIOD       ((Pio        *)0xFFFFFA00U) /**< \brief (PIOD      ) Base Address */
#define PMC        ((Pmc        *)0xFFFFFC00U) /**< \brief (PMC       ) Base Address */
#define RSTC       ((Rstc       *)0xFFFFFE00U) /**< \brief (RSTC      ) Base Address */
#define SHDWC      ((Shdwc      *)0xFFFFFE10U) /**< \brief (SHDWC     ) Base Address */
#define RTT        ((Rtt        *)0xFFFFFE20U) /**< \brief (RTT       ) Base Address */
#define PIT        ((Pit        *)0xFFFFFE40U) /**< \brief (PIT       ) Base Address */
#define SCKC       ((Sckc       *)0xFFFFFE50U) /**< \brief (SCKC      ) Base Address */
#define BSC        ((Bsc        *)0xFFFFFE54U) /**< \brief (BSC       ) Base Address */
#define GPBR       ((Gpbr       *)0xFFFFFE60U) /**< \brief (GPBR      ) Base Address */
#define RTC        ((Rtc        *)0xFFFFFEA8U) /**< \brief (RTC       ) Base Address */
#define SYSCWP     ((Syscwp     *)0xFFFFFEDCU) /**< \brief (SYSCWP    ) Base Address */
#define WDT        ((Wdt        *)0xFFFFFF80U) /**< \brief (WDT       ) Base Address */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */
/*@}*/

/* ************************************************************************** */
/*   PIO DEFINITIONS FOR SAM9X60 */
/* ************************************************************************** */
/** \addtogroup SAM9X60_pio Peripheral Pio Definitions */
/*@{*/

#include "pio/pio_sam9x60.h"
/*@}*/

/* ************************************************************************** */
/*   MEMORY MAPPING DEFINITIONS FOR SAM9X60 */
/* ************************************************************************** */

#define IRAM_SIZE (0x20000u)
#define ECC_ROM_ADDR    (0x00100000u) /**< ECC ROM base address */
#define IRAM0_ADDR      (0x00300000u) /**< Internal RAM 0 base address */
#define IRAM1_ADDR      (0x00400000u) /**< Internal RAM 1 base address */
#define UDPHS_RAM_ADDR  (0x00500000u) /**< USB High Speed Device Port RAM base address */
#define UHPHS_OHCI_ADDR (0x00600000u) /**< USB High Speed Device Port RAM base address */
#define UHPHS_EHCI_ADDR (0x00700000u) /**< USB High Speed Device Port RAM base address */
#define EBI_CS0_ADDR    (0x10000000u) /**< EBI Chip Select 0 base address */
#define EBI_CS1_ADDR    (0x20000000u) /**< EBI Chip Select 1 base address */
#define DDR_CS_ADDR     (0x20000000u) /**< MPDDR SDRAM Controller on EBI Chip Select 1 base address */
#define EBI_MPDDR_ADDR  (0x20000000u) /**< MPDDR on EBI Chip Select 1 base address */
#define EBI_SDRAM_ADDR  (0x20000000u) /**< SDRAM on EBI Chip Select 1 base address */
#define EBI_CS2_ADDR    (0x30000000u) /**< EBI Chip Select 2 base address */
#define EBI_CS3_ADDR    (0x40000000u) /**< EBI Chip Select 3 base address */
#define EBI_NF_ADDR     (0x40000000u) /**< NAND Flash on EBI Chip Select 3 base address */
#define EBI_CS4_ADDR    (0x50000000u) /**< EBI Chip Select 4 base address */
#define EBI_CS5_ADDR    (0x60000000u) /**< EBI Chip Select 5 base address */
#define QSPIMEM0_ADDR   (0x70000000u) /**< QSPI Memory base address */
#define SDMMC0_ADDR     (0x80000000u) /**< SDMMC 0 base address */
#define SDMMC1_ADDR     (0x90000000u) /**< SDMMC 1 base address */
#define OTPC_ADDR       (0xEFF00000u) /**< OTPC base address */
#define IRAM_ADDR       IRAM0_ADDR

/* ************************************************************************** */
/*   MISCELLANEOUS DEFINITIONS FOR SAM9X60 */
/* ************************************************************************** */

#define CHIP_JTAGID (0x05B4403FUL)
#define CHIP_CIDR   (0x819B81A0UL)
#define CHIP_EXID   (0x00000002UL)

/* ************************************************************************** */
/*   ELECTRICAL DEFINITIONS FOR SAM9X60 */
/* ************************************************************************** */

/* %ATMEL_ELECTRICAL% */

#ifdef __cplusplus
}
#endif

/*@}*/

#endif /* _SAM9X60_ */
