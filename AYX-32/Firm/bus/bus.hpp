
// AYX-32 bus header
//
// AYX-32 project
// (c) TS-Labs
//
// Iske rauta

/// Types
typedef void (*A_VEC)();
typedef void (*W_VEC)(u8);
typedef u8 (*R_VEC)();
typedef void (*C_VEC)();

#pragma pack(1)
typedef struct
{
  u8 num;
  A_VEC addr;
} T_A_VEC;

typedef struct
{
  u8 num;
  W_VEC addr;
} T_W_VEC;

typedef struct
{
  u8 num;
  R_VEC addr;
} T_R_VEC;

typedef struct
{
  u8 num;
  C_VEC addr;
} T_C_VEC;
#pragma pack()

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

/// Variables
A_VEC write_addr_vec[256];
W_VEC write_reg_vec[256];
R_VEC read_reg_vec[256];
C_VEC command_vec[256];

u8 regnum;        // currently selected chip register
STATUS status;    // status byte
ERR error;        // error code

TASK nx_task;               // next task in command

u8 param[8];    // command parameters
u8 resp[8];     // command response

#ifndef BOOT
u8 psg_readback[PSG_CHIPS_MAX][32]; // readback copy of PSG registers
u8 psg_readback_sel;                // selected PSG chip for readback
u8 readback[256];                   // readback copy of misc registers
#endif

PTR r_ptr;
PTR w_ptr;
PTR rd_ptr;
PTR wd_ptr;

u32 temp_32;
u16 temp_16;

/// Prototypes
void initialize_bus();
void write_address(u8);
void write_reg(u8);
u8 read_reg();
void init_vectors();
void init_vectors_ext();

void wa_empty() {}
void wr_empty(u8 val) {}
u8 rr_empty_ff() { return 0xFF; }
u8 rr_empty_00() { return 0x00; }
void cm_empty() { error = E_CMDERR; }
