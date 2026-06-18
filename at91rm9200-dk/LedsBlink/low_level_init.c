/**************************************************
 *
 * This module contains the function `__low_level_init', a function
 * that is called before the `main' function of the program.  Normally
 * low-level initializations - such as setting the prefered interrupt
 * level or setting the watchdog - can be performed here.
 *
 * Note that this function is called before the data segments are
 * initialized, this means that this function cannot rely on the
 * values of global or static variables.
 *
 * When this function returns zero, the startup code will inhibit the
 * initialization of the data segments. The result is faster startup,
 * the drawback is that neither global nor static data will be
 * initialized.
 *
 * Copyright 1999-2004 IAR Systems. All rights reserved.
 *
 * $Revision: 10 $
 *
 **************************************************/

#include "AT91RM9200.h"

/* ICODE is the same segment as cstartup. By placing __low_level_init
 * in the same segment, we make sure it can be reached with BL. */

#ifdef __cplusplus
extern "C" {
#endif

#pragma language=extended

__interwork int __low_level_init(void);

#pragma location="ICODE"
__interwork int __low_level_init(void)
{
  /*==================================*/
  /*  Initialize hardware.            */
  /*==================================*/
  AT91C_BASE_ST->ST_WDMR &= ~( AT91C_ST_RSTEN ) ;  /* disable watchdog reset */
  AT91C_BASE_SMC2->SMC2_CSR[0] |= ( AT91C_SMC2_WSEN ) ;
  AT91C_BASE_SMC2->SMC2_CSR[0] |= ( 1 ) ;  /* acivate wait states before speeding up the cpu */
  if( ( AT91C_PMC_CSS_MAIN_CLK ) != ( AT91C_BASE_PMC->PMC_MCKR & AT91C_PMC_CSS ) )
    {  /* CPU does not run with main clock */
    AT91C_BASE_CKGR->CKGR_MOR |= ( AT91C_CKGR_MOSCEN ) ;  /* enable main clock */
    while( !( AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MOSCS ) )
      {  /* wait for main clock ready */
      }
    AT91C_BASE_PMC->PMC_MCKR = ( AT91C_PMC_CSS_MAIN_CLK|AT91C_PMC_PRES_CLK ) ;  /* select main clock */
    }

  /*==================================*/
  /* Choose if segment initialization */
  /* should be done or not.           */
  /* Return: 0 to omit seg_init       */
  /*         1 to run seg_init        */
  /*==================================*/
  return (1);
}

#pragma language=default

#ifdef __cplusplus
}
#endif
