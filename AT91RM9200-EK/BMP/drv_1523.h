/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2005
 *
 *    File name   : drv_1523.h
 *    Description : Driver of ISC1523 Definitions
 *
 *    History :
 *    1. Data        : October, 12 2005
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 10 $
**************************************************************************/
#ifndef __DRV_1523_H
#define __DRV_1523_H

#include "drv_twi.h"
#include "sys_tmr.h"

// ICS1523 I2C clock definition
#define Ics1523_Speed         100KHZ

// ICS1523 Device Addresses Definition
#define Ics1523_SlaveAddress  0x26

// ICS1523 Registers Internal Addresses Definition

#define		Ics1523_ICR				  0x0		  /* Input Control Register */
#define		Ics1523_LCR			  	0x1		  /* Loop Control Register */
#define		Ics1523_FD0				  0x2		  /* PLL FeedBack Divider LSBs */
#define		Ics1523_FD1				  0x3		  /* PLL FeedBack Divider MSBs */
#define		Ics1523_DPAO			  0x4		  /* Dynamic Phase Aligner Offset */
#define		Ics1523_DPAC			  0x5		  /* Dynamic Phase Aligner Resolution */
#define		Ics1523_OE				  0x6		  /* Output Enables Register */
#define		Ics1523_OD				  0x7		  /* Osc Divider Register */
#define		Ics1523_SWRST			  0x8		  /* DPA & PLL Reset Register */
#define		Ics1523_VID				  0x10	  /* Chip Version Register */
#define		Ics1523_RID				  0x11	  /* Chip Revision Register */
#define		Ics1523_SR				  0x12	  /* Status Register */

// Ics1523_ICR
#define		Ics1523_CPen				0x1		  /* Charge Pump Enable */
#define		Ics1523_CP_Pol			0x2		  /* COAST Pin Charge Pump Enable Polarity */
#define		Ics1523_Ref_Pol			0x4		  /* External Reference Polarity */
#define		Ics1523_Fbk_Pol			0x8		  /* External Feedback Polarity */
#define		Ics1523_Fbk_Sel			0x10	  /* External Feedback Select */
#define		Ics1523_Func_Sel		0x20	  /* Function Out Select */
#define		Ics1523_EnPLS				0x40	  /* Enable PLL Lock/Ref Status Output */

// Ics1523_LCR bits definitions
#define		Ics1523_ICP					0x7		  /* ICP (Charge Pump Current) */
#define		Ics1523_VCOD				0x30	  /* VCO Divider */

// Ics1523_FD0
#define		Ics1523_FBDL		    0xFF		/* PLL FeedBack Divider LSBs */

// Ics1523_FD1
#define		Ics1523_FBDM		    0xF			/* PLL FeedBack Divider MSBs */

// Ics1523_DPAO
#define		Ics1523_DPA_OS			0x2F		/*Dynamic Phase Aligner Offset */
#define		Ics1523_Fil_Sel			0x80		/* Loop Filter Select */

// Ics1523_DPAC
#define		Ics1523_DPA_Res		 	0x3			/* Dynamic Phase Aligner Resolution */
#define		Ics1523_Metal_Rev		0xFC		/* Metal Mask Revision Number */

// Ics1523_OE
#define		Ics1523_OE_Pck  		0x1			/* Output Enable for PECL CLK (Pins 20, 21) */
#define		Ics1523_OE_Tck			0x2			/* Output Enable for STTL_3 CLK (Pin 17) */
#define		Ics1523_OE_P2				0x4			/* Output Enable for PECL CLK/2 (Pins 22, 23) */
#define		Ics1523_OE_T2				0x8			/* Output Enable for STTL_3 CLK/2 (Pin 16) */
#define		Ics1523_OE_F				0x10		/* Output Enable for STTL_3 FUNC Output (Pin15) */
#define		Ics1523_Ck2_Inv		  0x20		/* CLK/2 Invert */
#define		Ics1523_Out_Scl			0xC0		/* CLK Scaler (pin 17) */

// Ics1523_OD
#define		Ics1523_Osc_Div			0x7F		/* Oscillator Divider Modulus */
#define		Ics1523_In_Sel			0x80		/* Input Select */

// Ics1523_SWRST
#define		Ics1523_DPA 				0x0A		/* Writing xAh resets DPA and loads working 0x5 */
#define		Ics1523_PLL 				0x50		/* Writing 5xh resets PLL and loads working 0x1- 0x3 */

// Ics1523_VID
#define		Ics1523_Chip_Ver		0xFF		/* Chip Version 0x17*/

// Ics1523_RID
#define		Ics1523_Chip_Rev		0xFF		/* Chip Revision 0x01*/

// Ics1523_SR
#define		Ics1523_PLL_Lock		0x2			/* PLL Lock Status */

typedef struct
{
	Int8U	icr ;			/* Input Control Register */
	Int8U	lcr ;			/* Loop Control Register */
	Int8U	fd0 ;			/* PLL FeedBack Divider LSBs */
	Int8U	fd1 ;			/* PLL FeedBack Divider MSBs */
	Int8U	dpao; 	  /* Dynamic Phase Aligner Offset */
	Int8U	dpac;	  	/* Dynamic Phase Aligner Resolution */
	Int8U	oes ;			/* Output Enables Register */
	Int8U	odr ;			/* Osc Divider Register */
	Int8U	rst ;			/* reset */
} Ics1523_Ctrl_t;

/*************************************************************************
 * Function Name: Ics1523_Init
 * Parameters: Int32U Cfg
 *
 * Return: Boolean
 *
 * Description: Init ICS1523 driver
 *
 *************************************************************************/
Boolean Ics1523_Init (Int32U Cfg);

#endif //__DRV_1523_H
