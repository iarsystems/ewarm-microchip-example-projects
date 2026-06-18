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
* File Name   : bsp_buzzer.h
* Description : Definitions and Standard Typedef for Buzzer.
* Version     : 1.01
*
*       +----- (NEW | MODify | ADD | DELete)                                 
*       |                                                                    
*  No   |   when       who                what               
*-----+---+----------+------------------+--------------------------------------
* 000  MOD  01/04/01   Olivier MAZUYER    Creation
*----------------------------------------------------------------------------*/


#ifndef BSP_BUZZER_H
#define BSP_BUZZER_H

/******************************************************************************
* Music Note definitions for the third octave
******************************************************************************/
/* US designation */
#define C         262    /* Music note C correspond to 262 Hz air vibration */
#define D         293    /* Music note D correspond to 293 Hz air vibration */
#define E         330    /* Music note E correspond to 330 Hz air vibration */
#define F         349    /* Music note F correspond to 349 Hz air vibration */
#define G         392    /* Music note G correspond to 392 Hz air vibration */
#define A         440    /* Music note A correspond to 440 Hz air vibration */
#define B         494    /* Music note B correspond to 494 Hz air vibration */

/* French designation */
#define DO        262    /* Music note DO  correspond to 262 Hz air vibration */
#define RE        293    /* Music note RE  correspond to 293 Hz air vibration */
#define MI        330    /* Music note MI  correspond to 330 Hz air vibration */
#define FA        349    /* Music note FA  correspond to 349 Hz air vibration */
#define SOL       392    /* Music note SOL correspond to 392 Hz air vibration */
#define LA        440    /* Music note LA  correspond to 440 Hz air vibration */
#define SI        494    /* Music note SI  correspond to 494 Hz air vibration */

/* No sounds */
#define NOSOUND   0      /* A 0 Hz air vibration will produce not sound      */  

/******************************************************************************
* Buzzer definitions
******************************************************************************/
#define CH_BUZZER  0x0    /* Buzzer is driven by channel 0                   */
#define HALF_DUTY  0x80   /* Dutycycle = 0x80 = 128 = 50%                    */


/****************************************************************************
* BSP Buzzer Structure Note Definitions                                          
****************************************************************************/
typedef struct 
{
   int freq;               /* Frequence of the note                        */
   int delay;              /* Delay of the note                            */
}BSP_NOTE_T, *BSP_NOTE_PTR;


/****************************************************************************
* BSP Buzzer 'Au Clair de la Lune' Declaration                             
****************************************************************************/
extern BSP_NOTE_T AuClairDeLaLune[]; 


/******************************************************************************
* External Functions
******************************************************************************/
extern void BSP_BuzzerSoundsFreq(U32_T freq_u32, U32_T delay_u32);

#endif /* BSP_BUZZER_H */
