
// PSG definitions
//
// AY-ARM project
// (c)2015 TS-Labs
//
// Iske rauta

enum
{
  PSG_TYP_AY,
  PSG_TYP_YM
};

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
{  //registers re-ordered to match memory alignment
  u32      sd_ns;    // Seed for Noise Generator
  w16      TF0;      // AY Reg #00, #01 - Tone frequency for CH0
  w16      TF1;      // AY Reg #02, #03 - Tone frequency for CH1
  w16      TF2;      // AY Reg #04, #05 - Tone frequency for CH2
  w16      EP;       // AY Reg #0B, #0C - Envelope period
  u16      ctr_tn0;  // Tone Generator Counter0
  u16      ctr_tn1;  // Tone Generator Counter1
  u16      ctr_tn2;  // Tone Generator Counter2
  u16      ctr_ev;   // Envelope Period Counter
  u8       NF;       // AY Reg #06 - Noise frequency
  AYMIX_T  MX;       // AY Reg #07 - Channels mux, I/O mode
  AYVOL_T  V0;       // AY Reg #08 - Volume for CH0
  AYVOL_T  V1;       // AY Reg #09 - Volume for CH1
  AYVOL_T  V2;       // AY Reg #0A - Volume for CH2
  u8       EC;       // AY Reg #0D - Envelope control
  u8       IOA;      // AY Reg #0E - I/O port A
  u8       IOB;      // AY Reg #0F - I/O port B
  u8       ctr_ns;   // Noise Generator Counter
  u8       env_rld;  // Reload of register 0D
  u8       env_sel;  // Envelope table selector (0-7)
  u8       env_ctr;  // Counter of Envelope phase (0-31)
  u8       env_ph;   // Phase of Envelope (0 - down, 1 - up). 'Up' means move forward by the envelope table
  u8       env_st;   // State of Envelope (0 - stall, 1 - run)
  u8       vol_0l;   // Channel A Volume Left  (0-64)
  u8       vol_0r;   // Channel A Volume Right
  u8       vol_1l;   // Channel B Volume Left
  u8       vol_1r;   // Channel B Volume Right
  u8       vol_2l;   // Channel C Volume Left
  u8       vol_2r;   // Channel C Volume Right
  u8       ph_tn0;   // Phase for Tone Generator0 (0 - off, 1 - on)
  u8       ph_tn1;   // Phase for Tone Generator1 (0 - off, 1 - on)
  u8       ph_tn2;   // Phase for Tone Generator2 (0 - off, 1 - on)
  u8       padding[21];
} PSG_REGS;
