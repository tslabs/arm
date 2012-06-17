
// AY Chip related functions
//
// (c) 2012, TS-Labs inc.
// All rights fucked out
//
// Sajnálom, de kizökkentetted a felséges urat


#include <stdio.h>
#include <string.h>
#include "types.h"
#include "ay-arm.h"
#include "ay.h"


// --- Variables declaration -----
		AY_Regs AY[AY_CHIPS_MAX];	// Registers for virtual AY chips
		U8		AYChipNum = AY_CHIPS_DEF;
		int		env_div = 0;


// --- Functions -----

// Initialize AY Chips
void AY_Init() {
	int i;
	// Registers nulling
	for (i=0; i<AY_CHIPS_MAX; i++) {
		memset(&AY[i], 0, sizeof(AY_Regs));
	}
}


// Iterate next tick for all AY-chips
// Called at F_osc/16 IRQ
void AY_Tick() {

	int i;
	for (i=0; i<AYChipNum; i++) {
		AY_Tick_tone(i);
		AY_Tick_noise(i);
		if (env_div & 16)
			AY_Tick_env(i);
	}

	env_div++;
	//AY_DAC_out();
}


// Iterate next tick of AY Tone Generators
void AY_Tick_tone(int n) {
	if (AY[n].ctr_tn0 < AY[n].TF0.h)
		AY[n].ctr_tn0++;
	else {
		AY[n].ph_tn0 ^= 0xFF;
		AY[n].ctr_tn0 = 1;
	}

	if (AY[n].ctr_tn1 < AY[n].TF1.h)
		AY[n].ctr_tn1++;
	else {
		AY[n].ph_tn1 ^= 0xFF;
		AY[n].ctr_tn1 = 1;
	}

	if (AY[n].ctr_tn2 < AY[n].TF2.h)
		AY[n].ctr_tn2++;
	else {
		AY[n].ph_tn2 ^= 0xFF;
		AY[n].ctr_tn2 = 1;
	}
}


// Iterate next tick of AY Noise Generator
void AY_Tick_noise(int n) {
	if (AY[n].ctr_ns < AY[n].NF)
		AY[n].ctr_ns++;
	else {
		AY[n].sd_ns = ((AY[n].sd_ns << 1) | 1) ^ (((AY[n].sd_ns >> 16) ^ (AY[n].sd_ns >> 13)) & 1);
		// ^^^ This was spizded from 'ZXTUNES' project by Vitamin/CAIG
		AY[n].ctr_ns = 1;		// !!! check this on a real chip !!!
	}
}


// Iterate next tick of AY Volume Envelope Generator
void AY_Tick_env(int n) {
	if (AY[n].ctr_ev < AY[n].EP.h)
		AY[n].ctr_ev++;
	else {
		AY[n].ctr_ev = 1;

	// to do: envelope processing
	}
}

