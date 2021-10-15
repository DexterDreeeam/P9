#pragma once

template<typename Ty>
class atom;

template<>
class atom<s64>
{
public:
    atom() :
        _value()
    {}

    atom(const atom<s64>& rhs) :
        _value(rhs._value)
    {}

    atom(s64 v) :
        _value(v)
    {}

    ~atom() = default;

public:
    s64 get()
    {
        return _value;
    }

    void set(s64 v)
    {
        _value = v;
    }

    operator bool()
    {
        return _value != 0;
    }

    atom<s64>& operator =(const atom<s64>& rhs)
    {
        _value = rhs._value;
        return *this;
    }

    atom<s64>& operator =(s64 v)
    {
        _value = v;
        return *this;
    }

    s64 operator ++()
    {
        s64 ret = WindowsMsvcNs::InterlockedIncrement(reinterpret_cast<volatile u64*>(&_value));
        return ret;
    }

    s64 operator ++(int)
    {
        s64 ret = WindowsMsvcNs::InterlockedIncrement(reinterpret_cast<volatile u64*>(&_value));
        return ret - 1;
    }

    s64 operator --()
    {
        s64 ret = WindowsMsvcNs::InterlockedDecrement(reinterpret_cast<volatile u64*>(&_value));
        return ret;
    }

    s64 operator --(int)
    {
        s64 ret = WindowsMsvcNs::InterlockedDecrement(reinterpret_cast<volatile u64*>(&_value));
        return ret + 1;
    }

    s64 weak_add(s64 v)
    {
        return _value += v;
    }

    s64 exchange(s64 replace)
    {
        s64 ret = WindowsMsvcNs::InterlockedExchange(reinterpret_cast<volatile u64*>(&_value), (u64)replace);
        return ret;
    }

    s64 compare_exchange(s64 expected, s64 replace)
    {
        s64 ret = WindowsMsvcNs::InterlockedCompareExchange(reinterpret_cast<volatile u64*>(&_value), (u64)replace, (u64)expected);
        return ret;
    }

public:
    volatile s64 _value;
};

template<>
class atom<u64>
{
public:
    atom() :
        _value()
    {}

    atom(const atom<u64>& rhs) :
        _value(rhs._value)
    {}

    atom(u64 v) :
        _value(v)
    {}

    ~atom() = default;

public:
    u64 get()
    {
        return _value;
    }

    void set(u64 v)
    {
        _value = v;
    }

    operator bool()
    {
        return _value != 0;
    }

    atom<u64>& operator =(const atom<u64>& rhs)
    {
        _value = rhs._value;
        return *this;
    }

    atom<u64>& operator =(u64 v)
    {
        _value = v;
        return *this;
    }

    u64 operator ++()
    {
        u64 ret = WindowsMsvcNs::InterlockedIncrement(&_value);
        return ret;
    }

    u64 operator ++(int)
    {
        s64 ret = WindowsMsvcNs::InterlockedIncrement(&_value);
        return ret - 1;
    }

    u64 operator --()
    {
        u64 ret = WindowsMsvcNs::InterlockedDecrement(&_value);
        return ret;
    }

    u64 operator --(int)
    {
        u64 ret = WindowsMsvcNs::InterlockedDecrement(&_value);
        return ret + 1;
    }

    u64 weak_add(s64 v)
    {
        return _value += v;
    }

    u64 exchange(u64 replace)
    {
        u64 ret = WindowsMsvcNs::InterlockedExchange(&_value, replace);
        return ret;
    }

    u64 compare_exchange(u64 expected, u64 replace)
    {
        u64 ret = WindowsMsvcNs::InterlockedCompareExchange(&_value, replace, expected);
        return ret;
    }

public:
    volatile u64 _value;
};

template<typename Ty>
class atom<Ty*>
{
public:
    atom() :
        _value(nullptr)
    {}

    atom(const atom<Ty>& rhs) :
        _value(rhs._value)
    {}

    atom(Ty* v) :
        _value(v)
    {}

    ~atom() = default;

public:
    Ty* get()
    {
        return _value;
    }

    void set(Ty* v)
    {
        _value = v;
    }

    operator Ty* ()
    {
        return _value;
    }

    operator bool()
    {
        return _value != nullptr;
    }

    atom<Ty*>& operator =(const atom<Ty*>& rhs)
    {
        _value = rhs._value;
        return *this;
    }

    atom<Ty*>& operator =(Ty* v)
    {
        _value = v;
        return *this;
    }

    Ty* exchange(Ty* replace)
    {
        void* ret = WindowsMsvcNs::InterlockedExchange(&_value, replace);
        return pointer_convert(ret, 0, Ty*);
    }

    Ty* compare_exchange(Ty* expected, Ty* replace)
    {
        void* ret = WindowsMsvcNs::InterlockedCompareExchange(&_value, replace, expected);
        return pointer_convert(ret, 0, Ty*);
    }

public:
    volatile Ty* _value;
};
