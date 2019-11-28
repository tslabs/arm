
// Console interrupts
//
// AY-ARM project
// (c) TS-Labs
//
// 당신 정말 좋아 나 몰라

void processRecv()
{
  UART_CONSOLE::clearRXNE();
  console_uart_in.put_byte(UART_CONSOLE_REGS->DR);
}

void processSend()
{
  if (console_uart_out.used())
    UART_CONSOLE_REGS->DR = console_uart_out.get_byte_nocheck();
  else
    UART_CONSOLE::disableTXEIE();
}

void processUART()
{
  u32 sr = UART_CONSOLE_REGS->SR;

  if (sr & usart::sr::rxne::DATA_RECEIVED)
    processRecv();

  if (sr & usart::sr::txe::DATA_TRANSFERED_TO_THE_SHIFT_REGISTER)
    processSend();
}
