
// Audio output functions
//
// (c) 2012, TS-Labs inc.
// All rights fucked out
//
// Nav tumša upe lāpas staros

/*
	WS can process up to specified number of wave forms (samples) and to mix them into the left/right
	audio output DACs with precision 12 bits per audio channel.
	The WS supports samples with such properties:
		- 8/16 bits
		- signed/unsigned
		- mono/stereo.
	Regardless of the sample bitness, it is read as a 8 bit value.
*/


#include <stdio.h>
#include <string.h>
#include "types.h"
#include "ay-arm.h"
#include "ws.h"


// --- Variables declaration -----
		WS_Regs	WS[WS_CH_MAX];
		U16			WSFreq = WS_FREQ;
		U8			WSChNum = WS_CH_DEF;
		DAC_t		DACOut;
		U16			TickCtr;		// Ticks counter


// --- Functions -----

// Initialize WS
void WS_Init(void) {
	U8 i;
	for (i=0; i<WS_CH_MAX; i++) {
		memset(&WS[i], 0, sizeof(WS_Regs));
	}
}


// WS tick
void WS_Tick(void) {
	int i;
	DAC_Sum Sum;
	Sum.l = Sum.r = 0;

	// Iterate all WS Channels tick
	for (i=0; i<WSChNum; i++) {
		Sum = WS_Chan_Tick(i);
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


__inline DAC_Sum WS_Chan_Tick(int chan) {
	S8 s1, s2;			// 8 bit signed read DAC samples
	S16 sl, sr;			// 16 bit signed interpolated DAC samples
	U32 tmp32;
	DAC_Sum Sum;
	
	// Check if channel active and its state is not 'stall'
	if (WS[chan].Control.act && WS[chan].State.play) {

		s1 = *WS[chan].Addr;			// read next DAC sample (left)
		if (WS[chan].Control.sgn)
			s1 = s1 ^ 0x80;				// convert to signed if need

		if (WS[chan].Control.chn) {		// if sample is stereo
			s2 = *(WS[chan].Addr + WS[chan].StepC);	// read next DAC sample (right)
			if (WS[chan].Control.sgn)
				s2 = s2 ^ 0x80;			// convert to signed if need
		}
		else
			s2 = s1;	// if mono - just use left for right
			
		// Calculate linear interpolation
		sl = WS[chan].IntL * (0x100 - WS[chan].SubAddr.b1) + s1 * WS[chan].SubAddr.b1;
		sr = WS[chan].IntR * (0x100 - WS[chan].SubAddr.b1) + s2 * WS[chan].SubAddr.b1;

		// Multiply DSC samples by volume values and add them to DAC summators
		Sum.l += sl * WS[chan].VolL;
		Sum.r += sr * WS[chan].VolR;

		// Process Address
		tmp32 = (WS[chan].SubAddr.h + WS[chan].SubStep);
		WS[chan].SubAddr.h = (U16)tmp32;
		
		if (tmp32 > 0xFFFF) {		// If actual address changed
			
			// Store new DAC samples as interpolation values
			WS[chan].IntL = s1;
			WS[chan].IntR = s2;
		
			// Move forward
			if (!WS[chan].State.dir) {
				WS[chan].Addr += WS[chan].StepA;
				
				// Check if moved out from the sample end point
				if (WS[chan].Addr > WS[chan].EndAddr) {
				
					// Loop of a 'forward' type
					if (WS[chan].Control.loop == CH_LP_FWD) {
						WS[chan].Addr = WS[chan].LoopAddr;		// set addr to loop start
					}
					
					// Loop of a 'bidi' type
					else if (WS[chan].Control.loop == CH_LP_BIDI) {
						WS[chan].Addr = WS[chan].EndAddr;			// set addr to sample end
						WS[chan].State.dir = 1;					// change direction to backwards
					}
					
					// No loop
					else {
						WS[chan].State.play = 0;					// turn off the light
					}
				}
			}
		
			// Move backward
			else {
				WS[chan].Addr -= WS[chan].StepA;
				
				// Check if moved out from the loop point
				// (the only way to get here is to have a 'bidi' loop, so no check of loop type needed)
				if (WS[chan].Addr < WS[chan].LoopAddr) {
					WS[chan].Addr = WS[chan].LoopAddr;		// set addr to loop start
					WS[chan].State.dir = 0;					// change direction to forwards
				}
			}
		}
	}
	
	return Sum;
}
