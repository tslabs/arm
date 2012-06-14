
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


// integer types
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


// combined types
typedef	struct {
	U8	l:8;
	U8	h:8;
} W16b;

typedef union {
    W16b b;
    U16 w;
} W16;


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __TYPES_H */
