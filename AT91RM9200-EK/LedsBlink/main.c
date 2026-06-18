/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2005
 *
 *    File name   : main.c
 *    Description : Define main module
 *
 *    History :
 *    1. Data        : October, 13 2005
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 10 $
**************************************************************************/
#include <intrinsics.h>
#include <stdio.h>
#include <string.h>

#include "AT91RM9200.h"
#include "lib_AT91RM9200.h"

#ifndef FALSE
#define	FALSE					        (1==0)
#endif
#ifndef TRUE
#define	TRUE					        (1==1)
#endif

#define	DELAY_PLL				      100
#define DELAY_MAIN_FREQ	      100

// Configuration for a Quartz 18.432000 MHz
#define	PLLAR 					      0x2026BF04	//* 179,712000 MHz for PCK
#define	PLLBR 					      0x10483F0E	//* 48,054857 MHz (divider by 2 for USB)
#define	MCKR  					      0x00000202	//* PCK/3 = MCK Master Clock = 59,904000MHz with PLLA selected
#define SLOWCLOCK				      32768		    //* In Hz
#define MAIN_OSC_CLK		      18432000		//* In Hz

#define Fmck                  59904000

#define SYS_TMR_TICK_PER_SEC  10000

#define _0_5_SEC              (int)(SYS_TMR_TICK_PER_SEC * 0.5)
#define _1_0_SEC              (int)(SYS_TMR_TICK_PER_SEC * 1  )
#define _2_0_SEC              (int)(SYS_TMR_TICK_PER_SEC * 2  )

// User Leds
#define USR_LED1  1
#define USR_LED2  2
#define USR_LED3  4

#define USR_LED1_ON()         AT91F_PIO_ClearOutput(AT91C_BASE_PIOB, USR_LED1)
#define USR_LED1_OFF()        AT91F_PIO_SetOutput(AT91C_BASE_PIOB, USR_LED1)

#define USR_LED2_ON()         AT91F_PIO_ClearOutput(AT91C_BASE_PIOB, USR_LED2)
#define USR_LED2_OFF()        AT91F_PIO_SetOutput(AT91C_BASE_PIOB, USR_LED2)

#define USR_LED3_ON()         AT91F_PIO_ClearOutput(AT91C_BASE_PIOB, USR_LED3)
#define USR_LED3_OFF()        AT91F_PIO_SetOutput(AT91C_BASE_PIOB, USR_LED3)

typedef void (*VoidFpnt_t)(void);

//* Message buffer
char MsgBuffer[256];
//* system timer counter
volatile unsigned int StTick;

/*************************************************************************
 * Function Name: AT91F_IRQ_HANDLER
 * Parameters: none
 *
 * Return: none
 *
 * Description: IRQ handler subroutine
 *
 *************************************************************************/
__irq __arm void AT91F_IRQ_HANDLER(void)
{
VoidFpnt_t Fpnt = (VoidFpnt_t)AT91C_BASE_AIC->AIC_IVR;
  AT91C_BASE_AIC->AIC_IVR = 0; // Debug mode
  Fpnt();
  AT91F_AIC_AcknowledgeIt(AT91C_BASE_AIC);
}

/*************************************************************************
 * Function Name: AT91F_InitClocks
 * Parameters: int PLLAR_Register,int PLLBR_Register ,int MCKR_Register
 *
 * Return: none
 *
 * Description: IRQ handler subroutine
 *
 *************************************************************************/
unsigned char AT91F_InitClocks(int PLLAR_Register,int PLLBR_Register ,int MCKR_Register)
{
volatile char tmp = 0;

  // Eanble main clock OSC and wait until expire the delay counter
  AT91C_BASE_CKGR->CKGR_MOR = 0x0000FF01;   // PMC_MOR: MOSCEN = 1, enable main clock
  while((AT91C_BASE_CKGR->CKGR_MCFR & AT91C_CKGR_MAINRDY) == 0);  // wair time out

  // Setting PLLA and Divider A
	AT91C_BASE_CKGR->CKGR_PLLAR = PLLAR_Register;
	//* Wait for PLLA stabilization LOCKA bit in PMC_SR
	tmp = 0;
	while( !(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_LOCKA) && (tmp++ < DELAY_PLL) ) ;

  // Setting PLLB and Divider B
	AT91C_BASE_CKGR->CKGR_PLLBR = PLLBR_Register;
	//* Wait for PLLB stabilization LOCKB bit in PMC_SR
	tmp = 0;
	while( !(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_LOCKB) && (tmp++ < DELAY_PLL) ) ;

  // Selection of Master Clock MCK (and Processor Clock PCK)

	//* Constraints of the Master Clock selection sequence
	//* Write in the MCKR dirty value concerning the clock selection CSS then overwrite it in a second sequence
	AT91C_BASE_PMC->PMC_MCKR = AT91C_PMC_CSS_MAIN_CLK;

  //* Wait until the master clock is established
  tmp = 0;
	while( !(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY) && (tmp++ < DELAY_MAIN_FREQ) );

  //* Second sequence
  //	AT91C_BASE_PMC->PMC_MCKR = MCKR_Register;
	AT91C_BASE_PMC->PMC_MCKR = MCKR_Register;

  //* Wait until the master clock is established
	tmp = 0;
	while( !(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY) && (tmp++ < DELAY_MAIN_FREQ) );

	return TRUE;
}

/*************************************************************************
 * Function Name: AT91F_InitFlash
 * Parameters: none
 *
 * Return: none
 *
 * Description: IRQ handler subroutine
 *
 *************************************************************************/
void AT91F_InitFlash(void)
{
	AT91C_BASE_MC->MC_PUIA[0] = AT91C_MC_PROT_PNAUNA;
	AT91C_BASE_MC->MC_PUP     = 0;
	AT91C_BASE_MC->MC_PUER    = 0;	//* Memory controller protection unit disable
	AT91C_BASE_MC->MC_ASR     = 0;  //* read only!
	AT91C_BASE_MC->MC_AASR    = 0;  //* read only!

	//* Setup MEMC to support CS0=Flash
	AT91C_BASE_EBI->EBI_CSA |= AT91C_EBI_CS0A_SMC;
	AT91C_BASE_EBI->EBI_CFGR = (AT91C_EBI_DBPUC & 0x00) | (AT91C_EBI_EBSEN & 0x00);

	//* Setup Flash
	AT91C_BASE_SMC2->SMC2_CSR[0] = (AT91C_SMC2_NWS & 0x4) | AT91C_SMC2_WSEN
									| (AT91C_SMC2_TDF & 0x200) | AT91C_SMC2_BAT | AT91C_SMC2_DBW_16;
}

/*************************************************************************
 * Function Name: AT91F_ST_HANDLER
 * Parameters: none
 *
 * Return: none
 *
 * Description: Get current count
 *
 *************************************************************************/
void AT91F_ST_HANDLER(void)
{
volatile int StStatus;
	// Read the system timer status register
	StStatus = *(AT91C_ST_SR);
	StTick++;
}


/*************************************************************************
 * Function Name: main
 * Parameters: none
 *
 * Return: none
 *
 * Description: Main subroutine
 *
 *************************************************************************/
void main(void)
{
volatile int StStatus;
unsigned int i;

  // iRam remap
#ifndef NDEBUG
  // Remap iRAM
  *(int *)0x200000 = 0xAAAA5555;
  if(*(int *)0 != 0xAAAA5555)
  {
    AT91C_BASE_MC->MC_RCR ^= 1;
  }
  // copy interrupts vectors
  memcpy((int*)0x00000000,(int *)0x10000000,0x40);
#else
  // Remap xFlash
  *(int *)0x200000 = 0xAAAA5555;
  if(*(int *)0 == 0xAAAA5555)
  {
    AT91C_BASE_MC->MC_RCR ^= 1;
  }
#endif

  // Enable IOB clock
  AT91F_PIOB_CfgPMC();
  AT91F_PIO_CfgOutput(AT91C_BASE_PIOB,USR_LED1 | USR_LED2 | USR_LED3);
  AT91F_PIO_SetOutput(AT91C_BASE_PIOB,USR_LED1 | USR_LED2 | USR_LED3);

  // Flash AT49BV6416 Initialisation : must be done before change the clocks -> Set correct Wait States
	AT91F_InitFlash();

  // Set Clocks
	AT91F_InitClocks(PLLAR,PLLBR,MCKR);
	
  // Init AIC controller
  // Reset all source vector and source mode registers
  for(i = 0; i < 32; i++)
  {
		AT91F_AIC_DisableIt(AT91C_BASE_AIC, i);
		AT91F_AIC_ConfigureIt(AT91C_BASE_AIC, i, AT91C_AIC_PRIOR_LOWEST, AT91C_AIC_SRCTYPE_INT_LEVEL_SENSITIVE, NULL);
  }
  // Clear AIC stack
  for(i = 0; i < 8; i++)
    AT91F_AIC_AcknowledgeIt(AT91C_BASE_AIC);

	AT91C_BASE_AIC->AIC_SPU = (unsigned int) NULL;
	AT91C_BASE_AIC->AIC_DCR = 1;

  // Init System timer 100us
	AT91C_BASE_ST->ST_IDR = AT91C_ST_PITS;	/* Interrupt disable Register    */
  AT91C_BASE_ST->ST_PIMR = 3;			        /* Period Interval Mode Register */
  // Read the system timer status register
	StStatus = *(AT91C_ST_SR);
  StTick = 0;
	AT91F_ST_EnableIt(AT91C_BASE_ST, AT91C_ST_PITS);
	AT91F_AIC_ConfigureIt (	AT91C_BASE_AIC,            // AIC base address
							AT91C_ID_SYS,                          // System peripheral ID
							AT91C_AIC_PRIOR_HIGHEST,               // Max priority
							AT91C_AIC_SRCTYPE_INT_LEVEL_SENSITIVE, // Level sensitive
							(VoidFpnt_t)AT91F_ST_HANDLER );						
	//* Enable ST interrupt
	AT91F_AIC_EnableIt(AT91C_BASE_AIC, AT91C_ID_SYS);

  // Enable interrupts.
  __enable_interrupt();

  while(1)
  {
    // ~ Tick 0.5 sec BLINK User LED1
    if((StTick & 0x1000) == 0)
    {
      USR_LED1_OFF();
    }
    else
    {
      USR_LED1_ON();
    }
    // ~ Tick 1 sec BLINK User LED1
    if((StTick & 0x2000) == 0)
    {
      USR_LED2_OFF();
    }
    else
    {
      USR_LED2_ON();
    }
    if((StTick & 0x4000) == 0)
    {
      USR_LED3_OFF();
    }
    else
    {
      USR_LED3_ON();
    }
  }	
}

