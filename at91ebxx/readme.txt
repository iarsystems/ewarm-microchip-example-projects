
########################################################################
#
#                         at91_ebxx.eww
#
#                                                   $Revision: 10081 $
#
########################################################################


DESCRIPTION
===========
This example project shows how to use the IAR Embedded Workbench for ARM
to develop code for the Microchip AT91 evaluation boards.
It shows basic use of parallel I/O, serial port, timer and the interrupt
controller.

It starts by showing two patterns on the LED's separated by one second.
After another second the LED's continuously displays a moving pattern.
The speed of the moving pattern can be controlled by the SW1 and SW2
buttons (SW3 and SW4 on AT91EB40).

Serial port A is configured at 38400 baud, 8-bits, no parity and one
stop bit. The program repeatedly sends a string on the TX line.
If a character is received on the RX line, the LED moving pattern is
disabled for two seconds and the characters ascii code is displayed.
In addition the received character is displayed in the TX string.

The serial port part of the application is disabled in Angel mode
since Angel uses port A as the debug channel.

COMPATIBILITY
=============
The project is compatible with the AT91EB40, AT91EB40A, AT91EB42, AT91EB55 and
AT91EB63 boards. The display capability is limited on the AT91EB40
board since it only has three LED's.
The project is by default configured to use the Angel debug monitor.

Note that the maximum serial port speed for Angel communication supported by
the Microchip AT91 evaluation boards is 38400 baud.

GETTING STARTED
===============
Start the IAR Embedded Workbench for ARM.
Select File->Open->Workspace...
Open the workspace file
...\IAR Systems\Embedded Workbench 4.0\ARM\examples\Microchip\AT91EBxx\at91_ebxx.eww

ANGEL
=====

The default setup builds the application for download using
the Angel ROM monitor protocol.
Make sure the project target is ramcode (not flashcode), this is default.
Build the project.

Connect a serial cable between the PC and port A on the AT91 board.
Also check that Angel is activated on the board; jumper JP1 in STD
position (not USER position), see the board documentation for details.
On EB40 there is a switch instead of JP1, set the switch the LOWER MEM position.

Start the debugger.

JTAG
====
Edit config.h and disable Angel with
#define ANGEL 0
Make sure the project target is ramcode (not flashcode), this is default.
Build the project.
Select Project->Options...
In the category field select C-SPY.
In the setup tab set driver to Macraigor, J-Link or RDI depending on your JTAG hardware.
Start the debugger.
The EB55 may require special setup in a C-SPY macro before the code is downloaded
by the debugger. The required setup is to increase the clock speed from the
initial 32 kHz and memory controller initialization. A macro file at91_eb55_startup.mac
is available for that purpose.


CONFIGURATION
=============
Edit config.h to match your AT91 evaluation board, the default setting
is for the AT91EB55 board.
The AT91EB40A have separate configurations; ramcode-EB40A and flashcode-EB40A.

The project uses a modified version of cstartup (at91_cstartup.s79). The
difference from the standard cstartup is the added code to perform the
AT91 memory controller remap command.
A remap command is necessary after a hardware reset of the AT91.

The low_level_init (at91_low_level_init.c) initializes peripheral
functions that must be avaiable before main() is executed.

Two targets are configured in the project; ramcode and flashcode:

- Ramcode uses the linker configuration file at91_lnk_ram.xcl, vectors
  are located at 0x00-0x1f, code at 0x2000000-0x200ffff and data
  at 0x2010000-0x201ffff.
  The ramcode target is setup to use a JTAG interface to download the code
  into RAM.
  The ramcode target have been verified on all supported targets.

- Flashcode uses the linker configuration file at91_lnk_flash.xcl, vectors
  and code is located at 0x0000-0x7fff.
  After remap the vectors are located at 0x00-0x1f, code at
  0x1000100-0x1007fff and data at 0x2000000-0x200ffff.
  Flashing is configured to use the J-Link JTAG interface for downloading
  to flash and debugging.
  The EB40 board is not supported by the flash loader.
  Flashing assumes that the flash board jumper is in the STD position.
  To flash in the USER jumper position, use the --user flash loader argument.

The AT91EB40A board requires other address ranges than the other boards
since it lacks on-board RAM.
Use the ramcode-EB40A and flashcode-EB40A configurations for the AT91EB40A.
