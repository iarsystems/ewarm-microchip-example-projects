// ****************************************************************************
// CP15 Register 0
// 	Read: ID code | cache type
//	Write: Unpredictable

//*----------------------------------------------------------------------------
//* \fn    AT91F_ARM_ReadIDCode
//* \brief Read ID code register
//*----------------------------------------------------------------------------
inline unsigned int AT91F_ARM_ReadIDCode()
{
	register unsigned int id;
 	asm("MRC p15, 0, id, c0, c0, 0");
 	return id;
}

//*----------------------------------------------------------------------------
//* \fn    AT91F_ARM_ReadCacheType
//* \brief Read cache type
//*----------------------------------------------------------------------------
inline unsigned int AT91F_ARM_ReadCacheType()
{
	register unsigned int type;
 	asm("MRC p15, 0, type, c0, c0, 1");
 	return type;
}

// ****************************************************************************
// CP15 Register 1
// 	Read: Control
//	Write: Control

//*----------------------------------------------------------------------------
//* \fn    AT91F_ARM_ReadControl
//* \brief Read Control register
//*----------------------------------------------------------------------------
inline unsigned int AT91F_ARM_ReadControl()
{
	register unsigned int ctl;
 	asm("MRC p15, 0, r0, c1, c0, 0");
 	return ctl;
}

//*----------------------------------------------------------------------------
//* \fn    AT91F_ARM_WriteControl
//* \brief Write Control register
//*----------------------------------------------------------------------------
inline void AT91F_ARM_WriteControl(unsigned int ctl)
{
 	asm("MCR  p15,0,r0,c1,c0,0");
}

// ****************************************************************************
// CP15 Register 2
// 	Read: Translation table Base
//	Write: Translation table Base

//*----------------------------------------------------------------------------
//* \fn    AT91F_ARM_ReadTTB
//* \brief Read Translation table base register
//*----------------------------------------------------------------------------
inline unsigned int AT91F_ARM_ReadTTB()
{
	register unsigned int ttb;
 	asm("MRC p15, 0, ttb, c2, c0, 0");
 	return ttb;
}

//*----------------------------------------------------------------------------
//* \fn    AT91F_ARM_WriteTTB
//* \brief Write Translation table base  register
//*----------------------------------------------------------------------------
inline void AT91F_ARM_WriteTTB(unsigned int ttb)
{
  ttb &= 0xFFFFC000;
 	asm("MCR p15, 0,r0, c2, c0, 0");
}

// ****************************************************************************
// CP15 Register 3
// 	Read: Read domain access control
//	Write: Write domain access control

//*----------------------------------------------------------------------------
//* \fn    AT91F_ARM_ReadDomain
//* \brief Read domain access control
//*----------------------------------------------------------------------------
inline unsigned int AT91F_ARM_ReadDomain()
{
	register unsigned int domain;
 	asm("MRC p15, 0, domain, c3, c0, 0");
 	return domain;
}

//*----------------------------------------------------------------------------
//* \fn    AT91F_ARM_WriteDomain
//* \brief Write domain access control
//*----------------------------------------------------------------------------
inline void AT91F_ARM_WriteDomain(unsigned int domain)
{
 	asm("MCR p15, 0, r0, c3, c0, 0");
}

// ****************************************************************************
// CP15 Register 5
// 	Read: Read Fault Status
//	Write: Write Fault Status

//*----------------------------------------------------------------------------
//* \fn    AT91F_ARM_ReadDataFSR
//* \brief Read data FSR value
//*----------------------------------------------------------------------------
inline unsigned int AT91F_ARM_ReadDataFSR()
{
	register unsigned int dataFSR;
 	asm("MRC p15, 0, dataFSR, c5, c0, 0");
 	return dataFSR;
}

//*----------------------------------------------------------------------------
//* \fn    AT91F_ARM_WriteDataFSR
//* \brief Write data FSR value
//*----------------------------------------------------------------------------
inline void AT91F_ARM_WriteDataFSR(unsigned int dataFSR)
{
 	asm("MCR p15, 0, dataFSR, c5, c0, 0");
}

//*----------------------------------------------------------------------------
//* \fn    AT91F_ARM_ReadPrefetchFSR
//* \brief Read prefetch FSR value
//*----------------------------------------------------------------------------
inline unsigned int AT91F_ARM_ReadPrefetchFSR()
{
	register unsigned int prefetchFSR;
 	asm("MRC p15, 0, prefetchFSR, c5, c0, 1");
 	return prefetchFSR;
}

//*----------------------------------------------------------------------------
//* \fn    AT91F_ARM_WritePrefetchFSR
//* \brief Write prefetch FSR value
//*----------------------------------------------------------------------------
inline void AT91F_ARM_WritePrefetchFSR(unsigned int prefetchFSR)
{
 	asm("MCR p15, 0, prefetchFSR, c5, c0, 1");
}

// ****************************************************************************
// CP15 Register 6
// 	Read: Read Fault Address
//	Write: Write Fault Address

//*----------------------------------------------------------------------------
//* \fn    AT91F_ARM_ReadFAR
//* \brief Read FAR data
//*----------------------------------------------------------------------------
inline unsigned int AT91F_ARM_ReadFAR()
{
	register unsigned int dataFAR;
 	asm("MRC p15, 0, dataFAR, c6, c0, 0");
 	return dataFAR;
}

//*----------------------------------------------------------------------------
//* \fn    AT91F_ARM_WriteFAR
//* \brief Write FAR data
//*----------------------------------------------------------------------------
inline void AT91F_ARM_WriteFAR(unsigned int dataFAR)
{
 	asm("MCR p15, 0, dataFAR, c6, c0, 0");
}

// ****************************************************************************
// CP15 Register 7
// 	Read: Unpredictable
//	Write: Cache operations

//*----------------------------------------------------------------------------
//* \fn    AT91F_ARM_InvalidateIDCache
//* \brief Invalidate ICache and DCache
//*----------------------------------------------------------------------------
inline void AT91F_ARM_InvalidateIDCache()
{
	register unsigned int sbz = 0;
 	asm("MCR p15, 0, sbz, c7, c7, 0");
}

//*----------------------------------------------------------------------------
//* \fn    AT91F_ARM_InvalidateICache
//* \brief Invalidate ICache
//*----------------------------------------------------------------------------
inline void AT91F_ARM_InvalidateICache()
{
	register unsigned int sbz = 0;
 	asm("MCR p15, 0, r0, c7, c5, 0");
}

//*----------------------------------------------------------------------------
//* \fn    AT91F_ARM_InvalidateICacheMVA
//* \brief Invalidate ICache single entry (using MVA)
//*----------------------------------------------------------------------------
inline void AT91F_ARM_InvalidateICacheMVA(unsigned int mva)
{
 	asm("MCR p15, 0, (mva & 0xFFFFFFE0), c7, c5, 1");
}

//*----------------------------------------------------------------------------
//* \fn    AT91F_ARM_PrefetchICacheLine
//* \brief Prefetch ICache line (using MVA)
//*----------------------------------------------------------------------------
inline void AT91F_ARM_PrefetchICacheLine(unsigned int mva)
{
  mva &= 0xFFFFFFE0;
 	asm("MCR p15, 0, r0 , c7, c13, 1");
}

//*----------------------------------------------------------------------------
//* \fn    AT91F_ARM_InvalidateDCache
//* \brief Invalidate DCache
//*----------------------------------------------------------------------------
inline void AT91F_ARM_InvalidateDCache()
{
	register unsigned int sbz = 0;
 	asm("MCR p15, 0, r0, c7, c6, 0");
}

//*----------------------------------------------------------------------------
//* \fn    AT91F_ARM_InvalidateDCacheMVA
//* \brief Invalidate DCache single entry (using MVA)
//*----------------------------------------------------------------------------
inline void AT91F_ARM_InvalidateDCacheMVA(unsigned int mva)
{
 	asm("MCR p15, 0, (mva & 0xFFFFFFE0), c7, c6, 1");
}

//*----------------------------------------------------------------------------
//* \fn    AT91F_ARM_CleanDCacheMVA
//* \brief Clean DCache single entry (using MVA)
//*----------------------------------------------------------------------------
inline void AT91F_ARM_CleanDCacheMVA(unsigned int mva)
{
 	asm("MCR p15, 0, (mva & 0xFFFFFFE0), c7, c10, 1");
}

//*----------------------------------------------------------------------------
//* \fn    AT91F_ARM_CleanInvalidateDCacheMVA
//* \brief Clean and Invalidate DCache single entry (using MVA)
//*----------------------------------------------------------------------------
inline void AT91F_ARM_CleanInvalidateDCacheMVA(unsigned int mva)
{
 	asm("MCR p15, 0, (mva & 0xFFFFFFE0), c7, c14, 1");
}

//*----------------------------------------------------------------------------
//* \fn    AT91F_ARM_CleanDCacheIDX
//* \brief Clean DCache single entry (using index)
//*----------------------------------------------------------------------------
inline void AT91F_ARM_CleanDCacheIDX(unsigned int index)
{
  index &= 0xFC0000E0;
 	asm("MCR p15, 0, r0, c7, c10, 2");
}

//*----------------------------------------------------------------------------
//* \fn    AT91F_ARM_CleanInvalidateDCacheIDX
//* \brief Clean and Invalidate DCache single entry (using index)
//*----------------------------------------------------------------------------
inline void AT91F_ARM_CleanInvalidateDCacheIDX(unsigned int index)
{
 	asm("MCR p15, 0, (index & 0xFC0000E0), c7, c14, 2");
}

//*----------------------------------------------------------------------------
//* \fn    AT91F_ARM_DrainWriteBuffer
//* \brief Drain Write Buffer
//*----------------------------------------------------------------------------
inline void AT91F_ARM_DrainWriteBuffer()
{
	register unsigned int sbz = 0;
 	asm("MCR p15, 0, r0, c7, c10, 4");
}

//*----------------------------------------------------------------------------
//* \fn    AT91F_ARM_WaitForInterrupt
//* \brief Wait for interrupt
//*----------------------------------------------------------------------------
inline void AT91F_ARM_WaitForInterrupt()
{
	register unsigned int sbz = 0;
	asm("MCR p15, 0, sbz, c7, c0, 4");
}

// ****************************************************************************
// CP15 Register 8
// 	Read: Unpredictable
//	Write: TLB operations

//*----------------------------------------------------------------------------
//* \fn    AT91F_ARM_InvalidateIDTLB
//* \brief Invalidate TLB(s)
//*----------------------------------------------------------------------------
inline void AT91F_ARM_InvalidateIDTLB()
{
	register unsigned int sbz = 0;
 	asm("MCR p15, 0, sbz, c8, c7, 0");
}

//*----------------------------------------------------------------------------
//* \fn    AT91F_ARM_InvalidateITLB
//* \brief Invalidate I TLB
//*----------------------------------------------------------------------------
inline void AT91F_ARM_InvalidateITLB()
{
	register unsigned int sbz = 0;
 	asm("MCR p15, 0, r0, c8, c5, 0");
}

//*----------------------------------------------------------------------------
//* \fn    AT91F_ARM_InvalidateITLBMVA
//* \brief Invalidate I TLB single entry (using MVA)
//*----------------------------------------------------------------------------
inline void AT91F_ARM_InvalidateITLBMVA(unsigned int mva)
{
 	asm("MCR p15, 0, (mva & 0xFFFFFE00), c8, c5, 1");
}

//*----------------------------------------------------------------------------
//* \fn    AT91F_ARM_InvalidateDTLB
//* \brief Invalidate D TLB
//*----------------------------------------------------------------------------
inline void AT91F_ARM_InvalidateDTLB()
{
	register unsigned int sbz = 0;
 	asm("MCR p15, 0, r0, c8, c6, 0");
}

//*----------------------------------------------------------------------------
//* \fn    AT91F_ARM_InvalidateDTLBMVA
//* \brief Invalidate D TLB single entry (using MVA)
//*----------------------------------------------------------------------------
inline void AT91F_ARM_InvalidateDTLBMVA(unsigned int mva)
{
 	asm("MCR p15, 0, (mva & 0xFFFFFE00), c8, c6, 1");
}


// ****************************************************************************
// CP15 Register 9
// 	Read: Cache lockdown
//	Write: Cache lockdown

//*----------------------------------------------------------------------------
//* \fn    AT91F_ARM_ReadDCacheLockdown
//* \brief Read D Cache lockdown
//*----------------------------------------------------------------------------
inline unsigned int AT91F_ARM_ReadDCacheLockdown()
{
	register unsigned int index;
 	asm("MRC p15, 0, index, c9, c0, 0");
    return index;
}

//*----------------------------------------------------------------------------
//* \fn    AT91F_ARM_WriteDCacheLockdown
//* \brief Write D Cache lockdown
//*----------------------------------------------------------------------------
inline void AT91F_ARM_WriteDCacheLockdown(
	unsigned int index)
{
	asm("MCR p15, 0, index, c9, c0, 0");
}

//*----------------------------------------------------------------------------
//* \fn    AT91F_ARM_ReadICacheLockdown
//* \brief Read I Cache lockdown
//*----------------------------------------------------------------------------
inline unsigned int AT91F_ARM_ReadICacheLockdown()
{
	register unsigned int index;
	asm("MRC p15, 0, index, c9, c0, 1");
	return index;
}

//*----------------------------------------------------------------------------
//* \fn    AT91F_ARM_WriteICacheLockdown
//* \brief Write I Cache lockdown
//*----------------------------------------------------------------------------
inline void AT91F_ARM_WriteICacheLockdown(
	unsigned int index)
{
	asm("MCR p15, 0, r0, c9, c0, 1");
}

// ****************************************************************************
// CP15 Register 10
// 	Read: TLB lockdown
//	Write: TLB lockdown

//*----------------------------------------------------------------------------
//* \fn    AT91F_ARM_ReadDTLBLockdown
//* \brief Read D TLB lockdown
//*----------------------------------------------------------------------------
inline unsigned int AT91F_ARM_ReadDTLBLockdown()
{
	register unsigned int lockdown;
 	asm("MRC p15, 0, lockdown, c10, c0, 0");
    return lockdown;
}

//*----------------------------------------------------------------------------
//* \fn    AT91F_ARM_WriteDTLBLockdown
//* \brief Write D TLB lockdown
//*----------------------------------------------------------------------------
inline void AT91F_ARM_WriteDTLBLockdown(
	unsigned int lockdown)
{
	asm("MCR p15, 0, lockdown, c10, c0, 0");
}

//*----------------------------------------------------------------------------
//* \fn    AT91F_ARM_ReadITLBLockdown
//* \brief Read I TLB lockdown
//*----------------------------------------------------------------------------
inline unsigned int AT91F_ARM_ReadITLBLockdown()
{
	register unsigned int lockdown;
	asm("MRC p15, 0, lockdown, c10, c0, 1");
	return lockdown;
}

//*----------------------------------------------------------------------------
//* \fn    AT91F_ARM_WriteITLBLockdown
//* \brief Write I TLB lockdown
//*----------------------------------------------------------------------------
inline void AT91F_ARM_WriteITLBLockdown(
	unsigned int lockdown)
{
//	asm("MCR p15, 0, lockdown, c10, c0, 1");
}

// ****************************************************************************
// CP15 Register 13
// 	Read: Read FCSE PID
//	Write: Write FCSE PID

//*----------------------------------------------------------------------------
//* \fn    AT91F_ARM_ReadFCSEPID
//* \brief Read FCSE PID
//*----------------------------------------------------------------------------
inline unsigned int AT91F_ARM_ReadFCSEPID()
{
	register unsigned int pid;
 	asm("MRC p15, 0, pid, c13, c0, 0");
 	return pid;
}

//*----------------------------------------------------------------------------
//* \fn    AT91F_ARM_WriteFCSEPID
//* \brief Write FCSE PID
//*----------------------------------------------------------------------------
inline void AT91F_ARM_WriteFCSEPID(unsigned int pid)
{
 	asm("MCR p15, 0, (pid & 0xFF000000), c13, c0, 0");
}

