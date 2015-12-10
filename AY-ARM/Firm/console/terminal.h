
#pragma once

#define __BLACK   0x01
#define __RED     0x02
#define __GREEN   0x03
#define __YELLOW  0x04
#define __BLUE    0x05
#define __MAGENTA 0x06
#define __CYAN    0x07
#define __WHITE   0x08

#define __CR      0x0A
#define __BR_OFF  0x0B
#define __BR_ON   0x0C

#define __CTAB    0x0D
#define __X       0x0E
#define __XY      0x0F
#define __NTS     0x10
#define __FLS     0x11
#define __NTSF    0x12
#define __FLSF    0x13
#define __DEC16   0x14
#define __HEX8    0x15

#define _BLACK    "\x01"
#define _RED      "\x02"
#define _GREEN    "\x03"
#define _YELLOW   "\x04"
#define _BLUE     "\x05"
#define _MAGENTA  "\x06"
#define _CYAN     "\x07"
#define _WHITE    "\x08"

#define _TAB      "\x09"
#define _CR       "\x0A"

#define _BR_OFF   "\x0B"
#define _BR_ON    "\x0C"
#define _CTAB     "\x0D"
#define _X(x)     "\x0E" _CT(_,x)
#define _XY(x,y)  "\x0F" _CT(_,x) _CT(_,y)
#define _NTS      "\x10"
#define _FLS      "\x11"
#define _NTSF     "\x12"
#define _FLSF     "\x13"
#define _DEC16    "\x14"
#define _HEX8     "\x15"

#define _TB       "\eH"

/* ANSI control sequences */
#define  _CLS     "\e[2J"
#define  _RES     "\ec"
#define  _F(a)    "\e[" _S(a) "C"

#define _A_CSI    "\e["
#define _A_CTAB   "\e[3g"
#define _BBLACK   "\e[40m"
#define _A_BR_ON  "\e[22m"
#define _A_BR_OFF "\e[1m"
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
