
namespace text
{
  template <void (*OUTPUT)(u8 d)>
  class Functions
  {
    public:
      static void print_dec(u32 num);
      static void print_dec(int num, int p, char filler);
      static void print_hex(u64 hex);
      static void print_hex(u64 num, int p, char filler);
      static void print_hex_str(u8 *data, int n);
      static void print_hex_str_s(u8 *data, int n);
      static void print_hex_digit(u8 hex);
      static void print_str(const char *str);
      static void print_str(const char *str, int n);

    private:
      Functions();
  };

  /// - Number print functions ---
  template <void (*OUTPUT)(u8 d)>
  void Functions <OUTPUT>::print_dec(u32 num)
  {
    Functions<OUTPUT>::print_dec(num, 10, 0);
  }

  // p - number of digits
  template <void (*OUTPUT)(u8 d)>
  void Functions <OUTPUT>::print_dec(int num, int p, char filler)
  {
    bool pre = false;

    for (int i = pow(10, p - 1); i; i /= 10)
    {
      if ((num >= i) || pre || (i == 1))
      {
        OUTPUT((num / i) + '0');
        num %= i;
        pre = true;
      }
      else if (filler)
        OUTPUT(filler);
    }
  }

  template <void (*OUTPUT)(u8 d)>
  void Functions <OUTPUT>::print_hex(u64 hex)
  {
    Functions <OUTPUT>::print_hex(hex, 16, 0);
  }

  template <void (*OUTPUT)(u8 d)>
  void Functions <OUTPUT>::print_hex(u64 num, int p, char filler)
  {
    bool pre = false;

    for (u8 i = p; i ; i--)
    {
      u8 hex = (num >> (((i - 1)) * 4)) & 0xF;

      if (hex || pre || (i == 1))
      {
        Functions <OUTPUT>::print_hex_digit(hex);
        pre = true;
      }
      else if (filler)
        OUTPUT(filler);
    }
  }

  template <void (*OUTPUT)(u8 d)>
  void Functions <OUTPUT>::print_hex_str(u8 *data, int n)
  {
    while(n--)
      Functions <OUTPUT>::print_hex(*data++, 2, '0');
  }

  template <void (*OUTPUT)(u8 d)>
  void Functions <OUTPUT>::print_hex_str_s(u8 *data, int n)
  {
    while(n--)
    {
      Functions <OUTPUT>::print_hex(*data++, 2, '0');
      OUTPUT(' ');
    }
  }

  template <void (*OUTPUT)(u8 d)>
  void Functions <OUTPUT>::print_hex_digit(u8 hex)
  {
    OUTPUT((hex > 9) ? (hex + 'A' - 10) : (hex + '0'));
  }

  template <void (*OUTPUT)(u8 d)>
  void Functions <OUTPUT>::print_str(const char *str)
  {
    u8 c;
    while (c = *str++) OUTPUT(c);
  }

  template <void (*OUTPUT)(u8 d)>
  void Functions <OUTPUT>::print_str(const char *str, int n)
  {
    while (n--) OUTPUT(*str++);
  }

  /// - Number parse functions ---
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

  bool parse_hex(u64 &sum, u8 *ptr_in, int n)
  {
    sum = 0;

    while (n--)
    {
      u8 c;
      if ((c = parse_hex_digit(*ptr_in++)) & 0x80) return false;
      sum = (sum << 4) + c;
    }

    return true;
  }

  bool parse_hex_string(u8 *ptr_in, u8 *ptr_out, int n)
  {
    u8 i, h;

    while (n--)
    {
      if ((i = parse_hex_digit(*ptr_in++)) & 0x80) return false;
      h = i << 4;
      if ((i = parse_hex_digit(*ptr_in++)) & 0x80) return false;
      *ptr_out++ = h | i;
    }

    return true;
  }

  u8 parse_dec_digit(u8 h)
  {
    if ((h >= '0') && (h <= '9'))
      return h - '0';
    else
      return 0x80;
  }

  bool parse_dec(u32 &sum, u8 *ptr_in, int n)
  {
    sum = 0;

    while (n--)
    {
      u8 c;
      if ((c = parse_dec_digit(*ptr_in++)) & 0x80) return false;
      sum = sum * 10 + c;
    }

    return true;
  }
}
