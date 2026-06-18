/*******************************************************************************
 * (c) Copyright 2007 Actel Corporation.  All rights reserved.
 *
 * Cortex-M1 default exception handlers.
 *  
 * SVN $Revision: 5 $
 * SVN $Date: 2013-12-23 13:52:47 +0100 (mån, 23 dec 2013) $
 */

void cortex_nmi_handler( void );
void cortex_fault_handler( void );
void cortex_sv_call( void );
void cortex_pend_sv( void );

/*------------------------------------------------------------------------------
 * Non Maskable Interrupt.
 */
void cortex_nmi_handler( void )
{
	while( 1 )
	{
		;
	}
}
 
/*------------------------------------------------------------------------------
 * Hard Fault.
 */
void cortex_fault_handler( void )
{
	while( 1 )
	{
		;
	}
}

/*------------------------------------------------------------------------------
 * SVCall.
 */
void cortex_sv_call( void )
{
	while( 1 )
	{
		;
	}
}

/*------------------------------------------------------------------------------
 * PendSV.
 */
void cortex_pend_sv( void )
{
	while( 1 )
	{
		;
	}
}
