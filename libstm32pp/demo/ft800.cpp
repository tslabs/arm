
#include "string.h"
#include "clock.hpp"
#include "interrupt.hpp"
#include "peripheral/gpio.hpp"
#include "peripheral/spi.hpp"
#include "peripheral/tim.hpp"
#include "driver/ft800.hpp"

typedef PD15 LED;

typedef SPI2        SPI;
#define SPI_AF      SPI1_2

// SPI
typedef PB15  SPI_MOSI;
typedef PC2   SPI_MISO;
typedef PB13  SPI_SCK;
typedef PB12  SPI_NSS;

// FT800
typedef ft800::Functions<
    gpio::GPIOB, 12,  // SPI_NSS
    spi::SPI2
    > FT800;

// MCO (XCLK)
typedef PA8 MCO1;

void initializeGpio()
{
  // clocks
  GPIOA::enableClock();
  GPIOB::enableClock();
  GPIOC::enableClock();
  GPIOD::enableClock();
  GPIOE::enableClock();

  // MCO (XCLK)
  MCO1::setSpeed(gpio::ospeedr::_2MHZ);   // to avoid digital noise
  MCO1::setAlternateFunction(gpio::afr::SYSTEM);
  MCO1::setMode(gpio::moder::ALTERNATE);

  // SPI
  SPI_MOSI::setAlternateFunction(gpio::afr::SPI_AF);
  SPI_MOSI::setMode(gpio::moder::ALTERNATE);
  SPI_MOSI::setSpeed(gpio::ospeedr::_2MHZ);

  SPI_MISO::setAlternateFunction(gpio::afr::SPI_AF);
  SPI_MISO::setMode(gpio::moder::ALTERNATE);
  SPI_MISO::setSpeed(gpio::ospeedr::_2MHZ);

  SPI_SCK::setAlternateFunction(gpio::afr::SPI_AF);
  SPI_SCK::setMode(gpio::moder::ALTERNATE);
  SPI_SCK::setSpeed(gpio::ospeedr::_2MHZ);

  SPI_NSS::setMode(gpio::moder::OUTPUT);
  SPI_NSS::setSpeed(gpio::ospeedr::_2MHZ);
  SPI_NSS::setHigh();

  // LED
  LED::enableClock();
  LED::setMode(gpio::moder::OUTPUT);
}

void initializeTimer()
{
  TIM6::enableClock();
  TIM6::configurePeriodicInterrupt<
      50 /* Hz */
  >();
}

void initializeSpi()
{
  SPI::enableClock();

  SPI::configure(
    spi::cr1::cpha::FIRST_CLOCK_TRANSITION_IS_FIRST_DATA_CAPTURED_EDGE,
    spi::cr1::cpol::CK_TO_1_WHEN_IDLE,
    spi::cr1::mstr::MASTER_CONFIGURATION,
    spi::cr1::br::BAUD_RATE_CONTROL_DIV_4,
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

void initializePeripherals()
{
  initializeGpio();
  initializeSpi();
  initializeTimer();
}

u8 buf[256];

int main()
{
  clk::initialize();

  initializePeripherals();
  TIM6::startCounter();

  FT800::powerUp();
  FT800::readMem(ft800::FT_ROM_CHIPID, 4, buf);
  FT800::readMem(ft800::REG_ID, 4, buf + 4);

  FT800::disableDisp();
  FT800::setRegs(ft800::wqvga_mode);
  FT800::sendCommand(ft800::FT_CLKEXT);
  FT800::enableDisp();

  while (true)
  {
    FT800::writeReg(ft800::FT_RAM_CMD, ft800::CMD_LOGO);
    FT800::writeReg(ft800::REG_CMD_WRITE, 4);
    
    // FT800::writeReg(ft800::FT_RAM_CMD, ft800::CMD_CALIBRATE);
    // FT800::writeReg(ft800::REG_CMD_WRITE, 4);
  }
}

void interrupt::TIM6_DAC()
{
  TIM6::clearUpdateFlag();

  static u8 led = 0;

  led ? LED::setHigh() : LED::setLow();
  led = ~led;

  // SPI_NSS::setLow();
  // SPI::sendByte(0x5A);
  // while(SPI::busy());
  // SPI_NSS::setHigh();
}