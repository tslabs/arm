#pragma once

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <stdint.h>

typedef uint64_t       U64;
typedef uint32_t       U32;
typedef uint16_t       U16;
typedef uint8_t        U8;
typedef uint64_t       u64;
typedef uint32_t       u32;
typedef uint16_t       u16;
typedef uint8_t        u8;

typedef int64_t        I64;
typedef int32_t        I32;
typedef int16_t        I16;
typedef int8_t         I8;
typedef int64_t        i64;
typedef int32_t        i32;
typedef int16_t        i16;
typedef int8_t         i8;

#define max(a,b)  (((a) > (b)) ? (a) : (b))
#define min(a,b)  (((a) < (b)) ? (a) : (b))

#define lo8(a)    ((u8)(a))
#define hi8(a)    ((u8)(a >> 8))
#define hx8(a)    ((u8)(a >> 16))

#define countof(a)  (sizeof(a) / sizeof(a[0]))
