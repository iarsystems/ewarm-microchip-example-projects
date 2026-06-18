/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support
 * ----------------------------------------------------------------------------
 * Copyright (c) 2009, Atmel Corporation
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

#ifndef _DEMO_BITMAPS_
#define _DEMO_BITMAPS_

//extern const uint8_t aucBmp_logo_atmel[] ;


//extern const uint8_t aucBmp_lena256x256x8[] ;
//extern const uint8_t aucBmp_lena128x128x24[] ;
//extern const uint8_t aucBmp_lena128x128x8[] ;
//extern const uint8_t aucBmp_lena128x128x24clr[] ;

//#define DEMO_BMP_PATH                            "/demo/bitmaps/"
#define DEMO_BMP_PATH                            "/demo/skins/default_8bpp/"
#define DEMO_BMP_EXT                             ".bmp"


#define DEMO_BMP_BACKGROUND                      (void*)DEMO_BMP_PATH"bmp_background"DEMO_BMP_EXT
#define DEMO_BMP_LOGO_ATMEL                      (void*)DEMO_BMP_PATH"bmp_logo_atmel"DEMO_BMP_EXT
#define DEMO_BMP_BTN_BACK                        (void*)DEMO_BMP_PATH"btn_back"DEMO_BMP_EXT
#define DEMO_BMP_CLOCK                           (void*)DEMO_BMP_PATH"bmp_clock"DEMO_BMP_EXT

// ScrIntro
#define DEMO_BMP_INTRO_SAM3S                     (void*)DEMO_BMP_PATH"Chip_SAM4S16"DEMO_BMP_EXT

// ScrMain
#define DEMO_BMP_BTN_AUDIO                       (void*)DEMO_BMP_PATH"btn_audio"DEMO_BMP_EXT
#define DEMO_BMP_BTN_VIDEO                       (void*)DEMO_BMP_PATH"btn_video"DEMO_BMP_EXT
#define DEMO_BMP_BTN_GFX_DEMO                    (void*)DEMO_BMP_PATH"btn_gfx_demo"DEMO_BMP_EXT
#define DEMO_BMP_BTN_SLIDESHOW                   (void*)DEMO_BMP_PATH"btn_slideshow"DEMO_BMP_EXT
#define DEMO_BMP_BTN_SETTINGS                    (void*)DEMO_BMP_PATH"btn_settings"DEMO_BMP_EXT
#define DEMO_BMP_BTN_QTOUCH                      (void*)DEMO_BMP_PATH"btn_qtouch"DEMO_BMP_EXT
#define DEMO_BMP_BTN_LOW_POWER_MODES             (void*)DEMO_BMP_PATH"btn_low_power_modes"DEMO_BMP_EXT
#define DEMO_BMP_BTN_INFOS                       (void*)DEMO_BMP_PATH"btn_infos"DEMO_BMP_EXT

// ScrLPModes
#define DEMO_BMP_BTN_BACKUP_MODE                 (void*)DEMO_BMP_PATH"btn_backup_mode"DEMO_BMP_EXT
#define DEMO_BMP_BTN_WAIT_MODE                   (void*)DEMO_BMP_PATH"btn_wait_mode"DEMO_BMP_EXT
#define DEMO_BMP_BTN_SLEEP_MODE                  (void*)DEMO_BMP_PATH"btn_sleep_mode"DEMO_BMP_EXT

// ScrLPModes_Backup
#define DEMO_BMP_BACKUP_MODE                     (void*)DEMO_BMP_PATH"bmp_backup_mode"DEMO_BMP_EXT

// ScrLPModes_Wait
#define DEMO_BMP_WAIT_MODE                       (void*)DEMO_BMP_PATH"bmp_wait_mode"DEMO_BMP_EXT

// ScrLPModes_Sleep
#define DEMO_BMP_SLEEP_MODE                      (void*)DEMO_BMP_PATH"bmp_sleep_mode"DEMO_BMP_EXT

// ScrAudio
//#define DEMO_BMP_UNDER_CONSTRUCTION              (void*)DEMO_BMP_PATH"bmp_under_construction"DEMO_BMP_EXT
#define DEMO_BMP_BTN_PLAY_PAUSE                  (void*)DEMO_BMP_PATH"btn_play_pause"DEMO_BMP_EXT
#define DEMO_BMP_BTN_PLAY_PAUSE_DISABLED         (void*)DEMO_BMP_PATH"btn_play_pause_disabled"DEMO_BMP_EXT
#define DEMO_BMP_BTN_PLAY_PAUSE_PRESSED          (void*)DEMO_BMP_PATH"btn_play_pause_pressed"DEMO_BMP_EXT
#define DEMO_BMP_BTN_STOP                        (void*)DEMO_BMP_PATH"btn_stop"DEMO_BMP_EXT
#define DEMO_BMP_BTN_STOP_DISABLED               (void*)DEMO_BMP_PATH"btn_stop_disabled"DEMO_BMP_EXT
#define DEMO_BMP_BTN_STOP_PRESSED                (void*)DEMO_BMP_PATH"btn_stop_pressed"DEMO_BMP_EXT
#define DEMO_BMP_BTN_VOLUME                      (void*)DEMO_BMP_PATH"btn_volume"DEMO_BMP_EXT
#define DEMO_BMP_BTN_VOLUME_DISABLED             (void*)DEMO_BMP_PATH"btn_volume_disabled"DEMO_BMP_EXT
#define DEMO_BMP_BTN_VOLUME_PRESSED              (void*)DEMO_BMP_PATH"btn_volume_pressed"DEMO_BMP_EXT
#define DEMO_BMP_BTN_RECORD                      (void*)DEMO_BMP_PATH"btn_record"DEMO_BMP_EXT
#define DEMO_BMP_BTN_RECORD_DISABLED             (void*)DEMO_BMP_PATH"btn_record_disabled"DEMO_BMP_EXT
#define DEMO_BMP_BTN_RECORD_PRESSED              (void*)DEMO_BMP_PATH"btn_record_pressed"DEMO_BMP_EXT
#define DEMO_BMP_BTN_MICROPHONE                  (void*)DEMO_BMP_PATH"btn_microphone"DEMO_BMP_EXT
#define DEMO_BMP_BTN_MICROPHONE_DISABLED         (void*)DEMO_BMP_PATH"btn_microphone_disabled"DEMO_BMP_EXT
#define DEMO_BMP_BTN_MICROPHONE_PRESSED          (void*)DEMO_BMP_PATH"btn_microphone_pressed"DEMO_BMP_EXT

// ScrSettings
#define DEMO_BMP_BTN_TIME_SETUP                  (void*)DEMO_BMP_PATH"btn_time_setup"DEMO_BMP_EXT
#define DEMO_BMP_BTN_DATE_SETUP                  (void*)DEMO_BMP_PATH"btn_date_setup"DEMO_BMP_EXT
#define DEMO_BMP_BTN_BACKLIGHT_SETUP             (void*)DEMO_BMP_PATH"btn_backlight_setup"DEMO_BMP_EXT
#define DEMO_BMP_BTN_TSD_SETUP                   (void*)DEMO_BMP_PATH"btn_settings_tsd"DEMO_BMP_EXT

#define DEMO_BMP_BTN_UP                          (void*)DEMO_BMP_PATH"btn_up"DEMO_BMP_EXT
#define DEMO_BMP_BTN_DOWN                        (void*)DEMO_BMP_PATH"btn_down"DEMO_BMP_EXT

// ScrSettings_Date

// ScrSettings_Time

// ScrSettings_Backlight
#define DEMO_BMP_CONFIG_BACKLIGHT                (void*)DEMO_BMP_PATH"bmp_config_backlight"DEMO_BMP_EXT

// ScrTSDCalibration
#define DEMO_BMP_BTN_AUDIO                       (void*)DEMO_BMP_PATH"btn_audio"DEMO_BMP_EXT

// ScrSkin


// ScrInfos
#define DEMO_BMP_INFO                            (void*)DEMO_BMP_PATH"bmp_info"DEMO_BMP_EXT


// ScrPPT

// ScrQTouch
#define DEMO_BMP_BTN_QT_UP                       (void*)DEMO_BMP_PATH"btn_up_disabled"DEMO_BMP_EXT
#define DEMO_BMP_BTN_QT_PREV                     (void*)DEMO_BMP_PATH"btn_left_disabled"DEMO_BMP_EXT
#define DEMO_BMP_BTN_QT_ENTER                    (void*)DEMO_BMP_PATH"btn_play_pause_disabled"DEMO_BMP_EXT
#define DEMO_BMP_BTN_QT_NEXT                     (void*)DEMO_BMP_PATH"btn_right_disabled"DEMO_BMP_EXT
#define DEMO_BMP_BTN_QT_DOWN                     (void*)DEMO_BMP_PATH"btn_down_disabled"DEMO_BMP_EXT

#define DEMO_BMP_BTN_QT_UP_PRESSED               (void*)DEMO_BMP_PATH"btn_up"DEMO_BMP_EXT
#define DEMO_BMP_BTN_QT_PREV_PRESSED             (void*)DEMO_BMP_PATH"btn_left"DEMO_BMP_EXT
#define DEMO_BMP_BTN_QT_ENTER_PRESSED            (void*)DEMO_BMP_PATH"btn_play_pause"DEMO_BMP_EXT
#define DEMO_BMP_BTN_QT_NEXT_PRESSED             (void*)DEMO_BMP_PATH"btn_right"DEMO_BMP_EXT
#define DEMO_BMP_BTN_QT_DOWN_PRESSED             (void*)DEMO_BMP_PATH"btn_down"DEMO_BMP_EXT

// ScrDISPDemo
// ScrMandelbrot

// ScrExplorer


#endif // _DEMO_BITMAPS_
