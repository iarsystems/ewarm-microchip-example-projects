/*******************************************************************************
 * (c) Copyright 2008 Actel Corporation
 * 
 * CoreAI calibration related functions implementation.
 *
 * SVN $Revision: 5 $
 * SVN $Date: 2013-12-23 13:52:47 +0100 (mån, 23 dec 2013) $
 */
#include "hal.h"
#include "hal_assert.h"
#include "coreai.h"
#include "coreai_cfg.h"
#include "cai_defines.h"
#include "cai_calibration.h"

#if defined(CAI_FACTORY_CALIBRATION) || defined(CAI_USER_CALIBRATION)

/***************************************************************************//**
 * Check that calibration configuration has been selected and that on one of
 *      CAI_NO_CALIBRATION
 *      CAI_FACTORY_CALIBRATION
 *      CAI_USER_CALIBRATION
 *  has been defined in "coreai_cfg.h".
 */
#if defined(CAI_NO_CALIBRATION)
#if defined(CAI_FACTORY_CALIBRATION) || defined(CAI_USER_CALIBRATION)
#error CoreAI driver calibration configuration: only one of CAI_NO_CALIBRATION, CAI_FACTORY_CALIBRATION or CAI_USER_CALIBRATION should be defined in file "coreai_cfg.h".
#endif
#elif defined(CAI_FACTORY_CALIBRATION)
#if defined(CAI_NO_CALIBRATION) || defined(CAI_USER_CALIBRATION)
#endif
#elif defined(CAI_USER_CALIBRATION)
#if defined(CAI_FACTORY_CALIBRATION) || defined(CAI_NO_CALIBRATION)
#endif
#else
#error CoreAI calibration configuration not selected. One of CAI_NO_CALIBRATION, CAI_FACTORY_CALIBRATION or CAI_USER_CALIBRATION must be defined in file "coreai_cfg.h".
#endif


/***************************************************************************//**
 *
 */
#define NVM_SPARE_PAGES_OFFSET      0x00100000UL
#define NVM_SPARE_PAGES_BASE_ADDR   CAI_CAL_NVM_BASE_ADDR + NVM_SPARE_PAGES_OFFSET

#define NVM_CALIB_DATA_START        0x1900U

/***************************************************************************//**
 * 
 */
#define NO_DATA_SET     0
#define DATASET_1       1
#define DATASET_2       2
#define DATASET_3       3
#define DATASET_4       4

#define INVALID_OFFSET  0xFF

#define DATA_SET    0
#define OFFSET      1

/* Calibration tag found at the beginning of the factory calibration datasets. */
#define CALIBRATION_TAG     0x00004243

/* Return values used to indicate if eNVM contains valid factory calibration data.*/
#define INCORRECT_CAL_DATA  0
#define VALID_CAL_DATA      1

/***************************************************************************//**
 *
 */
/*------------------------------------------------------------------------------
 * Start offset of each factory calibration dataset.
 */
#if defined(DIE_AFS1500)
#if defined(DIE_AFS600) || defined(DIE_AFS250) || defined(DIE_AFS090) 
#error Multiple die selection in CoreAI driver. You must define only one of DIE_AFS1500, DIE_AFS600, DIE_AFS250 or DIE_AFS090 in file "coreai_cfg.h".
#endif
#elif defined(DIE_AFS600)
#if defined(DIE_AFS1500) || defined(DIE_AFS250) || defined(DIE_AFS090) 
#error Multiple die selection in CoreAI driver. You must define only one of DIE_AFS1500, DIE_AFS600, DIE_AFS250 or DIE_AFS090 in file "coreai_cfg.h".
#endif
#elif defined(DIE_AFS250)
#if defined(DIE_AFS1500) || defined(DIE_AFS600) || defined(DIE_AFS090) 
#error Multiple die selection in CoreAI driver. You must define only one of DIE_AFS1500, DIE_AFS600, DIE_AFS250 or DIE_AFS090 in file "coreai_cfg.h".
#endif
#elif defined(DIE_AFS090)
#if defined(DIE_AFS1500) || defined(DIE_AFS600) || defined(DIE_AFS250) 
#error Multiple die selection in CoreAI driver. You must define only one of DIE_AFS1500, DIE_AFS600, DIE_AFS250 or DIE_AFS090 in file "coreai_cfg.h".
#endif
#else
#error No die selected. You must define either DIE_AFS1500, DIE_AFS600, DIE_AFS250 or DIE_AFS090 in file "coreai_cfg.h" in order to select the device for which you are building the CoreAI driver.
#endif

/***************************************************************************//**
 *
 */
#if defined(CAI_FACTORY_CALIBRATION)
#if defined(DIE_AFS600) || defined(DIE_AFS1500)

static const uint16_t dataset_start[5] =
{
    0,      /* NO_DATA_SET */
    20,     /* DATASET_1 */
    820,    /* DATASET_2 */
    860,    /* DATASET_3 */
    1060    /* DATASET_4 */
};

static const uint16_t data_set_size[5] =
{
    0,      /* NO_DATA_SET */
    800,    /* DATASET_1 */
    40,     /* DATASET_2 */
    200,    /* DATASET_3 */
    560     /* DATASET_4 */
};

#elif defined(DIE_AFS250)

static const uint16_t dataset_start[5] =
{
    0,      /* NO_DATA_SET */
    20,     /* DATASET_1 */
    500,    /* DATASET_2 */
    524,    /* DATASET_3 */
    644     /* DATASET_4 */
};

static const uint16_t data_set_size[5] =
{
    0,      /* NO_DATA_SET */
    480,    /* DATASET_1 */
    24,     /* DATASET_2 */
    120,    /* DATASET_3 */
    336     /* DATASET_4 */
};

#elif defined(DIE_AFS090)

static const uint16_t dataset_start[5] =
{
    0,      /* NO_DATA_SET */
    20,     /* DATASET_1 */
    420,    /* DATASET_2 */
    440,    /* DATASET_3 */
    540     /* DATASET_4 */
};

static const uint16_t data_set_size[5] =
{
    0,      /* NO_DATA_SET */
    400,    /* DATASET_1 */
    20,     /* DATASET_2 */
    100,    /* DATASET_3 */
    280     /* DATASET_4 */
};

#endif

/***************************************************************************//**
 *
 */
/*------------------------------------------------------------------------------
 * Number of bytes assigned in each dataset for the configuration of a quad.
 */
static const uint16_t data_set_quad_size[5] =
{
    0,      /* NO_DATA_SET */
    80,     /* DATASET_1 */
    4,      /* DATASET_2 */
    20,     /* DATASET_3 */
    56      /* DATASET_4 */
};

/***************************************************************************//**
 *
 */
static const uint8_t av_cal_dataset_lut[AV_NB_CFG_OPTIONS][2] =
{
    {NO_DATA_SET, INVALID_OFFSET},  /* AV_DISABLED */
    {DATASET_3, 0},                 /* AV_VAREF */
    {DATASET_1, 0},                 /* AV_16V */
    {DATASET_1, 1},                 /* AV_8V */
    {DATASET_1, 2},                 /* AV_4V */
    {DATASET_1, 3},                 /* AV_2V */
    {DATASET_1, 4},                 /* AV_1V */
    {DATASET_4, 0},                 /* AV_500MV */
    {DATASET_4, 1},                 /* AV_250MV */
    {DATASET_4, 2},                 /* AV_125MV */
    {DATASET_1, 5},                 /* AV_MINUS_16V */
    {DATASET_1, 6},                 /* AV_MINUS_8V */
    {DATASET_1, 7},                 /* AV_MINUS_4V */
    {DATASET_1, 8},                 /* AV_MINUS_2V */
    {DATASET_4, 3},                 /* AV_MINUS_1V */
    {DATASET_4, 4},                 /* AV_MINUS_500MV */
    {DATASET_4, 5},                 /* AV_MINUS_250MV */
    {DATASET_4, 6}                  /* AV_MINUS_125MV */
};

/***************************************************************************//**
 *
 */
static const uint8_t ac_cal_dataset_lut[AC_NB_OF_CFG_OPTIONS][2] =
{
    {NO_DATA_SET, INVALID_OFFSET},  /* AC_DISABLED */
    {DATASET_3, 1},                 /* AC_VAREF */
    {DATASET_1, 9},                 /* AC_16V */
    {DATASET_1, 10},                /* AC_8V */
    {DATASET_1, 11},                /* AC_4V */
    {DATASET_1, 12},                /* AC_2V */
    {DATASET_1, 13},                /* AC_1V */
    {DATASET_4, 7},                 /* AC_500MV */
    {DATASET_4, 8},                 /* AC_250MV */
    {DATASET_4, 9},                 /* AC_125MV */
    {DATASET_1, 14},                /* AC_MINUS_16V */
    {DATASET_1, 15},                /* AC_MINUS_8V */
    {DATASET_1, 16},                /* AC_MINUS_4V */
    {DATASET_1, 17},                /* AC_MINUS_2V */
    {DATASET_4, 10},                /* AC_MINUS_1V */
    {DATASET_4, 11},                /* AC_MINUS_500MV */
    {DATASET_4, 12},                /* AC_MINUS_250MV */
    {DATASET_4, 13},                /* AC_MINUS_125MV */
    {DATASET_3, 2},                 /* AC_CURRENT_MONITOR */
};

/***************************************************************************//**
 *
 */
static const uint8_t at_cal_dataset_lut[AT_NB_OF_CFG_OPTIONS][2] =
{
    {NO_DATA_SET, INVALID_OFFSET},  /* AT_DISABLED */
    {DATASET_3, 4},                 /* AT_VAREF */
    {DATASET_1, 18},                /* AT_16V */
    {DATASET_1, 19},                /* AT_4V */
    {DATASET_2,  0}                 /* AT_TEMPERATURE_MONITOR */
};


#endif  /* defined(CAI_FACTORY_CALIBRATION) */

/***************************************************************************//**
 *
 */
extern const uint8_t acm_cfg_idx_2_chan_nb_lut[NB_QUAD_CFG_BYTES];
extern const uint8_t g_cai_quads_config[NB_QUAD_CFG_BYTES];
extern const adc_settings_t g_cai_adc_config[ADC_NB_OF_CHANNELS];

/***************************************************************************//**
 * Factory calibration m and c coefficients. These arrays are populated from the
 * factory calibration data stored in eNVM during the call to CAI_init().
 * These arrays are only used if factory calibration is enabled.
 * These arrays constant and statically populated in "coreai_cfg.c" if user
 * calibration is enabled.
 */
#if defined(CAI_FACTORY_CALIBRATION)

static uint16_t g_cai_m_coef[ADC_NB_OF_CHANNELS];
static uint16_t g_cai_c_coef[ADC_NB_OF_CHANNELS];

#elif defined(CAI_USER_CALIBRATION)

extern uint16_t g_cai_m_coef[ADC_NB_OF_CHANNELS];
extern uint16_t g_cai_c_coef[ADC_NB_OF_CHANNELS];

#endif


uint8_t *g_factory_calibration = 0;

/***************************************************************************//**
 * Local functions
 */
static uint8_t valid_fact_cal_data( void );
static void load_calibration( void );

uint16_t
spare_pages_crc
(
    uint16_t start_offset,
    uint16_t data_length
);

/***************************************************************************//**
 * Local functions
 */
#if defined(CAI_FACTORY_CALIBRATION)
void init_calibration( void )
{
    if ( VALID_CAL_DATA == valid_fact_cal_data() )
    {
        load_calibration();
    }
    else
    {
        uint8_t channel_nb;
        for ( channel_nb = 0; channel_nb < ADC_NB_OF_CHANNELS; channel_nb++ )
        {
            /* Use neutral coefficients. */
            g_cai_m_coef[channel_nb] = 1024;
            g_cai_c_coef[channel_nb] = 0;
        }
    }
}
#endif

/***************************************************************************//**
 * get_cal_data_offset() calculates the offset where an input's calibration data
 * can be found within the factory calibration table.
 * calibration data for the input configuration specified in g_cai_quads_config[].
 * The value written in cal_table_offset indicates the byte offset within the
 * factory calibration table.
 *
 * Returns 1 if the value of dataset and dataset_offset is set.
 * Returns 0 if no calibration dataset info could be found.
 */
#if defined(CAI_FACTORY_CALIBRATION)
uint8_t get_cal_data_offset
(
    uint8_t cfg_byte_idx,
    uint16_t *cal_table_offset
)
{
    uint8_t input_config;
    uint8_t ret_val = 1;
    uint8_t quad_nb;
    uint8_t dataset;
    uint8_t ds_quad_offset; /* Offset within a quad calibration dataset. */
    
    /* The calibration data location is based on the input's configuration. */
    input_config = g_cai_quads_config[cfg_byte_idx];
    
    /* Find out which dataset contains the input's calibration data and the
     * offset for an input's configuration within a quad. */
    switch( cfg_byte_idx & CHANNEL_NB_MASK )
    {
    case VOLTAGE_CHANNEL_NB:
        dataset = av_cal_dataset_lut[input_config][DATA_SET];
        ds_quad_offset = av_cal_dataset_lut[input_config][OFFSET];
        break;
        
    case CURRENT_INPUT_CHANNEL_NB:
        dataset = ac_cal_dataset_lut[input_config][DATA_SET];
        ds_quad_offset = ac_cal_dataset_lut[input_config][OFFSET];
        break;
        
    case TEMPERATURE_CHANNEL_NB:
        dataset = at_cal_dataset_lut[input_config][DATA_SET];
        ds_quad_offset = at_cal_dataset_lut[input_config][OFFSET];
        break;
      
    default:
        /* must be configuration for a gate driver output. */
        ret_val = 0;
        break;
    }

    /* calculate the calibration data address. */
    if ( ret_val == 1 )
    {
        quad_nb = cfg_byte_idx / 4;
        *cal_table_offset = NVM_CALIB_DATA_START 
                            + dataset_start[dataset] 
                            + (quad_nb * data_set_quad_size[dataset]) 
                            + (ds_quad_offset * 4);
    }
      
    return ret_val;
}
#endif  /* defined(CAI_FACTORY_CALIBRATION) */

#define ADC_BIT_SIZE_MASK   0x03U
#define TWO_POW_8       256U
#define TWO_POW_8_MASK  0xFFU
#define TWO_POW_16      65536UL

/***************************************************************************//**
 * Retrieve the factory calibration data from the eNVM spare pages and populate
 * the m and c coefficients arrays g_cai_m_coef[] and g_cai_c_coef[] used for
 * calibration.
 */
#if defined(CAI_FACTORY_CALIBRATION)
void load_calibration( void )
{
    uint8_t cfg_byte_idx;
    uint8_t channel_nb;
    uint32_t cal_data_offset;
    
    for ( cfg_byte_idx = 0; cfg_byte_idx < NB_QUAD_CFG_BYTES; cfg_byte_idx++ )
    {
        uint16_t cal_table_offset;
        
        channel_nb = acm_cfg_idx_2_chan_nb_lut[cfg_byte_idx];
        
        if ( channel_nb < ADC_NB_OF_CHANNELS )
        {
            if ( get_cal_data_offset( cfg_byte_idx, &cal_table_offset ) )
            {
                uint16_t m1, m2, c;
                uint32_t cal_data_addr;
                uint8_t adc_cfg;
                
                /* Calculate the address of the calibration data in the eNVM spare pages.
                * This is done to account for address bits [11:7] being dropped when
                * accessing eNVM spare pages. */
                cal_data_addr = ((cal_table_offset << 5U) & 0x0003F000UL) | (cal_table_offset & 0x7FU);
                cal_data_offset = NVM_SPARE_PAGES_BASE_ADDR + cal_data_addr;
                
                /* Read coefficients from factory calibration table. */
                m2 = g_factory_calibration[cal_data_offset];
                m1 = g_factory_calibration[cal_data_offset + 1];
                c = g_factory_calibration[cal_data_offset + 2];
    
                /* Translate the m1 and m2 coefficient bits into a 16 bit value.*/
                g_cai_m_coef[channel_nb] = ((m1 << 3U) & 0x7F8U) | ((m2 >> 5U) & 0x07U);
                
                /* Adjust the value of c based on the ADC configuration so that 
                 * calibration adjustment can be done using a simple add operation.
                 * The c coefficient is also changed from an 8 bit 2's complement
                 * to a 16 bit 2's complement format in order to use unsigned uint16_t
                 * calculation in calibration adjustment. */
                adc_cfg =  g_cai_adc_config[channel_nb].mode & ADC_BIT_SIZE_MASK;
                /* Extend c from 8 to 16 bit 2's complement. */
                if ( (c & 0x80) != 0 )
                {
                    c = 0xFF00 | c;
                }
                switch( adc_cfg )
                {
                case ADC_8_BIT:
                    g_cai_c_coef[channel_nb] = c;
                    break;
                    
                case ADC_10_BIT:
                    g_cai_c_coef[channel_nb] = c << 2;
                    break;
                    
                case ADC_12_BIT:
                    g_cai_c_coef[channel_nb] = c << 4;
                    break;
                    
                default:
                    HAL_ASSERT( 0 )
                    break;
                }
            }
            else
            {
                /* Use neutral coefficients. */
                g_cai_m_coef[channel_nb] = 1024;
                g_cai_c_coef[channel_nb] = 0;
            }
        }
    }
    /* Populate the calibration table for the internal voltage and temperature monitors. */
    g_cai_m_coef[ADC_CHAN_VCC] = 1024;
    g_cai_c_coef[ADC_CHAN_VCC] = 0;
    g_cai_m_coef[ADC_CHAN_INT_TEMP] = 1024;
    g_cai_c_coef[ADC_CHAN_INT_TEMP] = 0;
}
#endif  /* defined(CAI_FACTORY_CALIBRATION) */

/***************************************************************************//**
 * Check if eNVM contains valid factory calibration data.
 * This function returns:
 * VALID_CAL_DATA       if valid factory calibration data is found in eNVM.
 * INCORRECT_CAL_DATA   if the factory calibration data tag was not found or a
 *                      checksum error was detected within a calibratiton dataset.
 */
uint8_t valid_fact_cal_data(void)
{
    uint32_t *mem_space = 0;
    uint8_t ret_val = VALID_CAL_DATA;
    uint32_t tag_offset = (NVM_SPARE_PAGES_BASE_ADDR + ((NVM_CALIB_DATA_START << 5) & 0x0003F000UL) | (NVM_CALIB_DATA_START & 0x7F)) / 4;
    uint8_t dataset = 0;
    uint16_t crc;
    
    do {
        dataset++;
        crc = spare_pages_crc( (uint16_t)(NVM_CALIB_DATA_START + dataset_start[dataset]), data_set_size[dataset] );
        if ( crc != mem_space[tag_offset + dataset] )
        {
            ret_val = INCORRECT_CAL_DATA;
        }
    } while ( (dataset < 4) && ( VALID_CAL_DATA == ret_val ) );
    
    return ret_val;
}

/***************************************************************************//**
 *
 */
#define NVM_ADDR(X) ((((X) << 5U) & 0x0003F000UL) | ((X) & 0x7FU))

uint16_t
spare_pages_crc
(
    uint16_t start_offset,
    uint16_t data_length
)
{
    uint16_t i, j;
    uint32_t crc = 0;
    uint32_t byte_offset = start_offset;
    uint8_t *p_nvm = 0;
    uint32_t nvm_offset;
    
    for( i = 0; i < data_length; i++, byte_offset++ )
    {
		for ( j = 0; j < 8; j++ )
		{
			uint8_t bit;
            uint8_t data;
            nvm_offset = NVM_SPARE_PAGES_BASE_ADDR + NVM_ADDR(byte_offset);
            data = p_nvm[nvm_offset];
            
			crc = crc << 1;

			bit = (data >> j) & 0x01U;

			if ( bit == 1 )
			{
				crc = crc + 1;
			}
			if ( crc >= 0x10000UL )
			{
				crc = crc ^ 0x11021UL;
			}
		}
    }
    
    return ((uint16_t)crc);
}

/***************************************************************************//**
 *
 */
uint16_t cal_adjust
(
    uint16_t adc_value,
    uint8_t channel_nb
)
{
    uint32_t cal32;
    uint16_t calibrated;
    
    /* Apply the m coefficient to the ADC value. */
    cal32 = adc_value;    
    cal32 = (cal32 * g_cai_m_coef[channel_nb]);
    /* Bring back calculations to 16 bits. */
    calibrated = cal32 >> 8;
    /* Apply c coefficient. */
    calibrated = calibrated + g_cai_c_coef[channel_nb];
    /* Bring back to a 12 bit value. */
    calibrated = calibrated >> 2;
    return( calibrated );
}

#endif  /* defined(CAI_FACTORY_CALIBRATION) || defined(CAI_USER_CALIBRATION) */
