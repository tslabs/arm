#pragma once

/**
  1. clear() must be called either upon runtime init or with interrupts disabled
  2. rdptr and wrptr are only modified by reader and writer respectively
  3. FIFO always uses 1 byte less than actual buffer size
*/

class FIFO
{
public:
  u8 *addr;
  int size;
  int rdptr;
  int wrptr;
  bool overflow;
  bool underflow;

// Init FIFO
  void init(void *a, int s)

  {
    addr = (u8*)a;
    size = s;
    clear();
  }

// Clear FIFO
  void clear()
  {
    rdptr = wrptr = 0;
    overflow = underflow = false;
  }

// Get number of free bytes
  int free()
  {
    int i = rdptr - wrptr;
    if (i < 1) i += size;
    return i - 1;
  }

// Get number of free bytes (volatile)
  int free_vol()
  {
    int i = _vol(rdptr) - _vol(wrptr);
    if (i < 1) i += size;
    return i - 1;
  }

// Get number of used bytes
  int used()
  {
    return size - free() - 1;
  }

// Get number of used bytes (volatile)
  int used_vol()
  {
    return _vol(size) - free_vol() - 1;
  }

// Put a word into FIFO (w/o checking free space)
  void put_word_nocheck(u32 x)
  {
    *(u32*)(addr + wrptr) = x;
    wrptr += 4;
    if (wrptr >= size) wrptr -= size;
  }

// Put a byte into FIFO (w/o checking free space)
  void put_byte_nocheck(u8 x)
  {
    *(addr + wrptr) = x;
    wrptr = (wrptr == (size - 1)) ? 0 : (wrptr + 1);
  }

// Put a byte into FIFO
  void put_byte(u8 x)
  {
    if (free())
      put_byte_nocheck(x);
    else
      overflow = true;
  }

// Extract a byte from FIFO (w/o checking used space)
  u8 get_byte_nocheck()
  {
    u8 c = *(addr + rdptr);
    rdptr = (rdptr == (size - 1)) ? 0 : (rdptr + 1);
    return c;
  }

// Extract a byte from FIFO
  u8 get_byte()
  {
    if (used())
      return get_byte_nocheck();
    else
      underflow = true;
  }

// Read first byte of FIFO without pushing it out
  u8 peek_byte()
  {
    return *(addr + rdptr);
  }

// Put a number of bytes into FIFO
  bool put(void *a, int num)
  {
    if ((size - used()) < num)
      return false;        // not enough free space

    u8 *buf = (u8*)a;
    while (num--) put_byte_nocheck(*buf++);
    return true;
  }

// Extract a number of bytes from FIFO
  bool get(void *a, int num)
  {
    if (used() < num)
      return false;        // not enough data in FIFO

    u8 *buf = (u8*)a;
    while (num--) *buf++ = get_byte_nocheck();
    return true;
  }
};
