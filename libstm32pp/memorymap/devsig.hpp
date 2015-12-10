
#pragma once

#include "common.hpp"

namespace devsig
{
  enum
  {
    ADDRESS = alias::DSIG
  };

  struct Registers
  {
    u16 F_SIZE;       // 0x00: Flash Size
    u8 dummy[6];
    u8 U_ID[12];      // Unique ID
  };
}
