
// Internal register definition file
//
// (c) 2012, TS-Labs inc.
// All rights fucked out
//
// A lány még mindig nagyon szép, de hol van már a vágy?


#ifndef __AY_H
#define __AY_H


// --- Structures definition -----


typedef struct {	//registers re-ordered to match memory alignment
	W16		TF0;	// #00, #01 - Tone frequency for CH0
	W16		TF1;	// #02, #03 - Tone frequency for CH1
	W16		TF2;	// #04, #05 - Tone frequency for CH2
	W16		EP;		// #0B, #0C - Envelope period
	U8		NF;		// #06 - Noise frequency
	U8		MX;		// #07 - Channels mux, I/O mode
	U8		V0;		// #08 - Volume for CH0
	U8		V1;		// #09 - Volume for CH1
	U8		V2;		// #0A - Volume for CH2
	U8		EC;		// #0D - Envelope control
	U8		IOA;	// #0E - I/O port A
	U8		IOB;	// #0F - I/O port B
} AY_Regs;		//--- AY Registers -----


typedef struct {
	U32		sd_ns;		// Seed for Noise Generator
	U16		ctr_tn0;	// Tone Generator Counter0
	U16		ctr_tn1;	// Tone Generator Counter1
	U16		ctr_tn2;	// Tone Generator Counter2
	U16		ctr_ev; 	// Envelope Period Counter
	U8		ctr_ns;		// Noise Generator Counter
	U8		ph_tn0;		// Phase for Tone Generator0 (0 - off, 1 - on)
	U8		ph_tn1;		// Phase for Tone Generator1 (0 - off, 1 - on)
	U8		ph_tn2;		// Phase for Tone Generator2 (0 - off, 1 - on)
	U8		ph_ev;		// Phase of Envelope (00 - down, 01 - up, 10 - fall, 11 - rise)
	U8		vol_ev;		// Current Volume Level of Envelope
} AY_Vars;		//--- AY Variables -----


// --- Functions prototypes -----
void AY_Init(void);
void AY_tick(void);
void AY_tick_tone(int n);
void AY_tick_noise(int n);
void AY_tick_env(int n);


#endif /* __AY_H */

