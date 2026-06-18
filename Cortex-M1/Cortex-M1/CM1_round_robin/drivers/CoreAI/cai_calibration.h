/*******************************************************************************
 * (c) Copyright 2008 Actel Corporation
 *
 * CoreAI calibration related functions prototypes.
 * This header file only needs to be included within the CoreAI driver code.
 *
 * SVN $Revision: 5 $
 * SVN $Date: 2013-12-23 13:52:47 +0100 (mån, 23 dec 2013) $
 */
#ifndef COREAI_CALIBRATION_HEADER
#define COREAI_CALIBRATION_HEADER

/***************************************************************************//**
 *
 */
void init_calibration( void );

/***************************************************************************//**
 *
 */
static 
uint8_t get_cal_data_offset
(
    uint8_t cfg_byte_idx,
    uint16_t *cal_table_offset
);

/***************************************************************************//**
 *
 */
uint16_t
cal_adjust
(
    uint16_t adc_value,
    uint8_t channel_nb
);

#endif
