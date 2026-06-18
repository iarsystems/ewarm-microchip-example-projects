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
 *    1. Data        : October, 10 2005
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 10 $
**************************************************************************/
#include <intrinsics.h>
#include <stdio.h>
#include "init.h"
#include "drv_twi.h"
#include "drv_1523.h"
#include "s1d13806.h"
#include "sys_tmr.h"

//* Message buffer
char MsgBuffer[256];

#define S1D13806_BASE			      ((unsigned char *) BASE_EBI_CS3_ADDRESS )
#define S1D13806_DISP_MEM_BASE	(((int)S1D13806_BASE)  + ((int) 0x200000))

const GUI_BITMAP * Bmps[] =
{
  &bm0, /*&bm1,&bm2, &bm3,*/  &bm4, /*&bm6,*/  &bm7,
};

#define BMPS_NUMB (sizeof(Bmps)/sizeof(GUI_BITMAP *))
#define _1_0_SEC    10000

/*************************************************************************
 * Function Name: main
 * Parameters: none
 *
 * Return: none
 *
 * Description: Main routine
 *
 *************************************************************************/
void main(void)
{
char message[64];
int i,initial_timeout;
const GUI_BITMAP * pBmp;

AT91PS_S1D13806_Desc	s1d13806_base = (AT91PS_S1D13806_Desc) S1D13806_BASE;

  USR_LED2_ON();
  // Debug message
	AT91F_DBGU_Printk("\n\rBasicBoot Successfull: Enter main()\n\r");
  // Init AIC
  AT91RM9200InitAIC();
  // Init System timer resolution 100us
  SysTmrInit(1);
  // Init TWI interface
  if(Twi_Init(100KHZ,0) == TwiOk)
  {
  	sprintf (message, " \n\rTWI interface Init OK\n\r" );
  }
  else
  {
 		sprintf (message, " \n\rrTWI interface Init failed\n\r" );
  }
  AT91F_DBGU_Printk(message);

  // Enable interrupts.
  __enable_interrupt();

  // Init Clock generator
  // 640  x 480 85Hz - 36.000 MHz
  // 800  x 600 85Hz - 40.000 Mhz
//  if (Ics1523_Init(CRT_Display_Conf_640_480))
  if (Ics1523_Init(CRT_Display_Conf_800_600))
  {
  	sprintf (message, " \n\rICS1523 Clock Generator Init OK\n\r" );
  }
  else
  {
 		sprintf (message, " \n\rICS1523 Clock Generator Init failed\n\r" );
  }
  AT91F_DBGU_Printk(message);
	
	AT91F_DBGU_Printk("\n\r-I- ========================================================\n\r");
	AT91F_DBGU_Printk(    "-I- AT91RM9200 Epson Embedded Memory Display Controller Test\n\r");
	AT91F_DBGU_Printk(    "-I- --------------------------------------------------------\n\r");

  // Epson S1D13806 Initialisation FOR A CRT Monitor !!!!!!!!!!!
	AT91F_S1D13806_Init ( s1d13806_base,
					S1D13806_DISP_MEM_BASE,
//				  CRT_Display_Conf_640_480,
					CRT_Display_Conf_800_600,
//					Display_Def_0,
					Display_Def_1,
          Char_Def_0,
          Lut_Def_0,
          Fmck ) ;

  AT91F_DBGU_Printk("S1D13806 Display Controller Init OK\n\r");

  // Show BMP pictures
  AT91F_DBGU_Printk("\n\rShow BMPs pictures\n\r");
  i = 0;
	while (1)
  {
    pBmp = Bmps[i++ % BMPS_NUMB];
    ShowBmp(pBmp);
  	sprintf ( message, "\n\rIAR Embedded Workbench for ARM - AT91RM9200-EK") ;
    AT91F_S1D13806_16bpp_print_string ( s1d13806_base, message );
    initial_timeout = AT91F_GetTickCount() + _1_0_SEC * 3;
    while( AT91F_GetTickCount() < initial_timeout);
	}
}

