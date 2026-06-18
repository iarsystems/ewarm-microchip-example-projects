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

#include "libsam_gui.h"
/* Bitmaps */
#include "bitmaps/bitmaps.h"

/**
 * \addtogroup SAM3S_demo AT91SAM3S demo
 * @{
 * \addtogroup screens Screens
 * @{
 *
 * \brief Screens declarations
 */


/** \ref screen_intro */
extern SWGTScreen ScrIntro ;
/** \ref screen_main */
extern SWGTScreen ScrMain ;

/** \ref screen_lp_modes */
extern SWGTScreen ScrLPModes ;
/** \ref screen_backup */
extern SWGTScreen ScrLPModes_Backup ;
/** \ref screen_wait_mode */
extern SWGTScreen ScrLPModes_Wait ;
/** \ref screen_sleep */
extern SWGTScreen ScrLPModes_Sleep ;

/** \ref screen_audio */
extern SWGTScreen ScrAudio ;

/** \ref screen_settings */
extern SWGTScreen ScrSettings ;
/** \ref screen_settings_date */
extern SWGTScreen ScrSettings_Date ;
/** \ref screen_settings_time */
extern SWGTScreen ScrSettings_Time ;
/** \ref screen_settings_backlight */
extern SWGTScreen ScrSettings_Backlight ;
/** \ref screen_tsd_calibration */
extern SWGTScreen ScrTSDCalibration ;

/** \ref screen_infos */
extern SWGTScreen ScrInfos ;

/** \ref screen_ppt */
extern SWGTScreen ScrPPT ;

/** \ref screen_qtouch */
extern SWGTScreen ScrQTouch ;

/** \ref screen_disp_demo */
extern SWGTScreen ScrDISPDemo ;

/** \ref screen_mandelbrot */
extern SWGTScreen ScrMandelbrot ;


extern SWGTFrontend* g_pFrontend_TSD ;
extern SWGTFrontend* g_pFrontend_Potentiometer ;
extern SWGTFrontend* g_pFrontend_PushButtons ;
extern SWGTFrontend* g_pFrontend_QTouch ;

/** @}
 * @} */
