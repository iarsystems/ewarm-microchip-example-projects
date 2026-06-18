/*******************************************************************************
 * (c) Copyright 2007 Actel Corporation.  All rights reserved.
 * 
 * CoreAI configuration.
 *
 * SVN $Revision: 5 $
 * SVN $Date: 2013-12-23 13:52:47 +0100 (mån, 23 dec 2013) $
 */
#ifndef COREAI_CONFIGURATION_HEADER
#define COREAI_CONFIGURATION_HEADER

/***************************************************************************//**
 * Specify which Fusion device the CoreAI driver is built to support.
 * The choices are:
 *      DIE_AFS1500
 *      DIE_AFS600
 *      DIE_AFS250
 *      DIE_AFS090
 * Only of these should be defined. Please note that whether a device is 
 * CortexM1 or CoreMP7 enabled is not relevant.
 */
#define DIE_AFS600

/***************************************************************************//**
 * Calibration mode selection.
 * There are three options:
 *      CAI_NO_CALIBRATION
 *      CAI_FACTORY_CALIBRATION
 *      CAI_USER_CALIBRATION
 *
 * You need to define one of the above symbols in order to specify how
 * calibration will be handled within the CoreAI drivers.
 *
 * Defining CAI_NO_CALIBRATION will cause no calibration adjustments to be
 * performed on the ADC conversion result.
 *
 * Defining CAI_FACTORY_CALIBRATION will cause the factory calibration data
 * stored in eNVM spare pages to be used to adjust the result of the ADC
 * conversion.
 *
 * Defining CAI_USER_CALIBRATION will cause the m and c coefficients stored in
 * g_cai_m_coef[] and g_cai_c_coef[] to be used for calibration adjustment. The 
 * initial values for these coefficient should be defined in "coreai_cfg.c".
 * The user application can set the content of these coefficients before calling
 * CAI_init() or while CoreAI is not sampling analog inputs.
 */
#define CAI_FACTORY_CALIBRATION

/***************************************************************************//**
 * Base address of the CoreAhbNVM instance in the system giving access to the
 * Fusion eNVM. This base address is only used when factory calibration is
 * enabled in order to locate the factory calibration data.
 */
#define CAI_CAL_NVM_BASE_ADDR   0x10000000UL

#endif
