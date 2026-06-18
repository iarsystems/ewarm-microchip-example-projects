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
* Processor   : SmartCAN1 for EVMSmartCAN
* File Name   : bsp_eeprom.h
* Description : Definitions and Standard Typedef for
                EEPROM access (Board Support Package Layer)               
* Version     : 1.00
*
*       +----- (NEW | MODify | ADD | DELete)                                 
*       |                                                                    
*  No   |   when       who                what               
*-----+---+----------+------------------+--------------------------------------
* 000  MOD  01/07/01   David Combe    Creation
*----------------------------------------------------------------------------*/


#ifndef BSP_EEPROM_H
#define BSP_EEPROM_H


/****************************************************************************
* BSP INSTRUCTION SET FOR EEPROM AT25080
****************************************************************************/
#define EEPROM_WRITE_ENABLE    0x06  /* Set Write Enable Latch       */
#define EEPROM_WRITE_DISABLE   0x04  /* Reset Write Enable Latch     */
#define EEPROM_READ_STATUS     0x05  /* Read Status Register         */
#define EEPROM_WRITE_STATUS    0x01  /* Write Status Register        */
#define EEPROM_READ            0x03  /* Read Data from Memory Array  */
#define EEPROM_WRITE           0x02  /* Write Data from Memory Array */


//typedef enum {NONE_CMD, STATUS_EEPROM_READ_EE, STATUS_EEPROM_WRITE_EE,
//             EEPROM_WRITE_END_EE,EEPROM_READ_END_EE } BSP_CMD_STATE_E;
typedef enum {
              NO_CMD,EEPROM_INIT_E,GET_STATUS_E,SET_STATUS_E,
              EEPROM_WRITE_E,EEPROM_WRITE_END_E,
              EEPROM_READ_E,EEPROM_READ_END_E
             } BSP_EEPROM_CMD_E;

/****************************************************************************
* BSP EEPROM Structure Definition
****************************************************************************/

/************************************/
/* Bsp EEPROM Data Definiton        */
/************************************/

/*******************************************************************************
Type: BSP_EEPROM_DATA_T
Description: BSP EEPROM Data Definiton
Fields:
- 创U32_T创 address_u32;
- 创U8_T创  byte_count_u8;
- 创U8_T创  data_a_t[32];
*******************************************************************************/
typedef struct
{
   U32_T address_u32;
   U8_T  byte_count_u8;
   U8_T  data_a_t[32];
}BSP_EEPROM_DATA_T;


/******************************************/
/* Bsp EEPROM State Structure Definiton  */
/******************************************/

/*******************************************************************************
Type: BSP_EEPROM_T
Description: Bsp EEPROM State Structure Definiton
Fields:
- 创BSP_EEPROM_DATA_T创       data_s; 
- 创U8_T创                    busy;
- 创BSP_EEPROM_CMD_E创        cmd_state_e;
- 创U8_T创                    wait_state;
- 创U8_T创                    chip_select;
- 创U8_T创                    page_size;
- 创U8_T创                    cmd_tx[64];
*******************************************************************************/
typedef struct
{
  BSP_EEPROM_DATA_T       data_s; 
  U8_T                    busy;
  BSP_EEPROM_CMD_E        cmd_state_e;
  U8_T                    wait_state;
  U8_T                    chip_select;
  U8_T                    page_size;
  U8_T                    cmd_tx[64];
} BSP_EEPROM_T;


/***************************************************************************
*  External BSP EEPROM Declaration
****************************************************************************/
extern BSP_EEPROM_T BSP_Eeprom_s;
extern U8_T eeprom_tx_count, eeprom_tx_init;


/***************************************************************************
*  External BSP Function Definition
****************************************************************************/
extern void BSP_EEPROMGetStatus(void);
extern void BSP_EEPROMInit(void);
extern void BSP_EEPROMRead(BSP_EEPROM_T *bsp_eeprom_p);
extern void BSP_EEPROMWrite(BSP_EEPROM_T *bsp_eeprom_p);

extern void BSP_EEPROM_InterruptHandler(void);
extern void ASM_EEPROM_InterruptHandler(void);

#endif /* BSP_EEPROM_H */
 
