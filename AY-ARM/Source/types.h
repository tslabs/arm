
// Common types definition
//
// (c) 2012, TS-Labs inc.
// All rights fucked out
//
// Ποιος είπε πως οι άγγελοι στη γη δεν κατεβαίνουν

#pragma once
#pragma anon_unions

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

// Integer types definiton
typedef unsigned char   	U8;
typedef unsigned short  	U16;
typedef unsigned int    	U32;
typedef unsigned long long	U64;

typedef signed char     	S8;
typedef short           	S16;
typedef int             	S32;
typedef long long       	S64;

typedef unsigned char   	BIT;
typedef unsigned char    	BOOL;

// Unioned types definiton
typedef	union
{
	struct
	{
		U8	b0:8;
		U8	b1:8;
	};
	U16 h;
} W16;

typedef	union
{
	struct
	{
		U8	b0:8;
		U8	b1:8;
		U8	b2:8;
		U8	b3:8;
	};
	struct
	{
		U16 h0:16;
		U16 h1:16;
	};
	U32 w:32;
} W32;

typedef	union
{
	struct
	{
		U8	b0:8;
		U8	b1:8;
		U8	b2:8;
		U8	b3:8;
		U8	b4:8;
		U8	b5:8;
		U8	b6:8;
		U8	b7:8;
	};
	struct
	{
		U16 h0:16;
		U16 h1:16;
		U16 h2:16;
		U16 h3:16;
	};
	struct
	{
		U32 w0:32;
		U32 w1:32;
	};
	U64 d;
} W64;

// Function types
typedef void (*FUNC)();
typedef void (*FUNC_W)(U8);
typedef U8 (*FUNC_R)();

// Special Types Definiton
typedef struct
{
	S32	l;
	S32	r;
} DAC_Sum;

typedef struct
{
	U16	l;
	U16	r;
} DAC_t;

#ifdef __cplusplus
}
#endif /* __cplusplus */

