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
//* Creation            : ODi   06/26/2002
//* Modify              : SBb   10/06/2005
//*
//*----------------------------------------------------------------------------
#include <stdio.h>
#include "AT91RM9200.h"
#include "mmu.h"
#include "init.h"

extern void AT91F_DBGU_Printk(char *);
extern void testloop(int);

//*----------------------------------------------------------------------------
//* \fn    AT91F_Dhrystone
//* \brief Count how many dhrystone have been executed in 1 sec
//*----------------------------------------------------------------------------
void AT91F_Dhrystone()
{
	unsigned int startTime, cnt, i;
	char msg[100];
	// Synchronize
	startTime = AT91C_BASE_ST->ST_CRTR;
	while (startTime == AT91C_BASE_ST->ST_CRTR);
	
	for (i = 0; i < 4; ++i) {
		startTime = AT91C_BASE_ST->ST_CRTR;
		cnt = 0;
		while (startTime == AT91C_BASE_ST->ST_CRTR) {
			testloop(1);
			++cnt;
		}
		sprintf(msg, "-I- %d Dhrystone per second\n\r", cnt);
		AT91F_DBGU_Printk(msg);
	}
}

//*----------------------------------------------------------------------------
//* \fn    AT91F_InitMMU
//* \brief Initialize MMU
//*----------------------------------------------------------------------------
void AT91F_InitMMU(unsigned int *pTranslationTable)
{
	int i;
	// Program the TTB
	AT91F_ARM_WriteTTB((unsigned int) pTranslationTable);
	// Program the domain access register
	AT91F_ARM_WriteDomain(0xC0000000); // domain 15: access are not checked

	// Reset table entries
	for (i = 0; i < 4096; ++i)
		pTranslationTable[i] = 0;
	// Program level 1 page table entry
	pTranslationTable[0x0] =
		(0x0 << 0) |     // Physical Address
		(1 << 10) |      // Access in supervisor mode
		(15 << 5) |      // Domain
		1 << 4 |
		0x2;             // Set as 1 Mbyte section
	pTranslationTable[0x2] =
		(0x2 << 0) |     // Physical Address
		(1 << 10) |      // Access in supervisor mode
		(15 << 5) |      // Domain
		1 << 4 |
		0x2;             // Set as 1 Mbyte section
	pTranslationTable[0x100] =
		(0x100 << 20) |  // Physical Address
		(1 << 10) |      // Access in supervisor mode
		(15 << 5) |      // Domain
		1 << 4 |
		(1 << 3) |       // Cachable
		0x2;             // Set as 1 Mbyte section
	pTranslationTable[0x200] =
		(0x200 << 20) |  // Physical Address
		(1 << 10) |      // Access in supervisor mode
		(15 << 5) |      // Domain
		1 << 4 |
		0x2;             // Set as 1 Mbyte section
	pTranslationTable[0x500] =
		(0x100 << 20) |  // Physical Address
		(1 << 10) |      // Access in supervisor mode
		(15 << 5) |      // Domain
		1 << 4 |
		(1 << 3) |       // Cachable
		0x2;             // Set as 1 Mbyte section
	pTranslationTable[0xFFF] =
		(0xFFF << 20) |  // Physical Address
		(1 << 10) |      // Access in supervisor mode
		(15 << 5) |      // Domain
		1 << 4 |
		0x2;             // Set as 1 Mbyte section

	// Enable the MMU
	AT91F_EnableMMU();
}


int main()
{
  *(int *)0x200000 = 0xAAAA5555;
  if(*(int *)0 != 0xAAAA5555)
  {
    AT91C_BASE_MC->MC_RCR ^= 1;
  }

  void (*pDhrystone)(void) = (void (*)()) ((unsigned int) AT91F_Dhrystone + 0x40000000);
	AT91F_DBGU_Printk("-I- Enter in main() section\n\r");

	AT91F_DBGU_Printk("-I- WO MMU & I+D Caches Disabled\n\r");
	AT91F_DisableICache();
	AT91F_DisableDCache();
	AT91F_Dhrystone();

	AT91F_DBGU_Printk("\n\r-I- WO MMU & I Cache Enabled\n\r");
	AT91F_EnableICache();
	AT91F_Dhrystone();

  AT91F_InitMMU((unsigned int *) 0x20010000);

	AT91F_DBGU_Printk("\n\r-I- MMU & I Cache Enabled\n\r");
	AT91F_EnableICache();
	pDhrystone();

	AT91F_DBGU_Printk("\n\r-I- MMU & D Cache Enabled\n\r");
	AT91F_EnableDCache();
	pDhrystone();

	AT91F_DisableICache();
	AT91F_DisableDCache();
  AT91F_DisableMMU();

  AT91F_DBGU_Printk("\n\rEnd of DHRYSTONE Benchmark!\n\r");
	while (1);
}
