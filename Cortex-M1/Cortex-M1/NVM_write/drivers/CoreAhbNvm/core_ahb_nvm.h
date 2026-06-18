/*******************************************************************************
 * (c) Copyright 2008 Actel Corporation.  All rights reserved.
 *
 * CoreAhbNvm bare metal driver public application programming interface (API).
 *
 * SVN $Revision: 5 $
 * SVN $Date: 2013-12-23 13:52:47 +0100 (mån, 23 dec 2013) $
 */
#ifndef CORE_AHB_NVM_H_
#define CORE_AHB_NVM_H_

#include "hal.h"

/***************************************************************************//**
 * Possible return values from function NVM_write().
 */
typedef enum {
    SUCCESS = 0,
    PROTECTION_ERROR,
    WRITE_ERROR
} nvm_status_t;

/***************************************************************************//**
 * This function writes the content of the buffer passed as parameter to Fusion
 * NVM. The data is written from the memory location specified by the first
 * parameter. This address is the absolute address in the processor's memory
 * space at which the NVM is located.
 *
 * @param start_addr    This is the address at which data willbe written.
 *                      This address is the absolute address in the processor's
 *                      memory space at which the NVM is located.
 * @param p_data        This is a pointer to the buffer holding the data to be
 *                      written into NVM.
 * @param nb_bytes      This is the number of bytes to be written into NVM.
 * @return              The return value indicates if the write was successful.
 */
nvm_status_t
NVM_write
(
 	addr_t start_addr,
 	uint8_t * p_data,
 	size_t nb_bytes
);

#endif /*CORE_AHB_NVM_H_*/

