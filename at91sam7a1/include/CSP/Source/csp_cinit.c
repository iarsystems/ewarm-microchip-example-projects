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
* File Name       : csp_cinit.c
* Description     : Copy RO code, RW data from ROM to RAM and zero-initialized
*                   the ZI data areas in RAM
* Version         : 2.00
* Compiler Tools  : ADS
*
*       +----- (NEW | MODify | ADD | DELete)                                 
*       |                                                                    
*  No   |   when       who                what               
*-----+---+----------+------------------+--------------------------------------
* 000  NEW  01/05/99   Patrice VILCHEZ    Creation
* 001  MOD  01/04/01   Olivier MAZUYER    Clean up
* 002  MOD  08/06/01   Frederic SAMSON    Clean Up
* 003  MOD  26/08/02   Christophe GARDIN  Clean up
*----------------------------------------------------------------------------*/

/******************************************************************************
* Include files
******************************************************************************/
#include "csp.h"


/******************************************************************************
* Declarations 
******************************************************************************/
extern U32_T Region$$Table$$Base;
extern U32_T Region$$Table$$Limit;
extern U32_T ZISection$$Table$$Base;
extern U32_T ZISection$$Table$$Limit;


/******************************************************************************
****************************  F U N C T I O N S  ******************************
******************************************************************************/

/******************************************************************************
* Function          : CSP_CInit
* Description       : Copy RO code, RW data from ROM to RAM and zero-initialized
*                     the ZI data areas in RAM
* Inputs            : None
* Functions called  : None
* Returns           : None
******************************************************************************/
void CSP_CInit(void)
{
   /* Local variables */
   U32_T index_u32;
   U32_T source_u32;
   U32_T destination_u32;
   U32_T taille_u32;
   U32_T i_u32;
   U32_T start_u32;
   U32_T end_u32;

   start_u32 = (U32_T) &Region$$Table$$Base;
   end_u32   = (U32_T) &Region$$Table$$Limit;
   
   /* For RO & RW sections */
   for(index_u32 = start_u32; index_u32 < end_u32; index_u32 += (3*4))
   {
      source_u32      = *((U32_T*) (index_u32));
      destination_u32 = *((U32_T*) (index_u32 + 4));
      taille_u32      = *((U32_T*) (index_u32 + 8)) / 4;
      
      for(i_u32=0; i_u32 < taille_u32; i_u32++)
      {
         *((U32_T*) (destination_u32)) =  *((U32_T*) (source_u32));
         destination_u32 += 4;
         source_u32      += 4;
      } 
   }
   
   start_u32 = (U32_T) &ZISection$$Table$$Base;
   end_u32   = (U32_T) &ZISection$$Table$$Limit;

   /* For ZI sections */
   for(index_u32 = start_u32; index_u32 < end_u32; index_u32 += (2*4))
   {
      destination_u32 = *((U32_T*) (index_u32));
      taille_u32      = *((U32_T*) (index_u32 + 4)) / 4;

      for(i_u32=0; i_u32 < taille_u32; i_u32++)
      {
         *((U32_T*) (destination_u32)) = 0;
         destination_u32 += 4;
      }
   }    
}
