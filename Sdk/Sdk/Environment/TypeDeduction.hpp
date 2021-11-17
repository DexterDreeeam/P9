#pragma once

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
