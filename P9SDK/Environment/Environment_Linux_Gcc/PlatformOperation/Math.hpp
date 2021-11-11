#pragma once

#define _abs(x)                           ( (x) >= (0) ? (x) : (-x) )
#define _min(x,y)                         ( (x) < (y) ? (x) : (y) )
#define _max(x,y)                         ( (x) > (y) ? (x) : (y) )
#define _clamp(n,_min,_max)               ( (n < _min) ? _min : (n > _max) ? _max : n )
#define _around(x,mod)                    ( (x + mod - 1) / mod * mod )
#define _floor(x,mod)                     ( (x / mod) * mod )
#define _ceil(x,mod)                      ( _around(x,mod) )
#define _power(x,exp)                     ( pow(x,exp) )

#define _KB                               (1024ULL)
#define _MB                               (1024ULL * 1024ULL)
#define _GB                               (1024ULL * 1024ULL * 1024ULL)
#define _sq(x)                            __sq(x)
#define _sqrt(x)                          __sqrt(x)
#define _sqrt_inv(x)                      __sqrt_inv(x)
#define _degree_radian(x)                 __degree_radian(x)
#define _radian_degree(x)                 __radian_degree(x)
#define _tan_radian(x)                    tan(x)
#define _tan_degree(x)                    tan(degree_radian(x))
#define _sin_radian(x)                    sin(x)
#define _sin_degree(x)                    sin(degree_radian(x))
#define _cos_radian(x)                    cos(x)
#define _cos_degree(x)                    cos(degree_radian(x))
#define _atan2_radian(x,y)                __atan2(x, y)                    //# (x != 0 || y != 0) : (-Pi, +Pi]
#define _atan2_degree(x,y)                __radian_degree(__atan2(x, y))   //# (x != 0 || y != 0) : (-180, 180]
#define _asin_radian(x)                   __asin(x)                        //# [-1, 1] : [-0.5 * Pi, 0.5 * Pi]
#define _asin_degree(x)                   __radian_degree(__asin(x))       //# [-1, 1] : [-90, 90]
#define _acos_radian(x)                   __acos(x)                        //# [-1, 1] : [0, +Pi]
#define _acos_degree(x)                   __radian_degree(__acos(x))       //# [-1, 1] : [0, 180]

_INLINE_ _NOALIAS_ f32 __degree_radian(f32 degree) { return static_cast<f32>(0.01745329251994329576923690768489f) * degree; }
_INLINE_ _NOALIAS_ f64 __degree_radian(f64 degree) { return static_cast<f64>(0.01745329251994329576923690768489) * degree; }
_INLINE_ _NOALIAS_ f32 __radian_degree(f32 radian) { return static_cast<f32>(57.295779513082320876798154814105f) * radian; }
_INLINE_ _NOALIAS_ f64 __radian_degree(f64 radian) { return static_cast<f64>(57.295779513082320876798154814105) * radian; }

_INLINE_ _NOTHROW_ _NOALIAS_ f32 __sqrt_inv(f32 n)
{
    s32 i;
    f32 x;
    const f32 threehalfs = 1.5f;
    x = n * 0.5f;
    i = *reinterpret_cast<s32*>(&n);
    i = 0x5f3759df - (i >> 1);
    n = *reinterpret_cast<f32*>(&i);
    n = n * (threehalfs - (x * n * n));
    //n = n * (threehalfs - (x * n * n));
    return n;
}
_INLINE_ _NOTHROW_ _NOALIAS_ f64 __sqrt_inv(f64 n)
{
    s64 i;
    f64 x;
    const f64 threehalfs = 1.5;
    x = n * 0.5;
    i = *reinterpret_cast<s64*>(&n);
    i = 0x5fe6ec85e7de30da - (i >> 1);
    n = *reinterpret_cast<f64*>(&i);
    n = n * (threehalfs - (x * n * n));
    //n = n * (threehalfs - (x * n * n));
    //n = n * (threehalfs - (x * n * n));
    return n;
}
_INLINE_ _NOTHROW_ _NOALIAS_ f32 __sq(f32 n) { return n * n; }
_INLINE_ _NOTHROW_ _NOALIAS_ f32 __sqrt(f32 n) { assert(__sqrt_inv(n) > f32_pos_min); return 1.0f / __sqrt_inv(n); }
_INLINE_ _NOTHROW_ _NOALIAS_ f64 __sq(f64 n) { return n * n; }
_INLINE_ _NOTHROW_ _NOALIAS_ f64 __sqrt(f64 n) { assert(__sqrt_inv(n) > f64_pos_min); return 1.0 / __sqrt_inv(n); }

_INLINE_ _NOALIAS_ f32 __acos(f32 n) { return acosf(n); }
_INLINE_ _NOALIAS_ f32 __asin(f32 n) { return asinf(n); }
_INLINE_ _NOALIAS_ f32 __atan2(f32 x, f32 y) { return atan2f(x, y); }
_INLINE_ _NOALIAS_ f64 __acos(f64 n) { return acos(n); }
_INLINE_ _NOALIAS_ f64 __asin(f64 n) { return asin(n); }
_INLINE_ _NOALIAS_ f64 __atan2(f64 x, f64 y) { return atan2(x, y); }
