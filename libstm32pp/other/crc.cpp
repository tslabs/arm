
namespace crc
{
  // Dallas/Maxim CRC-8
  // Poly: 0x31
  // Initial: 0xFF
  u8 init_crc8()
  {
    return 0xFF;
  }

  u8 update_crc8(u8 crc, u8 data)
  {
    crc ^= data;
    u8 i = 8;

    do
    {
      if (crc & 0x80)
        crc = (crc << 1) ^ 0x31;
      else
        crc <<= 1;
    } while(--i);

    return crc;
  }

  // CRC-CCITT
  // Poly: 0x8408
  // Initial: 0xFFFF
  u16 init_crc16()
  {
    return 0xFFFF;
  }
  
  u16 update_crc16(u16 crc, u8 data)
  {
    data ^= (u8)(crc);
    data ^= data << 4;

    return ((((u16)data << 8) | (u8)(crc >> 8)) ^ (u8)(data >> 4) ^ ((u16)data << 3));
  }

  u8 calc_crc8(u8 *ptr, int num)
  {
    u8 crc = 0xFF;

    while (num--)
      crc = update_crc8(crc, *ptr++);

    return crc;
  }

  u16 calc_crc16(u8 *ptr, int num)
  {
    u16 crc = 0xFFFF;

    while (num--)
      crc = update_crc16(crc, *ptr++);

    return crc;
  }
}
