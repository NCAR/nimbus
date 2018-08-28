/*
* Filename:  stddefs.h
* Author:    Marc Anderson
* Compiler:  GNU C 2.6.0
* Copyright 1994, All Rights Reserved
*
* Description:
*   Standard definitions and base datatypes to be used throughout project --
*   use these types to maintain portability.
*
* Debugging: (testcode included at bottom of file)
*   to test sizes of types:
*      % cp stddefs.h xx.c
*      % cc -DSTDDEFS_TESTCODE xx.c
*      % a.out
*
*/

#ifndef STDDEFS_H
#define STDDEFS_H

#include <unistd.h>    /* for definition of boolean on chinook */
#include <limits.h>

typedef int boolean;


/* 
* standard datatypes
*/
typedef signed char int8;
typedef unsigned char u_int8;

typedef signed short int16;
typedef unsigned short u_int16;

typedef signed int int32;
typedef unsigned int u_int32;

/* no such thing as "unsigned float" or "unsigned double" */
typedef float real32;
typedef double real64;



/*
* limits of our primitives
*/
#define INT8_MIN     CHAR_MIN
#define INT8_MAX     CHAR_MAX

#define U_INT8_MIN   0
#define U_INT8_MAX   UCHAR_MAX

#define INT16_MIN    SHRT_MIN
#define INT16_MAX    SHRT_MAX

#define U_INT16_MIN  0
#define U_INT16_MAX  USHRT_MAX

#define INT32_MIN    LONG_MIN
#define INT32_MAX    LONG_MAX

#define U_INT32_MIN  0
#define U_INT32_MAX  ULONG_MAX

#define REAL32_MIN   -FLT_MAX
#define REAL32_MAX   FLT_MAX

#define REAL64_MIN   -DBL_MAX
#define REAL64_MAX   DBL_MAX


/*
* define NULL pointer
*/
#ifndef NULL
#define NULL		   (void *) 0
#endif


/*
* useful _general_ macros
*
* (note that 'x' is lower limit and 'y' is upper limit)
*/
#define IS_BTWN(a,x,y)          ((a)>(x)) && ((a)<(y)))     /* x >  a > y ?? */
#define IS_BTWN_OR_EQUAL(a,x,y) (((a)>=(x)) && ((a)<=(y)))  /* x >= a >= y ? */

#endif


#ifdef STDDEFS_TESTCODE
#include <stdio.h>

main()
{
	printf("sizeof variables:\n");

	printf("int8    == %d\n", sizeof(int8) * 8);
	printf("u_int8  == %d\n", sizeof(u_int8) * 8);
	printf("int16   == %d\n", sizeof(int16) * 8);
	printf("u_int16 == %d\n", sizeof(u_int16) * 8);
	printf("int32   == %d\n", sizeof(int32) * 8);
	printf("u_int32 == %d\n", sizeof(u_int32) * 8);
	printf("real32  == %d\n", sizeof(real32) * 8);
	printf("real64  == %d\n", sizeof(real64) * 8);

	printf("\n");

	printf("INT8_MIN    == %d\n", (int) INT8_MIN);
	printf("INT8_MAX    == %d\n", (int) INT8_MAX);
	printf("U_INT8_MIN  == %u\n", (unsigned) U_INT8_MIN);
	printf("U_INT8_MAX  == %u\n", (unsigned) U_INT8_MAX);
	printf("INT16_MIN   == %d\n", (int) INT16_MIN);
	printf("INT16_MAX   == %d\n", (int) INT16_MAX);
	printf("U_INT16_MIN == %u\n", (unsigned) U_INT16_MIN);
	printf("U_INT16_MAX == %u\n", (unsigned) U_INT16_MAX);
	printf("INT32_MIN   == %ld\n",(long) INT32_MIN);
	printf("INT32_MAX   == %ld\n",(long) INT32_MAX);
	printf("U_INT32_MIN == %lu\n",(unsigned long) U_INT32_MIN);
	printf("U_INT32_MAX == %lu\n",(unsigned long) U_INT32_MAX);
	printf("REAL32_MIN  == %f\n", (double) REAL32_MIN);
	printf("REAL32_MAX  == %f\n", (double) REAL32_MAX);
	printf("REAL64_MIN  == %f\n", (double) REAL64_MIN);
	printf("REAL64_MAX  == %f\n", (double) REAL64_MAX);
}
#endif

