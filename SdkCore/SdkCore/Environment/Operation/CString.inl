#pragma once

_INLINE_ _NOALIAS_ boole char_equal_case_insensitive(char c1, char c2) noexcept
{
    s64 diff = (s64)c1 - (s64)c2;
    constexpr s64 lower_upper_diff = (s64)'a' - (s64)'A';
    constexpr s64 upper_lower_diff = (s64)'A' - (s64)'a';

    switch (diff)
    {
    case 0:
        return boole::True;
    case lower_upper_diff:
        return c1 >= 'a' && c1 <= 'z';
    case upper_lower_diff:
        return c1 >= 'A' && c1 <= 'Z';
    default:
        return boole::False;
    }
}

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
    assert(s1);
    assert(s2);
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

_INLINE_ _NOALIAS_ boole str_equal_case_insensitive(const char* s1, const char* s2) noexcept
{
    assert(s1);
    assert(s2);
    while (char_equal_case_insensitive(*s1, *s2))
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

_INLINE_ _NOALIAS_ boole is_blank_char(char c) noexcept
{
    return c == ' ';
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

_INLINE_ _NOALIAS_ void str_pick_out_blank(_OUT_ char* s) noexcept
{
    s64 len = str_len(s);
    const char* s_from = s;
    const char* s_to = s;
    const char* s_end = s + len;
    char* buf = memory::alloc<char>(len + 1);
    s64 len_buf = 0;

    while (s_to < s_end)
    {
        while (s_to < s_end && !is_alike_blank_char(*s_to))
        {
            ++s_to;
        }
        s64 len_copy = s_to - s_from;
        memory::copy(s_from, buf + len_buf, len_copy);
        len_buf += len_copy;
        ++s_to;
        s_from = s_to;
    }
    memory::copy(buf, s, len_buf);
    s[len_buf] = 0;
}
