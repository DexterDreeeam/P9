#pragma once

_INLINE_ void* memory::alloc_zero(u64 sz)
{
    auto* p = alloc(sz);
    assert(p);
    set(p, 0, sz);
    return p;
}

_INLINE_ void* memory::alloc_copy(const void* src, u64 alloc_sz, u64 copy_sz)
{
    auto* p = alloc(alloc_sz);
    assert(p);
    copy(src, p, copy_sz);
    return p;
}

template<typename Ty>
_INLINE_ Ty* memory::alloc(u64 sz)
{
    void* p = alloc(sz);
    return pointer_convert(p, 0, Ty*);
}

template<typename Ty>
_INLINE_ Ty* memory::alloc_zero(u64 sz)
{
    void* p = alloc_zero(sz);
    return pointer_convert(p, 0, Ty*);
}

template<typename Ty>
_INLINE_ Ty* memory::alloc_copy(const void* src, u64 alloc_sz, u64 copy_sz)
{
    void* p = alloc_copy(src, alloc_sz, copy_sz);
    return pointer_convert(p, 0, Ty*);
}
