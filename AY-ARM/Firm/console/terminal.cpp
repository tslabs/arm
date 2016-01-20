
#include "terminal.h"

/* UART input modes */
enum UIM
{
  UARTIM_NUL = 0,
  UARTIM_EDIT,    // console editor
  UARTIM_KEY      // console key input
};

/* KB input modes */
enum
{
    KB_OFF = 0,
    KB_EDIT,
    KB_KEY
};

// allow debug messages output
enum DBG_MODE
{
  DBG_OFF,
  DBG_ON
};

// --- Vars ---
UIM uart_input_mode;
u8 kb_max;              // Max number of chars allowed for input
u8 kb_cnt;              // number of chars typed
u8 kb_done;             // Keyboard buffer has a data ready to process
u8 uart_inbuf[256];     // buffer for keyboard input

DBG_MODE dbg_mode;

// --- Prototypes ---
void print(const char*, ...);
void print_debug(const char*, ...);
void send_uart_byte(u8);
void send_uart(u8*, u8);
void set_color(u8);
void set_x(u8);
void set_xy(u8, u8);
void print_msg(u8);
void process_UART_input(void);
void process_UART_output(void);
void print_dec(int);
void print_dec(int, int, char);
void cr();
void tab();

// --- Low level ---
void send_uart_byte(u8 data)
{
  console_uart_out.put_byte(data);
}

// sends a fixed length string
void send_uart(u8 *str, u8 n)
{
  while (n--)
    send_uart_byte(*str++);
}

// --- Low level ---
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

// --- Terminal control functions ---
void set_color(u8 col)
{
  print(_A_COL);
  send_uart_byte(col + '0');
  send_uart_byte('m');
}

void set_x(u8 x)
{
  print(_A_CSI);
  print_dec(x);
  send_uart_byte('G');
}

void set_xy(u8 x, u8 y)
{
  print(_A_CSI);
  print_dec(y);
  send_uart_byte(';');
  print_dec(x);
  send_uart_byte('H');
}

void clear(u8 n)
{
  u8 i = n;
  while (i--) send_uart_byte(' ');
  while (n--) send_uart_byte('\b');
}

void print_line(u8 n)
{
  u8 i;
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

// --- Number print functions ---
void print_hex_digit(u8 hex)
{
  send_uart_byte((hex > 9) ? (hex + 'A' - 10) : (hex + '0'));
}

void print_hex(u8 hex)
{
  print_hex_digit((hex & 0xF0) >> 4);
  print_hex_digit(hex & 0x0F);
}

void print_hex(u16 hex)
{
  print_hex((u8)(hex >> 8));
  print_hex((u8)hex);
}

void print_hex(u32 hex)
{
  print_hex((u16)(hex >> 16));
  print_hex((u16)hex);
}

void print_hex_str(u8 *data, u8 n)
{
  while(n--)
    print_hex(*data++);
}

void print_hex_str_s(u8 *data, u8 n)
{
  while(n--)
  {
    print_hex(*data++);
    send_uart_byte(' ');
  }
}

void print_dec(int num)
{
  bool pre = false;

  for (int i = 1000000000; i; i /= 10)
  {
    if ((num >= i) || pre || (i == 1))
    {
      send_uart_byte((num / i) + '0');
      num %= i;
      pre = true;
    }
  }
}

void print_dec(int num, int p, char filler)
{
  bool pre = false;

  for (int i = pow(10, p - 1); i; i /= 10)
  {
    if ((num >= i) || pre || (i == 1))
    {
      send_uart_byte((num / i) + '0');
      num %= i;
      pre = true;
    }
    else
      send_uart_byte(filler);
  }
}

// --- Number tab functions ---
// 16 bit tabulated DEC values in a column
void print_tab_dec16(u16 val)
{
  print(_CR _TAB);
  print_dec(val, 5, ' ');
}

// Numbers zebra
void print_num_zebra(u8 beg, u8 inc, u8 num, u8 x, u8 step)
{
  u8 i = 0;
  while (num--)
  {
    print_char((i++ & 1) ? __BLU : __BBLU);
    set_x(x);
    print_hex_digit(beg);
    beg += inc;
    x += step;
  }
}

// 16 bit tabulated DEC values in a row
void print_row_dec(u8 num, u8 x, u8 step, u16 *arr)
{
  print_char(__YLW);

  u8 i = 0;
  while (num--)
  {
    set_x(x);
    print_dec(arr[i++], 5, ' ');
    x += step;
  }
}

// 32 bit tabulated DEC values in a row
void print_row_dec(u8 num, u8 x, u8 step, u32 *arr)
{
  print_char(__YLW);

  u8 i = 0;
  while (num--)
  {
    set_x(x);
    print_dec(arr[i++], 7, ' ');
    x += step;
  }
}

// --- Number parse functions ---
u8 parse_hex_digit(u8 h)
{
  if ((h >= '0') && (h <= '9'))
    return h - '0';
  else if ((h >= 'A') && (h <= 'F'))
    return h - 'A' + 10;
  else if ((h >= 'a') && (h <= 'f'))
    return h - 'a' + 10;
  else
    return 0x80;
}

u8 parse_hex_string(volatile u8 *ptr_in, u8 *ptr_out, u8 n)
{
  u8 i, h;

  while (n--)
  {
    if ((i = parse_hex_digit(*ptr_in++)) & 0x80) return FALSE;
    h = i << 4;
    if ((i = parse_hex_digit(*ptr_in++)) & 0x80) return FALSE;
    *ptr_out++ = h | i;
  }

  return TRUE;
}

u8 parse_dec_digit(u8 h)
{
  if ((h >= '0') && (h <= '9'))
    return h - '0';
  else
    return 0x80;
}

u8 parse_dec32(volatile u8 *ptr_in, u8 *ptr_out, u8 n, u8 m)
{
  u32 sum = 0;
  u8 c;

  while (n--)
  {
    if ((c = parse_dec_digit(*ptr_in++)) & 0x80)
      return FALSE;

    sum = sum * 10 + c;
  }

  while (m--)
  {
    *ptr_out++ = lo8(sum);
    sum >>= 8;
  }

  return TRUE;
}

// --- Input ---
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

// --- Print functions ---

// formatted print
void vprint(const char *str, ...)
{
  va_list args;

  while (u8 c = *str++)
  {
    if (c == '%')
      switch (*str++)
      {
        /* null terminated string */
        case 's':
        {
          u8 c;
          const char *str = va_arg(args, const char*);
          while (c = *str++) print_char(c);
        }
        break;

        /* fixed length string */
        case 'S':
        {
          const char *str = va_arg(args, const char*);
          int cnt = va_arg(args, int);
          while (cnt--) print_char(*str++);
        }
        break;

        /* fixed length hex string */
        case 'H':
        {
          const char *str = va_arg(args, const char*);
          int cnt = va_arg(args, int);
          print_hex_str_s((u8*)str, cnt);
        }
        break;

        /* DEC number */
        case 'd':
          print_dec((int)va_arg(args, int));
        break;

        /* HEX8 number */
        case 'x':
          print_hex((u8)va_arg(args, int));
        break;

        /* HEX16 number */
        case 'X':
          print_hex((u16)va_arg(args, int));
        break;

        /* HEX32 number */
        case 'Y':
          print_hex((u32)va_arg(args, int));
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
  print("\n");
  vprint(str, args);
}
