
// Commands definition
//
// AYX-32 project
// (c) TS-Labs
//
// من اگه جای تو بودم

/// Commands
enum CMD
{
  C_BREAK     = 0x00,
  C_UPLD_FW   = 0xE8,
  C_FLASH_FW  = 0xE9,
  C_SAVE_CFG  = 0xEA,
  C_RESET     = 0xEB,
};

/// Error codes
enum ERR
{
  E_NONE    = 0x00,
  E_DONE    = 0x01,
  E_BREAK   = 0x02,
  E_BUSY    = 0x03,
  E_CMDERR  = 0x04,
  E_MODERR  = 0x05,
  E_PARMERR = 0x06,
  E_DATAERR = 0x07,
  E_SEQERR  = 0x08,
  E_EXECERR = 0x09,
};

/// Structures
// command status
enum
{
#ifdef BOOT
  STATUS_CLR = 1 << 7,
#else
  STATUS_CLR = 0,
#endif
};

typedef union
{
  struct
  {
    u8 busy:1;   // bit 0: Command in progress
    u8 drq:1;    // bit 1: Data request
    u8 xxx:5;    // bit 2..6
    u8 boot:1;   // bit 7: Boot mode
  };
  u8 b;
} STATUS;

typedef void (*C_VEC)();

/// Constants
enum
{
  MAGIC_FFW = 0x7841AA55, // 'Flash Firmware' parameter
  MAGIC_CFG = 0x37C855AA, // 'Save Config' parameter
  MAGIC_RES = 0xDEADBEEF, // 'Reset' parameter
};

/// Functions
void command(u8);
extern const C_VEC command_vec[];

// tasks
#ifndef BOOT
void bg_save_cfg();
#endif
#ifdef BOOT
void t_up_fw();
void bg_up_fw();
void bg_fl_fw();
#endif
