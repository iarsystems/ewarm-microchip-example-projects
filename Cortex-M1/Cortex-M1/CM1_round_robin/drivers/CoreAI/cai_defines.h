/*******************************************************************************
 * (c) Copyright 2008 Actel Corporation
 * 
 * SVN $Revision: 5 $
 * SVN $Date: 2013-12-23 13:52:47 +0100 (mån, 23 dec 2013) $
 */
#ifndef COREAI_DEFINES_HEADER
#define COREAI_DEFINES_HEADER

/***************************************************************************//**
 * Defines used to derive the channel type (AV, AC, AG, AT) from the
 * g_cai_quads_config[] ACM/configuration table byte index.
 */
#define CHANNEL_NB_MASK             0x03
#define VOLTAGE_CHANNEL_NB          0x00
#define CURRENT_INPUT_CHANNEL_NB    0x01
#define GATE_DRIVER_CHANNEL_NB      0x02
#define TEMPERATURE_CHANNEL_NB      0x03

/***************************************************************************//**
 * Channel types.
 */
#define UNUSED_CHAN_TYPE        0x00
#define VOLTAGE_CHAN_TYPE       0x01
#define CURRENT_CHAN_TYPE       0x02
#define TEMPERATURE_CHAN_TYPE   0x03
#define NEGATIVE_CHAN_TYPE      0x04

#endif
