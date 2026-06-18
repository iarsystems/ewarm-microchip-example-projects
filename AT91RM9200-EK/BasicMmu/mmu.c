//*----------------------------------------------------------------------------
//*         ATMEL Microcontroller Software Support  -  ROUSSET  -
//*----------------------------------------------------------------------------
//* The software is delivered "AS IS" without warranty or condition of any
//* kind, either express, implied or statutory. This includes without
//* limitation any warranty or condition with respect to merchantability or
//* fitness for any particular purpose, or against the infringements of
//* intellectual property rights of others.
//*----------------------------------------------------------------------------
//* File Name           : mmu.c
//* Object              : Common MMU and CACHE operations
//* Creation            : ODi   Aug 22nd 02
//* Modify              : SBb   10/06/2005
//*
//*----------------------------------------------------------------------------
#include "lib_arm920t.h"

//*----------------------------------------------------------------------------
//* \fn    AT91F_CleanDCache
//* \brief Clean and invalidate D Cache
//*----------------------------------------------------------------------------
void AT91F_CleanDCache(void)
{
	register char seg, index;
	for (seg = 0; seg < 8; ++seg) {
		for (index = 0; index < 64; ++index) {
			AT91F_ARM_CleanDCacheIDX((index << 26) | (seg << 5));
		}
	}
}

//*----------------------------------------------------------------------------
//* \fn    AT91F_ResetICache
//* \brief Reset I Cache (Should be run from a non cachable area)
//*----------------------------------------------------------------------------
void AT91F_ResetICache(void)
{
	// Flush I TLB
	AT91F_ARM_InvalidateITLB();
	// Flush I cache
	AT91F_ARM_InvalidateICache();
}

//*----------------------------------------------------------------------------
//* \fn    AT91F_ResetDCache
//* \brief Reset D Cache (Should be run from a non cachable area)
//*----------------------------------------------------------------------------
void AT91F_ResetDCache(void)
{
	// Achieve pending write operations
	AT91F_CleanDCache();
	// Flush write buffers
	AT91F_ARM_DrainWriteBuffer();
	// Flush D TLB
	AT91F_ARM_InvalidateDTLB();
	// Flush D cache
	AT91F_ARM_InvalidateDCache();
}

//*----------------------------------------------------------------------------
//* \fn    AT91F_EnableMMU
//* \brief Enable MMU
//*----------------------------------------------------------------------------
void AT91F_EnableMMU(void)
{
	register unsigned int ctl;

	ctl = AT91F_ARM_ReadControl();
	ctl |= (1 << 0);
	AT91F_ARM_WriteControl(ctl);
}

//*----------------------------------------------------------------------------
//* \fn    AT91F_DisableMMU
//* \brief Disable MMU
//*----------------------------------------------------------------------------
void AT91F_DisableMMU(void)
{
	unsigned int ctl;

	ctl = AT91F_ARM_ReadControl();
	ctl &= ~(1 << 0);
	AT91F_ARM_WriteControl(ctl);
}


//*----------------------------------------------------------------------------
//* \fn    AT91F_EnableICache
//* \brief Enable I Cache
//*----------------------------------------------------------------------------
void AT91F_EnableICache(void)
{
	unsigned int ctl;

	ctl = AT91F_ARM_ReadControl();
	ctl |= (1 << 12);
	AT91F_ARM_WriteControl(ctl);
}

//*----------------------------------------------------------------------------
//* \fn    AT91F_DisableICache
//* \brief Disable I Cache
//*----------------------------------------------------------------------------
void AT91F_DisableICache(void)
{
	unsigned int ctl;

	ctl = AT91F_ARM_ReadControl();
	ctl &= ~(1 << 12);
	AT91F_ARM_WriteControl(ctl);
}

//*----------------------------------------------------------------------------
//* \fn    AT91F_EnableDCache
//* \brief Enable D Cache
//*----------------------------------------------------------------------------
void AT91F_EnableDCache(void)
{
	unsigned int ctl;

	ctl = AT91F_ARM_ReadControl();
	ctl |= (1 << 2);
	AT91F_ARM_WriteControl(ctl);
}

//*----------------------------------------------------------------------------
//* \fn    AT91F_DisableDCache
//* \brief Disable D Cache
//*----------------------------------------------------------------------------
void AT91F_DisableDCache(void)
{
	unsigned int ctl;

	ctl = AT91F_ARM_ReadControl();
	ctl &= ~(1 << 2);
	AT91F_ARM_WriteControl(ctl);
}

//*----------------------------------------------------------------------------
//* \fn    AT91F_LockITLB
//* \brief Lock one I TLB entry after entries previously locked
//*----------------------------------------------------------------------------
void AT91F_LockITLB(unsigned int address)
{
	// Set the P bit
	AT91F_ARM_WriteITLBLockdown( 1); // base 0 victim 0 P 1
	// Prefetch the instruction assuming an ITLB miss occurs, the entry base is loaded
	AT91F_ARM_PrefetchICacheLine(address);
	// Update base and victim values
	AT91F_ARM_WriteITLBLockdown((1 << 26) | (1 << 20)); // base 1 victim 1 P 0
}

//*----------------------------------------------------------------------------
//* \fn    AT91F_ARM_WriteITLBLockdown
//* \brief Write I TLB lockdown  (Should be run from a non cachable area)
//*----------------------------------------------------------------------------
void AT91F_LockICache(unsigned int startAddress, unsigned int size)
{
	unsigned int victim = 0;

	AT91F_ARM_InvalidateICache();
	AT91F_ARM_WriteICacheLockdown((victim++) << 26);
	while (size) {
		// Prefetch ICache line
		AT91F_ARM_PrefetchICacheLine(startAddress);
		startAddress += 32;
		// Test for segment 0, and if so increment victim pointer
		if ( !(startAddress & 0xE0) )
			AT91F_ARM_WriteICacheLockdown((victim++) << 26);
		size -= 32;

	}
	// If segment != 0 the increment victim pointer
	if ( (startAddress & 0xE0) )
		AT91F_ARM_WriteICacheLockdown(victim << 26);
}
