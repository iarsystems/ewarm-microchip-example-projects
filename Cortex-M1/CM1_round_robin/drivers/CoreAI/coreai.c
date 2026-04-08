/*******************************************************************************
 * (c) Copyright 2006 Actel Corporation
 * 
 * CoreAI driver implementation.
 *
 * SVN $Revision: 5 $
 * SVN $Date: 2013-12-23 13:52:47 +0100 (mån, 23 dec 2013) $
 */
#include "hal.h"
#include "hal_assert.h"
#include "coreai.h"
#include "coreai_regs.h"
#include "coreai_cfg.h"
#include "cai_calibration.h"
#include "cai_defines.h"


/*------------------------------------------------------------------------------
 * Delays:
 * Counter values used to create 5us and 10us delays for timing the current and 
 * temperature sampling strobes.
 * These delays are calculated for an ARM7 running at 12.5MHz and should be 
 * adjusted if the PCLK frequency is higher.
 */
#define DELAY_5_US		3
#define DELAY_10_US		6
/*
#define DELAY_5_US		125
#define DELAY_10_US		250
*/
/*------------------------------------------------------------------------------
 * Base address value used to detect calls to CoreAI before it is initialised.
 * Use a value unlikely to be a real base address for CoreAI.
 */
#define UNINITIALISED_BASE_ADDR		0xDEADBEEFUL

/*------------------------------------------------------------------------------
 *
 */
#define NB_QUADS			10
#define NB_QUAD_CFG_BYTES	40
#define QUAD_CFG_ACM_START	1

/*------------------------------------------------------------------------------
 *	Analog to Digital converter configuration:
 */
#define ADC_TVC_CFG		0	
#define ADC_STC_CFG		32
#define ADC_MODE_CFG	1

/*------------------------------------------------------------------------------
 * Analog Configuration Mux (ACM) addresses:
 */
#define COUNTER0_ACM_ADDR	0x40
#define RTC_MATCH0_ACM_ADDR	0x48
#define CTRL_STAT_ACM_ADDR	0x58

#define ACM_COUNTER_BYTE_SIZE	5

/*------------------------------------------------------------------------------
 * ACM CRTL_STAT register bits:
 */
#define XTAL_EN			0x01
#define RSTB_CNT		0x02
#define RST_CNT_OMAT	0x04
#define XT_MODE_32KHZ	0x08

#define VR_EN_MAT		0x20
#define CNTR_EN			0x40
#define RTC_RST			0x80

/*------------------------------------------------------------------------------
 *
 */
#define TEMPERATURE_CHANNELS	( 	(1 << ADC_CHAN_AT0) | (1 << ADC_CHAN_AT1) | \
									(1 << ADC_CHAN_AT2) | (1 << ADC_CHAN_AT3) | \
									(1 << ADC_CHAN_AT4) | (1 << ADC_CHAN_AT5) | \
									(1 << ADC_CHAN_AT6) | (1 << ADC_CHAN_AT7) | \
									(1 << ADC_CHAN_AT8) | (1 << ADC_CHAN_AT9) )

#define CURRENT_CHANNELS		(	(1 << ADC_CHAN_AC0) | (1 << ADC_CHAN_AC1) | \
									(1 << ADC_CHAN_AC2) | (1 << ADC_CHAN_AC3) | \
									(1 << ADC_CHAN_AC4) | (1 << ADC_CHAN_AC5) | \
									(1 << ADC_CHAN_AC6) | (1 << ADC_CHAN_AC7) | \
									(1 << ADC_CHAN_AC8) | (1 << ADC_CHAN_AC9) )


/* Bit mask used to detect a negative polarity channel from an ACM configuration byte. */
#define NEGATIVE_CHANNEL_POLARITY	0x40

#define INVALID_CHANNEL     -1

/*------------------------------------------------------------------------------
 * Look up table used to get the byte index in the ACM configuration (acm_cfg[])
 * for a specified channel.
 */
static const int8_t chan_2_acm_cfg_idx_lut[ADC_NB_OF_CHANNELS] = 
{
    -1,		/* ADC_CHAN_VCC		0 */

    0,		/* ADC_CHAN_AV0		1 */
    1,		/* ADC_CHAN_AC0		2 */
    3,		/* ADC_CHAN_AT0		3 */

    4,		/* ADC_CHAN_AV1		4 */
    5,		/* ADC_CHAN_AC1		5 */
    7,		/* ADC_CHAN_AT1		6 */

    8,		/* ADC_CHAN_AV2		7 */
    9,		/* ADC_CHAN_AC2		8 */
    11,		/* ADC_CHAN_AT2		9 */

    12,		/* ADC_CHAN_AV3		10 */
    13,		/* ADC_CHAN_AC3		11 */
    15,		/* ADC_CHAN_AT3		12 */

    16,		/* ADC_CHAN_AV4		13 */
    17,		/* ADC_CHAN_AC4		14 */
    19,		/* ADC_CHAN_AT4		15 */

    20,		/* ADC_CHAN_AV5		16 */
    21,		/* ADC_CHAN_AC5		17 */
    23,		/* ADC_CHAN_AT5		18 */

    24,		/* ADC_CHAN_AV6		19 */
    25,		/* ADC_CHAN_AC6		20 */
    27,		/* ADC_CHAN_AT6		21 */

    28,		/* ADC_CHAN_AV7		22 */
    29,		/* ADC_CHAN_AC7		23 */
    31,		/* ADC_CHAN_AT7		24 */

    32,		/* ADC_CHAN_AV8		25 */
    33,		/* ADC_CHAN_AC8		26 */
    35,		/* ADC_CHAN_AT8		27 */

    36,		/* ADC_CHAN_AV9		28 */
    37,		/* ADC_CHAN_AC9		29 */
    39,		/* ADC_CHAN_AT9		30 */

    -1		/* ADC_CHAN_INT_TEMP	31 */
};

const uint8_t acm_cfg_idx_2_chan_nb_lut[NB_QUAD_CFG_BYTES] = 
{
    /* --------------- Quad 0 --------------- */
    ADC_CHAN_AV0,                      /* AV0 */
    ADC_CHAN_AC0,                      /* AC0 */
    INVALID_CHANNEL_NB,                /* AG0 */
    ADC_CHAN_AT0,                      /* AT0 */
    /* --------------- Quad 1 --------------- */
    ADC_CHAN_AV1,                      /* AV1 */
    ADC_CHAN_AC1,                      /* AC1 */
    ADC_NB_OF_CHANNELS,                /* AG1 */
    ADC_CHAN_AT1,                      /* AT1 */
    /* --------------- Quad 2 --------------- */
    ADC_CHAN_AV2,                      /* AV2 */
    ADC_CHAN_AC2,                      /* AC2 */
    INVALID_CHANNEL_NB,                /* AG2 */
    ADC_CHAN_AT2,                      /* AT2 */
    /* --------------- Quad 3 --------------- */
    ADC_CHAN_AV3,                      /* AV3 */
    ADC_CHAN_AC3,                      /* AC3 */
    INVALID_CHANNEL_NB,                /* AG3 */
    ADC_CHAN_AT3,                      /* AT3 */
    /* --------------- Quad 4 --------------- */
    ADC_CHAN_AV4,                      /* AV4 */
    ADC_CHAN_AC4,                      /* AC4 */
    INVALID_CHANNEL_NB,                /* AG4 */
    ADC_CHAN_AT4,                      /* AT4 */
    /* --------------- Quad 5 --------------- */
    ADC_CHAN_AV5,                      /* AV5 */
    ADC_CHAN_AC5,                      /* AC5 */
    INVALID_CHANNEL_NB,                /* AG5 */
    ADC_CHAN_AT5,                      /* AT5 */
    /* --------------- Quad 6 --------------- */
    ADC_CHAN_AV6,                      /* AV6 */
    ADC_CHAN_AC6,                      /* AC6 */
    INVALID_CHANNEL_NB,                /* AG6 */
    ADC_CHAN_AT6,                      /* AT6 */
    /* --------------- Quad 7 --------------- */
    ADC_CHAN_AV7,                      /* AV7 */
    ADC_CHAN_AC7,                      /* AC7 */
    INVALID_CHANNEL_NB,                /* AG7 */
    ADC_CHAN_AT7,                      /* AT7 */
    /* --------------- Quad 8 --------------- */
    ADC_CHAN_AV8,                      /* AV8 */
    ADC_CHAN_AC8,                      /* AC8 */
    INVALID_CHANNEL_NB,                /* AG8 */
    ADC_CHAN_AT8,                      /* AT8 */
    /* --------------- Quad 9 --------------- */
    ADC_CHAN_AV9,                      /* AV9 */
    ADC_CHAN_AC9,                      /* AC9 */
    INVALID_CHANNEL_NB,                /* AG9 */
    ADC_CHAN_AT9                       /* AT9 */
};

/***************************************************************************//**
 * Look up table used to derive the value of the Analog Configuration 
 * Multiplexers (ACM) configuration value for a voltage input from the selected
 * voltage input configuration specified in g_cai_quads_config[].
 */
static const uint8_t g_cai_volt_acm_cfg_lut[AV_NB_CFG_OPTIONS] =
{
    0x00,   /* AV_DISABLED */
    0x08,   /* AV_VAREF */
    0x80,   /* AV_16V */
    0x81,   /* AV_8V */
    0x82,   /* AV_4V */
    0x83,   /* AV_2V */
    0x84,   /* AV_1V */
    0x85,   /* AV_500MV */
    0x86,   /* AV_250MV */
    0x87,   /* AV_125MV */
    0xC0,   /* AV_MINUS_16V */
    0xC1,   /* AV_MINUS_8V */
    0xC2,   /* AV_MINUS_4V */
    0xC3,   /* AV_MINUS_2V */
    0xC4,   /* AV_MINUS_1V */
    0xC5,   /* AV_MINUS_500MV */
    0xC6,   /* AV_MINUS_250MV */
    0xC7,   /* AV_MINUS_125MV */
    0x20    /* AV_DIGITAL_INPUT */  
};

/***************************************************************************//**
 * Look up table used to derive the value of the Analog Configuration 
 * Multiplexers (ACM) configuration value for a current input from the selected
 * current input configuration specified in g_cai_quads_config[].
 */
static const uint8_t g_cai_curr_acm_cfg_lut[AC_NB_OF_CFG_OPTIONS] =
{
    0x00,   /* AC_DISABLED */
    0x08,   /* AC_VAREF */
    0x80,   /* AC_16V */
    0x81,   /* AC_8V */
    0x82,   /* AC_4V */
    0x83,   /* AC_2V */
    0x84,   /* AC_1V */
    0x85,   /* AC_500MV */
    0x86,   /* AC_250MV */
    0x87,   /* AC_125MV */
    0xC0,   /* AC_MINUS_16V */
    0xC1,   /* AC_MINUS_8V */
    0xC2,   /* AC_MINUS_4V */
    0xC3,   /* AC_MINUS_2V */
    0xC4,   /* AC_MINUS_1V */
    0xC5,   /* AC_MINUS_500MV */
    0xC6,   /* AC_MINUS_250MV */
    0xC7,   /* AC_MINUS_125MV */
    0x10,   /* AC_CURRENT_MONITOR */ /* WARNING!!! special case where the AV config must be or'ed with 0x10*/
    0x20    /* AC_DIGITAL_INPUT */  
};

/***************************************************************************//**
 * Look up table used to derive the value of the Analog Configuration 
 * Multiplexers (ACM) configuration value for a temperature input from the
 * selected temperature input configuration specified in g_cai_quads_config[].
 */
static const uint8_t g_cai_temp_acm_cfg_lut[AT_NB_OF_CFG_OPTIONS] =
{
    0x00,   /* AT_DISABLED */
    0x08,   /* AT_VAREF */
    0x80,   /* AT_16V */
    0x81,   /* AT_4V */
    0x10,   /* AT_TEMPERATURE_MONITOR */  /* WARNING!!! special case where the AG config must be or'ed with 0x10*/
    0x20    /* AT_DIGITAL_INPUT */
};

/***************************************************************************//**
 * Look up table used to derive the value of the Analog Configuration 
 * Multiplexers (ACM) configuration value for a gate driver output from the
 * selected gate driver output configuration specified in g_cai_quads_config[].
 */
static const uint8_t g_cai_gate_drv_acm_cfg_lut[AG_NB_OF_CFG_OPTIONS] =
{
    0x00,   /* AG_DISABLED */
    0x00,   /* AG_1UA_SOURCE */
    0x04,   /* AG_3UA_SOURCE */
    0x08,   /* AG_10UA_SOURCE */
    0x0C,   /* AG_30UA_SOURCE */
    0x80,   /* AG_25MA_SOURCE */
    0x40,   /* AG_1UA_SINK */
    0x44,   /* AG_3UA_SINK */
    0x48,   /* AG_10UA_SINK */
    0x4C,   /* AG_30UA_SINK */
    0xC0    /* AG_25MA_SINK */
};


/*------------------------------------------------------------------------------
 *	Analog inputs ADC full scale voltages.
 */
#define NB_OF_ADC_INPUT_RANGES	8

/* The values for these ranges were obtained from the quad's specification.
 * Do not confuse range name with actual voltage range. */
static const uint16_t g_cai_input_range_lut[NB_OF_ADC_INPUT_RANGES] =
{
	16368,	/* 16V */
	8184,	/* 8 V*/
	4092,	/* 4 V*/
	2046,	/* 2 V*/
	1023,	/* 1 V*/
	511,	/* 0.5 V*/
	255,	/* 0.25 V*/
	127		/* 0.125 V*/
};

static uint16_t g_cai_adc_ranges[ADC_NB_OF_CHANNELS];

/***************************************************************************//**
 * Look up table containing the strobe register value to be applied to the
 * CoreAI strobe registers for sampling current and temperature monitors.
 */
static const int16_t g_strobes_reg_lut[ADC_NB_OF_CHANNELS] = 
{
    0,		    /* ADC_CHAN_VCC		0 */

    0,		    /* ADC_CHAN_AV0		1 */
    0x0001,		/* ADC_CHAN_AC0		2 */
    0x0001,		/* ADC_CHAN_AT0		3 */

    0,		    /* ADC_CHAN_AV1		4 */
    0x0002,		/* ADC_CHAN_AC1		5 */
    0x0002,		/* ADC_CHAN_AT1		6 */

    0,		    /* ADC_CHAN_AV2		7 */
    0x0004,		/* ADC_CHAN_AC2		8 */
    0x0004,		/* ADC_CHAN_AT2		9 */

    0,		    /* ADC_CHAN_AV3		10 */
    0x0008,		/* ADC_CHAN_AC3		11 */
    0x0008,		/* ADC_CHAN_AT3		12 */

    0,		    /* ADC_CHAN_AV4		13 */
    0x0010,		/* ADC_CHAN_AC4		14 */
    0x0010,		/* ADC_CHAN_AT4		15 */

    0,		    /* ADC_CHAN_AV5		16 */
    0x0020,		/* ADC_CHAN_AC5		17 */
    0x0020,		/* ADC_CHAN_AT5		18 */

    0,		    /* ADC_CHAN_AV6		19 */
    0x0040,		/* ADC_CHAN_AC6		20 */
    0x0040,		/* ADC_CHAN_AT6		21 */

    0,		    /* ADC_CHAN_AV7		22 */
    0x0080,		/* ADC_CHAN_AC7		23 */
    0x0080,		/* ADC_CHAN_AT7		24 */

    0,		    /* ADC_CHAN_AV8		25 */
    0x0100,		/* ADC_CHAN_AC8		26 */
    0x0100,		/* ADC_CHAN_AT8		27 */

    0,		    /* ADC_CHAN_AV9		28 */
    0x0200,		/* ADC_CHAN_AC9		29 */
    0x0200,		/* ADC_CHAN_AT9		30 */

    0x0400		/* ADC_CHAN_INT_TEMP	31 */
};

/*------------------------------------------------------------------------------
 *	Analog quads configuration:
 */
#define NB_QUADS			10
#define NB_QUAD_CFG_BYTES	40
#define QUAD_CFG_ACM_START	1

extern const uint8_t g_cai_quads_config[NB_QUAD_CFG_BYTES];
extern const adc_settings_t g_cai_adc_config[ADC_NB_OF_CHANNELS];

/*------------------------------------------------------------------------------
 * CoreAI base address. Its value is initialised as part of CAI_init().
 * Note that it is OK for this driver to hold the base address of CoreAI since
 * there can only ever be one instance of CoreAI on a Fusion part.
 */
static addr_t g_cai_address = UNINITIALISED_BASE_ADDR;

/*------------------------------------------------------------------------------
 *
 */
static uint8_t g_cai_channel_type[ADC_NB_OF_CHANNELS];

/*------------------------------------------------------------------------------
 *	Local functions:
 */
static void build_adc_ranges_table( uint8_t *acm_config );
static void configure_adc( void );
static uint8_t read_acm( uint8_t acm_addr );
static void write_acm(	uint8_t acm_addr, uint8_t acm_data );
static void delay( uint16_t count );
static void load_acm_config( uint8_t *acm_config );
static void extract_config( uint8_t *acm_config );

/*------------------------------------------------------------------------------
 *	CAI_init()
 */
void CAI_init( addr_t base_addr )
{
    uint8_t acm_config[NB_QUAD_CFG_BYTES];

	/* Init base address of CoreAI that will be used in subsequent calls to this
 	 * driver. */
	g_cai_address = base_addr;
	
    /* Extract configuration information from g_cai_quads_config[]. */
    extract_config( acm_config );
    
	/* Build table of the full scale ranges for the ADC based on the ACM config. */
	build_adc_ranges_table( acm_config );
	
	/* Download ACM configuration into Fusion Analog Block macro. */
    load_acm_config( acm_config );
    
#if defined(CAI_FACTORY_CALIBRATION)
    /* Initialise the calibration coefficients table. */
    init_calibration();
#endif
    
	/* Configure the Analog to Digital converter. */
	configure_adc();
}

/***************************************************************************//**
 *
 */
void extract_config( uint8_t *acm_config )
{
    uint8_t cfg_byte_idx;
    
    /* Generate the ACM configuration based on the quads configuration. */
    for ( cfg_byte_idx = 0; cfg_byte_idx < NB_QUAD_CFG_BYTES; cfg_byte_idx++ )
    {
		uint8_t selected_cfg;
        uint8_t channel_nb;
        
        selected_cfg = g_cai_quads_config[cfg_byte_idx];
        channel_nb = acm_cfg_idx_2_chan_nb_lut[cfg_byte_idx];
        
        switch( cfg_byte_idx & CHANNEL_NB_MASK )
        {
        case CURRENT_INPUT_CHANNEL_NB:
            HAL_ASSERT( selected_cfg < AC_NB_OF_CFG_OPTIONS )
            HAL_ASSERT( channel_nb != INVALID_CHANNEL_NB )
            acm_config[cfg_byte_idx] = g_cai_curr_acm_cfg_lut[selected_cfg];
            if ( selected_cfg == AC_CURRENT_MONITOR )
            {
                /* handle special case of temperature configuration where some
                 * bits also need to be set in the AV configuration byte.*/
                acm_config[cfg_byte_idx - 1] |= 0x10;
                g_cai_channel_type[channel_nb] = CURRENT_CHAN_TYPE;
            }
            else if ( selected_cfg == AC_DISABLED )
            {
                g_cai_channel_type[channel_nb] = UNUSED_CHAN_TYPE;
            }
            else
            {   /* Input channel configured as voltage monitor. */
                g_cai_channel_type[channel_nb] = VOLTAGE_CHAN_TYPE;
                if ( (selected_cfg >= AC_MINUS_16V) && (selected_cfg <= AC_MINUS_125MV) )
                {
                    g_cai_channel_type[channel_nb] |= NEGATIVE_CHAN_TYPE;
                }
            }
            break;
            
        case GATE_DRIVER_CHANNEL_NB:
            HAL_ASSERT( selected_cfg < AG_NB_OF_CFG_OPTIONS)
            acm_config[cfg_byte_idx] = g_cai_gate_drv_acm_cfg_lut[selected_cfg];
            break;
            
        case TEMPERATURE_CHANNEL_NB:
            HAL_ASSERT( selected_cfg < AT_NB_OF_CFG_OPTIONS )
            HAL_ASSERT( channel_nb != INVALID_CHANNEL_NB )
            acm_config[cfg_byte_idx] = g_cai_temp_acm_cfg_lut[selected_cfg];
            if ( selected_cfg == AT_TEMPERATURE_MONITOR )
            {
                /* handle special case of temperature configuration where some
                 * bits also need to be set in the AG configuration byte.*/
                acm_config[cfg_byte_idx - 1] |= 0x01;
                g_cai_channel_type[channel_nb] = TEMPERATURE_CHAN_TYPE;
            }
            else if ( selected_cfg == AT_DISABLED )
            {
                g_cai_channel_type[channel_nb] = UNUSED_CHAN_TYPE;
            }
            else
            {
                g_cai_channel_type[channel_nb] = VOLTAGE_CHAN_TYPE;
            }
            break;
            
        default:
            HAL_ASSERT( selected_cfg < AV_NB_CFG_OPTIONS )
            HAL_ASSERT( channel_nb != INVALID_CHANNEL_NB )
            acm_config[cfg_byte_idx] = g_cai_volt_acm_cfg_lut[selected_cfg];
            if ( selected_cfg == AV_DISABLED )
            {
                g_cai_channel_type[channel_nb] = UNUSED_CHAN_TYPE;
            }
            else
            {
                g_cai_channel_type[channel_nb] = VOLTAGE_CHAN_TYPE;
                if ( (selected_cfg >= AV_MINUS_16V) && (selected_cfg <= AV_MINUS_125MV) )
                {
                    g_cai_channel_type[channel_nb] |= NEGATIVE_CHAN_TYPE;
                }
            }
            break;
        }
    }
  
    /**/
    g_cai_channel_type[ADC_CHAN_VCC] = VOLTAGE_CHAN_TYPE;
    g_cai_channel_type[ADC_CHAN_INT_TEMP] = TEMPERATURE_CHAN_TYPE;
}

/***************************************************************************//**
 * Derive the Analog Configuration Multiplexers configuration from the CoreAI
 * configuration specified in g_cai_quads_config[].
 */
void load_acm_config( uint8_t *acm_config )
{
    uint8_t cfg_byte_idx;
    uint8_t acm_addr = QUAD_CFG_ACM_START;
    volatile uint16_t acm_status;
    
	/* Reset the ACM. */
	HAL_set_16bit_reg( g_cai_address, ACM_CTRL_STATUS, ACMRESET_MASK );
	do {
		acm_status = HAL_get_16bit_reg( g_cai_address, ACM_CTRL_STATUS );
	} while ( (acm_status & ACMRESETBUSY_MASK) > 0 );
    
  
    /* Load ACM configuration into analog block. */    
	for ( cfg_byte_idx = 0; cfg_byte_idx < NB_QUAD_CFG_BYTES; cfg_byte_idx++, acm_addr++ )
	{
		write_acm( acm_addr, acm_config[cfg_byte_idx] );		
	}
    
}

/*------------------------------------------------------------------------------
 * Start analog to digital convertion of the channel passed as argument.
 */
void CAI_adc_start( uint8_t channel_nb )
{
	uint16_t adc_status;
    uint16_t adc_busy;
	uint16_t strobe;
	uint16_t converting;
	
	HAL_ASSERT( g_cai_address != UNINITIALISED_BASE_ADDR )
	
	/* Wait for the ADC to finish calibrating, sampling, converting. */
	do {
		adc_status = HAL_get_16bit_reg( g_cai_address, ADC_STATUS );
        adc_busy = adc_status & (CALIBRATE_MASK | SAMPLE_MASK | BUSY_MASK);
	} while ( adc_busy );
    
    /* Set the ADC configuration for the next channel to be sampled. */
	HAL_set_16bit_reg_field( g_cai_address, MODE, (uint_fast16_t)g_cai_adc_config[channel_nb].mode );
	HAL_set_16bit_reg_field( g_cai_address, TVC, (uint_fast16_t)g_cai_adc_config[channel_nb].tvc );
	HAL_set_16bit_reg_field( g_cai_address, STC, (uint_fast16_t)g_cai_adc_config[channel_nb].stc );

	/* Handle strobing if required. */
	if ( CAI_is_current( channel_nb ) )
	{
		/* Select channel.*/
		HAL_set_16bit_reg_field( g_cai_address, CHNUMBER, (uint_fast16_t)channel_nb );

		/* Make sure strobe is low for at least 5 micro-seconds. */		
        HAL_set_16bit_reg( g_cai_address, ADC_CTRL_3, 0 );
		delay( DELAY_5_US );
		
		/* set strobe high */
        strobe = g_strobes_reg_lut[channel_nb];
		HAL_set_16bit_reg( g_cai_address, ADC_CTRL_3, (uint_fast16_t)strobe );
		delay( DELAY_5_US );
		
		/* Initiate convertion. */
		HAL_set_16bit_reg_field( g_cai_address, ADCSTART, 1 );
		
		/* Wait for conversion complete. */
		converting = HAL_get_16bit_reg_field( g_cai_address, BUSY );
		while ( converting )
		{
			converting = HAL_get_16bit_reg_field( g_cai_address, BUSY );
		}

		HAL_set_16bit_reg( g_cai_address, ADC_CTRL_3, 0 );
	}
	else if ( CAI_is_temperature( channel_nb ) )
	{
		/* Select channel.*/
		HAL_set_16bit_reg_field( g_cai_address, CHNUMBER, (uint_fast16_t)channel_nb );
		
		/* Make sure strobe is low for at least 5 micro-seconds. */		
        HAL_set_16bit_reg( g_cai_address, ADC_CTRL_4, 0 );
		delay( DELAY_5_US );

		/* set strobe high. */
        strobe = g_strobes_reg_lut[channel_nb];
        HAL_set_16bit_reg( g_cai_address, ADC_CTRL_4, (uint_fast16_t)strobe );
		delay( DELAY_10_US );
		
		/* Initiate convertion. */
		HAL_set_16bit_reg_field( g_cai_address, ADCSTART, 1 );

		/* Wait for conversion complete. */
		converting = HAL_get_16bit_reg_field( g_cai_address, BUSY );
		while ( converting )
		{
			converting = HAL_get_16bit_reg_field( g_cai_address, BUSY );
		}
		
		/* set strobe low */
        HAL_set_16bit_reg( g_cai_address, ADC_CTRL_4, 0 );
	}
	else
	{
		/* Select channel.*/
		HAL_set_16bit_reg_field( g_cai_address, CHNUMBER, (uint_fast16_t)channel_nb );
		
		/* Initiate convertion. */
		HAL_set_16bit_reg_field( g_cai_address, ADCSTART, 1 );
	}
}

/*------------------------------------------------------------------------------
 * Read result of the last completed analog to digital convertion or waits
 * for the current convertion to complete before to return its result.
 */
uint16_t CAI_get_adc_result( void )
{
	uint16_t adc_status;
    uint16_t adc_busy;
	uint16_t adc_result;
	
	HAL_ASSERT( g_cai_address != UNINITIALISED_BASE_ADDR )

	/* Wait for convertion to complete. */
	do {
		adc_status = HAL_get_16bit_reg( g_cai_address, ADC_STATUS );
        adc_busy = adc_status & (CALIBRATE_MASK | SAMPLE_MASK | BUSY_MASK);
	} while ( adc_busy );

	/* Clear any strobe signal that might have been set. */
	HAL_set_16bit_reg_field( g_cai_address, TMSTB, 0 );
	HAL_set_16bit_reg_field( g_cai_address, CMSTB, 0 );
	
	/* Read convertion result. */
	adc_result = HAL_get_16bit_reg_field( g_cai_address, RESULT );
    
#if defined(CAI_FACTORY_CALIBRATION) || defined(CAI_USER_CALIBRATION)
    /* Adjust using calibration data. */
    {
        uint8_t channel_nb;
        channel_nb = HAL_get_16bit_reg_field( g_cai_address, CHNUMBER );
        adc_result = cal_adjust( adc_result, channel_nb );
    }
#endif
    
	return( adc_result );
    
}

/*------------------------------------------------------------------------------
 */
uint8_t CAI_round_robin( uint16_t *adc_samples )
{
    static uint8_t channel_nb = ADC_NB_OF_CHANNELS;
    uint16_t adc_status;
    
    /* Read ADC status. */
    adc_status = HAL_get_16bit_reg( g_cai_address, ADC_STATUS );

    /* Only do stuff if the ADC is not in the process of sampling an input. */
    if ( (adc_status & SAMPLE_MASK) == 0 )
    {
        /* ADC is not currently sampling an input. */
        uint16_t valid_data = adc_status & DATAVALID_MASK;
        
        if ( valid_data )
        {
            /* Valid ADC conversion result available. */
            channel_nb = HAL_get_16bit_reg_field( g_cai_address, CHNUMBER );
            adc_samples[channel_nb] = CAI_get_adc_result();
        }
        /* Find next channel to sample. */
        do {
            channel_nb++;
            if ( channel_nb >= ADC_NB_OF_CHANNELS )
            {
                channel_nb = ADC_CHAN_VCC;
            }
        } while( adc_samples[channel_nb] == UNUSED_CHANNEL );
        
        /* Start next channel sampling. */
        CAI_adc_start( channel_nb );
    }
    
    return channel_nb;
}

/*------------------------------------------------------------------------------
 * Sets the state of the gate drivers identified as part of the bit mask passed
 * as first argument. Second argument is the requested state.
 */
void CAI_set_gate_driver
(
	uint16_t gate_ids_mask,
	uint8_t gate_state
)
{
	uint_fast16_t gate_ctrl;
	
	HAL_ASSERT( g_cai_address != UNINITIALISED_BASE_ADDR )
	
	gate_ctrl = HAL_get_16bit_reg( g_cai_address, ADC_CTRL_5 );
	if ( gate_state == 0 )
	{
		gate_ctrl &= ~(gate_ids_mask);
	}
	else
	{
		gate_ctrl |= gate_ids_mask;
	}
	HAL_set_16bit_reg( g_cai_address, ADC_CTRL_5, gate_ctrl);
}

/*------------------------------------------------------------------------------
 * Set the Real Time Counter value.
 */
void CAI_set_rtc_count( uint8_t * p_count)
{
	uint8_t i;
	uint8_t acm_addr;

	HAL_ASSERT( g_cai_address != UNINITIALISED_BASE_ADDR )
	
	write_acm( CTRL_STAT_ACM_ADDR, RSTB_CNT | XTAL_EN );
	
	for ( i = 0, acm_addr = COUNTER0_ACM_ADDR ; i < ACM_COUNTER_BYTE_SIZE; i++, acm_addr++ )
	{
		write_acm( acm_addr, p_count[i]);
	}
	write_acm( CTRL_STAT_ACM_ADDR, CNTR_EN | XT_MODE_32KHZ | RSTB_CNT | XTAL_EN );
}

/*------------------------------------------------------------------------------
 * Read the Real Time Counter current value.
 */
void CAI_get_rtc_count( uint8_t * p_count)
{
	uint8_t i;
	uint8_t acm_addr;
	
	HAL_ASSERT( g_cai_address != UNINITIALISED_BASE_ADDR )
	
	for ( i = 0, acm_addr = COUNTER0_ACM_ADDR ; i < ACM_COUNTER_BYTE_SIZE; i++, acm_addr++ )
	{
		p_count[i] = read_acm( acm_addr );
	}
}

/*------------------------------------------------------------------------------
 * CAI_set_rtc_match:
 * Sets the real Time Counter's match register to the value contained in the 5 
 * bytes buffer pointed to by the pointer passed as parameter.
 */
void CAI_set_rtc_match( uint8_t * match )
{
	uint8_t i;
	uint8_t acm_addr;

	HAL_ASSERT( g_cai_address != UNINITIALISED_BASE_ADDR )
	
	write_acm( CTRL_STAT_ACM_ADDR, RSTB_CNT | XTAL_EN );
	
	for ( i = 0, acm_addr = RTC_MATCH0_ACM_ADDR ; i < ACM_COUNTER_BYTE_SIZE; i++, acm_addr++ )
	{
		write_acm( acm_addr, match[i]);
	}
	write_acm( CTRL_STAT_ACM_ADDR, CNTR_EN | VR_EN_MAT | XT_MODE_32KHZ | RST_CNT_OMAT | RSTB_CNT | XTAL_EN );
}

/*------------------------------------------------------------------------------
 * Returns 1 if the channel specified as argument is configured for current
 * measure.
 * Returns 0 otherwise.
 */
uint8_t CAI_is_current( uint8_t channel_nb )
{
    uint8_t ret_val = 0;
	if ( g_cai_channel_type[channel_nb] == CURRENT_CHAN_TYPE )
	{
		ret_val = 1;
	}
	return ret_val;
}

/*------------------------------------------------------------------------------
 * Returns 1 if the channel specified as argument is configured for temperature
 * measure.
 * Returns 0 otherwise.
 */
uint8_t CAI_is_temperature( uint8_t channel_nb )
{
    uint8_t ret_val = 0;
	if ( g_cai_channel_type[channel_nb] == TEMPERATURE_CHAN_TYPE )
	{
		ret_val = 1;
	}
	return ret_val;
}

/*------------------------------------------------------------------------------
 * Returns 1 if the channel specified as argument is sampling a negative signal.
 * Returns 0 otherwise.
 */
uint8_t CAI_is_negative( uint8_t channel_nb )
{
    uint8_t ret_val = 0;
    uint8_t is_negative = g_cai_channel_type[channel_nb] & NEGATIVE_CHAN_TYPE;
	if ( is_negative )
	{
		ret_val = 1;
	}
	return ret_val;
}

/*------------------------------------------------------------------------------
 * Populate the g_cai_adc_ranges[] table containing the ADC full scale values
 * for the voltage monitors based on the inputs prescaler configuration.
 */
void build_adc_ranges_table( uint8_t *acm_config )
{
	uint8_t channel_id;
    
	for ( channel_id = ADC_CHAN_AV0; channel_id < ADC_NB_OF_CHANNELS; channel_id++ )
	{
		uint8_t channel_cfg;
		channel_cfg = acm_config[chan_2_acm_cfg_idx_lut[channel_id]];
		channel_cfg &= 0x07;
		g_cai_adc_ranges[channel_id] = g_cai_input_range_lut[channel_cfg];
	}
    /* The full ADC voltage range for the internal voltage monitor is a special
     * case as it is not prescaled and directly uses the 2.56V ADC voltage
     * reference. */
	g_cai_adc_ranges[ADC_CHAN_VCC] = 2560;  
}

/*------------------------------------------------------------------------------
 * Delay function used to time the current and temperature strobes.
 * This should ideally be reimplemented using a hardware timer present in your
 * design.
 */
void delay( uint16_t count )
{
	volatile uint16_t i = 0;
	
	while ( i < count )
	{
		i++;
	}
}

/*------------------------------------------------------------------------------
 * Configure the analog to digital converter.
 */
void configure_adc( void )
{
	/*
	 * Reset the ADC
	 */
	HAL_set_16bit_reg_field( g_cai_address, ADCRESET, 1 );
	HAL_set_16bit_reg( g_cai_address, ADC_CTRL_1, 1 );

	HAL_set_16bit_reg_field( g_cai_address, MODE, ADC_MODE_CFG );
	HAL_set_16bit_reg_field( g_cai_address, TVC, ADC_TVC_CFG );
	HAL_set_16bit_reg_field( g_cai_address, STC, ADC_STC_CFG );
}

/*------------------------------------------------------------------------------
 * Reads an ACM register. The ACM register address is passed as argument.
 */
uint8_t read_acm( uint8_t acm_addr )
{
	uint16_t acm_status;
	uint16_t acm_data;
    uint16_t acm_busy;
	
	HAL_set_8bit_reg( g_cai_address, ACM_ADDR, (uint_fast8_t)acm_addr );
	HAL_set_16bit_reg( g_cai_address, ACM_CTRL_STATUS, ACMRDSTART_MASK );
	
	do {
		acm_status = HAL_get_16bit_reg( g_cai_address, ACM_CTRL_STATUS );
        acm_busy = acm_status & ACMRDBUSY_MASK;
	} while ( acm_busy );
	
	acm_data = HAL_get_16bit_reg( g_cai_address, ACM_DATA );
	
	return( (uint8_t)acm_data );
}

/*------------------------------------------------------------------------------
 * Writes an ACM register. The ACM register's address is passed as the first
 * argument. The second argument contains the value to be written into the ACM.
 */
void write_acm
(
	uint8_t acm_addr,
	uint8_t acm_data
)
{
	uint16_t acm_status;
    uint16_t acm_busy;
	
	HAL_set_8bit_reg( g_cai_address, ACM_ADDR, (uint_fast8_t)acm_addr );
	HAL_set_8bit_reg( g_cai_address, ACM_DATA, (uint_fast8_t)acm_data );

	do {
		acm_status = HAL_get_16bit_reg( g_cai_address, ACM_CTRL_STATUS );
        acm_busy = acm_status & ACMWRBUSY_MASK;
	} while ( acm_busy );
}

/*------------------------------------------------------------------------------
 * Returns the ADC full scale voltage for the ADC channel specified as argument.
 * This is only relevant to analog inputs configured as voltage monitors.
 * The returned value is in mill-volts.
 */
uint16_t CAI_input_full_range( uint8_t channel_nb )
{
	return g_cai_adc_ranges[channel_nb];
}


