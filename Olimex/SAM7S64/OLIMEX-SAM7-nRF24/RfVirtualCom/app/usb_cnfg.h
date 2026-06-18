/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2007
 *
 *    File name   : usb_cnfg.h
 *    Description : USB config file
 *
 *    History :
 *    1. Date        : June 16, 2007
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 10 $
 **************************************************************************/

#include <includes.h>

#ifndef __USB_CNFG_H
#define __USB_CNFG_H

/* USB High Speed support */
#define USB_HIGH_SPEED                  0

/* USB interrupt priority */
#define USB_INTR_PRIORITY               2
#define USB_CONN_DETECT_PRIORITY        2

/* USB Events */
#define USB_SOF_EVENT                   1

/* USB Clock settings */
#define USB_DIV         				        AT91C_CKGR_USBDIV_1

/* Device power atrb  */
#define USB_SELF_POWERED                0
#define USB_REMOTE_WAKEUP               0

/* Max Interfaces number*/
#define USB_MAX_INTERFACE               1

/* Endpoint definitions */
#define Ep0MaxSize                      8

#define CommOutEp                       ENP1_OUT
#define CommOutEpMaxSize                64

#define CommInEp                        ENP2_IN
#define CommInEpMaxSize                 64

#define ReportEp                        ENP3_IN
#define ReportEpMaxSize                 8

/* Class definitions*/
// CDC
#define CDC_DEVICE_SUPPORT_BREAK        1
#define CDC_BRK_TIMER_INTR_PRI          0

#define CDC_DATA_RATE                   CBR_9600
#define CDC_DATA_BITS                   8
#define CDC_PARITY                      NOPARITY
#define CDC_STOP_BITS                   ONESTOPBIT

#define CDC_LINE_DTR                    0
#define CDC_LINE_RTS                    0

/* Other definitions */

#endif //__USB_CNFG_H
