/*******************************************************************************
 * (c) Copyright 2006 Actel Corporation
 * 
 * CoreAI driver public API.
 *
 * SVN $Revision: 5 $
 * SVN $Date: 2013-12-23 13:52:47 +0100 (mån, 23 dec 2013) $
 */
#ifndef __COREAI_DRIVER_H
#define __COREAI_DRIVER_H	1

#include "cpu_types.h"

/***************************************************************************//**
 * ADC channels ID:
 * The following defines are used to identify the analog inputs. They are 
 * typically used as function arguments such as CAI_adc_start(), CAI_is_current(),
 * CAI_is_temperature() and CAI_is_negative().
 */
#define ADC_CHAN_VCC		0

#define ADC_CHAN_AV0		1
#define ADC_CHAN_AC0		2
#define ADC_CHAN_AT0		3

#define ADC_CHAN_AV1		4
#define ADC_CHAN_AC1		5
#define ADC_CHAN_AT1		6

#define ADC_CHAN_AV2		7
#define ADC_CHAN_AC2		8
#define ADC_CHAN_AT2		9

#define ADC_CHAN_AV3		10
#define ADC_CHAN_AC3		11
#define ADC_CHAN_AT3		12

#define ADC_CHAN_AV4		13
#define ADC_CHAN_AC4		14
#define ADC_CHAN_AT4		15

#define ADC_CHAN_AV5		16
#define ADC_CHAN_AC5		17
#define ADC_CHAN_AT5		18

#define ADC_CHAN_AV6		19
#define ADC_CHAN_AC6		20
#define ADC_CHAN_AT6		21

#define ADC_CHAN_AV7		22
#define ADC_CHAN_AC7		23
#define ADC_CHAN_AT7		24

#define ADC_CHAN_AV8		25
#define ADC_CHAN_AC8		26
#define ADC_CHAN_AT8		27

#define ADC_CHAN_AV9		28
#define ADC_CHAN_AC9		29
#define ADC_CHAN_AT9		30

#define ADC_CHAN_INT_TEMP	31

/* Number of analog sources that can be sampled. */
#define ADC_NB_OF_CHANNELS	32

#define INVALID_CHANNEL_NB      ADC_NB_OF_CHANNELS

/***************************************************************************//**
 * Gate drivers ID:
 */
#define GATE_DRIVER_0	0x001
#define GATE_DRIVER_1	0x002
#define GATE_DRIVER_2	0x004
#define GATE_DRIVER_3	0x008
#define GATE_DRIVER_4	0x010
#define GATE_DRIVER_5	0x020
#define GATE_DRIVER_6	0x040
#define GATE_DRIVER_7	0x080
#define GATE_DRIVER_8	0x100
#define GATE_DRIVER_9	0x200

/***************************************************************************//**
 * Gate drivers state:
 */
#define GATE_DRIVER_OFF	0
#define GATE_DRIVER_ON	1

/***************************************************************************//**
 * Number of bytes contained in the Quad configuration table g_cai_quads_config[].
 */
#define NB_QUAD_CFG_BYTES   40

/***************************************************************************//**
 * Defines used with round robin function to specify which channels should be
 * sampleds
 */
#define UNUSED_CHANNEL  0xFFFFU
#define SAMPLED_CHANNEL 0xF000U

/***************************************************************************//**
 * Voltage inputs possible configuration.
 */
typedef enum AV_input_cfg
{
    AV_DISABLED = 0,
    AV_VAREF,
    AV_16V,
    AV_8V,
    AV_4V,
    AV_2V,
    AV_1V,
    AV_500MV,
    AV_250MV,
    AV_125MV,
    AV_MINUS_16V,
    AV_MINUS_8V,
    AV_MINUS_4V,
    AV_MINUS_2V,
    AV_MINUS_1V,
    AV_MINUS_500MV,
    AV_MINUS_250MV,
    AV_MINUS_125MV,
    AV_DIGITAL_INPUT,
    AV_NB_CFG_OPTIONS
} AV_input_cfg_t;

/***************************************************************************//**
 * Current inputs possible configuration.
 */
typedef enum AC_input_cfg
{
    AC_DISABLED = 0,
    AC_VAREF,
    AC_16V,
    AC_8V,
    AC_4V,
    AC_2V,
    AC_1V,
    AC_500MV,
    AC_250MV,
    AC_125MV,
    AC_MINUS_16V,
    AC_MINUS_8V,
    AC_MINUS_4V,
    AC_MINUS_2V,
    AC_MINUS_1V,
    AC_MINUS_500MV,
    AC_MINUS_250MV,
    AC_MINUS_125MV,
    AC_CURRENT_MONITOR,
    AC_DIGITAL_INPUT,
    AC_NB_OF_CFG_OPTIONS
} AC_input_cfg_t;

/***************************************************************************//**
 * Temperature inputs possible configuration.
 */
typedef enum AT_input_cfg
{
    AT_DISABLED = 0,
    AT_VAREF,
    AT_16V,
    AT_4V,
    AT_TEMPERATURE_MONITOR,
    AT_DIGITAL_INPUT,
    AT_NB_OF_CFG_OPTIONS
} AT_input_cfg_t;

/***************************************************************************//**
 * Gate drivers possible configuration.
 */
typedef enum AG_output_cfg
{
    AG_DISABLED = 0,
    AG_1UA_SOURCE,
    AG_3UA_SOURCE,
    AG_10UA_SOURCE,
    AG_30UA_SOURCE,
    AG_25MA_SOURCE,
    AG_1UA_SINK,
    AG_3UA_SINK,
    AG_10UA_SINK,
    AG_30UA_SINK,
    AG_25MA_SINK,
    AG_NB_OF_CFG_OPTIONS
} AG_output_cfg_t;

/***************************************************************************//**
 * ADC mode configuration defines.
 */
#define ADC_10_BIT          0x00
#define ADC_12_BIT          0x01
#define ADC_8_BIT           0x02
#define ADC_NO_POWER_DOWN   0x04
#define ADC_NO_CALIBRATION  0x08

/***************************************************************************//**
 * ADC setting.
 */
typedef struct adc_settings
{
    uint8_t mode;
    uint8_t tvc;        /* ADC clock divider */
    uint8_t stc;        /* Sample Time Control */
} adc_settings_t;

/***************************************************************************//**
 * CAI_init initialises the CoreAI driver.
 *
 * @param base_addr     Base address of the CoreAI instance to initialise.
 */
void CAI_init( addr_t base_addr );

/***************************************************************************//**
 * CAI_adc_start starts an analog to digital convertion of ADC channel specified
 * as argument.
 *
 * @param channel_nb    Channel number of the analog input to be sampled.
 */
void CAI_adc_start( uint8_t channel_nb );

/***************************************************************************//**
 * CAI_get_adc_result returns the result of the ADC conversion previously
 * initiated by a call to CAI_adc_start(). This function only returns once the
 * conversion has completed.
 */
uint16_t CAI_get_adc_result( void );

/***************************************************************************//**
 * CAI_round_robin() is used for sampling analog inputs. It updates the content
 * of a 32 sample table passed as parameter.
It is a
 * non-blocking function managing the sampling of the enabled analog inputs and
 * upd
 */
uint8_t CAI_round_robin( uint16_t *adc_samples );

/***************************************************************************//**
 * CAI_is_temperature is used to find out if the channel identified by the
 * parameter is configured as a temperature monitor.
 *
 * @param channel_nb        Channel number identifier of the input we are
 *                          inquiring about.
 *
 * @return                  Returns 1 if the channel specified as argument is
 *                          configured for temperature measure.
 *                          Returns 0 otherwise.
 */
uint8_t CAI_is_temperature( uint8_t channel_nb );

/***************************************************************************//**
 * CAI_is_current is used to find out if the channel identified by the parameter
 * is configured as a current monitor.
 *
 * @param channel_nb        Channel number identifier of the input we are
 *                          inquiring about.
 *
 * @return                  Returns 1 if the channel specified as argument is
 *                          configured for current measure. Returns 0 otherwise.
 */
uint8_t CAI_is_current( uint8_t channel_nb );

/***************************************************************************//**
 * CAI_is_negative is used to find out if the channel identified as parameter
 * is configured to monitor a negative voltage.
 *
 * @param channel_nb        Channel number identifier of the input we are
 *                          inquiring about.
 *
 * @return                  Returns 1 if the channel specified as argument is
 *                          sampling a negative signal. Returns 0 otherwise.
 */
uint8_t CAI_is_negative( uint8_t channel_nb );

/***************************************************************************//**
 * CAI_set_gate_driver sets the output gate drivers identified by the first
 * argument to the state specified by the second argument.
 *
 * @param gate_ids_mask     This parameter is used to identify one or more
 *                          gate driver output which state will be changed by 
 *                          this function. It is a bit mask. Multiple gate
 *                          drivers can be controlled by or'ing several gate
 *                          drivers identifiers. For example, gate drivers 3 and
 *                          6 are specified as follows: GATE_DRIVER_3 | GATE_DRIVER_6
 * @param gate_state        Indicates whether the gate drivers identified with
 *                          the first parameter should turned on or off. The
 *                          defines GATE_DRIVER_OFF or GATE_DRIVER_ON can be
 *                          used as value for thsi parameter.
 */
void CAI_set_gate_driver
(
	uint16_t gate_ids_mask,
	uint8_t gate_state
);

/***************************************************************************//**
 * CAI_set_rtc_count sets the Real Time Counter value.
 *
 * @param p_count           A pointer to a 5 bytes array containing the value of
 *                          the Real Time Counter.
 */
void CAI_set_rtc_count( uint8_t * p_count );

/***************************************************************************//**
 * CAI_get_rtc_count reads the current value of the Real Time Counter. 
 * The RTC value is written to a 5 bytes buffer located at the address specified
 * by the pointer passed as argument. It is the caller's responsability to
 * allocate memory for the buffer where the RTC count will be written.
 *
 * @param p_count           A pointer to the 5 bytes array where the Real Time
 *                          Counter value will be stored.
 */
void CAI_get_rtc_count( uint8_t * p_count );

/***************************************************************************//**
 * CAI_set_rtc_match sets the real Time Counter's match register to the value
 * contained in the 5 bytes buffer pointed to by the pointer passed as parameter.
 *
 * @param match             Pointer to the 5 bytes array where where the Real
 *                          Time Counter match value is stored.
 */
void CAI_set_rtc_match( uint8_t * match );

/***************************************************************************//**
 * CAI_input_full_range returns the ADC full scale voltage for the ADC channel
 * specified as argument.
 * This is only relevant to analog inputs configured as voltage monitors.
 * The returned value is in milli-volts.
 *
 * @param channel_nb        Channel number identifier of the input which we are
 *                          inquiring about.
 */
uint16_t CAI_input_full_range( uint8_t channel_nb );

#endif	/* __COREAI_DRIVER_H */
