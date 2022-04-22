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

    static boole equal(f64 x, f64 y);

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

    static constexpr f64 pi();

    static constexpr f64 epsilon();

    static constexpr u64 kilobyte();

    static constexpr u64 megabyte();

    static constexpr u64 gigabyte();

    static constexpr u64 terabyte();

    static constexpr u64 petabyte();
};
