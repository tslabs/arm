// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <sys/stat.h>

typedef unsigned long long  U64;
typedef unsigned long       U32;
typedef unsigned short      U16;
typedef unsigned char       U8;
typedef unsigned long long  u64;
typedef unsigned long       u32;
typedef unsigned short      u16;
typedef unsigned char       u8;

typedef signed long long    I64;
typedef signed long         I32;
typedef signed short        I16;
typedef signed char         I8;
typedef signed long long    i64;
typedef signed long         i32;
typedef signed short        i16;
typedef signed char         i8;

#define max(a,b)  (((a) > (b)) ? (a) : (b))
#define min(a,b)  (((a) < (b)) ? (a) : (b))

#define lo8(a)    ((u8)(a))
#define hi8(a)    ((u8)(a >> 8))
#define hx8(a)    ((u8)(a >> 16))

#define countof(a)  (sizeof(a) / sizeof(a[0]))
