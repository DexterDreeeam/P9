#pragma once

class memory final
{
public:
    static void* alloc(u64 sz);

    static void free(void* addr);

    static void set(void* addr, u8 v, u64 sz);

    static void copy(const void* src, void* dst, u64 sz);

    static void* alloc_zero(u64 sz)
    {
        auto* p = alloc(sz);
        assert(p);
        set(p, 0, sz);
        return p;
    }

    static void* alloc_copy(const void* src, u64 alloc_sz, u64 copy_sz)
    {
        auto* p = alloc(alloc_sz);
        assert(p);
        copy(src, p, copy_sz);
        return p;
    }

    template<typename Ty = void>
    static Ty* alloc(u64 sz)
    {
        void* p = alloc(sz);
        return pointer_convert(p, 0, Ty*);
    }

    template<typename Ty = void>
    static Ty* alloc_zero(u64 sz)
    {
        void* p = alloc_zero(sz);
        return pointer_convert(p, 0, Ty*);
    }

    template<typename Ty = void>
    static Ty* alloc_copy(const void* src, u64 alloc_sz, u64 copy_sz)
    {
        void* p = alloc_copy(src, alloc_sz, copy_sz);
        return pointer_convert(p, 0, Ty*);
    }
};
