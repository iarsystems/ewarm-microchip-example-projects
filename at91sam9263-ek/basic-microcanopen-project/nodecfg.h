/**************************************************************************
MODULE:    NODECFG
CONTAINS:  MicroCANopen Node Configuation
COPYRIGHT: Embedded Systems Academy, Inc. 2002-2007.
           All rights reserved. www.microcanopen.com
           This software was written in accordance to the guidelines at
           www.esacademy.com/software/softwarestyleguide.pdf
DISCLAIM:  Read and understand our disclaimer before using this code!
           www.esacademy.com/disclaim.htm
LICENSE:   THIS IS THE EDUCATIONAL VERSION OF MICROCANOPEN
           See file license_educational.txt or
           www.microcanopen.com/license_educational.txt
           A commercial MicroCANopen license is available at
           www.CANopenStore.com
VERSION:   3.30, ESA 30-JAN-07
           $LastChangedDate: 2014-01-27 15:12:42 +0100 (mån, 27 jan 2014) $
           $LastChangedRevision: 10 $
***************************************************************************/ 

#ifndef _NODECFG_H
#define _NODECFG_H

// Auto-generated file from CANopenArchitectEDS
//#include "..\Utils\pimg.h"
#include "pimg.h"

/**************************************************************************
DEFINES: MEMORY TYPES USED
**************************************************************************/

// CONST Object Dictionary Data
#define MEM_CONST const //code

// Process data and frequently used variables
#define MEM_NEAR //data

// Seldomly used variables
#define MEM_FAR //xdata


/**************************************************************************
DEFINES: DATA TYPES USED
**************************************************************************/

#define UNSIGNED8 unsigned char
#define UNSIGNED16 unsigned int
#define UNSIGNED32 unsigned long


/**************************************************************************
DEFINES: ENABLING/DISABLING CODE FUNCTIONALITY
**************************************************************************/

// Maximum number of transmit PDOs (0 to 4)
#define NR_OF_TPDOS 2

// Maximum number of receive PDOs (0 to 4)
#define NR_OF_RPDOS 2

// If defined, 1 or more TPDOs use the event timer
#define USE_EVENT_TIME

// If defined, 1 or more TPDOs are change-of-state and use the inhibit timer
#define USE_INHIBIT_TIME

// If defined, the Process Data is accesible via SDO requests
// Entries must be added to the ODProcTable in user_xxxx.c
// Suppported in commercial version of MicroCANopen available from
// www.CANopenStore.com

// #define PROCIMG_IN_OD

// If defined, OD entry [1017,00] is supported with SDO read/write accesses
// This is also an example on how to implement dynamic/variable OD entries
#define DYNAMIC_HEARTBEAT

// If set to 1, MicroLSS Slave (Layer Setting Services) are used allowing
// SW assignment of the node ID.
//#define USE_MICROLSS 1

// If defined, node starts up automatically (does not wait for NMT master)
//#define AUTOSTART

// If defined, all parameters passed to functions are checked for consistency.
// On failures, the user function MCOUSER_FatalError is called.
#define CHECK_PARAMETERS


#endif
