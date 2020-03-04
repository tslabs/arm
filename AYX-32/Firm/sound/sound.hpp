
// Sound renderer, PSG, WS definitions
//
// AY-ARM project
// (c) TS-Labs
//
// Armoittoman lapsen uni, armoittoman lapsen

#pragma once

/// Enums
enum
{
  SIZE_OF_AMPTAB = 32
};

enum WFORM
{
  WF_SQUARE = 0
};

enum ENV_PHASE
{
  EF_FALL,
  EF_RISE,
  EF_STILL
};

enum CLKSEL
{
  CLK_1_75  = 0,
  CLK_1_775 = 1,
  CLK_3_5   = 2,
  CLK_3_55  = 3,
  CLK_1_0   = 4,
  CLK_2_0   = 5,
  CLK_EXT   = 6,
  CLK_EXT_S = 7,
};

enum PSG_MUL_MODE
{
  PMM_TS  = 0,
  PMM_1   = 1,
  PMM_2   = 2,
  PMM_3   = 3,
  PMM_4   = 4,
  PMM_DIS = 7,
};

enum AMP_TAB
{
  ATAB_AY = 0,
  ATAB_YM = 1,
  ATAB_US = 2,
  ATAB_CS = 3,
};

enum WS_FMT
{
  M_U8,
  M_S8,
  S_U8,
  S_S8,
  M_U16,
  M_S16,
  S_U16,
  S_S16,
};

enum WS_CMD
{
  WSC_END_OF_LIST = 0xFF,
};

enum WS_LOOPTYPE
{
  WSLT_NONE,
  WSLT_FWD,
  WSLT_BIDI,
};

/// Types
typedef void (*E_VEC)();

#pragma pack(1)
typedef struct
{
  u8 num;
  E_VEC addr;
} T_E_VEC;
#pragma pack()

typedef union
{
  struct
  {
    u16 time;
    u8 reg;
    u8 val;
  };
  u32 w;
} BUS_EVT;

typedef struct
{
  u16 time;
} WS_EVT;

typedef union
{
  struct
  {
    u8 enabled :1;
    u8 restart :1;
  };
  u8 b;
} WS_CTRL;

typedef union
{
  u16 h[2];
  u32 w;
} SAMP;

typedef union
{
  struct
  {
    u8  t0:1;
    u8  t1:1;
    u8  t2:1;
    u8  n0:1;
    u8  n1:1;
    u8  n2:1;
    u8  i0:1;
    u8  i1:1;
  };
  u8 b;
} AYMIX_T;

typedef union
{
  struct
  {
    u8  vol:5;
    u8  mode:1;
    u8  padding:2;
  };
  u8 b;
} AYVOL_T;

typedef union
{
  struct
  {
    u8  intfrq:3;  // internal frequency (see. Excel)
    u8  trben:1;   // 0 - turbo scheme disabled, 1 - enabled
    u8  selen:1;   // 0 - SEL pin disables, 1 - enabled
    u8  iclk:1;    // 0 - ext pin, 1 - int clock
    u8  padding:1;
    u8  type:1;    // 0 - AY / 1 - YM
  };
  u8 b;
} AYCTRL;

typedef struct
{
  u8 amp;
  bool is_env;
  bool is_tone;
  bool is_noise;
} PSG_CHAN;

typedef struct
{
  u16 cnt;
  union
  {
    struct
    {
      u8 periodl :8;
      u8 periodh :4;
    };
    u16 period;
  };
  bool bit;
} TONE_GEN;

typedef struct
{
  u32 seed;
  u16 cnt;
  u8 period;
} NOISE_GEN;

typedef struct
{
  u16 cnt;
  union
  {
    struct
    {
      u8 periodl :8;
      u8 periodh :8;
    };
    u16 period;
  };
  u8 type;
  ENV_PHASE phase;
  u8 amp;
} ENV_GEN;

typedef struct
{
  void *addr;
  void *end_addr;
  u16 addr_f;
  u16 step_f;
  u8 step_i;
  WS_FMT fmt;
  u8 vol[2];
  bool is_enabled;
  bool is_fwd;
} WS_CHAN;

typedef struct
{
  void *start_addr;
  void *end_addr;
  void *loop_addr;
  u32 c5_freq;
  u8 vol[2];
  WS_FMT fmt;
  WS_LOOPTYPE loop_type;
} WS_SAMP;

#pragma pack(1)
typedef union
{
  struct
  {
    CLKSEL clksel :3;
  };
  u8 b;
} PSGCCTRL;

typedef union
{
  struct
  {
    PSG_MUL_MODE psgmul :3;
  };
  u8 b;
} PSGBCTRL;

typedef union
{
  struct
  {
    AMP_TAB ampsel0 :2;
    AMP_TAB ampsel1 :2;
    AMP_TAB ampsel2 :2;
    AMP_TAB ampsel3 :2;
  };
  u8 b;
} PSGACTRL;

typedef struct
{
  u16 ver;
  u16 amptab[SIZE_OF_AMPTAB];
  PSGCCTRL clkctr;
  PSGBCTRL busctr;
  PSGACTRL ampctr;
  u8 psgvol[PSG_CHIPS_MAX][3][2];
} CONFIG;

typedef union
{
  struct
  {
    u8 fmt :3;
    u8 _pad :4;
    u8 en :1;
  };
  u8 b;
} DACCTRL;
#pragma pack()

/// Variables
E_VEC event_vec[256];

u16 buf_time; // the timestamp of 1st sample in current buffer
u8 curr_buf;  // current buffer to render
u32 *sndbuf;  // address of current buffer
int offs[max(PSG_CHIPS_MAX, WS_CHAN_MAX)];     // current offset in buffer
u16 snum;     // number of samples to render
u32 dac_buf[2][DAC_SAMPLES_COUNT] __attribute__((section(".sram2")));

u8 psg_bus_events_buf[PSG_BUS_EVENTS_SIZE];
FIFO psg_bus_events;

BUS_EVT bus_evt;  // next PSG event to process
bool is_bus_evt;  // next PSG event validity flag

CONFIG config;

// PSG
u8 psg_chip_num; // current number of PSG chips
u8 selected_psg; // currently selected PSG chip

PSG_CHAN chan[PSG_CHIPS_MAX][3];
TONE_GEN tone[PSG_CHIPS_MAX][3];
NOISE_GEN noise[PSG_CHIPS_MAX];
ENV_GEN env[PSG_CHIPS_MAX];
SAMP vtab[PSG_CHIPS_MAX][3][SIZE_OF_AMPTAB];
u16 *amptab_ptr[PSG_CHIPS_MAX];
u8 psgvol[PSG_CHIPS_MAX][3][2];

u8 tone_buf[DAC_SAMPLES_COUNT];
u8 noise_buf[DAC_SAMPLES_COUNT];
u8 env_buf[DAC_SAMPLES_COUNT];
u32 xlf_buf[1024];
u32 xlf_acc[2];
u16 xlf_ptr;

// WS
u8 &ws_chan_num = bus::readback[bus::R_WS_NUM];     // current number of WS channels
u8 selected_ws;    // currently selected PSG chip
u8 selected_samp;  // currently selected sample number

WS_CHAN wschan[WS_CHAN_MAX];
WS_SAMP wssamp[WS_SAMP_MAX];

WS_EVT ws_evt;
u8 is_ws_evt;

u8 ws_int_cmdlist_buf[WS_CMDLIST_SIZE];
u8 ws_ext_cmdlist_buf[WS_CMDLIST_SIZE];
FIFO ws_int_cmdlist;
FIFO ws_ext_cmdlist;

u8 ws_events_buf[WS_EVENTS_SIZE];
FIFO ws_events;

u32 ws_buf[WS_SAMPLES_COUNT];

u8 dac_fifo_buf[DAC_FIFO_SIZE];
FIFO dac_fifo;

/// Constants
// Credits to Introspec for AY/YM tables
const u16 amptab_ay[] =
{
  0x0000, 0x0000, 0x028F, 0x028F, 0x03B3, 0x03B3, 0x0564, 0x0564,
  0x07DC, 0x07DC, 0x0BA9, 0x0BA9, 0x1083, 0x1083, 0x1B7C, 0x1B7C,
  0x2068, 0x2068, 0x347A, 0x347A, 0x4ACE, 0x4ACE, 0x5F72, 0x5F72,
  0x7E16, 0x7E16, 0xA2A4, 0xA2A4, 0xCE3A, 0xCE3A, 0xFFFF, 0xFFFF
};

const u16 amptab_ym[] =
{
  0x0000, 0x0000, 0x0131, 0x01FA, 0x02CE, 0x0393, 0x045A, 0x0520,
  0x063D, 0x079A, 0x08FA, 0x0A57, 0x0C6D, 0x0EEF, 0x116C, 0x13E9,
  0x17AF, 0x1C70, 0x2137, 0x2603, 0x2D3A, 0x3628, 0x3F13, 0x47F6,
  0x556F, 0x6682, 0x77A6, 0x88D0, 0xA29A, 0xC20C, 0xE142, 0xFFFF
};

const u16 amptab_us[] =
{
  0x0000, 0x0000, 0x00EF, 0x01D0, 0x0290, 0x032A, 0x03EE, 0x04D2,
  0x0611, 0x0782, 0x0912, 0x0A36, 0x0C31, 0x0EB6, 0x1130, 0x13A0,
  0x1751, 0x1BF5, 0x20E2, 0x2594, 0x2CA1, 0x357F, 0x3E45, 0x475E,
  0x5502, 0x6620, 0x7730, 0x8844, 0xA1D2, 0xC102, 0xE0A2, 0xFFFF
};

// Limit of chips (see PSG_MUL_MODE)
const u8 lim_chip[8] = {2, 1, 2, 3, 4, 1, 1, 0};
const u16 *amp_tab_addr[4] = {amptab_ay, amptab_ym, amptab_us, config.amptab};

/// Functions
void put_bus_evt(u8, u8);
void put_ws_event();
void process_bus_event();
bool save_cfg();
void init_psg_generators();
void init_ws_channels();
void init_envelope(ENV_GEN&, u8);
void init_vtab(u8, u8, u8, u8);
void init_vectors();
