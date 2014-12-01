
#pragma once

#include "../device_select.hpp"
#include "../defs.hpp"

#include "../peripheral/gpio.hpp"
#include "peripheral/spi.hpp"

namespace ft800 {

  /* Host commands */
  enum {
    FT_ACTIVE				= 0x00,			// Place FT800 in active state
    FT_STANDBY			= 0x41,			// Place FT800 in Standby (clk running)
    FT_SLEEP				= 0x42,			// Place FT800 in Sleep (clk off)
    FT_PWRDOWN			= 0x50,			// Place FT800 in Power Down (core off)
    FT_CLKEXT				= 0x44,			// Select external clock source
    FT_CLKINT				= 0x48,			// Select internal clock source
    FT_CLK48M				= 0x62,			// Select 48MHz PLL output
    FT_CLK36M				= 0x61,			// Select 36MHz PLL output
    FT_CORERST			= 0x68			// Reset core - all registers default and processors reset
  };

  /* Registers */
  enum {
    REG_ID							    = 0x102400UL,
    REG_FRAMES						  = 0x102404UL,
    REG_CLOCK						    = 0x102408UL,
    REG_FREQUENCY					  = 0x10240CUL,
    REG_SCREENSHOT_EN				= 0x102410UL,
    REG_SCREENSHOT_Y				= 0x102414UL,
    REG_SCREENSHOT_START 		= 0x102418UL,
    REG_CPURESET 					  = 0x10241CUL,
    REG_TAP_CRC 					  = 0x102420UL,
    REG_TAP_MASK 					  = 0x102424UL,
    REG_HCYCLE 						  = 0x102428UL,
    REG_HOFFSET 					  = 0x10242CUL,
    REG_HSIZE 						  = 0x102430UL,
    REG_HSYNC0 						  = 0x102434UL,
    REG_HSYNC1 						  = 0x102438UL,
    REG_VCYCLE 						  = 0x10243CUL,
    REG_VOFFSET 					  = 0x102440UL,
    REG_VSIZE 						  = 0x102444UL,
    REG_VSYNC0 						  = 0x102448UL,
    REG_VSYNC1 						  = 0x10244CUL,
    REG_DLSWAP 						  = 0x102450UL,
    REG_ROTATE 						  = 0x102454UL,
    REG_OUTBITS 					  = 0x102458UL,
    REG_DITHER 						  = 0x10245CUL,
    REG_SWIZZLE 					  = 0x102460UL,
    REG_CSPREAD 					  = 0x102464UL,
    REG_PCLK_POL 					  = 0x102468UL,
    REG_PCLK 						    = 0x10246CUL,
    REG_TAG_X 						  = 0x102470UL,
    REG_TAG_Y 						  = 0x102474UL,
    REG_TAG 						    = 0x102478UL,
    REG_VOL_PB 						  = 0x10247CUL,
    REG_VOL_SOUND 					= 0x102480UL,
    REG_SOUND 						  = 0x102484UL,
    REG_PLAY 						    = 0x102488UL,
    REG_GPIO_DIR 					  = 0x10248CUL,
    REG_GPIO 						    = 0x102490UL,
    REG_INT_FLAGS           = 0x102498UL,
    REG_INT_EN              = 0x10249CUL,
    REG_INT_MASK            = 0x1024A0UL,
    REG_PLAYBACK_START      = 0x1024A4UL,
    REG_PLAYBACK_LENGTH     = 0x1024A8UL,
    REG_PLAYBACK_READPTR    = 0x1024ACUL,
    REG_PLAYBACK_FREQ       = 0x1024B0UL,
    REG_PLAYBACK_FORMAT     = 0x1024B4UL,
    REG_PLAYBACK_LOOP       = 0x1024B8UL,
    REG_PLAYBACK_PLAY       = 0x1024BCUL,
    REG_PWM_HZ              = 0x1024C0UL,
    REG_PWM_DUTY            = 0x1024C4UL,
    REG_MACRO_0             = 0x1024C8UL,
    REG_MACRO_1             = 0x1024CCUL,
    REG_SCREENSHOT_BUSY			= 0x1024D8UL,
    REG_CMD_READ            = 0x1024E4UL,
    REG_CMD_WRITE           = 0x1024E8UL,
    REG_CMD_DL              = 0x1024ECUL,
    REG_TOUCH_MODE          = 0x1024F0UL,
    REG_TOUCH_ADC_MODE      = 0x1024F4UL,
    REG_TOUCH_CHARGE        = 0x1024F8UL,
    REG_TOUCH_SETTLE        = 0x1024FCUL,
    REG_TOUCH_OVERSAMPLE    = 0x102500UL,
    REG_TOUCH_RZTHRESH      = 0x102504UL,
    REG_TOUCH_RAW_XY        = 0x102508UL,
    REG_TOUCH_RZ            = 0x10250CUL,
    REG_TOUCH_SCREEN_XY     = 0x102510UL,
    REG_TOUCH_TAG_XY        = 0x102514UL,
    REG_TOUCH_TAG           = 0x102518UL,
    REG_TOUCH_TRANSFORM_A   = 0x10251CUL,
    REG_TOUCH_TRANSFORM_B   = 0x102520UL,
    REG_TOUCH_TRANSFORM_C   = 0x102524UL,
    REG_TOUCH_TRANSFORM_D   = 0x102528UL,
    REG_TOUCH_TRANSFORM_E   = 0x10252CUL,
    REG_TOUCH_TRANSFORM_F   = 0x102530UL,
    REG_SCREENSHOT_READ			= 0x102554UL,
    REG_TRIM						    = 0x10256CUL,
    REG_TOUCH_DIRECT_XY 		= 0x102574UL,
    REG_TOUCH_DIRECT_Z1Z2 	= 0x102578UL,
    REG_TRACKER						  = 0x109000UL
  };

  /* Chip identifier */
  enum {
    FT800_CHIPID	= 0x00010008UL
  };

  /* Graphics commands constants */
  enum {
    FT_NEVER      = 0,
    FT_LESS       = 1,
    FT_LEQUAL     = 2,
    FT_GREATER    = 3,
    FT_GEQUAL     = 4,
    FT_EQUAL      = 5,
    FT_NOTEQUAL   = 6,
    FT_ALWAYS     = 7
  };

  /* Graphics primitives */
  enum {
    FT_BITMAPS        = 1,
    FT_POINTS         = 2,
    FT_LINES          = 3,
    FT_LINE_STRIP     = 4,
    FT_EDGE_STRIP_R   = 5,
    FT_EDGE_STRIP_L   = 6,
    FT_EDGE_STRIP_A   = 7,
    FT_EDGE_STRIP_B   = 8,
    FT_RECTS          = 9
  };

  /* Bitmap formats */
  enum {
    FT_ARGB1555   = 0,
    FT_L1         = 1,
    FT_L4         = 2,
    FT_L8         = 3,
    FT_RGB332     = 4,
    FT_ARGB2      = 5,
    FT_ARGB4      = 6,
    FT_RGB565     = 7,
    FT_PALETTED   = 8,
    FT_TEXT8X8    = 9,
    FT_TEXTVGA    = 10,
    FT_BARGRAPH   = 11
  };

  /* Bitmap filter types */
  enum {
    FT_NEAREST    = 0,
    FT_BILINEAR   = 1
  };

  /* Bitmap wrap types */
  enum {
    FT_BORDER     = 0,
    FT_REPEAT     = 1
  };

  /* Stencils */
  enum {
    FT_KEEP       = 1,
    FT_REPLACE    = 2,
    FT_INCR       = 3,
    FT_DECR       = 4,
    FT_INVERT     = 5
  };

  /* Alpha blending types */
  enum {
    FT_ZERO                 = 0,
    FT_ONE                  = 1,
    FT_SRC_ALPHA            = 2,
    FT_DST_ALPHA            = 3,
    FT_ONE_MINUS_SRC_ALPHA  = 4,
    FT_ONE_MINUS_DST_ALPHA  = 5
  };

  /* Touch modes */
  enum {
    FT_TMODE_OFF          = 0,
    FT_TMODE_ONESHOT      = 1,
    FT_TMODE_FRAME        = 2,
    FT_TMODE_CONTINUOUS   = 3
  };

  /* Graphics display list swap types */
  enum {
    FT_DLSWAP_DONE    = 0,
    FT_DLSWAP_LINE    = 1,
    FT_DLSWAP_FRAME   = 2
  };

  /* Interrupt bits */
  enum {
    FT_INT_SWAP           = 0x01,
    FT_INT_TOUCH          = 0x02,
    FT_INT_TAG            = 0x04,
    FT_INT_SOUND          = 0x08,
    FT_INT_PLAYBACK       = 0x10,
    FT_INT_CMDEMPTY       = 0x20,
    FT_INT_CMDFLAG        = 0x40,
    FT_INT_CONVCOMPLETE   = 0x80
  };

  /* GPIO bit macros */
  enum {
    FT_GPIO0	  = 0,
    FT_AUD_EN	  = 1, 	// default GPIO for audio shutdown: 1 - enable, 0 - disable
    FT_DISP_EN  = 7 	// default GPIO for display enable: 1 - enable, 0 - disable
  };

  /* Memory definitions */
  enum {
    FT_RAM_G						= 0x000000UL,
    FT_ROM_CHIPID				= 0x0C0000UL,
    FT_ROM_FONT					= 0x0BB23CUL,
    FT_ROM_FONT_ADDR		= 0x0FFFFCUL,
    FT_RAM_DL						= 0x100000UL,
    FT_RAM_PAL					= 0x102000UL,
    FT_RAM_CMD					= 0x108000UL,
    FT_RAM_SCREENSHOT		= 0x1C2000UL
  };

  enum {
/* Memory buffer sizes */
    FT_RAM_G_SIZE				= 256*1024L,
    FT_CMDFIFO_SIZE			= 	4*1024L,
    FT_RAM_DL_SIZE			= 	8*1024L,
    FT_RAM_PAL_SIZE			= 	1*1024L
  };

  /* Coprocessor related commands */
  enum {
    CMD_APPEND           			= 0xFFFFFF1EUL,
    CMD_BGCOLOR          			= 0xFFFFFF09UL,
    CMD_BITMAP_TRANSFORM 			= 0xFFFFFF21UL,
    CMD_BUTTON           			= 0xFFFFFF0DUL,
    CMD_CALIBRATE        			= 0xFFFFFF15UL,
    CMD_CLOCK            			= 0xFFFFFF14UL,
    CMD_COLDSTART        			= 0xFFFFFF32UL,
    CMD_CRC              			= 0xFFFFFF03UL,
    CMD_DIAL             			= 0xFFFFFF2DUL,
    CMD_DLSTART          			= 0xFFFFFF00UL,
    CMD_EXECUTE          			= 0xFFFFFF07UL,
    CMD_FGCOLOR          			= 0xFFFFFF0AUL,
    CMD_GAUGE            			= 0xFFFFFF13UL,
    CMD_GETMATRIX        			= 0xFFFFFF33UL,
    CMD_GETPOINT         			= 0xFFFFFF08UL,
    CMD_GETPROPS         			= 0xFFFFFF25UL,
    CMD_GETPTR           			= 0xFFFFFF23UL,
    CMD_GRADCOLOR        			= 0xFFFFFF34UL,
    CMD_GRADIENT         			= 0xFFFFFF0BUL,
    CMD_HAMMERAUX        			= 0xFFFFFF04UL,
    CMD_IDCT             			= 0xFFFFFF06UL,
    CMD_INFLATE          			= 0xFFFFFF22UL,
    CMD_INTERRUPT        			= 0xFFFFFF02UL,
    CMD_KEYS             			= 0xFFFFFF0EUL,
    CMD_LOADIDENTITY     			= 0xFFFFFF26UL,
    CMD_LOADIMAGE        			= 0xFFFFFF24UL,
    CMD_LOGO             			= 0xFFFFFF31UL,
    CMD_MARCH            			= 0xFFFFFF05UL,
    CMD_MEMCPY           			= 0xFFFFFF1DUL,
    CMD_MEMCRC           			= 0xFFFFFF18UL,
    CMD_MEMSET           			= 0xFFFFFF1BUL,
    CMD_MEMWRITE         			= 0xFFFFFF1AUL,
    CMD_MEMZERO          			= 0xFFFFFF1CUL,
    CMD_NUMBER           			= 0xFFFFFF2EUL,
    CMD_PROGRESS         			= 0xFFFFFF0FUL,
    CMD_REGREAD          			= 0xFFFFFF19UL,
    CMD_ROTATE           			= 0xFFFFFF29UL,
    CMD_SCALE            			= 0xFFFFFF28UL,
    CMD_SCREENSAVER      			= 0xFFFFFF2FUL,
    CMD_SCROLLBAR        			= 0xFFFFFF11UL,
    CMD_SETFONT          			= 0xFFFFFF2BUL,
    CMD_SETMATRIX        			= 0xFFFFFF2AUL,
    CMD_SKETCH           			= 0xFFFFFF30UL,
    CMD_SLIDER           			= 0xFFFFFF10UL,
    CMD_SNAPSHOT         			= 0xFFFFFF1FUL,
    CMD_SPINNER          			= 0xFFFFFF16UL,
    CMD_STOP             			= 0xFFFFFF17UL,
    CMD_SWAP             			= 0xFFFFFF01UL,
    CMD_TEXT             			= 0xFFFFFF0CUL,
    CMD_TOGGLE           			= 0xFFFFFF12UL,
    CMD_TOUCH_TRANSFORM  			= 0xFFFFFF20UL,
    CMD_TRACK            			= 0xFFFFFF2CUL,
    CMD_TRANSLATE        			= 0xFFFFFF27UL
  };

  /* Coprocessor reset */
  enum {
    FT_RESET_HOLD_COPROCESSOR		  = 1,
    FT_RESET_RELEASE_COPROCESSOR	= 0
  };

  /* Widget commands */
  enum {
    FT_OPT_MONO         = 1,
    FT_OPT_NODL         = 2,
    FT_OPT_FLAT         = 256,
    FT_OPT_CENTERX      = 512,
    FT_OPT_CENTERY      = 1024,
    FT_OPT_CENTER       = (FT_OPT_CENTERX | FT_OPT_CENTERY),
    FT_OPT_NOBACK       = 4096,
    FT_OPT_NOTICKS      = 8192,
    FT_OPT_NOHM         = 16384,
    FT_OPT_NOPOINTER    = 16384,
    FT_OPT_NOSECS       = 32768,
    FT_OPT_NOHANDS      = 49152,
    FT_OPT_RIGHTX       = 2048,
    FT_OPT_SIGNED       = 256
  };

  /* Display rotation */
  enum {
    FT_DISPLAY_0	  = 0,	//0 degrees rotation
    FT_DISPLAY_180  = 1	  //180 degrees rotation
  };

  /* Audio sample types */
  enum {
    FT_LINEAR_SAMPLES   = 0,	//8bit signed samples
    FT_ULAW_SAMPLES     = 1,	//8bit ulaw samples
    FT_ADPCM_SAMPLES    = 2	  //4bit ima adpcm samples
  };

  /* Synthesized sound frequencies, midi notes */
  enum {
    FT_MIDI_A0      = 21,
    FT_MIDI_A_0     = 22,
    FT_MIDI_B0      = 23,
    FT_MIDI_C1      = 24,
    FT_MIDI_C_1     = 25,
    FT_MIDI_D1      = 26,
    FT_MIDI_D_1     = 27,
    FT_MIDI_E1      = 28,
    FT_MIDI_F1      = 29,
    FT_MIDI_F_1     = 30,
    FT_MIDI_G1      = 31,
    FT_MIDI_G_1     = 32,
    FT_MIDI_A1      = 33,
    FT_MIDI_A_1     = 34,
    FT_MIDI_B1      = 35,
    FT_MIDI_C2      = 36,
    FT_MIDI_C_2     = 37,
    FT_MIDI_D2      = 38,
    FT_MIDI_D_2     = 39,
    FT_MIDI_E2      = 40,
    FT_MIDI_F2      = 41,
    FT_MIDI_F_2     = 42,
    FT_MIDI_G2      = 43,
    FT_MIDI_G_2     = 44,
    FT_MIDI_A2      = 45,
    FT_MIDI_A_2     = 46,
    FT_MIDI_B2      = 47,
    FT_MIDI_C3      = 48,
    FT_MIDI_C_3     = 49,
    FT_MIDI_D3      = 50,
    FT_MIDI_D_3     = 51,
    FT_MIDI_E3      = 52,
    FT_MIDI_F3      = 53,
    FT_MIDI_F_3     = 54,
    FT_MIDI_G3      = 55,
    FT_MIDI_G_3     = 56,
    FT_MIDI_A3      = 57,
    FT_MIDI_A_3     = 58,
    FT_MIDI_B3      = 59,
    FT_MIDI_C4      = 60,
    FT_MIDI_C_4     = 61,
    FT_MIDI_D4      = 62,
    FT_MIDI_D_4     = 63,
    FT_MIDI_E4      = 64,
    FT_MIDI_F4      = 65,
    FT_MIDI_F_4     = 66,
    FT_MIDI_G4      = 67,
    FT_MIDI_G_4     = 68,
    FT_MIDI_A4      = 69,
    FT_MIDI_A_4     = 70,
    FT_MIDI_B4      = 71,
    FT_MIDI_C5      = 72,
    FT_MIDI_C_5     = 73,
    FT_MIDI_D5      = 74,
    FT_MIDI_D_5     = 75,
    FT_MIDI_E5      = 76,
    FT_MIDI_F5      = 77,
    FT_MIDI_F_5     = 78,
    FT_MIDI_G5      = 79,
    FT_MIDI_G_5     = 80,
    FT_MIDI_A5      = 81,
    FT_MIDI_A_5     = 82,
    FT_MIDI_B5      = 83,
    FT_MIDI_C6      = 84,
    FT_MIDI_C_6     = 85,
    FT_MIDI_D6      = 86,
    FT_MIDI_D_6     = 87,
    FT_MIDI_E6      = 88,
    FT_MIDI_F6      = 89,
    FT_MIDI_F_6     = 90,
    FT_MIDI_G6      = 91,
    FT_MIDI_G_6     = 92,
    FT_MIDI_A6      = 93,
    FT_MIDI_A_6     = 94,
    FT_MIDI_B6      = 95,
    FT_MIDI_C7      = 96,
    FT_MIDI_C_7     = 97,
    FT_MIDI_D7      = 98,
    FT_MIDI_D_7     = 99,
    FT_MIDI_E7      = 100,
    FT_MIDI_F7      = 101,
    FT_MIDI_F_7     = 102,
    FT_MIDI_G7      = 103,
    FT_MIDI_G_7     = 104,
    FT_MIDI_A7      = 105,
    FT_MIDI_A_7     = 106,
    FT_MIDI_B7      = 107,
    FT_MIDI_C8      = 108
  };

  struct REGSET
  {
    u32 addr;
    u32 data;
  };

/* Common graphics mode parameters */
// WQVGA 480x272
  const REGSET wqvga_mode[] = {
    REG_VSYNC0,   0,
    REG_VSYNC1,   10,
    REG_VOFFSET,  12,
    REG_VCYCLE,   292,
    REG_HSYNC0,   0,
    REG_HSYNC1,   41,
    REG_HOFFSET,  43,
    REG_HCYCLE,   548,
    REG_HSIZE,    480,
    REG_VSIZE,    272,
    REG_PCLK_POL, 1,
    REG_SWIZZLE,  0,
    REG_PCLK,     5,
    0, 0
  };

// QVGA 320x240
  const REGSET qvga_mode[] = {
    REG_VSYNC0,   0,
    REG_VSYNC1,   2,
    REG_VOFFSET,  13,
    REG_VCYCLE,   263,
    REG_HSYNC0,   0,
    REG_HSYNC1,   10,
    REG_HOFFSET,  70,
    REG_HCYCLE,   408,
    REG_HSIZE,    320,
    REG_VSIZE,    240,
    REG_PCLK_POL, 0,
    REG_SWIZZLE,  2,
    REG_PCLK,     8,
    0, 0
  };

  template<
      gpio::Address SPI_NSS_PORT,
      u8 SPI_NSS_PIN,
      spi::Address SPI_ADDRESS
  >

  class Functions {
      static_assert(SPI_NSS_PIN < 16, "Invalid GPIO pin.");

      typedef gpio::Pin<SPI_NSS_PORT, SPI_NSS_PIN> SPI_NSS;
      typedef spi::Functions<SPI_ADDRESS> SPI;

    public:
      static void powerUp();
      static void powerDown();
      static void sendCommand(u8);
      static u32 readReg(u32 addr);
      static void writeReg(u32 addr, u32 val);
      static void readMem(u32 addr, u16 num, u8* buf);
      static void writeMem(u32 addr, u16 num, u8* buf);
      static void sendByte(u8);
      static void setRegs(const REGSET[]);
      static void enableDisp();
      static void disableDisp();

    private:
      Functions();
  };
}

/* FT800 graphics engine specific macros for static display list generation */
#define VERTEX2F(x,y) ((1UL<<30)|(((x)&32767UL)<<15)|(((y)&32767UL)<<0))
#define VERTEX2II(x,y,handle,cell) ((2UL<<30)|(((x)&511UL)<<21)|(((y)&511UL)<<12)|(((handle)&31UL)<<7)|(((cell)&127UL)<<0))
#define BITMAP_SOURCE(addr) ((1UL<<24)|(((addr)&1048575UL)<<0))
#define CLEAR_COLOR_RGB(red,green,blue) ((2UL<<24)|(((red)&255UL)<<16)|(((green)&255UL)<<8)|(((blue)&255UL)<<0))
#define TAG(s) ((3UL<<24)|(((s)&255UL)<<0))
#define COLOR_RGB(red,green,blue) ((4UL<<24)|(((red)&255UL)<<16)|(((green)&255UL)<<8)|(((blue)&255UL)<<0))
#define BITMAP_HANDLE(handle) ((5UL<<24)|(((handle)&31UL)<<0))
#define CELL(cell) ((6UL<<24)|(((cell)&127UL)<<0))
#define BITMAP_LAYOUT(format,linestride,height) ((7UL<<24)|(((format)&31UL)<<19)|(((linestride)&1023UL)<<9)|(((height)&511UL)<<0))
#define BITMAP_SIZE(filter,wrapx,wrapy,width,height) ((8UL<<24)|(((filter)&1UL)<<20)|(((wrapx)&1UL)<<19)|(((wrapy)&1UL)<<18)|(((width)&511UL)<<9)|(((height)&511UL)<<0))
#define ALPHA_FUNC(func,ref) ((9UL<<24)|(((func)&7UL)<<8)|(((ref)&255UL)<<0))
#define STENCIL_FUNC(func,ref,mask) ((10UL<<24)|(((func)&7UL)<<16)|(((ref)&255UL)<<8)|(((mask)&255UL)<<0))
#define BLEND_FUNC(src,dst) ((11UL<<24)|(((src)&7UL)<<3)|(((dst)&7UL)<<0))
#define STENCIL_OP(sfail,spass) ((12UL<<24)|(((sfail)&7UL)<<3)|(((spass)&7UL)<<0))
#define POINT_SIZE(size) ((13UL<<24)|(((size)&8191UL)<<0))
#define LINE_WIDTH(width) ((14UL<<24)|(((width)&4095UL)<<0))
#define CLEAR_COLOR_A(alpha) ((15UL<<24)|(((alpha)&255UL)<<0))
#define COLOR_A(alpha) ((16UL<<24)|(((alpha)&255UL)<<0))
#define CLEAR_STENCIL(s) ((17UL<<24)|(((s)&255UL)<<0))
#define CLEAR_TAG(s) ((18UL<<24)|(((s)&255UL)<<0))
#define STENCIL_MASK(mask) ((19UL<<24)|(((mask)&255UL)<<0))
#define TAG_MASK(mask) ((20UL<<24)|(((mask)&1UL)<<0))
#define BITMAP_TRANSFORM_A(a) ((21UL<<24)|(((a)&131071UL)<<0))
#define BITMAP_TRANSFORM_B(b) ((22UL<<24)|(((b)&131071UL)<<0))
#define BITMAP_TRANSFORM_C(c) ((23UL<<24)|(((c)&16777215UL)<<0))
#define BITMAP_TRANSFORM_D(d) ((24UL<<24)|(((d)&131071UL)<<0))
#define BITMAP_TRANSFORM_E(e) ((25UL<<24)|(((e)&131071UL)<<0))
#define BITMAP_TRANSFORM_F(f) ((26UL<<24)|(((f)&16777215UL)<<0))
#define SCISSOR_XY(x,y) ((27UL<<24)|(((x)&511UL)<<9)|(((y)&511UL)<<0))
#define SCISSOR_SIZE(width,height) ((28UL<<24)|(((width)&1023UL)<<10)|(((height)&1023UL)<<0))
#define CALL(dest) ((29UL<<24)|(((dest)&65535UL)<<0))
#define JUMP(dest) ((30UL<<24)|(((dest)&65535UL)<<0))
#define BEGIN(prim) ((31UL<<24)|(((prim)&15UL)<<0))
#define COLOR_MASK(r,g,b,a) ((32UL<<24)|(((r)&1UL)<<3)|(((g)&1UL)<<2)|(((b)&1UL)<<1)|(((a)&1UL)<<0))
#define CLEAR(c,s,t) ((38UL<<24)|(((c)&1UL)<<2)|(((s)&1UL)<<1)|(((t)&1UL)<<0))
#define END() ((33UL<<24))
#define SAVE_CONTEXT() ((34UL<<24))
#define RESTORE_CONTEXT() ((35UL<<24))
#define RETURN() ((36UL<<24))
#define MACRO(m) ((37UL<<24)|(((m)&1UL)<<0))
#define DISPLAY() ((0UL<<24))

#include "../../bits/ft800.tcc"
