
// AY-ARM main module
//
// (c) 2014, TS-Labs inc.
// All rights fucked out
//
// Demoscene is alive!!!


//--- Includes ---

//#include <stdio.h>
//#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "hw.h"
#include "interrupts.h"
#include "ay-arm.h"
#include "ay.h"
#include "ws.h"
#include "zx.h"
#include "main.h"


//--- Main function (RESET) ---
// Executed after reset

extern	U8 AYChipNum;
extern	WS_Regs WS[WS_CH_MAX];

int main (void) {

	// memset((U8*)0x20000000, 0xCC, 0x10000);
	GPIO_Init();
	Int_Init();
	
	while(1);
	{
		int t;
        // int t = (GPIOA->IDR & 2) ? 1:0;
		// GPIO_Bit_Set(D, 15, t);
        t = ~t;
	};
	
	AY_Init();
	WS_Init();
	CmdF(0xF1);
	WAddr(0xE2);

	WS[0].Addr = (U8*)0x08000000;
	WS[0].EndAddr = (U8*)0x08000010;
	WS[0].LoopAddr = (U8*)0x08000005;
	WS[0].SubAddr.h = 0x9000;
	WS[0].Control.act = 1;
	WS[0].Control.chn = 1;
	WS[0].Control.sgn = 1;
	WS[0].Control.bw = 1;
	WS[0].Control.loop = 1;
	WS[0].State.play = 1;
	WS[0].State.dir = 0;
	WS[0].SubStep = 0x8000;
	WS[0].Step = 1;
	WS[0].StepA = 4;
	WS[0].StepC = 2;
	WS[0].VolL = 64;
	WS[0].VolR = 64;
	
	WAddr(0x0D);
	WReg(0x0E);

	while(1)
	{
		// U32 t = a + b;


		AY_Tick();
		//WS_Tick();
		//i = RReg();
	}
}


// ---------- Scratchpad -----------

			// Calc addr increment from step
			// = WS[chan].Control.chn ? (WS[chan].Control.bw ? (WS[chan].Step << 2) : (WS[chan].Step << 1)) : (WS[chan].Control.bw ? (WS[chan].Step << 1) : WS[chan].Step);
