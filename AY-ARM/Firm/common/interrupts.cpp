
namespace exception
{
  extern "C"
  {
    void SysTick()
    {
    }
  }
}

namespace interrupt
{
  // Console
  void USART1()
  {
    u32 sr = UART_CONSOLE_REGS->SR;

    if (sr & usart::sr::rxne::DATA_RECEIVED)
      console::processRecv();

    if (sr & usart::sr::tc::TRANSMISSION_COMPLETED)
      console::processSend();
  }
};

void initializeSystick()
{
  SYSTICK::configure<
    SYSTICK_PERIOD,
    stk::cr::clksource::EXTERNAL_CLOCK,
    stk::cr::tickint::TICKINT_ENABLED,
    stk::cr::enable::SYSTICK_ENABLED>();
}

void initializeInterrupts()
{
  SCB::setPriority<scb::irqn::SysTick, SYSTICK_PRIORITY>();
  NVIC::setPriority<nvic::irqn::USART1, UART_CONSOLE_PRIORITY>();
  initializeSystick();
  NVIC::enableIrq<nvic::irqn::USART1>();
}
