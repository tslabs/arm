
// Hardware
//
// AY-ARM project
// (c) TS-Labs
//
// Degoša lāpa būs mūsu saule

/*
void initializeClock()
{
  RCC::useHseOscillator();
  RCC::enableHse();
  while (!RCC::isHseStable());
  RCC::configurePll<
    rcc::cfgr::pllsrc::USE_PREDIV1_OUTPUT_AS_PLL_SOURCE,
    7, // rcc::cfgr::pllmul::PLL_INPUT_CLOCK_X_9
    0, // rcc::cfgr2::prediv1
    0, // rcc::cfgr2::prediv2
    7, // rcc::cfgr2::pll2mul
    6, // rcc::cfgr2::pll3mul
    rcc::cfgr2::prediv1src::USE_HSE_OSCILLATOR_AS_PREDIV1_INPUT,
    rcc::cfgr2::i2s2src::USE_SYSTEM_CLOCK_AS_I2S2_CLOCK,
    rcc::cfgr2::i2s3src::USE_PLL3_CLOCK_AS_I2S3_CLOCK>();
  RCC::configureBusPrescalers<
    0, // rcc::cfgr::hpre::SYSCLK_NOT_DIVIDED,
    4, // rcc::cfgr::ppre1::HCLK_DIVIDED_BY_2,
    0, // rcc::cfgr::ppre2::HCLK_NOT_DIVIDED,
    3, // rcc::cfgr::adcpre::P2CLK_DIVIDED_BY_6,
    1  // rcc::cfgr::otgfspre::PLL_CLOCK_DIVIDED_BY_1_DOT_5
  >();
  RCC::enablePll();
  while (!RCC::isPllStable());
  FLASH::configure(
    flash::acr::latency::TWO_WAIT_STATES,
    flash::acr::hlfcya::FLASH_HALF_CYCLE_ACCESS_DISABLED,
    flash::acr::prftbe::PREFETCH_ENABLED);
  RCC::setSystemClockSource(rcc::cfgr::sw::PLL_SELECTED_AS_SYSTEM_CLOCK);
  while (!RCC::isSystemClockSourceStable());
}
*/

void initializePins()
{
  // GPIO
  GPIOA::enableClock();
  GPIOB::enableClock();

  TEST::setSpeed(gpio::ospeedr::_25MHZ);
  TEST::setLow();
  TEST::setMode(gpio::moder::OUTPUT);

  // AY Bus
#ifdef AYBUS_USES_EXTI
  AY_BDIR::setMode(gpio::moder::INPUT);
  AY_BDIR::setPullMode(gpio::pupdr::PULL_DOWN);
  AY_BC1::setMode(gpio::moder::INPUT);
  AY_BC1::setPullMode(gpio::pupdr::PULL_DOWN);
#else
  AY_BDIR::setAlternateFunction(gpio::afr::TIM3_5);
  AY_BDIR::setMode(gpio::moder::ALTERNATE);
  AY_BC1::setAlternateFunction(gpio::afr::TIM3_5);
  AY_BC1::setMode(gpio::moder::ALTERNATE);
#endif
  AY_BC2::setMode(gpio::moder::INPUT);
  AY_A8::setMode(gpio::moder::INPUT);
  AY_A9::setMode(gpio::moder::INPUT);
  AY_SEL::setMode(gpio::moder::INPUT);
  AY_RST::setMode(gpio::moder::INPUT);
  AY_RST::setPullMode(gpio::pupdr::PULL_UP);

#ifndef AYCLK_INTERNAL
  AY_CLK::setAlternateFunction(gpio::afr::TIM1_2);
  AY_CLK::setMode(gpio::moder::ALTERNATE);
  AY_CLK::setPullMode(gpio::pupdr::PULL_DOWN);
#endif

  AY_D0::setSpeed(gpio::ospeedr::_25MHZ); // 25MHz speed is best choice - 2MHz gives too slow slew rate, 50MHz is too noisy
  AY_D1::setSpeed(gpio::ospeedr::_25MHZ);
  AY_D2::setSpeed(gpio::ospeedr::_25MHZ);
  AY_D3::setSpeed(gpio::ospeedr::_25MHZ);
  AY_D4::setSpeed(gpio::ospeedr::_25MHZ);
  AY_D5::setSpeed(gpio::ospeedr::_25MHZ);
  AY_D6::setSpeed(gpio::ospeedr::_25MHZ);
  AY_D7::setSpeed(gpio::ospeedr::_25MHZ);

  AY_D0::setMode(gpio::moder::INPUT);
  AY_D1::setMode(gpio::moder::INPUT);
  AY_D2::setMode(gpio::moder::INPUT);
  AY_D3::setMode(gpio::moder::INPUT);
  AY_D4::setMode(gpio::moder::INPUT);
  AY_D5::setMode(gpio::moder::INPUT);
  AY_D6::setMode(gpio::moder::INPUT);
  AY_D7::setMode(gpio::moder::INPUT);

  AY_IOA0::setMode(gpio::moder::INPUT);
  AY_IOA1::setMode(gpio::moder::INPUT);
  AY_IOA2::setMode(gpio::moder::INPUT);
  AY_IOA3::setMode(gpio::moder::INPUT);
  AY_IOA4::setMode(gpio::moder::INPUT);
  AY_IOA5::setMode(gpio::moder::INPUT);
  AY_IOA6::setMode(gpio::moder::INPUT);
  AY_IOA7::setMode(gpio::moder::INPUT);

  // Console UART
  U_RX::setAlternateFunction(gpio::afr::USART1_3);
  U_RX::setMode(gpio::moder::ALTERNATE);
  U_RX::setPullMode(gpio::pupdr::PULL_UP);

  U_TX::setAlternateFunction(gpio::afr::USART1_3);
  U_TX::setMode(gpio::moder::ALTERNATE);
  U_TX::setSpeed(gpio::ospeedr::_2MHZ);

  // DAC
  AU_L::setMode(gpio::moder::ANALOG);
  AU_R::setMode(gpio::moder::ANALOG);
}

void initializeUART()
{
  UART_CONSOLE::enableClock();
  UART_CONSOLE::setBaudRate<UART_CONSOLE_BAUD_RATE>();
  UART_CONSOLE::configure(
    usart::cr1::rwu::RECEIVER_IN_ACTIVE_MODE,
    usart::cr1::re::RECEIVER_ENABLED,
    usart::cr1::te::TRANSMITTER_ENABLED,
    usart::cr1::idleie::IDLE_INTERRUPT_DISABLED,
    usart::cr1::rxneie::RXNE_ORE_INTERRUPT_ENABLED,
    usart::cr1::tcie::TC_INTERRUPT_DISABLED,
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
    usart::cr3::dmar::RECEIVER_DMA_DISABLED,
    usart::cr3::dmat::TRANSMITTER_DMA_DISABLED,
    usart::cr3::rtse::RTS_HARDWARE_FLOW_DISABLED,
    usart::cr3::ctse::CTS_HARDWARE_FLOW_DISABLED,
    usart::cr3::ctsie::CTS_INTERRUPT_DISABLED,
    usart::cr3::onebit::THREE_SAMPLE_BIT_METHOD);
}

#ifndef BOOT
void initializeDAC()
{
  DAC::enableClock();
  DAC::configureBasic(
    dac::cr::dac1::enable::ENABLED,
    dac::cr::dac1::boff::ENABLED,
    dac::cr::dac2::enable::ENABLED,
    dac::cr::dac2::boff::ENABLED);
}

void initializeAudioDMA()
{
  AU_DMA::enableClock();
  AU_DMA::configure(
    dma::stream::cr::dmeie::DIRECT_MODE_ERROR_INTERRUPT_DISABLED,
    dma::stream::cr::teie::TRANSFER_ERROR_INTERRUPT_DISABLED,
    dma::stream::cr::htie::HALF_TRANSFER_INTERRUPT_DISABLED,
    dma::stream::cr::tcie::TRANSFER_COMPLETE_INTERRUPT_ENABLED,
    dma::stream::cr::pfctrl::DMA_FLOW_CONTROLLER,
    // dma::stream::cr::pfctrl::PERIPHERAL_FLOW_CONTROLLER,
    dma::stream::cr::dir::MEMORY_TO_PERIPHERAL,
    dma::stream::cr::circ::CIRCULAR_MODE_ENABLED,
    dma::stream::cr::pinc::PERIPHERAL_INCREMENT_MODE_DISABLED,
    dma::stream::cr::minc::MEMORY_INCREMENT_MODE_ENABLED,
    dma::stream::cr::psize::PERIPHERAL_SIZE_32BITS,
    dma::stream::cr::msize::MEMORY_SIZE_32BITS,
    dma::stream::cr::pincos::PERIPHERAL_INCREMENT_OFFSET_SIZE_32BITS,
    dma::stream::cr::pl::PRIORITY_LEVEL_VERY_HIGH,
    dma::stream::cr::dbm::DOUBLE_BUFFER_MODE_ENABLED,
    dma::stream::cr::ct::CURRENT_TARGET_MEMORY_0,
    dma::stream::cr::pburst::PERIPHERAL_BURST_TRANSFER_SINGLE,
    dma::stream::cr::mburst::MEMORY_BURST_TRANSFER_SINGLE,
    dma::stream::cr::chsel::AU_DMA_CH);
  AU_DMA::configureFIFO(
    dma::stream::fcr::fth::FIFO_THRESHOLD_SELECTION_FULL,
    dma::stream::fcr::dmdis::DIRECT_MODE_DISABLED,
    dma::stream::fcr::feie::FIFO_ERROR_INTERRUPT_DISABLED);
  AU_DMA::setPeripheralAddress(&DAC_REGS->DHR12LD);   // dual left-aligned 12 bit
  AU_DMA::setMemory0Address(snd::dac_buf[0]);
  AU_DMA::setMemory1Address(snd::dac_buf[1]);
  AU_DMA::setNumberOfTransactions(DAC_SAMPLES_COUNT);
  AU_DMA::enablePeripheral();
}

void initializeAYCLK()
{
#ifdef AYCLK_INTERNAL
  // To do: add TIMx_CR1->ARPE->1: TIMx_ARR register is buffered - to avoid deranges while changing sampling rate
  AU_TIM::enableClock();
  AU_TIM::setMaxResolution();
  AU_TIM::setAutoReload(AU_TIM::FREQUENCY * 8 / PSG_CLK - 1);   // 218.75kHz
  AU_TIM::enableUpdateDma();

  EVT_TIM::enableClock();
  EVT_TIM::configureCounter(
    tim::cr1::cen::COUNTER_DISABLED,
    tim::cr1::udis::UPDATE_EVENT_ENABLED,
    tim::cr1::urs::UPDATE_REQUEST_SOURCE_OVERFLOW_UNDERFLOW,
    tim::cr1::opm::DONT_STOP_COUNTER_AT_NEXT_UPDATE_EVENT,
    tim::cr1::arpe::AUTO_RELOAD_UNBUFFERED);
  EVT_TIM::setPrescaler(EVT_TIM::FREQUENCY * 8 / PSG_CLK - 1);
  EVT_TIM::setAutoReload(65535);
  EVT_TIM::generateUpdate();
  EVT_TIM::clearUpdateFlag();
#else
  // TIM2::enableClock();
  // TIM3::enableClock();
  // REGn_SET_FLD(TIM, 2, SMCR, TS, 6);          // select TI2FP2 for input
  // REGn_SET_FLD(TIM, 2, SMCR, SMS, 7);         // set external clock source
  // REGn_SET_FLD(TIM, 2, CCMR1, IC2F, 2);       // set filter value for TI2
  // REGn_SET_BIT(TIM, 2, DIER, UIE, 1);         // enable interrupts
  // TIM2->ARR = 15;                             // set auto reload value to 1.75MHz / 16 = 109.375kHz
  // TIM2->ARR = 7;                              // set auto reload value to 1.75MHz / 8 (for YM2149F)
  // REGn_SET_BIT(TIM, 2, CR1, CEN, 1);          // enable counter
#endif
}
#endif

void initializeAYBUS()
{
#ifdef AYBUS_USES_EXTI
  SYSCFG::enableClock();
  SYSCFG::selectExtiPin<BDIR_PIN, BDIR_PORT>();
  SYSCFG::selectExtiPin<BC1_PIN, BC1_PORT>();
  BDIR::enableHardwareInterruptByRisingEdge();
  BC1::enableHardwareInterruptByRisingEdge();
#else
  // TIM3_REGS->CCMR2 = tim::ccmr2::cc3s::INPUT_IC3_TI3  // select TIM3 channel TI3 for Capture input
                   // | tim::ccmr2::cc4s::INPUT_IC4_TI4  // select TIM3 channel TI4 for Capture input
                   // | tim::ccmr2::ic3f::FCK_INT_N2     // set filter
                   // | tim::ccmr2::ic4f::FCK_INT_N2;
  // TIM3_REGS->CCER  = tim::ccer::cc3e::ENABLED         // enable Capture inputs
                   // | tim::ccer::cc4e::ENABLED;
  // TIM3_REGS->DIER  = tim::dier::cc3ie::ENABLED        // enable interrupts
                   // | tim::dier::cc4ie::ENABLED;
#endif
  SYSCFG::selectExtiPin<RST_PIN, RST_PORT>();
  RST::enableHardwareInterruptByFallingEdge();
}

void initializePeripherals()
{
  initializePins();
  initializeUART();
#ifndef BOOT
  initializeDAC();
  initializeAudioDMA();
  initializeAYCLK();
#endif
  initializeAYBUS();
}
