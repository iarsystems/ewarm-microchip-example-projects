// A startup file without a vector table.
// BLE OS has already set up the process stack for us to use.
// If this image is debugged alone in RAM, 
// the process stack needs to be set up by the debugger.
// If the program collides with the process stack, you can move the program entry.
// Reconfiguring the stack placement may be possible using ROM patches, 
// see how to add these in flash_header.s.

extern int PROC_STACK$$Length;

void __cmain( void );
__weak void __iar_init_core( void );
__weak void __iar_init_vfp( void );

#pragma required=PROC_STACK$$Length

void __iar_program_start( void )
{
  __iar_init_core();
  __iar_init_vfp();
  __cmain();
  
  // Return to OS control
}
