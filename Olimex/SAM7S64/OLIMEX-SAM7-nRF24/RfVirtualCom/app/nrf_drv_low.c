/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2008
 *
 *    File name   : nrf_drv_low.c
 *    Description : nRF24L01 low level driver
 *
 *    History :
 *    1. Date        : April 17 2008
 *       Author      : Stanimir Bonev
 *       Description : Initial Revision
 *    $Revision: 10 $
 **************************************************************************/
#define RNF_DRV_LOW_GLOBAL

#include "nrf_drv_low.h"

/*************************************************************************
 * Function Name: SpiNrfSendData
 * Parameters: Byte to send.
 *
 * Return:  The value of the received Byte
 *
 * Description: Sends and receives a Byte from the SPI1.
 *
 *************************************************************************/
BYTE SpiNrfSendData (BYTE Byte)
{
	AT91C_BASE_SPI->SPI_TDR = Byte;
	while(!(AT91C_BASE_SPI->SPI_SR & AT91C_SPI_RDRF));
	return(	AT91C_BASE_SPI->SPI_RDR);
}

/*************************************************************************
 * Function Name: nrf24l_intr_handler
 * Parameters: none
 *
 * Return: none
 *
 * Description:
 *
 *************************************************************************/
static
void nrf24l_intr_handler ( void )
{
Int32U Save;
  ENTR_CRT_SECTION(Save);
  RF_IRQ_Handler();
  EXT_CRT_SECTION(Save);
}

/*************************************************************************
 * Function Name: nrf24l_init
 * Parameters: none
 *
 * Return: none
 *
 * Description:
 *
 *************************************************************************/
void nrf24l_init(void)
{
  // Assign pins
	// Configure PIO controllers to periph mode
	AT91F_PIO_CfgPeriph(
		AT91C_BASE_PIOA, // PIO controller base address		
		((unsigned int) AT91C_PA13_MOSI    ) |
		((unsigned int) AT91C_PA12_MISO    ) |
		((unsigned int) AT91C_PA14_SPCK    ), // Peripheral A
		0                                  ); // Peripheral B
  AT91F_PIO_CfgOutput( AT91C_BASE_PIOA,
                       NRF24L_CE_MASK
                     | NRF24L_CS_MASK);
  // nRF chip disable
  RF24L_CE_L();
  // CS High
  AT91C_BASE_PIOA->PIO_SODR = NRF24L_CS_MASK;

  // Enable internal pull-up
	AT91C_BASE_PIOA->PIO_PPUER = NRF24L_IRQ_MASK;
	AT91F_PIO_CfgPeriph(
		AT91C_BASE_PIOA, // PIO controller base address		
		((unsigned int) NRF24L_IRQ_MASK    ), // Peripheral A
		0                                  ); // Peripheral B

  // nRF24L01 interrupt init
  AT91F_AIC_ConfigureIt(AT91C_BASE_AIC,
                        AT91C_ID_IRQ1,
                        NRF24L_INTR_PRIORITY,
                        AT91C_AIC_SRCTYPE_EXT_LOW_LEVEL,
                        nrf24l_intr_handler);

	// init SPI
  AT91F_SPI_CfgPMC();
  AT91F_SPI_Disable(AT91C_BASE_SPI);
  AT91F_SPI_Reset(AT91C_BASE_SPI);
  AT91F_SPI_CfgMode( AT91C_BASE_SPI,
                     AT91C_SPI_MSTR
                   | AT91C_SPI_PS_FIXED
                   | AT91C_SPI_MODFDIS
                   | (0xDUL<<16));
  AT91F_SPI_CfgCs( AT91C_BASE_SPI,1,
                   AT91C_SPI_NCPHA
                 | AT91C_SPI_BITS_8
                 | (((AT91C_MASTER_CLOCK /(4UL MHZ)) & 0xFF)<<8));

  AT91F_SPI_Enable(AT91C_BASE_SPI);


  // Enable interrupt
  AT91F_AIC_EnableIt(AT91C_BASE_AIC,AT91C_ID_IRQ1);
  AT91F_PITEnableInt(AT91C_BASE_PITC);

  // Init TX Buffer - buffer number depend of platform

  AT91F_PIO_CfgOutput( AT91C_BASE_PIOA,
                       1<<26);
  AT91C_BASE_PIOA->PIO_CODR = 1<<26;


}

/*************************************************************************
 * Function Name: nrf24l_ClrPendingIntr
 * Parameters: none
 *
 * Return: none
 *
 * Description:
 *
 *************************************************************************/
void nrf24l_ClrPendingIntr(void)
{
}
