/*******************************************************************************
 * (c) Copyright 2008 Actel Corporation
 * 
 *
 * SVN $Revision: 5 $
 * SVN $Date: 2013-12-23 13:52:47 +0100 (mån, 23 dec 2013) $
 */
#include "hal.h"
#include "platform.h"
#include "coreai.h"
#include "core_uart_apb.h"

/***************************************************************************//**
 *
 */
#define MAX_TEXT_BUFFER_SIZE    64
#define CHAN_NAME_MAX_SIZE      16

#define ADC_RESOLUTION  0xFFF       /*  ADC configured for 12-bit mode. */

/******************************************************************************
 * Baud value to achieve a 57600 baud rate with a 12.5MHz system clock.
 * This value is calculated using the following equation:
 *      BAUD_VALUE = (CLOCK / (16 * BAUD_RATE)) - 1 
 *****************************************************************************/ 
#define BAUD_VALUE_57600    13

const uint8_t channel_name_lut[ADC_NB_OF_CHANNELS][CHAN_NAME_MAX_SIZE] =
{
  {"Internal Vcc:  "},
  {"    AV0     :  "},
  {"    AC0     :  "},
  {"    AT0     :  "},
  {"    AV1     :  "},
  {"    AC1     :  "},
  {"    AT1     :  "},
  {"    AV2     :  "},
  {"    AC2     :  "},
  {"    AT2     :  "},
  {"    AV3     :  "},
  {"    AC3     :  "},
  {"    AT3     :  "},
  {"    AV4     :  "},
  {"    AC4     :  "},
  {"    AT4     :  "},
  {"    AV5     :  "},
  {"    AC5     :  "},
  {"    AT5     :  "},
  {"    AV6     :  "},
  {"    AC6     :  "},
  {"    AT6     :  "},
  {"    AV7     :  "},
  {"    AC7     :  "},
  {"    AT7     :  "},
  {"    AV8     :  "},
  {"    AC8     :  "},
  {"    AT8     :  "},
  {"    AV9     :  "},
  {"    AC9     :  "},
  {"    AT9     :  "},
  {"Intern. Temp:  "}
};

const uint8_t line_return[2] = "\n\r";
const uint8_t sep[] = "==========================================================================";

/***************************************************************************//**
 * Local functions.
 */
uint8_t key_pressed( void );

void process_samples( uint16_t *adc_samples );

void display_current
(
    uint8_t channel_nb,
    uint16_t raw_value
);

void display_temperature
(
    uint8_t channel_nb,
    uint16_t raw_value
);

void display_voltage
(
    uint8_t channel_nb,
    uint16_t raw_value
);

void display_start( void );
void display_update_sep( void );
void display_input_sep( void );

/***************************************************************************//**
 *
 */
UART_instance_t g_the_uart;
int g_signal_count = 0;

/***************************************************************************//**
 *
 */
int main( void )
{
    static uint16_t adc_samples[ADC_NB_OF_CHANNELS] = 
    {
        SAMPLED_CHANNEL,    /* ADC_CHAN_VCC */
        SAMPLED_CHANNEL,    /* ADC_CHAN_AV0 */
        SAMPLED_CHANNEL,    /* ADC_CHAN_AC0 */
        SAMPLED_CHANNEL,    /* ADC_CHAN_AT0 */
        SAMPLED_CHANNEL,    /* ADC_CHAN_AV1 */
        SAMPLED_CHANNEL,    /* ADC_CHAN_AC1 */
        SAMPLED_CHANNEL,    /* ADC_CHAN_AT1 */
        SAMPLED_CHANNEL,    /* ADC_CHAN_AV2 */
        SAMPLED_CHANNEL,    /* ADC_CHAN_AC2 */
        UNUSED_CHANNEL,     /* ADC_CHAN_AT2 */
        SAMPLED_CHANNEL,    /* ADC_CHAN_AV3 */
        SAMPLED_CHANNEL,    /* ADC_CHAN_AC3 */
        UNUSED_CHANNEL,     /* ADC_CHAN_AT3 */
        SAMPLED_CHANNEL,    /* ADC_CHAN_AV4 */
        SAMPLED_CHANNEL,    /* ADC_CHAN_AC4 */
        SAMPLED_CHANNEL,    /* ADC_CHAN_AT4 */
        SAMPLED_CHANNEL,    /* ADC_CHAN_AV5 */
        SAMPLED_CHANNEL,    /* ADC_CHAN_AC5 */
        SAMPLED_CHANNEL,    /* ADC_CHAN_AT5 */
        SAMPLED_CHANNEL,    /* ADC_CHAN_AV6 */
        SAMPLED_CHANNEL,    /* ADC_CHAN_AC6 */
        SAMPLED_CHANNEL,    /* ADC_CHAN_AT6 */
        SAMPLED_CHANNEL,    /* ADC_CHAN_AV7 */
        SAMPLED_CHANNEL,    /* ADC_CHAN_AC7 */
        SAMPLED_CHANNEL,    /* ADC_CHAN_AT7 */
        SAMPLED_CHANNEL,    /* ADC_CHAN_AV8 */
        SAMPLED_CHANNEL,    /* ADC_CHAN_AC8 */
        SAMPLED_CHANNEL,    /* ADC_CHAN_AT8 */
        SAMPLED_CHANNEL,    /* ADC_CHAN_AV9 */
        SAMPLED_CHANNEL,    /* ADC_CHAN_AC9 */
        SAMPLED_CHANNEL,    /* ADC_CHAN_AT9 */
        SAMPLED_CHANNEL,    /* ADC_CHAN_INT_TEMP */
    };
    
    CAI_init( COREAI_BASE_ADDR );
    UART_init( &g_the_uart, UART_BASE_ADDR, BAUD_VALUE_57600, (DATA_8_BITS | NO_PARITY) );

    display_start();
    
    while( 1 )
    {
        CAI_round_robin( adc_samples );
        
        if ( key_pressed() )
        {
            process_samples( adc_samples );
        }
    }
}

/***************************************************************************//**
 *
 */
uint8_t key_pressed( void )
{
    uint8_t ret_val = 0;
    size_t rx_size;
    uint8_t rx_buffer[16];

    rx_size = UART_get_rx( &g_the_uart, rx_buffer, sizeof(rx_buffer) );
    if ( rx_size > 0 )
    {
        ret_val = 1;
    }
    
    return ret_val;
}

/***************************************************************************//**
 *
 */
void process_samples( uint16_t *adc_samples )
{
    uint8_t channel_nb;
    
    display_update_sep();
    
    for ( channel_nb = ADC_CHAN_VCC; channel_nb < ADC_NB_OF_CHANNELS; channel_nb++ )
    {
        if ( (adc_samples[channel_nb] != UNUSED_CHANNEL) && (adc_samples[channel_nb] != SAMPLED_CHANNEL) )
        {
            uint16_t raw_value;
            
            raw_value = adc_samples[channel_nb];
        
            if ( CAI_is_current( channel_nb ) )
            {
                display_current( channel_nb, raw_value );
            }
            else if ( CAI_is_temperature( channel_nb ) )
            {
                display_temperature( channel_nb, raw_value );
            }
            else
            {
                display_voltage( channel_nb, raw_value );
            }
            
            display_input_sep();
        }
    }
}

/***************************************************************************//**
 *
 */
void display_current
(
    uint8_t channel_nb,
    uint16_t raw_value
)
{
    uint8_t value_text_reverse[MAX_TEXT_BUFFER_SIZE];
    uint8_t idx = 0;
    uint16_t voltage;
    
    /* Translate from raw ADC value to mV. */
    voltage = (2560 * raw_value) / ADC_RESOLUTION;
    
    /* display channel name.*/
    UART_send( &g_the_uart, &channel_name_lut[channel_nb][0], CHAN_NAME_MAX_SIZE );
    
    /* generate text. */
    idx = MAX_TEXT_BUFFER_SIZE - 1;
    value_text_reverse[idx--] = 0x00;
    value_text_reverse[idx--] = 'A';
    value_text_reverse[idx--] = 'm';
    value_text_reverse[idx] = ' ';
    do {
        idx--;
        value_text_reverse[idx] = (voltage % 10) + '0';
        voltage = voltage / 10;
    } while( ( voltage > 0 ) && (idx > 0) );
    
    /* pad with spaces for alignment. */
    while( idx > (MAX_TEXT_BUFFER_SIZE - 8) )
    {
        idx--;
        value_text_reverse[idx] = ' ';
    }
    
    
    UART_send( &g_the_uart, &value_text_reverse[idx], MAX_TEXT_BUFFER_SIZE - idx );
}

/***************************************************************************//**
 *
 */
void display_temperature
(
    uint8_t channel_nb,
    uint16_t raw_value
)
{
    uint8_t value_text_reverse[MAX_TEXT_BUFFER_SIZE];
    uint8_t idx = 0;
    uint32_t voltage;
    
    /* Translate from raw ADC value to mV. */
    voltage = (raw_value / 4 ) - 273;
    
    /* display channel name.*/
    UART_send( &g_the_uart, &channel_name_lut[channel_nb][0], CHAN_NAME_MAX_SIZE );
    
    /* generate text. */
    idx = MAX_TEXT_BUFFER_SIZE - 1;
    value_text_reverse[idx--] = 0x00;
    value_text_reverse[idx--] = 'C';
    value_text_reverse[idx] = ' ';
    do {
        idx--;
        value_text_reverse[idx] = (voltage % 10) + '0';
        voltage = voltage / 10;
    } while( ( voltage > 0 ) && (idx > 0) );
    
    /* pad with spaces for alignment. */
    while( idx > (MAX_TEXT_BUFFER_SIZE - 8) )
    {
        idx--;
        value_text_reverse[idx] = ' ';
    }
    
    
    UART_send( &g_the_uart, &value_text_reverse[idx], MAX_TEXT_BUFFER_SIZE - idx );
}

/***************************************************************************//**
 *
 */
void display_voltage
(
    uint8_t channel_nb,
    uint16_t raw_value
)
{
    uint8_t value_text_reverse[MAX_TEXT_BUFFER_SIZE];
    uint8_t unit[] = " mV";
    uint8_t idx = 0;
    uint32_t full_scale;
    uint16_t voltage;
    
    /* Translate from raw ADC value to mV. */
    full_scale = CAI_input_full_range( channel_nb );
    voltage = (full_scale * raw_value) / ADC_RESOLUTION;
    
    /* display channel name.*/
    UART_send( &g_the_uart, &channel_name_lut[channel_nb][0], CHAN_NAME_MAX_SIZE );
    
    /* generate text. */
    idx = MAX_TEXT_BUFFER_SIZE;
    do {
        idx--;
        value_text_reverse[idx] = (voltage % 10) + '0';
        voltage = voltage / 10;
    } while( ( voltage > 0 ) && (idx > 0) );
    
    /* pad with spaces for alignment. */
    while( idx > (MAX_TEXT_BUFFER_SIZE - 4) )
    {
        idx--;
        value_text_reverse[idx] = ' ';
    }
    
    /* Display voltage value. */
    UART_send( &g_the_uart, &value_text_reverse[idx], MAX_TEXT_BUFFER_SIZE - idx );
    UART_send( &g_the_uart, unit, sizeof(unit) );
    
}

/***************************************************************************//**
 *
 */
void display_start( void )
{
    uint8_t mesg[] = "Press any key to display current analog inputs values.";
    UART_send( &g_the_uart, line_return, sizeof(line_return) );
    UART_send( &g_the_uart, sep, sizeof(sep) );
    UART_send( &g_the_uart, line_return, sizeof(line_return) );
    UART_send( &g_the_uart, mesg, sizeof(mesg) );
    UART_send( &g_the_uart, line_return, sizeof(line_return) );
}

/***************************************************************************//**
 *
 */
void display_update_sep( void )
{
    UART_send( &g_the_uart, line_return, sizeof(line_return) );
    UART_send( &g_the_uart, sep, sizeof(sep) );
    UART_send( &g_the_uart, line_return, sizeof(line_return) );
    g_signal_count = 0;
}


/***************************************************************************//**
 *
 */
void display_input_sep( void )
{
    uint8_t inp_sep[] = "     |      ";
    g_signal_count++;
    if ( (g_signal_count % 2) == 1 )
    {
        UART_send( &g_the_uart, inp_sep, sizeof(inp_sep) );
    }
    else
    {
        UART_send( &g_the_uart, line_return, sizeof(line_return) );
    }
}
