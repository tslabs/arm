
// common types
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;
typedef signed char s8;
typedef signed short s16;
typedef signed long s32;
typedef u8 bool;
typedef void (*TASK)();

// macros
#define false 0
#define true 1
#define max(a,b)  (((a) > (b)) ? (a) : (b))
#define min(a,b)  (((a) < (b)) ? (a) : (b))
#define countof(a)  (sizeof(a) / sizeof(a[0]))

// structs
typedef struct
{
  u8 hdr;       // Header version always 1
  u8 hw;        // H/W version
  u16 ver;      // F/W version
  u16 sver;     //
  u16 offs;     // data offset
  u32 size;     // F/W size
  u16 crc[];    // CRC16 for chunks (one per 256, 768 bytes max)
} FW_HDR;

typedef struct
{
  u16 hw;
  u16 fw;
  u16 cf;
} DEV_VER;

typedef union
{
  struct
  {
    u8 clksel :3;
  };
  u8 b;
} PSGCCTRL;

typedef union
{
  struct
  {
    u8 psgmul :3;
  };
  u8 b;
} PSGBCTRL;

typedef union
{
  struct
  {
    u8 ampsel0 :2;
    u8 ampsel1 :2;
    u8 ampsel2 :2;
    u8 ampsel3 :2;
  };
  u8 b;
} PSGACTRL;

// vars
u8 cx, cy;    // text coordinates
u8 defx;      // left X, default
u8 cc;        // text attributes

u8 menu;      // current menu
bool req_unpress;
TASK task;

PSGACTRL atb_sel;
PSGBCTRL bus_sel;
PSGCCTRL clk_sel;
u16 c_amp[32];

// AYX-32 regs
enum REG
{
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
  R_DAC_CTRL    = 0x40,
  R_DAC_VOL_L   = 0x41,
  R_DAC_VOL_R   = 0x42,
  R_DAC_SMPR    = 0x43,
  R_DAC_FREE    = 0x44,
  R_DAC_USED    = 0x45,
  R_DAC_DATA    = 0x46,

  // Device Control
  R_PSG_SEL     = 0xD0,
  R_PSG_CCTRL   = 0xD1,
  R_PSG_BCTRL   = 0xD2,
  R_PSG_ACTRL   = 0xD3,
  R_M_VOL_L     = 0xD4,
  R_M_VOL_R     = 0xD5,
  R_PSG_AMP_TAB = 0xD6,

  // System
  R_DEV_SIG     = 0xE0, // Rd
  R_CMD         = 0xE1, // W
  R_STATUS      = 0xE1, // R
  R_ERROR       = 0xE2, // R
  R_PARAM       = 0xE3, // Wd
  R_RESP        = 0xE3, // Rd
  R_DATA        = 0xE4, // RdWd
  R_UPTIME      = 0xEB, // Rd
  R_VER         = 0xEC, // Rd
  R_CPR_STR     = 0xED, // Rd
  R_BLD_STR     = 0xEE, // Rd
  R_CORE_FRQ    = 0xEF, // Rd
};

// Commands
enum CMD
{
  C_BREAK     = 0x00,
  C_LOCK      = 0xE4,
  C_UP_FW     = 0xE8,
  C_FLASH_FW  = 0xE9,
  C_SAVE_CFG  = 0xEA,
  C_RESET     = 0xEB
};

// Error codes
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

// Status
enum STAT
{
  S_BUSY = 0x01,
  S_DRQ  = 0x02,
  S_BOOT = 0x80
};

// Constants
enum MAGIC
{
  M_DEVSIG = 0xAA55,      // Device signature
  MAGIC_FFW = 0x7841AA55, // 'Flash Firmware' parameter
  MAGIC_CFG = 0x37C855AA, // 'Save Config' parameter
  MAGIC_RES = 0xDEADBEEF, // 'Reset' parameter
  MAGIC_LCK = 0xC0DEFACE, // 'Unlock' parameter
};

// keys
enum
{
  K_CS = 0,
  K_Z,
  K_X,
  K_C,
  K_V,
  K_A,
  K_S,
  K_D,
  K_F,
  K_G,
  K_Q,
  K_W,
  K_E,
  K_R,
  K_T,
  K_1,
  K_2,
  K_3,
  K_4,
  K_5,
  K_0,
  K_9,
  K_8,
  K_7,
  K_6,
  K_P,
  K_O,
  K_I,
  K_U,
  K_Y,
  K_EN,
  K_L,
  K_K,
  K_J,
  K_H,
  K_SP,
  K_SS,
  K_M,
  K_N,
  K_B,
  K_NONE
};

// colors
enum
{
  C_NORM = 0x47,
  C_OK   = 0x44,
  C_BUTN = 0x44,
  C_ERR  = 0x42,
  C_WARN = 0x46,
  C_ACHT = 0x43,
  C_CRIT = 0x42,
  C_HEAD = 0x45,
  C_SEL  = 0x46,
  C_INFO = 0x44,
  C_DATA = 0x46,
  C_QUST = 0x46,
  C_FRAM = 0x07
};

// menus
enum
{
  M_MAIN,
  M_INFO,
  M_SET,
  M_SET_B,
  M_SET_C,
  M_MIX,
  M_MIX_1,
  M_MIX_2,
  M_MIX_3,
  M_AMP,
  M_AMP_1,
  M_AMP_2,
  M_AMP_3,
  M_AMP_4,
  M_CUST_AMP,
  M_DAC,
  M_RES,
  M_BOOT,
  M_SAVE,
  M_SAVE1,
  M_FUPD,
  M_FUPD1
};

char *const clk_sel_txt[] =
{
  "Internal 1.75MHz",
  "Internal 1.775MHz",
  "Internal 3.5MHz",
  "Internal 3.55MHz",
  "Internal 1.0MHz",
  "Internal 2.0MHz",
  "External",
  "External with SEL"
};

char *const atab_sel_txt[] =
{
  "AY-3-8912",
  "YM-2149F",
  "Unreal Speccy",
  "Custom"
};

char *const bus_sel_txt[] =
{
  "2 chips (TS)",
  "1 chip",
  "2 chips",
  "3 chips",
  "4 chips",
  "",
  "",
  "Disable PSG"
};

const u8 const mix_sel_val[][6] = 
{
  {64, 0, 32, 32, 0, 64},
  {48, 16, 32, 32, 16, 48},
  {32, 32, 32, 32, 32, 32}
};
