#pragma once

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
typedef                            u64  hndl;

typedef struct
{
    s32 year;
    s32 month;
    s32 day;
    s32 week_day;
    s32 hour;
    s32 minute;
    s32 second;
    s32 millisec;
} date;

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
const s32 s32_magic = 0x7a9f82e3L;

const u64 u64_max = (u64)0 - 1;
const u64 u64_min = (u64)0;
const s64 s64_max = (s64)(((s64)1 << 63) - 1);
const s64 s64_min = (s64)((s64)1 << 63);
const s64 s64_magic = 0x7a9f82e301d65c4bLL;

const f32 f32_pos_min = (1.0e-30f);
const f32 f32_pos_max = (1.0e30f);
const f32 f32_epsilon = (1.0e-5f);
const f32 f32_pi = (3.1415926535897932384626433832795f);

const f64 f64_pos_min = (1.0e-60);
const f64 f64_pos_max = (1.0e60);
const f64 f64_epsilon = (1.0e-5);
const f64 f64_pi = (3.1415926535897932384626433832795);

#define class_offset(cls, ele)            reinterpret_cast<unsigned long long>(&(reinterpret_cast<cls*>(nullptr)->ele))
#define pointer_convert(p, offset, newp)  reinterpret_cast<newp>(const_cast<char*>(reinterpret_cast<const char*>(p)) + (s64(offset)))

#include "Boole.hpp"

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
    return rt > 0L ? boole::True : boole::False;
}

_INLINE_ _NOALIAS_ boole is_digit(char c) noexcept
{
    return c >= '0' && c <= '9';
}

_INLINE_ _NOALIAS_ boole is_lower_case(char c) noexcept
{
    return c >= 'a' && c <= 'z';
}

_INLINE_ _NOALIAS_ boole is_upper_case(char c) noexcept
{
    return c >= 'A' && c <= 'Z';
}

_INLINE_ _NOALIAS_ boole is_alpha(char c) noexcept
{
    return is_lower_case(c) || is_upper_case(c);
}

_INLINE_ _NOALIAS_ boole is_alphanumeric(char c) noexcept
{
    return is_digit(c) || is_alpha(c);
}

template<typename Ty1, typename Ty2>
_INLINE_ _NOALIAS_ void swap(Ty1& e1, Ty2& e2) noexcept
{
    Ty1 tmp = e1;
    e1 = e2;
    e2 = tmp;
}

template<typename Ty>
constexpr auto right_value_type(Ty& e) noexcept -> Ty&&
{
    return static_cast<Ty&&>(e);
}
