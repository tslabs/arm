
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
// <h> AY Section

//	<o> Max number of AY chips emulated <1-16>
#define NUM_AY_CHIPS	0x10

//	<o> Mask for AY chip selector <0x00=> 1 chip <0x01=> 2 chips <0x03=> 4 chips <0x07=> 8 chips <0x0F=> 16 chips
#define MASK_AY_CHIPS	0x0F

//	<o> Mask for Turbo AY coarse decoding <0xF0=> #F0-#FF <0xF8=> #F8-#FF <0xFE=> #FE-#FF
#define MASK_TURBO_AY	0xF8

// </h>


//--- Audio parameters -----
// <h> Digital Section

//	<o> Default output audio samplerate, Hz <8000-50000>
#define SAMP_FREQ	44100

// </h>


#endif /* __AY_ARM_H */
