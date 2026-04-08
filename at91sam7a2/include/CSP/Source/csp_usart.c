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
* File Name       : csp_usart.c
* Description     : Function declarations for Universal Synchronous/Asynchronous 
*                   Receiver/Transmitter management
* Library Version : 2.00
* Module Version  : 1.XX
*
*       +----- (NEW | MODify | ADD | DELete)                                 
*       |                                                                    
*  No   |   When       Who                What               
*-----+---+----------+------------------+--------------------------------------
* 000  NEW  01/05/99   Patrice VILCHEZ    Creation
* 001  MOD  01/04/01   Olivier MAZUYER    Clean up
* 002  ADD  07/06/01   Frederic SAMSON    Add fonctions :
*                                         CSP_USARTDisableInterrupt
* 003  MOD  08/06/01   Frederic SAMSON    Add CSP_USARTDisableInterrupt
* 004  MOD  16/07/01   Frederic SAMSON    Clean Up
* 005  MOD  05/11/01   Christophe GARDIN  Clean Up
* 006  MOD  11/04/02   Christophe GARDIN  Add CSP_USARTClose function
* 007  MOD  18/07/02   Christophe GARDIN  Add LIN functions
*----------------------------------------------------------------------------*/

/******************************************************************************
* Include Files
******************************************************************************/
#include "csp.h"


/******************************************************************************
****************************  F U N C T I O N S  ******************************
******************************************************************************/

/******************************************************************************
* Function          : CSP_USARTInit
* Description       : Switch on the clock, reset the registers and configure 
*                     the USART module mode and the PDC RX/TX lines
* Inputs            : <*usart>     = Pointer to USART structure
*                     <mode>       = Configure the USART mode
*                     <baudrate>   = Configure the Baud Rate Generator
*                     <time_out>   = Configure the receiver time-out : the time-out 
*                                    is loaded when the Start Time-out Command 
*                                    is given or when each new character is received
*                     <time_guard> = Configure the transmit time-guard : TXD is 
*                                    inactive (high level) after the transmission 
*                                    of each character for the time-guard duration
* Functions called  : CSP_PDCInitTx, CSP_PDCInitRx
* Returns           : None
******************************************************************************/
void CSP_USARTInit(CSP_USART_T *const usart, 
                   U32_T mode, 
                   U16_T baudrate, 
                   U8_T time_out, 
                   U8_T time_guard)
{
   /* Local Variables */
   U8_T transfert_size = 0;

   /* Enable USART Clock */
   CSP_USART_SET_ECR(usart, (USART | PIO));

   /* USART Software Reset */
   CSP_USART_SET_CR(usart, (SWRST | RSTRX | RSTTX | RSTSTA));

   /* Disable USART PIO */
   CSP_USART_SET_PDR(usart, (RXD | TXD | SCK));

   /* Configure USART Mode */
   CSP_USART_SET_MR(usart, mode);
    
   /* Set Receiver Time Out */
   CSP_USART_SET_RTOR(usart, time_out);
  
   /* Set Transmitter Timer Guard */
   CSP_USART_SET_TTGR(usart, time_guard);
  
   /* Set Baudrate */
   CSP_USART_SET_BRGR(usart, baudrate);
  
   /* Configure the transfer size in accordance with the Mode Register */
   if((mode & MODE9) == 0)
   {
      transfert_size = SIZE_BYTE;
   }
   else
   {
      transfert_size = SIZE_HALFWORD;
   }
   
   /* Configure PDC Tx Line */
   CSP_PDCInitTx((U32_T)usart, (U32_T) &(usart->THR), transfert_size);

   /* Configure PDC Rx Line */
   CSP_PDCInitRx((U32_T)usart, (U32_T) &(usart->RHR), transfert_size);
}
 

/******************************************************************************
* Function          : CSP_USARTInitLin
* Description       : Switch on the clock, reset the registers and configure 
*                     the USART module in LIN mode
* Inputs            : <*usart>     = Pointer to USART structure
*                     <time_guard> = Configure the transmit time-guard (interbyte 
*                                    space)
* Functions called  : None
* Returns           : None
******************************************************************************/
void CSP_USARTInitLin(CSP_USART_T *const usart, U8_T time_guard)
{
   /* Enable USART Clock */
   CSP_USART_SET_ECR(usart, (USART | PIO));

   /* USART Software Reset */
   CSP_USART_SET_CR(usart, (SWRST | RSTRX | RSTTX));

   /* Disable USART PIO */
   CSP_USART_SET_PDR(usart, (RXD | TXD | SCK));

   /* Configure LIN Mode */
   CSP_USART_SET_MR(usart, LIN);
    
   /* Set Transmitter Timer Guard */
   CSP_USART_SET_TTGR(usart, time_guard);
}


/******************************************************************************
* Function          : CSP_USARTClose
* Description       : Reset and switch off the clock
* Inputs            : <*usart> = Pointer to USART structure
* Functions called  : None
* Returns           : None
******************************************************************************/
void CSP_USARTClose(CSP_USART_T *const usart)
{
   /* USART Software Reset */
   CSP_USART_SET_CR(usart, (SWRST | RSTRX | RSTTX | RSTSTA));

   /* Disable USART Clock */
   CSP_USART_SET_DCR(usart, (USART | PIO));
}


/******************************************************************************
* Function          : CSP_USARTConfigInterrupt
* Description       : Configure USART Interrupts
* Inputs            : <*usart>   = Pointer to USART structure
*                     <int_mode> = Configure the priority level and source type
*                     <int_mask> = Configure which interrupt bits are activated
*                     <callback> = Function called through the assembler interrupt handler
* Functions called  : CSP_GET_INTERRUPT_NUMBER, CSP_GICConfigInterrupt
* Returns           : None
******************************************************************************/
void CSP_USARTConfigInterrupt(CSP_USART_T *const usart, 
                              U32_T int_mode, 
                              U32_T int_mask, 
                              U32_T callback)
{
   /* Local Variables */
   U8_T int_num = 0;

   /* Get Peripheral Interrupt Number */
   int_num = CSP_GET_INTERRUPT_NUMBER(usart);

   /* Disable all interrupt */
   CSP_USART_SET_IDR(usart, 0xFFFFFFFF);
   
   /* Interrupt Enable */
   CSP_USART_SET_IER(usart, int_mask);

   /* Configure USART controller interrupt mode in GIC module */
   CSP_GICConfigInterrupt(int_num, int_mode, callback);
}


/******************************************************************************
* Function          : CSP_USARTEnable
* Description       : Enable USART Rx and/or Tx
* Inputs            : <*usart>      = Pointer to USART structure
*                     <enable_mask> = Configure which functions (RX and/or TX) 
*                     are enabled
* Functions called  : None
* Returns           : None
******************************************************************************/
void CSP_USARTEnable(CSP_USART_T *const usart, U32_T enable_mask)
{
   /* USART Rx and/or Tx Enable */
   CSP_USART_SET_CR(usart, enable_mask);
}


/******************************************************************************
* Function          : CSP_USARTDisable
* Description       : Disable USART Rx and/or Tx
* Inputs            : <*usart>       = Pointer to USART structure
*                     <disable_mask> = Configure which functions (RX and/or TX) 
*                     are disabled
* Functions called  : None
* Returns           : None
******************************************************************************/
void CSP_USARTDisable(CSP_USART_T *const usart, U32_T disable_mask)
{
   /* USART Rx and/or Tx Disable */
   CSP_USART_SET_CR(usart, disable_mask);
}
 

/******************************************************************************
* Function          : CSP_USARTReceive
* Description       : Configure USART to receive data. They will be transferred 
*                     automatically to memory thanks to PDC.
* Preconditions     : You must wait 1 baud rate period after resetting the receiver 
*                     (RSTRX in CR) before receiving data
* Inputs            : <*usart> = Pointer to USART structure
*                     <*data>  = Pointer to an array where data packet received will be store
*                     <length> = Number of byte to receive
* Functions called  : CSP_PDCStartRx
* Returns           : None
******************************************************************************/
void CSP_USARTReceive(CSP_USART_T *const usart, U8_T *data, U16_T length)
{
   /* Disable Reception */
   CSP_USART_SET_CR(usart, RXDIS);

   /* Configure PDC Rx Line to Receive a packet */
   CSP_PDCStartRx((U32_T) usart, (U32_T) data, length);

   /* Clear Status */
   CSP_USART_SET_CR(usart, STTTO);

   /* Enable Reception (if RTOR = 0 time out is disabled) */
   CSP_USART_SET_CR(usart, RXEN);
}


/******************************************************************************
* Function          : CSP_USARTTransmit
* Description       : Configure USART to transmit data. They are transferred 
*                     automatically from memory to USART transmission buffer thanks to PDC.
* Preconditions     : You must wait 1 baud rate period after resetting the transmitter 
*                     (RSTTX in CR) before transmitting data
* Inputs            : <*usart> = Pointer to USART structure
*                     <*data>  = Pointer to an array where data packet sent is stored
*                     <length> = Number of byte to transmit
* Functions called  : CSP_PDCStartTx
* Returns           : None
******************************************************************************/
void CSP_USARTTransmit(CSP_USART_T *const usart, U8_T *data, U16_T length)
{
   /* Disable Transmission */
   CSP_USART_SET_CR(usart, TXDIS);

   /* Configure PDC Tx Line to Send a packet */
   CSP_PDCStartTx((U32_T) usart, (U32_T) data, length);

   /* Enable Transmission */
   CSP_USART_SET_CR(usart, TXEN);
}


/******************************************************************************
* Function          : CSP_USARTTransmitLinHeaderFrame
* Description       : Transmit a LIN header Frame
* Postconditions    : You should wait the set of the ENDHEADER bit in SR
* Inputs            : <*usart>     = Pointer to USART structure
*                     <sblr>       = Configure the Synchro Break Length 
*                     <identifier> = Configure the LIN's identifier
* Functions called  : None
* Returns           : None
******************************************************************************/
void CSP_USARTTransmitLinHeaderFrame(CSP_USART_T *const usart, U8_T sblr, U8_T identifier)
{
   /* Configure the Sync Break Length */
   CSP_USART_SET_SBLR(usart, sblr);
   
   /* Configure identifier */
   CSP_USART_SET_LIR(usart, identifier);

   /* Transmit */
   CSP_USART_SET_CR(usart, STHEADER);
}


/******************************************************************************
* Function          : CSP_USARTTransmitLinResponseFrame
* Description       : Transmit a LIN Response Frame
* Preconditions     : You should use the CSP_USARTTransmitLinHeaderFrame function 
*                     before using this function
* Postconditions    : You should wait the set of the ENDMESS bit in SR
* Inputs            : <*usart> = Pointer to USART structure
*                     <*data>  = Pointer to an array where data packet sent is stored
*                     <length> = Number of byte to transmit
* Postconditions    : You should wait the set of the ENDMESS bit in SR
* Functions called  : None
* Returns           : None
******************************************************************************/
void CSP_USARTTransmitLinResponseFrame(CSP_USART_T *const usart, U8_T *data, U8_T length)
{
   /* Local Variables */
   U8_T i = 0; 
   U8_T dfwr[8] = {0, 0, 0, 0, 0, 0, 0, 0};     /* Clear DFWR */

   /* Copy N bytes in DFWR to Send */
   for(i = 0; i < length; i++)
   {
      dfwr[i] = data[i];
   }
   
   /* The writing in DFWR must be in 32 bits */
   CSP_USART_SET_DFWR0(usart, (*(U32_T*)&dfwr[0]));
   CSP_USART_SET_DFWR1(usart, (*(U32_T*)&dfwr[4]));

   /* Transmit */
   CSP_USART_SET_CR(usart, STRESP);
}


/******************************************************************************
* Function          : CSP_USARTPioInit
* Description       : Configure USART PIO 
* Inputs            : <*usart>     = Pointer to USART structure
*                     <pio_mask>   = Configure which pins are activated
*                     <output_pio> = Configure which pins are configured as output 
* Functions called  : None
* Returns           : None
******************************************************************************/
void CSP_USARTPioInit(CSP_USART_T *const usart, U32_T pio_mask, U32_T output_pio)
{
   /* Enable PIO block */
   CSP_USART_SET_ECR(usart, PIO);

   /* Disable all PIO */
   CSP_USART_SET_PDR(usart, 0xFFFFFFFF);
   CSP_USART_SET_ODR(usart, 0xFFFFFFFF);

   /* Enable PIO */
   CSP_USART_SET_PER(usart, pio_mask);
  
   /* Set Output PIO */
   CSP_USART_SET_OER(usart, output_pio);
}


/******************************************************************************
* Function          : CSP_USARTPioGetStatus
* Description       : Read the pin data status
* Inputs            : <*usart> = Pointer to USART structure
* Functions called  : None
* Returns           : 32-bit value of pin data status
******************************************************************************/
U32_T CSP_USARTPioGetStatus(CSP_USART_T *const usart)
{
   /* Return PIO State */
   return (CSP_USART_GET_PDSR(usart));
}


/******************************************************************************
* Function          : CSP_USARTPioClear
* Description       : Set the PIO to low level 
* Inputs            : <*usart>   = Pointer to USART structure
*                     <pio_mask> = Configure which pins are set to low level
* Functions called  : None
* Returns           : None
******************************************************************************/
void CSP_USARTPioClear(CSP_USART_T *const usart, U32_T pio_mask)
{
   /* Set PIO State */
   CSP_USART_SET_CODR(usart, pio_mask);
}


/******************************************************************************
* Function          : CSP_USARTPioSet
* Description       : Set the PIO to high level 
* Inputs            : <*usart>   = Pointer to USART structure
*                     <pio_mask> = Configure which pins are set to high level
* Functions called  : None
* Returns           : None
******************************************************************************/
void CSP_USARTPioSet(CSP_USART_T *const usart, U32_T pio_mask)
{
   /* Set PIO State */
   CSP_USART_SET_SODR(usart, pio_mask);
}
