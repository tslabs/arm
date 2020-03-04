
/// - Header includes ---
#include <stdarg.h>
#include <stdlib.h>
#include <limits.h>
#include "string.h"
#include "exception.cpp"
#include "interrupt.hpp"
#include "clock.hpp"
#include "core/stk.hpp"
#include "core/scb.hpp"
#include "peripheral/gpio.hpp"
#include "peripheral/tim.hpp"
#include "peripheral/dma.hpp"
#include "peripheral/spi.hpp"
#include "peripheral/usart.hpp"
#include "peripheral/exti.hpp"
#include "peripheral/usb_fs.hpp"
// #include "peripheral/syscfg.hpp"
#include "../other/fifo.hpp"
#define USE_FT81x
#include "driver/ft8xx.hpp"
#include "main.hpp"

/// - Vars ---
volatile u32 time;

volatile bool is_sending;        // indicates that UART output is in progress
FIFO uart_in;
FIFO uart_out;
u8 uart_inbuf [9];
u8 uart_outbuf[257];
u8 dma_buf[256];

const ft8xx::MODE vmode = {8,  1, 24, 136, 160, 1024, 3,  6, 29, 768};   //  7: 1024x768@59Hz (64MHz)

u8 buf[64];

/// - Functions ---
void initializeRuntime()
{
  time = 0;

  uart_in.init (uart_inbuf,  sizeof(uart_inbuf));
  uart_out.init(uart_outbuf, sizeof(uart_outbuf));
  is_sending = false;
}

namespace clk
{
  void hseFailureHandler()
  {
    while (true);
  }
}

/// - Hardware ---
void initializeClock()
{
  RCC::useHseOscillator();
  RCC::enableHse();
  while (!RCC::isHseStable());
  RCC::configurePll<
    rcc::cfgr::sws::HSI_OSCILLATOR_USED_AS_SYSTEM_CLOCK,
    rcc::cfgr::pllsrc::USE_PREDIV1_OUTPUT_AS_PLL_SOURCE,
    rcc::cfgr::pllxtpre::HSE_SELECTED_AS_PREDIV1_SOURCE,
    rcc::cfgr::pllmul::PLL_INPUT_CLOCK_X_10,
    rcc::cfgr::hpre::SYSCLK_NOT_DIVIDED,
    rcc::cfgr::ppre1::HCLK_DIVIDED_BY_2,
    rcc::cfgr::ppre2::HCLK_NOT_DIVIDED,
    rcc::cfgr::usbpre::PLL_CLOCK_DIVIDED_BY_1_DOT_5,
    rcc::cfgr::adcpre::P2CLK_DIVIDED_BY_6
  >();
  RCC::enablePll();
  while (!RCC::isPllStable());
  RCC::configureClockOutput(rcc::cfgr::mco::OUTPUT_PLL_CLOCK_OVER_2);
  FLASH::configure(
    flash::acr::latency::TWO_WAIT_STATES,
    flash::acr::hlfcya::FLASH_HALF_CYCLE_ACCESS_DISABLED,
    flash::acr::prftbe::PREFETCH_ENABLED);
  RCC::setSystemClockSource(rcc::cfgr::sw::PLL_SELECTED_AS_SYSTEM_CLOCK);
  while (!RCC::isSystemClockSourceStable());
}

void initializePeripherals()
{
  LED::enableClock();
  LED::setLow();
  LED::setMode(gpio::cr::GP_PUSH_PULL_2MHZ);

  HB::enableClock();
  HB::setMode(gpio::cr::FLOATING_INPUT);
  VB::enableClock();
  VB::setMode(gpio::cr::FLOATING_INPUT);

  MCO::enableClock();
  MCO::setMode(gpio::cr::AF_PUSH_PULL_50MHZ);

  RX::enableClock();
  RX::pullUp();
  RX::setMode(gpio::cr::INPUT_PULL_X);
  TX::setMode(gpio::cr::AF_PUSH_PULL_2MHZ);
  UART::enableClock();
  UART::setBaudRate<UART_BAUD_RATE>();
  UART::configure(
    usart::cr1::rwu::RECEIVER_IN_ACTIVE_MODE,
    usart::cr1::re::RECEIVER_ENABLED,
    usart::cr1::te::TRANSMITTER_ENABLED,
    usart::cr1::idleie::IDLE_INTERRUPT_DISABLED,
    usart::cr1::rxneie::RXNE_ORE_INTERRUPT_ENABLED,
    usart::cr1::tcie::TC_INTERRUPT_ENABLED,         // aware: false(?) TC interrupt pending right after initialization
    usart::cr1::txeie::TXEIE_INTERRUPT_DISABLED,
    usart::cr1::peie::PEIE_INTERRUPT_DISABLED,
    usart::cr1::ps::EVEN_PARITY,
    usart::cr1::pce::PARITY_CONTROL_DISABLED,
    usart::cr1::wake::WAKE_ON_IDLE_LINE,
    usart::cr1::m::START_8_DATA_N_STOP,
    usart::cr1::ue::USART_ENABLED,
    usart::cr1::over8::OVERSAMPLING_BY_16,
    usart::cr2::stop::_1_STOP_BIT,
    usart::cr3::eie::ERROR_INTERRUPT_DISABLED,
    usart::cr3::hdsel::FULL_DUPLEX,
    usart::cr3::dmar::RECEIVER_DMA_ENABLED,
    usart::cr3::dmat::TRANSMITTER_DMA_DISABLED,
    usart::cr3::rtse::RTS_HARDWARE_FLOW_DISABLED,
    usart::cr3::ctse::CTS_HARDWARE_FLOW_DISABLED,
    usart::cr3::ctsie::CTS_INTERRUPT_DISABLED,
    usart::cr3::onebit::THREE_SAMPLE_BIT_METHOD);

  PB0::setHigh();
  PB0::setMode(gpio::cr::GP_PUSH_PULL_2MHZ);

  SS::setHigh();
  SS::setMode(gpio::cr::GP_PUSH_PULL_2MHZ);
  SCK::setMode(gpio::cr::AF_PUSH_PULL_50MHZ);
  MOSI::setMode(gpio::cr::AF_PUSH_PULL_10MHZ);
  MISO::pullUp();
  MISO::setMode(gpio::cr::INPUT_PULL_X);

  SPI::enableClock();
  SPI::configure(
    spi::cr1::cpha::FIRST_CLOCK_TRANSITION_IS_FIRST_DATA_CAPTURED_EDGE,
    spi::cr1::cpol::CK_TO_0_WHEN_IDLE,
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
    // spi::cr2::txdmaen::TX_BUFFER_DMA_ENABLED,
    spi::cr2::txeie::TXE_INTERRUPT_DISABLED);
  SPI::enable();

  DMA::enableClock();
  DMA::configure(
    dma::channel::cr::tcie::TRANSFER_COMPLETE_INTERRUPT_DISABLED,
    dma::channel::cr::htie::HALF_TRANSFER_INTERRUPT_DISABLED,
    dma::channel::cr::teie::TRANSFER_ERROR_INTERRUPT_DISABLED,
    dma::channel::cr::dir::READ_FROM_MEMORY,
    dma::channel::cr::circ::CIRCULAR_MODE_DISABLED,
    dma::channel::cr::pinc::PERIPHERAL_INCREMENT_MODE_DISABLED,
    dma::channel::cr::minc::MEMORY_INCREMENT_MODE_ENABLED,
    dma::channel::cr::psize::PERIPHERAL_SIZE_8BITS,
    dma::channel::cr::msize::MEMORY_SIZE_8BITS,
    dma::channel::cr::pl::CHANNEL_PRIORITY_LEVEL_VERY_HIGH,
    dma::channel::cr::mem2mem::MEMORY_TO_MEMORY_MODE_DISABLED);
  DMA::setPeripheralAddress(&SPI_REGS->DR);
  DMA::setMemoryAddress(dma_buf);
}

/// - Interrupts ---
void initializeInterrupts()
{
  SYSTICK::configure<
    SYSTICK_PERIOD,
    stk::cr::clksource::EXTERNAL_CLOCK,
    stk::cr::tickint::TICKINT_ENABLED,
    stk::cr::enable::SYSTICK_ENABLED>();

  NVIC::setPriority<nvic::irqn::UART_IRQ, UART_IRQ_PRIORITY>();
  NVIC::enableIrq<nvic::irqn::UART_IRQ>();

  // NVIC::setPriority<nvic::irqn::DMA_IRQ, DMA_IRQ_PRIORITY>();
  // NVIC::enableIrq<nvic::irqn::DMA_IRQ>();
}

void processRecv()
{
  UART::clearRXNE();
  uart_in.put_byte(UART_REGS->DR);
}

void processSend()
{
  UART::clearTC();
  if (uart_out.used())
  {
    is_sending = true;
    UART_REGS->DR = uart_out.get_byte_nocheck();
  }
  else
    is_sending = false;
}

void processUART()
{
  u32 sr = UART_REGS->SR;

  if (sr & usart::sr::rxne::DATA_RECEIVED)
    processRecv();

  if (sr & usart::sr::tc::TRANSMISSION_COMPLETED)
    processSend();
}

namespace exception
{
  extern "C"
  {
    void SysTick()
    {
      time += SYSTICK_PERIOD;
    }

    void BusFault()
    {
      SCB::generateReset(scb::airc::VECTRESET | scb::airc::VECTCLRACTIVE);
      while (1);
    }
  }
}

namespace interrupt
{
  void UART_IRQ()
  {
    processUART();
  }

  void DMA_IRQ()
  {
    DMA::clearTransferCompleteFlag();
    DMA::clearHalfTransferFlag();
  }
}

u32 cmdb[0x800];

enum
{
  X_POS = 350,
  Y_POS = 250
};

void screen()
{
  ft8xx::ccmdStart(cmdb);
  ft8xx::Dlstart();
  ft8xx::VertexFormat(0);
  ft8xx::ClearColorRGB(0, 0, 0);
  ft8xx::ClearColorA(255);
  ft8xx::Clear(1, 1, 1);

  ft8xx::ColorA(255);
  ft8xx::Begin(ft8xx::prim::POINTS);
  ft8xx::PointSize(100 << 4);
  ft8xx::BlendFunc(ft8xx::afunc::SRC_ALPHA, ft8xx::afunc::ONE);
  ft8xx::ColorRGB(255, 0, 0);
  ft8xx::Vertex2f(ft8xx::rsin(80, 0) + X_POS, ft8xx::rcos(80, 0) + Y_POS);
  ft8xx::ColorRGB(0, 255, 0);
  ft8xx::Vertex2f(ft8xx::rsin(80, 21845) + X_POS, ft8xx::rcos(80, 21845) + Y_POS);
  ft8xx::ColorRGB(0, 0, 255);
  ft8xx::Vertex2f(ft8xx::rsin(80, 43690) + X_POS, ft8xx::rcos(80, 43690) + Y_POS);

  ft8xx::ClearColorA(32);
  ft8xx::ColorMask(0, 0, 0, 1);
  ft8xx::Clear(1, 1, 1);

  ft8xx::BlendFunc(ft8xx::afunc::ONE, ft8xx::afunc::ONE);
  ft8xx::Begin(ft8xx::prim::POINTS);
  ft8xx::ColorA(20);

  for (int i = 120, j = 0; j <= 255; i -= 5, j += 20)
  {
    ft8xx::PointSize(i << 4);
    ft8xx::Vertex2f(X_POS, Y_POS);
  }

  ft8xx::ColorRGB(0, 0, 0);
  ft8xx::ColorMask(1, 1, 1, 1);
  ft8xx::BlendFunc(ft8xx::afunc::ONE_MINUS_DST_ALPHA, ft8xx::afunc::DST_ALPHA);
  ft8xx::Begin(ft8xx::prim::RECTS);
  ft8xx::Vertex2f(0, 0);
  ft8xx::Vertex2f(799, 599);

  ft8xx::ColorA(255);
  ft8xx::BlendFunc(ft8xx::afunc::SRC_ALPHA, ft8xx::afunc::ONE_MINUS_SRC_ALPHA);
  ft8xx::RomFont(14, 34);
  ft8xx::ColorRGB(255, 0, 0);
  ft8xx::Number(100, 80, 14, 0, FT812::readReg(ft8xx::reg::CLOCK));
  ft8xx::ColorRGB(0, 200, 240);
  ft8xx::Number(100, 220, 14, 0, FT812::readReg(ft8xx::reg::FRAMES));
  ft8xx::ColorRGB(0, 200, 0);
  ft8xx::Text(100, 50, 31, 0, "Clocks:");
  ft8xx::Text(100, 190, 31, 0, "Frames:");
  ft8xx::ColorRGB(255, 120, 0);
  ft8xx::Text(220, 500, 31, 0, "All your base are belong to us!");

  ft8xx::Display();
  ft8xx::Swap();
  FT812::ccmdWrite();
  FT812::cpWait();
}

void uart_input(u8 d)
{
  // switch (d)
  // {
  // }
}

/// - Main ---
void resetHandler()
{
  initializeClock();
  initializeRuntime();
  initializePeripherals();
  initializeInterrupts();
  LED::setLow();
  FT812::initialize(vmode);
  LED::setHigh();

  while (1)
  {
    screen();

    if (!is_sending && uart_out.used())
      processSend();

    if (uart_in.used())
      uart_input(uart_in.get_byte_nocheck());
  }
}
