
// Common types definition
//
// (c) 2015, TS-Labs inc.
//
// Ποιος είπε πως οι άγγελοι στη γη δεν κατεβαίνουν

#pragma once

#include "defs.hpp"

// Unioned types definiton
typedef union
{
  struct
  {
    u8  b0:8;
    u8  b1:8;
  };
  u16 h;
} w16;

typedef union
{
  struct
  {
    u8  b0:8;
    u8  b1:8;
    u8  b2:8;
    u8  b3:8;
  };
  struct
  {
    u16 h0:16;
    u16 h1:16;
  };
  u32 w:32;
} w32;

typedef union
{
  struct
  {
    u8  b0:8;
    u8  b1:8;
    u8  b2:8;
    u8  b3:8;
    u8  b4:8;
    u8  b5:8;
    u8  b6:8;
    u8  b7:8;
  };
  struct
  {
    u16 h0:16;
    u16 h1:16;
    u16 h2:16;
    u16 h3:16;
  };
  struct
  {
    u32 w0:32;
    u32 w1:32;
  };
  u64 d;
} w64;


typedef struct
{
  s32 l;
  s32 r;
} DAC_SUM;

typedef union
{
  struct
  {
    u16 l;
    u16 r;
  };
  u32 w;
} DAC_T;
