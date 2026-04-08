
########################################################################
#
#                         AT91RM9200-BMP-EK-IAR.eww
#
#                                                   $Revision: 10081 $
#
########################################################################


DESCRIPTION
===========
This example project shows how to use the IAR Embedded Workbench for ARM
to develop code for the Microchip AT91 evaluation boards.
It shows basic use of EPSON VGA controller, timer and the interrupt controller
and TWI controller.

It starts by showing pictures on CRT output (800x600x56Hz). 

COMPATIBILITY
=============
The project is compatible with the AT91ERM9200-EK board.

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
initial 32 kHz and memory controller initialization. A macro file FlashAT91RM9200_EK.mac
is available for that purpose.
