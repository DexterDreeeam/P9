#pragma once

#include "Memory.h"

void* operator new(size_t sz)
{
    return memory_alloc(sz);
}

void operator delete(void* ptr)
{
    memory_free(ptr);
}

void* operator new[](size_t sz)
{
    return memory_alloc(sz);
}

void operator delete[](void* ptr)
{
    memory_free(ptr);
}

void* operator new(size_t sz, void* ptr)
{
    return ptr;
}

void* operator new[](size_t sz, void* ptr)
{
    return ptr;
}
