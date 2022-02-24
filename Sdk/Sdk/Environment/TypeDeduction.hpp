#pragma once

template<typename Ty1, typename Ty2>
constexpr bool is_same_type = false;

template<typename Ty>
constexpr bool is_same_type<Ty, Ty> = true;

template<typename FromTy, typename ToTy>
concept is_convertible =
    is_same_type<FromTy, ToTy> ||
    requires(FromTy f, ToTy t)
    {
        static_cast<ToTy>(f);
    };

template<typename Ty>
constexpr auto right_value_type(const Ty& e) noexcept -> Ty&&
{
    return (Ty&&)(e);
}

template<typename Ty>
constexpr auto right_value_type(Ty&& e) noexcept -> Ty&&
{
    return (Ty&&)(e);
}

template<typename Ty>
struct __prime_value_type
{
public:
    using T = Ty;
};

template<typename Ty>
struct __prime_value_type<Ty&>
{
public:
    using T = Ty;
};

template<typename Ty>
struct __prime_value_type<const Ty&>
{
public:
    using T = Ty;
};

template<typename Ty>
struct __prime_value_type<Ty&&>
{
public:
    using T = Ty;
};

template<typename Ty>
using prime_value_type = typename __prime_value_type<Ty>::T;
