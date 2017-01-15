
/// Prototypes
void print(const char*, ...);
void print_debug(const char*, ...);
void send_uart_byte(u8);
void send_uart(u8*, u8);
void print_char(u8);
void set_color(u8);
void set_x(u8);
void set_xy(u8, u8);
void print_msg(u8);
void process_UART_input(void);
void process_UART_output(void);
void cr();
void tab();

/// Enums
/* UART input modes */
enum UIM
{
  UARTIM_NUL = 0,
  UARTIM_EDIT,    // console editor
  UARTIM_KEY      // console key input
};

/* KB input modes */
enum
{
    KB_OFF = 0,
    KB_EDIT,
    KB_KEY
};

// allow debug messages output
enum DBG_MODE
{
  DBG_OFF,
  DBG_ON
};

/// Types
typedef text::Functions<print_char> CONSOLE;

/// ANSI control codes
#pragma once

#define __BLK    0x01
#define __RED    0x02
#define __GRN    0x03
#define __YLW    0x04
#define __BLU    0x05
#define __MGN    0x06
#define __CYA    0x07
#define __WHT    0x08

#define __TAB    0x09
#define __CR     0x0A

#define __X      0x0B
#define __XY     0x0C
#define __CTAB   0x0D

#define __BBLK   0x11
#define __BRED   0x12
#define __BGRN   0x13
#define __BYLW   0x14
#define __BBLU   0x15
#define __BMGN   0x16
#define __BCYA   0x17
#define __BWHT   0x18

#define _BLK    "\x01"
#define _RED    "\x02"
#define _GRN    "\x03"
#define _YLW    "\x04"
#define _BLU    "\x05"
#define _MGN    "\x06"
#define _CYA    "\x07"
#define _WHT    "\x08"

#define _TAB      "\x09"
#define _CR       "\x0A"

#define _X(x)     "\x0B" _CT(_,x)
#define _XY(x,y)  "\x0C" _CT(_,x) _CT(_,y)
#define _CTAB     "\x0D"

#define _BBLK   "\x11"
#define _BRED   "\x12"
#define _BGRN   "\x13"
#define _BYLW   "\x14"
#define _BBLU   "\x15"
#define _BMGN   "\x16"
#define _BCYA   "\x17"
#define _BWHT   "\x18"

#define _TB       "\eH"

/// ANSI control sequences
#define  _CLS     "\e[2J"
#define  _RES     "\ec"
#define  _F(a)    "\e[" _S(a) "C"

#define _A_CSI    "\e["
#define _A_CTAB   "\e[3g"
#define _BGBLK    "\e[40m"
#define _A_BR_OFF "\e[22m"
#define _A_BR_ON  "\e[1m"
#define _A_COL    "\e[3"
#define _C_OFF    "\e[?25l"
#define _C_ON     "\e[?25h"

#define _0   "\x00"
#define _1   "\x01"
#define _2   "\x02"
#define _3   "\x03"
#define _4   "\x04"
#define _5   "\x05"
#define _6   "\x06"
#define _7   "\x07"
#define _8   "\x08"
#define _9   "\x09"
#define _10  "\x0A"
#define _11  "\x0B"
#define _12  "\x0C"
#define _13  "\x0D"
#define _14  "\x0E"
#define _15  "\x0F"
#define _16  "\x10"
#define _17  "\x11"
#define _18  "\x12"
#define _19  "\x13"
#define _20  "\x14"
#define _21  "\x15"
#define _22  "\x16"
#define _23  "\x17"
#define _24  "\x18"
#define _25  "\x19"
#define _26  "\x1A"
#define _27  "\x1B"
#define _28  "\x1C"
#define _29  "\x1D"
#define _30  "\x1E"
#define _31  "\x1F"
#define _32  "\x20"
#define _33  "\x21"
#define _34  "\x22"
#define _35  "\x23"
#define _36  "\x24"
#define _37  "\x25"
#define _38  "\x26"
#define _39  "\x27"
#define _40  "\x28"
#define _41  "\x29"
#define _42  "\x2A"
#define _43  "\x2B"
#define _44  "\x2C"
#define _45  "\x2D"
#define _46  "\x2E"
#define _47  "\x2F"
#define _48  "\x30"
#define _49  "\x31"
#define _50  "\x32"
#define _51  "\x33"
#define _52  "\x34"
#define _53  "\x35"
#define _54  "\x36"
#define _55  "\x37"
#define _56  "\x38"
#define _57  "\x39"
#define _58  "\x3A"
#define _59  "\x3B"
#define _60  "\x3C"
#define _61  "\x3D"
#define _62  "\x3E"
#define _63  "\x3F"
#define _64  "\x40"
#define _65  "\x41"
#define _66  "\x42"
#define _67  "\x43"
#define _68  "\x44"
#define _69  "\x45"
