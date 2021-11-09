#pragma once

class math final
{
public:
    template<typename Ty>
    static Ty abs(Ty x);

    template<typename Ty1, typename Ty2>
    static Ty1 min(Ty1 x, Ty2 y);

    template<typename Ty1, typename Ty2>
    static Ty1 max(Ty1 x, Ty2 y);

    template<typename Ty1, typename Ty2, typename Ty3>
    static Ty1 clamp(Ty1 x, Ty2 from, Ty3 to);

    template<typename Ty1, typename Ty2>
    static Ty1 ceil(Ty1 x, Ty2 mod);

    template<typename Ty1, typename Ty2>
    static Ty1 floor(Ty1 x, Ty2 mod);

    static f64 power(f64 x, f64 exp);

    template<typename Ty>
    static Ty square(Ty x);

    static f64 square_root_inv(f64 x);

    static f64 square_root(f64 x);

    static f64 degree_to_radian(f64 x);

    static f64 radian_to_degree(f64 x);

    static f64 tan(f64 x);

    static f64 sin(f64 x);

    static f64 cos(f64 x);

    static f64 atan2(f64 x, f64 y);

    static f64 asin(f64 x);

    static f64 acos(f64 x);

    static u64 kilobyte();

    static u64 megabyte();

    static u64 gigabyte();

    static u64 terabyte();

    static u64 petabyte();
};

template<typename Ty>
_INLINE_ Ty math::abs(Ty x)
{
    return x >= 0 ? x : -x;
}

template<typename Ty1, typename Ty2>
_INLINE_ Ty1 math::min(Ty1 x, Ty2 y)
{
    return x <= y ? x : y;
}

template<typename Ty1, typename Ty2>
_INLINE_ Ty1 math::max(Ty1 x, Ty2 y)
{
    return x >= y ? x : y;
}

template<typename Ty1, typename Ty2, typename Ty3>
_INLINE_ Ty1 math::clamp(Ty1 x, Ty2 from, Ty3 to)
{
    return x < from ? from : x > to ? to : x;
}

template<typename Ty1, typename Ty2>
_INLINE_ Ty1 math::ceil(Ty1 x, Ty2 mod)
{
    return (x + mod - 1) / mod * mod;
}

template<typename Ty1, typename Ty2>
_INLINE_ Ty1 math::floor(Ty1 x, Ty2 mod)
{
    return x / mod * mod;
}

template<typename Ty>
_INLINE_ Ty math::square(Ty x)
{
    return x * x;
}

_INLINE_ f64 math::square_root_inv(f64 x)
{
    s64 i;
    f64 k;
    const f64 threehalfs = 1.5;
    k = x * 0.5;
    i = *reinterpret_cast<s64*>(&x);
    i = 0x5fe6ec85e7de30da - (i >> 1);
    x = *reinterpret_cast<f64*>(&i);
    x = x * (threehalfs - (k * x * x));
    //x = x * (threehalfs - (k * x * x));
    //x = x * (threehalfs - (k * x * x));
    return x;
}

_INLINE_ f64 math::square_root(f64 x)
{
    return 1.0 / square_root_inv(x);
}

_INLINE_ f64 math::degree_to_radian(f64 x)
{
    return 0.01745329251994329576923690768489 * x;
}

_INLINE_ f64 math::radian_to_degree(f64 x)
{
    return 57.295779513082320876798154814105 * x;
}

_INLINE_ u64 math::kilobyte()
{
    return 1024ULL;
}

_INLINE_ u64 math::megabyte()
{
    return kilobyte() * 1024ULL;
}

_INLINE_ u64 math::gigabyte()
{
    return megabyte() * 1024ULL;
}

_INLINE_ u64 math::terabyte()
{
    return gigabyte() * 1024ULL;
}

_INLINE_ u64 math::petabyte()
{
    return terabyte() * 1024ULL;
}
