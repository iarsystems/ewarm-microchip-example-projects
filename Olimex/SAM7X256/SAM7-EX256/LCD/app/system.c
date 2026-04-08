/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : system.c
 *    Description : System initialization functions
 *
 *    History :
 *    1. Date        : Aug 7, 2006
 *       Author      : Todor Atanasov
 *       Description : Created
 *
 *    $Revision: 10 $
 **************************************************************************/
#include "system.h"
#include "ioat91sam7x256.h"

AT91PS_PMC pPMC   = AT91C_BASE_PMC;

AT91PS_PIO    p_pPioA  = AT91C_BASE_PIOA;
AT91PS_PIO    p_pPioB  = AT91C_BASE_PIOB;
AT91PS_PMC    p_pPMC   = AT91C_BASE_PMC;
AT91PS_USART  p_pUSART = AT91C_BASE_US0;
AT91PS_PDC    p_pPDC   = AT91C_BASE_PDC_US0;
AT91PS_MC     p_pMC    = AT91C_BASE_MC;
AT91PS_AIC    p_pAic   = AT91C_BASE_AIC;
AT91PS_SYS    p_pSys   = AT91C_BASE_SYS;

void InitFrec(void)
{
  // Set Flash Wait state - Flash Memory Controler
  AT91C_BASE_MC->MC_FMR = ((AT91C_MC_FMCN)&(48 <<16)) | AT91C_MC_FWS_1FWS ;

  // Watchdog Disabled
  AT91C_BASE_WDTC->WDTC_WDMR= AT91C_WDTC_WDDIS;

  // Set MCK at 47 923 200
  // Enable the Main Oscillator:
  // SCK = 1/32768 = 30.51 uSecond
  // Start up time = 8 * 6 / SCK = 56 * 30.51 = 1,46484375 ms
  pPMC->PMC_MOR = (( AT91C_CKGR_OSCOUNT & (0x06 <<8) | AT91C_CKGR_MOSCEN ));

  // Wait the startup time
  while(!(pPMC->PMC_SR & AT91C_PMC_MOSCS));

  // Set PLL and divider:
  // - div by 5 Fin = 3,6864 =(18,432 / 5)
  // - Mul 25+1: Fout =	95,8464 =(3,6864 *26)
  // for 96 MHz the error is 0.16%
  // PLLCOUNT pll startup time estimate at : 0.844 ms
  // PLLCOUNT 28 = 0.000844 /(1/32768)
  pPMC->PMC_PLLR = ((AT91C_CKGR_DIV & 0x05) | (AT91C_CKGR_PLLCOUNT & (28<<8)) | (AT91C_CKGR_MUL & (25<<16)));

  // Wait the startup time
  while(!(pPMC->PMC_SR & AT91C_PMC_LOCK));
  while(!(pPMC->PMC_SR & AT91C_PMC_MCKRDY));

  // Selection of Master Clock and Processor Clock
  // select the PLL clock divided by 2
  pPMC->PMC_MCKR =  AT91C_PMC_PRES_CLK_2 ;
  while(!(pPMC->PMC_SR & AT91C_PMC_MCKRDY));

  pPMC->PMC_MCKR |= AT91C_PMC_CSS_PLL_CLK  ;
  while(!(pPMC->PMC_SR & AT91C_PMC_MCKRDY));
}

void InitPeriphery(void) {
  //enable the clock of the PIOA
  p_pPMC->PMC_PCER = 1 << AT91C_ID_PIOA;
  //enable the clock of the PIOB
  p_pPMC->PMC_PCER = 1 << AT91C_ID_PIOB;

  // GPIO init
  p_pPioA->PIO_ODR    = 0xffffffff;   // All as input
  p_pPioB->PIO_ODR    = 0xffffffff;   // All as input
  p_pSys->PIOA_PPUDR  = 0xffffffff;   // Disable Pull-up resistor
  p_pSys->PIOB_PPUDR  = 0xffffffff;   // Disable Pull-up resistor

  // BUTTON SW1
  p_pPioB->PIO_ODR |= BIT24; // Input
  p_pPioB->PIO_PER |= BIT24; // Enable PB24

  // BUTTON SW2
  p_pPioB->PIO_ODR |= BIT25; // Input
  p_pPioB->PIO_PER |= BIT25; // Enable PB25
}

