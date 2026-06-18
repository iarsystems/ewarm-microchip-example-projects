DESCRIPTION
===========
   This project shows how to use IAR Embedded Workbench for ARM to develop code
  for SAM9x60-Curiosity Kit.
  Running the example will make LD1 Green LED to blink.
  
COMPATIBILITY
=============

   The example project is compatible with SAM9x60-Curiosity board. By default,
  the project is configured to use J-Link debugger.
   
CONFIGURATION
=============

SRAM-Debug
   This configuration will run the example from the internal RAM.
DDRAM-Debug
   This configuration builds the example for DDRAM.

GETTING STARTED
===============

Build the Getting Started Example:

  1) Open GettingStarted workspace and GettingStarted Project.

  2) Select a configuration. 

  3) Use the Make button (or F7) to build the example.

  4) Use Download and Debug (Ctrl+D) to start a debug session.
  
Note: It is good to run a BootStrap before starting a debug session. It is
  mandatory before starting a DDRAM debug session.

 