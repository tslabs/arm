
// Internal register definition file
//
// (c) 2012, TS-Labs inc.
// All rights fucked out
//
// A lány még mindig nagyon szép, de hol van már a vágy?


#include "types.h"

#ifndef __REGS_H
#define __REGS_H


//--- AY Registers -----

typedef struct
{
	U16		TF0;	// #00
	U16		TF1;	// #02
	U16		TF2;	// #04
	U8		NF;		// #06
	U8		MX;		// #07
	U8		V0;		// #08
	U8		V1;		// #09
	U8		V2;		// #0A
	U16		EF;		// #0B
	U8		EC;		// #0D
	U8		IOA;	// #0E
	U8		IOB;	// #0F
} AY_Regs;


#endif /* __REGS_H */
