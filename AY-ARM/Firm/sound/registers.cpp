
// Chip registers read/write
//
// AY-ARM project
// (c)2015 TS-Labs
//
// Tiktai tau galbūt šypseną sukels keliom sekundėm

#include "ay.hpp"
#include "ws.hpp"
#include "registers.hpp"

/// Variables
u8 regnum = 0;        // currently selected chip register
u8 regval = 0;        // calue to be written into the register (should be replaced by read Z80 Bus value)
u8 cmd = 0;           // command
STAT status = {};     // status byte
u8 error = 0;         // error code
u8 cmdarg[16] = {};   // command arguments
u8 cmdresp[16] = {};  // command Acknowledge/Response

u8 *dataraddr_nx;     // pointer to next data to be read
u32 datarlen_nx;      // length of next data to be read
u8 *datawaddr_nx;     // pointer to next data to be written
u32 datawlen_nx;      // length of next data to be written

u8 *dataraddr;        // pointer to current data to be read
u8 *datawaddr;        // pointer to current data to be written
u32 datarctr = 0;     // counter for data to be read, bytes
u32 datawctr = 0;     // counter for data to be written, bytes

PSG_REGS ay_chip[AY_CHIPS_MAX]; // registers for virtual PSG chips
u8 aychip_num = AY_CHIPS_DEF;   // current number of PSG chips
u8 aychip_sel = 0;              // selected PSG chip

WS_REGS ws_chan[WS_CH_MAX]; // registers for WS channels
u8 wschan_num = WS_CH_DEF;  // current number of WS channels
u8 wschan_sel = 0;          // selected WS channel
u16 ws_freq = WS_FREQ;

/// Functions
void command(u8);

// Write AY register address
void write_address(u8 addr)
{
  regnum = addr;

  if (!(~addr & TURBO_AY_MASK))   // if Address is from Turbo-AY decoding span,
    aychip_sel = ~addr & 1;       // select AY chip using bit0 (0 - chip1, 1 - chip0) - NedoPC scheme

  else
  switch(regnum)
  {
    // Get Chip Version String
    case R_VER_STR:
      dataraddr = (u8*)chipstring;
      datarctr = sizeof(chipstring);
    break;

    // Get Copyright String
    case R_CPR_STR:
      dataraddr = (u8*)cprstring;
      datarctr = sizeof(cprstring);
    break;

    case R_DATA:
      dataraddr = dataraddr_nx;
      datarctr = datarlen_nx;
      datawaddr = datawaddr_nx;
      datawctr = datawlen_nx;
    break;

    case R_ARG_RESP:
      dataraddr = cmdresp;
      datarctr = sizeof(cmdresp);
      datawaddr = cmdarg;
      datawctr = sizeof(cmdarg);
    break;
  };
}

// Write AY Register
// Note: splitted into four parts to allow GCC implement TBB/TBH branching
void write_reg(u8 val)
{
  switch(regnum)  // 00-3F
  {
    // AY Reg 00
    case 0x00:
      ay_chip[aychip_sel].TF0.b0 = val;
    return;

    // AY Reg 01
    case 0x01:
      ay_chip[aychip_sel].TF0.b1 = val & 0x0F;
    return;

    // AY Reg 02
    case 0x02:
      ay_chip[aychip_sel].TF1.b0 = val;
    return;

    // AY Reg 03
    case 0x03:
      ay_chip[aychip_sel].TF1.b1 = val & 0x0F;
    return;

    // AY Reg 04
    case 0x04:
      ay_chip[aychip_sel].TF2.b0 = val;
    return;

    // AY Reg 05
    case 0x05:
      ay_chip[aychip_sel].TF2.b1 = val & 0x0F;
    return;

    // AY Reg 06
    case 0x06:
      ay_chip[aychip_sel].NF = val & 0x1F;
    return;

    // AY Reg 07
    case 0x07:
      ay_chip[aychip_sel].MX.b = val;
    return;

    // AY Reg 08
    case 0x08:
      ay_chip[aychip_sel].V0.b = (val & 0x1F) << 1;
    return;

    // AY Reg 09
    case 0x09:
      ay_chip[aychip_sel].V1.b = (val & 0x1F) << 1;
    return;

    // AY Reg 0A
    case 0x0A:
      ay_chip[aychip_sel].V2.b = (val & 0x1F) << 1;
    return;

    // AY Reg 0B
    case 0x0B:
      ay_chip[aychip_sel].EP.b0 = val;
    return;

    // AY Reg 0C
    case 0x0C:
      ay_chip[aychip_sel].EP.b1 = val;
    return;

    // AY Reg 0D
    case 0x0D:
      ay_chip[aychip_sel].EC = val & 0x0F;
      ay_chip[aychip_sel].env_rld = 1;
    return;

    // AY Reg 0E
    case 0x0E:
      ay_chip[aychip_sel].IOA = val;
    return;

    // AY Reg 0F
    case 0x0F:
      ay_chip[aychip_sel].IOB = val;
    return;

    // Current number of AY chips
    case 0x30:
      aychip_num = (val >= AY_CHIPS_MAX) ? (AY_CHIPS_MAX - 1) : val;
      if (aychip_sel >= aychip_num)
        aychip_sel = aychip_num - 1;
    return;

    // AY Chip Select
    case 0x31:
      aychip_sel = (val > aychip_num) ? aychip_num : val;;
    return;

    // Current number of WS channels
    case 0x34:
      wschan_num = (val >= WS_CH_MAX) ? WS_CH_MAX - 1 : val;
      if (wschan_sel >= wschan_num)
        wschan_sel = wschan_num - 1;
    return;

    // WS Channel Select
    case 0x35:
      wschan_sel = (val > wschan_num) ? wschan_num : val;
    return;
  }

  switch(regnum)  // 40-7F
  {
  }

  switch(regnum)  // 80-BF
  {
  }

  switch(regnum)  // C0-FF
  {
    // Command
    case 0xE0:
      command(val);
    return;

    // Data In
    case 0xE2:
    return;
  }
}

// Read AY register
// Note: splitted into four parts to allow GCC implement TBB/TBH branching
u8 read_reg()
{
  switch(regnum)  // 00-3F
  {
    // AY Reg 00
    case 0x00:
    return ay_chip[aychip_sel].TF0.b0;

    // AY Reg 01
    case 0x01:
    return ay_chip[aychip_sel].TF0.b1;

    // AY Reg 02
    case 0x02:
    return ay_chip[aychip_sel].TF1.b0;

    // AY Reg 03
    case 0x03:
    return ay_chip[aychip_sel].TF1.b1;

    // AY Reg 04
    case 0x04:
    return ay_chip[aychip_sel].TF2.b0;

    // AY Reg 05
    case 0x05:
    return ay_chip[aychip_sel].TF2.b1;

    // AY Reg 06
    case 0x06:
    return ay_chip[aychip_sel].NF;

    // AY Reg 07
    case 0x07:
    return ay_chip[aychip_sel].MX.b;

    // AY Reg 08
    case 0x08:
    return ay_chip[aychip_sel].V0.b >> 1;

    // AY Reg 09
    case 0x09:
    return ay_chip[aychip_sel].V1.b >> 1;

    // AY Reg 0A
    case 0x0A:
    return ay_chip[aychip_sel].V2.b >> 1;

    // AY Reg 0B
    case 0x0B:
    return ay_chip[aychip_sel].EP.b0;

    // AY Reg 0C
    case 0x0C:
    return ay_chip[aychip_sel].EP.b1;

    // AY Reg 0D
    case 0x0D:
    return ay_chip[aychip_sel].EC;

    // AY Reg 0E
    case 0x0E:
    return ay_chip[aychip_sel].IOA;

    // AY Reg 0F
    case 0x0F:
    return ay_chip[aychip_sel].IOB;

    // Current number of AY chips
    case 0x30:
    return aychip_num;

    // AY-Chip Number
    case 0x31:
    return aychip_sel;

    // Current number of WS channels
    case 0x34:
    return wschan_num;

    // WS Channel Number
    case 0x35:
    return wschan_sel;

    // Output Audio Sample Rate, Lower Byte
    case 0x36:
    return ((w16*)&ws_freq)->b0;

    // Output Audio Sample Rate, Higher Byte
    case 0x37:
    return ((w16*)&ws_freq)->b1;
  }

  switch(regnum)  // 40-7F
  {
  }

  switch(regnum)  // 80-BF
  {
  }

  switch(regnum)  // C0-FF
  {
    // Max supported number of AY chips
    case R_AY_MAX:
    return AY_CHIPS_MAX;

    // Max supported number of WS channels
    case R_WS_MAX:
    return WS_CH_MAX;

    // Device byte 0
    case R_D_BYTE0:
    return DEV_BYTE0;

    // Device byte 1
    case R_D_BYTE1:
    return DEV_BYTE1;

    // Chip Version
    case R_CHIP_VER:
    return NUM_VER;

    // Chip Sub-Version
    case R_CHIP_SVER:
    return NUM_SVER;

    // status
    case R_CMD_STATUS:
    return status.b;

    // error Code
    case R_ERROR:
    return error;

    // Data Out
    case R_VER_STR:
    case R_CPR_STR:
    case R_DATA:
    case R_ARG_RESP:
    {
      u8 tmp = 0;
      if (datarctr)
      {
        tmp = *dataraddr;      // Read new byte
        dataraddr++;
        datarctr--;
      }
      return tmp;
    }
  }

  return 0xFF;
}

// Execute command
// Note: splitted into four parts to allow GCC implement TBB/TBH branching
void command(u8 cmd)
{
  // another command in progress but BREAK issued
  if (status.busy)
  {
    if (cmd == C_BRK)
    {
      datarctr = 0;       // Clear data counter
      status.b = 0;
      status.brk = 1;     // Set BRK status bit
      error = E_BRK;      // Set BREAK error code
    }
  }

  else
  {
    status.b = 0;       // Clear status (all other status bits will be set by a specific command
    status.busy = 1;    // Set BUSY status bit
    error = E_OK;       // Clear error code. Specific command function may modify it later

    switch(cmd)  // 00-3F
    {
      // Break
      case 0x00:
        status.brk = 1;
        error = E_BRK;
        status.busy = 0;      // Reset BUSY status bit
      return;
    }

    switch(cmd)  // 40-7F
    {
    }

    switch(cmd)  // 80-BF
    {
    }

    switch(cmd)  // C0-FF
    {
    }

    // Illegal command
    status.err = 1;
    error = E_ILG;
  }

  return;

drd:
  *(u32*)cmdresp = sizeof(datarlen_nx);
  status.drd = 1;    // Data is ready to be transferred from Chip to Host
acc:
  status.acc = 1;
}
