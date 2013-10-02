
// Audio output functions definitions
//
// (c) 2012, TS-Labs inc.
// All rights fucked out
//
// I wtedy wrescie sam tez wlacze sie do akcji


#pragma once
#include "types.h"


// --- Structures definition -----
typedef union {
	struct {
		U8	act:1;		// channel is active
		U8	chn:1;		// 0 - mono, 1 - stereo
		U8	sgn:1;		// 0 - signed, 1 - unsigned
		U8	bw:1;		// 0 - 8 bit, 1 - 16 bit
		U8	loop:2;		// 0 - off, 1 - forward, 2 - bidi, 3 - reserved
	} i;
	U8 b;
} ChCtrl;

typedef struct {
		U8	play:1;		// 0 - stall, 1 - play
		U8	dir:1;		// 0 - forward, 1 - backward
} SStat;

typedef struct {
	U8		*Addr;              // Address of next DAC sample to read
	U8		*EndAddr;           // End Address of Sample
	U8		*LoopAddr;          // Loop Address of Sample
	U32		NewAddr;            // New Start Address (reloaded at the start of SSG burst)
	U32		NewEndAddr;         // New End Address
	U32		NewLoopAddr;		// New Loop Address
	W16		SubAddr;			// Current Sub-Address
	U16		SubStep;			// Increment for Sub-Address
	U16		NewSubStep;			// New Increment for Sub-Address
	W16		UserSamp;			// User Sample
	W32		UserOffset;			// User Sample Offset
	W16		UserFreq;			// User Frequency for sample playback (steps are calculated on the flow, actualized at next Tick)
	S8		IntL;				// Interpolation value for Left Audio Channel (previous DAC sample)
	S8		IntR;				// Interpolation value for Right Audio Channel
	U8		SampType;			// Sample Type: bit0 - mono/stereo, bit1 - 8bit/16bit, bit2 - unsigned/signed
	U8		Step;				// Increment for Address
	U8		StepA;				// Increment for Address coerced to Sample parameters
	U8		StepC;				// Next DAC Sample Address coerced to Sample parameters
	U8		NewStep;			// New Increment for Address
	ChCtrl	Control;			// Sample Control: Activeness, Type of Sample Loop, Sample Mode
	ChCtrl	NewControl;			// New Sample Control (taken from Sample Descriptor)
	SStat	State;				// Current state of Sample processing
	U8		VolL;				// Sample Volume in Left Audio Channel (0-64)
	U8		VolR;				// Sample Volume in Right Audio Channel (0-64)
	U8		NewVolL;			// New Sample Volume in Left Audio Channel
	U8		NewVolR;			// New Sample Volume in Right Audio Channel
	U8		isUserFreq;			// Indicates that Steps should be updated from User Frequency
	U8		isUserSamp;			// Indicates that Sample should be updated from User Sample
	U8		padding[7];			// ! Always calculate padding value to align this structure by the power of 2 size
} SSG_Regs;		//--- SSG Registers and Variables -----

#define		SAMPLE_MONO_STEREO	0x01
#define		SAMPLE_8_16_BIT		0x02
#define		SAMPLE_SIGNED_UNSG	0x04

#define		CH_LP_OFF			0x00
#define		CH_LP_FWD			0x01
#define		CH_LP_BIDI			0x02


// --- Functions prototypes -----
void 	SSG_Init(void);
void 	SSG_Tick(void);
DAC_Sum	SSG_Chan_Tick(int);

