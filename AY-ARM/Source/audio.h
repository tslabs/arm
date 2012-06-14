
// Audio output functions definitions
//
// (c) 2012, TS-Labs inc.
// All rights fucked out
//
// I wtedy wrescie sam tez wlacze sie do akcji


#ifndef __AUDIO_H
#define __AUDIO_H


// --- Structures definition -----
typedef struct
{
	U32		Addr;               // Address of next DAC sample to read
	U32		EndAddr;            // End Address of Sample
	U32		LoopAddr;           // Loop Address of Sample
	U32		NewAddr;            // New Start Address (reloaded at the start of SSG burst)
	U32		NewEndAddr;         // New End Address
	U32		NewLoopAddr;		// New Loop Address
	U16		SubAddress;			// Current Sub-Address
	U16		SubStep;			// Increment for Sub-Address
	U16		NewSubStep;			// New Increment for Sub-Address
	U16		IntLeft;			// Interpolation value for Left Audio Channel (previous DAC sample)
	U16		IntRight;			// Interpolation value for Right Audio Channel
	U16		DACLeft;			// DAC value for Left Audio Channel
	U16		DACRight;			// DAC value for Right Audio Channel
	U8		SampType;			// Sample Type: bit0 - mono/stereo, bit1 - 8bit/16bit, bit2 - unsigned/signed
	U8		Step;				// Increment for Address
	U8		NewStep;			// New Increment for Address
	U8		Reload;				// Flag for reload of channel parameters at the start of SSG burst
	U8		LoopType;			// Type of Sample Loop: 0 - off, 1 - on, 2 - bidi
	U8		Dir;				// Current state of Sample processing: 0 - stall, 1 - forward, 2 - backward
	U8		VolL;				// Sample Volume in Left Audio Channel
	U8		VolR;				// Sample Volume in Right Audio Channel
} SSG_Regs;		//--- SSG Registers (for one channel) -----

#define		SAMPLE_MONO_STEREO	0x01
#define		SAMPLE_8_16_BIT		0x02
#define		SAMPLE_SIGNED_UNSG	0x04


// --- Functions prototypes -----


#endif /* __AUDIO_H */

