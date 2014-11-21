
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

    volatile U32 idle_cnt;
    volatile U8 idle_cnt_upd;

int main (void)
{

	GPIO_Init();
	AY_Init();
	WS_Init();
	Int_Init();

	while(1)
    {
        if (idle_cnt_upd)
        {
            idle_cnt = 0;
            idle_cnt_upd = 0;
        }
        
        else
            idle_cnt++;
    }
}


// ---------- Scratchpad -----------

			// Calc addr increment from step
			// = WS[chan].Control.chn ? (WS[chan].Control.bw ? (WS[chan].Step << 2) : (WS[chan].Step << 1)) : (WS[chan].Control.bw ? (WS[chan].Step << 1) : WS[chan].Step);
