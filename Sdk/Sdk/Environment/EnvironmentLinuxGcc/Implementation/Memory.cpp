#pragma once

#include "../../Interface.hpp"
#include "../EnvironmentHeader.hpp"

void* memory::alloc(u64 sz)
{
    void* p = ::malloc(sz);
    assert(p);
    return p;
}

void* memory::alloc_zero(u64 sz)
{
    auto* p = alloc(sz);
    if (p)
    {
        set(p, 0, sz);
    }
    return p;
}

void* memory::alloc_copy(const void* src, u64 alloc_sz, u64 copy_sz)
{
    auto* p = alloc(alloc_sz);
    if (p && copy_sz > 0)
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
