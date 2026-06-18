
/*
 * $Revision: 10081 $
 */

// Define one of the following to one to select type of board.
#ifndef AT91_EB40A // Defined by the EB40A configuration.
#define AT91_EB40  0
#define AT91_EB40A 0     // Note that the linker configuration file (.xcl) must be
                         // changed if using the AT91EB40A. Open the .xcl file and search
                         // for AT91EB40A to get more details.
#define AT91_EB42  0
#define AT91_EB55  1
#define AT91_EB63  0
#endif

#ifndef FLASHCODE // Defined in AARM/ICCARM options tab (#define/preprocessor)
// To build Angel compatible application.
#define ANGEL 1
#endif // FLASHCODE

// To build with memory controller remap code.
#ifdef FLASHCODE // Defined in AARM/ICCARM options tab (#define/preprocessor)
#define AT91_REMAP 1
#endif // FLASHCODE

// Serial port speed is 38400 baud.
#define BAUD_RATE 38400

// Serial port receive buffer size.
#define RXBUF_SIZE 256

// cstartup build flags
//#define __THUMB_LIBRARY__ 1
#define __ARM_LIBRARY__ 1

#if __IAR_SYSTEMS_ICC__
#include <intrinsics.h>
#endif


//
// EB40 specifics.
//

#if AT91_EB40
#define AT91_EBXX  1
#define AT91_MCK 32768000
#endif

//
// EB40 specifics.
//

#if AT91_EB40A
#define AT91_EBXX  1
#define AT91_MCK 66000000
#endif

//
// EB40 and EB40A specifics.
//

#if AT91_EB40 || AT91_EB40A

#include <Microchip/ioat91x40.h>

// The code uses EB55 SFR names, map them to corresponding EB40 names.

// PIO
#define __PIO_PDSRB __PIO_PDSR
#define __PIO_CODRB __PIO_CODR
#define __PIO_SODRB __PIO_SODR
#define __PIO_PERB __PIO_PER
#define __PIO_PDRB __PIO_PDR
#define __PIO_OERB __PIO_OER
#define __PIO_ODRB __PIO_ODR
#define __PIO_IDRB __PIO_IDR

// Counter
#define __TC_SRC0 __TC_SR
#define __TC_IDRC0 __TC_IDR
#define __TC_CMRC0 __TC_CMR
#define __TC_RCC0 __TC_RC
#define __TC_CCRC0 __TC_CCR
#define __TC_IERC0_bit __TC_IER_bit
#endif // AT91_EB40 || AT91_EB40A


//
// AT91EB42 specifics.
//

#if AT91_EB42
#define AT91_EBXX  1
#define AT91_MCK 32000000

#include <Microchip/ioat91m42800.h>

// The code uses EB55 SFR names, map them to corresponding EB42 names.
#define __APMC_PCER __PMC_PCER
#define us0irq us0
#define tc0irq tc0
#define US0IRQ US0
#endif // AT91_EB55


//
// AT91EB55 specifics.
//

#if AT91_EB55
#define AT91_EBXX  1
#define AT91_MCK 32000000

#include <Microchip/ioat91m55800.h>
#endif // AT91_EB55


//
// AT91EB63 specifics.
//

#if AT91_EB63
#define AT91_EBXX  1
#define AT91_MCK 25000000

#include <Microchip/ioat91m63200.h>

// The code uses EB55 SFR names, map them to corresponding EB63 names.
#define __APMC_PCER __PMC_PCER
#endif // AT91_EB63

#if AT91_EBXX && __IAR_SYSTEMS_ICC__
#include "at91.h"
#endif


