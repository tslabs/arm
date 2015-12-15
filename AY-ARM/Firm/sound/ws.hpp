
// Wave sythesis definitions
//
// AY-ARM project
// (c)2015 TS-Labs
//
// I wtedy wreszcie sam też włączę się do akcji

typedef union
{
  struct
  {
    u8  act:1;    // channel is active
    u8  chn:1;    // 0 - mono, 1 - stereo
    u8  sgn:1;    // 0 - signed, 1 - unsigned
    u8  bw:1;    // 0 - 8 bit, 1 - 16 bit
    u8  loop:2;    // 0 - off, 1 - forward, 2 - bidi, 3 - reserved
  };
  u8 b;
} CHCTRL;

typedef struct
{
  u8 play:1;    // 0 - stall, 1 - play
  u8 dir:1;     // 0 - forward, 1 - backward
} SSTAT;

typedef struct
{
  u8      *Addr;        // Address of next DAC sample to read
  u8      *EndAddr;     // End Address of Sample
  u8      *LoopAddr;    // Loop Address of Sample
  u32     NewAddr;      // New Start Address (reloaded at the start of WS burst)
  u32     NewEndAddr;   // New End Address
  u32     NewLoopAddr;  // New Loop Address
  w16     SubAddr;      // Current Sub-Address
  u16     SubStep;      // Increment for Sub-Address
  u16     NewSubStep;   // New Increment for Sub-Address
  w16     UserSamp;     // User Sample
  w32     UserOffset;   // User Sample Offset
  w16     UserFreq;     // User Frequency for sample playback (steps are calculated on the flow, actualized at next Tick)
  s8      IntL;         // Interpolation value for Left Audio Channel (previous DAC sample)
  s8      IntR;         // Interpolation value for Right Audio Channel
  u8      SampType;     // Sample Type: bit0 - mono/stereo, bit1 - 8bit/16bit, bit2 - unsigned/signed
  u8      Step;         // Increment for Address
  u8      StepA;        // Increment for Address coerced to Sample parameters
  u8      StepC;        // Next DAC Sample Address coerced to Sample parameters
  u8      NewStep;      // New Increment for Address
  CHCTRL  Control;      // Sample Control: Activeness, Type of Sample Loop, Sample Mode
  CHCTRL  NewControl;   // New Sample Control (taken from Sample Descriptor)
  SSTAT   State;        // Current state of Sample processing
  u8      VolL;         // Sample Volume in Left Audio Channel (0-64)
  u8      VolR;         // Sample Volume in Right Audio Channel (0-64)
  u8      NewVolL;      // New Sample Volume in Left Audio Channel
  u8      NewVolR;      // New Sample Volume in Right Audio Channel
  u8      isUserFreq;   // Indicates that Steps should be updated from User Frequency
  u8      isUserSamp;   // Indicates that Sample should be updated from User Sample
  u8      padding[7];   // ! Always calculate padding value to align this structure by the power of 2 size
} WS_REGS;
