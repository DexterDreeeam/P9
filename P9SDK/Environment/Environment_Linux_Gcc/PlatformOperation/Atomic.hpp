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
        _value(rhs.get())
    {}

    atom(s64 v) :
        _value(v)
    {}

    ~atom() = default;

public:
    s64 get() const
    {
        return _value.load();
    }

    void set(s64 v)
    {
        _value.store(v);
    }

    template<typename Convert_Ty>
    operator Convert_Ty() const = delete;

    atom<s64>& operator =(const atom<s64>& rhs)
    {
        set(rhs.get());
        return *this;
    }

    atom<s64>& operator =(s64 v)
    {
        set(v);
        return *this;
    }

    bool operator ==(s64 v)
    {
        return get() == v;
    }

    bool operator !=(s64 v)
    {
        return get() != v;
    }

    bool operator >(s64 v)
    {
        return get() > v;
    }

    bool operator <(s64 v)
    {
        return get() < v;
    }

    bool operator >=(s64 v)
    {
        return get() >= v;
    }

    bool operator <=(s64 v)
    {
        return get() <= v;
    }

    s64 operator ++()
    {
        s64 ret = ++_value;
        return ret;
    }

    s64 operator ++(int)
    {
        s64 ret = _value++;
        return ret;
    }

    s64 operator --()
    {
        s64 ret = --_value;
        return ret;
    }

    s64 operator --(int)
    {
        s64 ret = _value--;
        return ret;
    }

    s64 weak_add(s64 v)
    {
        return _value += v;
    }

    s64 exchange(s64 replace)
    {
        s64 ret = _value.exchange(replace);
        return ret;
    }

    s64 compare_exchange(s64 expected, s64 replace)
    {
        _value.compare_exchange_strong(expected, replace);
        return expected;
    }

public:
    std::atomic<s64> _value;
};

template<>
class atom<u64>
{
public:
    atom() :
        _value()
    {}

    atom(const atom<u64>& rhs) :
        _value(rhs.get())
    {}

    atom(u64 v) :
        _value(v)
    {}

    ~atom() = default;

public:
    u64 get() const
    {
        return _value.load();
    }

    void set(u64 v)
    {
        _value.store(v);
    }

    operator bool()
    {
        return get() != 0;
    }

    atom<u64>& operator =(const atom<u64>& rhs)
    {
        set(rhs._value);
        return *this;
    }

    atom<u64>& operator =(u64 v)
    {
        set(v);
        return *this;
    }

    bool operator ==(u64 v)
    {
        return get() == v;
    }

    bool operator !=(u64 v)
    {
        return get() != v;
    }

    bool operator >(u64 v)
    {
        return get() > v;
    }

    bool operator <(u64 v)
    {
        return get() < v;
    }

    bool operator >=(u64 v)
    {
        return get() >= v;
    }

    bool operator <=(u64 v)
    {
        return get() <= v;
    }

    u64 operator ++()
    {
        u64 ret = ++_value;
        return ret;
    }

    u64 operator ++(int)
    {
        u64 ret = _value++;
        return ret;
    }

    u64 operator --()
    {
        u64 ret = --_value;
        return ret;
    }

    u64 operator --(int)
    {
        u64 ret = _value--;
        return ret;
    }

    u64 weak_add(u64 v)
    {
        return _value += v;
    }

    u64 exchange(u64 replace)
    {
        u64 ret = _value.exchange(replace);
        return ret;
    }

    u64 compare_exchange(u64 expected, u64 replace)
    {
        _value.compare_exchange_strong(expected, replace);
        return expected;
    }

public:
    std::atomic<u64> _value;
};

template<typename Ty>
class atom<Ty*>
{
public:
    atom() :
        _value(nullptr)
    {}

    atom(const atom<Ty>& rhs) :
        _value(rhs._value.get())
    {}

    atom(Ty* v) :
        _value(v)
    {}

    ~atom() = default;

public:
    Ty* get() const
    {
        return _value.load();
    }

    void set(Ty* v)
    {
        _value.store(v);
    }

    operator Ty* ()
    {
        return get();
    }

    operator bool()
    {
        return get() != nullptr;
    }

    atom<Ty*>& operator =(const atom<Ty*>& rhs)
    {
        _value.set(rhs._value);
        return *this;
    }

    atom<Ty*>& operator =(Ty* v)
    {
        _value.set(v);
        return *this;
    }

    bool operator ==(Ty* v)
    {
        return get() == v;
    }

    bool operator !=(Ty* v)
    {
        return get() != v;
    }

    Ty* exchange(Ty* replace)
    {
        Ty* ret = _value.exchange(replace);
        return ret;
    }

    Ty* compare_exchange(Ty* expected, Ty* replace)
    {
        _value.compare_exchange_strong(expected, replace);
        return expected;
    }

public:
    std::atomic<Ty*> _value;
};
