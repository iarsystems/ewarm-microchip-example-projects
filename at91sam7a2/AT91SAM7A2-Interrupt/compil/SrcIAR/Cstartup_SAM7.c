//*----------------------------------------------------------------------------
//*         ATMEL Microcontroller Software Support  -  ROUSSET  -
//*----------------------------------------------------------------------------
//* The software is delivered "AS IS" without warranty or condition of any
//* kind, either express, implied or statutory. This includes without
//* limitation any warranty or condition with respect to merchantability or
//* fitness for any particular purpose, or against the infringements of
//* intellectual property rights of others.
//*----------------------------------------------------------------------------
//* File Name           : Cstartup_SAM7.c
//* Object              : Low level initializations written in C for IAR Tools
//* Creation            : 01/Jul/05
//*----------------------------------------------------------------------------

// Include the board file description
#include "Board.h"

// The following functions must be write in ARM mode this function called directly
// by exception vector
extern void AT91F_Spurious_handler(void);
extern void AT91F_Default_IRQ_handler(void);
extern void AT91F_Default_FIQ_handler(void);


//*----------------------------------------------------------------------------
//* \fn    AT91F_LowLevelInit
//* \brief This function performs very low level HW initialization
//*        this function can be use a Stack, depending the compilation
//*        optimization mode
//* Input Parameters
//* <Vector>  vector table Address determinate in Relative addressing
//* <InternalRam> Internal Address determinate in Relative addressing
//* Output Parameters
//* <AMC address>
//*----------------------------------------------------------------------------
unsigned int AT91F_LowLevelInit( unsigned int * Vector, unsigned int * InternalRam, unsigned int size)  @ "ICODE"
{
 int            i;
 CSP_AMC_T*    pAMC;
 CSP_GIC_T*    pGic;

        //-----------------------------------------------------------------------------
        // Speed up the Boot sequence
        //---------------------------
        // After reset, the number of wait states on chip select 0 is 8. All AT91
        // Evaluation Boards fits fast flash memories, so that the number of wait
        // states can be optimized to fast up the boot sequence.
        //-----------------------------------------------------------------------------
        // set sandart Wait State
        pAMC = (CSP_AMC_T*)    AMC_BASE_ADDRESS ;

        pAMC->CSR[0] = AMC_CSR_0 ;

        /* Disable PLL */
        CSP_CM_SET_CD(((CSP_CM_T*)CM_BASE_ADDRESS), PLLSLCT);

        /* PLL = 10 ==> Coreclock = 6Mhz*10/2 = 30 Mhz */
        CSP_CM_SET_PDIV(((CSP_CM_T*)CM_BASE_ADDRESS), PLLDIV2 | 10 );

        /* Enable PLL */
        CSP_CM_SET_CE(((CSP_CM_T*)CM_BASE_ADDRESS), PLLSLCT);

        //-----------------------------------------------------------------------------
        //  Set up EBI value
        //--------------------
        // After reset, All EBI register are setted at the default value
        // The new value will be effective only after the remap command
        //-----------------------------------------------------------------------------
        // Load System pAMC Base address and CSR0 Init Value
        pAMC->CSR[1] = AMC_CSR_1 ;
        pAMC->CSR[2] = AMC_CSR_2 ;
        pAMC->CSR[3] = AMC_CSR_3 ;
        pAMC->CSR[4] = AMC_CSR_4 ;
        pAMC->CSR[5] = AMC_CSR_5 ;
        pAMC->CSR[6] = AMC_CSR_6 ;
        pAMC->CSR[7] = AMC_CSR_7 ;
        // 6 memory regions, standard read
        pAMC->MCR = MCR_VAL ;
        //-----------------------------------------------------------------------------
        // Reset the Interrupt Controller
        //-------------------------------
        // Normally, the code is executed only if a reset has been actually performed.
        // So, the GIC initialization resumes at setting up the default vectors.
        //-----------------------------------------------------------------------------
        // Load System pGIC Base address
        pGic = (CSP_GIC_T*)   GIC_BASE_ADDRESS;

        // Mask All interrupt
        pGic->IDCR = 0xFFFFFFFF;

        // Perform 8 End Of Interrupt Command to make sure GIC will not Lock out nIRQ
        for (i=0;i < 8; i++)
        {
                pGic->EOICR = 0;
        }

        // Set up the default interrupts handler vectors
        pGic->SVR[0] = (int) AT91F_Default_FIQ_handler ;
        for (i=1;i < 31; i++)
        {
                pGic->SVR[i] = (int) AT91F_Default_IRQ_handler ;
        }
        pGic->SPU  = (int) AT91F_Spurious_handler ;

        // Setup Exception Vectors in Internal RAM before Remap
        //-----------------------------------------------------
	//-----------------------------------------------------------------------------
	// Setup Exception Vectors in Internal RAM before Remap
	//-----------------------------------------------------
	// That's important to perform this operation before Remap in order to guarantee
	// that the core has valid vectors at any time during the remap operation.
	// Note: There are only 5 offsets as the vectoring is used.
	// Before Remap the internal RAM it's 0x300000
	// After  Remap the internal RAM it's 0x000000
	// Remap it's already executed it's no possible to write to 0x300000.
	//-----------------------------------------------------------------------------
	//  Copy the ARM exception vectors and indirect table

	for  (i=0;i < size/4; i++ )
	{
	   *InternalRam++=*Vector++;
	}

        return (unsigned int) (&pAMC->RCR);
}

