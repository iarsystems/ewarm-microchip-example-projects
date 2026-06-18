/*******************************************************************************
 * (c) Copyright 2007 Actel Corporation.  All rights reserved.
 * 
 * CoreAI configuration.
 *
 * SVN $Revision: 5 $
 * SVN $Date: 2013-12-23 13:52:47 +0100 (mån, 23 dec 2013) $
 */
#include "hal.h"
#include "coreai.h"
#include "coreai_cfg.h"

/***************************************************************************//**
 * Analog quads configuration.
 */
const uint8_t g_cai_quads_config[NB_QUAD_CFG_BYTES] =
{
    /* --------------- Quad 0 --------------- */
    AV_4V,                             /* AV0 */
    AC_CURRENT_MONITOR,                /* AC0 */
    AG_30UA_SOURCE,                    /* AG0 */
    AT_TEMPERATURE_MONITOR,            /* AT0 */
    /* --------------- Quad 1 --------------- */
    AV_8V,                             /* AV1 */
    AC_CURRENT_MONITOR,                /* AC1 */
    AG_30UA_SOURCE,                    /* AG1 */
    AT_16V,                            /* AT1 */
    /* --------------- Quad 2 --------------- */
    AV_16V,                            /* AV2 */
    AC_CURRENT_MONITOR,                /* AC2 */
    AG_30UA_SOURCE,                    /* AG2 */
    AT_TEMPERATURE_MONITOR,            /* AT2 */
    /* --------------- Quad 3 --------------- */
    AV_16V,                            /* AV3 */
    AC_CURRENT_MONITOR,                /* AC3 */
    AG_30UA_SINK,                      /* AG3 */
    AT_TEMPERATURE_MONITOR,            /* AT3 */
    /* --------------- Quad 4 --------------- */
    AV_16V,                            /* AV4 */
    AC_CURRENT_MONITOR,                /* AC4 */
    AG_30UA_SOURCE,                    /* AG4 */
    AT_TEMPERATURE_MONITOR,            /* AT4 */
    /* --------------- Quad 5 --------------- */
    AV_MINUS_16V,                      /* AV5 */
    AC_8V,                             /* AC5 */
    AG_30UA_SOURCE,                    /* AG5 */
    AT_16V,                            /* AT5 */
    /* --------------- Quad 6 --------------- */
    AV_16V,                            /* AV6 */
    AC_8V,                             /* AC6 */
    AG_DISABLED,                       /* AG6 */
    AT_TEMPERATURE_MONITOR,            /* AT6 */
    /* --------------- Quad 7 --------------- */
    AV_8V,                             /* AV7 */
    AC_4V,                             /* AC7 */
    AG_DISABLED,                       /* AG7 */
    AT_16V,                            /* AT7 */
    /* --------------- Quad 8 --------------- */
    AV_4V,                             /* AV8 */
    AC_2V,                             /* AC8 */
    AG_DISABLED,                       /* AG8 */
    AT_TEMPERATURE_MONITOR,            /* AT8 */
    /* --------------- Quad 9 --------------- */
    AV_16V,                            /* AV9 */
    AC_CURRENT_MONITOR,                /* AC9 */
    AG_25MA_SINK,                       /* AG9 */
    AT_16V                            /* AT9 */
};

/***************************************************************************//**
 * Analog to digital converter configuration.
 */
const adc_settings_t g_cai_adc_config[ADC_NB_OF_CHANNELS] =
{
    /* adc_mode,    TVC,    STC*/
    { ADC_12_BIT,   0,      32},    /* ADC_CHAN_VCC */
    { ADC_12_BIT,   0,      32},    /* ADC_CHAN_AV0 */
    { ADC_12_BIT,   0,      32},    /* ADC_CHAN_AC0 */
    { ADC_12_BIT,   0,      32},    /* ADC_CHAN_AT0 */
    { ADC_12_BIT,   0,      32},    /* ADC_CHAN_AV1 */
    { ADC_12_BIT,   0,      32},    /* ADC_CHAN_AC1 */
    { ADC_12_BIT,   0,      32},    /* ADC_CHAN_AT1 */
    { ADC_12_BIT,   0,      32},    /* ADC_CHAN_AV2 */
    { ADC_12_BIT,   0,      32},    /* ADC_CHAN_AC2 */
    { ADC_12_BIT,   0,      32},    /* ADC_CHAN_AT2 */
    { ADC_12_BIT,   0,      32},    /* ADC_CHAN_AV3 */
    { ADC_12_BIT,   0,      32},    /* ADC_CHAN_AC3 */
    { ADC_12_BIT,   0,      32},    /* ADC_CHAN_AT3 */
    { ADC_12_BIT,   0,      32},    /* ADC_CHAN_AV4 */
    { ADC_12_BIT,   0,      32},    /* ADC_CHAN_AC4 */
    { ADC_12_BIT,   0,      32},    /* ADC_CHAN_AT4 */
    { ADC_12_BIT,   0,      32},    /* ADC_CHAN_AV5 */
    { ADC_12_BIT,   0,      32},    /* ADC_CHAN_AC5 */
    { ADC_12_BIT,   0,      32},    /* ADC_CHAN_AT5 */
    { ADC_12_BIT,   0,      32},    /* ADC_CHAN_AV6 */
    { ADC_12_BIT,   0,      32},    /* ADC_CHAN_AC6 */
    { ADC_12_BIT,   0,      32},    /* ADC_CHAN_AT6 */
    { ADC_12_BIT,   0,      32},    /* ADC_CHAN_AV7 */
    { ADC_12_BIT,   0,      32},    /* ADC_CHAN_AC7 */
    { ADC_12_BIT,   0,      32},    /* ADC_CHAN_AT7 */
    { ADC_12_BIT,   0,      32},    /* ADC_CHAN_AV8 */
    { ADC_12_BIT,   0,      32},    /* ADC_CHAN_AC8 */
    { ADC_12_BIT,   0,      32},    /* ADC_CHAN_AT8 */
    { ADC_12_BIT,   0,      32},    /* ADC_CHAN_AV9 */
    { ADC_12_BIT,   0,      32},    /* ADC_CHAN_AC9 */
    { ADC_12_BIT,   0,      32},    /* ADC_CHAN_AT9 */
    { ADC_12_BIT,   0,      32}     /* ADC_CHAN_INT_TEMP */
};

/***************************************************************************//**
 * User calibration m and c coefficients.
 */
#if defined(CAI_USER_CALIBRATION)

uint16_t g_cai_m_coef[ADC_NB_OF_CHANNELS] =
{
    1024,    /* ADC_CHAN_VCC */
    1024,    /* ADC_CHAN_AV0 */
    1024,    /* ADC_CHAN_AC0 */
    1024,    /* ADC_CHAN_AT0 */
    1024,    /* ADC_CHAN_AV1 */
    1024,    /* ADC_CHAN_AC1 */
    1024,    /* ADC_CHAN_AT1 */
    1024,    /* ADC_CHAN_AV2 */
    1024,    /* ADC_CHAN_AC2 */
    1024,    /* ADC_CHAN_AT2 */
    1024,    /* ADC_CHAN_AV3 */
    1024,    /* ADC_CHAN_AC3 */
    1024,    /* ADC_CHAN_AT3 */
    1024,    /* ADC_CHAN_AV4 */
    1024,    /* ADC_CHAN_AC4 */
    1024,    /* ADC_CHAN_AT4 */
    1024,    /* ADC_CHAN_AV5 */
    1024,    /* ADC_CHAN_AC5 */
    1024,    /* ADC_CHAN_AT5 */
    1024,    /* ADC_CHAN_AV6 */
    1024,    /* ADC_CHAN_AC6 */
    1024,    /* ADC_CHAN_AT6 */
    1024,    /* ADC_CHAN_AV7 */
    1024,    /* ADC_CHAN_AC7 */
    1024,    /* ADC_CHAN_AT7 */
    1024,    /* ADC_CHAN_AV8 */
    1024,    /* ADC_CHAN_AC8 */
    1024,    /* ADC_CHAN_AT8 */
    1024,    /* ADC_CHAN_AV9 */
    1024,    /* ADC_CHAN_AC9 */
    1024,    /* ADC_CHAN_AT9 */
    1024     /* ADC_CHAN_INT_TEMP */
};

uint16_t g_cai_c_coef[ADC_NB_OF_CHANNELS] =
{
    0,    /* ADC_CHAN_VCC */
    0,    /* ADC_CHAN_AV0 */
    0,    /* ADC_CHAN_AC0 */
    0,    /* ADC_CHAN_AT0 */
    0,    /* ADC_CHAN_AV1 */
    0,    /* ADC_CHAN_AC1 */
    0,    /* ADC_CHAN_AT1 */
    0,    /* ADC_CHAN_AV2 */
    0,    /* ADC_CHAN_AC2 */
    0,    /* ADC_CHAN_AT2 */
    0,    /* ADC_CHAN_AV3 */
    0,    /* ADC_CHAN_AC3 */
    0,    /* ADC_CHAN_AT3 */
    0,    /* ADC_CHAN_AV4 */
    0,    /* ADC_CHAN_AC4 */
    0,    /* ADC_CHAN_AT4 */
    0,    /* ADC_CHAN_AV5 */
    0,    /* ADC_CHAN_AC5 */
    0,    /* ADC_CHAN_AT5 */
    0,    /* ADC_CHAN_AV6 */
    0,    /* ADC_CHAN_AC6 */
    0,    /* ADC_CHAN_AT6 */
    0,    /* ADC_CHAN_AV7 */
    0,    /* ADC_CHAN_AC7 */
    0,    /* ADC_CHAN_AT7 */
    0,    /* ADC_CHAN_AV8 */
    0,    /* ADC_CHAN_AC8 */
    0,    /* ADC_CHAN_AT8 */
    0,    /* ADC_CHAN_AV9 */
    0,    /* ADC_CHAN_AC9 */
    0,    /* ADC_CHAN_AT9 */
    0     /* ADC_CHAN_INT_TEMP */
};

#endif

