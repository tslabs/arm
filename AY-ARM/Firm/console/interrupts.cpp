
// - Interrupts -------------------------
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
    UART_CONSOLE_REGS->DR = console_uart_out.get_byte_unsafe();
  }
  else
    is_sending = false;
}
