#pragma once

class memory final
{
public:
    template<typename Ty = void>
    static Ty* alloc(u64 sz);

    template<typename Ty = void>
    static Ty* alloc_zero(u64 sz);

    template<typename Ty = void>
    static Ty* alloc_copy(const void* src, u64 alloc_sz, u64 copy_sz);

    static void free(void* addr);

    static void set(void* addr, u8 v, u64 sz);

    static void copy(const void* src, void* dst, u64 sz);
};
