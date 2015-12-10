
// Hardware
//
// AY-ARM project
// (c)2015 TS-Labs
//
// Degoša lāpa būs mūsu saule

void initializePins()
{
  // GPIO
  GPIOA::enableClock();
  GPIOB::enableClock();
  DAC::enableClock();
  TIM2::enableClock();
  TIM3::enableClock();
  TIM6::enableClock();

  // AY Bus
  AY_CLK::setAlternateFunction(gpio::afr::TIM1_2);
  AY_CLK::setMode(gpio::moder::ALTERNATE);
  AY_CLK::setSpeed(gpio::ospeedr::_2MHZ);

  AY_BDIR::setAlternateFunction(gpio::afr::TIM3_5);
  AY_BDIR::setMode(gpio::moder::ALTERNATE);
  AY_BDIR::setSpeed(gpio::ospeedr::_2MHZ);

  AY_BC1::setAlternateFunction(gpio::afr::TIM3_5);
  AY_BC1::setMode(gpio::moder::ALTERNATE);
  AY_BC1::setSpeed(gpio::ospeedr::_2MHZ);

  AY_BC2::setMode(gpio::moder::INPUT);
  AY_BC2::setSpeed(gpio::ospeedr::_2MHZ);

  AY_A8::setMode(gpio::moder::INPUT);
  AY_A8::setSpeed(gpio::ospeedr::_2MHZ);

  AY_A9::setMode(gpio::moder::INPUT);
  AY_A9::setSpeed(gpio::ospeedr::_2MHZ);

  AY_RST::setMode(gpio::moder::INPUT);
  AY_RST::setSpeed(gpio::ospeedr::_2MHZ);

  AY_SEL::setMode(gpio::moder::INPUT);
  AY_SEL::setSpeed(gpio::ospeedr::_2MHZ);

  AY_D0::setSpeed(gpio::ospeedr::_100MHZ);
  AY_D1::setSpeed(gpio::ospeedr::_100MHZ);
  AY_D2::setSpeed(gpio::ospeedr::_100MHZ);
  AY_D3::setSpeed(gpio::ospeedr::_100MHZ);
  AY_D4::setSpeed(gpio::ospeedr::_100MHZ);
  AY_D5::setSpeed(gpio::ospeedr::_100MHZ);
  AY_D6::setSpeed(gpio::ospeedr::_100MHZ);
  AY_D7::setSpeed(gpio::ospeedr::_100MHZ);

  AY_D0::setMode(gpio::moder::INPUT);
  AY_D1::setMode(gpio::moder::INPUT);
  AY_D2::setMode(gpio::moder::INPUT);
  AY_D3::setMode(gpio::moder::INPUT);
  AY_D4::setMode(gpio::moder::INPUT);
  AY_D5::setMode(gpio::moder::INPUT);
  AY_D6::setMode(gpio::moder::INPUT);
  AY_D7::setMode(gpio::moder::INPUT);

  // AY_CLK
  // REGn_SET_FLD(TIM, 2, SMCR, TS, 6);          // select TI2FP2 for input
  // REGn_SET_FLD(TIM, 2, SMCR, SMS, 7);         // set external clock source
  // REGn_SET_FLD(TIM, 2, CCMR1, IC2F, 2);       // set filter value for TI2
  // REGn_SET_BIT(TIM, 2, DIER, UIE, 1);         // enable interrupts
  // TIM2->ARR = 15;                             // set auto reload value to 1.75MHz / 16 = 109.375kHz
  // TIM2->ARR = 7;                              // set auto reload value to 1.75MHz / 8 (for YM2149F)
  // REGn_SET_BIT(TIM, 2, CR1, CEN, 1);          // enable counter

  // BC1, BDIR
  // TIM3_REGS->CCMR2 = tim::ccmr2::cc3s::INPUT_IC3_TI3  // select TIM3 channel TI3 for Capture input
                   // | tim::ccmr2::cc4s::INPUT_IC4_TI4  // select TIM3 channel TI4 for Capture input
                   // | tim::ccmr2::ic3f::FCK_INT_N2     // set filter
                   // | tim::ccmr2::ic4f::FCK_INT_N2;
  // TIM3_REGS->CCER  = tim::ccer::cc3e::ENABLED         // enable Capture inputs
                   // | tim::ccer::cc4e::ENABLED;
  // TIM3_REGS->DIER  = tim::dier::cc3ie::ENABLED        // enable interrupts
                   // | tim::dier::cc4ie::ENABLED;

  // Console UART
  U_RX::setAlternateFunction(gpio::afr::USART1_3);
  U_RX::setMode(gpio::moder::ALTERNATE);
  U_RX::setPullMode(gpio::pupdr::PULL_UP);
  U_RX::setSpeed(gpio::ospeedr::_2MHZ);
  U_TX::setAlternateFunction(gpio::afr::USART1_3);
  U_TX::setMode(gpio::moder::ALTERNATE);
  U_TX::setSpeed(gpio::ospeedr::_2MHZ);

  UART_CONSOLE::enableClock();
  UART_CONSOLE::setBaudRate<UART_CONSOLE_BAUD_RATE>();
  UART_CONSOLE::configure(
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
    usart::cr3::dmar::RECEIVER_DMA_DISABLED,
    usart::cr3::dmat::TRANSMITTER_DMA_DISABLED,
    usart::cr3::rtse::RTS_HARDWARE_FLOW_DISABLED,
    usart::cr3::ctse::CTS_HARDWARE_FLOW_DISABLED,
    usart::cr3::ctsie::CTS_INTERRUPT_DISABLED,
    usart::cr3::onebit::THREE_SAMPLE_BIT_METHOD);

  // DAC
  AU_L::setMode(gpio::moder::ANALOG);
  AU_R::setMode(gpio::moder::ANALOG);

  DAC::enablePeripheral();
  DAC_REGS->DHR12RD = 0;
}

void initializePeripherals()
{
  initializePins();
}
