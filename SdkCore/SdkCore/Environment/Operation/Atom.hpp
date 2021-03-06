#pragma once

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

    template<typename ValTy>
    atom(const ValTy& v);

    ~atom();

public:
    s64 get() const;

    void set(s64 v);

    template<typename ValTy>
    void set(const ValTy& v);

    atom<s64>& operator =(const atom<s64>& rhs);

    atom<s64>& operator =(s64 v);

    template<typename ValTy>
    atom<s64>& operator =(const ValTy& v);

    template<typename Convert_Ty>
    operator Convert_Ty() const = delete;

    s64 operator ++();

    s64 operator ++(int);

    s64 operator --();

    s64 operator --(int);

    s64 weak_add(s64 v);

    s64 exchange(s64 replace);

    template<typename ValTy>
    s64 exchange(const ValTy& replace);

    s64 compare_exchange(s64 expected, s64 replace);

    template<typename ValTy1, typename ValTy2>
    s64 compare_exchange(const ValTy1& expected, const ValTy2& replace);

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

    template<typename ValTy>
    atom(const ValTy& v);

    ~atom();

public:
    u64 get() const;

    void set(u64 v);

    template<typename ValTy>
    void set(const ValTy& v);

    atom<u64>& operator =(const atom<u64>& rhs);

    atom<u64>& operator =(u64 v);

    template<typename ValTy>
    atom<u64>& operator =(const ValTy& v);

    template<typename Convert_Ty>
    operator Convert_Ty() const = delete;

    u64 operator ++();

    u64 operator ++(int);

    u64 operator --();

    u64 operator --(int);

    u64 weak_add(s64 v);

    u64 exchange(u64 replace);

    template<typename ValTy>
    u64 exchange(const ValTy& replace);

    u64 compare_exchange(u64 expected, u64 replace);

    template<typename ValTy1, typename ValTy2>
    u64 compare_exchange(const ValTy1& expected, const ValTy2& replace);

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
    void* get() const;

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
    atom();

    atom(const atom<Ty*>& rhs);

    atom(Ty* v);

    ~atom() = default;

public:
    Ty* get() const;

    void set(Ty* v);

    operator Ty* ();

    atom<Ty*>& operator =(const atom<Ty*>& rhs);

    atom<Ty*>& operator =(Ty* v);

    template<typename Convert_Ty>
    operator Convert_Ty() const = delete;

    Ty* exchange(Ty* replace);

    Ty* compare_exchange(Ty* expected, Ty* replace);

private:
    atom<void*> _shadow;
};
