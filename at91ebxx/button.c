
/*
 * $Revision: 10 $
 */

#include "config.h"
#include "button.h"

static unsigned int(*button_function)();

int led_slow_speed = 0; // LED moving pattern speed flag.

static void button_event(int event)
{
  if (event == BTN_SW1_DOWN)
  {
    led_slow_speed = 1;
  }
  if (event == BTN_SW2_DOWN)
  {
    led_slow_speed = 0;
  }
}

void ButtonInit(unsigned int(*button_func)())
{
  button_function = button_func;
}


/*-------------------------------------------------------------------

  INPUT HANDLERS

  SCAN_BUTTONS
  Scans buttons, checks for up/down and down/up transitions.
  Events are added to the event queue.

--------------------------------------------------------------------*/
void ButtonScan(void)
{
  static int first = 1;
  static unsigned int lastscan;
  unsigned int thisscan;

  thisscan = (*button_function)();

  if (first)
  {
    lastscan = thisscan;
    first = 0;
  }
  
  if (thisscan != lastscan)
  {
    if ((thisscan ^ lastscan) & 0x01)
    {
      if (thisscan & 0x01)
        button_event(BTN_SW4_DOWN);
      if (~thisscan & 0x01)
        button_event(BTN_SW4_UP);
    }
     
    if ((thisscan ^ lastscan) & 0x02)
    {
      if (thisscan & 0x02)
        button_event(BTN_SW3_DOWN);
      if (~thisscan & 0x02)
        button_event(BTN_SW3_UP);
    }

    if ((thisscan ^ lastscan) & 0x04)
    {
      if (thisscan & 0x04)
        button_event(BTN_SW2_DOWN);
      if (~thisscan & 0x04)
        button_event(BTN_SW2_UP);
    }
     
    if ((thisscan ^ lastscan) & 0x08)
    {
      if (thisscan & 0x08)
        button_event(BTN_SW1_DOWN);
      if (~thisscan & 0x08)
        button_event(BTN_SW1_UP);
    }

    lastscan = thisscan;
  }
}

