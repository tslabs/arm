
// AY-ARM internals and parameters definition file
//
// (c) 2012, TS-Labs inc.
// All rights fucked out
//
// Mammals, happy mammaling!


// <<< Use Configuration Wizard in Context Menu >>> 


#ifndef __AY_ARM_H
#define __AY_ARM_H


//--- AY-related parameters -----

//	<o> Max number of AY chips emulated <1-16>
#define NUM_AY_CHIPS	0x10

//	<o> Mask for AY chip selector <0x0-0xF>
#define MASK_AY_CHIPS	0x0F


//--- Audio parameters -----
//	<o> Output audio samplerate, Hz <8000-50000>
#define SAMP_FREQ	44100


#endif /* __AY_ARM_H */
