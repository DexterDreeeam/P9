#pragma once

#include "Memory.h"

_INLINE_ _NOALIAS_ s64 str_len(const char* s) noexcept
{
    s64 i = 0;
    while (*(s + i) != '\0')
    {
        ++i;
    }
    return i;
}

_INLINE_ _NOALIAS_ boole str_equal(const char* s1, const char* s2) noexcept
{
    while (*s1 == *s2)
    {
        if (*s1 == '\0')
        {
            return boole::True;
        }
        ++s1;
        ++s2;
    }
    return boole::False;
}

_INLINE_ _NOALIAS_ s64 str_compare(const char* s1, const char* s2) noexcept
{
    s64 i = 0;
    while (*s1 == *s2)
    {
        if (*s1 == 0)
        {
            return 0; //# equal
        }
        ++s1;
        ++s2;
    }
    return (s64)*s1 - (s64)*s2;
}

_INLINE_ _NOALIAS_ boole is_alike_blank_char(char c) noexcept
{
    switch (c)
    {
    case ' ':
    case '\n':
    case '\t':
    case '\v':
    case '\r':
        return boole::True;
    default:
        break;
    }
    return boole::False;
}

_INLINE_ _NOALIAS_ void str_pick_out_blank(OUT char* s) noexcept
{
    s64 len = str_len(s);
    const char* s_from = s;
    const char* s_to = s;
    const char* s_end = s + len;
    char* buf = (char*)memory_alloc(len + 1);
    s64 len_buf = 0;

    while (s_to < s_end)
    {
        while (s_to < s_end && !is_alike_blank_char(*s_to))
        {
            ++s_to;
        }
        s64 len_copy = s_to - s_from;
        memory_copy(s_from, buf + len_buf, len_copy);
        len_buf += len_copy;
        ++s_to;
        s_from = s_to;
    }
    memory_copy(buf, s, len_buf);
    s[len_buf] = 0;
}
