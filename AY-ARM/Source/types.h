
// Common types definition
//
// (c) 2012, TS-Labs inc.
// All rights fucked out
//
// Ποιος είπε πως οι άγγελοι στη γη δεν κατεβαίνουν


#ifndef __TYPES_H
#define __TYPES_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */


// Integer Types Definiton
typedef unsigned char   	U8;
typedef unsigned short  	U16;
typedef unsigned int    	U32;
typedef unsigned long long	U64;

typedef signed char     	S8;
typedef short           	S16;
typedef int             	S32;
typedef long long       	S64;

typedef unsigned char   	BIT;
typedef unsigned int    	BOOL;


// Combined Types Definiton
typedef	struct {
	U8	b0:8;
	U8	b1:8;
} W16b;

typedef	struct {
	U8	b0:8;
	U8	b1:8;
	U8	b2:8;
	U8	b3:8;
} W32b;

typedef	struct {
	U8	b0:8;
	U8	b1:8;
	U8	b2:8;
	U8	b3:8;
	U8	b4:8;
	U8	b5:8;
	U8	b6:8;
	U8	b7:8;
} W64b;

typedef	struct {
	U16	h0:16;
	U16	h1:16;
} W32h;

typedef	struct {
	U16	h0:16;
	U16	h1:16;
	U16	h2:16;
	U16	h3:16;
} W64h;

typedef	struct {
	U32	w0:32;
	U32	w1:32;
} W64w;


typedef union {
    W16b b;
    U16 h;
} W16;

typedef union {
    W32b b;
    W32h h;
    U32 w;
} W32;

typedef union {
    W64b b;
	W64h h;
	W64w w;
    U64 d;
} W64;


// Special Types Definiton
typedef struct {
	S32	l;
	S32	r;
} DAC_Sum;

typedef struct {
	U16	l;
	U16	r;
} DAC_t;


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __TYPES_H */
