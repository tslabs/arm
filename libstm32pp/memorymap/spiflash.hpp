
#pragma once

#include "common.hpp"

namespace spiflash
{
  namespace cmd
  {
    enum
    {
      // Instruction format: <address bytes>:<dummy bytes>:<data bytes>
      // Table 1: Erase, Program
      READ_STAT   = 0x05,    // 0:0:1
      READ_STAT2  = 0x35,    // 0:0:1 (for W25Qxx)
      WRITE_EN    = 0x06,    // 0:0:0
      WRITE_DIS   = 0x04,    // 0:0:0
      WRITE_STAT  = 0x01,    // 0:0:1 (0:0:2 for W25Qxx)
      WRITE       = 0x02,    // 3:0:1..256
      ERASE_SEC   = 0x20,    // 3:0:0 (for W25Qxx)
      ERASE_BLK32 = 0x52,    // 3:0:0 (for W25Qxx)
      ERASE_BLK64 = 0xD8,    // 3:0:0
      ERASE_CHIP  = 0xC7,    // 0:0:0

      // Table 2: Read
      READ        = 0x03,    // 3:0:1+
      FAST_READ   = 0x0B,    // 3:1:1+

      // Table 3: ID, Security
      DEV_ID      = 0xAB,    // 0:3:1 (works for most SPI flash devices)
      MAN_DEV_ID  = 0x90,    // 0:3:2
      JEDEC_ID    = 0x9F,    // 0:0:3
      UNIQUE_ID   = 0x4B     // 0:4:8
    };
  };

  namespace status
  {
    enum
    {
      BUSY = 0x01,
      WEL  = 0x02,
      BP0  = 0x04,
      BP1  = 0x08,
      BP2  = 0x10,
      TB   = 0x20,
      SEC  = 0x40,
      SRP0 = 0x80
    };
  };

  namespace status2   // for W25Qxx
  {
    enum
    {
      SRP1 = 0x01,
      QE   = 0x02,
      LB1  = 0x08,
      LB2  = 0x10,
      LB3  = 0x20,
      CMP  = 0x40,
      SUS  = 0x80
    };
  };

  namespace id
  {
    enum
    {
      KBIT_512 = 0x05,
      MBIT_1   = 0x10,
      MBIT_2   = 0x11,
      MBIT_4   = 0x12,
      MBIT_8   = 0x13,
      MBIT_16  = 0x14,
      MBIT_32  = 0x15,
      MBIT_64  = 0x16,
      MBIT_128 = 0x17,
      MBIT_256 = 0x18
    };
  };

  namespace jedec
  {
    namespace capacity
    {
      enum
      {
        KBIT_512 = 0x10,
        MBIT_1   = 0x11,
        MBIT_2   = 0x12,
        MBIT_4   = 0x13,
        MBIT_8   = 0x14,
        MBIT_16  = 0x15,
        MBIT_32  = 0x16,
        MBIT_64  = 0x17,
        MBIT_128 = 0x18,
        MBIT_256 = 0x19,
      };
    };

    namespace type
    {
      enum
      {
        W25X     = 0x30,
        W25Q     = 0x40,
        W25QW    = 0x60
      };
    };

    namespace manufacturer
    {
      enum
      {
        WINBOND = 0xEF
      };
    };
  };
};
