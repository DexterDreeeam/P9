#pragma once

#include "Date.hpp"

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

_INLINE_ u64 random(u64 mod)
{
    LinuxGccNs::srand((u32)(tick_count() % u32_max));
    return LinuxGccNs::rand() % (mod);
}

_INLINE_ s32 atom_increment(s32 volatile& x)
{
    return LinuxGccNs::InterlockedIncrement(&x);
}

_INLINE_ s64 atom_increment(s64 volatile& x)
{
    return LinuxGccNs::InterlockedIncrement64(&x);
}

_INLINE_ s32 atom_decrement(s32 volatile& x)
{
    return LinuxGccNs::InterlockedDecrement(&x);
}

_INLINE_ s64 atom_decrement(s64 volatile& x)
{
    return LinuxGccNs::InterlockedDecrement64(&x);
}

_INLINE_ s32 atom_exchange(s32 volatile& x, s32 replace)
{
    return LinuxGccNs::InterlockedExchange(&x, replace);
}

_INLINE_ s64 atom_exchange(s64 volatile& x, s64 replace)
{
    return LinuxGccNs::InterlockedExchange64(&x, replace);
}

_INLINE_ void* atom_exchange(void* volatile& x, void* replace)
{
    return LinuxGccNs::InterlockedExchangePointer(&x, replace);
}

_INLINE_ s32 atom_compare_exchange(s32 volatile& x, s32 compare, s32 replace)
{
    return LinuxGccNs::InterlockedCompareExchange(&x, replace, compare);
}

_INLINE_ s64 atom_compare_exchange(s64 volatile& x, s64 compare, s64 replace)
{
    return LinuxGccNs::InterlockedCompareExchange64(&x, replace, compare);
}

_INLINE_ void* atom_compare_exchange(void* volatile& x, void* compare, void* replace)
{
    return LinuxGccNs::InterlockedCompareExchangePointer(&x, replace, compare);
}
