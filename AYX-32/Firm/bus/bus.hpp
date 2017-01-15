
// AYX-32 bus header
//
// AYX-32 project
// (c) TS-Labs
//
// ...

/// Types
typedef void (*V_VEC)();
typedef void (*W_VEC)(u8);
typedef u8 (*R_VEC)();

class PTR
{
public:
  u8 *addr;  // pointer to array
  u32 max;   // max number of bytes
  u32 cnt;   // counter of bytes

  void nul()
  {
    max = 0;
  }

  void init(u8 *a, u32 m)
  {
    addr = a;
    max = m;
    cnt = 0;
  }

  u8 read()
  {
    if (cnt < max)
    {
      cnt++;
      return *addr++;
    }
    else
      return 0xFF;
  }

  void write(u8 d)
  {
    if (cnt < max)
    {
      cnt++;
      *addr++ = d;
    }
  }

  void write_t(u8 d, TASK t)
  {
    if (cnt < max)
    {
      cnt++;
      *addr++ = d;

      if (cnt == max) t();
    }
  }
};

/// Prototypes
void initialize_bus();
void write_address(u8);
void write_reg(u8);
u8 read_reg();

extern const V_VEC write_addr_vec[];
extern const W_VEC write_reg_vec[];
extern const R_VEC read_reg_vec[];
