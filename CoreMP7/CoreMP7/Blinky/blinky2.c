/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : Blinky2.c
 *    Description : This shows how to set up the leds on Actel board
 *
 *    History :
 *    1. Data        : May 30, 2006
 *       Author      : Ed Harada
 *                   : Francis Cheng for the flash loader
 *       Description : Update
 *
 * Jumper settings:
 * ----------------
 *  Connect AB15 to AB17 for DebugRAM
 *  AB15 to AB17 open for DebugFlash and Release
 *
 *    $Revision: 5 $

**************************************************************************/

#pragma language=extended
__intrinsic void    __disable_interrupt(void);
__intrinsic void    __enable_interrupt(void);
__intrinsic void    __no_operation(void);

/**************************************************************************/
// Actel
// i/o pins
volatile int *LEDS     = (int *)0xC2000000; // led port address
volatile int *SWITCHES = (int *)0xC2000000; // led port address

int pat, pat1, ledmask;
#define LEDPATTERN 0x000000aa
#define LEDMASK 0x000000ff
/*************************************************************************
 * Function Name: main
 *************************************************************************/
int main (void)
{
  pat=LEDPATTERN;
  pat<<=0;  // shift pattern to proper location
  for(;;){ //main loop

    for (volatile int i=0;i<80000;i++){  // delay timer
    }

   pat1=pat; // save default led pattern.
   pat=((pat<<1)+(pat>>7));
    *LEDS=(pat|*SWITCHES);    // complement the pattern
  }
}
