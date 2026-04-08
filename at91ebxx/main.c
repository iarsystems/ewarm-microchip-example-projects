
/*
 * $Revision: 10 $
 */

#include <stdio.h>
#include <string.h>
#include "config.h"
#include "timer.h"
#include "uart.h"
#include "button.h"

// LED moving pattern codes.
static int led_table[] = {
  0x18, 0x3c, 0x7e, 0xff, 0xe7, 0xc3, 0x81, 0x42, 0x24, -1
};

static char sRxBuf[RXBUF_SIZE];  // Receive buffer.

void main(void)
{
  int n; // Number of received characters.

  // Initialize UART module and register getchar/putchar callbacks.
  UartInit(AT91UartGetchar, AT91UartPutchar);
  // Initialize button module and register getbuttons callback.
  ButtonInit(AT91GetButtons);
  
  // Set up peripheral registers.
  // First disable interrupts.
  __disable_interrupt();
  // Setup interrupt controller.
  AT91InitInterrupt(TimerBeat, UartRxrdy);
  // Periodic timer initialization.
  AT91InitTimer();
  // Setup serial port.
  AT91UartInit();
  // Enable interrupts.
  __enable_interrupt();

  // Start periodic timer.
  AT91StartTimer();

  // Initial LED pattern.
  AT91LedSet(0x18);
  // Display pattern for 1 second.
  Sleep(1000);
  // Next LED pattern.
  AT91LedSet(0xc3);
  // Display pattern for 1 second.
  Sleep(1000);

  // Loop forever.
  for (;;)
  {
    int *ip;

    // Loop over all LED patterns in table.
    for (ip = led_table; *ip != -1; ip++)
    {
      // Update LED's.
      AT91LedSet(*ip);
      // Check serial port, timeout in milleseconds is second parameter.
      n = ReceiveLine(sRxBuf, led_slow_speed ? 200 : 75);
      // Repeat while characters are available.
      while (n)
      {
        char str[64];
        // Echo received character in TX message.
        sprintf(str, "IAR Embedded Workbench for ARM, got char: %c\r\n", sRxBuf[0]);
        SendLine(str);
        // Show ASCII code on LED's.
        AT91LedSet(sRxBuf[0]);
        // Check for new character.
        n = ReceiveLine(sRxBuf, 2000);
      }
    }
    // Periodic TX message.
    SendLine("IAR Embedded Workbench for ARM, moving LED pattern\r\n");
  }
}
