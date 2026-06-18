/*
 * Copyright (c) 1996 - 2025 IAR Systems AB.
 *
 * IAR Embedded Workbench tutorial
 *
 * main.c
 *
 * $Revision: $
 */

#include <stdint.h>
#include <intrinsics.h>
#include <Microchip/iosam9x60.h>

void _delay(uint32_t dly)
{
  while(dly--);
}

/* Main program. */
int32_t main(void)
{

  PIOD_REGS->PIO_OER = 0x2A0000;

  while (1)
  {
    PIOD_REGS->PIO_SODR = 0x80000;
    _delay(5000000);
    PIOD_REGS->PIO_CODR = 0x80000;
    _delay(5000000);
  }
}
