#pragma once

static void trim_index_from(const string& str, OUT s64& from, s64 to)
{
    while (from < to)
    {
        if (is_alike_blank_char(str[from]))
        {
            ++from;
        }
        else
        {
            break;
        }
    }
}

static void trim_index_to(const string& str, s64 from, OUT s64& to)
{
    while (from < to)
    {
        if (is_alike_blank_char(str[to - 1]))
        {
            --to;
        }
        else
        {
            break;
        }
    }
}

// cannot handle case "\\n"
static void trim_index(const string& str, OUT s64& from, OUT s64& to)
{
    trim_index_from(str, from, to);
    trim_index_to(str, from, to);
}

static boole is_escape_blank_char_pair(char ch1, char ch2)
{
    return
        ch1 == '\\' &&
        (ch2 == 't' || ch2 == 'n' || ch2 == 'r');
}

_INLINE_ static char char_pair_to_escape_char(char ch1, char ch2)
{
    if (ch1 == '\\')
    {
        switch (ch2)
        {
        case 'n':
        return '\n';
        case 't':
        return '\t';
        case 'r':
        return '\r';
        case '0':
        return '\0';
        case '\"':
        return '\"';
        default:
        break;
        }
    }
    return -1;
}

template<typename Fc>
_INLINE_ static s64 iterate_quotation_json_string(const string& str, s64 first_quotation, Fc function_each_char)
{
    s64 len = str.size();

    assert(first_quotation < len);
    assert(str[first_quotation] == '\"');

    s64 idx = first_quotation + 1;

    while (idx + 1 < len)
    {
        char ch1 = str[idx];
        if (ch1 != '\\')
        {
            if (ch1 == '\"')
            {
                // find tail 1
                return idx;
            }
            function_each_char(ch1);
            ++idx;
            continue;
        }
        char esc_ch = char_pair_to_escape_char(ch1, str[idx + 1]);
        if (esc_ch >= 0)
        {
            // escape charactor
            function_each_char(esc_ch);
            idx += 2;
            continue;
        }
        else
        {
            // error
            return -1;
        }
    }
    if (idx == len - 1 && str[idx] == '\"')
    {
        // find tail 1
        return idx;
    }
    // error
    return -1;
}
