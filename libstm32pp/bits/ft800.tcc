
#pragma once

#define _FT800_TCC_TEMPL \
  template< \
    gpio::Address SPI_NSS_PORT, \
    u8 SPI_NSS_PIN, \
    spi::Address SPI_ADDRESS>

#define _FT800_TCC_FUNC \
  Functions< \
      SPI_NSS_PORT, \
      SPI_NSS_PIN, \
      SPI_ADDRESS>

// Enable clock for FT800
namespace ft800
{
  // Power up FT800
  _FT800_TCC_TEMPL void _FT800_TCC_FUNC::powerUp()
  {
    // add PD pin = 1
    sendCommand(FT_ACTIVE);
  }

  // Power down FT800
  _FT800_TCC_TEMPL void _FT800_TCC_FUNC::powerDown()
  {
    // add PD pin = 0
  }

  // Read 32 bit register
  _FT800_TCC_TEMPL u32 _FT800_TCC_FUNC::readReg(u32 addr)
  {
    u32 val;
    readMem(addr, sizeof(val), (u8*)&val);
    return val;
  }

// Write 32 bit register
  _FT800_TCC_TEMPL void _FT800_TCC_FUNC::writeReg(u32 addr, u32 val)
  {
    writeMem(addr, sizeof(val), (u8*)&val);
  }

  // Read FT800 memory
  // Format: 00aaaaaa aaaaaaaa aaaaaaaa 00000000 <byte0> ... <byteN>
  _FT800_TCC_TEMPL void _FT800_TCC_FUNC::readMem(u32 addr, u16 num, u8* buf)
  {
    SPI_NSS::setLow();

    sendByte((addr >> 16) & 0x3F);
    sendByte((addr >> 8) & 0xFF);
    sendByte(addr & 0xFF);

    while (num--)
    {
      sendByte(0);
      *buf++ = SPI::getByte();
    }

    SPI_NSS::setHigh();
  }

  // Write FT800 memory
  // Format: 10aaaaaa aaaaaaaa aaaaaaaa <byte0> ... <byteN>
  _FT800_TCC_TEMPL void _FT800_TCC_FUNC::writeMem(u32 addr, u16 num, u8* buf)
  {
    SPI_NSS::setLow();

    sendByte(((addr >> 16) & 0x3F) | 0x80);
    sendByte((addr >> 8) & 0xFF);
    sendByte(addr & 0xFF);

    while (num--)
      sendByte(*buf++);

    SPI_NSS::setHigh();
  }

  // Send FT800 command
  // Format: 01cccccc 00000000 00000000
  _FT800_TCC_TEMPL void _FT800_TCC_FUNC::sendCommand(u8 cmd)
  {
    SPI_NSS::setLow();
    sendByte(cmd);
    sendByte(0);
    sendByte(0);
    SPI_NSS::setHigh();
  }

  // Send byte via SPI
  // Waits for SPI busy before/after the transaction
  _FT800_TCC_TEMPL void _FT800_TCC_FUNC::sendByte(u8 byte)
  {
    while(SPI::busy());
    SPI::sendByte(byte);
    while(SPI::busy());
  }

  // Set regs using constant array
  _FT800_TCC_TEMPL void _FT800_TCC_FUNC::setRegs(const REGSET rset[])
  {
    int i = 0;
    while (rset[i].addr)
    {
      writeReg(rset[i].addr, rset[i].data);
      i++;
    }
  }

  // Enable Display
  _FT800_TCC_TEMPL void _FT800_TCC_FUNC::enableDisp()
  {
    writeReg(REG_GPIO, readReg(REG_GPIO) | (1 << FT_DISP_EN));
    writeReg(REG_GPIO_DIR, readReg(REG_GPIO_DIR) | (1 << FT_DISP_EN));
  }

  // Disable Display
  _FT800_TCC_TEMPL void _FT800_TCC_FUNC::disableDisp()
  {
    writeReg(REG_GPIO, readReg(REG_GPIO) & ~(1 << FT_DISP_EN));
    writeReg(REG_GPIO_DIR, readReg(REG_GPIO_DIR) | (1 << FT_DISP_EN));
  }
}  // namespace ft800
