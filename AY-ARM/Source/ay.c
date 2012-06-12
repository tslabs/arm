
// AY Chip related functions
//
// (c) 2012, TS-Labs inc.
// All rights fucked out
//
// Sajnálom, de kizökkentetted a felséges urat


// --- Includes -----
#include <stdio.h>
#include <string.h>
#include "types.h"
#include "ay-arm.h"
#include "ay.h"


// --- Vars declaration -----
AY_Regs ayregs[NUM_AY_CHIPS];	// Registers for virtual AY chips
AY_Vars ayvars[NUM_AY_CHIPS];	// Variables for virtual AY chips processing


// Initialize AY Chips
void AY_Init()	{
	int i;
	for (i=0; i<NUM_AY_CHIPS; i++)	{
		memset(&ayregs[i], 0, sizeof(AY_Regs));
		memset(&ayvars[i], 0, sizeof(AY_Vars));
	}
}


// Iterate next AY tick for all AY-chips
// Called at F_osc/16 IRQ
void AY_tick()	{
	int i;
	for (i=0; i<NUM_AY_CHIPS; i++)	{
		AY_tick_noise(i);
		AY_tick_tone(i);
	}
		AY_DAC_out();
}

