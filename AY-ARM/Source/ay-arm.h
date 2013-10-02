
// AY-ARM internals and parameters definition file
//
// (c) 2012, TS-Labs inc.
// All rights fucked out
//
// Mammals, happy mammaling!

#pragma once


// <<< Use Configuration Wizard in Context Menu >>> 

//--- Chip-related parameters -----
// <h> Chip

//	<o> Internal SRAM Size, kB <1-112>
#define I_SRAM_SIZE	112

//	<o> External SRAM Size, kB <1-2048>
#define E_SRAM_SIZE	2048

//	<o> Address of Wave Buffer <0x2001C000-0x2001F000>
#define WBUF_ADDR	0x2001C000

// </h>


//--- AY parameters -----
// <h> AY

//	<o> Max number of AY chips emulated <1-16>
#define AY_CHIPS_MAX	16

//	<o> Default number of AY chips emulated <1-16>
#define AY_CHIPS_DEF	8

//	<o> Range for Turbo AY decoding <0xF0=> #F0-#FF <0xF8=> #F8-#FF <0xFE=> #FE-#FF
#define MASK_TURBO_AY	0xF8

// </h>


//--- SSG parameters -----
// <h> SSG

//	<o> Default output audio samplerate, Hz <8000-50000>
#define SSG_FREQ	44100

//	<o> Max number of SSG channels <1-64>
#define SSG_CH_MAX	64

//	<o> Default number of SSG channels <1-64>
#define SSG_CH_DEF	32

// </h>


//--- F/W update parameters -----
// <h> F/W update

//	<o> Command 'FW Mode' Constant <0x0-0xFFFFFFFF:32>
#define MAGIC_FWM	0x552A773C

//	<o> Command 'Flash Firmware' Constant <0x0-0xFFFFFFFF:32>
#define MAGIC_FFW	0x7841AA55

// </h>


//--- Version parameters -----
// <h> Version

//	<s> Chip Version String
#define	CHIP_STRING	"AY-ARM Sound Chip, v.1.00"

//	<s> Copyright String
#define	CPR_STRING	"(c)2012 TS-Labs inc."

//	<o> Numeric Version <0-255:8>
#define NUM_VER		1

//	<o> Numeric Sub-Version <1-255:8>
#define NUM_SVER	0

//	<o> Device Byte0 <0x00-0xFF:8>
#define DEV_BYTE0	0x55

//	<o> Device Byte1 <0x00-0xFF:8>
#define DEV_BYTE1	0xAA

// </h>

