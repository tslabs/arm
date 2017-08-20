#pragma once

class FIFO
{
public:
  u8 *addr;
  u32 size;
  u32 rdptr;
  u32 wrptr;
  u32 rdcnt;
  u32 wrcnt;
  bool overflow;
  bool underflow;

  inline void init(void*, u32);
  inline void clear();
  inline u32 free();
  inline u32 used();
  inline void put_word_nocheck(u32 x);
  inline void put_byte_nocheck(u8 x);
  inline void put_byte(u8 x);
  inline u8 peek_byte();
  inline u8 get_byte_nocheck();
  inline u8 get_byte();
  inline bool put(void *a, u32 num);
  inline bool get(void *a, u32 num);
};
    
// Init FIFO
void FIFO::init(void *a, u32 s)
{
  addr = (u8*)a;
  size = s;
  clear();
}

// Clear FIFO
void FIFO::clear()
{
  rdptr = wrptr = 0;
  rdcnt = wrcnt = 0;
  overflow = underflow = false;
}

// Get number of free bytes
u32 FIFO::free()
{
  return size - (wrcnt - rdcnt);
}

// Get number of used bytes
u32 FIFO::used()
{
  return wrcnt - rdcnt;
}

// Put a word into FIFO (w/o checking free space)
void FIFO::put_word_nocheck(u32 x)
{
  *(u32*)(addr + wrptr) = x;
  wrcnt += 4;
  wrptr += 4;
  if (wrptr >= size) wrptr = 0;
}

// Put a byte into FIFO (w/o checking free space)
void FIFO::put_byte_nocheck(u8 x)
{
  *(addr + wrptr) = x;
  wrcnt += 1;
  wrptr += 1;
  if (wrptr >= size) wrptr = 0;
}

// Put a byte into FIFO
void FIFO::put_byte(u8 x)
{
  if (free())
    put_byte_nocheck(x);
  else
    overflow = true;
}

// Extract a byte from FIFO (w/o checking used space)
u8 FIFO::get_byte_nocheck()
{
  u8 c = *(addr + rdptr);
  rdcnt += 1;
  rdptr += 1;
  if (rdptr >= size) rdptr = 0;
  return c;
}

// Extract a byte from FIFO
u8 FIFO::get_byte()
{
  if (used())
    return get_byte_nocheck();
  else
    underflow = true;
}

// Read first byte of FIFO without pushing it out
u8 FIFO::peek_byte()
{
  return *(addr + rdptr);
}

// Put a number of bytes into FIFO
bool FIFO::put(void *a, u32 num)
{
  if ((free()) < num)
    return false;        // not enough free space

  u8 *buf = (u8*)a;
  while (num--) put_byte_nocheck(*buf++);
  return true;
}

// Extract a number of bytes from FIFO
bool FIFO::get(void *a, u32 num)
{
  if (used() < num)
    return false;        // not enough data in FIFO

  u8 *buf = (u8*)a;
  while (num--) *buf++ = get_byte_nocheck();
  return true;
}

