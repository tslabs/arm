
// Sound renderer definitions
//
// AY-ARM project
// (c) TS-Labs
//
// Armoittoman lapsen uni, armoittoman lapsen

#pragma once

/// Enums
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

/// Typedefs
typedef struct
{
  u16 time;
  u8 reg;
  u8 val;
} BUS_EVT;

typedef struct
{
  u8 amp;
  bool is_env;
  bool is_tone;
  bool is_noise;
} PSG_CHAN;

typedef struct
{
  bool bit;
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
} TONE_GEN;

typedef struct
{
  u16 cnt;
  u8 period;
  union
  {
    struct
    {
      u16 pad;
      bool bit :1;
    };
    u32 seed;
  };
} NOISE_GEN;

typedef struct
{
  u16 cnt;
  union
  {
    struct
    {
      u8 pad :4;
      u8 periodl :8;
      u8 periodh :4;
    };
    u16 period;
  };
  u8 type;
  ENV_PHASE phase;
  u8 amp;
} ENV_GEN;

typedef union
{
  struct
  {
    u16 l;
    u16 r;
  };
  u32 w;
} SAMP;

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

typedef union
{
  struct
  {
    bool en :1;
    bool size :1;
    bool mode :1;
    bool sign :1;
  };
  u8 b;
} DACCTRL;

typedef struct
{
  u16 ver;
  u16 amptab[32];
  PSGCCTRL clkctr;
  PSGBCTRL busctr;
  PSGACTRL ampctr;
} CONFIG;
#pragma pack()

/// Constants
// Limit of chips (see PSG_MUL_MODE)
const u8 lim_chip[8] = {2, 1, 2, 3, 4, 1, 1, 0};

/// Variables
PSG_CHAN chan[PSG_CHIPS_MAX][3];
TONE_GEN tone[PSG_CHIPS_MAX][3];
NOISE_GEN noise[PSG_CHIPS_MAX];
ENV_GEN env[PSG_CHIPS_MAX];
u16 vtab[PSG_CHIPS_MAX][3][16][2];

u8 tone_buf[DAC_SAMPLES_COUNT];
u8 noise_buf[DAC_SAMPLES_COUNT];
u8 env_buf[DAC_SAMPLES_COUNT];

u8 ay_bus_events_buf[AY_BUS_EVENTS_SIZE];
FIFO ay_bus_events;
u8 dac_fifo_buf[DAC_FIFO_SIZE];
FIFO dac_fifo;

u8 psg_chip_num;      // current number of PSG chips
u8 current_psg_chip;  // selected PSG chip
u8 wschan_num;        // current number of WS channels

u16 buf_time; // the timestamp of 1st sample in current buffer
u8 curr_buf;  // current buffer to render
u32 *sndbuf;  // address of current buffer
u16 offs;     // current offset in buffer
u16 snum;     // number of samples to render

CONFIG config;

BUS_EVT evt;  // next event to process
bool is_evt;  // next event validity flag

/// Functions
void put_event(u8, u8);
void process_event();
void init_generators();
void init_envelope(ENV_GEN&, u8);
void init_vtab(u8, u8, u8, u8);
bool save_cfg();
