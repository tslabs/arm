
bool is_sending;        // indicates that UART output is in progress

void processRecv()
{
  UART_CONSOLE::clearRXNE();
  console_uart_in.put_byte(UART_CONSOLE_REGS->DR);
}

void processSend()
{
  UART_CONSOLE::clearTC();
  if (console_uart_out.used())
  {
    is_sending = true;
    UART_CONSOLE_REGS->DR = console_uart_out.get_byte_nocheck();
  }
  else
    is_sending = false;
}

void processUART()
{
  u32 sr = UART_CONSOLE_REGS->SR;

  if (sr & usart::sr::rxne::DATA_RECEIVED)
    processRecv();

  if (sr & usart::sr::tc::TRANSMISSION_COMPLETED)
    processSend();
}
