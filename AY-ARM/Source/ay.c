
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
		AY_Regs ayregs[NUM_AY_CHIPS];	// Registers for virtual AY chips
		AY_Vars ayvars[NUM_AY_CHIPS];	// Variables for virtual AY chips processing
		int		env_div;


// Initialize AY Chips
void AY_Init()	{
	int i;
	i = sizeof(AY_Regs);
	
	for (i=0; i<NUM_AY_CHIPS; i++)	{
		memset(&ayregs[i], 0, sizeof(AY_Regs));
		memset(&ayvars[i], 0, sizeof(AY_Vars));
		env_div = 0;
	}
}


// Iterate next tick for all AY-chips
// Called at F_osc/16 IRQ
void AY_tick()	{

	int i;
	for (i=0; i<NUM_AY_CHIPS; i++)	{
		AY_tick_tone(i);
		AY_tick_noise(i);
		if (env_div & 16)
			AY_tick_env(i);
	}
	
	env_div++;
	//AY_DAC_out();
}


// Iterate next tick of AY Tone Generators
void AY_tick_tone(int n)	{
	if (ayvars[n].ctr_tn0 < ayregs[n].TF0.w)
		ayvars[n].ctr_tn0++;
	else	{
		ayvars[n].ph_tn0 ^= 1;
		ayvars[n].ctr_tn0 = 1;
	}
	
	if (ayvars[n].ctr_tn1 < ayregs[n].TF1.w)
		ayvars[n].ctr_tn1++;
	else	{
		ayvars[n].ph_tn1 ^= 1;
		ayvars[n].ctr_tn1 = 1;
	}

	if (ayvars[n].ctr_tn2 < ayregs[n].TF2.w)
		ayvars[n].ctr_tn2++;
	else	{
		ayvars[n].ph_tn2 ^= 1;
		ayvars[n].ctr_tn2 = 1;
	}
}


// Iterate next tick of AY Noise Generator
void AY_tick_noise(int n)	{
	if (ayvars[n].ctr_ns < ayregs[n].NF)
		ayvars[n].ctr_ns++;
	else	{
		ayvars[n].sd_ns = ((ayvars[n].sd_ns << 1) | 1) ^ (((ayvars[n].sd_ns >> 16) ^ (ayvars[n].sd_ns >> 13)) & 1);
		ayvars[n].ctr_ns = 1;		// !!! check this on a real chip !!!
	}
}


// Iterate next tick of AY Volume Envelope Generator
void AY_tick_env(int n)	{
	if (ayvars[n].ctr_ev < ayregs[n].EP.w)
		ayvars[n].ctr_ev++;
	else	{
		ayvars[n].ctr_ev = 1;
		
	// envelope processing	
	}
}

