#pragma once

#include <stddef.h>

typedef unsigned char                u8;
typedef unsigned short               u16;
typedef unsigned long                u32;
typedef unsigned long long           u64;

typedef signed char                  s8;
typedef short                        s16;
typedef long                         s32;
typedef long long                    s64;

typedef volatile unsigned char       vu8;
typedef volatile unsigned short      vu16;
typedef volatile unsigned long       vu32;
typedef volatile unsigned long long  vu64;

typedef volatile signed char         vs8;
typedef volatile short               vs16;
typedef volatile long                vs32;
typedef volatile long long           vs64;

typedef float                        f32;
typedef double                       f64;

// typedef u8  BYTE;
// typedef u16 HALF;
// typedef u32 WORD;

typedef _Bool bool;

// #ifndef bool
    // #define bool _Bool
// #endif

typedef u8                          bool8;

#if !defined(_SIZE_T) && !defined(_SIZE_T_) && !defined(_SIZE_T_DEF)
#define _SIZE_T
#define _SIZE_T_DEF            /* exeGCC size_t define label */
#if (_MIPS_SZLONG == 32)
typedef unsigned int    size_t;
#endif
#if (_MIPS_SZLONG == 64)
typedef unsigned long   size_t;
#endif
#endif

//- Common definitions
#ifndef TRUE
#define TRUE    1
#endif

#ifndef FALSE
#define FALSE   0
#endif

#ifndef NULL
#define NULL    0
#endif

#ifndef NIL
#define NIL 0
#endif


