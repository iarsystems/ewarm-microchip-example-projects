//*----------------------------------------------------------------------------
//*         ATMEL Microcontroller Software Support  -  ROUSSET  -
//*----------------------------------------------------------------------------
//* The software is delivered "AS IS" without warranty or condition of any
//* kind, either express, implied or statutory. This includes without
//* limitation any warranty or condition with respect to merchantability or
//* fitness for any particular purpose, or against the infringements of
//* intellectual property rights of others.
//*----------------------------------------------------------------------------
//* File Name           : init.h
//* Object              : Low level initialisations written in C
//* Create              : SBb   10/06/2005
//*
//*----------------------------------------------------------------------------

#ifndef __AT91_INIT
#define __AT91_INIT

#include <string.h>
#include "AT91RM9200.h"
#include "lib_AT91RM9200.h"
#include "arm_comm.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef FALSE
#define	FALSE					  (1==0)
#endif
#ifndef TRUE
#define	TRUE					  (1==1)
#endif

#define	DELAY_PLL				100
#define DELAY_MAIN_FREQ	100

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Configuration for a Quartz 18.432000 MHz
/////////////////////////////////////////////////////////////////////////////////////////////////////

#define	PLLAR 					      0x2026BF04	//* 179,712000 MHz for PCK
#define	PLLBR 					      0x10483F0E	//* 48,054857 MHz (divider by 2 for USB)
#define	MCKR  					      0x00000202	//* PCK/3 = MCK Master Clock = 59,904000MHz with PLLA selected
#define SLOWCLOCK				      32768		    //* In Hz
#define MAIN_OSC_CLK		      18432000		//* In Hz

#define Fmck                  59904000

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Frequencies Range
/////////////////////////////////////////////////////////////////////////////////////////////////////

#define INPUT_FREQ_MIN			  900000
#define INPUT_FREQ_MAX			  32000000

#define BASE_EBI_CS0_ADDRESS	0x10000000	//* base address to access memory on CS0
#define BASE_EBI_CS1_ADDRESS	0x20000000	//* base address to access memory on CS1
#define BASE_EBI_CS3_ADDRESS	0x40000000	//* base address to access memory on CS3

#define OUTPUT_FREQ_MIN			  80000000
#define OUTPUT_FREQ_MAX			  240000000

#define USR_LED1  1
#define USR_LED2  2
#define USR_LED3  4

#define USR_LED1_ON()         AT91F_PIO_ClearOutput(AT91C_BASE_PIOB, USR_LED1)
#define USR_LED1_OFF()        AT91F_PIO_SetOutput(AT91C_BASE_PIOB, USR_LED1)

#define USR_LED2_ON()         AT91F_PIO_ClearOutput(AT91C_BASE_PIOB, USR_LED2)
#define USR_LED2_OFF()        AT91F_PIO_SetOutput(AT91C_BASE_PIOB, USR_LED2)

#define USR_LED3_ON()         AT91F_PIO_ClearOutput(AT91C_BASE_PIOB, USR_LED3)
#define USR_LED3_OFF()        AT91F_PIO_SetOutput(AT91C_BASE_PIOB, USR_LED3)

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

//*----------------------------------------------------------------------------
//* \fn    AT91F_WaitForMainClockFrequency
//* \brief This function performs very low level HW initialization
//*----------------------------------------------------------------------------
unsigned char AT91F_WaitForMainClockFrequency();

//*----------------------------------------------------------------------------
//* \fn    AT91F_CheckPLL_FrequencyRange
//* \brief This function performs very low level HW initialiszation
//*----------------------------------------------------------------------------
unsigned char AT91F_CheckPLL_FrequencyRange(int MainClock,int pllDivider ,int pllMultiplier);

//*----------------------------------------------------------------------------
//* \fn    AT91F_InitClocks
//* \brief This function performs very low level HW initialization
//*----------------------------------------------------------------------------
unsigned char AT91F_InitClocks(int PLLAR_Register,int PLLBR_Register ,int MCKR_Register);

//*----------------------------------------------------------------------------
//* \fn    AT91F_InitSDRAM
//* \brief This function performs very low level HW initialisation
//*----------------------------------------------------------------------------
void AT91F_InitSDRAM();

//*----------------------------------------------------------------------------
//* \fn    AT91F_DBGU_Printk
//* \brief This function is used to send a string through the DBGU channel (Very low level debugging)
//*----------------------------------------------------------------------------
void AT91F_DBGU_Printk(char *buffer);

//*----------------------------------------------------------------------------
//* \fn    AT91F_DataAbort
//* \brief This function reports an Abort
//*----------------------------------------------------------------------------
void AT91F_SpuriousHandler();

//*----------------------------------------------------------------------------
//* \fn    AT91F_DataAbort
//* \brief This function reports an Abort
//*----------------------------------------------------------------------------
void AT91F_DataAbort();

//*----------------------------------------------------------------------------
//* \fn    AT91F_FetchAbort
//* \brief This function reports an Abort
//*----------------------------------------------------------------------------
void AT91F_FetchAbort();

//*----------------------------------------------------------------------------
//* \fn    AT91F_Undef
//* \brief This function reports an Abort
//*----------------------------------------------------------------------------
void AT91F_Undef();

//*----------------------------------------------------------------------------
//* \fn    AT91F_UndefHandler
//* \brief This function reports that no handler have been set for current IT
//*----------------------------------------------------------------------------
void AT91F_UndefHandler();

//*----------------------------------------------------------------------------
//* \fn    AT91RM9200InitAIC
//* \brief This function performs AIC initialization
//*----------------------------------------------------------------------------
void AT91RM9200InitAIC(void);

#endif // __AT91_INIT
