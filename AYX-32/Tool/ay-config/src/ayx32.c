
// AYX-32 functions
void wr_addr(u8 v) __naked
{
  v;  // to avoid warning
  __asm
    ld hl, #2
    add hl, sp
    ld a, (hl)
    ld bc, #0xFFFD
    out (c), a
    ret
  __endasm;
}

void wr_reg(u8 v) __naked
{
  v;  // to avoid warning
  __asm
    ld hl, #2
    add hl, sp
    ld a, (hl)
    ld bc, #0xBFFD
    out (c), a
    ret
  __endasm;
}

void wr_reg8(u8 reg, u8 data)
{
  wr_addr(reg);
  wr_reg(data);
}

void wr_reg32(u8 reg, u32 data)
{
  wr_addr(reg);
  wr_reg((u8)data);
  wr_reg((u8)(data >> 8));
  wr_reg((u8)(data >> 16));
  wr_reg((u8)(data >> 24));
}

#define CHUNK_SIZE 64

void wr_data_chunk(u8 *v) __naked
{
  v;  // to avoid warning
  __asm
    ld hl, #2
    add hl, sp
    ld e, (hl)
    inc hl
    ld d, (hl)
    ex de, hl
    ld bc, #0xC0FD
    outi  // 64 times: no DUP/EDUP in fucking SDCC...
    outi
    outi
    outi
    outi
    outi
    outi
    outi
    outi
    outi
    outi
    outi
    outi
    outi
    outi
    outi
    outi
    outi
    outi
    outi
    outi
    outi
    outi
    outi
    outi
    outi
    outi
    outi
    outi
    outi
    outi
    outi
    outi
    outi
    outi
    outi
    outi
    outi
    outi
    outi
    outi
    outi
    outi
    outi
    outi
    outi
    outi
    outi
    outi
    outi
    outi
    outi
    outi
    outi
    outi
    outi
    outi
    outi
    outi
    outi
    outi
    outi
    outi
    outi
    ret
  __endasm;
}

void wr_data_arr(u8 *ptr, u16 size)
{
  wr_addr(R_DATA);

  while (size >= CHUNK_SIZE)
  {
    wr_data_chunk(ptr);
    ptr += CHUNK_SIZE;
    size -= CHUNK_SIZE;
  }
  
  while (size--)
    wr_reg(*ptr++);
}

u8 rd_reg() __naked
{
  __asm
    ld bc, #0xFFFD
    in l, (c)
    ret
  __endasm;
}

u8 rd_reg8(u8 reg)
{
  wr_addr(reg);
  return rd_reg();
}

u16 rd_reg16(u8 reg)
{
  u16 rc;
  wr_addr(reg);
  rc = rd_reg();
  rc |= rd_reg() << 8;
  return rc;
}

u32 rd_reg32(u8 reg)
{
  u32 rc;
  wr_addr(reg);
  rc  = (u32)rd_reg();
  rc |= (u32)rd_reg() << 8;
  rc |= (u32)rd_reg() << 16;
  rc |= (u32)rd_reg() << 24;
  return rc;
}

void rd_reg_arr(u8 reg, u8 *ptr, u16 size)
{
  wr_addr(reg);

  while (size--)
    *ptr++ = rd_reg();
}

void rd_reg_str(u8 reg, u8 *str, u8 n)
{
  u8 d;

  wr_addr(reg);

  while (--n)
  {
    d = rd_reg();
    if (!d) break;
    *str++ = d;
  }

  *str = 0;
}

void wait_busy()
{
  while (rd_reg8(R_STATUS) & S_BUSY);
}
