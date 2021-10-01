#pragma once

#include "ComplierDirective.h"

typedef    unsigned               char  u8;
typedef      signed               char  s8;
typedef    unsigned  short         int  u16;
typedef      signed  short         int  s16;
typedef    unsigned       long     int  u32;
typedef      signed       long     int  s32;
typedef    unsigned  long long     int  u64;
typedef      signed  long long     int  s64;
typedef                          float  f32;
typedef                         double  f64;
typedef                            u64  uint;
typedef                            s64  sint;
typedef                            s32  boole;
typedef                            u64  hndl;

#pragma warning (push)
#pragma warning (disable : 26450)
#pragma warning (disable : 26454)
#pragma warning (disable : 26812)
#pragma warning (disable : 4307)

const u8  u8_max = (u8)0 - 1;
const u8  u8_min = (u8)0;
const s8  s8_max = (s8)(((s8)1 << 7) - 1);
const s8  s8_min = (s8)((s8)1 << 7);
const u16 u16_max = (u16)0 - 1;
const u16 u16_min = (u16)0;
const s16 s16_max = (s16)(((s16)1 << 15) - 1);
const s16 s16_min = (s16)((s16)1 << 15);
const u32 u32_max = (u32)0 - 1;
const u32 u32_min = (u32)0;
const s32 s32_max = (s32)(((s32)1 << 31) - 1);
const s32 s32_min = (s32)((s32)1 << 31);
const u64 u64_max = (u64)0 - 1;
const u64 u64_min = (u64)0;
const s64 s64_max = (s64)(((s64)1 << 63) - 1);
const s64 s64_min = (s64)((s64)1 << 63);
const f32 f32_pos_min = (1.0e-30f);
const f32 f32_pos_max = (1.0e30f);
const f32 f32_epsilon = (1.0e-5f);
const boole boole_true = ((s32)1);
const boole boole_false = ((s32)0);

typedef enum : s32
{
    RET_FAILED = 0,
    RET_FALSE = 0,
    RET_SUCCESS = 1,
    RET_TRUE = 1,

    RET_FAILURE_BASE = 1L << 31,
    RET_NOSENSE,
    RET_OUT_OF_TIME,
    RET_INVALID_PARAM,

    RET_SUCCEED_BASE = 1L << 30,
    RET_NOT_IMMEDIATE,
} RET;

_INLINE_ _NOALIAS_ boole is_success(RET rt) noexcept
{
    return rt > 0L ? boole_true : boole_false;
}

template<typename Ty1, typename Ty2>
_INLINE_ _NOALIAS_ void swap(Ty1& e1, Ty2& e2) noexcept
{
    Ty1 tmp = e1;
    e1 = e2;
    e2 = tmp;
}

#pragma warning (pop)
