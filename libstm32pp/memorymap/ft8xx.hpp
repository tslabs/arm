
#pragma once

#include "common.hpp"

namespace ft8xx
{
  /* Signatures */
  namespace id
  {
    enum
    {
#ifdef USE_FT800
      // +++
#elif defined(USE_FT81x)
      CHIP           = 0x7C,
#endif
    };
  }

  /* Memory addresses */
  namespace addr
  {
    enum
    {
#ifdef USE_FT800
      RAM_G           = 0x000000,
      ROM_CHIPID      = 0x0C0000,
      ROM_FONT        = 0x0BB23C,
      ROM_FONT_ADDR   = 0x0FFFFC,
      RAM_DL          = 0x100000,
      RAM_PAL         = 0x102000,
      RAM_CMD         = 0x108000,
      RAM_SCREENSHOT  = 0x1C2000,
#elif defined(USE_FT81x)
      RAM_G           = 0x000000,   // Main graphics RAM
      ROM_CHIPID      = 0x0C0000,   // Chip ID and revision
      ROM_FONT        = 0x1E0000,   // Fonts
      ROM_FONT_ADDR   = 0x2FFFFC,   // Font table pointer address
      RAM_DL          = 0x300000,   // Display list RAM
      RAM_REG         = 0x302000,   // Registers
      RAM_CMD         = 0x308000,   // Coprocessor command buffer
#endif
    };
  }

  /* Memory sizes */
  namespace memsz
  {
    enum
    {
#ifdef USE_FT800
      RAM_G        = 0x40000,
      RAM_DL       = 8192,          // Display List
      CMD_FIFO     = 4096,          // Coprocessor FIFO
      RAM_PAL      = 1024,
#elif defined(USE_FT81x)
      RAM_G        = 0x100000,
      RAM_DL       = 8192,
      CMD_FIFO     = 4096,
#endif
    };
  }

  /* Host commands */
  namespace cmd
  {
    enum
    {
#ifdef USE_FT800
      ACTIVE      = 0x00,  // Active state
      STANDBY     = 0x41,  // Standby (clk running)
      SLEEP       = 0x42,  // Sleep (clk off)
      CLKEXT      = 0x44,  // Select external clock source
      CLKINT      = 0x48,  // Select internal clock source
      PWRDOWN     = 0x50,  // Power Down (core off)
      CLK36M      = 0x61,  // Select 36MHz PLL output
      CLK48M      = 0x62,  // Select 48MHz PLL output
      CORERST     = 0x68,  // Reset core - all registers default and processors reset
#elif defined(USE_FT81x)
      ACTIVE      = 0x00,  // cc 00 00
      STANDBY     = 0x41,  // cc 00 00
      SLEEP       = 0x42,  // cc 00 00
      PWRDOWN     = 0x43,  // cc 00 00
      CLKEXT      = 0x44,  // cc 00 00
      CLKINT      = 0x48,  // cc 00 00
      PDROMS      = 0x49,  // cc xx 00
      CLKSEL      = 0x61,  // cc xx 00 -> [5:0] - mul, [7:6] - PLL range (0 for mul=0..3, 1 for mul=4..5)
      RST_PULSE   = 0x68,  // cc 00 00
#endif
    };
  }

  /* Registers */
  namespace reg
  {
    enum Regs
    {
#ifdef USE_FT800
      ID                  = 0x102400,
      FRAMES              = 0x102404,
      CLOCK               = 0x102408,
      FREQUENCY           = 0x10240C,
      SCREENSHOT_EN       = 0x102410,
      SCREENSHOT_Y        = 0x102414,
      SCREENSHOT_START    = 0x102418,
      CPURESET            = 0x10241C,
      TAP_CRC             = 0x102420,
      TAP_MASK            = 0x102424,
      HCYCLE              = 0x102428,
      HOFFSET             = 0x10242C,
      HSIZE               = 0x102430,
      HSYNC0              = 0x102434,
      HSYNC1              = 0x102438,
      VCYCLE              = 0x10243C,
      VOFFSET             = 0x102440,
      VSIZE               = 0x102444,
      VSYNC0              = 0x102448,
      VSYNC1              = 0x10244C,
      DLSWAP              = 0x102450,
      ROTATE              = 0x102454,
      OUTBITS             = 0x102458,
      DITHER              = 0x10245C,
      SWIZZLE             = 0x102460,
      CSPREAD             = 0x102464,
      PCLK_POL            = 0x102468,
      PCLK                = 0x10246C,
      TAG_X               = 0x102470,
      TAG_Y               = 0x102474,
      TAG                 = 0x102478,
      VOL_PB              = 0x10247C,
      VOL_SOUND           = 0x102480,
      SOUND               = 0x102484,
      PLAY                = 0x102488,
      GPIO_DIR            = 0x10248C,
      GPIO                = 0x102490,
      INT_FLAGS           = 0x102498,
      INT_EN              = 0x10249C,
      INT_MASK            = 0x1024A0,
      PLAYBACK_START      = 0x1024A4,
      PLAYBACK_LENGTH     = 0x1024A8,
      PLAYBACK_READPTR    = 0x1024AC,
      PLAYBACK_FREQ       = 0x1024B0,
      PLAYBACK_FORMAT     = 0x1024B4,
      PLAYBACK_LOOP       = 0x1024B8,
      PLAYBACK_PLAY       = 0x1024BC,
      PWM_HZ              = 0x1024C0,
      PWM_DUTY            = 0x1024C4,
      MACRO_0             = 0x1024C8,
      MACRO_1             = 0x1024CC,
      SCREENSHOT_BUSY     = 0x1024D8,
      CMD_READ            = 0x1024E4,
      CMD_WRITE           = 0x1024E8,
      CMD_DL              = 0x1024EC,
      TOUCH_MODE          = 0x1024F0,
      TOUCH_ADC_MODE      = 0x1024F4,
      TOUCH_CHARGE        = 0x1024F8,
      TOUCH_SETTLE        = 0x1024FC,
      TOUCH_OVERSAMPLE    = 0x102500,
      TOUCH_RZTHRESH      = 0x102504,
      TOUCH_RAW_XY        = 0x102508,
      TOUCH_RZ            = 0x10250C,
      TOUCH_SCREEN_XY     = 0x102510,
      TOUCH_TAG_XY        = 0x102514,
      TOUCH_TAG           = 0x102518,
      TOUCH_TRANSFORM_A   = 0x10251C,
      TOUCH_TRANSFORM_B   = 0x102520,
      TOUCH_TRANSFORM_C   = 0x102524,
      TOUCH_TRANSFORM_D   = 0x102528,
      TOUCH_TRANSFORM_E   = 0x10252C,
      TOUCH_TRANSFORM_F   = 0x102530,
      SCREENSHOT_READ     = 0x102554,
      TRIM                = 0x10256C,
      TOUCH_DIRECT_XY     = 0x102574,
      TOUCH_DIRECT_Z1Z2   = 0x102578,
      TRACKER             = 0x109000,
#elif defined(USE_FT81x)
      ID                  = 0x302000,
      FRAMES              = 0x302004,
      CLOCK               = 0x302008,
      FREQUENCY           = 0x30200C,
      RENDERMODE          = 0x302010,
      SNAPY               = 0x302014,
      SNAPSHOT            = 0x302018,
      SNAPFORMAT          = 0x30201C,
      CPURESET            = 0x302020,
      TAP_CRC             = 0x302024,
      TAP_MASK            = 0x302028,
      HCYCLE              = 0x30202C,
      HOFFSET             = 0x302030,
      HSIZE               = 0x302034,
      HSYNC0              = 0x302038,
      HSYNC1              = 0x30203C,
      VCYCLE              = 0x302040,
      VOFFSET             = 0x302044,
      VSIZE               = 0x302048,
      VSYNC0              = 0x30204C,
      VSYNC1              = 0x302050,
      DLSWAP              = 0x302054,
      ROTATE              = 0x302058,
      OUTBITS             = 0x30205C,
      DITHER              = 0x302060,
      SWIZZLE             = 0x302064,
      CSPREAD             = 0x302068,
      PCLK_POL            = 0x30206C,
      PCLK                = 0x302070,
      TAG_X               = 0x302074,
      TAG_Y               = 0x302078,
      TAG                 = 0x30207C,
      VOL_PB              = 0x302080,
      VOL_SOUND           = 0x302084,
      SOUND               = 0x302088,
      PLAY                = 0x30208C,
      GPIO_DIR            = 0x302090,
      GPIO                = 0x302094,
      GPIOX_DIR           = 0x302098,
      GPIOX               = 0x30209C,
      INT_FLAGS           = 0x3020A8,
      INT_EN              = 0x3020AC,
      INT_MASK            = 0x3020B0,
      PLAYBACK_START      = 0x3020B4,
      PLAYBACK_LENGTH     = 0x3020B8,
      PLAYBACK_READPTR    = 0x3020BC,
      PLAYBACK_FREQ       = 0x3020C0,
      PLAYBACK_FORMAT     = 0x3020C4,
      PLAYBACK_LOOP       = 0x3020C8,
      PLAYBACK_PLAY       = 0x3020CC,
      PWM_HZ              = 0x3020D0,
      PWM_DUTY            = 0x3020D4,
      MACRO_0             = 0x3020D8,
      MACRO_1             = 0x3020DC,
      CMD_READ            = 0x3020F8,
      CMD_WRITE           = 0x3020FC,
      CMD_DL              = 0x302100,
      TOUCH_MODE          = 0x302104,
      TOUCH_ADC_MODE      = 0x302108,
      TOUCH_CHARGE        = 0x30210C,
      TOUCH_SETTLE        = 0x302110,
      TOUCH_OVERSAMPLE    = 0x302114,
      TOUCH_RZTHRESH      = 0x302118,
      TOUCH_RAW_XY        = 0x30211C,
      TOUCH_RZ            = 0x302120,
      TOUCH_SCREEN_XY     = 0x302124,
      TOUCH_TAG_XY        = 0x302128,
      TOUCH_TAG           = 0x30212C,
      TOUCH_TAG1_XY       = 0x302130,
      TOUCH_TAG1          = 0x302134,
      TOUCH_TAG2_XY       = 0x302138,
      TOUCH_TAG2          = 0x30213C,
      TOUCH_TAG3_XY       = 0x302140,
      TOUCH_TAG3          = 0x302144,
      TOUCH_TAG4_XY       = 0x302148,
      TOUCH_TAG4          = 0x30214C,
      TOUCH_TRANSFORM_A   = 0x302150,
      TOUCH_TRANSFORM_B   = 0x302154,
      TOUCH_TRANSFORM_C   = 0x302158,
      TOUCH_TRANSFORM_D   = 0x30215C,
      TOUCH_TRANSFORM_E   = 0x302160,
      TOUCH_TRANSFORM_F   = 0x302164,
      TOUCH_CONFIG        = 0x302168,
      CTOUCH_TOUCH4_X     = 0x30216C,
      BIST_EN             = 0x302174,
      TRIM                = 0x302180,
      ANA_COMP            = 0x302184,
      SPI_WIDTH           = 0x302188,
      TOUCH_DIRECT_XY     = 0x30218C,
      TOUCH_DIRECT_Z1Z2   = 0x302190,
      DATESTAMP           = 0x302564,
      CMDB_SPACE          = 0x302574,
      CMDB_WRITE          = 0x302578,
      TRACKER             = 0x309000,
      TRACKER_1           = 0x309004,
      TRACKER_2           = 0x309008,
      TRACKER_3           = 0x30900C,
      TRACKER_4           = 0x309010,
      MEDIAFIFO_READ      = 0x309014,
      MEDIAFIFO_WRITE     = 0x309018,
#endif
    };
  }

  /* Chip identifier */
  namespace id
  {
    enum
    {
      FT800  = 0x00010008,
    };
  }

  /* Graphic primitives */
  namespace prim
  {
    enum
    {
      BITMAPS        = 1,
      POINTS         = 2,
      LINES          = 3,
      LINE_STRIP     = 4,
      EDGE_STRIP_R   = 5,
      EDGE_STRIP_L   = 6,
      EDGE_STRIP_A   = 7,
      EDGE_STRIP_B   = 8,
      RECTS          = 9,
    };
  }

  /* Bitmap formats */
  namespace bmfmt
  {
    enum
    {
#ifdef USE_FT800
      ARGB1555     = 0,
      L1           = 1,
      L4           = 2,
      L8           = 3,
      RGB332       = 4,
      ARGB2        = 5,
      ARGB4        = 6,
      RGB565       = 7,
      PALETTED     = 8,
      TEXT8X8      = 9,
      TEXTVGA      = 10,
      BARGRAPH     = 11,
#elif defined(USE_FT81x)
      ARGB1555     = 0,
      L1           = 1,
      L4           = 2,
      L8           = 3,
      RGB332       = 4,
      ARGB2        = 5,
      ARGB4        = 6,
      RGB565       = 7,
      TEXT8X8      = 9,
      TEXTVGA      = 10,
      BARGRAPH     = 11,
      PALETTED565  = 14,
      PALETTED4444 = 15,
      PALETTED8    = 16,
      L2           = 17,
#endif
    };
  }

  /* Bitmap filter types */
  namespace bmflt
  {
    enum
    {
      NEAREST    = 0,
      BILINEAR   = 1
    };
  }

  /* Bitmap wrap types */
  namespace bmwrap
  {
    enum
    {
      BORDER     = 0,
      REPEAT     = 1
    };
  }

  /* Stencil test functions */
  namespace stfunc
  {
    enum
    {
      NEVER      = 0,
      LESS       = 1,
      LEQUAL     = 2,
      GREATER    = 3,
      GEQUAL     = 4,
      EQUAL      = 5,
      NOTEQUAL   = 6,
      ALWAYS     = 7,
    };
  }

  /* Stencil operations */
  namespace stop
  {
    enum
    {
      KEEP       = 1,
      REPLACE    = 2,
      INCR       = 3,
      DECR       = 4,
      INVERT     = 5,
#if defined(USE_FT81x)
      INCR_WRAP  = 6,  // undocumented???
      DECR_WRAP  = 7,  // undocumented???
#endif
    };
  }

  /* Alpha blending functions */
  namespace afunc
  {
    enum
    {
      ZERO                 = 0,
      ONE                  = 1,
      SRC_ALPHA            = 2,
      DST_ALPHA            = 3,
      ONE_MINUS_SRC_ALPHA  = 4,
      ONE_MINUS_DST_ALPHA  = 5,
    };
  }

  /* Graphics display list swap types */
  namespace dlswap
  {
    enum
    {
      DONE    = 0,
      LINE    = 1,
      FRAME   = 2,
    };
  }

  /* Touch modes */
  namespace touch
  {
    enum
    {
      OFF          = 0,
      ONESHOT      = 1,
      FRAME        = 2,
      CONTINUOUS   = 3,
    };
  }

  /* Interrupt bits */
  namespace interr
  {
    enum
    {
      SWAP           = 0x01,
      TOUCH          = 0x02,
      TAG            = 0x04,
      SOUND          = 0x08,
      PLAYBACK       = 0x10,
      CMDEMPTY       = 0x20,
      CMDFLAG        = 0x40,
      CONVCOMPLETE   = 0x80,
    };
  }

  /* GPIO bits */
  namespace gpios
  {
    enum
    {
      GPIO0    = 0,
      AUD_EN   = 1,  // default GPIO for audio shutdown: 1 - enable, 0 - disable
      DISP_EN  = 7   // default GPIO for display enable: 1 - enable, 0 - disable
    };
  }

  /* Coprocessor commands */
  namespace ccmd
  {
    enum
    {
#ifdef USE_FT800
      APPEND            = 0xFFFFFF1E,
      BGCOLOR           = 0xFFFFFF09,
      BITMAP_TRANSFORM  = 0xFFFFFF21,
      BUTTON            = 0xFFFFFF0D,
      CALIBRATE         = 0xFFFFFF15,
      CLOCK             = 0xFFFFFF14,
      COLDSTART         = 0xFFFFFF32,
      CRC               = 0xFFFFFF03,
      DIAL              = 0xFFFFFF2D,
      DLSTART           = 0xFFFFFF00,
      EXECUTE           = 0xFFFFFF07,
      FGCOLOR           = 0xFFFFFF0A,
      GAUGE             = 0xFFFFFF13,
      GETMATRIX         = 0xFFFFFF33,
      GETPOINT          = 0xFFFFFF08,
      GETPROPS          = 0xFFFFFF25,
      GETPTR            = 0xFFFFFF23,
      GRADCOLOR         = 0xFFFFFF34,
      GRADIENT          = 0xFFFFFF0B,
      HAMMERAUX         = 0xFFFFFF04,
      IDCT              = 0xFFFFFF06,
      INFLATE           = 0xFFFFFF22,
      INTERRUPT         = 0xFFFFFF02,
      KEYS              = 0xFFFFFF0E,
      LOADIDENTITY      = 0xFFFFFF26,
      LOADIMAGE         = 0xFFFFFF24,
      LOGO              = 0xFFFFFF31,
      MARCH             = 0xFFFFFF05,
      MEMCPY            = 0xFFFFFF1D,
      MEMCRC            = 0xFFFFFF18,
      MEMSET            = 0xFFFFFF1B,
      MEMWRITE          = 0xFFFFFF1A,
      MEMZERO           = 0xFFFFFF1C,
      NUMBER            = 0xFFFFFF2E,
      PROGRESS          = 0xFFFFFF0F,
      REGREAD           = 0xFFFFFF19,
      ROTATE            = 0xFFFFFF29,
      SCALE             = 0xFFFFFF28,
      SCREENSAVER       = 0xFFFFFF2F,
      SCROLLBAR         = 0xFFFFFF11,
      SETFONT           = 0xFFFFFF2B,
      SETMATRIX         = 0xFFFFFF2A,
      SKETCH            = 0xFFFFFF30,
      SLIDER            = 0xFFFFFF10,
      SNAPSHOT          = 0xFFFFFF1F,
      SPINNER           = 0xFFFFFF16,
      STOP              = 0xFFFFFF17,
      SWAP              = 0xFFFFFF01,
      TEXT              = 0xFFFFFF0C,
      TOGGLE            = 0xFFFFFF12,
      TOUCH_TRANSFORM   = 0xFFFFFF20,
      TRACK             = 0xFFFFFF2C,
      TRANSLATE         = 0xFFFFFF27,
#elif defined(USE_FT81x)
      APPEND            = 0xFFFFFF1E,
      BGCOLOR           = 0xFFFFFF09,
      BITMAP_TRANSFORM  = 0xFFFFFF21,
      BUTTON            = 0xFFFFFF0D,
      CALIBRATE         = 0xFFFFFF15,
      CLOCK             = 0xFFFFFF14,
      COLDSTART         = 0xFFFFFF32,
      CRC               = 0xFFFFFF03,
      CSKETCH 		      = 0xFFFFFF35,
      DIAL              = 0xFFFFFF2D,
      DLSTART           = 0xFFFFFF00,
      EXECUTE           = 0xFFFFFF07,
      FGCOLOR           = 0xFFFFFF0A,
      GAUGE             = 0xFFFFFF13,
      GETMATRIX         = 0xFFFFFF33,
      GETPOINT          = 0xFFFFFF08,
      GETPROPS          = 0xFFFFFF25,
      GETPTR            = 0xFFFFFF23,
      GRADCOLOR         = 0xFFFFFF34,
      GRADIENT          = 0xFFFFFF0B,
      HAMMERAUX         = 0xFFFFFF04,
      IDCT_DELETED      = 0xFFFFFF06,
      INFLATE           = 0xFFFFFF22,
      INTERRUPT         = 0xFFFFFF02,
      INT_RAMSHARED     = 0xFFFFFF3D,
      INT_SWLOADIMAGE   = 0xFFFFFF3E,
      KEYS              = 0xFFFFFF0E,
      LOADIDENTITY      = 0xFFFFFF26,
      LOADIMAGE         = 0xFFFFFF24,
      LOGO              = 0xFFFFFF31,
      MARCH             = 0xFFFFFF05,
      MEDIAFIFO         = 0xFFFFFF39,
      MEMCPY            = 0xFFFFFF1D,
      MEMCRC            = 0xFFFFFF18,
      MEMSET            = 0xFFFFFF1B,
      MEMWRITE          = 0xFFFFFF1A,
      MEMZERO           = 0xFFFFFF1C,
      NUMBER            = 0xFFFFFF2E,
      PLAYVIDEO         = 0xFFFFFF3A,
      PROGRESS          = 0xFFFFFF0F,
      REGREAD           = 0xFFFFFF19,
      ROMFONT           = 0xFFFFFF3F,
      ROTATE            = 0xFFFFFF29,
      SCALE             = 0xFFFFFF28,
      SCREENSAVER       = 0xFFFFFF2F,
      SCROLLBAR         = 0xFFFFFF11,
      SETBASE           = 0xFFFFFF38,
      SETBITMAP         = 0xFFFFFF43,
      SETFONT           = 0xFFFFFF2B,
      SETFONT2          = 0xFFFFFF3B,
      SETMATRIX         = 0xFFFFFF2A,
      SETROTATE         = 0xFFFFFF36,
      SETSCRATCH        = 0xFFFFFF3C,
      SKETCH            = 0xFFFFFF30,
      SLIDER            = 0xFFFFFF10,
      SNAPSHOT          = 0xFFFFFF1F,
      SNAPSHOT2         = 0xFFFFFF37,
      SPINNER           = 0xFFFFFF16,
      STOP              = 0xFFFFFF17,
      SWAP              = 0xFFFFFF01,
      SYNC              = 0xFFFFFF42,
      TEXT              = 0xFFFFFF0C,
      TOGGLE            = 0xFFFFFF12,
      TOUCH_TRANSFORM   = 0xFFFFFF20,
      TRACK             = 0xFFFFFF2C,
      TRANSLATE         = 0xFFFFFF27,
      VIDEOFRAME        = 0xFFFFFF41,
      VIDEOSTART        = 0xFFFFFF40,
#endif
    };
  }

  /* Coprocessor reset */
  namespace cpres
  {
    enum
    {
      RESET_HOLD_COPROCESSOR     = 1,
      RESET_RELEASE_COPROCESSOR  = 0,
    };
  }

  /* Widget commands */
  namespace wdg
  {
    enum
    {
      MONO         = 1,
      NODL         = 2,
      FLAT         = 256,
      CENTERX      = 512,
      CENTERY      = 1024,
      CENTER       = CENTERX + CENTERY,
      NOBACK       = 4096,
      NOTICKS      = 8192,
      NOHM         = 16384,
      NOPOINTER    = 16384,
      NOSECS       = 32768,
      NOHANDS      = 49152,
      RIGHTX       = 2048,
      SIGNED       = 256,
    };
  }

  /* Display rotation */
  namespace drot
  {
    enum
    {
      DISPLAY_0    = 0,  //0 degrees rotation
      DISPLAY_180  = 1    //180 degrees rotation
    };
  }

  /* Audio sample types */
  namespace ausmp
  {
    enum
    {
      LINEAR_SAMPLES   = 0,  //8bit signed samples
      ULAW_SAMPLES     = 1,  //8bit ulaw samples
      ADPCM_SAMPLES    = 2    //4bit ima adpcm samples
    };
  }

  /* Synthesized sounds */
  namespace snd
  {
    enum
    {
      SILENCE      = 0x00,
      SQUAREWAVE   = 0x01,
      SINEWAVE     = 0x02,
      SAWTOOTH     = 0x03,
      TRIANGLE     = 0x04,
      BEEPING      = 0x05,
      ALARM        = 0x06,
      WARBLE       = 0x07,
      CAROUSEL     = 0x08,
      PIPS         = 0x0F,
      HARP         = 0x40,
      XYLOPHONE    = 0x41,
      TUBA         = 0x42,
      GLOCKENSPIEL = 0x43,
      ORGAN        = 0x44,
      TRUMPET      = 0x45,
      PIANO        = 0x46,
      CHIMES       = 0x47,
      MUSICBOX     = 0x48,
      BELL         = 0x49,
      CLICK        = 0x50,
      SWITCH       = 0x51,
      COWBELL      = 0x52,
      NOTCH        = 0x53,
      HIHAT        = 0x54,
      KICKDRUM     = 0x55,
      POP          = 0x56,
      CLACK        = 0x57,
      CHACK        = 0x58,
      MUTE         = 0x60,
      UNMUTE       = 0x61
    };
  }

  /* Synthesized sound frequencies, midi notes */
  namespace note
  {
    enum
    {
      A0      = 21,
      A_0     = 22,
      B0      = 23,
      C1      = 24,
      C_1     = 25,
      D1      = 26,
      D_1     = 27,
      E1      = 28,
      F1      = 29,
      F_1     = 30,
      G1      = 31,
      G_1     = 32,
      A1      = 33,
      A_1     = 34,
      B1      = 35,
      C2      = 36,
      C_2     = 37,
      D2      = 38,
      D_2     = 39,
      E2      = 40,
      F2      = 41,
      F_2     = 42,
      G2      = 43,
      G_2     = 44,
      A2      = 45,
      A_2     = 46,
      B2      = 47,
      C3      = 48,
      C_3     = 49,
      D3      = 50,
      D_3     = 51,
      E3      = 52,
      F3      = 53,
      F_3     = 54,
      G3      = 55,
      G_3     = 56,
      A3      = 57,
      A_3     = 58,
      B3      = 59,
      C4      = 60,
      C_4     = 61,
      D4      = 62,
      D_4     = 63,
      E4      = 64,
      F4      = 65,
      F_4     = 66,
      G4      = 67,
      G_4     = 68,
      A4      = 69,
      A_4     = 70,
      B4      = 71,
      C5      = 72,
      C_5     = 73,
      D5      = 74,
      D_5     = 75,
      E5      = 76,
      F5      = 77,
      F_5     = 78,
      G5      = 79,
      G_5     = 80,
      A5      = 81,
      A_5     = 82,
      B5      = 83,
      C6      = 84,
      C_6     = 85,
      D6      = 86,
      D_6     = 87,
      E6      = 88,
      F6      = 89,
      F_6     = 90,
      G6      = 91,
      G_6     = 92,
      A6      = 93,
      A_6     = 94,
      B6      = 95,
      C7      = 96,
      C_7     = 97,
      D7      = 98,
      D_7     = 99,
      E7      = 100,
      F7      = 101,
      F_7     = 102,
      G7      = 103,
      G_7     = 104,
      A7      = 105,
      A_7     = 106,
      B7      = 107,
      C8      = 108,
    };
  }
} // namespace ft8xx
