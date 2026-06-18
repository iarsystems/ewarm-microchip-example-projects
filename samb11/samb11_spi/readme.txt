SAMB11 BLE debugging instructions
Oscar Molin, IAR Systems 2016
---------------------------------

Revised April 21, 2016

This project collects a BLE firmware, ROM patches and a user application
and builds a single image file. The image can be flashed to the SPI flash
using the SPI flashloader. This flash is not directly accessible so
usual debugger-side verification of the flashing is not possible.
After loading and hardware reset, the bootloader will execute and
load code from the SPI flash into RAM and the jump to its entry point.
The entry of the user application is exported from the application project 
along with the stack position and size.
Therefore stack and application placement can be set by simply editing 
the application project.

Debug symbols for the user application are loaded from the Application project, 
as the SPI image itself lacks debug information.

Use the Application project to edit the application, and use
the SPI image creator to download and debug the user application.
Both projects can also be used to attach to an already running
application project, as the application debug symbols are 
loaded in both projects.

A special hardware reset script is used to perform resets, as register
needs to be changed in order for the bootloader to load 
the application and firmware into RAM when it detects that
a debug probe is attached.

For information about the BLE OS and the bootloader refer to Microchip documentation.


When the user application returns from main(), the BLE OS will resume control.

Getting started:

1. Build Application project. This generates an elf file, a binary file and 
an elf object file containing the application entry and CSTACK information exported using isymexport.
2. Build SPI image creator. This takes the bin file from 1), the symbol object file,
a FW image and ROM patches and makes an elf file. Ielftools patches the elf files with CRC
checksums, zero fills and then strips debug info.
3. Download and debug using SPI image creator project. 
Use the custom hardware reset script.
This downloads the image to SPI flash.
4. When the debugger starts, the debug symbols from the elf in step 1 are loaded.
