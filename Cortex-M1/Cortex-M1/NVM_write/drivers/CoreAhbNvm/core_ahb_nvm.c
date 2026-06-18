/*******************************************************************************
 * (c) Copyright 2008 Actel Corporation.  All rights reserved.
 *
 * CoreAhbNvm bare metal driver implementation.
 *
 * SVN $Revision: 5 $
 * SVN $Date: 2013-12-23 13:52:47 +0100 (mån, 23 dec 2013) $
 */
#include "core_ahb_nvm.h"
#include "hal.h"

/***************************************************************************//**
 * CoreAhbNvm commands.
 */
#define READ_ARRAY_CMD		0xFF
#define READ_STATUS_CMD		0x70
#define CLEAR_STATUS_CMD	0x50
#define ERASE_PAGE_CMD		0x20
#define SINGLE_WRITE_CMD	0x40
#define MULTI_WRITE_CMD		0xE8
#define CONFIRM_CMD			0xD0

/***************************************************************************//**
 * CoreAhbNvm status register bits.
 */
#define READY_BIT_MASK			0x80
#define READ_ERROR_BIT_MASK		0x02
#define WRITE_ERROR_BIT_MASK	0x10

/***************************************************************************//**
 * NVM page size in bytes.
 */
#define PAGE_SIZE	128

/***************************************************************************//**
 * Local functions:
 */
static nvm_status_t
write_page_fragment
(
 	addr_t start_address,
 	uint8_t * p_data,
 	size_t nb_bytes
);

/***************************************************************************//**
 * NVM_write()
 * See "core_ahb_nvm.h" for details of how to use this function.
 */
nvm_status_t
NVM_write
(
 	addr_t start_address,
 	uint8_t * p_data,
 	size_t nb_bytes
)
{
	size_t block_address;
	size_t size_written;
    nvm_status_t status = SUCCESS;
      
	size_written = 0;
	block_address = start_address;
	while ( (size_written < nb_bytes) && (status == SUCCESS) )
	{
		size_t block_size;
		addr_t page_start;
		size_t size_left;
		block_address = start_address + size_written;
		page_start = block_address & ~(PAGE_SIZE - 1);
		block_size = PAGE_SIZE - ( block_address - page_start );
		size_left = nb_bytes - size_written;
		if ( block_size > size_left )
		{
			block_size = size_left;
		}
		
		status = write_page_fragment( block_address, &p_data[size_written], block_size );
		size_written += block_size;
	}
    
    return status;
}

/***************************************************************************//**
 * This function writes part or all of the content of a single NVM page.
 */
nvm_status_t
write_page_fragment
(
 	addr_t start_address,
 	uint8_t * p_data,
 	size_t nb_bytes
)
{
	addr_t page_address;
	uint8_t status;
    uint8_t ready;
	size_t nb_bytes_written;
    nvm_status_t ret_value = SUCCESS;
	
	page_address = start_address & ~(PAGE_SIZE - 1);
	
	/* Write the command "Setup  Write Buffer" to the page address. */
	HW_set_8bit_reg( page_address, MULTI_WRITE_CMD );
	
	/* Wait for status ready bit to go high. */
    do {
        uint8_t read_error;
	    status = HW_get_32bit_reg( page_address);
        read_error = status & READ_ERROR_BIT_MASK;
        ready = status & READY_BIT_MASK;
		
		if ( read_error )
		{
			/* Protection error. */
			ret_value = PROTECTION_ERROR;
		}
	} while ( ( !ready ) && ( ret_value == SUCCESS ) );
	
    if  ( ret_value == SUCCESS )
    {
        /* Write the number of element count. */
        HW_set_8bit_reg( page_address, nb_bytes - 1 );
        
        /* Write elements. */
        for ( nb_bytes_written = 0; nb_bytes_written < nb_bytes; nb_bytes_written++ )
        {
            HW_set_8bit_reg( start_address + nb_bytes_written, (uint_fast8_t)p_data[nb_bytes_written] );
        }
        
        /* Inititate page write. */
        HW_set_8bit_reg( page_address, CONFIRM_CMD );
	
        /* Wait for page write to complete. */
        do {
            status = HW_get_8bit_reg( page_address );
        } while ( ( status & READY_BIT_MASK ) == 0 );
        
        if ( ( status & WRITE_ERROR_BIT_MASK ) != 0 )
        {
            /* Write failed. */
            ret_value = WRITE_ERROR;
        }	
    }
	
	/* Make array readable. */
	HW_set_8bit_reg( page_address, READ_ARRAY_CMD );
    
	return ret_value;
}
