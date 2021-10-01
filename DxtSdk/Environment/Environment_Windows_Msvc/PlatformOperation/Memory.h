#pragma once

_INLINE_ void* memory_alloc(u64 sz);
_INLINE_ void* memory_alloc_zero(u64 sz);
_INLINE_ void  memory_free(void* addr);
_INLINE_ void  memory_set(void* addr, u8 val, u64 sz);
_INLINE_ void  memory_copy(const void* src, void* dst, u64 sz);
_INLINE_ void* memory_alloc_copy(const void* src, u64 alloc_sz, u64 copy_sz);

_INLINE_ void* memory_alloc(u64 sz)
{
    void* p = reinterpret_cast<void*>(WindowsMsvcNs::malloc(sz));
    assert(p);
    return p;
}

_INLINE_ void* memory_alloc_zero(u64 sz)
{
    void* p = reinterpret_cast<void*>(WindowsMsvcNs::malloc(sz));
    assert(p);
    WindowsMsvcNs::memset(p, 0, sz);
    return p;
}

_INLINE_ void memory_free(void* addr)
{
    assert(addr);
    WindowsMsvcNs::free(addr);
}

_INLINE_ void memory_set(void* addr, u8 val, u64 sz)
{
    WindowsMsvcNs::memset(addr, val, sz);
}

_INLINE_ void memory_copy(const void* src, void* dst, u64 sz)
{
    WindowsMsvcNs::memcpy(dst, src, sz);
}

_INLINE_ void* memory_alloc_copy(const void* src, u64 alloc_sz, u64 copy_sz)
{
    assert(alloc_sz >= copy_sz);
    void* p = memory_alloc(alloc_sz);
    assert(p);
    memory_copy(src, p, copy_sz);
    return p;
}
