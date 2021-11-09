#pragma once

#include "Memory.hpp"

_INLINE_ void* operator new(size_t sz)
{
    return memory::alloc<void>(sz);
}

_INLINE_ void operator delete(void* ptr)
{
    memory::free(ptr);
}

_INLINE_ void* operator new[](size_t sz)
{
    return memory::alloc<void>(sz);
}

_INLINE_ void operator delete[](void* ptr)
{
    memory::free(ptr);
}

_INLINE_ void* operator new(size_t sz, void* ptr)
{
    return ptr;
}

_INLINE_ void* operator new[](size_t sz, void* ptr)
{
    return ptr;
}
