/*******************************************************************************
 * (c) Copyright 2008 Actel Corporation.  All rights reserved.
 * 
 * Cortex-M1 vector table.
 *
 * SVN $Revision: 5 $
 * SVN $Date: 2013-12-23 13:52:47 +0100 (mån, 23 dec 2013) $
 */

        MODULE  vector_table

        ;; Forward declaration of sections.
        SECTION CSTACK:DATA:NOROOT(3)

        SECTION .intvec:CODE:ROOT(2)
	
        EXTERN  __iar_program_start
        
        EXTERN   cortex_nmi_handler
        EXTERN   cortex_fault_handler
        EXTERN   cortex_sv_call
        EXTERN   cortex_pend_sv
        
        EXTWEAK  cortex_systick_isr
        EXTWEAK  cortex_irq_0_isr
        EXTWEAK  cortex_irq_1_isr
        EXTWEAK  cortex_irq_2_isr
        EXTWEAK  cortex_irq_3_isr
        EXTWEAK  cortex_irq_4_isr
        EXTWEAK  cortex_irq_5_isr
        EXTWEAK  cortex_irq_6_isr
        EXTWEAK  cortex_irq_7_isr
        
        PUBLIC  __vector_table

        DATA
__vector_table
        DC32    sfe(CSTACK)
        DC32    __iar_program_start
        DC32    cortex_nmi_handler      ; __nmi_handler
        DC32    cortex_fault_handler    ; __fault_handler
        DC32    0                       ; Reserved
        DC32    0                       ; Reserved
        DC32    0                       ; Reserved
        DC32    0                       ; Reserved
        DC32    0                       ; Reserved
        DC32    0                       ; Reserved
        DC32    0                       ; Reserved
        DC32    cortex_sv_call          ; System service call with SVC instruction.
        DC32    0                       ; Reserved.
        DC32    0                       ; Reserved.
        DC32    cortex_pend_sv          ; Pendable request for system service.
        DC32    cortex_systick_isr      ; System tick timer.
        DC32    cortex_irq_0_isr        ; External interrupt 0.
        DC32    cortex_irq_1_isr        ; External interrupt 1.
        DC32    cortex_irq_2_isr        ; External interrupt 2.
        DC32    cortex_irq_3_isr        ; External interrupt 3.
        DC32    cortex_irq_4_isr        ; External interrupt 4.
        DC32    cortex_irq_5_isr        ; External interrupt 5.
        DC32    cortex_irq_6_isr        ; External interrupt 6.
        DC32    cortex_irq_7_isr        ; External interrupt 7.

	END

