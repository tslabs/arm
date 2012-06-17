
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
		S32			SLeft;
		S32			SRight;
		DAC_LR		DACOut;


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
	U8 i;
	SLeft = 0;
	SRight = 0;
	
	for (i=0; i<SSGChNum; i++) {
		SSG_Chan_Tick(i);
	}
	
	if (SLeft < -32768)
		SLeft = -32768;
	else if (SLeft > 32767)
		SLeft = 32767;
	DACOut.left =  SLeft + 32768;
	
	if (SRight < -32768)
		SRight = -32768;
	else if (SRight > 32767)
		SRight = 32767;
	DACOut.right =  SRight + 32768;
}


// SSG Channel Tick
void SSG_Chan_Tick(U8 chan) {
	return;
}

