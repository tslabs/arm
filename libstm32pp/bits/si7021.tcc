namespace si7021
{
  // initialize IOs
  template<gpio::Address SCL_PORT, u8 SCL_PIN, gpio::Address SDA_PORT, u8 SDA_PIN, tim::Address DELAY_TIMER_ADDRESS, u32 FREQUENCY>
  void Functions<SCL_PORT, SCL_PIN, SDA_PORT, SDA_PIN, DELAY_TIMER_ADDRESS, FREQUENCY>::initialize()
  {
    DEVICE::initialize();
  }

  // measure humidity and temperature
  template<gpio::Address SCL_PORT, u8 SCL_PIN, gpio::Address SDA_PORT, u8 SDA_PIN, tim::Address DELAY_TIMER_ADDRESS, u32 FREQUENCY>
  bool Functions<SCL_PORT, SCL_PIN, SDA_PORT, SDA_PIN, DELAY_TIMER_ADDRESS, FREQUENCY>::measure(u16 &hum, u16 &tem)
  {
    DEVICE::sendStart();
    if (!DEVICE::sendByte(DEVICE_ADDR << 1)) goto err;
    if (!DEVICE::sendByte(CMD_MS_HUM)) goto err;

    for (u8 i = 1; i < 255; i++)
    {
      DEVICE::sendStart();
      if (DEVICE::sendByte((DEVICE_ADDR << 1) | 1)) goto read1;
    }
    goto err;

  read1:
    hum = (DEVICE::recvByte(true)) << 8;
    hum |= DEVICE::recvByte(false);
    DEVICE::sendStop();

    DEVICE::sendStart();
    if (!DEVICE::sendByte(DEVICE_ADDR << 1)) goto err;
    if (!DEVICE::sendByte(CMD_MS_TEM)) goto err;

    for (u16 i = 1; i < 1000; i++)
    {
      DEVICE::sendStart();
      if (DEVICE::sendByte((DEVICE_ADDR << 1) | 1)) goto read2;
    }
    goto err;

  read2:
    tem = (DEVICE::recvByte(true)) << 8;
    tem |= DEVICE::recvByte(false);
    DEVICE::sendStop();
    return true;

  err:
    DEVICE::sendStop();
    return false;
  }

  // read unique serial number
  template<gpio::Address SCL_PORT, u8 SCL_PIN, gpio::Address SDA_PORT, u8 SDA_PIN, tim::Address DELAY_TIMER_ADDRESS, u32 FREQUENCY>
  bool Functions<SCL_PORT, SCL_PIN, SDA_PORT, SDA_PIN, DELAY_TIMER_ADDRESS, FREQUENCY>::readSn(u8 *sn)
  {
    DEVICE::sendStart();
    if (!DEVICE::sendByte(DEVICE_ADDR << 1)) goto err;
    if (!DEVICE::sendByte(CMD_RD_ID1_1)) goto err;
    if (!DEVICE::sendByte(CMD_RD_ID1_2)) goto err;
    DEVICE::sendStart();
    if (!DEVICE::sendByte((DEVICE_ADDR << 1) | 1)) goto err;
    for (u8 i = 0; i < 4; i++)
    {
      sn[i] = DEVICE::recvByte(true);
      DEVICE::recvByte(i != 3);
    }
    DEVICE::sendStop();

    DEVICE::sendStart();
    if (!DEVICE::sendByte(DEVICE_ADDR << 1)) goto err;
    if (!DEVICE::sendByte(CMD_RD_ID2_1)) goto err;
    if (!DEVICE::sendByte(CMD_RD_ID2_2)) goto err;
    DEVICE::sendStart();
    if (!DEVICE::sendByte((DEVICE_ADDR << 1) | 1)) goto err;
    for (u8 i = 4; i < 8;)
    {
      sn[i++] = DEVICE::recvByte(true);
      sn[i++] = DEVICE::recvByte(true);
      DEVICE::recvByte(i != 8);
    }
    DEVICE::sendStop();
    return true;

  err:
    DEVICE::sendStop();
    return false;
  }

  // returns normalized humidity value in 8.8 fixed point format: bits 8..15 - integer part, bits 0..7 - fractal part
  u16 calculateHumidity(u16 val)
  {
    return ((125 * (u32)val) >> 8) - (6 << 8);   // %RH = ((125 * hum) / 65536) - 6
  }

  // returns normalized temperature value in 8.8 fixed point format: bits 8..15 - integer part, bits 0..7 - fractal part
  u16 calculateTemperature(u16 val)
  {
    return ((44984 * (u32)val) >> 16) - 11993;   // 'C = ((175.72 * hum) / 65536) - 46.85
  }
}