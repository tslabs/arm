
// AY chip registers function vectors (just not to annoy in the main module)
//
// (c) 2012, TS-Labs inc.
// All rights fucked out
//
// Njet, Molotoff!!!


//------------------------------------------------------------------------------------------
// Write Registers Functions Vectors
void (*WRegVec[0x100])(U8) = {
		W_00,			// 0x00 - AY R00
		W_01,			// 0x01 - AY R01
		W_02,			// 0x02 - AY R02
		W_03,			// 0x03 - AY R03
		W_04,			// 0x04 - AY R04
		W_05,			// 0x05 - AY R05
		W_06,			// 0x06 - AY R06
		W_07,			// 0x07 - AY R07
		W_08,			// 0x08 - AY R08
		W_09,			// 0x09 - AY R09
		W_0A,			// 0x0A - AY R0A
		W_0B,			// 0x0B - AY R0B
		W_0C,			// 0x0C - AY R0C
		W__,			// 0x0D - AY R0D
		W_0E,			// 0x0E - AY R0E
		W_0F,			// 0x0F - AY R0F

		W__,			// 0x10
		W__,			// 0x11
		W__,			// 0x12
		W__,			// 0x13
		W__,			// 0x14
		W__,			// 0x15
		W__,			// 0x16
		W__,			// 0x17
		W__,			// 0x18
		W__,			// 0x19
		W__,			// 0x1A
		W__,			// 0x1B
		W__,			// 0x1C
		W__,			// 0x1D
		W__,			// 0x1E
		W__,			// 0x1F

		W__,			// 0x20
		W__,			// 0x21
		W__,			// 0x22
		W__,			// 0x23
		W__,			// 0x24
		W__,			// 0x25
		W__,			// 0x26
		W__,			// 0x27
		W__,			// 0x28
		W__,			// 0x29
		W__,			// 0x2A
		W__,			// 0x2B
		W__,			// 0x2C
		W__,			// 0x2D
		W__,			// 0x2E
		W__,			// 0x2F

		W_30,			// 0x30 - Current number of AY chips
		W_31,			// 0x31 - AY-Chip Select
		W__,			// 0x32
		W__,			// 0x33
		W_34,			// 0x34 - Current number of SSG channels
		W_35,			// 0x35 - SSG Channel Select
		W__,			// 0x36
		W__,			// 0x37
		W__,			// 0x38
		W__,			// 0x39
		W__,			// 0x3A
		W__,			// 0x3B
		W__,			// 0x3C
		W__,			// 0x3D
		W__,			// 0x3E
		W__,			// 0x3F

		W__,			// 0x40
		W__,			// 0x41
		W__,			// 0x42
		W__,			// 0x43
		W__,			// 0x44
		W__,			// 0x45
		W__,			// 0x46
		W__,			// 0x47
		W__,			// 0x48
		W__,			// 0x49
		W__,			// 0x4A
		W__,			// 0x4B
		W__,			// 0x4C
		W__,			// 0x4D
		W__,			// 0x4E
		W__,			// 0x4F

		W__,			// 0x50
		W__,			// 0x51
		W__,			// 0x52
		W__,			// 0x53
		W__,			// 0x54
		W__,			// 0x55
		W__,			// 0x56
		W__,			// 0x57
		W__,			// 0x58
		W__,			// 0x59
		W__,			// 0x5A
		W__,			// 0x5B
		W__,			// 0x5C
		W__,			// 0x5D
		W__,			// 0x5E
		W__,			// 0x5F

		W__,			// 0x60
		W__,			// 0x61
		W__,			// 0x62
		W__,			// 0x63
		W__,			// 0x64
		W__,			// 0x65
		W__,			// 0x66
		W__,			// 0x67
		W__,			// 0x68
		W__,			// 0x69
		W__,			// 0x6A
		W__,			// 0x6B
		W__,			// 0x6C
		W__,			// 0x6D
		W__,			// 0x6E
		W__,			// 0x6F

		W__,			// 0x70
		W__,			// 0x71
		W__,			// 0x72
		W__,			// 0x73
		W__,			// 0x74
		W__,			// 0x75
		W__,			// 0x76
		W__,			// 0x77
		W__,			// 0x78
		W__,			// 0x79
		W__,			// 0x7A
		W__,			// 0x7B
		W__,			// 0x7C
		W__,			// 0x7D
		W__,			// 0x7E
		W__,			// 0x7F

		W__,			// 0x80
		W__,			// 0x81
		W__,			// 0x82
		W__,			// 0x83
		W__,			// 0x84
		W__,			// 0x85
		W__,			// 0x86
		W__,			// 0x87
		W__,			// 0x88
		W__,			// 0x89
		W__,			// 0x8A
		W__,			// 0x8B
		W__,			// 0x8C
		W__,			// 0x8D
		W__,			// 0x8E
		W__,			// 0x8F

		W__,			// 0x90
		W__,			// 0x91
		W__,			// 0x92
		W__,			// 0x93
		W__,			// 0x94
		W__,			// 0x95
		W__,			// 0x96
		W__,			// 0x97
		W__,			// 0x98
		W__,			// 0x99
		W__,			// 0x9A
		W__,			// 0x9B
		W__,			// 0x9C
		W__,			// 0x9D
		W__,			// 0x9E
		W__,			// 0x9F

		W__,			// 0xA0
		W__,			// 0xA1
		W__,			// 0xA2
		W__,			// 0xA3
		W__,			// 0xA4
		W__,			// 0xA5
		W__,			// 0xA6
		W__,			// 0xA7
		W__,			// 0xA8
		W__,			// 0xA9
		W__,			// 0xAA
		W__,			// 0xAB
		W__,			// 0xAC
		W__,			// 0xAD
		W__,			// 0xAE
		W__,			// 0xAF

		W__,			// 0xB0
		W__,			// 0xB1
		W__,			// 0xB2
		W__,			// 0xB3
		W__,			// 0xB4
		W__,			// 0xB5
		W__,			// 0xB6
		W__,			// 0xB7
		W__,			// 0xB8
		W__,			// 0xB9
		W__,			// 0xBA
		W__,			// 0xBB
		W__,			// 0xBC
		W__,			// 0xBD
		W__,			// 0xBE
		W__,			// 0xBF

		W__,			// 0xC0
		W__,			// 0xC1
		W__,			// 0xC2
		W__,			// 0xC3
		W__,			// 0xC4
		W__,			// 0xC5
		W__,			// 0xC6
		W__,			// 0xC7
		W__,			// 0xC8
		W__,			// 0xC9
		W__,			// 0xCA
		W__,			// 0xCB
		W__,			// 0xCC
		W__,			// 0xCD
		W__,			// 0xCE
		W__,			// 0xCF

		W__,			// 0xD0
		W__,			// 0xD1
		W__,			// 0xD2
		W__,			// 0xD3
		W__,			// 0xD4
		W__,			// 0xD5
		W__,			// 0xD6
		W__,			// 0xD7
		W__,			// 0xD8
		W__,			// 0xD9
		W__,			// 0xDA
		W__,			// 0xDB
		W__,			// 0xDC
		W__,			// 0xDD
		W__,			// 0xDE
		W__,			// 0xDF

		W_E0,			// 0xE0 - Command
		W__,			// 0xE1
		W_E2,			// 0xE2 - Data In
		W__,			// 0xE3
		W__,			// 0xE4
		W__,			// 0xE5
		W__,			// 0xE6
		W__,			// 0xE7
		W_E8,			// 0xE8 - Command Parameter0
		W_E9,			// 0xE9 - Command Parameter1
		W_EA,			// 0xEA - Command Parameter2
		W_EB,			// 0xEB - Command Parameter3
		W_EC,			// 0xEC - Command Parameter4
		W_ED,			// 0xED - Command Parameter5
		W_EE,			// 0xEE - Command Parameter6
		W_EF,			// 0xEF - Command Parameter7

		W__,			// 0xF0
		W__,			// 0xF1
		W__,			// 0xF2
		W__,			// 0xF3
		W__,			// 0xF4
		W__,			// 0xF5
		W__,			// 0xF6
		W__,			// 0xF7
		W__,			// 0xF8
		W__,			// 0xF9
		W__,			// 0xFA
		W__,			// 0xFB
		W__,			// 0xFC
		W__,			// 0xFD
		W__,			// 0xFE
		W__				// 0xFF
};

//------------------------------------------------------------------------------------------
// Read Registers Functions Vectors
U8 (*RRegVec[0x100])() = {
		R_00,			// 0x00 - AY R00
		R_01,			// 0x01 - AY R01
		R_02,			// 0x02 - AY R02
		R_03,			// 0x03 - AY R03
		R_04,			// 0x04 - AY R04
		R_05,			// 0x05 - AY R05
		R_06,			// 0x06 - AY R06
		R_07,			// 0x07 - AY R07
		R_08,			// 0x08 - AY R08
		R_09,			// 0x09 - AY R09
		R_0A,			// 0x0A - AY R0A
		R_0B,			// 0x0B - AY R0B
		R_0C,			// 0x0C - AY R0C
		R_0D,			// 0x0D - AY R0D
		R_0E,			// 0x0E - AY R0E
		R_0F,			// 0x0F - AY R0F

		R__,			// 0x10
		R__,			// 0x11
		R__,			// 0x12
		R__,			// 0x13
		R__,			// 0x14
		R__,			// 0x15
		R__,			// 0x16
		R__,			// 0x17
		R__,			// 0x18
		R__,			// 0x19
		R__,			// 0x1A
		R__,			// 0x1B
		R__,			// 0x1C
		R__,			// 0x1D
		R__,			// 0x1E
		R__,			// 0x1F

		R__,			// 0x20
		R__,			// 0x21
		R__,			// 0x22
		R__,			// 0x23
		R__,			// 0x24
		R__,			// 0x25
		R__,			// 0x26
		R__,			// 0x27
		R__,			// 0x28
		R__,			// 0x29
		R__,			// 0x2A
		R__,			// 0x2B
		R__,			// 0x2C
		R__,			// 0x2D
		R__,			// 0x2E
		R__,			// 0x2F

		R_30,			// 0x30 - Current number of AY chips
		R_31,			// 0x31 - AY-Chip Number
		R__,			// 0x32
		R__,			// 0x33
		R_34,			// 0x34 - Current number of SSG channels
		R_35,			// 0x35 - Current SSG channel
		R__,			// 0x36
		R__,			// 0x37
		R__,			// 0x38
		R__,			// 0x39
		R__,			// 0x3A
		R__,			// 0x3B
		R__,			// 0x3C
		R__,			// 0x3D
		R__,			// 0x3E
		R__,			// 0x3F

		R__,			// 0x40
		R__,			// 0x41
		R__,			// 0x42
		R__,			// 0x43
		R__,			// 0x44
		R__,			// 0x45
		R__,			// 0x46
		R__,			// 0x47
		R__,			// 0x48
		R__,			// 0x49
		R__,			// 0x4A
		R__,			// 0x4B
		R__,			// 0x4C
		R__,			// 0x4D
		R__,			// 0x4E
		R__,			// 0x4F

		R__,			// 0x50
		R__,			// 0x51
		R__,			// 0x52
		R__,			// 0x53
		R__,			// 0x54
		R__,			// 0x55
		R__,			// 0x56
		R__,			// 0x57
		R__,			// 0x58
		R__,			// 0x59
		R__,			// 0x5A
		R__,			// 0x5B
		R__,			// 0x5C
		R__,			// 0x5D
		R__,			// 0x5E
		R__,			// 0x5F

		R__,			// 0x60
		R__,			// 0x61
		R__,			// 0x62
		R__,			// 0x63
		R__,			// 0x64
		R__,			// 0x65
		R__,			// 0x66
		R__,			// 0x67
		R__,			// 0x68
		R__,			// 0x69
		R__,			// 0x6A
		R__,			// 0x6B
		R__,			// 0x6C
		R__,			// 0x6D
		R__,			// 0x6E
		R__,			// 0x6F

		R__,			// 0x70
		R__,			// 0x71
		R__,			// 0x72
		R__,			// 0x73
		R__,			// 0x74
		R__,			// 0x75
		R__,			// 0x76
		R__,			// 0x77
		R__,			// 0x78
		R__,			// 0x79
		R__,			// 0x7A
		R__,			// 0x7B
		R__,			// 0x7C
		R__,			// 0x7D
		R__,			// 0x7E
		R__,			// 0x7F

		R__,			// 0x80
		R__,			// 0x81
		R__,			// 0x82
		R__,			// 0x83
		R__,			// 0x84
		R__,			// 0x85
		R__,			// 0x86
		R__,			// 0x87
		R__,			// 0x88
		R__,			// 0x89
		R__,			// 0x8A
		R__,			// 0x8B
		R__,			// 0x8C
		R__,			// 0x8D
		R__,			// 0x8E
		R__,			// 0x8F

		R__,			// 0x90
		R__,			// 0x91
		R__,			// 0x92
		R__,			// 0x93
		R__,			// 0x94
		R__,			// 0x95
		R__,			// 0x96
		R__,			// 0x97
		R__,			// 0x98
		R__,			// 0x99
		R__,			// 0x9A
		R__,			// 0x9B
		R__,			// 0x9C
		R__,			// 0x9D
		R__,			// 0x9E
		R__,			// 0x9F

		R__,			// 0xA0
		R__,			// 0xA1
		R__,			// 0xA2
		R__,			// 0xA3
		R__,			// 0xA4
		R__,			// 0xA5
		R__,			// 0xA6
		R__,			// 0xA7
		R__,			// 0xA8
		R__,			// 0xA9
		R__,			// 0xAA
		R__,			// 0xAB
		R__,			// 0xAC
		R__,			// 0xAD
		R__,			// 0xAE
		R__,			// 0xAF

		R__,			// 0xB0
		R__,			// 0xB1
		R__,			// 0xB2
		R__,			// 0xB3
		R__,			// 0xB4
		R__,			// 0xB5
		R__,			// 0xB6
		R__,			// 0xB7
		R__,			// 0xB8
		R__,			// 0xB9
		R__,			// 0xBA
		R__,			// 0xBB
		R__,			// 0xBC
		R__,			// 0xBD
		R__,			// 0xBE
		R__,			// 0xBF

		R__,			// 0xC0
		R__,			// 0xC1
		R__,			// 0xC2
		R__,			// 0xC3
		R__,			// 0xC4
		R__,			// 0xC5
		R__,			// 0xC6
		R__,			// 0xC7
		R__,			// 0xC8
		R__,			// 0xC9
		R__,			// 0xCA
		R__,			// 0xCB
		R__,			// 0xCC
		R__,			// 0xCD
		R__,			// 0xCE
		R__,			// 0xCF

		R_D0,			// 0xD0 - Output Samplerate
		R_D1,			// 0xD1 ^
		R_D2,			// 0xD2 - Max number of AY chips
		R_D3,			// 0xD3 - Max number of SSG channels
		R__,			// 0xD4
		R__,			// 0xD5
		R__,			// 0xD6
		R__,			// 0xD7
		R__,			// 0xD8
		R__,			// 0xD9
		R__,			// 0xDA
		R__,			// 0xDB
		R_DC,			// 0xDC
		R_DD,			// 0xDD
		R_DE,			// 0xDE - Chip Version
		R_DF,			// 0xDF - Chip Sub-Version

		R_E0,			// 0xE0 - Status
		R_E1,			// 0xE1 - Error
		R_E2,			// 0xE2 - Data Out
		R__,			// 0xE3
		R__,			// 0xE4
		R__,			// 0xE5
		R__,			// 0xE6
		R__,			// 0xE7
		R_E8,			// 0xE8 - Command Acknowledge/Response0
		R_E9,			// 0xE9 - Command Acknowledge/Response1
		R_EA,			// 0xEA - Command Acknowledge/Response2
		R_EB,			// 0xEB - Command Acknowledge/Response3
		R_EC,			// 0xEC - Command Acknowledge/Response4
		R_ED,			// 0xED - Command Acknowledge/Response5
		R_EE,			// 0xEE - Command Acknowledge/Response6
		R_EF,			// 0xEF - Command Acknowledge/Response7

		R__,			// 0xF0
		R__,			// 0xF1
		R__,			// 0xF2
		R__,			// 0xF3
		R__,			// 0xF4
		R__,			// 0xF5
		R__,			// 0xF6
		R__,			// 0xF7
		R__,			// 0xF8
		R__,			// 0xF9
		R__,			// 0xFA
		R__,			// 0xFB
		R__,			// 0xFC
		R__,			// 0xFD
		R__,			// 0xFE
		R__				// 0xFF
};

//------------------------------------------------------------------------------------------
// - Command Functions Vectors -
void (*CmdFVec[0x100])() = {
		C_00,			// 0x00 - Break
		C__,			// 0x01
		C__,			// 0x02
		C__,			// 0x03
		C__,			// 0x04
		C__,			// 0x05
		C__,			// 0x06
		C__,			// 0x07
		C__,			// 0x08
		C__,			// 0x09
		C__,			// 0x0A
		C__,			// 0x0B
		C__,			// 0x0C
		C__,			// 0x0D
		C__,			// 0x0E
		C__,			// 0x0F

		C__,			// 0x10
		C__,			// 0x11
		C__,			// 0x12
		C__,			// 0x13
		C__,			// 0x14
		C__,			// 0x15
		C__,			// 0x16
		C__,			// 0x17
		C__,			// 0x18
		C__,			// 0x19
		C__,			// 0x1A
		C__,			// 0x1B
		C__,			// 0x1C
		C__,			// 0x1D
		C__,			// 0x1E
		C__,			// 0x1F

		C__,			// 0x20
		C__,			// 0x21
		C__,			// 0x22
		C__,			// 0x23
		C__,			// 0x24
		C__,			// 0x25
		C__,			// 0x26
		C__,			// 0x27
		C__,			// 0x28
		C__,			// 0x29
		C__,			// 0x2A
		C__,			// 0x2B
		C__,			// 0x2C
		C__,			// 0x2D
		C__,			// 0x2E
		C__,			// 0x2F

		C__,			// 0x30
		C__,			// 0x31
		C__,			// 0x32
		C__,			// 0x33
		C__,			// 0x34
		C__,			// 0x35
		C__,			// 0x36
		C__,			// 0x37
		C__,			// 0x38
		C__,			// 0x39
		C__,			// 0x3A
		C__,			// 0x3B
		C__,			// 0x3C
		C__,			// 0x3D
		C__,			// 0x3E
		C__,			// 0x3F

		C__,			// 0x40
		C__,			// 0x41
		C__,			// 0x42
		C__,			// 0x43
		C__,			// 0x44
		C__,			// 0x45
		C__,			// 0x46
		C__,			// 0x47
		C__,			// 0x48
		C__,			// 0x49
		C__,			// 0x4A
		C__,			// 0x4B
		C__,			// 0x4C
		C__,			// 0x4D
		C__,			// 0x4E
		C__,			// 0x4F

		C__,			// 0x50
		C__,			// 0x51
		C__,			// 0x52
		C__,			// 0x53
		C__,			// 0x54
		C__,			// 0x55
		C__,			// 0x56
		C__,			// 0x57
		C__,			// 0x58
		C__,			// 0x59
		C__,			// 0x5A
		C__,			// 0x5B
		C__,			// 0x5C
		C__,			// 0x5D
		C__,			// 0x5E
		C__,			// 0x5F

		C__,			// 0x60
		C__,			// 0x61
		C__,			// 0x62
		C__,			// 0x63
		C__,			// 0x64
		C__,			// 0x65
		C__,			// 0x66
		C__,			// 0x67
		C__,			// 0x68
		C__,			// 0x69
		C__,			// 0x6A
		C__,			// 0x6B
		C__,			// 0x6C
		C__,			// 0x6D
		C__,			// 0x6E
		C__,			// 0x6F

		C__,			// 0x70
		C__,			// 0x71
		C__,			// 0x72
		C__,			// 0x73
		C__,			// 0x74
		C__,			// 0x75
		C__,			// 0x76
		C__,			// 0x77
		C__,			// 0x78
		C__,			// 0x79
		C__,			// 0x7A
		C__,			// 0x7B
		C__,			// 0x7C
		C__,			// 0x7D
		C__,			// 0x7E
		C__,			// 0x7F

		C__,			// 0x80
		C__,			// 0x81
		C__,			// 0x82
		C__,			// 0x83
		C__,			// 0x84
		C__,			// 0x85
		C__,			// 0x86
		C__,			// 0x87
		C__,			// 0x88
		C__,			// 0x89
		C__,			// 0x8A
		C__,			// 0x8B
		C__,			// 0x8C
		C__,			// 0x8D
		C__,			// 0x8E
		C__,			// 0x8F

		C__,			// 0x90
		C__,			// 0x91
		C__,			// 0x92
		C__,			// 0x93
		C__,			// 0x94
		C__,			// 0x95
		C__,			// 0x96
		C__,			// 0x97
		C__,			// 0x98
		C__,			// 0x99
		C__,			// 0x9A
		C__,			// 0x9B
		C__,			// 0x9C
		C__,			// 0x9D
		C__,			// 0x9E
		C__,			// 0x9F

		C__,			// 0xA0
		C__,			// 0xA1
		C__,			// 0xA2
		C__,			// 0xA3
		C__,			// 0xA4
		C__,			// 0xA5
		C__,			// 0xA6
		C__,			// 0xA7
		C__,			// 0xA8
		C__,			// 0xA9
		C__,			// 0xAA
		C__,			// 0xAB
		C__,			// 0xAC
		C__,			// 0xAD
		C__,			// 0xAE
		C__,			// 0xAF

		C__,			// 0xB0
		C__,			// 0xB1
		C__,			// 0xB2
		C__,			// 0xB3
		C__,			// 0xB4
		C__,			// 0xB5
		C__,			// 0xB6
		C__,			// 0xB7
		C__,			// 0xB8
		C__,			// 0xB9
		C__,			// 0xBA
		C__,			// 0xBB
		C__,			// 0xBC
		C__,			// 0xBD
		C__,			// 0xBE
		C__,			// 0xBF

		C__,			// 0xC0
		C__,			// 0xC1
		C__,			// 0xC2
		C__,			// 0xC3
		C__,			// 0xC4
		C__,			// 0xC5
		C__,			// 0xC6
		C__,			// 0xC7
		C__,			// 0xC8
		C__,			// 0xC9
		C__,			// 0xCA
		C__,			// 0xCB
		C__,			// 0xCC
		C__,			// 0xCD
		C__,			// 0xCE
		C__,			// 0xCF

		C__,			// 0xD0
		C__,			// 0xD1
		C__,			// 0xD2
		C__,			// 0xD3
		C__,			// 0xD4
		C__,			// 0xD5
		C__,			// 0xD6
		C__,			// 0xD7
		C__,			// 0xD8
		C__,			// 0xD9
		C__,			// 0xDA
		C__,			// 0xDB
		C__,			// 0xDC
		C__,			// 0xDD
		C__,			// 0xDE
		C__,			// 0xDF

		C__,			// 0xE0
		C__,			// 0xE1
		C__,			// 0xE2
		C__,			// 0xE3
		C__,			// 0xE4
		C__,			// 0xE5
		C__,			// 0xE6
		C__,			// 0xE7
		C__,			// 0xE8
		C__,			// 0xE9
		C__,			// 0xEA
		C__,			// 0xEB
		C__,			// 0xEC
		C__,			// 0xED
		C__,			// 0xEE
		C__,			// 0xEF

		C_F0,			// 0xF0
		C_F1,			// 0xF1
		C__,			// 0xF2
		C__,			// 0xF3
		C__,			// 0xF4
		C__,			// 0xF5
		C__,			// 0xF6
		C__,			// 0xF7
		C__,			// 0xF8
		C__,			// 0xF9
		C__,			// 0xFA
		C__,			// 0xFB
		C__,			// 0xFC
		C__,			// 0xFD
		C__,			// 0xFE
		C__,			// 0xFF
};
