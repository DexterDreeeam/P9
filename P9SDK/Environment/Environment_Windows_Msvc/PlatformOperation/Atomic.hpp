#pragma once

template<typename Ty>

template<typename Ty>
class atom
{
public:
    atom() :
        value()
    {}

    atom(Ty v) :
        value(v)
    {}

    ~atom() = default;

public:
    operator Ty()
    {
        return value;
    }

public:
    Ty value;
};

_INLINE_ s32   atom_increment(s32 volatile& x);
_INLINE_ s64   atom_increment(s64 volatile& x);
_INLINE_ s32   atom_decrement(s32 volatile& x);
_INLINE_ s64   atom_decrement(s64 volatile& x);
_INLINE_ s32   atom_exchange(s32 volatile& x, s32 replace);
_INLINE_ s64   atom_exchange(s64 volatile& x, s64 replace);
_INLINE_ void* atom_exchange(void* volatile& x, void* replace);
_INLINE_ s32   atom_compare_exchange(s32 volatile& x, s32 compare, s32 replace);
_INLINE_ s64   atom_compare_exchange(s64 volatile& x, s64 compare, s64 replace);
_INLINE_ void* atom_compare_exchange(void* volatile& x, void* compare, void* replace);
_INLINE_ u64   random(u64 mod);

#include "Date.hpp"

_INLINE_ u64 random(u64 mod)
{
    WindowsMsvcNs::srand((u32)(tick_count() % u32_max));
    return WindowsMsvcNs::rand() % (mod);
}

_INLINE_ s32 atom_increment(s32 volatile& x)
{
    return WindowsMsvcNs::InterlockedIncrement(&x);
}

_INLINE_ s64 atom_increment(s64 volatile& x)
{
    return WindowsMsvcNs::InterlockedIncrement64(&x);
}

_INLINE_ s32 atom_decrement(s32 volatile& x)
{
    return WindowsMsvcNs::InterlockedDecrement(&x);
}

_INLINE_ s64 atom_decrement(s64 volatile& x)
{
    return WindowsMsvcNs::InterlockedDecrement64(&x);
}

_INLINE_ s32 atom_exchange(s32 volatile& x, s32 replace)
{
    return WindowsMsvcNs::InterlockedExchange(&x, replace);
}

_INLINE_ s64 atom_exchange(s64 volatile& x, s64 replace)
{
    return WindowsMsvcNs::InterlockedExchange64(&x, replace);
}

_INLINE_ void* atom_exchange(void* volatile& x, void* replace)
{
    return WindowsMsvcNs::InterlockedExchangePointer(&x, replace);
}

_INLINE_ s32 atom_compare_exchange(s32 volatile& x, s32 compare, s32 replace)
{
    return WindowsMsvcNs::InterlockedCompareExchange(&x, replace, compare);
}

_INLINE_ s64 atom_compare_exchange(s64 volatile& x, s64 compare, s64 replace)
{
    return WindowsMsvcNs::InterlockedCompareExchange64(&x, replace, compare);
}

_INLINE_ void* atom_compare_exchange(void* volatile& x, void* compare, void* replace)
{
    return WindowsMsvcNs::InterlockedCompareExchangePointer(&x, replace, compare);
}
