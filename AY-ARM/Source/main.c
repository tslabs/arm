
// AY-ARM main module
//
// (c) 2012, TS-Labs inc.
// All rights fucked out
//
// Demoscene is alive!!!


//--- Includes ------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f4xx.h"


//--- Main function -----

	int i, j;

int main (void) {

	while(1)	{
		i = i+1;
		for (j=0; j<i; j++);
	};

}
