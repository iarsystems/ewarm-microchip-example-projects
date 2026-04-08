/**************************************************************************
NAME:    rCC01io.c                                                   
INFO:    In/Out module for the Atmel CANary evaluation board and Atmel
         CANopen demo board, both equipped with a T89C51CC01
         microcontroller.                 
RIGHTS:  Embedded Systems Academy   www.esacademy.com                
---------------------------------------------------------------------------
DETAILS:                                                             
---------------------------------------------------------------------------
HISTORY: V1.01 AA 15-OCT-2003 Added support for Atmel CANopen board
         V1.00 Kl 09-OCT-2002                                  
         $LastChangedDate: 2014-01-27 15:12:42 +0100 (mån, 27 jan 2014) $
         $LastChangedRevision: 10 $
**************************************************************************/

#include "mco.h"
#include "r_atmel_io.h"

/*-------------------------- LOCAL DATA --------------------------------*/

/*----------------------- GLOBAL SUBROUTINES ---------------------------*/

/**************************************************************************
DOES:    Read the settings of the DIP switches              
RETURNS: Current value of the DIP switches                    
**************************************************************************/
UNSIGNED8 read_dip_switches
  (
  void
  )
{
  UNSIGNED8 dip;

  // enable the switches
//   SWITCH_ENABLE;
  // read the switches
//   dip = P1 & 0x1F;
  // disable the switches
//   SWITCH_DISABLE;

  // return the value read from the switches
   return (dip);
}


/**************************************************************************
DOES:    Outputs an 8-bit pattern on the LEDs                 
RETURNS: nothing                                                     
**************************************************************************/
void switch_leds
  (
  UNSIGNED8 led7_0  // set bits to turn the corresponding LEDs on
  )
{
   // enable the bargraph LED display
//   BARGRAPH_ENABLE;  
   // output to the LED display
//   P1 = led7_0;  
   // disable the bargraph LED display
//   BARGRAPH_DISABLE;
}


/**************************************************************************
DOES:    If a program discovers a fatal error, this routine can   
         be used to display an error code to the user.            
         THIS FUNCTION NEVER RETURNS! ONLY A RESET WILL RECOVER   
         THE BOARD!                                               
RETURNS: nothing                                          
**************************************************************************/
void error_state
  (
  UNSIGNED8 error  // error value to be displayed on all LEDs as a blinking
              // pattern. Only values 1 - 12 can be reasonably
              // recognized by counting the blinks
  )
{
  UNSIGNED16 i,j;

  // disable all interrupts
//  EA = 0;

  // infinate loop
  while (1)
  {
    // loop the necessary number of times
    for (j = 1; j <= error; j++)
    {
      // all leds on
      for (i = 0; i <= 20000; i++)
      {
        switch_leds(0xFF);
      }
      // all leds off
      for (i = 0; i <= 20000; i++)
      {
        switch_leds(0x00);
      }
    }
    // delay before next loop
    // all leds off
    for (i = 0; i <= 50000; i++)
    {
      switch_leds(0x00);
    }
  }
}


