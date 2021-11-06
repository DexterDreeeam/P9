#pragma once

#include "../../Interface.hpp"
#include "../EnvironmentHeader.hpp"

template<typename Ty>
Ty* memory::alloc(u64 sz)
{
    void* p = ::malloc(sz);
    assert(p);
    return pointer_convert(p, 0, Ty*);
}

template<typename Ty>
Ty* memory::alloc_zero(u64 sz)
{
    auto* p = alloc(sz);
    if (p)
    {
        set(p, 0, sz);
    }
    return p;
}

template<typename Ty>
Ty* memory::alloc_copy(const void* src, u64 alloc_sz, u64 copy_sz)
{
    auto* p = alloc(alloc_sz);
    if (p)
    {
        copy(src, p, copy_sz);
    }
    return p;
}

void memory::free(void* addr)
{
    ::free(addr);
}

void memory::set(void* addr, u8 v, u64 sz)
{
    assert(addr);
    ::memset(addr, v, sz);
}

void memory::copy(const void* src, void* dst, u64 sz)
{
    assert(src && dst);
    ::memcpy_s(dst, sz, src, sz);
}
