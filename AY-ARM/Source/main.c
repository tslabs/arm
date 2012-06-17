
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
#include "ssg.h"
#include "zx.h"
#include "main.h"


//--- Main function -----
// Executed after reset

int i;
extern U8 AYChipNum;

int main (void) {

	i = AYChipNum;
	
	AY_Init();
	SSG_Init();
	//CmdF(0xF1);
	//WAddr(0xE2);

	while(1)	{
		// AY_Tick();
		SSG_Tick();
		//i = RReg();
	}
}
