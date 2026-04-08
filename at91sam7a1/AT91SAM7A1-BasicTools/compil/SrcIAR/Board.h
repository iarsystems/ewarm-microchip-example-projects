/*----------------------------------------------------------------------------
*         ATMEL Microcontroller Software Support  -  ROUSSET  -
*----------------------------------------------------------------------------
* The software is delivered "AS IS" without warranty or condition of any
* kind, either express, implied or statutory. This includes without
* limitation any warranty or condition with respect to merchantability or
* fitness for any particular purpose, or against the infringements of
* intellectual property rights of others.
*----------------------------------------------------------------------------
* File Name           : Board.h
* Object              : AT91SAM7A1 Evaluation Board Features Definition File.
*
* Creation            : JPP   22/Jun/2005
*----------------------------------------------------------------------------
*/
#ifndef Board_h
#define Board_h

#include "csp.h"
#include "bsp.h"

/*-------------------------------*/
/* SAM7Board Memories Definition */
/*-------------------------------*/
/* 2-Mbytes 16-bit Flash AT91BV162A */

/* Number of Chips Select */
#define NB_CS    2               /* 2 Chips Select */

/*  Chip Select Definition */
#define AMC_CSR_0  0x400030A9    /* FLASH : Base Address = 0x40000000 */
                                 /* CS Enable, Byte Select access, 0 tdf, 4MB, 3 waitstates, 16 bits */
#define AMC_CSR_1  0x48003081    /* RAM  : Base Address = 0x48000000 */
                                 /*  CS Enable, Byte Select access, 0 tdf, 4MB, 0 waitstates, 16 bits  */
#define AMC_CSR_2  0x50000000    /* Base Address = 0x50000000 CS Disable */
#define AMC_CSR_3  0x58000000    /* Base Address = 0x58000000 CS Disable */
#define AMC_CSR_4  0x60000000    /* Base Address = 0x60000000 CS Disable */
#define AMC_CSR_5  0x68000000    /* Base Address = 0x68000000 CS Disable */
#define AMC_CSR_6  0x70000000    /* Base Address = 0x70000000 CS Disable */
#define AMC_CSR_7  0x78000000    /* Base Address = 0x78000000 CS Disable */
#define MCR_VAL    0x00000017    /* Memory Control Register Value Early Read Protocol, All chip selects valid */

/*-------------------------*/
/* Push Buttons Definition */
/*-------------------------*/

#define SW2     TIOA      /* SW2 button correspond to TIOA2 pin   */
#define SW3     TIOB      /* SW3 button correspond to TIOB1 pin   */
#define SW4     IQRQ0     /* SW4 Button correspond to IRQ0 pin(44)*/

/*--------------*/
/* Master Clock */
/*--------------*/

#define EXT_OC           6000000   // Exetrnal ocilator MAINCK

#endif /* Board_h */
