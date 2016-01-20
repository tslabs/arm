
#pragma once

#include "common.hpp"

namespace devsig
{
  enum
  {
#ifdef STM32F1XX
    UID   = 0x1FFFF7E8,
    FSIZE = 0x1FFFF7E0
#else // STM32F2XX, STM32F4XX
    UID   = 0x1FFF7A10,
    FSIZE = 0x1FFF7A22
#endif
  };
}
