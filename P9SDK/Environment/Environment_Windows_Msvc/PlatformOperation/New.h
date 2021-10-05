#pragma once

#include "Memory.h"

_INLINE_ void* operator new(size_t sz)
{
    return memory_alloc(sz);
}

_INLINE_ void operator delete(void* ptr)
{
    memory_free(ptr);
}

_INLINE_ void* operator new[](size_t sz)
{
    return memory_alloc(sz);
}

_INLINE_ void operator delete[](void* ptr)
{
    memory_free(ptr);
}

_INLINE_ void* operator new(size_t sz, void* ptr)
{
    return ptr;
}

_INLINE_ void* operator new[](size_t sz, void* ptr)
{
    return ptr;
}
