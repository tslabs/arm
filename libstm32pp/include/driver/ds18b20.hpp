
#pragma once

#include "1wire.hpp"

namespace ds18b20
{
  // DS18B20 commands
  enum
  {
    READ_ROM   = 0x33,
    MATCH_ROM  = 0x55,
    SKIP_ROM   = 0xCC,
    SEARCH_ROM = 0xF0,
    CONVERT_T  = 0x44,
    READ       = 0xBE,
    WRITE      = 0x4E,
    COPY       = 0x48,
    RECALL_EE  = 0xB8,
    READ_PS    = 0xB4
  };

  template<gpio::Address DATA_PORT, u8 DATA_PIN, tim::Address DELAY_TIMER_ADDRESS>
  class Functions
  {
    typedef onewire::Functions<DATA_PORT, DATA_PIN, DELAY_TIMER_ADDRESS> DEVICE;

    public:
      static inline void initialize();
      static inline bool runTemperatureConversion();
      static inline bool readTemperature(u16&);

    private:
      static bool is_valid;
      Functions();
  };
  
}

#include "../../bits/ds18b20.tcc"
