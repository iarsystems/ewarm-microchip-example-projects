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
//* Creation            : JPP   22/Jun/2005
//*----------------------------------------------------------------------------


// Include Standard files
#include "board.h"

#define SOFT_INTERRUPT_LEVEL	5

CSP_GIC_T   *const GIC     = ((CSP_GIC_T*)   GIC_BASE_ADDRESS);    /* Generic Interrupt Controller*/

extern void asm_SWIRQ0_Handler_Entry(void);
extern void asm_FIQ_Handlesr(void);

/* Global variables */
volatile unsigned int software_interrupt=0;

#define SPEED 		(EXT_OC/100)
unsigned int LedSpeed = SPEED *5 ;

CSP_GPT_T   *const GPT0CH0 = ((CSP_GPT_T*)   GPT0CH0_BASE_ADDRESS);/* 16-bit Timer T0 channel 0   */
CSP_GPT_T   *const GPT0CH1 = ((CSP_GPT_T*)   GPT0CH1_BASE_ADDRESS);/* 16-bit Timer T0 channel 1   */
CSP_GPT_T   *const GPT0CH2 = ((CSP_GPT_T*)   GPT0CH2_BASE_ADDRESS);/* 16-bit Timer T0 channel 2   */

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
//* Function Name       : GPT_LED_init
//* Object              : This function set tyhe PIO's red, orange and green LED.
//*--------------------------------------------------------------------------------------
void GPT_SW_init(void)
{
      /* Timer Chanel 2 SW2 => TIOA2  */
      CSP_GPT_SET_ECR(GPT0CH2, PIO);   /* Enable channel 0 PIO clock  for RED & ORANGE*/
      CSP_GPT_SET_PER(GPT0CH2, SW2 );  /* Enable the PIO to control TIOA2 pin */
      CSP_GPT_SET_ODR(GPT0CH2, SW2 );  /* Enable the PIO input on TIOA1 pin */

      /* Timer Chanel 1 SW3 => TIOA2  */
      CSP_GPT_SET_ECR(GPT0CH1, PIO);   /* Enable channel 0 PIO clock  for RED & ORANGE*/
      CSP_GPT_SET_PER(GPT0CH1, SW3 );  /* Enable the PIO to control TIOA2 pin */
      CSP_GPT_SET_ODR(GPT0CH1, SW3 );  /* Enable the PIO input on TIOA1 pin */

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

//*----------------------------------------------------------------------------
//* Function Name       : aic_software_interrupt
//* Object              : Software interrupt function
//* Input Parameters    : none
//* Output Parameters   : none
//* Functions called    : at91_pio_write
//*----------------------------------------------------------------------------
void c_software_interrupt(void)
{
     software_interrupt++;
    //* Read the output state
    if ( (software_interrupt & 0x01 ) == 0 )
    {
	/* Light off orange led */
        GPT_Light_On_LED(ORANGE);
    }
    else
    {
   	/* Light on orange led */
   	GPT_Light_Off_LED(ORANGE);
    }
}
//*----------------------------------------------------------------------------
//* Function Name       : FIQ_init_handler
//* Object              : Irq Handler called by the FIQ interrupt with AT91
//*                       compatibility
///*----------------------------------------------------------------------------
void c_FIQ_interrupt(void)
{
	GPT_Light_Off_LED(RED);
	GPT_Light_Off_LED(ORANGE);
	GPT_Light_Off_LED(GREEN);
}

//*--------------------------------------------------------------------------------------
//* Function Name       : main
//* Object              : Main application. It swings LEDs.
//*--------------------------------------------------------------------------------------
int main (void)
{

    unsigned int   loop_count ;

	//* Init
     	loop_count = 0 ;
   /* Light off all LEDs */
     GPT_LED_init();
   /* Init SW */
     GPT_SW_init();
// Set Software interrupt
    //* Disable the interrupt on the interrupt controller
    GIC->IDCR =  IBIT_SWIRQ0 ;
    //* Save the interrupt handler routine pointer and the interrupt priority
    GIC->SVR[SWIRQ0] = (unsigned int) asm_SWIRQ0_Handler_Entry ;

    //* Store the Source Mode Register
    GIC->SMR[SWIRQ0] = ( POSITIVE_EDGE_TRIGGERED | PRIOR_5 )  ;
    //* Clear the interrupt on the interrupt controller
    GIC->ICCR = IBIT_SWIRQ0 ;
    //* Ebnable IT
    GIC->IECR = IBIT_SWIRQ0 ;
   //* Software interrupt
    GIC->ISCR = IBIT_SWIRQ0 ;

 // Set FIQ
    //* open  FIQ interrupt
    //* Disable the interrupt on the interrupt controller
    GIC->IDCR =  IBIT_FIQ ;
    //* Save the interrupt handler routine pointer and the interrupt priority
    GIC->SVR[FIQ] = (unsigned int)c_FIQ_interrupt; // asm_FIQ_Handlesr ;
    //* Store the Source Mode Register
    GIC->SMR[FIQ] = ( POSITIVE_EDGE_TRIGGERED  )  ;
    //* Clear the interrupt on the interrupt controller
    GIC->ICCR = IBIT_FIQ ;
    //* Ebnable IT
    GIC->IECR = IBIT_FIQ ;
   //* Software interrupt
    GIC->ISCR = IBIT_FIQ ;


   while(1)
   {

  if ( (CSP_GPT_GET_PDSR(GPT0CH2) & SW2) == SW2)
  {
  		/* Light on green led */
   		GPT_Light_On_LED(GREEN);
  }
  if ( (CSP_GPT_GET_PDSR(GPT0CH1) & SW3) == SW3)
  {
  		/* Light on green led */
   		GPT_Light_On_LED(GREEN);
  }
   		/* Wait */
   		wait();

   		/* Light on red led */
   		GPT_Light_On_LED(RED);

   		wait();
   		/* Light off red led */
   		GPT_Light_Off_LED(RED);

   		/* Wait */
   		wait();

   		/* Light off green led */
   		GPT_Light_Off_LED(GREEN);
        loop_count ++ ;
    //* Set LED by software interrupt
        if (loop_count == 10)
        {
             loop_count=0;
         //* Software interrupt
             GIC->ISCR = IBIT_SWIRQ0 ;
        }
   }
}
