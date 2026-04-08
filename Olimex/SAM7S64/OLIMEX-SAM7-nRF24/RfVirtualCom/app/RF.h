/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2007
 *
 *    File name   : RF.h
 *    Description : RF Header file.
 *
 *    History :
 *    1. Date        : 09.1.2008 
 *       Author      : Stoyan Choynev
 *       Description : initial version
 *
 *    $Revision: 4039 $
 **************************************************************************/

/** include files **/
#ifndef __RF_H
#define __RF_H
/** definitions **/
#define RAND_MASK 0x1F //
#define RAND_GARD_TIME ((rand() & RAND_MASK)+5)   //Random Gard time (min is 5ms max i 36ms)
#define RX_TX_GARD_TIME 2                         //2ms 

typedef enum
{
  RF_RX_MODE,
  RF_START_TX_MODE,
  RF_TX_MODE,
}RfModeTypeDef;
/** public data **/
extern RfModeTypeDef RfMode;
/** public functions **/
/*************************************************************************
 * Function Name: RfInit
 * Parameters: None
 *
 * Return: None
 *
 * Description: Inits RF module. Transceiver stays in Power Down mode
 *
 *************************************************************************/
extern void RfInit(void);
/*************************************************************************
 * Function Name: RxPowerUp
 * Parameters: None
 *
 * Return: None
 *
 * Description: Puts Transceiver in Power Up mode. Clears all
 *              Receive and Transmit Buffers
 *************************************************************************/
extern void RfPowerUp(void);
/*************************************************************************
 * Function Name: RxPowerDown
 * Parameters: None
 *
 * Return: None
 *
 * Description: Puts Transceiver in Power Down mode. Clears all
 *              Receive and Transmit Buffers
 *************************************************************************/
extern void RfPowerDown(void);
/*************************************************************************
 * Function Name: StartRx
 * Parameters:None
 *
 * Return:None
 *
 * Description:Sets Transceiver in RX Mode
 *
 *************************************************************************/
extern void StartRx(void);
/*************************************************************************
 * Function Name: StartRx
 * Parameters:None
 *
 * Return:None
 *
 * Description:Copies Data from RfTxBuffer to NRF TX FIFO and 
 *                    Sets Transceiver in TX Mode
 *************************************************************************/
extern void StartTx(void);
/*************************************************************************
 * Function Name: DecGardTxTimer
 * Parameters:None
 *
 * Return: None
 *
 * Description:Decrements GardtxTimer if it is not 0
 *
 *************************************************************************/
extern void DecGardTxTimer(void);
/*************************************************************************
 * Function Name: GetGardTxTimer
 * Parameters:None
 *
 * Return: None
 *
 * Description:Returns the GardTxTimer Value
 *
 *************************************************************************/
extern int GetGardTxTimer(void);
extern void StartGardTxTimer(Int32U);

/*************************************************************************
 * Function Name: RF_IRQ_Handler
 * Parameters:None
 *
 * Return: None
 *
 * Description: Handler of Transceiver RX, TX and MAX RT interrupts
 *
 *************************************************************************/
extern void RF_IRQ_Handler(void);

/*************************************************************************
 * Function Name: RfGetData
 * Parameters:pInt32U pointer to the Dest Buffer
 *            Int32U Available buffer size
 * Return:    Int32U size of the transferred data
 *
 * Description: Read data From Nrf FIFO and puts it to Dest Buffer until
 *              Fifo gets empty or Buffer gets Full
 *************************************************************************/
extern Int32U RfGetData(pInt8U , Int32U);

/*************************************************************************
 * Function Name: RfPutData
 * Parameters:pInt32U pointer to the Source Buffer
 *            Int32U Data size
 * Return:    Int32U size of the transferred data
 *
 * Description: Fills Nrf Fifo with data from the source Buffer
 *              until all data is sent of Fifo gets full
 *************************************************************************/
extern Int32U RfPutData(pInt8U pBuffer, Int32U DataSize);

#endif /*__RF_H*/
