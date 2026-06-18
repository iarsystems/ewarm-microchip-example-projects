/**************************************************************************
MODULE:    PROCIMG
CONTAINS:  Process Image Configuration
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

#ifndef _PROCIMG_H
#define _PROCIMG_H


/**************************************************************************
DEFINES: Definition of the process image
Modify these for your application
**************************************************************************/

// Define the size of the process image
#define PROCIMG_SIZE 16

// Define process variables: offsets into the process image 
// Digital Input 1
#define IN_digi_1 0x00
// Digital Input 2
#define IN_digi_2 0x01
// Digital Input 3
#define IN_digi_3 0x02
// Digital Input 4
#define IN_digi_4 0x03

// Analog Input 1
#define IN_ana_1 0x04
// Analog Input 2
#define IN_ana_2 0x06

// Digital Output 1
#define OUT_digi_1 0x08
// Digital Output 2
#define OUT_digi_2 0x09
// Digital Output 3
#define OUT_digi_3 0x0A
// Digital Output 4
#define OUT_digi_4 0x0B

// Analog Output 1
#define OUT_ana_1 0x0C
// Analog Output 2
#define OUT_ana_2 0x0E

#endif
