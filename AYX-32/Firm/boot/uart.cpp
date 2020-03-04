
// Boot UART
//
// AYX-32 project
// (c) TS-Labs
//
// Merede tuules on päikest ja liiva nad ootavad vaid kui me jõuame sinna

enum UART_STATE
{
  ST_IDLE,
  ST_SYNC,
  ST_LOAD,
  ST_NONE
};

enum UART_ERR
{
  UE_OK    = 0x00,
  UE_SYNC  = 0x01,
  UE_DATA  = 0x02,
  UE_FLASH = 0x03,
};

volatile UART_STATE st;
u32 st_cnt;
u8 *st_addr;

struct
{
  u32 magic;
  u32 size;
} hdr;

void initialize()
{
  uart_in.init(uart_inbuf, sizeof(uart_inbuf));
  uart_out.init(uart_outbuf, sizeof(uart_outbuf));
  st = ST_IDLE;
}

void processRecv()
{
  UART_CONSOLE::clearRXNE();
  uart_in.put_byte(UART_CONSOLE_REGS->DR);
}

void processSend()
{
  if (uart_out.used())
    UART_CONSOLE_REGS->DR = uart_out.get_byte_nocheck();
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

void output_err(u32 err)
{
  uart_out.put_byte(err);
  uart_out.put_byte(0x55);
  uart_out.put_byte(0xAA);
  uart_out.put_byte(0x78);

  UART_CONSOLE::enableTXEIE();
}

void bg_fw()
{
  if (!flasher::check_fw())
  {
    output_err(UE_DATA);
    st = ST_IDLE;
  }
  else
  {
    output_err(UE_OK);
    wait_ms(10);

    if (!flasher::flash_fw())
      output_err(UE_FLASH);
    else
      output_err(UE_OK);

    st = ST_IDLE;
  }
}

void input_byte(u8 data)
{
  switch (st)
  {
    case ST_IDLE:
      if (data == 0x5A)
      {
        output_err(UE_OK);

        st_addr = (u8*)&hdr;
        st_cnt = 0;
        st = ST_SYNC;
      }
    break;

    case ST_SYNC:
      *st_addr++ = data;
      st_cnt++;

      if (st_cnt == sizeof(hdr))
      {
        if ((hdr.magic != bus::MAGIC_FFW) || !hdr.size || (hdr.size > sizeof(fw_buf)))
        {
          output_err(UE_SYNC);
          st = ST_IDLE;
        }
        else
        {
          output_err(UE_OK);

          st_addr = fw_buf;
          st_cnt = 0;
          st = ST_LOAD;
        }
      }
    break;

    case ST_LOAD:
      *st_addr++ = data;
      st_cnt++;

      if (st_cnt == hdr.size)
      {
        set_bg_task(bg_fw);
        st = ST_NONE;
      }
    break;
  }
}
