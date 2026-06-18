/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support
 * ----------------------------------------------------------------------------
 * Copyright (c) 2010, Atmel Corporation
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Atmel's name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ----------------------------------------------------------------------------
 */

/* Include QTouch definitions and initialization API */
#include "libqtouch.h"

/* Include AT91SAM3S-EK definitions and initialization API */
#include "board.h"

/* Include RTOS Wrapper API from SAM-GUI */
#include "libsam_gui.h"

#include <string.h>
#include <stdio.h>
#include <assert.h>

/*----------------------------------------------------------------------------
  prototypes
----------------------------------------------------------------------------*/

/*  Assign the parameters values to global configuration parameter structure    */
static void qt_set_parameters( void );

/* Configure the sensors */
static void config_sensors(void);


/*----------------------------------------------------------------------------
                                    macros
----------------------------------------------------------------------------*/
/* Timer period in msec. */
uint16_t qt_measurement_period_msec = 50u;

#define GET_SENSOR_STATE(SENSOR_NUMBER) (qt_measure_data.qt_touch_status.sensor_states[(SENSOR_NUMBER/8)] & (1 << (SENSOR_NUMBER % 8)))
#define GET_ROTOR_SLIDER_POSITION(ROTOR_SLIDER_NUMBER) qt_measure_data.qt_touch_status.rotor_slider_values[ROTOR_SLIDER_NUMBER]


/*----------------------------------------------------------------------------
                                extern variables
----------------------------------------------------------------------------*/
/* This configuration data structure parameters if needs to be changed will be
   changed in the qt_set_parameters function */
extern qt_touch_lib_config_data_t qt_config_data;
/* measurement data */
extern qt_touch_lib_measure_data_t qt_measure_data;
/* Get sensor delta values */
extern int16_t qt_get_sensor_delta( uint8_t sensor);
/* Output can be observed in the watch window using this pointer */
qt_touch_lib_measure_data_t *pqt_measure_data = &qt_measure_data;
/*----------------------------------------------------------------------------
  static variables
----------------------------------------------------------------------------*/

/* current time, set by timer ISR */
static volatile uint16_t current_time_ms_touch = 0u;

/**
 * \brief QTouchInitialize
 * QTouchInitialize initialise the QTouch interface
*/
int QTouchInitialize( void )
{
    /* reset touch sensing */
    qt_reset_sensing();




    /* Configure the Sensors as keys or Keys With Rotor/Sliders in this function */
    config_sensors();

    /* initialise touch sensing */
    qt_init_sensing();

    /* Set the parameters like recalibration threshold, Max_On_Duration etc in this function by the user */
    qt_set_parameters( );




    /* Address to pass address of user functions */
    /* This function is called after the library has made capacitive measurements,
    * but before it has processed them. The user can use this hook to apply filter
    * functions to the measured signal values.(Possibly to fix sensor layout faults) */
    qt_filter_callback = 0 ;





    /* enable interrupts */
    //sei() ;

    return 0 ;
}


/**
 * \brief TaskQTouch task.
 * This task periodically update the QTouch.
 */
void TaskQTouch( void* pParameter )
{
    uint16_t status_flag = 0u;
    uint16_t burst_flag = 0u;
    static unsigned old_position=0;
    unsigned char vld_pressed=0;
    unsigned char up_pressed=0;
    unsigned char dwn_pressed=0;
    unsigned char lft_pressed=0;
    unsigned char rgt_pressed=0;

    /* loop forever */
    for ( ; ; )
    {
        //sei() ;
        do {
            /*  one time measure touch sensors    */
            status_flag = qt_measure_sensors( current_time_ms_touch );

            burst_flag = status_flag & QTLIB_BURST_AGAIN;

            /*Time critical host application code goes here*/

        }while (burst_flag) ;

        /*  Time Non-critical host application code goes here  */

        // SLIDER: PA0, PA1, PA2, PA3, PA4, PA5: CHANNEL 0
        if ( GET_ROTOR_SLIDER_POSITION( 0 ) != old_position )
        {
            old_position = GET_ROTOR_SLIDER_POSITION( 0 ) ;
            WGT_SendMessage(WGT_MSG_KEY_PRESSED, WGT_KEY_S1, 255-old_position ) ;
        }

        if ( (GET_SENSOR_STATE( 0 ) != 0 )&&(vld_pressed==0) )
        {
            vld_pressed=1 ;
            WGT_SendMessage(WGT_MSG_KEY_PRESSED, WGT_KEY_K3, 0 ) ;
        }
        else
        {
            if ( (GET_SENSOR_STATE( 0 ) == 0)&&(vld_pressed==1) )
            {
                vld_pressed=0 ;
                WGT_SendMessage(WGT_MSG_KEY_RELEASED, WGT_KEY_K3, 0 ) ;
            }
        }

        if ( ( GET_SENSOR_STATE( 1 ) != 0 ) && (up_pressed==0) )
        {
            up_pressed=1 ;
            WGT_SendMessage( WGT_MSG_KEY_PRESSED, WGT_KEY_K1, 0 ) ;
        }
        else
        {
          if ( ( GET_SENSOR_STATE( 1 ) == 0) && (up_pressed==1) )
          {
              up_pressed=0 ;
              WGT_SendMessage( WGT_MSG_KEY_RELEASED, WGT_KEY_K1, 0 ) ;
          }
        }

        if ( (GET_SENSOR_STATE( 2 ) != 0) && (dwn_pressed==0) )
        {
            dwn_pressed=1 ;
            WGT_SendMessage( WGT_MSG_KEY_PRESSED, WGT_KEY_K5, 0 ) ;
        }
        else
        {
          if ( (GET_SENSOR_STATE( 2 ) == 0) && (dwn_pressed==1) )
          {
              dwn_pressed=0 ;
              WGT_SendMessage( WGT_MSG_KEY_RELEASED, WGT_KEY_K5, 0 ) ;
          }
        }

        if ( (GET_SENSOR_STATE( 3 ) != 0) && (lft_pressed == 0) )
        {
            lft_pressed=1 ;
            WGT_SendMessage( WGT_MSG_KEY_PRESSED, WGT_KEY_K2, 0 ) ;
        }
        else
        {
            if ( (GET_SENSOR_STATE( 3 ) == 0) && (lft_pressed==1) )
            {
                lft_pressed=0 ;
                WGT_SendMessage( WGT_MSG_KEY_RELEASED, WGT_KEY_K2, 0 ) ;
            }
        }

        if ( (GET_SENSOR_STATE( 4 ) != 0 ) && (rgt_pressed == 0) )
        {
            rgt_pressed=1 ;
            WGT_SendMessage( WGT_MSG_KEY_PRESSED, WGT_KEY_K4, 0 ) ;
        }
        else
        {
            if ( (GET_SENSOR_STATE( 4 ) == 0) && (rgt_pressed == 1) )
            {
                rgt_pressed=0 ;
                WGT_SendMessage( WGT_MSG_KEY_RELEASED, WGT_KEY_K4, 0 ) ;
             }
        }

        SAMGUI_TaskDelay( qt_measurement_period_msec ) ;
        
        /* update the current time */
        current_time_ms_touch += qt_measurement_period_msec;
    }
}

/*============================================================================
Name    :   qt_set_parameters
------------------------------------------------------------------------------
Purpose :   This will fill the default threshold values in the configuration
            data structure.But User can change the values of these parameters .
Input   :   n/a
Output  :   n/a
Notes   :   initialize configuration data for processing
============================================================================*/

static void qt_set_parameters( void )
{
    /*  This will be modified by the user to different values   */
    qt_config_data.qt_di              = DEF_QT_DI;
    qt_config_data.qt_neg_drift_rate  = DEF_QT_NEG_DRIFT_RATE;
    qt_config_data.qt_pos_drift_rate  = DEF_QT_POS_DRIFT_RATE;
    qt_config_data.qt_max_on_duration = DEF_QT_MAX_ON_DURATION;
    qt_config_data.qt_drift_hold_time = DEF_QT_DRIFT_HOLD_TIME;
    qt_config_data.qt_recal_threshold = DEF_QT_RECAL_THRESHOLD;
    qt_config_data.qt_pos_recal_delay = DEF_QT_POS_RECAL_DELAY;
}

/*============================================================================
  Name : config_sensors
------------------------------------------------------------------------------
Purpose :   Configure the sensors
Input  : n/a
Output : n/a
Notes  :
               ChanelX  ChannelY
         -----------------------
    PIOC     0  1  chanel 0
    PIOC     2  3  chanel 1
    PIOC     4  5  chanel 2
    PIOC     6  7  chanel 3
    PIOC     8  9  chanel 4
    PIOC    10 11  chanel 5
    PIOC    12 13  chanel 6
    PIOC    14 15  chanel 7
    PIOC    16 17  chanel 8
    PIOC    18 19  chanel 9
    PIOC    20 21  chanel 10
    PIOC    22 23  chanel 11   : Valid
    PIOC    24 25  chanel 12   : UP
    PIOC    26 27  chanel 13   : Down
    PIOC    28 29  chanel 14   : Left
    PIOC    30 31  chanel 15   : Right
    PIOA     0  1  chanel 16   : slider
    PIOA     2  3  chanel 17   : slider
    PIOA     4  5  chanel 18   : slider
    PIOA     6  7  chanel 19
    PIOA     8  9  chanel 20
    PIOA    10 11  chanel 21
    PIOA    12 13  chanel 22
    PIOA    14 15  chanel 23
    PIOA    16 17  chanel 24
    PIOA    18 19  chanel 25
    PIOA    20 21  chanel 26
    PIOA    22 23  chanel 27
    PIOA    24 25  chanel 28
    PIOA    26 27  chanel 29
    PIOA    28 29  chanel 30
    PIOA    30 31  chanel 31

============================================================================*/
static void config_sensors(void)
{
    PMC_EnablePeripheral(ID_PIOA);
    PMC_EnablePeripheral(ID_PIOC);

    /* Valid: SNS = PC22, SNSK = PC23 */
    qt_enable_key( CHANNEL_11, AKS_GROUP_1, 20u, HYST_6_25 );

    /* Up: SNS = PC24, SNSK = PC25 */
    qt_enable_key( CHANNEL_12, AKS_GROUP_1, 20u, HYST_6_25 );

    /* Down: SNS = PC26, SNSK = PC27 */
    qt_enable_key( CHANNEL_13, AKS_GROUP_1, 20u, HYST_6_25 );

    /* Left: SNS = PC28, SNSK = PC29 */
    qt_enable_key( CHANNEL_14, AKS_GROUP_1, 20u, HYST_6_25 );

    /* Right: SNS = PC30, SNSK = PC31 */
    qt_enable_key( CHANNEL_15, AKS_GROUP_1, 20u, HYST_6_25 );

    /* Slider: PA0, PA1, PA2, PA3, PA4, PA5 */
    qt_enable_slider( CHANNEL_16, CHANNEL_18, AKS_GROUP_1, 16u, HYST_6_25, RES_8_BIT, 0u  );
}




//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
