
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
#include "ay.h"


//--- Main function -----

	U8 i, j;
	AY_Regs ay[16];

int main (void) {

	while(1)	{
		i = i+1;
		if ((i&255)==0)
			j += 1;
		ay[i&15].TF0 = i;
	};

}
