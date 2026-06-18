/*-----------------------------------------------------------------------------
*   EUROPE TECHNOLOGIES Software Support
*------------------------------------------------------------------------------
* The software is delivered "AS IS" without warranty or condition of any
* kind, either express, implied or statutory. This includes without
* limitation any warranty or condition with respect to merchantability or
* fitness for any particular purpose, or against the infringements of
* intellectual property rights of others.
*------------------------------------------------------------------------------
*
* File Name       : csp_spi.c
* Description     : Function declarations for Serial Peripheral Interface 
                    management
* Library Version : 2.00
* Module Version  : 1.XX
*
*       +----- (NEW | MODify | ADD | DELete)                                 
*       |                                                                    
*  No   |   When       Who                What               
*-----+---+----------+------------------+--------------------------------------
* 000  NEW  01/05/99   Patrice VILCHEZ    Creation
* 001  MOD  01/04/01   Olivier MAZUYER    Clean up
* 002  MOD  08/06/01   Frederic SAMSON    Clean Up 
* 003  MOD  19/11/01   Christophe GARDIN  Clean Up
* 004  MOD  11/04/02   Christophe GARDIN  Add CSP_SPIClose function
* 005  MOD  13/05/02   Christophe GARDIN  Modify CSP_SPIReceive function
*----------------------------------------------------------------------------*/

/******************************************************************************
* Include Files
******************************************************************************/
#include "csp.h"


/******************************************************************************
****************************  F U N C T I O N S  ******************************
******************************************************************************/

/******************************************************************************
* Function          : CSP_SPIInit
* Description       : Switch on the clock, reset the registers and configure 
*                     the SPI module mode
* Inputs            : <*spi> = Pointer to SPI structure
*                     <mode> = Configure the SPI mode
* Functions called  : None
* Returns           : None
******************************************************************************/
void CSP_SPIInit(CSP_SPI_T *const spi, U32_T mode)
{
   /* Enable SPI Clock */
   CSP_SPI_SET_ECR(spi, (SPI | PIO));

   /* SPI Software Reset */
   CSP_SPI_SET_CR(spi, SWRST);
 
   /* Disable SPI PIO */
   CSP_SPI_SET_PDR(spi, (NPCS3 | NPCS2 | NPCS1 | NPCS0 | MOSI | MISO | SPCK));

   /* Configure SPI Mode */
   CSP_SPI_SET_MR(spi, mode);
}


/******************************************************************************
* Function          : CSP_SPIClose
* Description       : Reset and switch off the clock
* Inputs            : <*spi> = Pointer to SPI structure
* Functions called  : None
* Returns           : None
******************************************************************************/
void CSP_SPIClose(CSP_SPI_T *const spi)
{
   /* SPI Software Reset */
   CSP_SPI_SET_CR(spi, SWRST);

   /* Disable SPI Clock */
   CSP_SPI_SET_DCR(spi, (SPI | PIO));
}


/******************************************************************************
* Function          : CSP_SPIConfigureCS
* Description       : Configure the SPI Chip Select 
* Inputs            : <*spi> = Pointer to SPI structure
*                     <cs>   = Chip Select [0-3] to be configured
*                     <mode> = Configure the SPI Chip Select mode
* Functions called  : None
* Returns           : None
******************************************************************************/
void CSP_SPIConfigureCS(CSP_SPI_T *const spi, U8_T cs, U32_T mode)
{   
   /* Configure SPI CS mode */
   CSP_SPI_CS_SET_CSR(spi, cs, mode);
}


/******************************************************************************
* Function          : CSP_SPIConfigInterrupt
* Description       : Configure SPI Interrupts
* Inputs            : <*spi>     = Pointer to SPI structure
*                     <int_mode> = Configure the priority level and source type
*                     <int_mask> = Configure which interrupt bits are activated
*                     <callback> = Function called through the assembler interrupt handler
* Functions called  : CSP_GET_INTERRUPT_NUMBER, CSP_GICConfigInterrupt
* Returns           : None
******************************************************************************/
void CSP_SPIConfigInterrupt(CSP_SPI_T *const spi, 
                            U32_T int_mode, 
                            U32_T int_mask, 
                            U32_T callback)
{
   /* Local Variables */
   U8_T int_num = 0;

   /* Get Peripheral Interrupt Number */
   int_num = CSP_GET_INTERRUPT_NUMBER(spi);

   /* Disable all interrupt */
   CSP_SPI_SET_IDR(spi, 0xFFFFFFFF);
   
   /* Interrupt Enable */
   CSP_SPI_SET_IER(spi, int_mask);

   /* Configure SPI controller interrupt mode in GIC module */
   CSP_GICConfigInterrupt(int_num, int_mode, callback);
}


/******************************************************************************
* Function          : CSP_SPIEnable
* Description       : Enable SPI
* Inputs            : <*spi> = Pointer to SPI structure 
* Functions called  : None
* Returns           : None
******************************************************************************/
void CSP_SPIEnable(CSP_SPI_T *const spi)
{
   /* Enable SPI */
   CSP_SPI_SET_CR(spi, SPIEN);
}


/******************************************************************************
* Function          : CSP_SPIDisable
* Description       : Disable SPI 
* Inputs            : <*spi> = Pointer to SPI structure
* Functions called  : None
* Returns           : None
******************************************************************************/
void CSP_SPIDisable(CSP_SPI_T *const spi)
{
   /* Disable SPI */
   CSP_SPI_SET_CR(spi, SPIDIS);
}


/******************************************************************************
* Function          : CSP_SPIReceive
* Description       : Configure SPI to receive data. They are transferred 
*                     automatically to thanks to PDC.
* Inputs            : <*spi>    = Pointer to SPI structure
*                     <cs>      = Receiver Chip Select [0-3]
*                     <*cmd_tx> = Pointer to an array where command packet sent 
*                                 is stored 
*                     <*data>   = Pointer to an array where data packet received 
*                                 will be stored
*                     <length>  = Number of byte to receive
* Functions called  : CSP_PDCInitTx, CSP_PDCInitRx, CSP_PDCStartRx, CSP_PDCStartTx
* Returns           : None
******************************************************************************/
void CSP_SPIReceive(CSP_SPI_T *const spi, U8_T cs, U8_T *cmd_tx, U8_T *data, U16_T length)
{  
   U8_T transfer_size = 0;

   /* Get the type of peripheral select */
   if((CSP_SPI_GET_MR(spi) & PS) == PS)
   {
      /* Variable peripheral select : 16 bits Data + PCS = 32 bits */
      /*-----------------------------------------------------------*/
      transfer_size = SIZE_WORD;
   }
   else
   {
      /* Fix peripheral select */
      /*-----------------------*/
  
      /* Get the transfer size */
      if((CSP_SPI_CS_GET_CSR(spi, cs) & BITS) == 0)
      {
         /* 8 bits Data transfer */
         transfer_size = SIZE_BYTE;
      }
      else
      {
         /* 16 bits Data transfer */
         transfer_size = SIZE_HALFWORD;
      }   
   }

   /* PDC Definition */
   /* Send To Peripheral Opcode to read to a specified address */
   CSP_PDCInitTx((U32_T) spi, (U32_T) &(spi->TDR), transfer_size);

   /* Receive data from SPI */
   CSP_PDCInitRx((U32_T) spi, (U32_T) &(spi->RDR), transfer_size);

   /* Enable SPI */
   CSP_SPI_SET_CR(spi, SPIEN);

   /* Start Dialog */
   CSP_PDCStartRx((U32_T) spi, (U32_T) data, length);
   CSP_PDCStartTx((U32_T) spi, (U32_T) cmd_tx, length);
}


/******************************************************************************
* Function          : CSP_SPITransmit
* Description       : Configure SPI to transmit data. They are transferred 
*                     automatically from memory to SPI transmission buffer thanks to PDC.
* Inputs            : <*spi>   = Pointer to SPI structure
*                     <cs>     = Transmitter Chip Select [0-3]
*                     <*data>  = Pointer to an array where data packet sent is stored
*                     <length> = Number of byte to transmit
* Functions called  : CSP_PDCInitTx, CSP_PDCStartTx 
* Returns           : None
******************************************************************************/
void CSP_SPITransmit(CSP_SPI_T *const spi, U8_T cs, U8_T *data, U16_T length)
{
   U8_T transfer_size = 0;

   /* Get the type of peripheral select */
   if((CSP_SPI_GET_MR(spi) & PS) == PS)
   {
      /* Variable peripheral select : 16 bits Data + PCS = 32 bits */
      /*-----------------------------------------------------------*/
      transfer_size = SIZE_WORD;
   }
   else
   {
      /* Fix peripheral select */
      /*-----------------------*/

      /* Get the transfer size */
      if((CSP_SPI_CS_GET_CSR(spi, cs) & BITS) == 0)
      {
         /* 8 bits data transfer */
         transfer_size = SIZE_BYTE;
      }
      else
      {
         /* 16 bits data transfer */
         transfer_size = SIZE_HALFWORD;
      }   
   }
   
   /* PDC Definition */
   /* Send to Peripheral Opcode to read to a specified address */
   CSP_PDCInitTx((U32_T) spi, (U32_T) &(spi->TDR), transfer_size);

   /* Enable SPI */
   CSP_SPI_SET_CR(spi, SPIEN);

   /* Start Dialog */
   CSP_PDCStartTx((U32_T) spi, (U32_T) data, length);
}


/******************************************************************************
* Function          : CSP_SPIPioInit
* Description       : Configure SPI PIO 
* Inputs            : <*spi>       = Pointer to SPI structure
*                     <pio_mask>   = Configure which pins are activated
*                     <output_pio> = Configure which pins are configured as output
* Functions called  : None
* Returns           : None
******************************************************************************/
void CSP_SPIPioInit(CSP_SPI_T *const spi, U32_T pio_mask, U32_T output_pio)
{
   /* Enable PIO block */
   CSP_SPI_SET_ECR(spi, PIO);
   
   /* Disable all PIO */
   CSP_SPI_SET_PDR(spi, 0xFFFFFFFF);
   CSP_SPI_SET_ODR(spi, 0xFFFFFFFF);

   /* Enable PIO */
   CSP_SPI_SET_PER(spi, pio_mask);

   /* Set Output PIO */
   CSP_SPI_SET_OER(spi, output_pio);
}


/******************************************************************************
* Function          : CSP_SPIPioGetStatus
* Description       : Read the pin data status 
* Inputs            : <*spi> = Pointer to SPI structure
* Functions called  : None
* Returns           : 32-bit value of pin data status
******************************************************************************/
U32_T CSP_SPIPioGetStatus(CSP_SPI_T *const spi)
{
   /* Return PIO State */
   return (CSP_SPI_GET_PDSR(spi));
}


/******************************************************************************
* Function          : CSP_SPIPioClear
* Description       : Set the PIO to low level 
* Inputs            : <*spi>     = Pointer to SPI structure
*                     <pio_mask> = Configure which pins are set to low level
* Functions called  : None
* Returns           : None
******************************************************************************/
void CSP_SPIPioClear(CSP_SPI_T *const spi, U32_T pio_mask)
{
   /* Set PIO State */
   CSP_SPI_SET_CODR(spi, pio_mask);
}


/******************************************************************************
* Function          : CSP_SPIPioSet
* Description       : Set the PIO to high level 
* Inputs            : <*spi>     = Pointer to SPI structure
*                     <pio_mask> = Configure which pins are set to high level
* Functions called  : None
* Returns           : None
******************************************************************************/
void CSP_SPIPioSet(CSP_SPI_T *const spi, U32_T pio_mask)
{
   /* Set PIO State */
   CSP_SPI_SET_SODR(spi, pio_mask);
}

