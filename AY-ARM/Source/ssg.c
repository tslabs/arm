
// Audio output functions
//
// (c) 2012, TS-Labs inc.
// All rights fucked out
//
// Nav tumša upe lāpas staros

/*
	SSG can process up to specified number of wave forms (samples) and to mix them into the left/right
	audio output DACs with precision 12 bits per audio channel.
	The SSG supports samples with such properties:
		- 8/16 bits
		- signed/unsigned
		- mono/stereo.
	Regardless of the sample bitness, it is read as a 8 bit value.
*/


#include <stdio.h>
#include <string.h>
#include "types.h"
#include "ay-arm.h"
#include "ssg.h"


// --- Variables declaration -----
		SSG_Regs	SSG[SSG_CH_MAX];
		U16			SSGFreq = SSG_FREQ;
		U8			SSGChNum = SSG_CH_DEF;
		DAC_t		DACOut;
		U16			TickCtr;		// Ticks counter


// --- Functions -----

// Initialize SSG
void SSG_Init(void) {
	U8 i;
	for (i=0; i<SSG_CH_MAX; i++) {
		memset(&SSG[i], 0, sizeof(SSG_Regs));
	}
}


// SSG tick
void SSG_Tick(void) {
	int i;
	DAC_Sum Sum;
	Sum.l = Sum.r = 0;

	// Iterate all SSG Channels tick
	for (i=0; i<SSGChNum; i++) {
		Sum = SSG_Chan_Tick(i);
	}

	// Normalizing to 16 bits as it was multiplied by 6 bits of volume

	// Cutting upper and lower dynamic limits
	Sum.l >>= 6;
	if (Sum.l < -32768)
		Sum.l = -32768;
	else if (Sum.l > 32767)
		Sum.l = 32767;
	DACOut.l =  Sum.l ^ 0x8000;		// convert to unsigned

	Sum.r >>= 6;
	if (Sum.r < -32768)
		Sum.r = -32768;
	else if (Sum.r > 32767)
		Sum.r = 32767;
	DACOut.r =  Sum.r ^ 0x8000;	// convert to unsigned
}


__inline DAC_Sum SSG_Chan_Tick(int chan) {
	S8 s1, s2;			// 8 bit signed read DAC samples
	S16 sl, sr;			// 16 bit signed interpolated DAC samples
	U32 tmp32;
	DAC_Sum Sum;
	
	// Check if channel active and its state is not 'stall'
	if (SSG[chan].Control.i.act && SSG[chan].State.play) {

		s1 = *SSG[chan].Addr;			// read next DAC sample (left)
		if (SSG[chan].Control.i.sgn)
			s1 = s1 ^ 0x80;				// convert to signed if need

		if (SSG[chan].Control.i.chn) {		// if sample is stereo
			s2 = *(SSG[chan].Addr + SSG[chan].StepC);	// read next DAC sample (right)
			if (SSG[chan].Control.i.sgn)
				s2 = s2 ^ 0x80;			// convert to signed if need
		}
		else
			s2 = s1;	// if mono - just use left for right
			
		// Calculate linear interpolation
		sl = SSG[chan].IntL * (0x100 - SSG[chan].SubAddr.b.b1) + s1 * SSG[chan].SubAddr.b.b1;
		sr = SSG[chan].IntR * (0x100 - SSG[chan].SubAddr.b.b1) + s2 * SSG[chan].SubAddr.b.b1;

		// Multiply DSC samples by volume values and add them to DAC summators
		Sum.l += sl * SSG[chan].VolL;
		Sum.r += sr * SSG[chan].VolR;

		// Process Address
		tmp32 = (SSG[chan].SubAddr.h + SSG[chan].SubStep);
		SSG[chan].SubAddr.h = (U16)tmp32;
		
		if (tmp32 > 0xFFFF) {		// If actual address changed
			
			// Store new DAC samples as interpolation values
			SSG[chan].IntL = s1;
			SSG[chan].IntR = s2;
		
			// Move forward
			if (!SSG[chan].State.dir) {
				SSG[chan].Addr += SSG[chan].StepA;
				
				// Check if moved out from the sample end point
				if (SSG[chan].Addr > SSG[chan].EndAddr) {
				
					// Loop of a 'forward' type
					if (SSG[chan].Control.i.loop == CH_LP_FWD) {
						SSG[chan].Addr = SSG[chan].LoopAddr;		// set addr to loop start
					}
					
					// Loop of a 'bidi' type
					else if (SSG[chan].Control.i.loop == CH_LP_BIDI) {
						SSG[chan].Addr = SSG[chan].EndAddr;			// set addr to sample end
						SSG[chan].State.dir = 1;					// change direction to backwards
					}
					
					// No loop
					else {
						SSG[chan].State.play = 0;					// turn off the light
					}
				}
			}
		
			// Move backward
			else {
				SSG[chan].Addr -= SSG[chan].StepA;
				
				// Check if moved out from the loop point
				// (the only way to get here is to have a 'bidi' loop, so no check of loop type needed)
				if (SSG[chan].Addr < SSG[chan].LoopAddr) {
					SSG[chan].Addr = SSG[chan].LoopAddr;		// set addr to loop start
					SSG[chan].State.dir = 0;					// change direction to forwards
				}
			}
		}
	}
	
	return Sum;
}
