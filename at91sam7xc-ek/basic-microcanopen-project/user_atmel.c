/**************************************************************************
MODULE:    USER
CONTAINS:  MicroCANopen Object Dictionary and Process Image implementation
COPYRIGHT: Embedded Systems Academy, Inc. 2002-2007.
           All rights reserved. www.microcanopen.com
           This software was written in accordance to the guidelines at
           www.esacademy.com/software/softwarestyleguide.pdf
DISCLAIM:  Read and understand our disclaimer before using this code!
           www.esacademy.com/disclaim.htm
LICENSE:   THIS IS THE EDUCATIONAL VERSION OF MICROCANOPEN
           See file license_educational.txt or
           www.microcanopen.com/license_educational.txt
           A commercial MicroCANopen license is available at
           www.CANopenStore.com
VERSION:   3.30, ESA 30-JAN-07
           $LastChangedDate: 2014-01-27 15:12:42 +0100 (m√•n, 27 jan 2014) $
           $LastChangedRevision: 10 $
***************************************************************************/ 

#include "mco.h"
#include "r_atmel_io.h"
#include "string.h"
#include "procimg.h"

// ensure the number of tpdos and rpdos is correct
#if (NR_OF_RPDOS != 2)
  #if (NR_OF_TPDOS != 2)
#error This example is for 2 TPDOs and 2 RPDOs only
  #endif
#endif

// global variables

// This structure holds all node specific configuration
UNSIGNED8 MEM_NEAR gProcImg[PIMGEND+1] = PIMGDEFAULTS;
//UNSIGNED8 gProcImg[PROCIMG_SIZE];

// Table with SDO Responses for read requests to OD
// Each Row has 8 Bytes:
// Command Specifier for SDO Response (1 byte)
//   bits 2+3 contain: '4' ñ {number of data bytes}
// Object Dictionary Index (2 bytes, low first)
// Object Dictionary Subindex (1 byte)
// Data (4 bytes, lowest bytes first)
UNSIGNED8 MEM_CONST SDOResponseTable[] = {

  // [1000h,00]: Device Type
  SDOREPLY(0x1000, 0x00, 4, OD_DEVICE_TYPE),

#ifdef OD_SERIAL
  // [1018h,00]: Identity Object, Number of Entries = 4
  SDOREPLY(0x1018, 0x00, 1, 0x00000004L),
#else
  // [1018h,00]: Identity Object, Number of Entries = 3
  SDOREPLY(0x1018, 0x00, 1, 0x00000003L),
#endif

  // [1018h,01]: Identity Object, Vendor ID
  SDOREPLY(0x1018, 0x01, 4, OD_VENDOR_ID),

  // [1018h,02]: Identity Object, Product Code
  SDOREPLY(0x1018, 0x02, 4, OD_PRODUCT_CODE),

  // [1018h,03]: Identity Object, Revision
  SDOREPLY(0x1018, 0x03, 4, OD_REVISION),

#ifdef OD_SERIAL
  // [1018h,04]: Identity Object, Serial
  SDOREPLY(0x1018, 0x04, 4, OD_SERIAL),
#endif

  // [2018h,00]: MicroCANopen Identity Object, Number of Entries = 3
  SDOREPLY(0x2018, 0x00, 1, 0x00000003L),

  // [2018h,01]: MicroCANopen Identity Object, Vendor ID = 01455341, ESA Inc.
  SDOREPLY(0x2018, 0x01, 4, 0x01455341L),

  // [2018h,02]: MicroCANopen Identity Object, Product Code = "MCOP"
  SDOREPLY4(0x2018, 0x02, 4, 'P', 'O', 'C', 'M'),

  // [2018h,03]: MicroCANopen Identity Object, Revision = 1.20
  SDOREPLY(0x2018, 0x03, 4, 0x00010020L),

#ifdef PDO_IN_OD
  // NOTE: These entries must be added manually. The parameters must match
  // the parameters used to call the functions MCO_InitRPDO and MCO_InitTPDO.

  // These entries are necessary to be fully CANopen compliant.
  // Suppported in commercial version of MicroCANopen available from
  // www.CANopenStore.com

  // Warning: This version is not fully CANopen compliant - PDO_IN_OD must not be defined
  #error Warning: This version of MicroCANopen has a limited Object Dictionary! Un-define PDO_IN_OD to confirm!
#endif // PDO_IN_OD

  // End-of-table marker
  SDOREPLY(0xFFFF, 0xFF, 0xFF, 0xFFFFFFFFL)
};

#ifdef PROCIMG_IN_OD
  // Table with Object Dictionary entries to process data.

  // These entries are necessary to be fully CANopen compliant.
  // Suppported in commercial version of MicroCANopen available from
  // www.CANopenStore.com

  // Warning: This version is not fully CANopen compliant - PROCIMG_IN_OD must not be defined
  #error Warning: This version of MicroCANopen has a limited Object Dictionary! Un-define PROCIMG_IN_OD to confirm!
#endif // PROCIMG_IN_OD


/**************************************************************************
DOES:    This function is called if a fatal error occurred.
         Error codes of mcohwxxx.c are in the range of 0x8000 to 0x87FF.
         Error codes of mco.c are in the range of 0x8800 to 0x8FFF.
         All other error codes may be used by the application.
RETURNS: nothing
**************************************************************************/
void MCOUSER_FatalError
  (
  UNSIGNED16 ErrCode  // the error code
  )
{
  //display blinking pattern on led
  error_state(ErrCode & 0xFF);
}

/**************************************************************************
DOES:    Call-back function for reset application.
         Starts the watchdog and waits until watchdog causes a reset.
RETURNS: nothing
**************************************************************************/
void MCOUSER_ResetApplication
  (
  void
  )
{
  // Watchdog reset not implemented in this simple example
  MCOUSER_ResetCommunication();
}

/**************************************************************************
DOES:    This function both resets and initializes both the CAN interface
         and the CANopen protocol stack. It is called from within the
         CANopen protocol stack, if a NMT master message was received that
         demanded "Reset Communication".
         This function should call MCO_Init and MCO_InitTPDO/MCO_InitRPDO.
RETURNS: nothing
**************************************************************************/
void MCOUSER_ResetCommunication
  (
  void
  )
{
  UNSIGNED8 i;

  // Initialize Process Variables
  for (i = 0; i < PROCIMG_SIZE; i++)
  {
    gProcImg[i] = 0;
  }

  // 1Mbit/s, Node 7, 2s heartbeat
  MCO_Init(1000,0x07,2000);

  // RPDO1, default ID (0x200+nodeID), 4 bytes
  MCO_InitRPDO(1,0,4,OUT_digi_1);

  // RPDO2, default ID (0x300+nodeID), 4 bytes
  MCO_InitRPDO(2,0,4,OUT_ana_1);

  // TPDO1, default ID (0x180+nodeID), 100ms event, 0ms inhibit, 4 bytes
  MCO_InitTPDO(1,0,100,0,4,IN_digi_1);

  // TPDO2, default ID (0x280+nodeID), 100ms event, 5ms inhibit, 4 bytes
  MCO_InitTPDO(2,0,100,5,4,IN_ana_1);
}

