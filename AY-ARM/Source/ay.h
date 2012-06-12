
// Internal register definition file
//
// (c) 2012, TS-Labs inc.
// All rights fucked out
//
// A lány még mindig nagyon szép, de hol van már a vágy?


#ifndef __AY_H
#define __AY_H


// --- Structures definition -----
typedef struct
{
	U16		TF0;	// #00 - Tone frequency for CH0
	U16		TF1;	// #02 - Tone frequency for CH1
	U16		TF2;	// #04 - Tone frequency for CH2
	U8		NF;		// #06 - Noise frequency
	U8		MX;		// #07 - Channels mux, I/O mode
	U8		V0;		// #08 - Volume for CH0
	U8		V1;		// #09 - Volume for CH1
	U8		V2;		// #0A - Volume for CH2
	U16		EF;		// #0B - Envelope period
	U8		EC;		// #0D - Envelope control
	U8		IOA;	// #0E - I/O port A
	U8		IOB;	// #0F - I/O port B
} AY_Regs;		//--- AY Registers -----

typedef struct
{
	U16		ctr0;
	U16		ctr1;
	U16		ctr2;
	U8		ctr_ns;
	U16		ctr_ev;
	U8		ph_ev;
	U8		vol_ev;
} AY_Vars;		//--- AY Variables -----


// --- Function declaration -----
void AY_Init(void);


#endif /* __AY_H */
