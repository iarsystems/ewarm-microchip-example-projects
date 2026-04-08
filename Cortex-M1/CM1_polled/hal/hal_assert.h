/*******************************************************************************
 * (c) Copyright 2008 Actel Corporation.  All rights reserved.
 * 
 * SVN $Revision: 5 $
 * SVN $Date: 2013-12-23 13:52:47 +0100 (mån, 23 dec 2013) $
 */
#ifndef HAL_ASSERT_HEADER
#define HAL_ASSERT_HEADER

#if defined(NDEBUG)
/***************************************************************************//**
 * HAL_ASSERT() is defined out when the NDEBUG symbol is used.
 ******************************************************************************/
#define HAL_ASSERT(CHECK)

#else
/***************************************************************************//**
 * Default behaviour for HAL_ASSERT() macro:
 *------------------------------------------------------------------------------
 * In this mode, an assertion will block the programs execution until a watchdog
 * resets the system.
 ******************************************************************************/

void HAL_assert_fail
(
    const uint8_t * file_name,
    uint32_t line_no
);

#define HAL_ASSERT(CHECK)  \
    do { \
        if (!(CHECK)) \
        { \
            uint8_t file_name[] = __FILE__; \
            HAL_assert_fail(file_name, __LINE__); \
        } \
    } while (0);


#endif  /* NDEBUG */

#endif  /* HAL_ASSERT_HEADER */
