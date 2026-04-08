/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support 
 * ----------------------------------------------------------------------------
 * Copyright (c) 2008, Atmel Corporation
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Atmel's name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ----------------------------------------------------------------------------
 */

//------------------------------------------------------------------------------
/// \dir "TWI Temerature Sensor Project"
///
/// !!!Purpose
///
/// This example program demonstrates how to use the TWI peripheral of an AT91
/// microcontroller to access an temerature sensor. 
///
/// !See
/// - twi: Two wire interface driver
///
/// !!!Requirements
///
/// An temperature sensor must be connected to the TWI bus of the 
/// microcontroller. For further information on this topic, please refer to 
/// the following application note: 
/// <a href="http://www.atmel.com/dyn/resources/prod_documents/doc6327.pdf">
/// Using the Two-wire interface (TWI) in Master Mode on AT91 Microcontrollers. </a>
///
/// Pay particular attention to the fact that on some boards, such as the
/// AT91SAM7S-EK, there is no pull-up on the TWI bus: they must be 
/// added externally. 
///
/// !!!Description
///
/// This software performs simple tests on reading the ambient temperature and related registers: 
/// - Read all the registers within the sensor. 
/// - Read the configuration such as adc resolution,alert mode etc.
/// - Get the current ambient temerature.
///
/// !!!Usage
///
/// -# Compile the application. 
/// -# Connect the DBGU port of the evaluation board to the computer and open 
/// it in a terminal.
///    - Settings: 115200 bauds, 8 bits, 1 stop bit, no parity, no flow control. 
/// -# Download the program inside the evaluation board and run it. Please refer to
///    <a href="http://www.atmel.com/dyn/resources/prod_documents/doc6132.pdf">
///    the SAM-BA User Guide, 
///    <a href="http://www.atmel.com/dyn/resources/prod_documents/doc6310.pdf">
///    the GNU-Based Software Development</a> application note or to the 
///    <a href="http://www.iar.se/website1/1.0.1.0/78/1/index.php?">
///    IAR EWARM User Guide</a>, 
///    depending on your chosen solution. 
/// -# Upon startup, the application will output the following line on the DBGU: 
///    \code
///     --  Basic TWI Temperature Sensor Project xxx --
///     -- AT91xxxxxx-xx
///     -- Compiled: xxx xx xxxx xx:xx:xx --
///    \endcode
/// -# User can do some test by inputting character through DBGU,following the 
///    output prompt.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// \unit
///
/// !Purpose
///
/// This file contains all the specific code for the 
/// basic-twi-tempsensor-project.
///
/// !Contents
/// The code can be roughly broken down as follows:
///    - TWI interrupt handler
///    - The main function, which implements the program behavior
///       - Configure TWI
///       - Sets the first and second page of the EEPROM to all zeroes
///       - Writes pattern in page 0; 
///          Reads back data in page 0 and compare with original pattern (polling). 
///       - Writes pattern in page 1; 
///          Reads back data in page 1 and compare with original pattern (interrupts). 
///
/// Please refer to the list of functions in the #Overview# tab of this unit
/// for more detailed information.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//         Headers
//------------------------------------------------------------------------------

#include <board.h>
#include <pio/pio.h>
#include <irq/irq.h>
#include <dbgu/dbgu.h>
#include <twi/twi.h>
#include <utility/math.h>
#include <utility/assert.h>
#include <utility/trace.h>
#include <drivers/async/async.h>
#include <drivers/twi/twid.h>

#include <stdio.h>
#include <string.h>

//------------------------------------------------------------------------------
//         Local definitions
//------------------------------------------------------------------------------

/// TWI clock frequency in Hz.
#define TWCK            100000

///TWI0 ID
#define TWI_TEMP_ID    AT91C_ID_TWI0

#define INVALID_TEMP (-110)
/// Slave address of Temperature Sensor.
#define MCP9800_ADDRESS   0x48

/// Internal register within MCP9800
#define TEMP_REG       0x0
#define CONF_REG       0x1
#define HYST_REG       0x2
#define LIMT_REG       0x3


typedef struct 
{
  unsigned char bOneShot;
  unsigned char bRes;
  unsigned char bFaultQueue;
  unsigned char bAlrtPol;
  unsigned char bMode;
  unsigned char bShutdown;
}Confg_Reg;



//------------------------------------------------------------------------------
//         Local variables
//------------------------------------------------------------------------------

/// Pio pins to configure.
static const Pin pins_twi_temp[] = {PINS_TWI0};

/// TWI driver instance.
static Twid twid;


/// constant represent float value
const char* sixteenths[]={ "0","0625","125","1875","25","3125","375","4375","5","5625","625","6875","75","8125","875",
"9375"};
//------------------------------------------------------------------------------
//         Local functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Get the real temperature code regardless of different adc resolutions
//------------------------------------------------------------------------------

short GetRealTemp(unsigned short reg_value,unsigned char res)
{
  if( 8 < res && 13 > res)
  {
    return (reg_value>>(16-res));
    
  }
  return INVALID_TEMP;
}
//------------------------------------------------------------------------------
///  Fill the register value into specific structure
//MCP9800 configuration register:8bits
//bit 7 ONE-SHOT bits
//bit 5-6 ж▓жд ADC RESOLUTION bit
//bit 3-4 FAULT QUEUE bit
//bit 2 ALERT POLARITY bit
//bit 1 COMP/INT bit
//bit 0 SHUTDOWN bit
//------------------------------------------------------------------------------

void FillConfigReg(unsigned char reg_value,Confg_Reg *conf)
{
  unsigned char res_array[]={9,10,11,12};
  unsigned char fault_queue_array[]={1,2,4,6};
  unsigned char temp;
  temp = ((reg_value>>7)&0x1);
  conf->bOneShot = temp;
  temp = ((reg_value>>5)&0x3);
  conf->bRes = res_array[temp];
  temp = ((reg_value>>3)&0x3);
  conf->bFaultQueue = fault_queue_array[temp];
  temp = ((reg_value>>2)&0x1);
  conf->bAlrtPol = temp;
  temp = ((reg_value>>1)&0x1);
  conf->bMode = temp;
  conf->bShutdown = (reg_value&0x1);
  
}
//------------------------------------------------------------------------------
/// Show the prompting menu to interact with user
//------------------------------------------------------------------------------

void ShowOperationMenu()
{
    printf(" --- Temperature Sensor Operation Menu: ---\n\r");
    printf(" A --- Read all the internal registers of Sensor\n\r");
    printf(" C --- Get current configuration\n\r");
    printf(" T --- Get Ambient Temperature\n\r");

}

//------------------------------------------------------------------------------
/// Main function
//------------------------------------------------------------------------------
int main()
{
    //volatile unsigned int i;
    //Async async;

    char key;
    unsigned char Tdata[2];
    short temp;
    Confg_Reg confg;

    PIO_Configure(pins_twi_temp, PIO_LISTSIZE(pins_twi_temp));
    TRACE_CONFIGURE(DBGU_STANDARD, 115200, BOARD_MCK);
    printf("-- Basic TWI Temperature Sensor Project %s --\n\r", SOFTPACK_VERSION);
    printf("-- %s\n\r", BOARD_NAME);
    printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);

    // Configure TWI
    AT91C_BASE_PMC->PMC_PCER = 1 << TWI_TEMP_ID;//AT91C_ID_TWI0;
    TWI_ConfigureMaster(AT91C_BASE_TWI0, TWCK, BOARD_MCK);
    TWID_Initialize(&twid, AT91C_BASE_TWI0);
    
    ///ShowOperationMenu
    ShowOperationMenu();
    
    //functions:
    //1)Read all the registers,'A' or 'a'
    //2)Read configuration,'C' or 'c'
    //3)Read temperatures,'T' or 't'
   
    while(1)
    {
      key = DBGU_GetChar();
      
      if(key == 'A' || key == 'a')
      {
         TWID_Read(&twid, MCP9800_ADDRESS, TEMP_REG, 0x01, Tdata, 0x02, 0);
         temp = ((Tdata[0]<<8)|(Tdata[1]));
         printf("--I-- ambient temperature value:0x%X\n\r",temp);
         
         TWID_Read(&twid, MCP9800_ADDRESS, CONF_REG, 0x01, Tdata, 0x01, 0);        
         printf("--I-- configuraiton register value:0x%X\n\r",Tdata[0]);
         
         TWID_Read(&twid, MCP9800_ADDRESS, HYST_REG, 0x01, Tdata, 0x02, 0);         
         printf("--I-- hysteresis register value:0x%X\n\r",((Tdata[0]<<8)|(Tdata[1])));
         
         TWID_Read(&twid, MCP9800_ADDRESS, LIMT_REG, 0x01, Tdata, 0x02, 0);
         printf("--I-- limit-set register value:0x%X\n\r",((Tdata[0]<<8)|(Tdata[1])));
         
         
        
      }
      
      if(key == 'C'|| key == 'c')
      {
         TWID_Read(&twid, MCP9800_ADDRESS, CONF_REG, 0x01, Tdata, 0x01, 0);  
         FillConfigReg(Tdata[0],&confg);
         printf("--I-- Temperature Configuration!!\n\r");
         printf("--I-- One-Shot Mode Enabled/Disabled:%d\n\r",confg.bOneShot);
         printf("--I-- ADC Resolution:%d bits \n\r",confg.bRes);
         printf("--I-- Fault Queue:%d\n\r",confg.bFaultQueue);
         printf("--I-- Alert Polarity:%d\n\r",confg.bAlrtPol);
         printf("--I-- COMP/INT mode:%d\n\r",confg.bMode);
         printf("--I-- ShutDown Mode Enabled/Disabled:%d\n\r",confg.bShutdown);
         
      }
      
      if(key == 'T'|| key == 't')
      {
        char index;
        short integer;
        TWID_Read(&twid, MCP9800_ADDRESS, CONF_REG, 0x01, Tdata, 0x01, 0);  
        FillConfigReg(Tdata[0],&confg);
        TWID_Read(&twid, MCP9800_ADDRESS, TEMP_REG, 0x01, Tdata, 0x02, 0);
        temp = ((Tdata[0]<<8)|(Tdata[1]));
        temp = GetRealTemp(temp,confg.bRes);
        index = temp%(2<<(confg.bRes-8-1));
        index =  index <<(4-(confg.bRes-8));
        integer =   temp/(2<<(confg.bRes-8-1));       
        printf("The ambient temperature is:%d.%s C\n\r",integer,sixteenths[index]);
        
        
        
      }
      ShowOperationMenu();
      
      
    }
}

