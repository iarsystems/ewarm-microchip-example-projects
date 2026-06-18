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
* File Name       : csp_pdc.c
* Description     : Function declarations for Peripheral Data Controller 
                    management
* Library Version : 2.00
* Module Version  : 1.XX
*
*       +----- (NEW | MODify | ADD | DELete)                                 
*       |                                                                    
*  No   |   When       Who                What               
*-----+---+----------+------------------+--------------------------------------
* 000  NEW  21/11/01   Christophe GARDIN  Creation
*----------------------------------------------------------------------------*/

/******************************************************************************
* Include Files
******************************************************************************/
#include "csp.h"


/******************************************************************************
****************************  F U N C T I O N S  ******************************
******************************************************************************/

/******************************************************************************
Function          : CSP_PDCInitRx
Description       : Switch on the clock and initialize the PDC Rx line
Inputs            :
- <periph_address>   = Peripheral used
- <register_address> = Configure the Peripheral Register Address
- <size>             = Configure the transfer size (8-bit/16-bit/32-bit)
Functions called  : 创CSP_PMCEnableClock创, 创CSP_GET_PDC_RX_NUMBER创
Returns           : None
******************************************************************************/
void CSP_PDCInitRx(U32_T periph_address, U32_T register_address, U8_T size)
{                        
   /* Enable PDC Clock */
   CSP_PMCEnableClock(PMC_PDC);

   /* Configure PDC Rx line */
   CSP_PDC_CHANNEL_SET_PRA(PDC, CSP_GET_PDC_RX_NUMBER(periph_address), register_address);
   CSP_PDC_CHANNEL_SET_CR(PDC, CSP_GET_PDC_RX_NUMBER(periph_address), 
                                             (U32_T)(size | DIR_PERIPH_TO_MEM));
}


/******************************************************************************
Function          : CSP_PDCInitTx
Description       : Switch on the clock and initialize the PDC Tx line
Inputs            :
- <periph_address>   = Peripheral used
- <register_address> = Configure the Peripheral Register Address
- <size>             = Configure the transfer size (8-bit/16-bit/32-bit)
Functions called  : 创CSP_PMCEnableClock创, 创CSP_GET_PDC_TX_NUMBER创
Returns           : None
******************************************************************************/
void CSP_PDCInitTx(U32_T periph_address, U32_T register_address, U8_T size)
{
   /* Enable PDC Clock */
   CSP_PMCEnableClock(PMC_PDC);

   /* Configure PDC Tx line */
   CSP_PDC_CHANNEL_SET_PRA(PDC, CSP_GET_PDC_TX_NUMBER(periph_address), register_address);
   CSP_PDC_CHANNEL_SET_CR(PDC, CSP_GET_PDC_TX_NUMBER(periph_address), 
                                             (U32_T)(size | DIR_MEM_TO_PERIPH));
}


/******************************************************************************
Function          : CSP_PDCStartRx
Description       : Start PDC Rx transfer
Inputs            :
- <periph_address> = Peripheral used
- <mem_address>    = Memory address where data packet received will be store
- <length>         = Number of byte to transfer
Functions called  : 创CSP_GET_PDC_RX_NUMBER创
Returns           : None
******************************************************************************/
void CSP_PDCStartRx(U32_T periph_address, U32_T mem_address, U16_T length)
{
   CSP_PDC_CHANNEL_SET_MPR(PDC, CSP_GET_PDC_RX_NUMBER(periph_address), mem_address);
   CSP_PDC_CHANNEL_SET_TCR(PDC, CSP_GET_PDC_RX_NUMBER(periph_address), (U32_T)length);
}


/******************************************************************************
Function          : CSP_PDCStartTx
Description       : Start PDC Tx transfer
Inputs            :
- <periph_address> = Peripheral used
- <mem_address>    = Memory address where data packet sent is stored
- <length>         = Number of byte to transfer
Functions called  : 创CSP_GET_PDC_TX_NUMBER创
Returns           : None
******************************************************************************/
void CSP_PDCStartTx(U32_T periph_address, U32_T mem_address, U16_T length)
{
   CSP_PDC_CHANNEL_SET_MPR(PDC, CSP_GET_PDC_TX_NUMBER(periph_address), mem_address);
   CSP_PDC_CHANNEL_SET_TCR(PDC, CSP_GET_PDC_TX_NUMBER(periph_address), (U32_T)length);
}
