
/*
 * $Revision: 10 $
 */

#include "config.h"

static void(*timer_function)();
static void(*rxrdy_function)();


//
// Clock initialization.
//

#if AT91_EB42
void AT91_EB42_PllStart()
{
#define PMC_PLL_LOCK   1     // PLL lock status

  // Speed up the System Frequency to 32 MHz.

  // Stay at PLL B if already set up.
  if ((__PMC_CGMR & 0x80) == 0)
  {
    // Run from 32 kHz until PLL is stabilized.
    // PLLCOUNT = 197, MUL = 976, CSS = 0, MCKODS = 0, MCKOSS = 1, PLLS = 1, PRES = 0
    __PMC_CGMR = 0xc503d008;

    // Reading the PMC Status register to detect when the PLL is stabilized
    while ((__PMC_SR & PMC_PLL_LOCK) != PMC_PLL_LOCK) ;
  }

  // Commuting from 32 kHz (or other PLL speed) to PLL @ 32 MHz
  // PLLCOUNT = 197, MUL = 976, CSS = 1, MCKODS = 0, MCKOSS = 1, PLLS = 1, PRES = 0
  __PMC_CGMR = 0xc503d098;

  // Reading the PMC Status register to detect when the PLL is stabilized
  while ((__PMC_SR & PMC_PLL_LOCK) != PMC_PLL_LOCK) ;

  // Now the Master clock is the output of PLL @ 32MHz
}
#endif

#if AT91_EB55
void AT91_EB55_PllStart()
{
#define APMC_MOSCS      1     // Main oscillator status bit
#define APMC_PLL_LOCK   2     // PLL lock status

  int i;

  // Speed up the System Frequency.

  __APMC_CGMR = 0x002F0002; // MOSCEN = 1, OSCOUNT = 47  (1.4ms/30µs)

  // Reading the APMC Status register to detect when the oscillator is stabilized
  while ((__APMC_SR & APMC_MOSCS) != APMC_MOSCS) ;

  // Commuting from Slow Clock to Main Oscillator (16Mhz)
  __APMC_CGMR = 0x002F4002; // MOSCEN = 1, OSCOUNT = 47  (1.4ms/30µs)

  // Setup the PLL to 32 MHz clock
  __APMC_CGMR = 0x032F4102; // MUL = 1, PLLCOUNT = 3, CSS = 1

  // Reading the APMC Status register to detect when the PLL is stabilized
  // Wait a little extra to ensure stable PLL.
  for (i = 0; i < 1000; i++)
  {
    while ((__APMC_SR & APMC_PLL_LOCK) != APMC_PLL_LOCK) ;
  }

  // Commuting from 16Mhz to PLL @ 32MHz
  __APMC_CGMR = 0x032F8102; // CSS = 2, MUL = 1

  // Now the Master clock is the output of PLL @ 32MHz
}
#endif

void AT91EnablePeripheralClocks()
{
#if AT91_EB40 || AT91_EB40A
  // Switch on clocks to all peripherals.
  __PS_PCER = 0x00017c;
#else
  // Switch on clocks to all peripherals.
  __APMC_PCER = 0x0007effc;
#endif
}


//
// Interrupt handlers.
//

 /* Timer interrupt handler */
__irq __arm void heartbeat_irq(void)
{
  // Called at 1000 Hz rate.
  __AIC_IVR = 0; // Debug variant of vector read, protected mode is used.

  (*timer_function)(); // Call timer callback function.

  __TC_SRC0; // Read timer/counter 0 status register.
  __AIC_EOICR = 0; // Signal end of interrupt to AIC.
}


/* Serial port RX interrupt handler */
__irq __arm void usart0_rxrdy_interrupt(void)
{
  __AIC_IVR = 0; // Debug variant of vector read, protected mode is used.

  (*rxrdy_function)(); // Call RX callback function.

  __AIC_EOICR = 0; // Signal end of interrupt to AIC.
}


/* Undefined interrupt handler */
__irq __arm void undefined_irq(void)
{
  __AIC_IVR = 0; // Debug variant of vector read, protected mode is used.

  // Do nothing.

  __AIC_EOICR = 0; // Signal end of interrupt to AIC.
}


//
// Interrupt functions.
//

void AT91InitInterrupt(void(*timer_func)(), void(*rxrdy_func)())
{
#if !ANGEL
  int      irq_id ;
#endif // ANGEL

  timer_function = timer_func;
  rxrdy_function = rxrdy_func;

#if !ANGEL
  // Disable all interrupts.
  __AIC_IDCR = 0xFFFFFFFF;
  // Clear all interrupts.
  __AIC_ICCR = 0xFFFFFFFF;

  // For each priority level.
  for (irq_id = 0; irq_id < 8; irq_id++)
  {
    // Unstack a level by writting in AIC_EOICR.
    // Value written has no effect.
    __AIC_EOICR = 0;
  }

  {
    // For each interrupt source.
    __REG32 volatile* aic_smr_base = &__AIC_SMR0;
    __REG32 volatile* aic_svr_base = &__AIC_SVR0;
    for (irq_id = 0; irq_id < 32; irq_id++)
    {
      // Priority is lowest.
      aic_smr_base[irq_id] = 0 ;
      // Interrupt routine is undefined.
      aic_svr_base[irq_id] = (unsigned long)&undefined_irq;
    }
  }

  __AIC_SPU = (unsigned long)&undefined_irq; // Spurious interrupt vector.
  __SF_PMR = 0x27a80020; // Run AIC in protected mode.
  __SF_PMR = 0x27a80020; // Run AIC in protected mode. EB40A requires this twice.
  // Initialize ARM IRQ vector to map to interrupt controller.
  *(unsigned long *)0x18 = 0xe51fff20; // ldr pc,[pc,#-0xf20]
#endif // ANGEL

#if ANGEL
  // The Angel ROM monitor code is written to allow AIC
  // protected mode. That means that from tis point it is
  // safe to look at the AIC registers from the memory and
  // register windows. Take care not to start up C-SPY with
  // AIC display active since the ROM monitor itself does not
  // Enable protected mode.
  __SF_PMR = 0x27a80020; // Run AIC in protected mode.
#endif // ANGEL
}


//
// Timer functions.
//

void AT91InitTimer()
{
  __TC_IDRC0 = 0xff; // Disable all timer/counter 0 interrupts.
#if AT91_EB40 || AT91_EB40A
  __AIC_SVR4 = (unsigned long)&heartbeat_irq;
  __AIC_SMR4 = 0x26;
#else
  __AIC_SVR6 = (unsigned long)&heartbeat_irq; // Timer/counter 0 interrupt vector.
  __AIC_SMR6 = 0x26; // SRCTYPE=1, PRIOR=6. Timer/counter 0 interrupt edge-triggered at prio 6.
#endif
  __AIC_ICCR_bit.tc0irq = 1; // Clears timer/counter 0 interrupt.
  __AIC_IECR_bit.tc0irq = 1; // Enable timer/counter 0 interrupt.

  __TC_CMRC0 = 0x00004002; // Capture mode, CPCTRG=1, TCCLKS=2 (/32).
  __TC_RCC0 = AT91_MCK / 32 / 1000; // Set RC (compare register), 1 ms interval.
  __TC_CCRC0 = 1; // Enable the clock.
  __TC_CCRC0 = 5; // Software trigger.
  __TC_CCRC0 = 1; // Clear trigger.
}

void AT91StartTimer()
{
#if AT91_EB40 || AT91_EB40A
  __AIC_ICCR = 0x10; // Clears timer/counter 0 interrupt.
#else
  __AIC_ICCR = 0x40; // Clears timer/counter 0 interrupt.
#endif
  __TC_SRC0; // Read timer/counter 0 status register to clear flags.
  __TC_IERC0_bit.cpcs = 1; // Interrupt on RC compare.
}


//
// Serial communication functions.
//

void AT91UartInit()
{
// Don't use the serial port when run in Angel mode.
#if !ANGEL
#if AT91_EB40 || AT91_EB40A
  __PIO_PDR = 0x000c000; // Disable PIO control of P14/TXD and P15/RXD.
#endif
#if AT91_EB42
  __PIO_PDR = 0x000000c0; // Disable PIO control of PA6/TXD0 and PA7/RXD0.
#endif
#if AT91_EB55 || AT91_EB63
  __PIO_PDR = 0x00018000; // Disable PIO control of PA15/TXD0 and PA16/RXD0.
#endif

  __US_MR = 0x000008c0; // Normal mode, 1 stop bit, no parity, async mode, 8 bits, MCK.
  __US_IDR = 0xffffffff; // Disable all USART interrupts.
  __US_IER = 1; // Interrupt on RXRDY.
  __US_TTGR = 5; // Transmit time guard in number of bit periods.
  __US_BRGR = AT91_MCK / BAUD_RATE / 16; // Set baud rate.

  __AIC_SVR2 = (unsigned long)&usart0_rxrdy_interrupt; // Usart 0 interrupt vector.
  __AIC_SMR2 = 0x63; // SRCTYPE=1, PRIOR=3. USART 0 interrupt positive edge-triggered at prio 3.
  __AIC_ICCR_bit.us0irq = 1; // Clears timer/counter 0 interrupt.
  __AIC_IECR_bit.us0irq = 1; // Enable timer/counter 0 interrupt.

  __AIC_ICCR = 1 << US0IRQ; // Clears usart 0 interrupt.

  __US_CR = 0x000000a0; // Disable receiver, disable transmitter.
  __US_CR = 0x0000010c; // Reset status bits, reset rx/tx.
  __US_CR = 0x00000050; // Enable receiver, enable transmitter.
#endif // ANGEL
}


int AT91UartGetchar()
{
// Don't use the serial port when run in Angel mode.
#if ANGEL
  return '?';
#else
  return __US_RHR;
#endif // ANGEL
}


void AT91UartPutchar(int ch)
{
// Don't use the serial port when run in Angel mode.
#if !ANGEL
  unsigned char status;

  do {
    status = __US_CSR;
  } while ((status & 0x02) == 0); // Wait for TXRDY

  __US_THR = ch;
#endif // ANGEL
}


//
// Parallel I/O functions.
//

unsigned int AT91GetButtons()
{
  unsigned long pa; // Port A value.
  unsigned long pb; // Port B value.
  unsigned int sw1; // SW1 button.
  unsigned int sw2; // SW2 button.
  unsigned int sw3; // SW3 button.
  unsigned int sw4; // SW4 button.
  unsigned int mask; // Bit mask of buttons.

#if AT91_EB40
  // SW3 - P12, SW4 - P5, SW6 - P9
  pa = pb = ~__PIO_PDSRB; // Switch on P5, P9, P12
  sw2 = !!(pb & 0x00000020); // p5
  sw3 = !!(pb & 0x00000200); // p9
  sw1 = !!(pa & 0x00001000); // p12
  sw4 = 1;
#endif

#if AT91_EB40A
  // SW1 - P12, SW2 - P9, SW3 - P1, SW4 - P2
  pa = pb = ~__PIO_PDSRB; // Switch on P5, P9, P12
  sw3 = !!(pa & 0x00000002); // p1
  sw4 = !!(pb & 0x00000004); // p2
  sw2 = !!(pb & 0x00000200); // p9
  sw1 = !!(pa & 0x00001000); // p12
#endif

#if AT91_EB42
  // As seen from card edge: SW4 - PB21, SW3 - PB6, SW2 - PA0, SW1 - PB7
  pa = ~__PIO_PDSR; // Switch on PA0
  pb = ~__PIO_PDSRB; // Switch on PB6, PB7, PB21
  sw2  = !!(pa & 0x00000001); // pa0
  sw3 = !!(pb & 0x00000040); // pb6
  sw1 = !!(pb & 0x00000080); // pb7
  sw4 = !!(pb & 0x00200000); // pb21
#endif

#if AT91_EB55
  // As seen from card edge: SW4 - PB19, SW3 - PB17, SW2 - PA9, SW1 - PB20
  pa = ~__PIO_PDSR; // Switch on PA9
  pb = ~__PIO_PDSRB; // Switch on PB17, PB19, PB20
  sw2  = !!(pa & 0x00000200); // pa9
  sw3 = !!(pb & 0x00020000); // pb17
  sw4 = !!(pb & 0x00080000); // pb19
  sw1 = !!(pb & 0x00100000); // pb20
#endif

#if AT91_EB63
  // As seen from card edge: SW4 - PA9, SW3 - PB5, SW2 - PB4, SW1 - PB3
  pa = ~__PIO_PDSR; // Switch on PA9
  pb = ~__PIO_PDSRB; // Switch on PB3, PB4, PB5
  sw4 = !!(pa & 0x00000200); // pa9
  sw1 = !!(pb & 0x00000008); // pb3
  sw2 = !!(pb & 0x00000010); // pb4
  sw3 = !!(pb & 0x00000020); // pb5
#endif

  mask = (sw1 << 3) | (sw2 << 2) | (sw3 << 1) | sw4;

  return mask;
}


void AT91InitPIO()
{
#if ANGEL
// In Angel mode the serial port is already initialized and
// used by the Angel monitor.

  // Initialize port B.
#if AT91_EB40
  __PIO_PERB = 0x1236; // enable register
  __PIO_PDRB = 0x0000; // disable register
  __PIO_OERB = 0x0016; // output enable
  __PIO_ODRB = 0x1220; // output disable
  __PIO_SODRB = 0x0016; // LED's off
  __PIO_IDRB = 0x1220;
#elif AT91_EB40A
  __PIO_PERB = 0xf127e; // enable register
  __PIO_PDRB = 0x00000; // disable register
  __PIO_OERB = 0xf0078; // output enable
  __PIO_ODRB = 0x01206; // output disable
  __PIO_SODRB = 0xf0078; // LED's off
  __PIO_IDRB = 0x1206;
#else
  __PIO_PERB = 0xff00; // enable register
  __PIO_OERB = 0xff00; // output enable
  __PIO_SODRB = 0xff00; // LED's off
#endif

#else // ANGEL

  // Initialize port A.
#if AT91_EB42
  __PIO_PER = 0x0040; // enable register (uart TX bit)
  __PIO_OER = 0x0040; // output enable (uart TX bit)
  __PIO_SODR = 0x0040; // inactive TX line
#endif

#if AT91_EB55 || AT91_EB63
  __PIO_PER = 0x8000; // enable register (uart TX bit)
  __PIO_OER = 0x8000; // output enable (uart TX bit)
  __PIO_SODR = 0x8000; // inactive TX line
#endif

  // Initialize port B.
#if AT91_EB40
  __PIO_PERB = 0x5236; // enable register
  __PIO_PDRB = 0x8000; // disable register
  __PIO_OERB = 0x4016; // output enable
  __PIO_ODRB = 0x1220; // output disable
  __PIO_SODRB = 0x4016; // LED's off
  __PIO_IDRB = 0x1220;
#elif AT91_EB40A
  __PIO_PERB = 0xf527e; // enable register
  __PIO_PDRB = 0x08000; // disable register
  __PIO_OERB = 0xf4078; // output enable
  __PIO_ODRB = 0x01206; // output disable
  __PIO_SODRB = 0xf4078; // LED's off
  __PIO_IDRB = 0x1206;
#else
  __PIO_PERB = 0xff00; // enable register
  __PIO_OERB = 0xff00; // output enable
  __PIO_SODRB = 0xff00; // LED's off
#endif

#endif // ANGEL
}


//
// LED output drivers.
//

static void led_on(unsigned int mask)
{
#if AT91_EB40
  __PIO_CODRB = mask & 0x16;
#elif AT91_EB40A
  __PIO_CODRB = (mask & 0x0f) << 16 | (mask & 0xf0) >> 1;
#else
  __PIO_CODRB = mask << 8;
#endif
}

static void led_off(unsigned int mask)
{
#if AT91_EB40
  __PIO_SODRB = mask & 0x16;
#elif AT91_EB40A
  __PIO_SODRB = (mask & 0x0f) << 16 | (mask & 0xf0) >> 1;
#else
  __PIO_SODRB = mask << 8;
#endif
}

void AT91LedSet(unsigned int mask)
{
  led_off(0xff); // Switch off all LED's.
  led_on(mask);  // Switch on selected LED's.
}
