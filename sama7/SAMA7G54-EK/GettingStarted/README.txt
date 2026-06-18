DESCRIPTION
===========
   This project shows how to use IAR Embedded Workbench for ARM to develop code
  for SAMA7G54-EK board.
  Running the example will make D11 Green LED to blink.
  
COMPATIBILITY
=============

   The example project is compatible with SAMA7G54-EK board. By default,
  the project is configured to use J-Link debugger.
   
CONFIGURATION
=============

SRAM-Debug
   This configuration will run the example from the internal RAM.
DDRAM-Debug
   This configuration builds the example for DDRAM.
QSPI-Debug
   The code will run from QSPI Flash (XiP)

Loader project allows programming the code build for DDRAM to QSPI memory. The
  image can be loaded to the DDRAM with a BootStrap.

GETTING STARTED
===============

Build the Getting Started Example:

  1) Open GettingStarted workspace and GettingStarted Project.

  2) Select a configuration. 

  3) Use the Make button (or F7) to build the example.

  4) Use Download and Debug (Ctrl+D) to start a debug session.
  
Note: It is good to run a BootStrap before starting a debug session. It is
  mandatory before starting a DDRAM debug session.

Program DDRAM code to QSPI with the Loader Project

  1) Open GettingStarted workspace and build DDRAM-Debug configuration.
  
  2) Switch to the Loader project.
  
  3) Select Project->Download->Download active application
  