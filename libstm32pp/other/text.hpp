
namespace text
{
  template <void (*OUTPUT)(u8 d)>
  class Functions
  {
    public:
      static void print_dec(int num);
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
  void Functions <OUTPUT>::print_dec(int num)
  {
    if (num < 0)
    {
      OUTPUT('-');
      num = -num;
    }
    
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

  int parse_hex(u8 *ptr_in)
  {
    int sum = 0;

    for (int a = 0; a < 10; a++)
    {
      u8 c;
      if ((c = parse_hex_digit(ptr_in[a])) & 0x80) break;
      sum = (sum << 4) + c;
    }

    return sum;
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

  int parse_dec(u8 *ptr_in)
  {
    int sum = 0;

    for (int a = 0; a < 8; a++)
    {
      u8 c;
      if ((c = parse_dec_digit(ptr_in[a])) & 0x80) break;
      sum = sum * 10 + c;
    }

    return sum;
  }

  /// - String functions ---
  // compare 0-terminated string
  bool compare_str(const void *s1, const void *s2)
  {
    u8 *p1 = (u8*)s1;
    u8 *p2 = (u8*)s2;

    for (int a = 0;; a++)
    {
      // 0-marker of string1 reached, comparison success
      if (!p1[a])
        return true;

      // premature buffer end, comparison fail
      if (!p2[a])
        return false;

      // symbol of string1 doesn't match symbol of string2, comparison fail
      if (p1[a] != p2[a])
        return false;
    }
  }

  // compare fixed length strings
  bool compare_str(const void *s1, const void *s2, int n)
  {
    u8 *p1 = (u8*)s1;
    u8 *p2 = (u8*)s2;

    for (int a = 0; a < n; a++)
    {
      // symbol of string doesn't match symbol in buffer, comparison fail
      if (p1[a] != p2[a])
        return false;
    }

    return true;
  }

  // determine string length (without terminating 0)
  int str_len(const void *s1)
  {
    u8 *p1 = (u8*)s1;

    for (int a = 0;; a++)
    {
      if (!p1[a])
        return a;
    }
  }

  // find 0-terminated string, returns index in string2, where string1 begins
  int find_str(const void *s1, const void *s2, int n)
  {
    u8 *p1 = (u8*)s1;
    u8 *p2 = (u8*)s2;

    for (int a = 0; a < n; a++)
    {
      if (compare_str(p1, &p2[a]))
        return a;
    }

    return -1;
  }

  // find 0-terminated string, returns index in string2, following string1
  int find_str_e(const void *s1, const void *s2, int n)
  {
    return find_str(s1, s2, n) + str_len(s1);
  }
}
