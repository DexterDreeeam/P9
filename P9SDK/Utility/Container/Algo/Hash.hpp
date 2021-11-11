#pragma once

#include "../Unsafe/String.hpp"

template<typename Ptr_Ty>
_INLINE_ u64 Hash(const Ptr_Ty* p) noexcept
{
    return reinterpret_cast<u64>(p);
}

_INLINE_ u64 Hash(int k) noexcept
{
    return static_cast<u64>(s64(k));
}

_INLINE_ u64 Hash(unsigned int k) noexcept
{
    return static_cast<u64>(k);
}

_INLINE_ u64 Hash(s8 k) noexcept
{
    return static_cast<u64>(s64(k));
}

_INLINE_ u64 Hash(u8 k) noexcept
{
    return static_cast<u64>(k);
}

_INLINE_ u64 Hash(s16 k) noexcept
{
    return static_cast<u64>(s64(k));
}

_INLINE_ u64 Hash(u16 k) noexcept
{
    return static_cast<u64>(k);
}

_INLINE_ u64 Hash(s32 k) noexcept
{
    return static_cast<u64>(s64(k));
}

_INLINE_ u64 Hash(u32 k) noexcept
{
    return static_cast<u64>(k);
}

_INLINE_ u64 Hash(s64 k) noexcept
{
    return static_cast<u64>(k);
}

_INLINE_ u64 Hash(u64 k) noexcept
{
    return k;
}

_INLINE_ u64 Hash(const string& s) noexcept
{
    s64 len = s.size();
    u64 hs = static_cast<u64>(s64_magic);
    s64 sample_gap = _max(len >> 4, 1);
    s64 idx = 0;
    while (idx < len)
    {
        hs += (len + s64_magic) / (idx + 1) * s[idx];
        idx += sample_gap;
    }
    return hs;
}

//_INLINE_ u64 Hash(const char *cstr) noexcept
//{
//    s64 len = str_len(cstr);
//    u64 hs = static_cast<u64>(magic_s64);
//    s64 sample_gap = max(len >> 4, 1);
//    s64 idx = 0;
//    while (idx < len)
//    {
//        hs += (len + magic_s64) / (idx + 1) * cstr[idx];
//        idx += sample_gap;
//    }
//    return hs;
//}
