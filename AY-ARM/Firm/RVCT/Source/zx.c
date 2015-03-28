
// ZX interface. AY chip registers reading/writing
//
// (c) 2012, TS-Labs inc.
// All rights fucked out
//
// 128kB is enough for everyone


#include "zx.h"
#include "ay-arm.h"
#include "ay.h"
#include "ws.h"


// --- Variables declaration -----
	// strings
		U8			ChipString[] = {CHIP_STRING};
		U8			CprString[] = {CPR_STRING};
		U8			dummy = 0;
	
	// locals
		U8			RegNum = 0;			// Currently selected Chip register
		U8			RegVal = 0;			// Value to be written into the register (should be replaced by read Z80 Bus value)
		U8			AYChip = 0;			// Selected AY Chip
		U8			WSChan = 0;		// Selected WS Channel
		U8			*DataAddr = &dummy;	// Pointer to next data to be read
		int			DataCtr = 0;		// Counter for data to be transferred, bytes
		U8			Cmd = 0;			// Command
		Stat		Status = {0};		// Status byte
		U8			Error = 0;			// Error code
		W64			CmdPar = {0};		// Command parameter
		W64			CmdResp = {0};		// Command Acknowledge/Response

	// external
extern	AY_Regs		AY[AY_CHIPS_MAX];	// Registers for virtual AY chips
extern	WS_Regs	    WS[AY_CHIPS_MAX];	// Registers for WS channels
extern	U8			AYChipNum;
extern	U8			WSChNum;
extern	W16			WSFreq;

//------------------------------------------------------------------------------------------
// - Write Register Functions -

// Dummy Register Write
void W__(U8 RegVal){
}

// AY Reg 00
void W_00(U8 RegVal){
	AY[AYChip].TF0.b0 = RegVal;
}

// AY Reg 01
void W_01(U8 RegVal){
	AY[AYChip].TF0.b1 = RegVal & 0x0F;
}

// AY Reg 02
void W_02(U8 RegVal){
	AY[AYChip].TF1.b0 = RegVal;
}

// AY Reg 03
void W_03(U8 RegVal){
	AY[AYChip].TF1.b1 = RegVal & 0x0F;
}

// AY Reg 04
void W_04(U8 RegVal){
	AY[AYChip].TF2.b0 = RegVal;
}

// AY Reg 05
void W_05(U8 RegVal){
	AY[AYChip].TF2.b1 = RegVal & 0x0F;
}

// AY Reg 06
void W_06(U8 RegVal){
	AY[AYChip].NF = RegVal & 0x1F;
}

// AY Reg 07
void W_07(U8 RegVal){
	AY[AYChip].MX.b = ~RegVal;	// kept inverted
}

// AY Reg 08
void W_08(U8 RegVal){
	AY[AYChip].V0.b = (RegVal & 0x1F) << 1;
}

// AY Reg 09
void W_09(U8 RegVal){
	AY[AYChip].V1.b = (RegVal & 0x1F) << 1;
}

// AY Reg 0A
void W_0A(U8 RegVal){
	AY[AYChip].V2.b = (RegVal & 0x1F) << 1;
}

// AY Reg 0B
void W_0B(U8 RegVal){
	AY[AYChip].EP.b0 = RegVal;
}

// AY Reg 0C
void W_0C(U8 RegVal){
	AY[AYChip].EP.b1 = RegVal;
}

// AY Reg 0D
void W_0D(U8 RegVal){
	AY[AYChip].EC = RegVal & 0x0F;
	AY[AYChip].env_rld = 1;
}

// AY Reg 0E
void W_0E(U8 RegVal){
	AY[AYChip].IOA = RegVal;
}

// AY Reg 0F
void W_0F(U8 RegVal){
	AY[AYChip].IOB = RegVal;
}

// Current number of AY chips
void W_30(U8 RegVal){
	AYChipNum = (RegVal >= AY_CHIPS_MAX) ? AY_CHIPS_MAX-1 : RegVal;
	if (AYChip >= AYChipNum)
		AYChip = AYChipNum-1;
}

// AY Chip Select
void W_31(U8 RegVal){
	AYChip = (RegVal > AYChipNum) ? AYChipNum : RegVal;;
}

// Current number of WS channels
void W_34(U8 RegVal){
	WSChNum = (RegVal >= WS_CH_MAX) ? WS_CH_MAX-1 : RegVal;
	if (WSChan >= WSChNum)
		WSChan = WSChNum-1;
}

// WS Channel Select
void W_35(U8 RegVal){
	WSChan = (RegVal > WSChNum) ? WSChNum : RegVal;
}

// Command
void W_E0(U8 RegVal){
	CmdF(RegVal);
}

// Data In
void W_E2(U8 RegVal){
}

// Command Parameter0
void W_E8(U8 RegVal){
	CmdPar.b0 = RegVal;
}

// Command Parameter1
void W_E9(U8 RegVal){
	CmdPar.b1 = RegVal;
}

// Command Parameter2
void W_EA(U8 RegVal){
	CmdPar.b2 = RegVal;
}

// Command Parameter3
void W_EB(U8 RegVal){
	CmdPar.b3 = RegVal;
}

// Command Parameter4
void W_EC(U8 RegVal){
	CmdPar.b4 = RegVal;
}

// Command Parameter5
void W_ED(U8 RegVal){
	CmdPar.b5 = RegVal;
}

// Command Parameter6
void W_EE(U8 RegVal){
	CmdPar.b6 = RegVal;
}

// Command Parameter7
void W_EF(U8 RegVal){
	CmdPar.b7 = RegVal;
}

//------------------------------------------------------------------------------------------
// - Read Register Functions -

// Dummy Register Read
U8 R__(void){
	return 0xFF;
}

// AY Reg 00
U8 R_00(void){
	return AY[AYChip].TF0.b0;
}

// AY Reg 01
U8 R_01(void){
	return AY[AYChip].TF0.b1;
}

// AY Reg 02
U8 R_02(void){
	return AY[AYChip].TF1.b0;
}

// AY Reg 03
U8 R_03(void){
	return AY[AYChip].TF1.b1;
}

// AY Reg 04
U8 R_04(void){
	return AY[AYChip].TF2.b0;
}

// AY Reg 05
U8 R_05(void){
	return AY[AYChip].TF2.b1;
}

// AY Reg 06
U8 R_06(void){
	return AY[AYChip].NF;
}

// AY Reg 07
U8 R_07(void){
	return ~AY[AYChip].MX.b;	// kept inverted
}

// AY Reg 08
U8 R_08(void){
	return AY[AYChip].V0.b >> 1;
}

// AY Reg 09
U8 R_09(void){
	return AY[AYChip].V1.b >> 1;
}

// AY Reg 0A
U8 R_0A(void){
	return AY[AYChip].V2.b >> 1;
}

// AY Reg 0B
U8 R_0B(void){
	return AY[AYChip].EP.b0;
}

// AY Reg 0C
U8 R_0C(void){
	return AY[AYChip].EP.b1;
}

// AY Reg 0D
U8 R_0D(void){
	return AY[AYChip].EC;
}

// AY Reg 0E
U8 R_0E(void){
	return AY[AYChip].IOA;
}

// AY Reg 0F
U8 R_0F(void){
	return AY[AYChip].IOB;
}

// Current number of AY chips
U8 R_30(void){
	return AYChipNum;
}

// AY-Chip Number
U8 R_31(void){
	return AYChip;
}

// Current number of WS channels
U8 R_34(void){
	return WSChNum;
}

// WS Channel Number
U8 R_35(void){
	return WSChan;
}

// Output Audio Sample Rate, Lower Byte
U8 R_36(void){
	return WSFreq.b0;
}

// Output Audio Sample Rate, Higher Byte
U8 R_37(void){
	return WSFreq.b1;
}

// Max supported number of AY chips
U8 R_DA(void){
	return AY_CHIPS_MAX;
}

// Max supported number of WS channels
U8 R_DB(void){
	return WS_CH_MAX;
}

// Device byte 0
U8 R_DC(void){
	return DEV_BYTE0;
}

// Device byte 1
U8 R_DD(void){
	return DEV_BYTE1;
}

// Chip Version
U8 R_DE(void){
	return NUM_VER;
}

// Chip Sub-Version
U8 R_DF(void){
	return NUM_SVER;
}

// Status
U8 R_E0(void){
	return Status.b;
}

// Error Code
U8 R_E1(void){
	return Error;
}

// Data Out
U8 R_E2(void){
	U8 tmp;
	
	tmp = *DataAddr;			// Read new byte
	if (DataCtr && Status.drd)	{
		DataAddr++;
		DataCtr--;
		if (!DataCtr)		{			// If last byte read - end of command
			Status.cmp = 1;
			Status.busy = 0;
			Status.drd = 0;
		}
	}
	return tmp;
}

// Command Acknowledge/Response0
U8 R_E8(void){
	return CmdResp.b0;
}

// Command Acknowledge/Response1
U8 R_E9(void){
	return CmdResp.b1;
}

// Command Acknowledge/Response2
U8 R_EA(void){
	return CmdResp.b2;
}

// Command Acknowledge/Response3
U8 R_EB(void){
	return CmdResp.b3;
}

// Command Acknowledge/Response4
U8 R_EC(void){
	return CmdResp.b4;
}

// Command Acknowledge/Response5
U8 R_ED(void){
	return CmdResp.b5;
}

// Command Acknowledge/Response6
U8 R_EE(void){
	return CmdResp.b6;
}

// Command Acknowledge/Response7
U8 R_EF(void){
	return CmdResp.b7;
}

//------------------------------------------------------------------------------------------
// - Command Functions -

// Illegal command
void C__(void){
	Status.err = 1;
	Error = CMD_ERR_ILG;
}

// Break
void C_00(void){
	Status.brk = 1;
	Error = CMD_ERR_BRK;
	Status.busy = 0;			// Reset BUSY status bit

}

// Get Chip Version String
void C_F0(void){
	Status.acc = 1;
	Status.drd = 1;		// Data is ready to be transferred from Chip to Host
	CmdResp.w0 = sizeof(ChipString);
	DataCtr = sizeof(ChipString);
	DataAddr = ChipString;
}

// Get Copyright String
void C_F1(void){
	Status.acc = 1;
	Status.drd = 1;		// Data is ready to be transferred from Chip to Host
	CmdResp.w0 = sizeof(CprString);
	DataCtr = sizeof(CprString);
	DataAddr = CprString;
}

#include "zx_vect.h"		// Function Vectors carried out to another file to not to be an eyesore

//------------------------------------------------------------------------------------------
// - Common Functions -

// Write Address
void WAddr(U8 addr){
	RegNum = addr;		// Write AY Register Address
	if (!(~addr & MASK_TURBO_AY))	// If Address is from Turbo-AY decoding span,
		AYChip = ~addr & 1;			// select AY chip using bit0 (0 - chip1, 1 - chip0) - NedoPC scheme
}

// Write Registers Functions Caller
void WReg(U8 RegVal){
	WRegVec[RegNum](RegVal);
}

// Read Registers Functions Caller
U8 RReg(void){
	return RRegVec[RegNum]();
}

// Command Functions Caller
void CmdF(U8 cmd){
	if (!Status.busy){		// If another command NOT in progress
		Status.b = 0;			// Clear status (all other status bits will be set by a specific command
		Status.busy = 1;		// Set BUSY status bit
		Error = CMD_ERR_OK;		// Clear error code. Specific command function may modify it later
		CmdFVec[cmd]();			// Call command procedure by its vector
	}

	else if(cmd == CMD_BRK){	// If BREAK issued
		DataCtr = 0;			// Clear data counter
		Status.b = 0;
		Status.brk = 1;		// Set BRK status bit
		Error = CMD_ERR_BRK;	// Set BREAK error code
	}
}

