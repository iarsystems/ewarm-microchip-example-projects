/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2005
 *
 *    File name   : sys_tmr.c
 *    Description : System timer
 *
 *    History :
 *    1. Data        : October, 12 2005
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 10 $
**************************************************************************/
#include "sys_tmr.h"

//* system timer counter
volatile Int32U StTick;

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
 * Function Name: SysTmrInit
 * Parameters: Int32U Resolution [100us]
 *
 * Return: none
 *
 * Description: Init system timer
 *
 *************************************************************************/
void SysTmrInit (Int32U Resolution)
{
	//* System Timer initialization
	AT91F_ST_SetPeriodIntervalTimer(AT91C_BASE_ST,3*Resolution);  // period <=> 100us
	AT91F_ST_EnableIt(AT91C_BASE_ST, AT91C_ST_PITS);
	AT91F_AIC_ConfigureIt (	AT91C_BASE_AIC,               // AIC base address
							AT91C_ID_SYS,                             // System peripheral ID
							AT91C_AIC_PRIOR_HIGHEST,                  // Max priority
							AT91C_AIC_SRCTYPE_INT_LEVEL_SENSITIVE,    // Level sensitive
							(VoidFpnt_t)AT91F_ST_HANDLER );						
	//* Enable ST interrupt
	AT91F_AIC_EnableIt(AT91C_BASE_AIC, AT91C_ID_SYS);
}

/*************************************************************************
 * Function Name: AT91F_GetTickCount
 * Parameters: none
 *
 * Return: Int32U
 *
 * Description: Get current count
 *
 *************************************************************************/
Int32U AT91F_GetTickCount(void)
{
	return(StTick);
}

/*************************************************************************
 * Function Name: AT91F_ST_SetPeriodIntervalTimer
 * Parameters: none
 *
 * Return: none
 *
 * Description: Get current count
 *
 *************************************************************************/
void AT91F_ST_SetPeriodIntervalTimer( AT91PS_ST pSt, unsigned int period)
{
volatile Int32U status;
	pSt->ST_IDR = AT91C_ST_PITS;	/* Interrupt disable Register */
	status = pSt->ST_SR;
  pSt->ST_PIMR = period;			  /* Period Interval Mode Register */
  StTick = 0;
}

