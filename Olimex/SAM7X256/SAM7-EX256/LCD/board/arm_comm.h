/***************************************************************************
 **
 **    Common defineition for IAR EW ARM
 **
 **    Used with ARM IAR C/C++ Compiler and Assembler.
 **
 **    (c) Copyright IAR Systems 2005
 **
 **    $Revision: 10 $
 **
 ***************************************************************************/

#ifndef ARM_COMM_DEF
#define ARM_COMM_DEF

#define MHZ           *1000000l
#define KHZ           *1000l
#define HZ            *1l

#ifndef FALSE
#define FALSE (1 == 0)
#endif

#ifndef TRUE
#define TRUE (1==1)
#endif

#ifndef NULL
#define NULL ((void*)0)
#endif

typedef double          Flo64;    // Double precision floating point
typedef double        * pFlo64;
typedef float           Flo32;    // Single precision floating point
typedef float         * pFlo32;
typedef signed   int    Int32S;   // Signed   32 bit quantity
typedef signed   int  * pInt32S;
typedef unsigned int    Int32U;   // Unsigned 32 bit quantity
typedef unsigned int  * pInt32U;
typedef signed   short  Int16S;   // Signed   16 bit quantity
typedef signed   short* pInt16S;
typedef unsigned short  Int16U;   // Unsigned 16 bit quantity
typedef unsigned short* pInt16U;
typedef signed   char   Int8S;    // Signed    8 bit quantity
typedef signed   char * pInt8S;
typedef unsigned char   Int8U;    // Unsigned  8 bit quantity
typedef unsigned char * pInt8U;
typedef unsigned char   Boolean;  // Boolean
typedef unsigned char * pBoolean;

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

typedef void * (*CommUserFpnt_t)(void *);
typedef void   (*VoidFpnt_t)(void);

#endif // ARM_COMM_DEF
