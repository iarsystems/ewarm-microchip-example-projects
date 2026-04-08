/***************************************************************************
 **
 **    This file defines the board specific definition
 **
 **    Used with ARM IAR C/C++ Compiler and Assembler.
 **
 **    (c) Copyright IAR Systems 2008
 **
 **    $Revision: 10 $
 **
 ***************************************************************************/
#include "arm_comm.h"

#ifndef __BOARD_H
#define __BOARD_H

/* OSC [MHz] */
#define FOSC                18432000UL
#define PLL_DIV             14
#define PLL_MUL             (73-1)

/* Core clk */
#define AT91C_PLL_CLOCK     (FOSC*(PLL_MUL+1)/PLL_DIV)
#define AT91C_MASTER_CLOCK  (AT91C_PLL_CLOCK/2)

#if AT91C_PLL_CLOCK > 150
#define PLL_OUT             AT91C_CKGR_OUT_1
#else
#define PLL_OUT             AT91C_CKGR_OUT_0
#endif

#define USB_D_MASK          (AT91C_PIO_PA24)
#define USB_PUP_MASK        (AT91C_PIO_PA16)
#define USB_PUP_POLARITY    1
#define USB_PUN_MASK        (AT91C_PIO_PA8)
#define USB_PUN_POLARITY    1

#define LED_MASK            (AT91C_PIO_PA18)
#define LED_STATE()         (!(AT91C_BASE_PIOA->PIO_PDSR & LED_MASK))
#define LED_ON()            (AT91C_BASE_PIOA->PIO_CODR = LED_MASK)
#define LED_OFF()           (AT91C_BASE_PIOA->PIO_SODR = LED_MASK)
#define LED_TOGGLE() \
{\
  if(LED_STATE())\
  {\
    LED_OFF();\
  }\
  else\
  {\
    LED_ON();\
  }\
}

#define NRF24L_IRQ_MASK     (AT91C_PIO_PA30)
#define NRF24L_CE_MASK      (AT91C_PIO_PA29)

#define NRF24L_CS_MASK      (AT91C_PIO_PA31)

#endif /* __BOARD_H */
