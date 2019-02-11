
namespace ds18b20
{
  template<gpio::Address DATA_PORT, u8 DATA_PIN, tim::Address DELAY_TIMER_ADDRESS>
  bool Functions<DATA_PORT, DATA_PIN, DELAY_TIMER_ADDRESS>::is_valid;

  template<gpio::Address DATA_PORT, u8 DATA_PIN, tim::Address DELAY_TIMER_ADDRESS>
  void Functions<DATA_PORT, DATA_PIN, DELAY_TIMER_ADDRESS>::initialize()
  {
    DEVICE::initialize();
    is_valid = false;
  }

  template<gpio::Address DATA_PORT, u8 DATA_PIN, tim::Address DELAY_TIMER_ADDRESS>
  bool Functions<DATA_PORT, DATA_PIN, DELAY_TIMER_ADDRESS>::runTemperatureConversion()
  {
    if (!DEVICE::reset())
    {
      is_valid = false;
      return false;
    }

    // Start the temperature conversion
    DEVICE::writeByte(SKIP_ROM);
    DEVICE::writeByte(CONVERT_T);
    is_valid = true;

    return true;
  }

  template<gpio::Address DATA_PORT, u8 DATA_PIN, tim::Address DELAY_TIMER_ADDRESS>
  bool Functions<DATA_PORT, DATA_PIN, DELAY_TIMER_ADDRESS>::readTemperature(u16 &temp)
  {
    temp = 0;

    if (!DEVICE::reset())
    {
      is_valid = false;
      return false;
    }

    if (!is_valid) return false;

    // Read scratchpad
    DEVICE::writeByte(SKIP_ROM);
    DEVICE::writeByte(READ);

    // 8T.4t is tttt0000, TTTTTTTT
    temp = DEVICE::readByte() << 4;
    temp |= DEVICE::readByte() << 12;
    temp ^= 0x8000;  // make signed-unsingned conversion

    return true;
  }
}
