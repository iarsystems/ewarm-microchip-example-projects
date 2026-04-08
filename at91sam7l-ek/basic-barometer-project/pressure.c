/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support 
 * ----------------------------------------------------------------------------
 * Copyright (c) 2008, Atmel Corporation
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Atmel's name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ----------------------------------------------------------------------------
 */

#undef TRACE_LEVEL
#define TRACE_LEVEL 0

//------------------------------------------------------------------------------
//         Headers
//------------------------------------------------------------------------------

#include "pressure.h"
#include <utility/trace.h>
#include <utility/assert.h>

//------------------------------------------------------------------------------
//         Local definitions
//------------------------------------------------------------------------------

/// Standard pressure at sea level (in 0.1 mbar).
#define STANDARD_PRESSURE           10132

/// Number of intervals.
#define NUM_INTERVALS               23

//------------------------------------------------------------------------------
//         Local type
//------------------------------------------------------------------------------

/// Models one interval of the altitude/meter calculation function.
typedef struct _Interval {

    /// Lower pressure bound of interval (in 0.1 mbar).
    unsigned short pLower;
    /// Upper pressure bound of interval (in 0.1 mbar).
    unsigned short pUpper;
    /// Iteration coefficient i.
    unsigned short i;
    /// Iteration coefficient j.
    unsigned short j;

} Interval;

//------------------------------------------------------------------------------
//         Local variables
//------------------------------------------------------------------------------

/// List of intervals to model the altitude/pressure function.
static const Interval pIntervals[NUM_INTERVALS] = {

    { 1000,  1130, 12256, 16212},
    { 1000,  1130, 12256, 16212}, 
    { 1130,  1300, 10758, 15434}, 
    { 1300,  1500,  9329, 14541},
    { 1500,  1730,  8085, 13630},
    { 1730,  2000,  7001, 12722},
    { 2000,  2300,  6069, 11799},
    { 2300,  2650,  5360, 10910},
    { 2650,  3000,  4816,  9994}, 
    { 3000,  3350,  4371,  9171}, 
    { 3350,  3700,  4020,  8424}, 
    { 3700,  4100,  3702,  7737}, 
    { 4100,  4500,  3420,  7014}, 
    { 4500,  5000,  3158,  6346}, 
    { 5000,  5500,  2908,  5575}, 
    { 5500,  6000,  2699,  4865}, 
    { 6000,  6500,  2523,  4206}, 
    { 6500,  7100,  2359,  3590}, 
    { 7100,  7800,  2188,  2899}, 
    { 7800,  8500,  2033,  2151}, 
    { 8500,  9200,  1905,  1456}, 
    { 9200,  9700,  1802,   805}, 
    { 9700, 10300,  1720,   365}
};

//------------------------------------------------------------------------------
//         Global functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Calculates and returns the current pressure at sea level given the current
/// pressure and altitude.
/// \param altitude  Current altitude in meters.
/// \param pressure  Pressure at current altitude.
//------------------------------------------------------------------------------
unsigned short CalcPressureAt0Meters(
    unsigned short altitude,
    unsigned short pressure)
{
    unsigned short pStandard;
    unsigned short pSea;
    unsigned char found;
    unsigned int i;

    TRACE_DEBUG("CalcPressureAt0Meters(%d, %d)\n\r", altitude, pressure);

    // Calculate standard pressure at current altitude
    TRACE_DEBUG("Calculating standard pressure at %dm\n\r", altitude);

    // Find interval containing standard pressure at sea level
    i = 0;
    found = 0;
    while (!found && (i < NUM_INTERVALS)) {
    
        if ((STANDARD_PRESSURE >= pIntervals[i].pLower)
            && (STANDARD_PRESSURE <= pIntervals[i].pUpper)) {
    
            found = 1;
        }
        else {
    
            i++;
        }
    }
    
    ASSERT(found, "-F- CalcPressureAt0Meters: Could not find interval for standard pressure at sea level.\n\r");
    TRACE_DEBUG("%d is in [%d; %d]\n\r", STANDARD_PRESSURE,
              pIntervals[i].pLower, pIntervals[i].pUpper);
    
    // Extrapolate standard pressure at current altitude
    found = 0;
    while (!found) {
    
        SANITY_CHECK(i < NUM_INTERVALS);
    
        // Calculate pressure
        pStandard = (unsigned int) (pIntervals[i].j - altitude) * 2048 / pIntervals[i].i + pIntervals[i].pLower;
    
        // Check if pressure is still in interval
        if (pStandard < pIntervals[i].pLower) {
    
            i--;
        }
        else if (pStandard > pIntervals[i].pUpper) {
    
            i++;
        }
        else {
    
            found = 1;
        }
    }
    TRACE_DEBUG("Standard pressure at %dm is %d.%d\n\r",
              altitude, pStandard / 10, pStandard % 10);
    
    // Calculate ratio between standard pressure and actual pressure to infer
    // current pressure at sea level
    pSea = (unsigned int) (pressure * STANDARD_PRESSURE) / pStandard;
    
    TRACE_DEBUG("Current pressure at sea level is %d.%d\n\r",
              pSea / 10, pSea % 10);
    
    return pSea;
}
