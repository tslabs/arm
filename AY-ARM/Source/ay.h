
// Internal register definition file
//
// (c) 2012, TS-Labs inc.
// All rights fucked out
//
// A lány még mindig nagyon szép, de hol van már a vágy?

#pragma once
#pragma anon_unions

#include "types.h"

// --- Structures definition -----


typedef union
{
	struct
	{
		U8	t0:1;
		U8	t1:1;
		U8	t2:1;
		U8	n0:1;
		U8	n1:1;
		U8	n2:1;
		U8	i0:1;
		U8	i1:1;
	};
	U8 b;
} AYMix_t;


typedef union
{
	struct
	{
		U8	vol:5;
		U8	mode:1;
		U8	padding:2;
	};
	U8 b;
} AYVol_t;


typedef struct
{	//registers re-ordered to match memory alignment
	U32		sd_ns;		// Seed for Noise Generator
	W16		TF0;		// AY Reg #00, #01 - Tone frequency for CH0
	W16		TF1;		// AY Reg #02, #03 - Tone frequency for CH1
	W16		TF2;		// AY Reg #04, #05 - Tone frequency for CH2
	W16		EP;			// AY Reg #0B, #0C - Envelope period
	U16		ctr_tn0;	// Tone Generator Counter0
	U16		ctr_tn1;	// Tone Generator Counter1
	U16		ctr_tn2;	// Tone Generator Counter2
	U16		ctr_ev; 	// Envelope Period Counter
	U8		NF;			// AY Reg #06 - Noise frequency
	AYMix_t	MX;			// AY Reg #07 - Channels mux, I/O mode, kept inverted (!!!)
	AYVol_t	V0;			// AY Reg #08 - Volume for CH0
	AYVol_t	V1;			// AY Reg #09 - Volume for CH1
	AYVol_t	V2;			// AY Reg #0A - Volume for CH2
	U8		EC;			// AY Reg #0D - Envelope control
	U8		IOA;		// AY Reg #0E - I/O port A
	U8		IOB;		// AY Reg #0F - I/O port B
	U8		ctr_ns;		// Noise Generator Counter
	U8		env_rld;	// Reload of register 0D
	U8		env_sel;	// Envelope table selector (0-7)
	U8		env_ctr;	// Counter of Envelope phase (0-31)
	U8		env_ph;		// Phase of Envelope (0 - down, 1 - up). 'Up' means move forward by the envelope table
	U8		env_st;		// State of Envelope (0 - stall, 1 - run)
	U8		vol_0l;		// Channel A Volume Left	(0-64)
	U8		vol_0r;		// Channel A Volume Right
	U8		vol_1l;		// Channel B Volume Left
	U8		vol_1r;		// Channel B Volume Right
	U8		vol_2l;		// Channel C Volume Left
	U8		vol_2r;		// Channel C Volume Right
	U8		ph_tn0;		// Phase for Tone Generator0 (0 - off, 1 - on)
	U8		ph_tn1;		// Phase for Tone Generator1 (0 - off, 1 - on)
	U8		ph_tn2;		// Phase for Tone Generator2 (0 - off, 1 - on)
	U8		padding[21];
} AY_Regs;		//--- AY Registers and Variables-----

typedef union
{
	struct
	{
		U8	intfrq:3;	// internal frequency (see. Excel)
		U8	trben:1;	// 0 - turbo scheme disabled, 1 - enabled
		U8	selen:1;	// 0 - SEL pin disables, 1 - enabled
		U8	iclk:1;		// 0 - ext pin, 1 - int clock
		U8	padding:1;
		U8	type:1;		// 0 - AY / 1 - YM
	};
	U8 b;
} AYCtrl;

enum
{
    AY_TYP_AY = 0,
    AY_TYP_YM
};

// --- Functions prototypes -----
void AY_Init(void);
void AY_Tick(void);
void AY_Tick_tone(int);
void AY_Tick_noise(int);
void AY_Tick_env(int, int, int);
void AY_Env_Proc(int, int, int);
DAC_Sum AY_DAC_Sum(int);
