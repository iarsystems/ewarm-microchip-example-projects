/*******************************************************************************
 * (c) Copyright 2008 Actel Corporation.  All rights reserved.
 *
 * CoreAhbNvm driver example. This simple program demonstrates how to use the
 * CoreAhbNvm driver to write two messages at consecutive location within the
 * Fusion NVM.
 * This example is targeted at the design provided with the Cortex-M1 System 
 * Management Development Kit.
 * 
 * SVN $Revision: 5 $
 * SVN $Date: 2013-12-23 13:52:47 +0100 (mån, 23 dec 2013) $
 */
#include "hal.h"
#include "core_ahb_nvm.h"

/******************************************************************************
 * Base address of the Fusion NVM into which this prograsm will write.
 *****************************************************************************/ 
#define NVM_BASE_ADDR   0x40000000UL

/******************************************************************************
 * Messages to be written into Fusion NVM.
 *****************************************************************************/ 
uint8_t message_1[] = "Data written to Fusion NVM. ";

uint8_t message_2[] = "This message is longer than 128 bytes. Therefore it will\
 span more than one Fusion NVM page once written into Fusion non-volatile memory.";

/******************************************************************************
 * main function.
 *****************************************************************************/ 
int main( void )
{
    /**************************************************************************
     * Write the first message at the start of the NVM.
     *************************************************************************/
    NVM_write( NVM_BASE_ADDR, message_1, sizeof(message_1) );
    
    /**************************************************************************
     * Write the second message in the NVM location following the end of the
     * first message.
     *************************************************************************/
    NVM_write( NVM_BASE_ADDR + sizeof(message_1) , message_2, sizeof(message_2) );
    
    /**************************************************************************
     * Infinite loop/
     *************************************************************************/
    while ( 1 )
    {
        ;
    }
    return 0;
}
