
// Chip registers definition
//
// AY-ARM project
// (c)2015 TS-Labs
//
// A lány még mindig nagyon szép, de hol van már a vágy

/// Register definitions
enum REG
{
  // AY generic
  R_AY_TF_AL   = 0x00,
  R_AY_TF_AH   = 0x01,
  R_AY_TF_BL   = 0x02,
  R_AY_TF_BH   = 0x03,
  R_AY_TF_CL   = 0x04,
  R_AY_TF_CH   = 0x05,
  R_AY_NF      = 0x06,
  R_AY_MX      = 0x07,
  R_AY_V_A     = 0x08,
  R_AY_V_B     = 0x09,
  R_AY_V_C     = 0x0A,
  R_AY_EPL     = 0x0B,
  R_AY_EPH     = 0x0C,
  R_AY_EC      = 0x0D,
  R_AY_IOA     = 0x0E,
  R_AY_IOB     = 0x0F,

  // AY extended
  R_AY_VOL_AL  = 0x10,
  R_AY_VOL_AR  = 0x11,
  R_AY_VOL_BL  = 0x12,
  R_AY_VOL_BR  = 0x13,
  R_AY_VOL_CL  = 0x14,
  R_AY_VOL_CR  = 0x15,
  R_AY_ENV_A   = 0x16,
  R_AY_ENV_B   = 0x17,
  R_AY_ENV_C   = 0x18,

  // WS
  R_WS_SAMPL   = 0x20,
  R_WS_SAMPH   = 0x21,
  R_WS_SOFFS0  = 0x22,
  R_WS_SOFFS1  = 0x23,
  R_WS_SOFFS2  = 0x24,
  R_WS_SOFFS3  = 0x25,
  R_WS_SFREQL  = 0x26,
  R_WS_SFREQH  = 0x27,
  R_WS_VOLL    = 0x28,
  R_WS_VOLR    = 0x29,

  // AY, WS, DAC
  R_AY_NUM     = 0x30,
  R_AY_CSEL    = 0x31,
  R_AY_CCTRL   = 0x32,
  R_WS_NUM     = 0x34,
  R_WS_CSEL    = 0x35,
  R_WS_FREQ_L  = 0x36,
  R_WS_FREQ_H  = 0x37,
  R_WS_BPM     = 0x38,
  R_M_VOL_L    = 0x39,
  R_M_VOL_R    = 0x3A,
  R_DAC_MODE   = 0x3B,
  R_DAC_LL     = 0x3C,
  R_DAC_LH     = 0x3D,
  R_DAC_RL     = 0x3E,
  R_DAC_RH     = 0x3F,
  R_AY_ENV_ADR = 0x40,
  R_AY_ENV_DAT = 0x41,

  // System info
  R_VER_STR    = 0xD8,
  R_CPR_STR    = 0xD9,
  R_AY_MAX     = 0xDA,
  R_WS_MAX     = 0xDB,
  R_D_BYTE0    = 0xDC,
  R_D_BYTE1    = 0xDD,
  R_CHIP_VER   = 0xDE,
  R_CHIP_SVER  = 0xDF,

  // Command
  R_CMD_STATUS = 0xE0,
  R_ERROR      = 0xE1,
  R_DATA       = 0xE2,
  R_ARG_RESP   = 0xE3,
  R_MODE       = 0xE8
};

/// Commands
enum CMD
{
  C_BRK = 0x00
};

/// Error codes
enum ERR
{
  E_OK  = 0x00,
  E_ILG = 0x01,
  E_BRK = 0x02,
  E_MOD = 0x03,
  E_PAR = 0x04
};

/// Strings
const u8 chipstring[] = {CHIP_STRING};
const u8 cprstring[] = {CPR_STRING};

/// Structures
// command status
typedef union
{
  struct
  {
    u8  busy:1;   // Command in progress
    u8  drq:1;    // Data Request. Byte should be sent to chip
    u8  drd:1;    // Data Ready. Byte should be read from chip
    u8  ndat:1;   // No data to transfer
    u8  brk:1;    // Command interrupted
    u8  cmp:1;    // Command completed
    u8  acc:1;    // Command accepted
    u8  err:1;    // Error occured
  };
  u8 b;
} STAT;
