//*----------------------------------------------------------------------------
//*         ATMEL Microcontroller Software Support  -  ROUSSET  -
//*----------------------------------------------------------------------------
//* The software is delivered "AS IS" without warranty or condition of any
//* kind, either express, implied or statutory. This includes without
//* limitation any warranty or condition with respect to merchantability or
//* fitness for any particular purpose, or against the infringements of
//* intellectual property rights of others.
//*----------------------------------------------------------------------------
//* File Name           : mmu.h
//* Object              : Common MMU and CACHE operations
//* Creation            : ODi   Aug 22nd 02
//*
//*----------------------------------------------------------------------------
#ifndef MMU_H
#define MMU_H

#include "lib_arm920t.h"

extern void AT91F_CleanDCache(void); // Clean and invalidate D Cache
extern void AT91F_ResetICache(void); // Reset I Cache (Should be run from a non cachable area)
extern void AT91F_ResetDCache(void); // Reset D Cache (Should be run from a non cachable area)
extern void AT91F_EnableMMU(void); // Enable MMU
extern void AT91F_DisableMMU(void); // Disable MMU
extern void AT91F_EnableICache(void); // Enable I Cache
extern void AT91F_DisableICache(void); // Disable I Cache
extern void AT91F_EnableDCache(void); // Enable D Cache
extern void AT91F_DisableDCache(void); // Disable D Cache
extern void AT91F_LockITLB(unsigned int address); // Lock one I TLB entry after entries previously locked
extern void AT91F_LockICache(unsigned int startAddress, unsigned int size); // Write I TLB lockdown  (Should be run from a non cachable area)

#endif // MMU_H
