
// ZX interface. AY chip registers reading/writing
//
// (c) 2012, TS-Labs inc.
// All rights fucked out
//
// 128kB is enough for everyone


#include "zx.h"
#include "ay-arm.h"
#include "ay.h"


// --- Variables declaration -----
		U8			reg_num;
		U8			wr_val;
		U8			rd_val;
extern	U8			ay_chip;
extern	AY_Regs		ayregs[NUM_AY_CHIPS];	// Registers for virtual AY chips


//------------------------------------------------------------------------------------------

// --- Interface Functions -----

// - Write Registers Functions -

void WDummy(U8 wr_val) {
	return;
}

void W00(U8 wr_val) {
	ayregs[ay_chip].TF0.b.l = wr_val;
	return;
}

void W01(U8 wr_val) {
	ayregs[ay_chip].TF0.b.h = wr_val;
	return;
}

void W02(U8 wr_val) {
	ayregs[ay_chip].TF1.b.l = wr_val;
	return;
}

void W03(U8 wr_val) {
	ayregs[ay_chip].TF1.b.h = wr_val;
	return;
}

void W04(U8 wr_val) {
	ayregs[ay_chip].TF2.b.l = wr_val;
	return;
}

void W05(U8 wr_val) {
	ayregs[ay_chip].TF2.b.h = wr_val;
	return;
}

void W06(U8 wr_val) {
	ayregs[ay_chip].NF = wr_val;
	return;
}

void W07(U8 wr_val) {
	ayregs[ay_chip].MX = wr_val;
	return;
}

void W08(U8 wr_val) {
	ayregs[ay_chip].V0 = wr_val;
	return;
}

void W09(U8 wr_val) {
	ayregs[ay_chip].V1 = wr_val;
	return;
}

void W0A(U8 wr_val) {
	ayregs[ay_chip].V2 = wr_val;
	return;
}

void W0B(U8 wr_val) {
	ayregs[ay_chip].EP.b.l = wr_val;
	return;
}

void W0C(U8 wr_val) {
	ayregs[ay_chip].EP.b.h = wr_val;
	return;
}

void W10(U8 wr_val) {
	ay_chip = wr_val & MASK_AY_CHIPS;
	return;
}

// Write Registers Functions Vectors
void (*WRegVec[0x100])(U8) = {
		W00,			// 0x00
		W01,			// 0x01
		W02,			// 0x02
		W03,			// 0x03
		W04,			// 0x04
		W05,			// 0x05
		W06,			// 0x06
		W07,			// 0x07
		W08,			// 0x08
		W09,			// 0x09
		W0A,			// 0x0A
		W0B,			// 0x0B
		W0C,			// 0x0C
		WDummy,			// 0x0D
		WDummy,			// 0x0E
		WDummy,			// 0x0F

		W10,			// 0x10 - AY-Chip Select
		WDummy,			// 0x11
		WDummy,			// 0x12
		WDummy,			// 0x13
		WDummy,			// 0x14
		WDummy,			// 0x15
		WDummy,			// 0x16
		WDummy,			// 0x17
		WDummy,			// 0x18
		WDummy,			// 0x19
		WDummy,			// 0x1A
		WDummy,			// 0x1B
		WDummy,			// 0x1C
		WDummy,			// 0x1D
		WDummy,			// 0x1E
		WDummy,			// 0x1F

		WDummy,			// 0x20
		WDummy,			// 0x21
		WDummy,			// 0x22
		WDummy,			// 0x23
		WDummy,			// 0x24
		WDummy,			// 0x25
		WDummy,			// 0x26
		WDummy,			// 0x27
		WDummy,			// 0x28
		WDummy,			// 0x29
		WDummy,			// 0x2A
		WDummy,			// 0x2B
		WDummy,			// 0x2C
		WDummy,			// 0x2D
		WDummy,			// 0x2E
		WDummy,			// 0x2F

		WDummy,			// 0x30
		WDummy,			// 0x31
		WDummy,			// 0x32
		WDummy,			// 0x33
		WDummy,			// 0x34
		WDummy,			// 0x35
		WDummy,			// 0x36
		WDummy,			// 0x37
		WDummy,			// 0x38
		WDummy,			// 0x39
		WDummy,			// 0x3A
		WDummy,			// 0x3B
		WDummy,			// 0x3C
		WDummy,			// 0x3D
		WDummy,			// 0x3E
		WDummy,			// 0x3F

		WDummy,			// 0x40
		WDummy,			// 0x41
		WDummy,			// 0x42
		WDummy,			// 0x43
		WDummy,			// 0x44
		WDummy,			// 0x45
		WDummy,			// 0x46
		WDummy,			// 0x47
		WDummy,			// 0x48
		WDummy,			// 0x49
		WDummy,			// 0x4A
		WDummy,			// 0x4B
		WDummy,			// 0x4C
		WDummy,			// 0x4D
		WDummy,			// 0x4E
		WDummy,			// 0x4F

		WDummy,			// 0x50
		WDummy,			// 0x51
		WDummy,			// 0x52
		WDummy,			// 0x53
		WDummy,			// 0x54
		WDummy,			// 0x55
		WDummy,			// 0x56
		WDummy,			// 0x57
		WDummy,			// 0x58
		WDummy,			// 0x59
		WDummy,			// 0x5A
		WDummy,			// 0x5B
		WDummy,			// 0x5C
		WDummy,			// 0x5D
		WDummy,			// 0x5E
		WDummy,			// 0x5F

		WDummy,			// 0x60
		WDummy,			// 0x61
		WDummy,			// 0x62
		WDummy,			// 0x63
		WDummy,			// 0x64
		WDummy,			// 0x65
		WDummy,			// 0x66
		WDummy,			// 0x67
		WDummy,			// 0x68
		WDummy,			// 0x69
		WDummy,			// 0x6A
		WDummy,			// 0x6B
		WDummy,			// 0x6C
		WDummy,			// 0x6D
		WDummy,			// 0x6E
		WDummy,			// 0x6F

		WDummy,			// 0x70
		WDummy,			// 0x71
		WDummy,			// 0x72
		WDummy,			// 0x73
		WDummy,			// 0x74
		WDummy,			// 0x75
		WDummy,			// 0x76
		WDummy,			// 0x77
		WDummy,			// 0x78
		WDummy,			// 0x79
		WDummy,			// 0x7A
		WDummy,			// 0x7B
		WDummy,			// 0x7C
		WDummy,			// 0x7D
		WDummy,			// 0x7E
		WDummy,			// 0x7F

		WDummy,			// 0x80
		WDummy,			// 0x81
		WDummy,			// 0x82
		WDummy,			// 0x83
		WDummy,			// 0x84
		WDummy,			// 0x85
		WDummy,			// 0x86
		WDummy,			// 0x87
		WDummy,			// 0x88
		WDummy,			// 0x89
		WDummy,			// 0x8A
		WDummy,			// 0x8B
		WDummy,			// 0x8C
		WDummy,			// 0x8D
		WDummy,			// 0x8E
		WDummy,			// 0x8F

		WDummy,			// 0x90
		WDummy,			// 0x91
		WDummy,			// 0x92
		WDummy,			// 0x93
		WDummy,			// 0x94
		WDummy,			// 0x95
		WDummy,			// 0x96
		WDummy,			// 0x97
		WDummy,			// 0x98
		WDummy,			// 0x99
		WDummy,			// 0x9A
		WDummy,			// 0x9B
		WDummy,			// 0x9C
		WDummy,			// 0x9D
		WDummy,			// 0x9E
		WDummy,			// 0x9F

		WDummy,			// 0xA0
		WDummy,			// 0xA1
		WDummy,			// 0xA2
		WDummy,			// 0xA3
		WDummy,			// 0xA4
		WDummy,			// 0xA5
		WDummy,			// 0xA6
		WDummy,			// 0xA7
		WDummy,			// 0xA8
		WDummy,			// 0xA9
		WDummy,			// 0xAA
		WDummy,			// 0xAB
		WDummy,			// 0xAC
		WDummy,			// 0xAD
		WDummy,			// 0xAE
		WDummy,			// 0xAF

		WDummy,			// 0xB0
		WDummy,			// 0xB1
		WDummy,			// 0xB2
		WDummy,			// 0xB3
		WDummy,			// 0xB4
		WDummy,			// 0xB5
		WDummy,			// 0xB6
		WDummy,			// 0xB7
		WDummy,			// 0xB8
		WDummy,			// 0xB9
		WDummy,			// 0xBA
		WDummy,			// 0xBB
		WDummy,			// 0xBC
		WDummy,			// 0xBD
		WDummy,			// 0xBE
		WDummy,			// 0xBF

		WDummy,			// 0xC0
		WDummy,			// 0xC1
		WDummy,			// 0xC2
		WDummy,			// 0xC3
		WDummy,			// 0xC4
		WDummy,			// 0xC5
		WDummy,			// 0xC6
		WDummy,			// 0xC7
		WDummy,			// 0xC8
		WDummy,			// 0xC9
		WDummy,			// 0xCA
		WDummy,			// 0xCB
		WDummy,			// 0xCC
		WDummy,			// 0xCD
		WDummy,			// 0xCE
		WDummy,			// 0xCF

		WDummy,			// 0xD0
		WDummy,			// 0xD1
		WDummy,			// 0xD2
		WDummy,			// 0xD3
		WDummy,			// 0xD4
		WDummy,			// 0xD5
		WDummy,			// 0xD6
		WDummy,			// 0xD7
		WDummy,			// 0xD8
		WDummy,			// 0xD9
		WDummy,			// 0xDA
		WDummy,			// 0xDB
		WDummy,			// 0xDC
		WDummy,			// 0xDD
		WDummy,			// 0xDE
		WDummy,			// 0xDF

		WDummy,			// 0xE0
		WDummy,			// 0xE1
		WDummy,			// 0xE2
		WDummy,			// 0xE3
		WDummy,			// 0xE4
		WDummy,			// 0xE5
		WDummy,			// 0xE6
		WDummy,			// 0xE7
		WDummy,			// 0xE8
		WDummy,			// 0xE9
		WDummy,			// 0xEA
		WDummy,			// 0xEB
		WDummy,			// 0xEC
		WDummy,			// 0xED
		WDummy,			// 0xEE
		WDummy,			// 0xEF

		WDummy,			// 0xF0
		WDummy,			// 0xF1
		WDummy,			// 0xF2
		WDummy,			// 0xF3
		WDummy,			// 0xF4
		WDummy,			// 0xF5
		WDummy,			// 0xF6
		WDummy,			// 0xF7
		WDummy,			// 0xF8
		WDummy,			// 0xF9
		WDummy,			// 0xFA
		WDummy,			// 0xFB
		WDummy,			// 0xFC
		WDummy,			// 0xFD
		WDummy,			// 0xFE
		WDummy			// 0xFF
};

// Write Registers Functions Caller
void WReg(U8 wr_val) {
	WRegVec[reg_num](wr_val);
}

//------------------------------------------------------------------------------------------

// - Read Registers Functions -

U8 RDummy(void) {
	return 0xFF;
}

U8 R00(void) {
	return ayregs[ay_chip].TF0.b.l;
}

U8 R01(void) {
	return ayregs[ay_chip].TF0.b.h;
}

U8 R02(void) {
	return ayregs[ay_chip].TF1.b.l;
}

U8 R03(void) {
	return ayregs[ay_chip].TF1.b.h;
}

U8 R04(void) {
	return ayregs[ay_chip].TF2.b.l;
}

U8 R05(void) {
	return ayregs[ay_chip].TF2.b.h;
}

U8 R06(void) {
	return ayregs[ay_chip].NF;
}

U8 R07(void) {
	return ayregs[ay_chip].MX;
}

U8 R08(void) {
	return ayregs[ay_chip].V0;
}

U8 R09(void) {
	return ayregs[ay_chip].V1;
}

U8 R0A(void) {
	return ayregs[ay_chip].V2;
}

U8 R0B(void) {
	return ayregs[ay_chip].EP.b.l;
}

U8 R0C(void) {
	return ayregs[ay_chip].EP.b.h;
}

U8 R10(void) {
	return ay_chip;
}

// Read Registers Functions Vectors
U8 (*RRegVec[0x100])() = {
		R00,			// 0x00
		R01,			// 0x01
		R02,			// 0x02
		R03,			// 0x03
		R04,			// 0x04
		R05,			// 0x05
		R06,			// 0x06
		R07,			// 0x07
		R08,			// 0x08
		R09,			// 0x09
		R0A,			// 0x0A
		R0B,			// 0x0B
		R0C,			// 0x0C
		RDummy,			// 0x0D
		RDummy,			// 0x0E
		RDummy,			// 0x0F

		R10,			// 0x10 - AY-Chip Number
		RDummy,			// 0x11
		RDummy,			// 0x12
		RDummy,			// 0x13
		RDummy,			// 0x14
		RDummy,			// 0x15
		RDummy,			// 0x16
		RDummy,			// 0x17
		RDummy,			// 0x18
		RDummy,			// 0x19
		RDummy,			// 0x1A
		RDummy,			// 0x1B
		RDummy,			// 0x1C
		RDummy,			// 0x1D
		RDummy,			// 0x1E
		RDummy,			// 0x1F

		RDummy,			// 0x20
		RDummy,			// 0x21
		RDummy,			// 0x22
		RDummy,			// 0x23
		RDummy,			// 0x24
		RDummy,			// 0x25
		RDummy,			// 0x26
		RDummy,			// 0x27
		RDummy,			// 0x28
		RDummy,			// 0x29
		RDummy,			// 0x2A
		RDummy,			// 0x2B
		RDummy,			// 0x2C
		RDummy,			// 0x2D
		RDummy,			// 0x2E
		RDummy,			// 0x2F

		RDummy,			// 0x30
		RDummy,			// 0x31
		RDummy,			// 0x32
		RDummy,			// 0x33
		RDummy,			// 0x34
		RDummy,			// 0x35
		RDummy,			// 0x36
		RDummy,			// 0x37
		RDummy,			// 0x38
		RDummy,			// 0x39
		RDummy,			// 0x3A
		RDummy,			// 0x3B
		RDummy,			// 0x3C
		RDummy,			// 0x3D
		RDummy,			// 0x3E
		RDummy,			// 0x3F

		RDummy,			// 0x40
		RDummy,			// 0x41
		RDummy,			// 0x42
		RDummy,			// 0x43
		RDummy,			// 0x44
		RDummy,			// 0x45
		RDummy,			// 0x46
		RDummy,			// 0x47
		RDummy,			// 0x48
		RDummy,			// 0x49
		RDummy,			// 0x4A
		RDummy,			// 0x4B
		RDummy,			// 0x4C
		RDummy,			// 0x4D
		RDummy,			// 0x4E
		RDummy,			// 0x4F

		RDummy,			// 0x50
		RDummy,			// 0x51
		RDummy,			// 0x52
		RDummy,			// 0x53
		RDummy,			// 0x54
		RDummy,			// 0x55
		RDummy,			// 0x56
		RDummy,			// 0x57
		RDummy,			// 0x58
		RDummy,			// 0x59
		RDummy,			// 0x5A
		RDummy,			// 0x5B
		RDummy,			// 0x5C
		RDummy,			// 0x5D
		RDummy,			// 0x5E
		RDummy,			// 0x5F

		RDummy,			// 0x60
		RDummy,			// 0x61
		RDummy,			// 0x62
		RDummy,			// 0x63
		RDummy,			// 0x64
		RDummy,			// 0x65
		RDummy,			// 0x66
		RDummy,			// 0x67
		RDummy,			// 0x68
		RDummy,			// 0x69
		RDummy,			// 0x6A
		RDummy,			// 0x6B
		RDummy,			// 0x6C
		RDummy,			// 0x6D
		RDummy,			// 0x6E
		RDummy,			// 0x6F

		RDummy,			// 0x70
		RDummy,			// 0x71
		RDummy,			// 0x72
		RDummy,			// 0x73
		RDummy,			// 0x74
		RDummy,			// 0x75
		RDummy,			// 0x76
		RDummy,			// 0x77
		RDummy,			// 0x78
		RDummy,			// 0x79
		RDummy,			// 0x7A
		RDummy,			// 0x7B
		RDummy,			// 0x7C
		RDummy,			// 0x7D
		RDummy,			// 0x7E
		RDummy,			// 0x7F

		RDummy,			// 0x80
		RDummy,			// 0x81
		RDummy,			// 0x82
		RDummy,			// 0x83
		RDummy,			// 0x84
		RDummy,			// 0x85
		RDummy,			// 0x86
		RDummy,			// 0x87
		RDummy,			// 0x88
		RDummy,			// 0x89
		RDummy,			// 0x8A
		RDummy,			// 0x8B
		RDummy,			// 0x8C
		RDummy,			// 0x8D
		RDummy,			// 0x8E
		RDummy,			// 0x8F

		RDummy,			// 0x90
		RDummy,			// 0x91
		RDummy,			// 0x92
		RDummy,			// 0x93
		RDummy,			// 0x94
		RDummy,			// 0x95
		RDummy,			// 0x96
		RDummy,			// 0x97
		RDummy,			// 0x98
		RDummy,			// 0x99
		RDummy,			// 0x9A
		RDummy,			// 0x9B
		RDummy,			// 0x9C
		RDummy,			// 0x9D
		RDummy,			// 0x9E
		RDummy,			// 0x9F

		RDummy,			// 0xA0
		RDummy,			// 0xA1
		RDummy,			// 0xA2
		RDummy,			// 0xA3
		RDummy,			// 0xA4
		RDummy,			// 0xA5
		RDummy,			// 0xA6
		RDummy,			// 0xA7
		RDummy,			// 0xA8
		RDummy,			// 0xA9
		RDummy,			// 0xAA
		RDummy,			// 0xAB
		RDummy,			// 0xAC
		RDummy,			// 0xAD
		RDummy,			// 0xAE
		RDummy,			// 0xAF

		RDummy,			// 0xB0
		RDummy,			// 0xB1
		RDummy,			// 0xB2
		RDummy,			// 0xB3
		RDummy,			// 0xB4
		RDummy,			// 0xB5
		RDummy,			// 0xB6
		RDummy,			// 0xB7
		RDummy,			// 0xB8
		RDummy,			// 0xB9
		RDummy,			// 0xBA
		RDummy,			// 0xBB
		RDummy,			// 0xBC
		RDummy,			// 0xBD
		RDummy,			// 0xBE
		RDummy,			// 0xBF

		RDummy,			// 0xC0
		RDummy,			// 0xC1
		RDummy,			// 0xC2
		RDummy,			// 0xC3
		RDummy,			// 0xC4
		RDummy,			// 0xC5
		RDummy,			// 0xC6
		RDummy,			// 0xC7
		RDummy,			// 0xC8
		RDummy,			// 0xC9
		RDummy,			// 0xCA
		RDummy,			// 0xCB
		RDummy,			// 0xCC
		RDummy,			// 0xCD
		RDummy,			// 0xCE
		RDummy,			// 0xCF

		RDummy,			// 0xD0
		RDummy,			// 0xD1
		RDummy,			// 0xD2
		RDummy,			// 0xD3
		RDummy,			// 0xD4
		RDummy,			// 0xD5
		RDummy,			// 0xD6
		RDummy,			// 0xD7
		RDummy,			// 0xD8
		RDummy,			// 0xD9
		RDummy,			// 0xDA
		RDummy,			// 0xDB
		RDummy,			// 0xDC
		RDummy,			// 0xDD
		RDummy,			// 0xDE
		RDummy,			// 0xDF

		RDummy,			// 0xE0
		RDummy,			// 0xE1
		RDummy,			// 0xE2
		RDummy,			// 0xE3
		RDummy,			// 0xE4
		RDummy,			// 0xE5
		RDummy,			// 0xE6
		RDummy,			// 0xE7
		RDummy,			// 0xE8
		RDummy,			// 0xE9
		RDummy,			// 0xEA
		RDummy,			// 0xEB
		RDummy,			// 0xEC
		RDummy,			// 0xED
		RDummy,			// 0xEE
		RDummy,			// 0xEF

		RDummy,			// 0xF0
		RDummy,			// 0xF1
		RDummy,			// 0xF2
		RDummy,			// 0xF3
		RDummy,			// 0xF4
		RDummy,			// 0xF5
		RDummy,			// 0xF6
		RDummy,			// 0xF7
		RDummy,			// 0xF8
		RDummy,			// 0xF9
		RDummy,			// 0xFA
		RDummy,			// 0xFB
		RDummy,			// 0xFC
		RDummy,			// 0xFD
		RDummy,			// 0xFE
		RDummy			// 0xFF
};

// Read Registers Functions Caller
U8 RReg(void) {
	return RRegVec[reg_num]();
}

//------------------------------------------------------------------------------------------

// - Write Address Functions -
void WAddr(U8 addr) {
	reg_num = addr;		// Write AY Register Address
	if (!(~addr & MASK_TURBO_AY))	// If Address if from Turbo-AY decoding span,
		ay_chip = ~addr & 1;		// select AY chip using bit0 (0 - chip1, 1 - chip0) - NedoPC scheme
}

