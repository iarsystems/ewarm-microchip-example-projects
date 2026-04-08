/*****************************************************************************
* © 2014 Microchip Technology Inc. and its subsidiaries.
* You may use this software and any derivatives exclusively with
* Microchip products.
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".
* NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
* INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
* AND FITNESS FOR A PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP
* PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.
* TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL
* CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF
* FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
* MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE
* OF THESE TERMS.
*****************************************************************************/

/** @file  mchp.h
 ******************************************************************************
 *  mchp.h  This is the main include file for ARM-based Sample
 **************************************************************************************
 */

/*  Revision Information     $Revision: #1
 *                           $DateTime: 2013/02/06
 *                           $Author: bg
 *  Change Description:      Initial Revision
 **********************************************************************************

*-----------------------------------------------------------------------------
 * Macros for accessing registers
 *---------------------------------------------------------------------------*/

#define REG8( x)            (*(volatile unsigned char  *)((unsigned long)(x)))
#define REG16(x)            (*(volatile unsigned short *)((unsigned long)(x)))
#define REG32(x)            (*(volatile unsigned long  *)((unsigned long)(x)))

// portable data types
typedef unsigned char       UINT8;
typedef unsigned short      UINT16;
typedef unsigned long       UINT32;

/*-----------------------------------------------------------------------------
 * Memory-mapped register address
 *---------------------------------------------------------------------------*/
/*   +---------------------------------+
     |  Timer0                         |
     +---------------------------------+                */
#define TIMER_BASE          ( 0x40000C00UL )
#define TIMER_COUNT         ( TIMER_BASE + 0x00 )
#define TIMER_PRELOAD       ( TIMER_BASE + 0x04 )
#define TIMER_STATUS        ( TIMER_BASE + 0x08 )
#define TIMER_INTEN         ( TIMER_BASE + 0x0C )
#define TIMER_CONTROL       ( TIMER_BASE + 0x10 )

/*   +---------------------------------+
     |  IRQs                           |
     +---------------------------------+                */
#define IRQ_BASE            ( 0x4000C000UL )
#define IRQ23_SOURCE        ( IRQ_BASE + 0x12C )
#define IRQ23_ENABLE        ( IRQ_BASE + 0x130 )
#define IRQ23_RESULT        ( IRQ_BASE + 0x134 )
#define IRQ23_ENABLE_CLEAR  ( IRQ_BASE + 0x138 )
#define IRQ23_BLOCK_EN      ( IRQ_BASE + 0x200 )


/*   +---------------------------------+
     |  EC Only Registers              |
     +---------------------------------+                */
#define EC_REG_BASE         ( 0x4000FC00UL )
#define EC_INT_CNTL         ( EC_REG_BASE + 0x18)

/*   +---------------------------------+
     |  GPIOs                          |
     +---------------------------------+                */
#define GPIO_BASE           ( 0x40081000UL )
#define GPIO_PIN_CONTROL(x) ( GPIO_BASE + (x<<2) )

#define GPIO_OUTPUT3        ( GPIO_BASE + 0x28C )

#define GPIO_MUX_BITPOS         (12)
#define GPIO_DIR_BITPOS         (9)
#define GPIO_BUFFTYPE_BITPOS    (8)
#define GPIO_OUTPUT_BITPOS      (16)
#define GPIO_MUX_GPIO           (0x00UL << (GPIO_MUX_BITPOS))
#define GPIO_MUX_FUNC1          (0x01UL << (GPIO_MUX_BITPOS))
#define GPIO_DIR_INPUT          (0x00UL << (GPIO_DIR_BITPOS))
#define GPIO_DIR_OUTPUT         (0x01UL << (GPIO_DIR_BITPOS))
#define GPIO_BUFFTYPE_OPENDRAIN (0x01UL << (GPIO_BUFFTYPE_BITPOS))
#define GPIO_OUTPUT_1           (0x01UL << (GPIO_OUTPUT_BITPOS))

#define LED1                     12
#define LED2                     13

/*   +---------------------------------+
     |  Trace FIFO Debug Port (TFDP)   |
     +---------------------------------+                */
#define TFDP_BASE           ( 0x40008C00UL )
#define TFDP_DATA           ( TFDP_BASE + 0x00 )        // REG32
#define TFDP_CONTROL        ( TFDP_BASE + 0x04 )        // REG32

#define LPC_INTERFACE       ( 0x400F3300UL )
#define LPC_ACTIVATE        ( LPC_INTERFACE + 0x30)

/*   +----------------------------------------------+
     |  NVIC - Nested Vectored Interrupt Controller |
     +----------------------------------------------+                */
#define NVIC_BASE           ( 0xE000E000UL )
#define NVIC_ENABLE2        ( NVIC_BASE + 0x108 )

/*-----------------------------------------------------------------------------
 * Macros for TFDP output
 *---------------------------------------------------------------------------*/
#define trace0(id, cat, lvl, str  )
#define TRACE0(id, cat, lvl, str  ) {                                    \
                            REG8( TFDP_DATA ) = 0xFD;                    \
                            REG8( TFDP_DATA ) = (unsigned char)id;      \
                            REG8( TFDP_DATA ) = (unsigned char)(id>>8); \
                            }
#define trace1(id, cat, lvl, str, p1  )
#define TRACE1(id, cat, lvl, str, p1  ) {                                \
                            REG8( TFDP_DATA ) = 0xFD;                    \
                            REG8( TFDP_DATA ) = (unsigned char)id;      \
                            REG8( TFDP_DATA ) = (unsigned char)(id>>8); \
                            REG8( TFDP_DATA ) = (unsigned char)p1;      \
                            REG8( TFDP_DATA ) = (unsigned char)(p1>>8); \
                            }
#define trace2(id, cat, lvl, str, p1, p2  )
#define TRACE2(id, cat, lvl, str, p1, p2  ) {                            \
                            REG8( TFDP_DATA ) = 0xFD;                    \
                            REG8( TFDP_DATA ) = (unsigned char)id;      \
                            REG8( TFDP_DATA ) = (unsigned char)(id>>8); \
                            REG8( TFDP_DATA ) = (unsigned char)p1;      \
                            REG8( TFDP_DATA ) = (unsigned char)(p1>>8); \
                            REG8( TFDP_DATA ) = (unsigned char)p2;      \
                            REG8( TFDP_DATA ) = (unsigned char)(p2>>8); \
                            }

#define TIMER_PRE_SCL_DIV_48    0x2F0000UL       //Hex value Divider for 47
#define TIMER_1MS               4
#define TIMER_60MS              240

//timer paramters
#define WAIT_IDLE_TIMEOUT      60
#define T_60MS_161x            38               //~60 msec
#define T_60MS_1609            60               //~60 msec
#define T_1MS                  1                //~1 msec


/**************************
* Oscillator defines
**************************/
#define ADDR_PCR_BASE           ( 0x40080100UL )
#define MMCR_OSC_LOCK_REG       ( 0x40080130UL )

#define MMCR_VBAT_CLOCK_ENABLE  ( 0x4000A408UL )

#define XOSEL_SINGLE              1
#define XOSEL_DUAL                0
#define BIT_32K_EN              ( 1<<1 )
