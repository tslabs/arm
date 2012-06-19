
// ZX interface. AY chip registers reading/writing
//
// (c) 2012, TS-Labs inc.
// All rights fucked out
//
// 128kB is enough for everyone


#include "zx.h"
#include "ay-arm.h"
#include "ay.h"
#include "ssg.h"


// --- Variables declaration -----
	// strings
		U8			ChipString[] = {CHIP_STRING};
		U8			CprString[] = {CPR_STRING};
		U8			dummy = 0;
	
	// locals
		U8			RegNum = 0;			// Currently selected Chip register
		U8			RegVal = 0;			// Value to be written into the register (should be replaced by read Z80 Bus value)
		U8			AYChip = 0;			// Selected AY Chip
		U8			SSGChan = 0;		// Selected SSG Channel
		U8			*DataAddr = &dummy;	// Pointer to next data to be read
		int			DataCtr = 0;		// Counter for data to be transferred, bytes
		U8			Cmd = 0;			// Command
		Stat		Status = {0};		// Status byte
		U8			Error = 0;			// Error code
		W64			CmdPar = {0};		// Command parameter
		W64			CmdResp = {0};		// Command Acknowledge/Response

	// external
extern	AY_Regs		AY[AY_CHIPS_MAX];	// Registers for virtual AY chips
extern	SSG_Regs	SSG[AY_CHIPS_MAX];	// Registers for SSG channels
extern	U8			AYChipNum;
extern	U8			SSGChNum;
extern	W16			SSGFreq;


//------------------------------------------------------------------------------------------
// - Write Register Functions -

// Dummy Register Write
void W__(U8 RegVal) {
}

// AY R00
void W_00(U8 RegVal) {
	AY[AYChip].TF0.b.b0 = RegVal;
}

// AY R01
void W_01(U8 RegVal) {
	AY[AYChip].TF0.b.b1 = RegVal;
}

// AY R02
void W_02(U8 RegVal) {
	AY[AYChip].TF1.b.b0 = RegVal;
}

// AY R03
void W_03(U8 RegVal) {
	AY[AYChip].TF1.b.b1 = RegVal;
}

// AY R04
void W_04(U8 RegVal) {
	AY[AYChip].TF2.b.b0 = RegVal;
}

// AY R05
void W_05(U8 RegVal) {
	AY[AYChip].TF2.b.b1 = RegVal;
}

// AY R06
void W_06(U8 RegVal) {
	AY[AYChip].NF = RegVal;
}

// AY R07
void W_07(U8 RegVal) {
	AY[AYChip].MX = RegVal;
}

// AY R08
void W_08(U8 RegVal) {
	AY[AYChip].V0 = RegVal;
}

// AY R09
void W_09(U8 RegVal) {
	AY[AYChip].V1 = RegVal;
}

// AY R0A
void W_0A(U8 RegVal) {
	AY[AYChip].V2 = RegVal;
}

// AY R0B
void W_0B(U8 RegVal) {
	AY[AYChip].EP.b.b0 = RegVal;
}

// AY R0C
void W_0C(U8 RegVal) {
	AY[AYChip].EP.b.b1 = RegVal;
}

// AY R0E
void W_0E(U8 RegVal) {
	AY[AYChip].IOA = RegVal;
}

// AY R0F
void W_0F(U8 RegVal) {
	AY[AYChip].IOB = RegVal;
}

// Current number of AY chips
void W_30(U8 RegVal) {
	AYChipNum = (RegVal >= AY_CHIPS_MAX) ? AY_CHIPS_MAX-1 : RegVal;
	if (AYChip >= AYChipNum)
		AYChip = AYChipNum-1;
}

// AY Chip Select
void W_31(U8 RegVal) {
	AYChip = (RegVal > AYChipNum) ? AYChipNum : RegVal;;
}

// Current number of SSG channels
void W_34(U8 RegVal) {
	SSGChNum = (RegVal >= SSG_CH_MAX) ? SSG_CH_MAX-1 : RegVal;
	if (SSGChan >= SSGChNum)
		SSGChan = SSGChNum-1;
}

// SSG Channel Select
void W_35(U8 RegVal) {
	SSGChan = (RegVal > SSGChNum) ? SSGChNum : RegVal;
}

// Command
void W_E0(U8 RegVal) {
	CmdF(RegVal);
}

// Data In
void W_E2(U8 RegVal) {
}

// Command Parameter0
void W_E8(U8 RegVal) {
	CmdPar.b.b0 = RegVal;
}

// Command Parameter1
void W_E9(U8 RegVal) {
	CmdPar.b.b1 = RegVal;
}

// Command Parameter2
void W_EA(U8 RegVal) {
	CmdPar.b.b2 = RegVal;
}

// Command Parameter3
void W_EB(U8 RegVal) {
	CmdPar.b.b3 = RegVal;
}

// Command Parameter4
void W_EC(U8 RegVal) {
	CmdPar.b.b4 = RegVal;
}

// Command Parameter5
void W_ED(U8 RegVal) {
	CmdPar.b.b5 = RegVal;
}

// Command Parameter6
void W_EE(U8 RegVal) {
	CmdPar.b.b6 = RegVal;
}

// Command Parameter7
void W_EF(U8 RegVal) {
	CmdPar.b.b7 = RegVal;
}

//------------------------------------------------------------------------------------------
// - Read Register Functions -

// Dummy Register Read
U8 R__(void) {
	return 0xFF;
}

// AY R00
U8 R_00(void) {
	return AY[AYChip].TF0.b.b0;
}

// AY R01
U8 R_01(void) {
	return AY[AYChip].TF0.b.b1;
}

// AY R02
U8 R_02(void) {
	return AY[AYChip].TF1.b.b0;
}

// AY R03
U8 R_03(void) {
	return AY[AYChip].TF1.b.b1;
}

// AY R04
U8 R_04(void) {
	return AY[AYChip].TF2.b.b0;
}

// AY R05
U8 R_05(void) {
	return AY[AYChip].TF2.b.b1;
}

// AY R06
U8 R_06(void) {
	return AY[AYChip].NF;
}

// AY R07
U8 R_07(void) {
	return AY[AYChip].MX;
}

// AY R08
U8 R_08(void) {
	return AY[AYChip].V0;
}

// AY R09
U8 R_09(void) {
	return AY[AYChip].V1;
}

// AY R0A
U8 R_0A(void) {
	return AY[AYChip].V2;
}

// AY R0B
U8 R_0B(void) {
	return AY[AYChip].EP.b.b0;
}

// AY R0C
U8 R_0C(void) {
	return AY[AYChip].EP.b.b1;
}

// AY R0D
U8 R_0D(void) {
	return AY[AYChip].EC;
}

// AY R0E
U8 R_0E(void) {
	return AY[AYChip].IOA;
}

// AY R0F
U8 R_0F(void) {
	return AY[AYChip].IOB;
}

// Current number of AY chips
U8 R_30(void) {
	return AYChipNum;
}

// AY-Chip Number
U8 R_31(void) {
	return AYChip;
}

// Current number of SSG channels
U8 R_34(void) {
	return SSGChNum;
}

// SSG Channel Number
U8 R_35(void) {
	return SSGChan;
}

// Output Audio Sample Rate, Lower Byte
U8 R_36(void) {
	return SSGFreq.b.b0;
}

// Output Audio Sample Rate, Higher Byte
U8 R_37(void) {
	return SSGFreq.b.b1;
}

// Max supported number of AY chips
U8 R_DA(void) {
	return AY_CHIPS_MAX;
}

// Max supported number of SSG channels
U8 R_DB(void) {
	return SSG_CH_MAX;
}

// Device byte 0
U8 R_DC(void) {
	return DEV_BYTE0;
}

// Device byte 1
U8 R_DD(void) {
	return DEV_BYTE1;
}

// Chip Version
U8 R_DE(void) {
	return NUM_VER;
}

// Chip Sub-Version
U8 R_DF(void) {
	return NUM_SVER;
}

// Status
U8 R_E0(void) {
	return Status.b;
}

// Error Code
U8 R_E1(void) {
	return Error;
}

// Data Out
U8 R_E2(void) {
	U8 tmp;
	
	tmp = *DataAddr;			// Read new byte
	if (DataCtr && Status.i.drd) {
		DataAddr++;
		DataCtr--;
		if (!DataCtr) {			// If last byte read - end of command
			Status.i.cmp = 1;
			Status.i.busy = 0;
			Status.i.drd = 0;
		}
	}
	return tmp;
}

// Command Acknowledge/Response0
U8 R_E8(void) {
	return CmdResp.b.b0;
}

// Command Acknowledge/Response1
U8 R_E9(void) {
	return CmdResp.b.b1;
}

// Command Acknowledge/Response2
U8 R_EA(void) {
	return CmdResp.b.b2;
}

// Command Acknowledge/Response3
U8 R_EB(void) {
	return CmdResp.b.b3;
}

// Command Acknowledge/Response4
U8 R_EC(void) {
	return CmdResp.b.b4;
}

// Command Acknowledge/Response5
U8 R_ED(void) {
	return CmdResp.b.b5;
}

// Command Acknowledge/Response6
U8 R_EE(void) {
	return CmdResp.b.b6;
}

// Command Acknowledge/Response7
U8 R_EF(void) {
	return CmdResp.b.b7;
}

//------------------------------------------------------------------------------------------
// - Command Functions -

// Illegal command
void C__(void) {
	Status.i.err = 1;
	Error = CMD_ERR_ILG;
}

// Break
void C_00(void) {
	Status.i.brk = 1;
	Error = CMD_ERR_BRK;
	Status.i.busy = 0;			// Reset BUSY status bit

}

// Get Chip Version String
void C_F0(void) {
	Status.i.acc = 1;
	Status.i.drd = 1;		// Data is ready to be transferred from Chip to Host
	CmdResp.w.w0 = sizeof(ChipString);
	DataCtr = sizeof(ChipString);
	DataAddr = ChipString;
}

// Get Copyright String
void C_F1(void) {
	Status.i.acc = 1;
	Status.i.drd = 1;		// Data is ready to be transferred from Chip to Host
	CmdResp.w.w0 = sizeof(CprString);
	DataCtr = sizeof(CprString);
	DataAddr = CprString;
}

#include "zx_vect.h"		// Function Vectors carried out to another file to not to be an eyesore

//------------------------------------------------------------------------------------------
// - Common Functions -

// - Write Address -
void WAddr(U8 addr) {
	RegNum = addr;		// Write AY Register Address
	if (!(~addr & MASK_TURBO_AY))	// If Address if from Turbo-AY decoding span,
		AYChip = ~addr & 1;			// select AY chip using bit0 (0 - chip1, 1 - chip0) - NedoPC scheme
}

// Write Registers Functions Caller
void WReg(U8 RegVal) {
	WRegVec[RegNum](RegVal);
}

// Read Registers Functions Caller
U8 RReg(void) {
	return RRegVec[RegNum]();
}

// Command Functions Caller
void CmdF(U8 cmd) {
	if (!Status.i.busy) {		// If another command NOT in progress
		Status.b = 0;			// Clear status (all other status bits will be set by a specific command
		Status.i.busy = 1;		// Set BUSY status bit
		Error = CMD_ERR_OK;		// Clear error code. Specific command function may modify it later
		CmdFVec[cmd]();			// Call command procedure by its vector
	}

	else if(cmd == CMD_BRK) {	// If BREAK issued
		DataCtr = 0;			// Clear data counter
		Status.b = 0;
		Status.i.brk = 1;		// Set BRK status bit
		Error = CMD_ERR_BRK;	// Set BREAK error code
	}
}

