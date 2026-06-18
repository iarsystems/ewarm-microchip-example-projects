//*----------------------------------------------------------------------------
//*         ATMEL Microcontroller Software Support  -  ROUSSET  -
//*----------------------------------------------------------------------------
//* The software is delivered "AS IS" without warranty or condition of any
//* kind, either express, implied or statutory. This includes without
//* limitation any warranty or condition with respect to merchantability or
//* fitness for any particular purpose, or against the infringements of
//* intellectual property rights of others.
//*----------------------------------------------------------------------------
//* File Name           : init.c
//* Object              : Low level initialisations written in C
//* Creation            : FB   23/10/2002
//* Modify              : SBb  10/06/2005
//*
//*----------------------------------------------------------------------------

#include "init.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

//*----------------------------------------------------------------------------
//* \fn    AT91F_WaitForMainClockFrequency
//* \brief This function performs very low level HW initialization
//*----------------------------------------------------------------------------
unsigned char AT91F_WaitForMainClockFrequency()
{
	volatile char	tmp	= 0;

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Step 2.
// Checking the Main Oscillator Frequency (Optional)
/////////////////////////////////////////////////////////////////////////////////////////////////////

	//* Determine the main clock frequency
	while(!(AT91C_BASE_CKGR->CKGR_MCFR & AT91C_CKGR_MAINRDY) && (tmp++ < DELAY_MAIN_FREQ));

	if (tmp >= DELAY_MAIN_FREQ)
		return FALSE;

	return TRUE;
}

//*----------------------------------------------------------------------------
//* \fn    AT91F_CheckPLL_FrequencyRange
//* \brief This function performs very low level HW initialiszation
//*----------------------------------------------------------------------------
unsigned char AT91F_CheckPLL_FrequencyRange(int MainClock,int pllDivider ,int pllMultiplier)
{
	if(pllDivider == 0)
		return FALSE;

	//* Check Input Frequency
	if( ((MainClock/pllDivider) < INPUT_FREQ_MIN)
	 || ((MainClock/pllDivider) > INPUT_FREQ_MAX) )
		return FALSE;

	//* Check Output Frequency
	if( ((MainClock/pllDivider*pllMultiplier) < OUTPUT_FREQ_MIN)
	 || ((MainClock/pllDivider*pllMultiplier) > OUTPUT_FREQ_MAX) )
		return FALSE;

	return TRUE;
}

//*----------------------------------------------------------------------------
//* \fn    AT91F_InitClocks
//* \brief This function performs very low level HW initialization
//*----------------------------------------------------------------------------
unsigned char AT91F_InitClocks(int PLLAR_Register,int PLLBR_Register ,int MCKR_Register)
{
	volatile char 	tmp = 0;
	unsigned int	MainClock;
	unsigned int 	pllDivider,pllMultiplier;

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Optionnal
/////////////////////////////////////////////////////////////////////////////////////////////////////

	//* Check if Input & Output Frequencies are in the correct range
	MainClock 		= AT91F_CKGR_GetMainClock(AT91C_BASE_CKGR,SLOWCLOCK);

  // Eanble main clock OSC and wait until expire the delay counter
  AT91C_BASE_CKGR->CKGR_MOR = 0x0000FF01;   // PMC_MOR: MOSCEN = 1, enable main clock
  while((AT91C_BASE_CKGR->CKGR_MCFR & AT91C_CKGR_MAINRDY) == 0);  // wair time out

	//* Check if Input & Output Frequencies are in the correct range
	MainClock 		= AT91F_CKGR_GetMainClock(AT91C_BASE_CKGR,SLOWCLOCK);

  if((MainClock < (MAIN_OSC_CLK * 0.9)) || (MainClock > (MAIN_OSC_CLK * 1.1)))
		return FALSE;

	pllDivider    	= (PLLAR_Register  & AT91C_CKGR_DIVA);
	pllMultiplier 	= ((PLLAR_Register  & AT91C_CKGR_MULA) >> 16) + 1;
	if(AT91F_CheckPLL_FrequencyRange(MainClock, pllDivider , pllMultiplier) == FALSE)
		return FALSE;

	pllDivider    	= (PLLBR_Register  & AT91C_CKGR_DIVB);
	pllMultiplier 	= ((PLLBR_Register  & AT91C_CKGR_MULB) >> 16) + 1;
	if(AT91F_CheckPLL_FrequencyRange(MainClock, pllDivider , pllMultiplier) == FALSE)
		return FALSE;

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Step 3.
// Setting PLLA and Divider A
/////////////////////////////////////////////////////////////////////////////////////////////////////

	AT91C_BASE_CKGR->CKGR_PLLAR = PLLAR_Register;
	//* Wait for PLLA stabilization LOCKA bit in PMC_SR
	tmp = 0;
	while( !(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_LOCKA) && (tmp++ < DELAY_PLL) ) ;

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Step 4.
// Setting PLLB and Divider B
/////////////////////////////////////////////////////////////////////////////////////////////////////

	AT91C_BASE_CKGR->CKGR_PLLBR = PLLBR_Register;
	//* Wait for PLLB stabilization LOCKB bit in PMC_SR
	tmp = 0;
	while( !(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_LOCKB) && (tmp++ < DELAY_PLL) ) ;

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Step 5.
// Selection of Master Clock MCK (and Processor Clock PCK)
/////////////////////////////////////////////////////////////////////////////////////////////////////

	//* Constraints of the Master Clock selection sequence
	//* Write in the MCKR dirty value concerning the clock selection CSS then overwrite it in a second sequence
	AT91C_BASE_PMC->PMC_MCKR = AT91C_PMC_CSS_MAIN_CLK;

  //* Wait until the master clock is established
  tmp = 0;
	while( !(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY) && (tmp++ < DELAY_MAIN_FREQ) );

  //* Second sequence
  //	AT91C_BASE_PMC->PMC_MCKR = MCKR_Register;
	AT91C_BASE_PMC->PMC_MCKR = MCKR_Register;

  //* Wait until the master clock is established
	tmp = 0;
	while( !(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY) && (tmp++ < DELAY_MAIN_FREQ) );

  for(tmp = 100; tmp; tmp--);
	return TRUE;
}

//*----------------------------------------------------------------------------
//* \fn    AT91F_InitSDRAM
//* \brief This function performs very low level HW initialisation
//*----------------------------------------------------------------------------
void AT91F_InitSDRAM()
{
	int 	i;
	volatile int 	*pSDRAM = (int *)BASE_EBI_CS1_ADDRESS;

	//* Configure PIOC as peripheral (D16/D31)
	AT91F_SDRC_CfgPIO();
	
	//* Setup MEMC to support CS1=SDRAM
	AT91C_BASE_EBI->EBI_CSA |= AT91C_EBI_CS1A;
	AT91C_BASE_EBI->EBI_CFGR = (AT91C_EBI_DBPUC & 0x00) | (AT91C_EBI_EBSEN & 0x00);

	//* Init SDRAM

	//* 1. A minimum pause of 200us is provided to precede any signal toggle
	AT91C_BASE_SDRC->SDRC_CR = AT91C_SDRC_NC_9 | AT91C_SDRC_NR_12 | AT91C_SDRC_NB_4_BANKS | AT91C_SDRC_CAS_2
								| 0x100 | 0x4000 | 0x8000
								| 0x880000
								| 0x21000000;


	//* 2. A Precharge All command is issued to the SDRAM
	AT91C_BASE_SDRC->SDRC_MR = AT91C_SDRC_MODE_PRCGALL_CMD;
	*pSDRAM = 0;

	//* 3. Eight Auto-refresh are provided
	AT91C_BASE_SDRC->SDRC_MR = AT91C_SDRC_MODE_RFSH_CMD;
	for(i=0;i<8;i++)
		*pSDRAM = 0;

	//* 4. A mode register cycle is issued to program the SDRAM parameters
	AT91C_BASE_SDRC->SDRC_MR = AT91C_SDRC_MODE_LMR_CMD;
	*(pSDRAM+0x80) = 0;

	//* 5. Write refresh rate into SDRAMC refresh timer COUNT register
	AT91C_BASE_SDRC->SDRC_TR = (AT91C_SDRC_COUNT & 0x2E0);
	*pSDRAM = 0;

	//* 6. A Normal Mode Command is provided, 3 clocks after tMRD is set
	AT91C_BASE_SDRC->SDRC_MR = AT91C_SDRC_MODE_NORMAL_CMD;
	*pSDRAM = 0;
}

//*----------------------------------------------------------------------------
//* \fn    AT91F_InitFlash
//* \brief This function performs very low level HW initialization
//*----------------------------------------------------------------------------
void AT91F_InitFlash()
{
	AT91C_BASE_MC->MC_PUIA[0] = AT91C_MC_PROT_PNAUNA;
	AT91C_BASE_MC->MC_PUP = 0;
	AT91C_BASE_MC->MC_PUER = 0;	//* Memory controller protection unit disable
	AT91C_BASE_MC->MC_ASR = 0;  //* read only!
	AT91C_BASE_MC->MC_AASR = 0; //* read only!

	//* Setup MEMC to support CS0=Flash
	AT91C_BASE_EBI->EBI_CSA |= AT91C_EBI_CS0A_SMC;
	AT91C_BASE_EBI->EBI_CFGR = (AT91C_EBI_DBPUC & 0x00) | (AT91C_EBI_EBSEN & 0x00);

	//* Setup Flash
	AT91C_BASE_SMC2->SMC2_CSR[0] = (AT91C_SMC2_NWS & 0x4) | AT91C_SMC2_WSEN
									| (AT91C_SMC2_TDF & 0x200) | AT91C_SMC2_BAT | AT91C_SMC2_DBW_16;
}

//*----------------------------------------------------------------------------
//* \fn    AT91F_DBGU_Printk
//* \brief This function is used to send a string through the DBGU channel (Very low level debugging)
//*----------------------------------------------------------------------------
void AT91F_DBGU_Printk(
	char *buffer) // \arg pointer to a string ending by \0
{
	while(*buffer != '\0') {
		while (!AT91F_US_TxReady((AT91PS_USART)AT91C_BASE_DBGU));
		AT91F_US_PutChar((AT91PS_USART)AT91C_BASE_DBGU, *buffer++);
	}
}

//*----------------------------------------------------------------------------
//* \fn    AT91F_DataAbort
//* \brief This function reports an Abort
//*----------------------------------------------------------------------------
void AT91F_SpuriousHandler()
{
	AT91F_DBGU_Printk("-F- Spurious Interrupt detected\n\r");
	while (1);
}


//*----------------------------------------------------------------------------
//* \fn    AT91F_DataAbort
//* \brief This function reports an Abort
//*----------------------------------------------------------------------------
void AT91F_DataAbort()
{
	AT91F_DBGU_Printk("-F- Data Abort detected\n\r");
	while (1);
}

//*----------------------------------------------------------------------------
//* \fn    AT91F_FetchAbort
//* \brief This function reports an Abort
//*----------------------------------------------------------------------------
void AT91F_FetchAbort()
{
	AT91F_DBGU_Printk("-F- Prefetch Abort detected\n\r");
	while (1);
}

//*----------------------------------------------------------------------------
//* \fn    AT91F_Undef
//* \brief This function reports an Abort
//*----------------------------------------------------------------------------
void AT91F_Undef()
{
	AT91F_DBGU_Printk("-F- Undef detected\n\r");
	while (1);
}

//*----------------------------------------------------------------------------
//* \fn    AT91F_UndefHandler
//* \brief This function reports that no handler have been set for current IT
//*----------------------------------------------------------------------------
void AT91F_UndefHandler()
{
	AT91F_DBGU_Printk("-F- Undef detected\n\r");
	while (1);
}

//*----------------------------------------------------------------------------
//* \fn    AT91F_LowLevelInit
//* \brief This function performs very low level HW initialization
//*----------------------------------------------------------------------------
#pragma location="ICODE"
__interwork int __low_level_init(void)
{
	volatile unsigned char 	status;
  // Enable IOB clock
  AT91F_PIOB_CfgPMC();
  AT91F_PIO_CfgOutput(AT91C_BASE_PIOB,USR_LED1 | USR_LED2 | USR_LED3);
  AT91F_PIO_SetOutput(AT91C_BASE_PIOB,USR_LED1 | USR_LED2 | USR_LED3);

  USR_LED1_ON();

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Step 1. Flash AT49BV6416 Initialisation : must be done before change the clocks -> Set correct Wait States
/////////////////////////////////////////////////////////////////////////////////////////////////////

	AT91F_InitFlash();

  /////////////////////////////////////////////////////////////////////////////////////////////////////
// Step 2.
/////////////////////////////////////////////////////////////////////////////////////////////////////

	status = AT91F_InitClocks(PLLAR,PLLBR,MCKR);


/////////////////////////////////////////////////////////////////////////////////////////////////////
// Step 3. SDRAM Initialisation
/////////////////////////////////////////////////////////////////////////////////////////////////////

	AT91F_InitSDRAM();
	

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Step 4. Other Initialisation
/////////////////////////////////////////////////////////////////////////////////////////////////////
	// Open PIO for DBGU
	AT91F_DBGU_CfgPIO();

  // 1sec
  AT91C_BASE_ST->ST_RTMR = 0x8000;

	// Configure DBGU
	AT91F_US_Configure (
		(AT91PS_USART) AT91C_BASE_DBGU,             // DBGU base address
		60000000,                  							    // 60 MHz
		AT91C_US_CHMODE_NORMAL | AT91C_US_PAR_NONE ,// mode Register to be programmed
		115200 ,                   							    // baudrate to be programmed
		0);                        							    // timeguard to be programmed

	// Enable Transmitter
	AT91F_US_EnableTx((AT91PS_USART) AT91C_BASE_DBGU);

	AT91F_DBGU_Printk("\n\rAT91F_LowLevelInit(): Debug channel initialized\n\r");
	AT91F_DBGU_Printk("\n\r\n\rAT91F_LowLevelInit() Complete!\n\r");

  return 1;
}


