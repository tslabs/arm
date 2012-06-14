
// AY-ARM main module
//
// (c) 2012, TS-Labs inc.
// All rights fucked out
//
// Demoscene is alive!!!


//--- Includes ------

//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
#include "stm32f4xx.h"
#include "types.h"
#include "ay-arm.h"
#include "ay.h"
#include "zx.h"
#include "main.h"


//--- Main function -----
// Executed after reset

int main (void) {

	AY_Init();

	while(1)	{
		AY_tick();
		WReg(0);
		WReg(1);
		RReg(1);
	}
}
