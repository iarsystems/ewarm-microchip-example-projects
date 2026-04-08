/***************************************************************************//**
 * (c) Copyright 2008 Actel Corporation.  All rights reserved.
 * 
 * CoreWatchdog driver implementation.
 *
 *
 * SVN $Revision: 5 $
 * SVN $Date: 2013-12-23 13:52:47 +0100 (mån, 23 dec 2013) $
 ******************************************************************************/
#include "hal.h"
#include "corewatchdog_regs.h"
#include "core_watchdog.h"
#include "hal_assert.h"

/***************************************************************************//**
*							INTERNAL DEFINES
*/

static wd_instance_t* NULL_instance; /* NULL value with appropriate typedef*/

/***************************************************************************//**
 * WD_init()
 * See "core_watchdog.h" for details of how to use this function.
 */
void
WD_init
(
	wd_instance_t *instance,
	addr_t base,
	uint32_t value,
	uint32_t scale
)
{
	HAL_ASSERT( instance != NULL_instance )
	HAL_ASSERT( base != 0 )
	HAL_ASSERT( ( base & 0x3 ) == 0 )
	HAL_ASSERT( scale <= WD_PRESCALER_DIV_1024 )

	instance->base_address = base;

    /* Make sure the watchdog timer is disabled. */
    HAL_set_32bit_reg_field( instance->base_address, WDOGCONTROL_WATCHDOG_ENABLE, 0 );
    
    /* Configure prescaler and load value. */
	HAL_set_32bit_reg_field( instance->base_address, WDOGCONTROL_PRESCALE, scale );
	HAL_set_32bit_reg( instance->base_address, WDOGLOAD, value );
}

/***************************************************************************//**
 * WD_enable()
 * See "core_watchdog.h" for details of how to use this function.
 */
void
WD_enable
(
	wd_instance_t *instance
)
{
	HAL_ASSERT( instance != NULL_instance )

	HAL_set_32bit_reg_field( instance->base_address, WDOGCONTROL_WATCHDOG_ENABLE, 1 );
}


/***************************************************************************//**
 * WD_disable()
 * See "core_watchdog.h" for details of how to use this function.
 */
void
WD_disable
(
	wd_instance_t *instance
)
{
	HAL_ASSERT( instance != NULL_instance )
    
	HAL_set_32bit_reg_field( instance->base_address, WDOGCONTROL_WATCHDOG_ENABLE, 0 );
}


/***************************************************************************//**
 * WD_reload()
 * See "core_watchdog.h" for details of how to use this function.
 */
void
WD_reload
(
	wd_instance_t *instance
)
{
	HAL_ASSERT( instance != NULL_instance )
    
	HAL_set_32bit_reg( instance->base_address, WDOGREFRESH, 1 );
}


/***************************************************************************//**
 * WD_current_value()
 * See "core_watchdog.h" for details of how to use this function.
 */
uint32_t
WD_current_value
(
	wd_instance_t *instance
)
{
	HAL_ASSERT( instance != NULL_instance )
    
	return HAL_get_32bit_reg( instance->base_address, WDOGVALUE );
}
