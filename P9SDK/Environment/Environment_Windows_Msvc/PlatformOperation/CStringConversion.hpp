#pragma once

_INLINE_ s64 s32_to_text(s32 v, char* text)
{
    if (v == 0)
    {
        text[0] = '0';
        return 1;
    }
    if (v == s32_min)
    {
        text[0] = '-';
        text[1] = '2';
        text[2] = '1';
        text[3] = '4';
        text[4] = '7';
        text[5] = '4';
        text[6] = '8';
        text[7] = '3';
        text[8] = '6';
        text[9] = '4';
        text[10] = '8';
        return 11;
    }
    s64 digits = 0;
    if (v < 0)
    {
        ++digits;
        text[0] = '-';
        v = -v;
    }
    s32 num = v;
    while (num)
    {
        ++digits;
        num /= 10;
    }
    s64 write_len = digits;
    while (v)
    {
        --digits;
        text[digits] = '0' + v % 10;
        v /= 10;
    }
    return write_len;
}

_INLINE_ s64 u32_to_text(u32 v, char* text)
{
    if (v == 0)
    {
        text[0] = '0';
        return 1;
    }
    s64 digits = 0;
    u32 num = v;
    while (num)
    {
        ++digits;
        num /= 10;
    }
    s64 write_len = digits;
    while (v)
    {
        --digits;
        text[digits] = '0' + v % 10;
        v /= 10;
    }
    return write_len;
}

_INLINE_ s64 u32_to_text_padding(u32 v, char* text, s64 holds)
{
    s64 myhold = holds;
    while (--myhold >= 0)
    {
        text[myhold] = '0' + v % 10;
        v /= 10;
    }
    return holds;
}

_INLINE_ s64 s64_to_text(s64 v, char* text)
{
    if (v == 0)
    {
        text[0] = '0';
        return 1;
    }
    if (v == s64_min)
    {
        text[0] = '-';
        text[1] = '9';
        text[2] = '2';
        text[3] = '2';
        text[4] = '3';
        text[5] = '3';
        text[6] = '7';
        text[7] = '2';
        text[8] = '0';
        text[9] = '3';
        text[10] = '6';
        text[11] = '8';
        text[12] = '5';
        text[13] = '4';
        text[14] = '7';
        text[15] = '7';
        text[16] = '5';
        text[17] = '8';
        text[18] = '0';
        text[19] = '8';
        return 20;
    }
    s64 digits = 0;
    if (v < 0)
    {
        ++digits;
        text[0] = '-';
        v = -v;
    }
    s64 num = v;
    while (num)
    {
        ++digits;
        num /= 10;
    }
    s64 write_len = digits;
    while (v)
    {
        --digits;
        text[digits] = '0' + v % 10;
        v /= 10;
    }
    return write_len;
}

_INLINE_ s64 u64_to_text(u64 v, char* text)
{
    if (v == 0)
    {
        text[0] = '0';
        return 1;
    }
    s64 digits = 0;
    u64 num = v;
    while (num)
    {
        ++digits;
        num /= 10;
    }
    s64 write_len = digits;
    while (v)
    {
        --digits;
        text[digits] = '0' + v % 10;
        v /= 10;
    }
    return write_len;
}

_INLINE_ s64 u64_to_text_padding(u64 v, char* text, s64 holds)
{
    s64 myholds = holds;
    while (--myholds >= 0)
    {
        text[myholds] = '0' + v % 10;
        v /= 10;
    }
    return holds;
}

_INLINE_ s64 u64_to_text_hex(u64 v, char* text)
{
    text[0] = '0';
    text[1] = 'x';
    text += 2;
    if (v == 0)
    {
        text[0] = '0';
        return 3;
    }
    s64 digits = 0;
    u64 num = v;
    while (num)
    {
        ++digits;
        num /= 16;
    }
    s64 write_len = digits + 2;
    while (v)
    {
        --digits;
        s64 rem = v % 16;
        text[digits] = (v % 16) >= 10 ? (v % 16) - 10 + 'a' : '0' + (v % 16);
        v /= 16;
    }
    return write_len;
}

_INLINE_ s64 u64_to_text_hex_padding(u64 v, char* text, s64 holds)
{
    s64 myhold = holds;
    while (--myhold >= 0)
    {
        s64 rem = v % 16;
        text[myhold] = (char)rem >= (char)10 ? (char)rem - (char)10 + 'a' : '0' + (char)rem;
        v /= 16;
    }
    return holds;
}

_INLINE_ boole text_to_s64(const char* text, s64 len, OUT s64& rst)
{
    if (len <= 0)
    {
        return boole::False;
    }
    boole neg = boole::False;
    auto* ptr = text;
    auto* ptr_end = text + len;
    if (*ptr == '-')
    {
        ++ptr;
        neg = boole::True;
    }

    rst = 0;
    while (ptr < ptr_end)
    {
        char c = *ptr;
        if (!is_digit(c))
        {
            return boole::False;
        }
        s64 digit = (s64)c - '0';
        if ((s64_max - digit) / 10 < rst)
        {
            return boole::False;
        }
        rst *= 10;
        rst += digit;
        ++ptr;
    }
    if (neg)
    {
        rst = -rst;
    }
    return boole::True;
}
