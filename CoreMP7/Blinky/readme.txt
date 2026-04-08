Blinky, bliny1 and blinky2 blink the 8 leds in different patters. 
Pressing a button turns the corresponding led on until the button is released.
Any number of buttons can be pressed simulatneously.

Blinky2 uses a volatile int for the timer loop to ensure that it does not get optimezed away
in the release version.

