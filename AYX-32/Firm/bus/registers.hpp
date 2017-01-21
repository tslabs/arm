
// Chip registers definition
//
// AYX-32 project
// (c) TS-Labs
//
// A lány még mindig nagyon szép, de hol van már a vágy

/// Register definitions
enum REG
{
#ifndef BOOT
  // AY generic
  R_PSG_TP_AL   = 0x00,
  R_PSG_TP_AH   = 0x01,
  R_PSG_TP_BL   = 0x02,
  R_PSG_TP_BH   = 0x03,
  R_PSG_TP_CL   = 0x04,
  R_PSG_TP_CH   = 0x05,
  R_PSG_NP      = 0x06,
  R_PSG_MX      = 0x07,
  R_PSG_V_A     = 0x08,
  R_PSG_V_B     = 0x09,
  R_PSG_V_C     = 0x0A,
  R_PSG_EPL     = 0x0B,
  R_PSG_EPH     = 0x0C,
  R_PSG_EC      = 0x0D,
  R_PSG_IOA     = 0x0E,
  R_PSG_IOB     = 0x0F,

  // AY extended
  R_PSG_VOL_AL  = 0x10,
  R_PSG_VOL_AR  = 0x11,
  R_PSG_VOL_BL  = 0x12,
  R_PSG_VOL_BR  = 0x13,
  R_PSG_VOL_CL  = 0x14,
  R_PSG_VOL_CR  = 0x15,
  R_PSG_TDC_A   = 0x16,
  R_PSG_TDC_B   = 0x17,
  R_PSG_TDC_C   = 0x18,

  // DAC
  R_DACCTRL     = 0x40,
  R_DACVOLL     = 0x41,
  R_DACVOLR     = 0x42,
  R_DACSMPR     = 0x43,
  R_DACFREE     = 0x44,
  R_DACUSED     = 0x45,
  R_DACDATA     = 0x46,

  // Device Control
  R_PSG_SEL     = 0xD0,
  R_PSG_CCTRL   = 0xD1,
  R_PSG_BCTRL   = 0xD2,
  R_PSG_ACTRL   = 0xD3,
  R_M_VOL_L     = 0xD4,
  R_M_VOL_R     = 0xD5,
  R_PSG_AMP_TAB = 0xD6,
  R_PUSH_UP     = 0xD7,
#endif

  // System
  R_DEV_SIG     = 0xE0,
  R_CMD         = 0xE1,
  R_STATUS      = 0xE1,
  R_ERROR       = 0xE2,
  R_PARAM       = 0xE3,
  R_RESP        = 0xE3,
  R_DATA        = 0xE4,
  R_CPU_LOAD    = 0xEA,
  R_UPTIME      = 0xEB,
  R_VER         = 0xEC,
  R_CPR_STR     = 0xED,
  R_BLD_STR     = 0xEE,
  R_CORE_FRQ    = 0xEF,
};
