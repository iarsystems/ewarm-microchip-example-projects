/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2005
 *
 *    File name   : drv_1523.c
 *    Description : Driver of ICS1523
 *
 *    History :
 *    1. Data        : October, 12 2005
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 10 $
**************************************************************************/
#include "drv_1523.h"

const Ics1523_Ctrl_t Ics1523_Ctrl[3] =
{
  // 640  x 480 85Hz - 36.000 MHz
  {
    // 0
		( Ics1523_CPen ),
    // 1
		( Ics1523_ICP   | (Ics1523_VCOD & 0x20)),
    // 2
		0x55,
    // 3
		0x00,
    // 4
		0x00,
    // 5
		0x00,
    // 6
		( Ics1523_OE_Tck | Ics1523_OE_T2 ),
    // 7
		( Ics1523_In_Sel | Ics1523_Osc_Div ),
    // 8
    ( Ics1523_DPA    | Ics1523_PLL),
  },
  // 800  x 600 72Hz - 40.000 MHz
  {
    // 0
		( Ics1523_CPen ),
    // 1
		( Ics1523_ICP   | (Ics1523_VCOD & 0x20)),
    // 2
		0x61,
    // 3
		0x00,
    // 4
		0x00,
    // 5
		0x00,
    // 6
		( Ics1523_OE_Tck | Ics1523_OE_T2 ),
    // 7
		( Ics1523_In_Sel | Ics1523_Osc_Div ),
    // 8
    ( Ics1523_DPA    | Ics1523_PLL),
  },
};

/*************************************************************************
 * Function Name: Ics1523_Init
 * Parameters: Int32U Cfg
 *
 * Return: Boolean
 *
 * Description: Init ICS1523 driver
 *
 *************************************************************************/
Boolean Ics1523_Init (Int32U Cfg)
{
Int32U initial_timeout;
Int8U PllStatus;
  if(Cfg >= sizeof(Ics1523_Ctrl)/sizeof(Ics1523_Ctrl_t))
  {
    return(FALSE);
  }

  if(Twi_Write( Ics1523_SlaveAddress,Ics1523_ICR,
     (Int8U *)&Ics1523_Ctrl[Cfg],sizeof(Ics1523_Ctrl_t)) != TwiOk)
  {
    return(FALSE);
  }

  for (Int32S i = Ics1523_ICP-1; i > 0; --i)
  {
	  //* Program 1ms temporizing period
	  initial_timeout = AT91F_GetTickCount();
	  while( AT91F_GetTickCount() < initial_timeout + 100);
    if(Twi_Read (Ics1523_SlaveAddress,Ics1523_SR,&PllStatus,1) != TwiOk)
    {
      return(FALSE);
    }
    if(PllStatus & Ics1523_PLL_Lock)
    {
      return(TRUE);
    }
    PllStatus = (i & Ics1523_ICP) | (Ics1523_Ctrl[Cfg].lcr & ~Ics1523_ICP);
    if(Twi_Write( Ics1523_SlaveAddress,Ics1523_LCR,&PllStatus,1) != TwiOk)
    {
      return(FALSE);
    }
    PllStatus = Ics1523_PLL;
    if(Twi_Write( Ics1523_SlaveAddress,Ics1523_SWRST,&PllStatus,1) != TwiOk)
    {
      return(FALSE);
    }
  }
  return(FALSE);
}
