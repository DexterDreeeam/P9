#pragma once

#include "../BuildinType.hpp"

template<typename Ty>
class atom;

template<>
class atom<s64>
{
    static const u64 _mem_sz = 8;

public:
    atom();

    atom(const atom<s64>& rhs);

    atom(s64 v);

    ~atom();

public:
    s64 get();

    void set(s64 v);

    atom<s64>& operator =(const atom<s64>& rhs);

    atom<s64>& operator =(s64 v);

    template<typename Convert_Ty>
    operator Convert_Ty() const = delete;

    s64 operator ++();

    s64 operator ++(int);

    s64 operator --();

    s64 operator --(int);

    s64 weak_add(s64 v);

    s64 exchange(s64 replace);

    s64 compare_exchange(s64 expected, s64 replace);

private:
    char _mem[_mem_sz];
};

template<>
class atom<u64>
{
    static const u64 _mem_sz = 8;

public:
    atom();

    atom(const atom<u64>& rhs);

    atom(u64 v);

    ~atom();

public:
    u64 get();

    void set(u64 v);

    atom<u64>& operator =(const atom<u64>& rhs);

    atom<u64>& operator =(u64 v);

    template<typename Convert_Ty>
    operator Convert_Ty() const = delete;

    u64 operator ++();

    u64 operator ++(int);

    u64 operator --();

    u64 operator --(int);

    u64 weak_add(s64 v);

    u64 exchange(u64 replace);

    u64 compare_exchange(u64 expected, u64 replace);

private:
    char _mem[_mem_sz];
};

template<>
class atom<void*>
{
    static const u64 _mem_sz = 8;

public:
    atom();

    atom(const atom<void*>& rhs);

    atom(void* v);

    ~atom();

public:
    void* get();

    void set(void* v);

    operator void* ();

    atom<void*>& operator =(const atom<void*>& rhs);

    atom<void*>& operator =(void* v);

    template<typename Convert_Ty>
    operator Convert_Ty() const = delete;

    void* exchange(void* replace);

    void* compare_exchange(void* expected, void* replace);

private:
    char _mem[_mem_sz];
};

template<typename Ty>
class atom<Ty*>
{
public:
    atom() :
        _shadow()
    {
    }

    atom(const atom<Ty*>& rhs) :
        _shadow(rhs._shadow)
    {
    }

    atom(Ty* v) :
        _shadow(v)
    {
    }

    ~atom() = default;

public:
    Ty* get()
    {
        return (Ty*)_shadow.get();
    }

    void set(Ty* v)
    {
        _shadow.set(v);
    }

    operator Ty* ()
    {
        return (Ty*)_shadow.operator void*();
    }

    atom<Ty*>& operator =(const atom<Ty*>& rhs)
    {
        _shadow.operator =(rhs._shadow);
        return *this;
    }

    atom<Ty*>& operator =(Ty* v)
    {
        _shadow.operator =(v);
        return *this;
    }

    template<typename Convert_Ty>
    operator Convert_Ty() const = delete;

    Ty* exchange(Ty* replace)
    {
        return (Ty*)_shadow.exchange(replace);
    }

    Ty* compare_exchange(Ty* expected, Ty* replace)
    {
        return (Ty*)_shadow.compare_exchange(expected, replace);
    }

private:
    atom<void*> _shadow;
};
