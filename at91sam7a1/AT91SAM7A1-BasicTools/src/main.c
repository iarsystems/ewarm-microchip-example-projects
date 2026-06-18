//*----------------------------------------------------------------------------
//*         ATMEL Microcontroller Software Support  -  ROUSSET  -
//*----------------------------------------------------------------------------
//* The software is delivered "AS IS" without warranty or condition of any
//* kind, either express, implied or statutory. This includes without
//* limitation any warranty or condition with respect to merchantability or
//* fitness for any particular purpose, or against the infringements of
//* intellectual property rights of others.
//*----------------------------------------------------------------------------
//* File Name           : main.c
//* Object              : main application written in C
//* Creation            : JPP   16/Jun/2005
//*----------------------------------------------------------------------------


// Include Standard files
#include "board.h"

/* Global variables */
#define SPEED 		(EXT_OC/100)
unsigned int LedSpeed = SPEED *5 ;
CSP_GPT_T   *const GPT0CH0 = ((CSP_GPT_T*)   GPT0CH0_BASE_ADDRESS);/* 16-bit Timer T0 channel 0   */
CSP_GPT_T   *const GPT0CH1 = ((CSP_GPT_T*)   GPT0CH1_BASE_ADDRESS);/* 16-bit Timer T0 channel 1   */

//*--------------------------------------------------------------------------------------
//* Function Name       : wait
//* Object              : Software waiting loop
//* Input Parameters    : none. Waiting time is defined by the global variable LedSpeed.
//* Output Parameters   : none
//*--------------------------------------------------------------------------------------
void wait ( void )
{//* Begin
    unsigned int waiting_time ;
    for(waiting_time = 0; waiting_time < LedSpeed; waiting_time++) ;
}//* End


//*--------------------------------------------------------------------------------------
//* Function Name       : GPT_LED_init
//* Object              : This function set tyhe PIO's red, orange and green LED.
//*--------------------------------------------------------------------------------------
void GPT_LED_init(void)
{

      /* Timer Chanel 0 RED LED => TIOA ORANGE LED => TIOB */
      CSP_GPT_SET_ECR(GPT0CH0, PIO);   /* Enable channel 0 PIO clock  for RED & ORANGE*/
      CSP_GPT_SET_PER(GPT0CH0, TIOA | TIOB);  /* Enable the PIO to control TIOA1 pin */
      CSP_GPT_SET_OER(GPT0CH0, TIOA | TIOB);  /* Enable the PIO output on TIOA1 pin */
      CSP_GPT_SET_SODR(GPT0CH0, TIOA | TIOB); /* Set output on TIOA1 pin */

      /* Timer Chanel 1 Green LED => TIOA */
      CSP_GPT_SET_ECR(GPT0CH1, PIO);          /* Enable channel 1 PIO clock */
      CSP_GPT_SET_PER(GPT0CH1, TIOA);         /* Enable the PIO to control TIOA0 pin */
      CSP_GPT_SET_OER(GPT0CH1, TIOA);          /* Enable the PIO output on TIOA0 pin */
      CSP_GPT_SET_SODR(GPT0CH1, TIOA);         /* Set output on TIOA0 pin */
}

//*--------------------------------------------------------------------------------------
//* Function Name       : GPT_Light_Off_LED
//* Object              : This function lights off red, orange and/or green LED.
//*--------------------------------------------------------------------------------------
void GPT_Light_On_LED(unsigned int led)
{
   /* If red LED is selected, do the following to light on red LED */
   if ((led & RED) == RED)
   {  /* Set output on TIOA1 pin */
      CSP_GPT_SET_SODR(GPT0CH0, TIOA);
   }

   /* If orange LED is selected, do the following to light on orange LED */
   if ((led & ORANGE) == ORANGE)
   {  /* Set output on TIOB0 pin */
      CSP_GPT_SET_SODR(GPT0CH0, TIOB);
   }

   /* If green LED is selected, do the following to light on green LED */
   if ((led & GREEN) == GREEN)
   {  /* Set output on TIOA0 pin */
      CSP_GPT_SET_SODR(GPT0CH1, TIOA);
   }
}

//*--------------------------------------------------------------------------------------
//* Function Name       : GPT_Light_Off_LED
//* Object              : This function lights off red, orange and/or green LED.
//*--------------------------------------------------------------------------------------
void GPT_Light_Off_LED(unsigned int led)
{
   /* If red LED is selected, do the following to light off red LED */
   if ((led & RED) == RED)
   {  /* Clear output on TIOA1 pin */
      CSP_GPT_SET_CODR(GPT0CH0, TIOA);
   }

   /* If orange LED is selected, do the following to light on orange LED */
   if ((led & ORANGE) == ORANGE)
   {  /* Clear output on TIOB0 pin */
      CSP_GPT_SET_CODR(GPT0CH0, TIOB);
   }

   /* If green LED is selected, do the following to light on green LED */
   if ((led & GREEN) == GREEN)
   {  /* Clear output on TIOA0 pin */
      CSP_GPT_SET_CODR(GPT0CH1, TIOA);
   }
}

//*--------------------------------------------------------------------------------------
//* Function Name       : main
//* Object              : Main application. It swings LEDs.
//*--------------------------------------------------------------------------------------
int main (void)
{

   /* Light off all LEDs */
   GPT_LED_init();

   while(1)
   {
   		/* Wait */
   		wait();

   		/* Light on red led */
   		GPT_Light_On_LED(RED);

   		/* Wait */
   		wait();

   		/* Light on orange led */
   		GPT_Light_On_LED(ORANGE);

   		/* Wait */
   		wait();

   		/* Light on green led */
   		GPT_Light_On_LED(GREEN);

   		/* Wait */
   		wait();

   		/* Light off red led */
   		GPT_Light_Off_LED(RED);

   		/* Wait */
   		wait();

   		/* Light off orange led */
   		GPT_Light_Off_LED(ORANGE);

   		/* Wait */
   		wait();

   		/* Light off green led */
   		GPT_Light_Off_LED(GREEN);
   }
}
