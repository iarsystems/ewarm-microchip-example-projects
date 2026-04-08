/*-----------------------------------------------------------------------------
*   EUROPE TECHNOLOGIES Software Support
*------------------------------------------------------------------------------
* The software is delivered "AS IS" without warranty or condition of any
* kind, either express, implied or statutory. This includes without
* limitation any warranty or condition with respect to merchantability or
* fitness for any particular purpose, or against the infringements of
* intellectual property rights of others.
*------------------------------------------------------------------------------
*
* File Name       : csp_types.h
* Description     : Definitions and Structures 
* Library Version : 2.00
*
*       +----- (NEW | MODify | ADD | DELete)                                 
*       |                                                                    
*  No   |   When       Who                What               
*-----+---+----------+------------------+--------------------------------------
* 000  NEW  01/05/99   Patrice VILCHEZ    Creation
* 001  MOD  01/04/01   Olivier MAZUYER    Clean up
* 002  MOD  08/06/01   Frederic SAMSON    Clean Up 
* 003  MOD  29/10/01   Christophe GARDIN  Clean Up
* 004  MOD  04/04/02   Christophe GARDIN  #ifndef FALSE and #ifndef TRUE
*----------------------------------------------------------------------------*/

#ifndef CSP_TYPES_H
#define CSP_TYPES_H


/******************************************************************************
*********************************** TYPES *************************************
******************************************************************************/

/******************************************************************************
* Data Types
******************************************************************************/
/* Signed Types                                                              */
typedef signed char     S8_T;
typedef short           S16_T;
typedef long            S32_T;

/* Unsigned Types                                                            */
typedef unsigned char   U8_T;
typedef unsigned short  U16_T;
typedef unsigned long   U32_T;

/* Float Types                                                               */
typedef float           F32_T;
typedef double          F64_T;

/* Boolean types declared as U8_T, as enums are generated as 16 bit          */
typedef U8_T            B_T;

/* Definitions for the two members of the Boolean type                       */
#ifndef FALSE
#define FALSE ((B_T) 0)
#endif

#ifndef TRUE
#define TRUE  ((B_T) 1)
#endif


/******************************************************************************
* Peripherals Type
******************************************************************************/
typedef volatile U32_T CSP_REGISTER_T; 


#endif   /* CSP_TYPE_H */
