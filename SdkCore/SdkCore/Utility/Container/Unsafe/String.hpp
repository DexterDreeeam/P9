#pragma once

class string;

namespace _Internal {
namespace _String {

const s64 string_unit_extent = 4LL;
const s64 string_cap_max = 1LL << 30;

class string_iter;
class string_ritr;
class string_const_iter;
class string_const_ritr;

class string_iter
{
    friend class string;
    using Self_Ty = string_iter;

public:
    string_iter() :
        ptr(nullptr)
    {}

    string_iter(const Self_Ty& rhs) :
        ptr(rhs.ptr)
    {}

    string_iter(char* p) :
        ptr(p)
    {}

    ~string_iter() noexcept = default;

    Self_Ty& operator =(const Self_Ty& rhs) noexcept
    {
        ptr = rhs.ptr;
        return *this;
    }

    Self_Ty& operator ++() noexcept
    {
        ptr = pointer_convert(ptr, 1, char*);
        return *this;
    }

    Self_Ty& operator --() noexcept
    {
        ptr = pointer_convert(ptr, -1, char*);
        return *this;
    }

    Self_Ty operator ++(int) noexcept
    {
        Self_Ty ret(ptr);
        ptr = pointer_convert(ptr, 1, char*);
        return ret;
    }

    Self_Ty operator --(int) noexcept
    {
        Self_Ty ret(ptr);
        ptr = pointer_convert(ptr, -1, char*);
        return ret;
    }

    bool operator ==(const Self_Ty& rhs) const noexcept
    {
        return ptr == rhs.ptr;
    }

    bool operator !=(const Self_Ty& rhs) const noexcept
    {
        return ptr != rhs.ptr;
    }

    char& operator *() noexcept
    {
        return *ptr;
    }

    s64 operator -(const Self_Ty& rhs) const noexcept
    {
        s64 diff = reinterpret_cast<s64>(ptr) - reinterpret_cast<s64>(rhs.ptr);
        return diff;
    }

private:
    char* ptr;
};

class string_ritr
{
    friend class string;
    using Self_Ty = string_ritr;

public:
    string_ritr() :
        ptr(nullptr)
    {}

    string_ritr(const Self_Ty& rhs) :
        ptr(rhs.ptr)
    {}

    string_ritr(char* p) :
        ptr(p)
    {}

    ~string_ritr() noexcept = default;

    Self_Ty& operator =(const Self_Ty& rhs) noexcept
    {
        ptr = rhs.ptr;
        return *this;
    }

    Self_Ty& operator ++() noexcept
    {
        ptr = pointer_convert(ptr, -1, char*);
        return *this;
    }

    Self_Ty& operator --() noexcept
    {
        ptr = pointer_convert(ptr, 1, char*);
        return *this;
    }

    Self_Ty operator ++(int) noexcept
    {
        Self_Ty ret(ptr);
        ptr = pointer_convert(ptr, -1, char*);
        return ret;
    }

    Self_Ty operator --(int) noexcept
    {
        Self_Ty ret(ptr);
        ptr = pointer_convert(ptr, 1, char*);
        return ret;
    }

    bool operator ==(const Self_Ty& rhs) const noexcept
    {
        return ptr == rhs.ptr;
    }

    bool operator !=(const Self_Ty& rhs) const noexcept
    {
        return ptr != rhs.ptr;
    }

    char& operator *() noexcept
    {
        return *ptr;
    }

    s64 operator -(const Self_Ty& rhs) const noexcept
    {
        s64 diff = reinterpret_cast<s64>(ptr) - reinterpret_cast<s64>(rhs.ptr);
        return diff;
    }

private:
    char* ptr;
};

class string_const_iter
{
    friend class string;
    using Self_Ty = string_const_iter;

public:
    string_const_iter() :
        ptr(nullptr)
    {}

    string_const_iter(const Self_Ty& rhs) :
        ptr(rhs.ptr)
    {}

    string_const_iter(char* p) :
        ptr(p)
    {}

    ~string_const_iter() noexcept = default;

    Self_Ty& operator =(const Self_Ty& rhs) noexcept
    {
        ptr = rhs.ptr;
        return *this;
    }

    Self_Ty& operator ++() noexcept
    {
        ptr = pointer_convert(ptr, 1, char*);
        return *this;
    }

    Self_Ty& operator --() noexcept
    {
        ptr = pointer_convert(ptr, -1, char*);
        return *this;
    }

    Self_Ty operator ++(int) noexcept
    {
        Self_Ty ret(ptr);
        ptr = pointer_convert(ptr, 1, char*);
        return ret;
    }

    Self_Ty operator --(int) noexcept
    {
        Self_Ty ret(ptr);
        ptr = pointer_convert(ptr, -1, char*);
        return ret;
    }

    bool operator ==(const Self_Ty& rhs) const noexcept
    {
        return ptr == rhs.ptr;
    }

    bool operator !=(const Self_Ty& rhs) const noexcept
    {
        return ptr != rhs.ptr;
    }

    const char& operator *() const noexcept
    {
        return *ptr;
    }

    s64 operator -(const Self_Ty& rhs) const noexcept
    {
        s64 diff = reinterpret_cast<s64>(ptr) - reinterpret_cast<s64>(rhs.ptr);
        return diff;
    }

private:
    char* ptr;
};

class string_const_ritr
{
    friend class string;
    using Self_Ty = string_const_ritr;

public:
    string_const_ritr() :
        ptr(nullptr)
    {}

    string_const_ritr(const Self_Ty& rhs) :
        ptr(rhs.ptr)
    {}

    string_const_ritr(char* p) :
        ptr(p)
    {}

    ~string_const_ritr() noexcept = default;

    Self_Ty& operator =(const Self_Ty& rhs) noexcept
    {
        ptr = rhs.ptr;
        return *this;
    }

    Self_Ty& operator ++() noexcept
    {
        ptr = pointer_convert(ptr, -1, char*);
        return *this;
    }

    Self_Ty& operator --() noexcept
    {
        ptr = pointer_convert(ptr, 1, char*);
        return *this;
    }

    Self_Ty operator ++(int) noexcept
    {
        Self_Ty ret(ptr);
        ptr = pointer_convert(ptr, -1, char*);
        return ret;
    }

    Self_Ty operator --(int) noexcept
    {
        Self_Ty ret(ptr);
        ptr = pointer_convert(ptr, 1, char*);
        return ret;
    }

    bool operator ==(const Self_Ty& rhs) const noexcept
    {
        return ptr == rhs.ptr;
    }

    bool operator !=(const Self_Ty& rhs) const noexcept
    {
        return ptr != rhs.ptr;
    }

    const char& operator *() const noexcept
    {
        return *ptr;
    }

    s64 operator -(const Self_Ty& rhs) const noexcept
    {
        s64 diff = reinterpret_cast<s64>(ptr) - reinterpret_cast<s64>(rhs.ptr);
        return diff;
    }

private:
    char* ptr;
};

}} // _Internal::_String

class string
{
    using Iter_Ty  = _Internal::_String::string_iter;
    using Ritr_Ty  = _Internal::_String::string_ritr;
    using cIter_Ty = _Internal::_String::string_const_iter;
    using cRitr_Ty = _Internal::_String::string_const_ritr;

    friend class _Internal::_String::string_iter;
    friend class _Internal::_String::string_ritr;
    friend class _Internal::_String::string_const_iter;
    friend class _Internal::_String::string_const_ritr;

    friend string operator +(char c, const string& s) noexcept;
    friend string operator +(const char* cstr, const string& s) noexcept;
    friend bool   operator ==(const char* cstr, const string& s) noexcept;
    friend bool   operator !=(const char* cstr, const string& s) noexcept;
    friend bool   operator <(const char* cstr, const string& s) noexcept;
    friend bool   operator >(const char* cstr, const string& s) noexcept;
    friend bool   operator <=(const char* cstr, const string& s) noexcept;
    friend bool   operator >=(const char* cstr, const string& s) noexcept;

    struct string_max_len_expectation
    {
        explicit string_max_len_expectation(s64 exp) :
            max_len(exp)
        {}
        ~string_max_len_expectation() = default;
        s64 max_len;
    };

public:
    string() noexcept :
        cap(0),
        len(0),
        ch(nullptr)
    {
        assert(_validation());
    }

    string(s64 vol, char init_char) :
        cap(_ceil_align(vol + 1)),
        len(vol),
        ch((char*)memory::alloc<void>(cap))
    {
        memory::set(ch, init_char, vol);
        _set_tail_zero();
    }

    string(const char* s) :
        string(s, str_len(s))
    {
        //s64 s_len = str_len(s);
        //cap = _ceil_align(s_len + 1);
        //len = s_len;
        //ch = (char*)memory::alloc_copy<void>(s, cap, len + 1);
        //assert(_validation());
    }

    string(const char* s, s64 s_len)
    {
        cap = _ceil_align(s_len + 1);
        len = s_len;
        ch = memory::alloc_copy<char>(s, cap, len);
        _set_tail_zero();
        assert(_validation());
    }

    string(const string& rhs) :
        cap(_ceil_align(rhs.len + 1)),
        len(rhs.len),
        ch(memory::alloc_copy<char>(rhs.ch, cap, len))
    {
        _set_tail_zero();
        assert(_validation());
    }

    string(string&& rhs) noexcept :
        cap(rhs.cap),
        len(rhs.len),
        ch(rhs.ch)
    {
        rhs.cap = 0;
        rhs.len = 0;
        rhs.ch = nullptr;
        assert(_validation());
    }

    ~string() noexcept
    {
        assert(_validation());
        clear();
    }

    string& operator =(const string& rhs) noexcept
    {
        clear();
        cap = _ceil_align(rhs.len + 1);
        len = rhs.len;
        ch = memory::alloc_copy<char>(rhs.ch, cap, len);
        _set_tail_zero();
        assert(_validation());
        return *this;
    }

    string& operator =(string&& rhs) noexcept
    {
        clear();
        cap = rhs.cap;
        len = rhs.len;
        ch = rhs.ch;
        rhs.cap = 0;
        rhs.len = 0;
        rhs.ch = nullptr;
        assert(_validation());
        return *this;
    }

    boole operator ==(const char* rhs) const noexcept
    {
        return str_equal(ch ? ch : "", rhs ? rhs : "");
    }

    boole operator ==(const string& rhs) const noexcept
    {
        return str_equal(ch ? ch : "", rhs.ch ? rhs.ch : "");
    }

    boole operator !=(const string& rhs) const noexcept
    {
        return !str_equal(ch ? ch : "", rhs.ch ? rhs.ch : "");
    }

    boole operator !=(const char* rhs) const noexcept
    {
        return !str_equal(ch ? ch : "", rhs ? rhs : "");
    }

    boole operator <(const string& rhs) const noexcept
    {
        return str_compare(ch ? ch : "", rhs.ch ? rhs.ch : "") < 0;
    }

    boole operator <(const char* cstr) const noexcept
    {
        return str_compare(ch ? ch : "", cstr) < 0;
    }

    boole operator >(const string& rhs) const noexcept
    {
        return str_compare(ch ? ch : "", rhs.ch ? rhs.ch : "") > 0;
    }

    boole operator >(const char* cstr) const noexcept
    {
        return str_compare(ch ? ch : "", cstr) > 0;
    }

    boole operator <=(const string& rhs) const noexcept
    {
        return str_compare(ch ? ch : "", rhs.ch ? rhs.ch : "") <= 0;
    }

    boole operator <=(const char* cstr) const noexcept
    {
        return str_compare(ch ? ch : "", cstr) <= 0;
    }

    boole operator >=(const string& rhs) const noexcept
    {
        return str_compare(ch ? ch : "", rhs.ch ? rhs.ch : "") >= 0;
    }

    boole operator >=(const char* cstr) const noexcept
    {
        return str_compare(ch ? ch : "", cstr) >= 0;
    }

    char& operator [](s64 pos) noexcept
    {
        assert(_validation());
        return ch[pos];
    }

    const char& operator [](s64 pos) const noexcept
    {
        assert(_validation());
        return ch[pos];
    }

    string operator +(const string& rhs) const noexcept
    {
        string ret(string_max_len_expectation(len + rhs.len));
        memory::copy(ch, ret.ch, len);
        memory::copy(rhs.ch, pointer_convert(ret.ch, len, void*), rhs.len);
        ret.len = len + rhs.len;
        ret._set_tail_zero();
        assert(ret._validation());
        return ret;
    }

    string operator +(const char* cstr) const noexcept
    {
        s64 cstr_len = str_len(cstr);
        string ret(string_max_len_expectation(len + cstr_len));
        memory::copy(ch, ret.ch, len);
        memory::copy(cstr, pointer_convert(ret.ch, len, void*), cstr_len + 1);
        ret.len = len + cstr_len;
        assert(ret._validation());
        return ret;
    }

    string operator +(char c) noexcept
    {
        string ret(string_max_len_expectation(len + 1));
        memory::copy(ch, ret.ch, len);
        ret.ch[len] = c;
        ret.ch[len + 1] = 0;
        ret.len = len + 1;
        assert(ret._validation());
        return ret;
    }

    string& operator +=(char c) noexcept
    {
        assert(_validation());
        _push_back(c);
        assert(_validation());
        return *this;
    }

    string& operator +=(const string& rhs) noexcept
    {
        assert(_validation());
        _push_back(rhs);
        assert(_validation());
        return *this;
    }

    string& operator +=(const char* cstr) noexcept
    {
        assert(_validation());
        _push_back(cstr);
        assert(_validation());
        return *this;
    }

    void clear()
    {
        assert(_validation());
        cap = 0;
        len = 0;
        if (ch)
        {
            memory::free(ch);
            ch = nullptr;
        }
    }

    s64 size() const noexcept
    {
        assert(_validation());
        return len;
    }

    bool empty() const noexcept
    {
        assert(_validation());
        return len == 0;
    }

    char* data() noexcept
    {
        assert(_validation());
        return ch;
    }

    const char* data() const noexcept
    {
        assert(_validation());
        return ch;
    }

    const char* c_str() const noexcept
    {
        assert(_validation());
        return ch;
    }

    void swap(string& rhs) noexcept
    {
        assert(_validation());
        ::swap(cap, rhs.cap);
        ::swap(len, rhs.len);
        ::swap(ch, rhs.ch);
        assert(_validation());
    }

    char& front() noexcept
    {
        assert(len > 0);
        return ch[0];
    }

    const char& front() const noexcept
    {
        assert(len > 0);
        return ch[0];
    }

    char& back() noexcept
    {
        assert(len > 0);
        return ch[len - 1];
    }

    const char& back() const noexcept
    {
        assert(len > 0);
        return ch[len - 1];
    }

    void push_back(char c) noexcept
    {
        assert(_validation());
        _push_back(c);
    }

    void pop_back() noexcept
    {
        assert(_validation());
        assert(len > 0);
        --len;
        _set_tail_zero();
    }

    void pop_back(s64 pop_number) noexcept
    {
        assert(_validation());
        assert(len >= pop_number);
        len -= pop_number;
        _set_tail_zero();
    }

    string substr(s64 pos) const noexcept
    {
        assert(_validation());
        assert(pos < len);
        return substr(pos, len - pos);
    }

    string substr(s64 pos, s64 sublen) const noexcept
    {
        assert(_validation());
        assert(pos < len);
        assert(pos + sublen <= len);
        return string(data() + pos, sublen);
    }

    boole contains(const string& str) const noexcept
    {
        for (s64 i = 0; size() - i >= str.size(); ++i)
        {
            if (*this == substr(i, str.size()))
            {
                return boole::True;
            }
        }
        return boole::False;
    }

    boole start_with(const string& str) const noexcept
    {
        if (size() < str.size())
        {
            return boole::False;
        }
        return substr(0, str.size()) == str;
    }

    boole end_with(const string& str) const noexcept
    {
        if (size() < str.size())
        {
            return boole::False;
        }
        return substr(size() - str.size()) == str;
    }

    string reverse() const noexcept
    {
        string rst = "";
        for (auto itr = rbegin(); itr != rend(); ++itr)
        {
            rst += *itr;
        }
        return rst;
    }

    Iter_Ty begin() noexcept
    {
        return Iter_Ty(ch);
    }

    cIter_Ty begin() const noexcept
    {
        return cIter_Ty(ch);
    }

    Iter_Ty end() noexcept
    {
        return Iter_Ty(ch + len);
    }

    cIter_Ty end() const noexcept
    {
        return cIter_Ty(ch + len);
    }

    Ritr_Ty rbegin() noexcept
    {
        return Ritr_Ty(ch + len - 1);
    }

    cRitr_Ty rbegin() const noexcept
    {
        return cRitr_Ty(ch + len - 1);
    }

    Ritr_Ty rend() noexcept
    {
        return Ritr_Ty(ch - 1);
    }

    cRitr_Ty rend() const noexcept
    {
        return cRitr_Ty(ch - 1);
    }

    cIter_Ty cbegin() noexcept
    {
        return cIter_Ty(ch);
    }

    cIter_Ty cend() noexcept
    {
        return cIter_Ty(ch + len);
    }

    cRitr_Ty crbegin() noexcept
    {
        return cRitr_Ty(ch + len - 1);
    }

    cRitr_Ty crend() noexcept
    {
        return cRitr_Ty(ch - 1);
    }

private:
    string(const string_max_len_expectation& exp) :
        cap(_ceil_align(exp.max_len + 1)),
        len(0),
        ch((char*)memory::alloc<void>(cap))
    {}

    static s64 _ceil_align(s64 len) noexcept
    {
        return math::ceil(len, _Internal::_String::string_unit_extent);
    }

    bool _validation() const noexcept
    {
        if (cap == 0)
        {
            if (len != 0 || ch)
            {
                assert_info(0, "Invalid string internal state.");
                return false;
            }
        }
        else if (ch == nullptr && len != 0)
        {
            assert_info(0, "Invalid string internal state.");
            return false;
        }
        else if (ch != nullptr && cap == 0)
        {
            assert_info(0, "Invalid string internal state.");
            return false;
        }
        else if (len >= cap)
        {
            assert_info(0, "Invalid string internal state.");
            return false;
        }

        if (ch == nullptr && cap != 0)
        {
            assert_info(0, "Invalid string internal state.");
            return false;
        }

        if (ch == nullptr)
        {
            return true;
        }

        if (ch[len] != 0)
        {
            assert_info(0, "String tail charactor should be '\\0'");
            return false;
        }
        else
        {
            for (s64 i = 0; i < len; ++i)
            {
                if (ch[i] == 0 || (ch[i] & (1 << 7)))
                {
                    assert_info(0, "String every charactor should not be '\\0' and in range of '\\1' to '\\127'");
                    return false;
                }
            }
        }
        return true;
    }

    void _set_tail_zero() noexcept
    {
        assert(len < cap);
        ch[len] = 0;
    }

    void _need_space(s64 add) noexcept
    {
        if (len + add < cap)
        {
            return;
        }
        while (len + add >= cap)
        {
            cap = cap == 0 ? _Internal::_String::string_unit_extent : (cap << 1);
            assert(cap <= _Internal::_String::string_cap_max);
        }
        char* new_ch = memory::alloc_copy<char>(ch, cap, len);
        if (ch)
        {
            memory::free(ch);
        }
        ch = new_ch;
        _set_tail_zero();
    }

    void _push_back(char c) noexcept
    {
        _need_space(1);
        ch[len] = c;
        ++len;
        _set_tail_zero();
    }

    void _push_back(const string& rhs) noexcept
    {
        _need_space(rhs.len);
        memory::copy(rhs.ch, &ch[len], rhs.len);
        len += rhs.len;
        _set_tail_zero();
    }

    void _push_back(const char* cstr) noexcept
    {
        s64 cstr_len = str_len(cstr);
        _need_space(cstr_len);
        memory::copy(cstr, &ch[len], cstr_len + 1);
        len += cstr_len;
    }

private:
    s64 cap;
    s64 len;
    char* ch;
};

_INLINE_ string operator +(char c, const string& s) noexcept
{
    string ret(string::string_max_len_expectation(s.len + 1));
    ret.ch[0] = c;
    memory::copy(s.ch, &ret.ch[1], s.len);
    ret.len = s.len + 1;
    ret._set_tail_zero();
    assert(ret._validation());
    return ret;
}

_INLINE_ string operator +(const char* cstr, const string& s) noexcept
{
    s64 cstr_len = str_len(cstr);
    string ret(string::string_max_len_expectation(cstr_len + s.len));
    memory::copy(cstr, ret.ch, cstr_len);
    memory::copy(s.ch, pointer_convert(ret.ch, cstr_len, void*), s.len);
    ret.len = cstr_len + s.len;
    ret._set_tail_zero();
    assert(ret._validation());
    return ret;
}

_INLINE_ bool operator ==(const char* cstr, const string& s) noexcept
{
    return str_equal(cstr, s.ch ? s.ch : "");
}

_INLINE_ bool operator !=(const char* cstr, const string& s) noexcept
{
    return !str_equal(cstr, s.ch ? s.ch : "");
}

_INLINE_ bool operator <(const char* cstr, const string& s) noexcept
{
    return str_compare(cstr, s.ch ? s.ch : "") < 0;
}

_INLINE_ bool operator >(const char* cstr, const string& s) noexcept
{
    return str_compare(cstr, s.ch ? s.ch : "") > 0;
}

_INLINE_ bool operator <=(const char* cstr, const string& s) noexcept
{
    return str_compare(cstr, s.ch ? s.ch : "") <= 0;
}

_INLINE_ bool operator >=(const char* cstr, const string& s) noexcept
{
    return str_compare(cstr, s.ch ? s.ch : "") >= 0;
}

template<typename Int_Ty>
_INLINE_ string to_string(Int_Ty integer) noexcept;

template<>
_INLINE_ string to_string<u64>(u64 integer) noexcept
{
    if (integer == 0)
    {
        return string("0");
    }
    s64 digits = 0;
    u64 num = integer;
    while (num)
    {
        ++digits;
        num /= 10;
    }
    string rst(digits, '0');
    while (integer)
    {
        --digits;
        rst[digits] = '0' + integer % 10;
        integer /= 10;
    }
    return rst;
}

template<>
_INLINE_ string to_string<s64>(s64 integer) noexcept
{
    if (integer == 0)
    {
        return string("0");
    }
    if (integer == s64_min)
    {
        return string("-9223372036854775808");
    }
    s64 digits = 0;
    s64 num = integer;
    while (num)
    {
        ++digits;
        num /= 10;
    }
    if (integer < 0)
    {
        ++digits;
        integer = -integer;
    }
    string rst(digits, '-');
    while (integer)
    {
        --digits;
        rst[digits] = '0' + integer % 10;
        integer /= 10;
    }
    return rst;
}

template<>
_INLINE_ string to_string<u32>(u32 integer) noexcept
{
    return to_string<u64>(integer);
}

template<>
_INLINE_ string to_string<s32>(s32 integer) noexcept
{
    return to_string<s64>(integer);
}

template<>
_INLINE_ string to_string<unsigned int>(unsigned int integer) noexcept
{
    return to_string<u64>(integer);
}

template<>
_INLINE_ string to_string<int>(int integer) noexcept
{
    return to_string<s64>(integer);
}

template<>
_INLINE_ string to_string<u16>(u16 integer) noexcept
{
    return to_string<u64>(integer);
}

template<>
_INLINE_ string to_string<s16>(s16 integer) noexcept
{
    return to_string<s64>(integer);
}

template<>
_INLINE_ string to_string<u8>(u8 integer) noexcept
{
    return to_string<u64>(integer);
}

template<>
_INLINE_ string to_string<s8>(s8 integer) noexcept
{
    return to_string<s64>(integer);
}

_INLINE_ s32 stoi(const string& str) noexcept
{
    if (str.size() == 0)
    {
        return 0;
    }
    const char* cstr = str.c_str();
    s64 max_digits;
    if (str[0] >= '0' && str[0] <= '9')
    {
        max_digits = 10;
    }
    else if (str[0] == '+' || str[0] == '-')
    {
        ++cstr;
        max_digits = 11;
    }
    else
    {
        return 0;
    }
    if (str.size() > max_digits || (str.size() == max_digits && str_compare(cstr, "2147483647") > 0))
    {
        return str[0] == '-' ? s32_min : s32_max;
    }
    s32 rst = 0;
    while (*cstr)
    {
        if (*cstr < '0' || *cstr > '9')
        {
            return 0;
        }
        rst *= 10;
        rst += *cstr - '0';
        ++cstr;
    }
    return str[0] == '-' ? -rst : rst;
}

_INLINE_ s64 stoll(const string& str) noexcept
{
    if (str.size() == 0)
    {
        return 0;
    }
    const char* cstr = str.c_str();
    s64 max_digits;
    if (str[0] >= '0' && str[0] <= '9')
    {
        max_digits = 19;
    }
    else if (str[0] == '+' || str[0] == '-')
    {
        ++cstr;
        max_digits = 20;
    }
    else
    {
        return 0;
    }
    if (str.size() > max_digits || (str.size() == max_digits && str_compare(cstr, "9223372036854775807") > 0))
    {
        return str[0] == '-' ? s64_min : s64_max;
    }
    s64 rst = 0;
    while (*cstr)
    {
        if (*cstr < '0' || *cstr > '9')
        {
            return 0;
        }
        rst *= 10;
        rst += (s64)*cstr - '0';
        ++cstr;
    }
    return str[0] == '-' ? -rst : rst;
}
