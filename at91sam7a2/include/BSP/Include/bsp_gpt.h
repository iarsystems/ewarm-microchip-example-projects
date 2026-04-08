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
* Processor   : easyCAN3
* File Name   : bsp_gpt.h
* Description : Definitions and Standard Typedef for Timer Channel
* Version     : 1.01
*
*       +----- (NEW | MODify | ADD | DELete)                                 
*       |                                                                    
*  No   |   when       who                what               
*-----+---+----------+------------------+--------------------------------------
* 000  NEW  29/06/00   Patrice VILCHEZ    Creation
* 001  MOD  01/04/01   Olivier MAZUYER    Clean up
* 002  MOD  05/02/02   Mahmoud MESGARZADEH  Clean up
*----------------------------------------------------------------------------*/


#ifndef BSP_GPT_H
#define BSP_GPT_H

/******************************************************************************
* Define LED
******************************************************************************/
#define RED     (0x01 << 0)           /* Red    LED correspond to TIOA pin   */
#define ORANGE  (0x01 << 1)           /* Orange LED correspond to TIOB pin   */
#define GREEN   (0x01 << 2)           /* Green  LED correspond to TIOA pin   */


/******************************************************************************
* Define Switch
******************************************************************************/
#define SW2     TIOA                  /* SW2 button correspond to TIOA pin   */
#define SW3     TIOB                  /* SW3 button correspond to TIOB pin   */


/******************************************************************************
* BSP Mux 16 External Functions Declaration                                   
******************************************************************************/
extern void BSP_GPTSetOutput(CSP_GPT_T *const gpt, U32_T tio, U32_T frequency_u32, U16_T dutycycle_u16);
extern void BSP_GPTLightOnLED(U32_T led);
extern void BSP_GPTLightOffLED(U32_T led);

#endif  /* BSP_GPT_H */
