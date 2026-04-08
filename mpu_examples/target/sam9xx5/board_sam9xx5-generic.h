/* ---------------------------------------------------------------------------- */
/*                Microchip Microcontroller Software Support                    */
/*                       SAM Software Package License                           */
/* ---------------------------------------------------------------------------- */
/* Copyright (c) 2019, Microchip Technology Inc.                                */
/*                                                                              */
/* All rights reserved.                                                         */
/*                                                                              */
/* Redistribution and use in source and binary forms, with or without           */
/* modification, are permitted provided that the following condition is met:    */
/*                                                                              */
/* - Redistributions of source code must retain the above copyright notice,     */
/* this list of conditions and the disclaimer below.                            */
/*                                                                              */
/* Microchip's name may not be used to endorse or promote products derived from */
/* this software without specific prior written permission.                     */
/*                                                                              */
/* DISCLAIMER:  THIS SOFTWARE IS PROVIDED BY MICROCHIP "AS IS" AND ANY EXPRESS  */
/* OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES */
/* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT    */
/* ARE DISCLAIMED. IN NO EVENT SHALL MICROCHIP BE LIABLE FOR ANY DIRECT,        */
/* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES           */
/* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; */
/* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND  */
/* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT   */
/* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF     */
/* THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.            */
/* ---------------------------------------------------------------------------- */

/**
 * \page sam9xx5_generic_board_desc sam9xx5-generic - Board Description
 *
 * \section Purpose
 *
 * This file is dedicated to describe a generic sam9xx5 board.
 *
 * \section Contents
 *
 *  - sam9xx5-generic
 *  - For sam9xx5-generic information, see \subpage sam9xx5_generic_board_info.
 *  - For operating frequency information, see \subpage sam9xx5_generic_opfreq.
 *  - For using portable PIO definitions, see \subpage sam9xx5_generic_piodef.
 *  - For on-board memories, see \subpage sam9xx5_generic_mem.
 *  - Several USB definitions are included here, see \subpage sam9xx5_generic_usb.
 *  - For External components, see \subpage sam9xx5_generic_extcomp.
 *  - For Individual chip definition, see \subpage sam9xx5_generic_chipdef.
 *
 * To get more software details and the full list of parameters related to the
 * sam9xx5-generic board configuration, please have a look at the source file:
 * \ref board.h\n
 *
 * \section Usage
 *
 *  - The code for booting the board is provided by board_cstartup_xxx.c and
 *    board_lowlevel.c.
 *  - For using board PIOs, board characteristics (clock, etc.) and external
 *    components, see board.h.
 *  - For manipulating memories, see board_memories.h.
 *
 * This file can be used as a template and modified to fit a custom board, with
 * specific PIOs usage or memory connections.
 */

/**
 *  \file board.h
 *
 *  Definition of sam9xx5-generic
 *  characteristics, sama5d4-dependant PIOs and external components interfacing.
 */

#ifndef BOARD_SAM9XX5_GENERIC_H_
#define BOARD_SAM9XX5_GENERIC_H_

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

#include "chip.h"

#include "board_support.h"

/*----------------------------------------------------------------------------
 *        HW BOARD Definitions
 *----------------------------------------------------------------------------*/

/**
 * \page sam9xx5_generic_board_info "sam9xx5-generic - Board informations"
 * This page lists several definition related to the board description.
 *
 * \section Definitions
 * - \ref BOARD_NAME
 */

/** Name of the board */
#if defined(CONFIG_BOARD_SAM9G15_GENERIC)
  #define BOARD_NAME "sam9g15-generic"
#elif defined(CONFIG_BOARD_SAM9G25_GENERIC)
  #define BOARD_NAME "sam9g25-generic"
#elif defined(CONFIG_BOARD_SAM9G35_GENERIC)
  #define BOARD_NAME "sam9g35-generic"
#elif defined(CONFIG_BOARD_SAM9X25_GENERIC)
  #define BOARD_NAME "sam9x25-generic"
#elif defined(CONFIG_BOARD_SAM9X35_GENERIC)
  #define BOARD_NAME "sam9x35-generic"
#endif

/*----------------------------------------------------------------------------*/

/** Frequency of the board main clock oscillator (autodetect) */
#define BOARD_MAIN_CLOCK_EXT_OSC 0

/** \def Board System timer */
#define BOARD_TIMER_TC      TC1
#define BOARD_TIMER_CHANNEL 0

#endif /* BOARD_SAM9XX5_GENERIC_H */
