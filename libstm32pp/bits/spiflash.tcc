
#pragma once

#ifdef STM32F1XX
namespace spiflash
{
  template <spi::Address S, gpio::Address SS_PORT, u8 SS_PIN>
  void Functions <S, SS_PORT, SS_PIN>::initialize()
  {
    SS::setHigh();
    SS::setMode(gpio::cr::GP_PUSH_PULL_50MHZ);

    SPI::enableClock();
    SPI::configure(
      spi::cr1::cpha::FIRST_CLOCK_TRANSITION_IS_FIRST_DATA_CAPTURED_EDGE,
      spi::cr1::cpol::CK_TO_0_WHEN_IDLE,
      spi::cr1::mstr::MASTER_CONFIGURATION,
      spi::cr1::br::BAUD_RATE_CONTROL_DIV_2,
      spi::cr1::lsbfirst::MSB_TRANSMITTED_FIRST,
      spi::cr1::ssm::SOFTWARE_SLAVE_MANAGEMENT_ENABLED,
      spi::cr1::ssi::INTERNAL_SLAVE_SELECT_ENABLED,
      spi::cr1::rxonly::FULL_DUPLEX,
      spi::cr1::dff::DATA_FRAME_FORMAT_8_BIT,
      spi::cr1::crcnext::NO_CRC_PHASE,
      spi::cr1::crcen::CRC_CALCULATION_DISABLED,
      spi::cr1::bidioe::OUTPUT_ENABLED,
      spi::cr1::bidimode::DATA_MODE_2LINE_UNIDIRECTIONAL,
      spi::cr2::errie::ERROR_INTERRUPT_DISABLED,
      spi::cr2::frf::SPI_MOTOROLA_MODE,
      spi::cr2::rxdmaen::RX_BUFFER_DMA_DISABLED,
      spi::cr2::rxneie::RXNE_INTERRUPT_DISABLED,
      spi::cr2::ssoe::SS_OUTPUT_DISABLED_MASTER_MODE,
      spi::cr2::txdmaen::TX_BUFFER_DMA_DISABLED,
      spi::cr2::txeie::TXE_INTERRUPT_DISABLED);
    SPI::enable();
  }

  template <spi::Address S, gpio::Address SS_PORT, u8 SS_PIN>
  void Functions <S, SS_PORT, SS_PIN>::ssLow()
  {
    SS::setLow();
  }

  template <spi::Address S, gpio::Address SS_PORT, u8 SS_PIN>
  void Functions <S, SS_PORT, SS_PIN>::ssHigh()
  {
    // while (SPI::busy());
    SS::setHigh();
  }

  template <spi::Address S, gpio::Address SS_PORT, u8 SS_PIN>
  u8 Functions <S, SS_PORT, SS_PIN>::sendByte(u8 data)
  {
    while (!SPI::canSendData());
    SPI::sendByte(data);
    while (!SPI::hasReceivedData());
    return SPI::getByte();
  }

  template <spi::Address S, gpio::Address SS_PORT, u8 SS_PIN>
  u32 Functions <S, SS_PORT, SS_PIN>::detectSize()
  {
    ssLow();
    sendByte(cmd::DEV_ID);
    sendDummy(3);
    u8 s = sendByte(0);
    ssHigh();

    u32 sz = 0;
    switch (s)
    {
      case id::KBIT_512: sz = 64;    break;
      case id::MBIT_1:   sz = 128;   break;
      case id::MBIT_2:   sz = 256;   break;
      case id::MBIT_4:   sz = 512;   break;
      case id::MBIT_8:   sz = 1024;  break;
      case id::MBIT_16:  sz = 2048;  break;
      case id::MBIT_32:  sz = 4096;  break;
      case id::MBIT_64:  sz = 8192;  break;
      case id::MBIT_128: sz = 16384; break;
      case id::MBIT_256: sz = 32768; break;
    };

    return sz * 1024;
  }

  template <spi::Address S, gpio::Address SS_PORT, u8 SS_PIN>
  void Functions <S, SS_PORT, SS_PIN>::readBlock(u32 addr, u8 *buf, u32 len)
  {
    ssLow();
    sendByte(cmd::READ);
    sendByte((u8)(addr >> 16));
    sendByte((u8)(addr >> 8));
    sendByte((u8)addr);

    while (len--)
      *buf++ = sendByte(0);

    ssHigh();
  }

  template <spi::Address S, gpio::Address SS_PORT, u8 SS_PIN>
  bool Functions <S, SS_PORT, SS_PIN>::verifyBlock(u32 addr, u8 *buf, u32 len)
  {
    bool rc = false;

    ssLow();
    sendByte(cmd::READ);
    sendByte((u8)(addr >> 16));
    sendByte((u8)(addr >> 8));
    sendByte((u8)addr);

    while (len--)
    {
      if (sendByte(0) != *buf++)
        goto exit;
    }
    rc = true;

  exit:
    ssHigh();
    return rc;
  }

  template <spi::Address S, gpio::Address SS_PORT, u8 SS_PIN>
  void Functions <S, SS_PORT, SS_PIN>::writeBlock(u32 addr, u8 *buf, u32 len)
  {
    ssLow();
    sendByte(cmd::WRITE_EN);
    ssHigh();

    ssLow();
    sendByte(cmd::WRITE);
    sendByte((u8)(addr >> 16));
    sendByte((u8)(addr >> 8));
    sendByte((u8)addr);

    while (len--)
      sendByte(*buf++);

    ssHigh();
  }

  template <spi::Address S, gpio::Address SS_PORT, u8 SS_PIN>
  void Functions <S, SS_PORT, SS_PIN>::eraseSector(u32 addr)
  {
    ssLow();
    sendByte(cmd::WRITE_EN);
    ssHigh();

    ssLow();
    sendByte(cmd::ERASE_SEC);
    sendByte((u8)(addr >> 16));
    sendByte((u8)(addr >> 8));
    sendByte((u8)addr);
    ssHigh();
  }

  template <spi::Address S, gpio::Address SS_PORT, u8 SS_PIN>
  void Functions <S, SS_PORT, SS_PIN>::eraseChip()
  {
    ssLow();
    sendByte(cmd::WRITE_EN);
    ssHigh();

    ssLow();
    sendByte(cmd::ERASE_CHIP);
    ssHigh();
  }

  template <spi::Address S, gpio::Address SS_PORT, u8 SS_PIN>
  bool Functions <S, SS_PORT, SS_PIN>::checkBlank(u32 addr, u32 len)
  {
    bool rc = false;

    ssLow();
    sendByte(cmd::READ);
    sendByte((u8)(addr >> 16));
    sendByte((u8)(addr >> 8));
    sendByte((u8)addr);

    while (len--)
      if (sendByte(0) != 0xFF) break;
    rc = len == -1;

    ssHigh();
    return rc;
  }

  template <spi::Address S, gpio::Address SS_PORT, u8 SS_PIN>
  u8 Functions <S, SS_PORT, SS_PIN>::readStatus()
  {
    ssLow();
    sendByte(cmd::READ_STAT);
    u8 rc = sendByte(0);
    ssHigh();
    return rc;
  }

  template <spi::Address S, gpio::Address SS_PORT, u8 SS_PIN>
  void Functions <S, SS_PORT, SS_PIN>::waitBusy()
  {
    while (readStatus() & status::BUSY);
  }

  template <spi::Address S, gpio::Address SS_PORT, u8 SS_PIN>
  bool Functions <S, SS_PORT, SS_PIN>::isBusy()
  {
    return (readStatus() & status::BUSY);
  }

  template <spi::Address S, gpio::Address SS_PORT, u8 SS_PIN>
  void Functions <S, SS_PORT, SS_PIN>::sendDummy(int n)
  {
    while (n--)
      sendByte(0);
  }
}  // namespace sccb
#else
  #error Only STM32F1XX yet implemented!
#endif

