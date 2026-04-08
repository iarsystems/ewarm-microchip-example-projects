/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support
 * ----------------------------------------------------------------------------
 * Copyright (c) 2010, Atmel Corporation
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

/**
 * \page rtc RTC Example
 *
 * \section Purpose
 *
 * This basic example shows how to use the Real-Time Clock (RTC) peripheral
 * available on the newest Atmel AT91 microcontrollers. The RTC enables easy
 * time and date management and allows the user to monitor events like a
 * configurable alarm, second change, calendar change, and so on.
 *
 * \section Requirements
 *
 * This package can be used with SAM3x evaluation kits.
 *
 * \section Description
 *
 * Upon startup, the program displays the currently set time and date
 * and a menu to perform the following:
 *     \code
 *     Menu:
 *        t - Set time
 *        d - Set date
 *        i - Set time alarm
 *        m - Set date alarm
 *        c - Clear the alarm notification (only if it has been triggered)
 *     \endcode
 *
 * Setting the time, date and time alarm is done by using Menu option "t", "d",
 * the display is updated accordingly.
 *
 * The time alarm is triggered only when the second, minute and hour match the preset
 * values; the date alarm is triggered only when the month and date match the preset
 * values. If both time alarm and date alarm are set, only when the second, minute,
 * hour, month and date match the preset values, the alarm will be triggered.
 *
 * \section Usage
 *
 * -# Build the program and download it inside the evaluation board. Please
 *    refer to the
 *    <a href="http://www.atmel.com/dyn/resources/prod_documents/doc6224.pdf">
 *    SAM-BA User Guide</a>, the
 *    <a href="http://www.atmel.com/dyn/resources/prod_documents/doc6310.pdf">
 *    GNU-Based Software Development</a> application note or to the
 *    <a href="ftp://ftp.iar.se/WWWfiles/arm/Guides/EWARM_UserGuide.ENU.pdf">
 *    IAR EWARM User Guide</a>, depending on your chosen solution.
 * -# On the computer, open and configure a terminal application
 *    (e.g. HyperTerminal on Microsoft Windows) with these settings:
 *   - 115200 bauds
 *   - 8 bits of data
 *   - No parity
 *   - 1 stop bit
 *   - No flow control
 * -# Start the application.
 * -# In the terminal window, the following text should appear:
 *    \code
 *     -- RTC Example xxx --
 *     -- xxxxxx-xx
 *     -- Compiled: xxx xx xxxx xx:xx:xx --
 *
 *     Menu:
 *     t - Set time
 *     d - Set date
 *     i - Set time alarm
 *     m - Set date alarm
 *     q - Quit
 *    \endcode
 * -# Press one of the keys listed in the menu to perform the corresponding action.
 *
 * \section References
 * - rtc/main.c
 * - rtc.c
 * - rtc.h
 */

/**
 * \file
 *
 * This file contains all the specific code for the rtc example.
 */

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

#include "board.h"

#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>

/*----------------------------------------------------------------------------
 *        Local definitions
 *----------------------------------------------------------------------------*/
/** Maximum size of edited string */
#define MAX_EDIT_SIZE       10

/** Macro for check digit character */
#define IsDigitChar(c) ((c) >= '0' && (c) <='9')
/** Macro for converting char to digit */
#define CharToDigit(c) ((c) - '0')
/** Macro for backspace key */
#define KEY_BACKSPACE_CODE           (8)
/** Macro for delete key */
#define KEY_DELETE_CODE              (0x7f)
/** Macro for enter key */
#define KEY_ENTER_CODE               (13)
/** Macro for return key */
#define KEY_RETURN_CODE              (10)
/** Macro for time array size */
#define RTC_TIME_ARRAY_SIZE          (8)
/** Macro for date array size */
#define RTC_DATE_ARRAY_SIZE          (10)
/**Special position of the char ':' or '/' */
#define RTC_TEMP_ARRAY_POS2          (2)
/**Special position of the char ':' or '/' */
#define RTC_TEMP_ARRAY_POS5          (5)  


/*----------------------------------------------------------------------------
 *        Local types
 *----------------------------------------------------------------------------*/
/* RTC status type */
typedef enum _EStates
{
  STATE_MENU=0,  // Default reset status
  STATE_GET_DATE,  // Get the new date
  STATE_GET_TIME,  // Get the new time
  STATE_SET_DATE,  // Begin to set the date
  STATE_SET_TIME,  // Begin to set the time
  STATE_GET_TIME_ALARM,  // Get the new alarm time 
  STATE_GET_DATE_ALARM,  // Get the new alarm date 
  STATE_SET_TIME_ALARM,  // Begin to set the time alarm
  STATE_SET_DATE_ALARM,  // Begin to set the date alarm
  STATE_ALARM,  // time/date alarm triggered status
  STATE_NONE
} EStates ;

/*----------------------------------------------------------------------------
 *        Local variables
 *----------------------------------------------------------------------------*/

/** Current state of application. */
EStates eRTCStatus ;
/**Record the actual action type */
EStates eLastRTCStatus ; 
/**Record the time/date array used position */
volatile uint8_t ucCurPos; 

/** Edited hour. */
static uint8_t ucNewHour;
/** Edited minute. */
static uint8_t ucNewMinute;
/** Edited second. */
static uint8_t ucNewSecond;

/** Edited year. */
static uint16_t wNewYear;
/** Edited month. */
static uint8_t ucNewMonth;
/** Edited day. */
static uint8_t ucNewDay;
/** Edited day-of-the-week. */
static uint8_t ucNewWeek;

/** store time string */
uint8_t aucRtcTime[RTC_TIME_ARRAY_SIZE+1] = {'0', '0', ':', '0', '0', ':', '0', '0','\0'};
/** store date string */
uint8_t aucDate[RTC_DATE_ARRAY_SIZE+1] = {'0', '0', '/', '0', '0', '/', '0', '0', '0', '0', '\0'};

/** store new time string */
uint8_t aucNewRtcTime[RTC_TIME_ARRAY_SIZE+1] = {'0', '0', ':', '0', '0', ':', '0', '0','\0'};
/** store new date string */
uint8_t aucNewDate[RTC_DATE_ARRAY_SIZE+1] = {'0', '0', '/', '0', '0', '/', '0', '0', '0', '0', '\0'};


/** week string */
uint8_t pDayNames[7][4] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
/** console erase sequence */
uint8_t pEraseSeq[] = "\b \b";
/** output format string buffer */
uint8_t aucCalendar[80];

/*----------------------------------------------------------------------------
 *        Local functions
 *----------------------------------------------------------------------------*/
/**
 * \brief Display a string on the terminal.
 */
static inline void _UARTPuts( const uint8_t *pStr )
{
    while ( *pStr )
    {
        UART_PutChar(*pStr++ ) ;
    }
}

/**
 * \brief Print a formatted string into a buffer.
 */
static int16_t _PrnToBuf( uint8_t *pBuf, const uint8_t *pFormat, ... )
{
    va_list    ap ;
    int16_t rc ;

    va_start( ap, pFormat ) ;
    rc = vsprintf( (char *)pBuf, (char const *)pFormat, ap ) ;
    va_end( ap ) ;

    return rc ;
}

/**
 * \brief Get new time, successful value is put in newHour, newMinute, newSecond.
 */
static int16_t _GetNewTime( uint8_t ucKey )
{
    /* Display the key input */
    UART_PutChar( ucKey ) ;

    /* actual handle the input key*/
    aucNewRtcTime[ucCurPos++] = ucKey ;

    /* Position 2 & 5 are reserved for ':' */
    if ( (ucCurPos == RTC_TEMP_ARRAY_POS2) || (ucCurPos == RTC_TEMP_ARRAY_POS5) )
    {
        UART_PutChar( ':' ) ;
        ucCurPos ++;
    }

    /* handle finishing input */
    if ( ucCurPos == RTC_TIME_ARRAY_SIZE )
    {
        ucNewHour = CharToDigit( aucNewRtcTime[0] ) * 10 + CharToDigit( aucNewRtcTime[1] ) ;
        ucNewMinute = CharToDigit( aucNewRtcTime[3] ) * 10 + CharToDigit( aucNewRtcTime[4] ) ;
        ucNewSecond = CharToDigit( aucNewRtcTime[6] ) * 10 + CharToDigit( aucNewRtcTime[7] ) ;

        if (eLastRTCStatus == STATE_SET_TIME)
        {
            /* Do actual set time */
            if ( RTC_SetTime( RTC, ucNewHour, ucNewMinute, ucNewSecond ) )
            {
                printf( "\n\r Time not set, invalid input!\n\r" ) ;
            }
            else
            {
                 printf( "\n\r" ) ;
            }
        }
        else if (eLastRTCStatus == STATE_SET_TIME_ALARM)
        {
            /* Do actual set time alarm */
            if ( RTC_SetTimeAlarm( RTC, &ucNewHour, &ucNewMinute, &ucNewSecond ) )
            {
                printf( "\n\r Time alarm not set, invalid input!\n\r" ) ;
            }
            else
            {
                printf( "\n\r Time alarm is set at %02d:%02d:%02d!", \
                       ucNewHour, ucNewMinute, ucNewSecond ) ;
            }
        }

        /* after set the new time, reset to menu status */
        eRTCStatus = STATE_MENU;
    }

    /* For printf */
    aucNewRtcTime[ucCurPos] = '\0';

    /* success input. verification of data is left to RTC internal Error Checking */
    return 0 ;
}

/**
 * \brief Calculate week from year, month,day.
 */
static uint8_t _CalcWeek( int16_t year, int16_t month, int16_t day )
{
    uint8_t week ;

    if ( month == 1 || month == 2 )
    {
        month += 12 ;
        --year ;
    }

    week = (day+2*month+3*(month+1)/5+year+year/4-year/100+year/400)%7;

    ++week ;

    return week ;
}

/**
 * \brief Get new time, successful value is put in newYear, newMonth, newDay, newWeek.
 */
static int16_t _GetNewDate( uint8_t ucKey )
{
    /* Display the key input */
    UART_PutChar( ucKey ) ;

    /* actual handle the input key*/
    aucNewDate[ucCurPos++] = ucKey ;

    /* Position 2 & 5 are reserved for '/' */
    if ( (ucCurPos == RTC_TEMP_ARRAY_POS2) || (ucCurPos == RTC_TEMP_ARRAY_POS5) )
    {
        UART_PutChar( '/' ) ;
        ucCurPos ++;
    }

    /* handle finishing input */
    if (ucCurPos == RTC_DATE_ARRAY_SIZE)
    {
        /* MM-DD-YY */
        ucNewMonth = CharToDigit( aucNewDate[0] ) * 10 + CharToDigit( aucNewDate[1] ) ;
        ucNewDay = CharToDigit( aucNewDate[3] ) * 10 + CharToDigit( aucNewDate[4] ) ; 
        wNewYear = CharToDigit(aucNewDate[6]) * 1000 + CharToDigit(aucNewDate[7]) * 100 + \
                  CharToDigit(aucNewDate[8]) * 10 + CharToDigit(aucNewDate[9] ) ;
        ucNewWeek = _CalcWeek( wNewYear, ucNewMonth, ucNewDay ) ;

        if (eLastRTCStatus == STATE_SET_DATE)
        {
            /* Do actual set date */
            if ( RTC_SetDate( RTC, wNewYear, ucNewMonth, ucNewDay, ucNewWeek ) )
            {
                printf( "\n\r Date not set, invalid input!\n\r" ) ;
            }
            else
            {
                 printf( "\n\r" ) ;
            }

        }
        else if (eLastRTCStatus == STATE_SET_DATE_ALARM)
        {
            /* Do actual set date alarm */
            if ( RTC_SetDateAlarm( RTC, &ucNewMonth, &ucNewDay ) )
            {
                printf( "\n\r Date alarm not set, invalid input!\n\r" ) ;
            }
            else
            {
                printf( "\n\r Date alarm is set on %02d/%02d!", ucNewMonth, ucNewDay ) ;
            }
        }

        /* after set the new date, reset to menu status */
        eRTCStatus = STATE_MENU;
    }

    /* For printf */
    aucNewDate[ucCurPos] = '\0';

    /* success input. verification of data is left to RTC internal Error Checking */
    return 0 ;
}


/**
 * \brief Displays the user interface on the terminal.
 */
static void _RefreshDisplay( void )
{
    uint8_t hour, minute, second ;
    uint16_t year ;
    uint8_t month, day, week ;

    RTC_GetTime( RTC, &hour, &minute, &second ) ;
    RTC_GetDate( RTC, &year, &month, &day, &week ) ; 
    
    _PrnToBuf( aucRtcTime, "%02d:%02d:%02d", hour, minute, second ) ;
    _PrnToBuf( aucDate, "%02d/%02d/%04d", month, day, year ) ;
    _PrnToBuf( aucCalendar, "[Time/Date: %s, %s %s ]", \
         aucRtcTime, aucDate, pDayNames[week-1] ) ;

    switch ( eRTCStatus )
    {
        case STATE_MENU :
            printf( "\r%s", (char *)aucCalendar ) ;
        break;

        case STATE_ALARM :
            printf( "  c - Press 'C' to clear alarm notification\n\r" ) ;
        break;

        case STATE_SET_TIME :
            printf( " Please Input New Time : " );
            aucNewRtcTime[0] = '\0';  // Reset tmp array for timer
            eRTCStatus = STATE_GET_TIME;
        break;

        case STATE_SET_DATE :
            printf( " Please Input Date: " );
            aucNewDate[0] = '\0';  // Reset tmp array for timer
            eRTCStatus = STATE_GET_DATE;
        break;

        case STATE_SET_TIME_ALARM :
            printf( " Please Input New Alarm Time : " );
            aucNewRtcTime[0] = '\0';  // Reset tmp array for date
            eRTCStatus = STATE_GET_TIME_ALARM;
        break;

        case STATE_SET_DATE_ALARM :
            printf( " Please Input New Alarm Date : " );
            aucNewDate[0] = '\0';  // Reset tmp array for date
            eRTCStatus = STATE_GET_DATE_ALARM;
        break;

        case STATE_GET_TIME :
            printf( "\r%s Please Input New Time : %s", \
                (char *)aucCalendar, (char *)aucNewRtcTime);
        break;

        case STATE_GET_TIME_ALARM :
            printf( "\r%s Please Input New Alarm Time : %s", \
                (char *)aucCalendar, (char *)aucNewRtcTime );
        break;

        case STATE_GET_DATE :
            printf( "\r%s Please Input Date: %s", \
                (char *)aucCalendar, (char *)aucNewDate );
        break;

        case STATE_GET_DATE_ALARM :
            printf( "\r%s Please Input New Alarm Date : %s", \
                (char *)aucCalendar, (char *)aucNewDate );
        break;

            
       default :
            break;
    }
}

/**
 * \brief Interrupt handler for the RTC. Refreshes the display.
 */
void RTC_IrqHandler( void )
{
    uint32_t dwStatus = RTC->RTC_SR ;

    /* Second increment interrupt */
    if ( (dwStatus & RTC_SR_SEC) == RTC_SR_SEC )
    {
        /* Disable RTC interrupt */
        RTC_DisableIt( RTC, RTC_IDR_SECDIS ) ;

        _RefreshDisplay() ;

        RTC->RTC_SCCR = RTC_SCCR_SECCLR ;

        RTC_EnableIt( RTC, RTC_IER_SECEN ) ;
    }
    /* Time or date alarm */
    else
    {
        if ( (dwStatus & RTC_SR_ALARM) == RTC_SR_ALARM )
        {
            /* Disable RTC interrupt */
            RTC_DisableIt( RTC, RTC_IDR_ALRDIS ) ;

            eRTCStatus = STATE_ALARM ;
            _RefreshDisplay() ;
            RTC->RTC_SCCR = RTC_SCCR_ALRCLR ;

            RTC_EnableIt( RTC, RTC_IER_ALREN ) ;
        }
    }
}

/*----------------------------------------------------------------------------
 *         Global functions
 *----------------------------------------------------------------------------*/

/**
 * \brief Application entry point for RTC example.
 *
 * \return Unused (ANSI-C compatibility).
 */

extern int main( void )
{
    uint8_t ucKey ;

    /* Disable watchdog */
    WDT_Disable( WDT ) ;

    /* Output example information */
    printf( "%c[2J\r", 27 ) ;
    printf( "-- RTC Example %s --\n\r", SOFTPACK_VERSION ) ;
    printf( "-- %s\n\r", BOARD_NAME ) ;
    printf( "-- Compiled: %s %s --\n\r", __DATE__, __TIME__ ) ;

    /* Default RTC configuration */
    RTC_SetHourMode( RTC, 0 ) ; /* 24-hour mode */
    if ( RTC_SetTimeAlarm( RTC, 0, 0, 0 ) )
    {
        printf( "\n\r Disable time alarm fail!" ) ;
    }

    if ( RTC_SetDateAlarm( RTC, 0, 0 ) )
    {
        printf( "\n\r Disable date alarm fail!" ) ;
    }

    /* Print the basic menu on init */
    printf( "\n\rMenu:\n\r" ) ;
    printf( "  h - Show Menu\n\r" );
    printf( "  t - Set time\n\r" ) ;
    printf( "  d - Set date\n\r" ) ;
    printf( "  i - Set time alarm\n\r" ) ;
    printf( "  m - Set date alarm\n\r" ) ;
    printf( "  q - Quit!\n\r" ) ;
    printf( "[Time/Date Format: HH:MM:SS, MM/DD/YYYY]\n\r" );
    printf( "\n\r" ) ; 

    /* Configure RTC interrupts */
    NVIC_DisableIRQ( RTC_IRQn ) ;
    NVIC_ClearPendingIRQ( RTC_IRQn ) ;
    NVIC_SetPriority( RTC_IRQn, 0 ) ;
    NVIC_EnableIRQ( RTC_IRQn ) ;
    RTC_EnableIt( RTC, RTC_IER_SECEN | RTC_IER_ALREN ) ;

    /* Refresh display once */
    _RefreshDisplay() ;

    /* Handle keypresses */
    while ( 1 )
    {
        ucKey = UART_GetChar() ;

        switch ( ucKey )
        {
           case 'h' :  // Show the menu
                printf( "%c[2J\r", 27 ) ;
                printf( "\n\rMenu:\n\r" ) ;
                printf( "  h - Show Menu\n\r" );
                printf( "  t - Set time\n\r" ) ;
                printf( "  d - Set date\n\r" ) ;
                printf( "  i - Set time alarm\n\r" ) ;
                printf( "  m - Set date alarm\n\r" ) ;
                printf( "  q - Quit!\n\r" ) ;
                printf( "[Time/Date Format: HH:MM:SS, MM/DD/YYYY]\n\r" );
                printf( "\n\r" ) ;

                ucCurPos = 0;  // reset the pos
                eRTCStatus = STATE_MENU;  // reset to default status
                eLastRTCStatus = eRTCStatus;  // reset to default status
            break;

            case 't' :  // Set the new time
                if ( eRTCStatus == STATE_MENU )
                {
                    /* reset the default values */
                    ucNewHour = ucNewMinute = ucNewSecond = 0xFF;
                    ucCurPos = 0;
                    eRTCStatus = STATE_SET_TIME;
                    eLastRTCStatus = eRTCStatus;
                }
            break;

            case 'd' :  // Set the new date
                if ( eRTCStatus == STATE_MENU )
                {
                    /* reset the default values */
                    wNewYear = 0xFFFF;
                    ucNewMonth = ucNewDay = ucNewWeek = 0xFF;
                    ucCurPos = 0;
                    eRTCStatus = STATE_SET_DATE;
                    eLastRTCStatus = eRTCStatus;
                }
            break;

            case 'i' :  // Set the new time alarm
                if ( eRTCStatus == STATE_MENU )
                {
                    /* reset the default values */
                    eRTCStatus = STATE_SET_TIME_ALARM;
                    ucCurPos = 0;
                    eLastRTCStatus = eRTCStatus;
                }
            break;

            case 'm' :  // Set the new date alarm
                if ( eRTCStatus == STATE_MENU )
                {
                    /* reset the default values */
                    eRTCStatus = STATE_SET_DATE_ALARM;
                    ucCurPos = 0;
                    eLastRTCStatus = eRTCStatus;
                }
            break;

            case 'c' :  // Clear the alarm
                 /* reset the default values */
                ucCurPos = 0;
                eRTCStatus = STATE_MENU;
                eLastRTCStatus = eRTCStatus;
            break;

            case 'q' :  // Quit, currently just reset the app
                /* reset the default values */
                ucCurPos = 0;
                eRTCStatus = STATE_MENU;
                eLastRTCStatus = eRTCStatus;
            break;

            case '0' :
            case '1' :
            case '2' :
            case '3' :
            case '4' :
            case '5' :
            case '6' :
            case '7' :
            case '8' :
            case '9' :
                if( (eRTCStatus == STATE_GET_TIME) | \
                     (eRTCStatus == STATE_GET_TIME_ALARM) )
                {
                    /* Get the new time */
                    _GetNewTime(ucKey);
                }
                else if ( (eRTCStatus == STATE_GET_DATE) | \
                     (eRTCStatus == STATE_GET_DATE_ALARM) )
                {
                    /* Get the new date */
                    _GetNewDate(ucKey);
                }
            break ;

            /* Handle the del/ backspace key, only vaild when getting the time/date*/
            case KEY_DELETE_CODE :
            case KEY_BACKSPACE_CODE :
                if( (eRTCStatus == STATE_GET_TIME) | \
                     (eRTCStatus == STATE_GET_TIME_ALARM) )
                {
                    /* for empty time array, just return */
                    if (ucCurPos == 0)
                        break;

                     /* erase one char */
                    _UARTPuts( pEraseSeq ) ;
                    ucCurPos --;

                    /* take care of the ':' */
                    if ( (ucCurPos == RTC_TEMP_ARRAY_POS2) || (ucCurPos == RTC_TEMP_ARRAY_POS5) )
                    {
                        _UARTPuts( pEraseSeq ) ;
                        ucCurPos --;
                    }

                    /* adjust the new time info for print */
                    aucNewRtcTime[ucCurPos] = '\0';

                }
                else if ( (eRTCStatus == STATE_GET_DATE) | \
                     (eRTCStatus == STATE_GET_DATE_ALARM) )
                {
                    /* empty date array */
                    if (ucCurPos == 0)
                        break;

                    /* erase one char */
                    _UARTPuts( pEraseSeq ) ;
                    ucCurPos --;

                    /* take care of the '/' */
                    if ( (ucCurPos == RTC_TEMP_ARRAY_POS2) || (ucCurPos == RTC_TEMP_ARRAY_POS5) )
                    {
                        _UARTPuts( pEraseSeq ) ;
                        ucCurPos --;
                    }

                    /* adjust the new date info for print */
                    aucNewDate[ucCurPos] = '\0';
                }

            break;

            default : /* doing nothing here */
            break;
        }

        _RefreshDisplay();
    }
}

