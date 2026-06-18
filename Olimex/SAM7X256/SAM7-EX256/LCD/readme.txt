########################################################################
#
#                    SAM7_EX256.eww
#
#                                                   $Revision: 10834 $
#
########################################################################

DESCRIPTION
===========
  This example project shows how to use the IAR Embedded Workbench for ARM to
  develop code for the Olimex SAM7-EX256 evaluation boards.
  The main purpose of the project is to show how the LCD works. It contains
  functions for the LCD driver initialization and basic LCD functionality -
  display an image, symbol, backlight control, contrast adjustment etc. After
  running the application an image with the IAR logo will appear. Current
  measured temperature will be displayed as well. The Real Time Timer is reset
  and we will see a timer started from 00:00:00. We can set this clock by
  pressing the joystick button. This way time adjust mode is entered. Then we can
  select which values we want to change - HH:MM:SS by using LEFT and RIGHT
  joystick positions. After we have placed the marker to the desired position we
  can change its' value by using UP and DOWN positions. To enter the selected
  time press Button SW2. For ESCAPE - joystick middle position.
  In normal mode (not time adjust mode) there is a possibility to adjust the contrast.
  Press Button SW1 and hold it. Then adjust the contrast by using the TRIM.
  Pressing Button SW2 will TURN OFF the backlight. To TURN it ON - press button
  SW1. In time adjust mode by pressing Button SW1 we can change the background
  image. Currently we can choose between 5 different pictures.

  This example can work standalone on the Olimex SAM7-EX256 board.

COMPATIBILITY
=============
  The project is compatible with the Olimex SAM7-EX256 evaluation board.
  The project is by default configured to use the J-Link JTAG interface.

CONFIGURATION
=============
  The application is downloaded to the flash.

  Jumpers:
    POWER_SELECT  - depends on the power source

GETTING STARTED
===============
  1) Put the POWER_SELECT jumper on the appropriate position.
  2) Select a configuration
  3) Build and download the example.
  4) Run the program.
