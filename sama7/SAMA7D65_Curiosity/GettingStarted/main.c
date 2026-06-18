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
#include <Microchip/iosama7d65.h>

extern uint32_t __vector[];

int __low_level_init(void)
{
  uint32_t sctlr;

  // Set VBAR address
  __MCR(15,0,(uint32_t)__vector,12,0,0);

  // IC enable
  sctlr = __MRC(15,0,1,0,0);
  __MCR(15,0,sctlr | SCTLR_I_Msk,1,0,0);
  
  return 1;
}

void _delay(uint32_t dly)
{
  while(dly--);
}

/* Main program. */
int32_t main(void)
{

  PIO_REGS->PIO_GROUP[1].PIO_MSKR = 0x28000;

  PIO_REGS->PIO_GROUP[1].PIO_CFGR =  0x100;

  while (1)
  {
    PIO_REGS->PIO_GROUP[1].PIO_SODR = 0x8000;
    _delay(5000000);
    PIO_REGS->PIO_GROUP[1].PIO_CODR = 0x8000;
    _delay(5000000);
  }
}
