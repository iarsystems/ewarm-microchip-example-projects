/*   EUROPE TECHNOLOGIES Software Support
*------------------------------------------------------------------------------
* The software is delivered "AS IS" without warranty or condition of any
* kind, either express, implied or statutory. This includes without
* limitation any warranty or condition with respect to merchantability or
* fitness for any particular purpose, or against the infringements of
* intellectual property rights of others.
*------------------------------------------------------------------------------
*
* Processor   : easyCAN3
* File Name   : bsp_eeprom.c
* Description : Function declarations for Eeprom for the BSP.
* Version     : 1.01
*
*       +----- (NEW | MODify | ADD | DELete)                                 
*       |                                                                    
*  No   |   when       who                what               
*-----+---+----------+------------------+--------------------------------------
* 000  MOD  01/07/01   David Combe    Creation
* 001  MOD  31/01/02   Mahmoud Mesgarzadeh   Clean Up
*----------------------------------------------------------------------------*/
#include <string.h>
#include "csp.h"
#include "bsp.h"


/*BSP Global Variable */
BSP_EEPROM_T BSP_Eeprom_s;
U8_T eeprom_tx_count, eeprom_tx_init;

/*****************************************************************************
* Function Name       : BSP_EEPROMInit
* Object              : Config. EEPROM & SPI port
* Input Parameters    : BSP_EEPROM_T *BSP_Eeprom_pt
* Output Parameters   : None
* Functions called    : CIP_SPIInit / CIP_SPIStart / CIP_SPIConfigueCS
*                     :
******************************************************************************/
void BSP_EEPROMInit(void)
{
   U32_T mode  ;
   U32_T cs;

   /* Configure BSP_EEPROM Characteristics */
   BSP_Eeprom_s.chip_select          = 0;     /* bsp_eeprom_chipselect */
   BSP_Eeprom_s.page_size            = 0x20;  /* bsp_eeprom_Page_size  */
   eeprom_tx_init                    = 2;

   /* Parameters of SPI Bus */
   cs = (0xF & (0<<BSP_Eeprom_s.chip_select))<<16;
   mode = ( MSTR| PSFIX| cs );

   /* Configures SPI Module */
   CSP_SPIInit(SPI0, mode);

   /* Config SPI CSR[0] */
   CSP_SPIConfigureCS(SPI0, 10<<24|5<<8|1<<0, BSP_Eeprom_s.chip_select);

   /* start SPI */
   CSP_SPIEnable(SPI0);
}

/*****************************************************************************
* Function Name       : BSP_EEPROMGetStatus
* Object              : Configue Commands to Get Status
*                     : of EEPROM (WEN/RDY)
* Input Parameters    : None
* Output Parameters   : None
* Functions called    : CSP_SPIConfigInterrupt  *                     : CSP_SPIWrite
*                     : CIP_STStart
******************************************************************************/
void BSP_EEPROMGetStatus(void)
{
   U32_T status;
   status= CSP_SPI_GET_SR(SPI0);

   /* Config. RDRF Interrupt */
   CSP_SPIConfigInterrupt(SPI0,(HIGH_LEVEL_SENSITIVE | PRIOR_1), RDRF,(U32_T) ASM_EEPROM_InterruptHandler);

   /* Read Status */
   /* Configure commandes */
   BSP_Eeprom_s.cmd_tx[0] =  EEPROM_READ_STATUS;
   BSP_Eeprom_s.cmd_tx[1] =  0xFF;

   /* Set Eeprom busy  (Default state) */
   BSP_Eeprom_s.busy = 1 ;

   /* Set initial data to send */
   eeprom_tx_count= 2 ;
   eeprom_tx_init = 2 ;

   /* Verify Write Enable */
   CSP_SPITransmit(SPI0, BSP_Eeprom_s.chip_select, BSP_Eeprom_s.cmd_tx, eeprom_tx_init);
 
}

/*****************************************************************************
* Function Name       : BSP_EEPROMWrite
* Object              : Configue Commands to on Specific EEPROM Page
* Input Parameters    : BSP_EEPROM_T *BSP_Eeprom_p
* Output Parameters   : None
* Functions called    : CIP_SPIWrite
******************************************************************************/
void BSP_EEPROMWrite(BSP_EEPROM_T *BSP_Eeprom_p)
{
   /* Local Variables */
   U8_T i_u8;
   
   /* Set Eeprom busy  (Default state) */
   BSP_Eeprom_s.busy = 1 ;
   
   /* Config. RDRF Interrupt */
   CSP_SPIConfigInterrupt(SPI0,(HIGH_LEVEL_SENSITIVE | PRIOR_1), RDRF,(U32_T) ASM_EEPROM_InterruptHandler);
   
   /* Configure commandes */
   BSP_Eeprom_p->cmd_tx[0] =  EEPROM_WRITE;
   BSP_Eeprom_p->cmd_tx[1] = (BSP_Eeprom_p->data_s.address_u32 & 0xFF00)>>8;
   BSP_Eeprom_p->cmd_tx[2] =  BSP_Eeprom_p->data_s.address_u32 & 0xFF;

   /* Get Data To Write */
   for(i_u8=0;i_u8<BSP_Eeprom_p->data_s.byte_count_u8;i_u8++)
   {
      BSP_Eeprom_p->cmd_tx[i_u8+3] = BSP_Eeprom_p->data_s.data_a_t[i_u8]  ;
   }

   eeprom_tx_init = BSP_Eeprom_p->data_s.byte_count_u8+3;
   eeprom_tx_count = eeprom_tx_init  ;

   /* Write In the EEPROM */
   CSP_SPITransmit(SPI0, BSP_Eeprom_p->chip_select, BSP_Eeprom_p->cmd_tx, BSP_Eeprom_p->data_s.byte_count_u8+3);
}


/*****************************************************************************
* Function Name       : BIOS_EEPROMRead
* Object              : reads Specified data_t from the EEPROM
*                     : address into a Ram buffer
* Input Parameters    : BSP_EEPROM_T *BSP_Eeprom_p
* Output Parameters   : None
* Functions called    : SPI_Read
*                     : CIP_STStart
******************************************************************************/
void BSP_EEPROMRead(BSP_EEPROM_T *BSP_Eeprom_p)
{
   /* Local Variable*/
   U8_T i_u8=0;

   /* Config. REND PDC Interrupt */
   CSP_SPIConfigInterrupt(SPI0,(HIGH_LEVEL_SENSITIVE | PRIOR_4), RDRF,(U32_T) ASM_EEPROM_InterruptHandler);

   /* Set Eeprom busy  (Default state) */
   BSP_Eeprom_s.busy = 1 ;

   /* Configure commandes */
   BSP_Eeprom_p->cmd_tx[0] =  EEPROM_READ;
   BSP_Eeprom_p->cmd_tx[1] = (BSP_Eeprom_p->data_s.address_u32  & 0xFF00)>>8;
   BSP_Eeprom_p->cmd_tx[2] =  BSP_Eeprom_p->data_s.address_u32  & 0xFF;

   /* Clear data_t array */
   for (i_u8=0; i_u8<BSP_Eeprom_p->data_s.byte_count_u8; i_u8++)
      BSP_Eeprom_p->cmd_tx[i_u8+3] = 0xAA;

   eeprom_tx_init = BSP_Eeprom_p->data_s.byte_count_u8+3;
   eeprom_tx_count = eeprom_tx_init  ;

   /* Read In the EEPROM */
   CSP_SPITransmit(SPI0, BSP_Eeprom_p->chip_select, BSP_Eeprom_p->cmd_tx, BSP_Eeprom_p->data_s.byte_count_u8+3);
}


/****************************************************************************
Function    : BSP_EEPROM_InterruptHandler
Description : Interrupt function for Eeprom Management
            : Due to waity state CS Set up Time
Inputs      : None
Returns     : None
****************************************************************************/
void BSP_EEPROM_InterruptHandler(void)
{
   /* Rx Full */
   if( (CSP_SPI_GET_SR(SPI0) & RDRF ) == RDRF)
   {
      /* Disable Interrupt RDRF */
      if((eeprom_tx_count-1) == 0x0)
      {
        SPI0->IDR  = RDRF;
        /* Set Eeprom busy  (Default state) */
        BSP_Eeprom_s.busy = 0; 
       }

      /* Read reception register*/
      BSP_Eeprom_s.cmd_tx[eeprom_tx_init - eeprom_tx_count] =((CSP_SPI_GET_RDR(SPI0) & RD)& 0xFF);

      /* Decrement value */
      eeprom_tx_count--;
   }
 }
