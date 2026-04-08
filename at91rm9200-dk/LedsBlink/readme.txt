
########################################################################
#
#                         AT91RM9200_Test.eww
#
#                                                   $Revision: 10081 $
#
########################################################################


DESCRIPTION
===========
This example project shows how to use the IAR Embedded Workbench for ARM
to develop code for the Microchip AT91 evaluation boards.
It shows basic use of parallel I/O, timer and the interrupt controller.

It starts by blinking the red USER LED.

COMPATIBILITY
=============
The project is compatible with the AT91ERM9200-DK board.

GETTING STARTED
===============
Start the IAR Embedded Workbench for ARM.
Select File->Open->Workspace...
Open the workspace file

JTAG
====
Build the project.
Select Project->Options...
In the category field select C-SPY.
In the setup tab set driver to Macraigor, J-Link or RDI depending on your JTAG hardware.
Start the debugger.
The board may require special setup in a C-SPY macro before the code is downloaded
by the debugger. The required setup is to increase the clock speed from the
initial 32 kHz and memory controller initialization. Macro files Debug_FLASH.mac/Debug_RAM.mac
are available for that purpose.

CONFIGURATION
=============
If you want to write into the external flash memory at address 0x0,
then the AT91RM9200 BMS pin must be low during reset.
