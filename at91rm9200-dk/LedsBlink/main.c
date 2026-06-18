#include "AT91RM9200.h"
//#include <Microchip/ioat91rm9200.h>
#include <intrinsics.h>


#define LED_USER ( 0x1<<2 )
#define TIME ( 0x2FFF )


static __arm __irq void TC2_Handler( void ) ;


int main()
{
  /* general preparations */
  AT91C_BASE_AIC->AIC_IDCR = 0xFFFFFFFF ;  /* reset the aic, disable all interrupts */
  AT91C_BASE_AIC->AIC_ICCR = 0xFFFFFFFF ;  /* clear all interrupts */
  AT91C_BASE_AIC->AIC_EOICR = 0 ;  /* unstack one level */
  AT91C_BASE_PMC->PMC_PCER |= ( 1UL<<AT91C_ID_PIOB ) ;  /* enable piob */
  AT91C_BASE_PMC->PMC_PCER |= ( 1UL<<AT91C_ID_TC2 ) ;  /* enable tc2 */
  /* start/prepare timer */
  AT91C_BASE_TC2->TC_IDR = ( 0xFFFFFFFF ) ;  /* disable all ints */
  AT91C_BASE_TC2->TC_SR ;
  AT91C_BASE_TC2->TC_CMR = ( AT91C_TC_WAVE|AT91C_TC_WAVESEL_UP|AT91C_TC_CLKS_TIMER_DIV5_CLOCK ) ;  /* wave mode, div5 */
  AT91C_BASE_TC2->TC_CMR |= ( AT91C_TC_EEVT_RISING /*AT91C_TC_EEVT_XC0*/ ) ;  /* external event not toib, enables comp b interrupt */
  AT91C_BASE_TC2->TC_CCR |= ( AT91C_TC_CLKEN ) ;  /* enable timer */
  AT91C_BASE_TC2->TC_CCR |= ( AT91C_TC_SWTRG ) ;  /* start timer */
  AT91C_BASE_AIC->AIC_IDCR |= ( 1UL<<AT91C_ID_TC2 ) ;  /* disable pio interrupt */
  AT91C_BASE_AIC->AIC_SVR[AT91C_ID_TC2] = ( AT91_REG )TC2_Handler ;  /* set isr */
  AT91C_BASE_AIC->AIC_SMR[AT91C_ID_TC2] = ( AT91C_AIC_SRCTYPE_INT_LEVEL_SENSITIVE )|( 1/* prio 1*/ ) ;
  AT91C_BASE_AIC->AIC_ICCR |= ( 1UL<<AT91C_ID_TC2 ) ;  /* clear pio interrupt */
  AT91C_BASE_AIC->AIC_IECR |= ( 1UL<<AT91C_ID_TC2 ) ;  /* enable pio interrupt */
  /* prepare output */
  AT91C_BASE_PIOB->PIO_PER |= ( LED_USER ) ;  /* pio enable for LED */
  AT91C_BASE_PIOB->PIO_OER |= ( LED_USER ) ;  /* LED are output */
  /* enable global interrupts */
  __enable_interrupt( ) ;
  /* init blinking */
  AT91C_BASE_TC2->TC_RA = AT91C_BASE_TC2->TC_CV + TIME ;
  AT91C_BASE_TC2->TC_IER |= AT91C_TC_CPAS ;  /* enable RA compare int */
  for( ; ; )
    {
    }
}


static __arm __irq void TC2_Handler( void )
{
  AT91_REG TC2_SR_Buffer ;

  AT91C_BASE_AIC->AIC_IVR = 0 ;  /* enter interrupt in protected mode */
  TC2_SR_Buffer = AT91C_BASE_TC2->TC_SR ;  /* clear/buffer flags */
  if( AT91C_TC_CPAS & TC2_SR_Buffer & AT91C_BASE_TC2->TC_IMR )
    {  /* timeout interrupt */
    AT91C_BASE_TC2->TC_RA += TIME ;
    if( AT91C_BASE_PIOB->PIO_PDSR & LED_USER )
      {
      AT91C_BASE_PIOB->PIO_CODR |= ( LED_USER ) ;
      }
    else
      {
      AT91C_BASE_PIOB->PIO_SODR |= ( LED_USER ) ;
      }
    }
  AT91C_BASE_AIC->AIC_EOICR = 0 ;  /* end interrupt */
}
