
// Audio output functions definitions
//
// (c) 2012, TS-Labs inc.
// All rights fucked out
//
// I wtedy wrescie sam tez wlacze sie do akcji


#ifndef __AUDIO_H
#define __AUDIO_H


#include "types.h"


// --- Structures definition -----
typedef union {
	struct {
		U8	act:1;		// channel is active
		U8	chn:1;		// 0 - mono, 1 - stereo
		U8	sgn:1;		// 0 - unsigned, 1 - signed
		U8	bw:1;		// 0 - 8 bit, 1 - 16 bit
		U8	padding:2;	// just padding to maintain 8 bit width
		U8	loop:2;		// 0 - off, 1 - forward, 2 - bidi, 3 - reserved
	} i;
	U8 b;
} ChCtrl;


typedef struct {
	U32		Addr;               // Address of next DAC sample to read
	U32		EndAddr;            // End Address of Sample
	U32		LoopAddr;           // Loop Address of Sample
	U32		NewAddr;            // New Start Address (reloaded at the start of SSG burst)
	U32		NewEndAddr;         // New End Address
	U32		NewLoopAddr;		// New Loop Address
	U16		SubAddress;			// Current Sub-Address
	U16		SubStep;			// Increment for Sub-Address
	U16		NewSubStep;			// New Increment for Sub-Address
	W16		UserSamp;			// User Sample
	W32		UserOffset;			// User Sample Offset
	W16		UserFreq;			// User Frequency for sample playback (steps are calculated on the flow, actualized at next Tick)
	U16		DACRight;			// DAC value for Right Audio Channel
	S8		IntLeft;			// Interpolation value for Left Audio Channel (previous DAC sample)
	S8		IntRight;			// Interpolation value for Right Audio Channel
	U8		SampType;			// Sample Type: bit0 - mono/stereo, bit1 - 8bit/16bit, bit2 - unsigned/signed
	U8		Step;				// Increment for Address
	U8		NewStep;			// New Increment for Address
	ChCtrl	Control;			// Sample Control: Activeness, Type of Sample Loop, Sample Mode
	ChCtrl	NewControl;			// New Sample Control (taken from Sample Descriptor)
	U8		Dir;				// Current state of Sample processing: 0 - stall, 1 - forward, 2 - backward
	U8		VolL;				// Sample Volume in Left Audio Channel
	U8		VolR;				// Sample Volume in Right Audio Channel
	U8		NewVolL;			// New Sample Volume in Left Audio Channel
	U8		NewVolR;			// New Sample Volume in Right Audio Channel
	U8		isUserFreq;			// Indicates that Steps should be updated from User Frequency
	U8		isUserSamp;			// Indicates that Sample should be updated from User Sample
	U8		padding[7];		// ! Always calculate padding value to align this structure by the power of 2 size
} SSG_Regs;		//--- SSG Registers and Variables -----

#define		SAMPLE_MONO_STEREO	0x01
#define		SAMPLE_8_16_BIT		0x02
#define		SAMPLE_SIGNED_UNSG	0x04


typedef struct {
	U16	left;
	U16	right;
} DAC_LR;


// --- Functions prototypes -----
void SSG_Init(void);
void SSG_Tick(void);
void SSG_Chan_Tick(U8);


#endif /* __AUDIO_H */

