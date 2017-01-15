
/// - Vars ---
UIM uart_input_mode;
u8 kb_max;              // Max number of chars allowed for input
u8 kb_cnt;              // number of chars typed
u8 kb_done;             // Keyboard buffer has a data ready to process
u8 uart_inbuf[256];     // buffer for keyboard input

DBG_MODE dbg_mode;

void send_uart_byte(u8 data)
{
  console_uart_out.put_byte(data);
}

// sends a fixed length string
void send_uart(u8 *str, int n)
{
  while (n--)
    send_uart_byte(*str++);
}

void print_char(u8 data)
{
  enum
  {
    PC_IDLE = 0,
    PC_X1,
    PC_XY1,
    PC_XY2
  };

  static u8 state = PC_IDLE;
  static u8 a;

  switch (state)
  {
    case PC_X1:
      state = PC_IDLE;
      set_x(data);
    break;

    case PC_XY1:
      a = data;
      state = PC_XY2;
    break;

    case PC_XY2:
      state = PC_IDLE;
      set_xy(a, data);
    break;

    default:
    {
      // foreground color
      if ((data >= __BLK) && (data <= __WHT))
      {
        print(_A_BR_OFF);
        set_color(data - __BLK);
      }

      else if ((data >= __BBLK) && (data <= __BWHT))
      {
        print(_A_BR_ON);
        set_color(data - __BBLK);
      }

      // CR,LN
      else if (data == __CR)
        cr();

      // clear tab
      else if (data == __CTAB)
        print(_A_CTAB);

      // cursor position X
      else if (data == __X)
        state = PC_X1;

      // cursor position XY
      else if (data == __XY)
        state = PC_XY1;

      else
        send_uart_byte(data);
    }
  }
}

/// - Terminal control functions ---
void set_color(u8 col)
{
  print(_A_COL);
  send_uart_byte(col + '0');
  send_uart_byte('m');
}

void set_x(u8 x)
{
  print(_A_CSI);
  CONSOLE::print_dec(x);
  send_uart_byte('G');
}

void set_xy(u8 x, u8 y)
{
  print(_A_CSI);
  CONSOLE::print_dec(y);
  send_uart_byte(';');
  CONSOLE::print_dec(x);
  send_uart_byte('H');
}

void clear(int n)
{
  int i = n;
  while (i--) send_uart_byte(' ');
  while (n--) send_uart_byte('\b');
}

void print_line(int n)
{
  while (n--) send_uart_byte('-');
}

void tab()
{
  send_uart_byte('\t');
}

void cr()
{
  send_uart_byte('\r');
  send_uart_byte('\n');
}

/// - Number tab functions ---
// 16 bit tabulated DEC values in a column
void print_tab_dec16(u16 val)
{
  print(_CR _TAB);
  CONSOLE::print_dec(val, 5, ' ');
}

// Numbers zebra
void print_num_zebra(u8 beg, u8 inc, int n, u8 x, u8 step)
{
  u8 i = 0;
  while (n--)
  {
    print_char((i++ & 1) ? __BLU : __BBLU);
    set_x(x);
    CONSOLE::print_hex_digit(beg);
    beg += inc;
    x += step;
  }
}

// 16 bit tabulated DEC values in a row
void print_row_dec(int n, u8 x, u8 step, u16 *arr)
{
  print_char(__YLW);

  int i = 0;
  while (n--)
  {
    set_x(x);
    CONSOLE::print_dec(arr[i++], 5, ' ');
    x += step;
  }
}

// 32 bit tabulated DEC values in a row
void print_row_dec(int n, u8 x, u8 step, u32 *arr)
{
  print_char(__YLW);

  int i = 0;
  while (n--)
  {
    set_x(x);
    CONSOLE::print_dec(arr[i++], 7, ' ');
    x += step;
  }
}

/// Input
void kb_disable()
{
  uart_input_mode = UARTIM_NUL;
}

void kb_enable(u8 sz)
{
  kb_cnt = 0;
  kb_done = false;

  if (sz == 255)
    uart_input_mode = UARTIM_KEY;
  else
  {
    print(_C_ON);   // cursor ON
    uart_input_mode = UARTIM_EDIT;
    kb_max = sz;
  }
}

void wait_enter()
{
  // do
    // wait_input(-1);
      // while (uart_inbuf[0] != '\r');
}

void uart_editor_input(u8 data)
{
  /* pressed Enter */
  if (data == '\r')
  {
    kb_disable();
    print(_C_OFF);  // cursor OFF
    kb_done = true;
  }

  /* pressed Backspace */
  else if ((data == '\b') || (data == 0x7F))
  {
    /* at least 1 char must be in buffer to delete */
    if (kb_cnt)
    {
      /* clear console char */
      send_uart_byte('\b');
      send_uart_byte(' ');
      send_uart_byte('\b');
      kb_cnt--;
    }
  }

  /* check if char is printable */
  else if ((data > 31) && (data < 127))
  {
    /* the number of chars in buffer should not exceed max value or buffer size */
    if ((kb_cnt < kb_max) && (kb_cnt < UART_CONSOLE_INBUF))
    {
      uart_inbuf[kb_cnt++] = data;
      send_uart_byte(data);
    }
  }
}

void uart_key_input(u8 data)
{
  uart_inbuf[0] = data;
  kb_disable();
  kb_done = true;
}

void uart_input(u8 data)
{
  switch (uart_input_mode)
  {
    case UARTIM_EDIT:
      uart_editor_input(data);
    break;

    case UARTIM_KEY:
      uart_key_input(data);
    break;
  }
}

/// - Print functions ---
// formatted print
void vprint(const char *str, ...)
{
  va_list args;

  while (u8 c = *str++)
  {
    if (c == '%')
      switch (*str++)
      {
        /* DEC number */
        case 'd':
          CONSOLE::print_dec((int)va_arg(args, int));
        break;

        /* HEX number */
        case 'h':
          CONSOLE::print_hex(va_arg(args, int));
        break;

        /* HEX8 number */
        case 'x':
          CONSOLE::print_hex(va_arg(args, int), 2, '0');
        break;

        /* HEX16 number */
        case 'X':
          CONSOLE::print_hex(va_arg(args, int), 4, '0');
        break;

        /* HEX32 number */
        case 'y':
          CONSOLE::print_hex(va_arg(args, int), 8, '0');
        break;

        /* fixed length hex string */
        case 'H':
        {
          const char *str = va_arg(args, const char*);
          int cnt = va_arg(args, int);
          CONSOLE::print_hex_str_s((u8*)str, cnt);
        }
        break;

        /* null terminated string */
        case 's':
        {
          const char *str = va_arg(args, const char*);
          CONSOLE::print_str(str);
        }
        break;

        /* fixed length string */
        case 'S':
        {
          const char *str = va_arg(args, const char*);
          int cnt = va_arg(args, int);
          CONSOLE::print_str(str, cnt);
        }
        break;

        case '%':
          print_char('%');
      }
    else
      print_char(c);
  }

  va_end(args);
}

// print
void print(const char *str, ...)
{
  va_list args;
  va_start(args, str);
  vprint(str, args);
}

// print debug
void print_debug(const char *str, ...)
{
  if (dbg_mode != DBG_ON) return;

  va_list args;
  va_start(args, str);
  vprint(str, args);
}
