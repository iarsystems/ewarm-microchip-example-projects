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
#include <Microchip/iosam9x7.h>

void _delay(uint32_t dly)
{
  while(dly--);
}

/* Main program. */
int32_t main(void)
{

  PIOC_REGS->PIO_OER = 0x304000;

  while (1)
  {
    PIOC_REGS->PIO_SODR = 0x200000;
    _delay(5000000);
    PIOC_REGS->PIO_CODR = 0x200000;
    _delay(5000000);
  }
}
