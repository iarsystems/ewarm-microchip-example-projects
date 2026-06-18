//  ----------------------------------------------------------------------------
//          ATMEL Microcontroller Software Support  -  ROUSSET  -
//  ----------------------------------------------------------------------------
//  DISCLAIMER:  THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
//  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
//  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
//  DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
//  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
//  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
//  OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
//  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
//  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
//  EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//  ----------------------------------------------------------------------------
// File Name           : trace.h
// Object              : Trace functions
// Creation            : Application Group - v1.0 25-Aug-2005
//  ----------------------------------------------------------------------------
#ifndef _TRACE_H
#define _TRACE_H

#include <stdio.h>


#ifndef NDEBUG
// Application dependent
#define TR_INFO     stdout
#define TR_WARNING  stdout
#define TR_ERROR    stdout
#define TR_FATAL    stdout

//#define TR_DEBUG_H  stdout
#define TR_DEBUG_M  stdout
//#define TR_DEBUG_L  stdout
#endif // NDEBUG

// **************************************************
//     Trace define
// **************************************************

#ifdef TR_DEBUG_H
	#define TRACE_DEBUG_H(...)    { if (TR_DEBUG_H != NULL) fprintf(TR_DEBUG_H,  __VA_ARGS__); }
#else
	#define TRACE_DEBUG_H(...)
#endif // TR_DEBUG_H

#ifdef TR_DEBUG_M
	#define TRACE_DEBUG_M(...)    { if (TR_DEBUG_M != NULL) fprintf(TR_DEBUG_M,  __VA_ARGS__); }
#else
	#define TRACE_DEBUG_M(...)
#endif // TR_DEBUG_M

#ifdef TR_DEBUG_L
	#define TRACE_DEBUG_L(...)    { if (TR_DEBUG_L != NULL) fprintf(TR_DEBUG_L,  __VA_ARGS__); }
#else
	#define TRACE_DEBUG_L(...)
#endif // TR_DEBUG_L

#ifdef TR_INFO
	#define TRACE_INFO(...)     { if (TR_INFO != NULL) fprintf(TR_INFO,  __VA_ARGS__); }
#else
	#define TRACE_INFO(...)
#endif // TR_INFO

#ifdef TR_WARNING
	#define TRACE_WARNING(...)  { if (TR_WARNING != NULL) fprintf(TR_WARNING,  __VA_ARGS__); }
#else
	#define TRACE_WARNING(...)
#endif // TR_WARNING

#ifdef TR_ERROR
	#define TRACE_ERROR(...)    { if (TR_ERROR != NULL) fprintf(TR_ERROR,  __VA_ARGS__); }
#else
	#define TRACE_ERROR(...)
#endif // TR_ERROR

#ifdef TR_FATAL
	#define TRACE_FATAL(...)  { if (TR_FATAL != NULL) fprintf(TR_FATAL,  __VA_ARGS__); }
#else
	#define TRACE_FATAL(...)
#endif // TR_FATAL


#endif // _TRACE_H

