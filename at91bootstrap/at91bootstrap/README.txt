DESCRIPTION
===========
This is a EWARM port of Microchip AT91Bootstrap Project
https://github.com/linux4sam/at91bootstrap

AT91Bootstrap is the 2nd level bootloader for Microchip microprocessors (aka AT91).
It provides a set of algorithms to manage the hardware initialization such as
clock speed configuration, PIO settings, DRAM initialization, to download your
main application from specified boot media: NAND FLASH, serial FLASH (both
AT25-compatible of DataFlash), serial EEPROM, SD Card, etc. to main memory and
to start it.

AT91Bootstrap 4 is the next milestone for AT91Bootstrap, which adds the support
for devices instead of boards, and full implementation for each device's
external memory support and serial connection.

Supported AT91 Devices:
- SAM9X60
- SAM9X7
- SAMA5D2
- SAMA5D3
- SAMA5D4
- SAMA7D65
- SAMA7G5

COMPATIBILITY
=============

   Currently, the EWARM workspace has projects for building QSPY boot and noboot
  BootStrap for SAM9x75-Curiosity, SAMA7D65-Curiosity, SAMA7G54-EK and SAM9x60-Curiosity boards.
  The workspace is compatible with EWARM 9.70.1

CONFIGURATION
=============

   The workspace contains projects for SAM9x75, SAMA7D65, SAMA7G54 and SAM9x60 devices. 
 
  SAM9x75 project has configurations for building BootStraps for SAM9x75-Curiosity LAN Kit.
  SAMA7D65 project has configurations for building BootStraps for SAMA7D65-Curiosity
  board. This project require SAM-BA installation and path to it in
  the Environment Variables.
  SAMA7G54 project has configurations for building BootStraps for SAMA7G54-EK board.
  SAM9x60 project has configurations for building BootStraps for SAM9x60-Curiosity Kit.

   The definitions for the BootStrap are in cfg_<board>_<boot media>.h and in the
  configuration's preprocessor settings.

   A Loader project has configurations that allow programming of a BootStrap in the QSPI
  memory.

GETTING STARTED
===============

Build a BootStrap:

  1) Open AT91BootStrap workspace.

  2) Select a BootStrap project and configuration. 

  3) Use make button (or press F7) to build the BootStrap.

  4) You can use Download and Debug (Ctrl+D) to start a debug session or you can use the
     Loader project to program the BootStrap in the QSPI memory. 

Program a BootStrap with the Loader Project

  1) Open AT91BootStrap workspace and build a BootStrap.
  
  2) Switch to the Loader project and select the corresponding configuration.
  
  3) Select Project->Download->Download active application
  