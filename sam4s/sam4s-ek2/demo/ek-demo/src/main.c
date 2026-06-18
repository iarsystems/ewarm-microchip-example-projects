/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support
 * ----------------------------------------------------------------------------
 * Copyright (c) 2009, Atmel Corporation
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

#include <string.h>

/* Atmel library includes. */
#include "board.h"

/* Atmel SAM-GUI includes. */
#include "libsam_gui.h"

/* Atmel NAND FAT fs includes. */
#include "memories.h"

/* Bitmaps */
//#include "bitmaps/bitmaps.h"

/* Demo GUI */
#include "gui/screens.h"
#include "demo_parameters.h"

/* Includes for Atmel FTL library */
#include "nand/ftldrv.h"

/* Atmel USB MSD include */
#include "usb/usbmsd.h"

#if defined DEMO_USE_QTOUCH
/* Atmel Qtouch library */
#    include "qtouch/qtouch.h"
#endif // defined DEMO_USE_QTOUCH

#include "frontend_ADS7843.h"
#include "frontend_pushbuttons.h"
#include "frontend_potentiometer.h"
#if defined DEMO_USE_QTOUCH
#include "frontend_qtouch.h"
#endif // defined DEMO_USE_QTOUCH

#include "board_dependencies.h"

SAMGUI_TaskHandle hTskQTouch ;
SAMGUI_TaskHandle hTskMonitor ;
SAMGUI_TaskHandle hTskFTL ;

SWGTFrontend* g_pFrontend_TSD ;
SWGTFrontend* g_pFrontend_Potentiometer ;
SWGTFrontend* g_pFrontend_PushButtons ;
SWGTFrontend* g_pFrontend_QTouch ;

extern void xPortSysTickHandler( void ) ;
extern void xPortPendSVHandler( void ) ;
extern void vPortSVCHandler( void ) ;

void SysTick_Handler( void )
{
    xPortSysTickHandler() ;
}

void PendSV_Handler( void )
{
    xPortPendSVHandler() ;
}

void SVC_Handler( void )
{
    vPortSVCHandler() ;
}

/**
 * \addtogroup SAM3S_demo AT91SAM3S demo
 * @{
 * \addtogroup main AT91SAM3S Demo main()
 * @{
 */

/**
 * Called if stack overflow during execution
 */
void vApplicationStackOverflowHook( xTaskHandle *pxTask, signed char *pcTaskName )
{
//	( void ) pxTask;
//	( void ) pcTaskName;

    printf( "stack overflow %x %s\r\n", pxTask, pcTaskName ) ;
    /* If the parameters have been corrupted then inspect pxCurrentTCB to
       identify which task has overflowed its stack. */
    for( ;; ) ;
}

/**
 * Configure the hardware for the demo.
 */
static void _SetupHardware( void )
{
    uint32_t i = 0;

    /* Initialize default IRQ priority for all Peripheral IRQs */
    for ( i=0 ; i < 35 ; i++ )
    {
        NVIC_SetPriority( (IRQn_Type)i, configMAX_SYSCALL_INTERRUPT_PRIORITY + (1<<4) ) ;
    }

    Demo_Parameters_Initialize() ;

    /* Initialize Power LED */
    LED_Configure( 2 ) ;
    LED_Set( 2 ) ;

    /* Initialize RTC */
    RTC_SetTime( RTC, g_demo_parameters.dwHour, g_demo_parameters.dwMin, g_demo_parameters.dwSec ) ;
    RTC_SetDate( RTC, g_demo_parameters.dwYear, g_demo_parameters.dwMonth, g_demo_parameters.dwDay, 1 ) ;
    /*   Configure RTC interrupts */
    NVIC_DisableIRQ( RTC_IRQn ) ;
    NVIC_ClearPendingIRQ( RTC_IRQn ) ;
//    NVIC_SetPriority( RTC_IRQn, configMAX_SYSCALL_INTERRUPT_PRIORITY+(1<<4) ) ;
    NVIC_EnableIRQ( RTC_IRQn ) ;
    RTC_EnableIt( RTC, RTC_IER_SECEN ) ;

    /* Initialize NVIC to get accept interrupts from PIO controllers */
    PIO_InitializeInterrupts(configMAX_SYSCALL_INTERRUPT_PRIORITY + 16);

#ifdef _DEMO_USE_USB_
    /* Initialize USB Disk */
    UsbMsdInitialize() ;
#endif // _DEMO_USE_USB_

    /* Initialize Disk */
    NandFlashInitialize() ;

#if defined DEMO_USE_QTOUCH
    /* Initialize QTouch */
    QTouchInitialize() ;
#endif // defined DEMO_USE_QTOUCH
}

extern void vApplicationIdleHook( void )
{
}

/**
 * Update current time
 */
static void _TimeUpdate( void )
{
}

/**
 * This function is called by FreeRTOS each tick
 */
extern void vApplicationTickHook( void )
{
    /** As we don't use RTC in this application, we have to write current date/time every minute */
    if ( xTaskGetTickCount()%60000 == 0 )
    {
        _TimeUpdate() ;
    }
}

/**
 * This task, when activated, send every ten seconds on debug UART the whole report of free heap and total tasks status
 */
static void _TaskMonitoring( void *pvParameters )
{
    static char szList[256] ;

    for ( ; ; )
    {

        printf( "--- task ## %u", uxTaskGetNumberOfTasks() ) ;
        vTaskList( (signed char*)szList ) ;
        printf( szList ) ;
        SAMGUI_TaskDelay( 1000 ) ;
    }
}

/**
 * Main entry point
 * Initialize hardware, SAM-GUI, App GUI, Monitor task, QTouch task, USB MassStorage task
 * Then start scheduler....
 */
extern int main( void )
{
    /** Disable watchdog. */
    WDT_Disable( WDT ) ;

    /** Output demo infomation. */
    printf( "-- Demo SAM4S %s --\n\r", DEMO_VERSION ) ;
    printf( "-- %s\n\r", BOARD_NAME ) ;
    printf( "-- Compiled: %s %s --\n\r", __DATE__, __TIME__ ) ;

    /** Prepare the hardware. */
    _SetupHardware() ;

    // Add ILI9325 backend
    DISP_AddBackend( &sDISP_Backend_ILI9325 ) ;

    /** Initialize and start SAM-GUI. */
    if ( WGT_Initialize() == SAMGUI_E_OK )
    {
        g_pFrontend_TSD=NULL ;
        if ( WGT_Frontend_AddFrontend( (SWGTFrontend*)&sWGT_Frontend_ADS7843 ) == SAMGUI_E_OK )
        {
            g_pFrontend_TSD=(SWGTFrontend*)&sWGT_Frontend_ADS7843 ;
            g_pFrontend_TSD->Initialize() ;
        }

        g_pFrontend_PushButtons=NULL ;
        if ( WGT_Frontend_AddFrontend( (SWGTFrontend*)&sWGT_Frontend_PushButtons ) == SAMGUI_E_OK )
        {
            g_pFrontend_PushButtons=(SWGTFrontend*)&sWGT_Frontend_PushButtons ;
            g_pFrontend_PushButtons->Initialize() ;
        }

        g_pFrontend_Potentiometer=NULL ;
        if ( WGT_Frontend_AddFrontend( (SWGTFrontend*)&sWGT_Frontend_Potentiometer ) == SAMGUI_E_OK )
        {
            g_pFrontend_Potentiometer=(SWGTFrontend*)&sWGT_Frontend_Potentiometer ;
            g_pFrontend_Potentiometer->Initialize() ;
        }

        g_pFrontend_QTouch=NULL ;
#if defined DEMO_USE_QTOUCH
        if ( WGT_Frontend_AddFrontend( (SWGTFrontend*)&sWGT_Frontend_QTouch ) == SAMGUI_E_OK )
        {
            g_pFrontend_QTouch=(SWGTFrontend*)&sWGT_Frontend_QTouch ;
            g_pFrontend_QTouch->Initialize() ;
        }
#endif // defined DEMO_USE_QTOUCH

        /** if no TSD calibration done */
        if ( g_demo_parameters.sCalibration.lSlopeX == 0 )
        {
            /** Set Calibration screen as first screen to be handled. */
            WGT_SetCurrentScreen( &ScrTSDCalibration ) ;
        }
        else
        {
            /** Set Intro as first screen to be handled. */
            WGT_SetCurrentScreen( &ScrIntro ) ;
        }

#if 1
        /** Start task to monitor processor activity */
        if ( SAMGUI_TaskCreate( _TaskMonitoring, "Monitor", NULL, TASK_MONITOR_STACK_SIZE,
                                TASK_MONITOR_STACK_PRIORITY, NULL, &hTskMonitor ) != SAMGUI_E_OK )
        {
            printf( "Failed to create Monitor task\r\n" ) ;
        }
#endif

        /** Start the tasks defined within this file/specific to this demo. */
        if ( SAMGUI_TaskCreate( TaskFtl, "FTL", NULL, TASK_FTL_STACK_SIZE,
                                TASK_FTL_STACK_PRIORITY, NULL, &hTskFTL ) != SAMGUI_E_OK )
        {
            printf( "Failed to create FTL task\r\n" ) ;
        }

#if defined DEMO_USE_QTOUCH
        if ( SAMGUI_TaskCreate( TaskQTouch, "QTOUCH", NULL, TASK_QTOUCH_STACK_SIZE,
                                TASK_QTOUCH_STACK_PRIORITY, NULL, &hTskQTouch ) != SAMGUI_E_OK )
        {
            printf( "Failed to create QTOUCH task\r\n" ) ;
        }
#endif // defined DEMO_USE_QTOUCH

        USB_Start() ;

        // Start the scheduler.
        vTaskStartScheduler() ;
    }

    // Will only get here if there was insufficient memory to create the idle task.
    return 0 ;
}

/** @} */
/** @} */
