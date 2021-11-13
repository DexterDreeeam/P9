
#include "../../Interface.hpp"
#include "../EnvironmentHeader.hpp"

template<typename Ty>
class shadow_class
{
public:
    using DataType = volatile Ty;

    DataType _data;
};

atom<s64>::atom()
{
    assert(_mem_sz >= sizeof(shadow_class<s64>));
    new (_mem) volatile s64(0);
}

atom<s64>::atom(const atom<s64>& rhs)
{
    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class<s64>*);
    auto& shadow_rhs = *pointer_convert(rhs._mem, 0, shadow_class<s64>*);

    shadow_self._data = shadow_rhs._data;
}

atom<s64>::atom(s64 v)
{
    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class<s64>*);

    shadow_self._data = v;
}

atom<s64>::~atom()
{
}

s64 atom<s64>::get() const
{
    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class<s64>*);

    return shadow_self._data;
}

void atom<s64>::set(s64 v)
{
    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class<s64>*);

    shadow_self._data = v;
}

atom<s64>& atom<s64>::operator =(const atom<s64>& rhs)
{
    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class<s64>*);
    auto& shadow_rhs = *pointer_convert(rhs._mem, 0, shadow_class<s64>*);

    shadow_self._data = shadow_rhs._data;
    return *this;
}

atom<s64>& atom<s64>::operator =(s64 v)
{
    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class<s64>*);

    shadow_self._data = v;
    return *this;
}

s64 atom<s64>::operator ++()
{
    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class<s64>*);

    u64 ret = InterlockedIncrement(reinterpret_cast<volatile u64*>(&shadow_self._data));
    return (s64)ret;
}

s64 atom<s64>::operator ++(int)
{
    s64 ret = operator ++();
    return ret - 1;
}

s64 atom<s64>::operator --()
{
    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class<s64>*);

    u64 ret = InterlockedDecrement(reinterpret_cast<volatile u64*>(&shadow_self._data));
    return (s64)ret;
}

s64 atom<s64>::operator --(int)
{
    s64 ret = operator --();
    return ret + 1;
}

s64 atom<s64>::weak_add(s64 v)
{
    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class<s64>*);

    return shadow_self._data += v;
}

s64 atom<s64>::exchange(s64 replace)
{
    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class<s64>*);

    u64 ret = InterlockedExchange(reinterpret_cast<volatile u64*>(&shadow_self._data), (u64)replace);
    return (s64)ret;
}

s64 atom<s64>::compare_exchange(s64 expected, s64 replace)
{
    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class<s64>*);

    u64 ret = InterlockedCompareExchange(reinterpret_cast<volatile u64*>(&shadow_self._data), (u64)replace, (u64)expected);
    return (s64)ret;
}

atom<u64>::atom()
{
    assert(_mem_sz >= sizeof(shadow_class<u64>));
    new (_mem) volatile u64(0);
}

atom<u64>::atom(const atom<u64>& rhs)
{
    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class<u64>*);
    auto& shadow_rhs = *pointer_convert(rhs._mem, 0, shadow_class<u64>*);

    shadow_self._data = shadow_rhs._data;
}

atom<u64>::atom(u64 v)
{
    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class<u64>*);

    shadow_self._data = v;
}

atom<u64>::~atom()
{
}

u64 atom<u64>::get() const
{
    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class<u64>*);

    return shadow_self._data;
}

void atom<u64>::set(u64 v)
{
    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class<u64>*);

    shadow_self._data = v;
}

atom<u64>& atom<u64>::operator =(const atom<u64>& rhs)
{
    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class<u64>*);
    auto& shadow_rhs = *pointer_convert(rhs._mem, 0, shadow_class<u64>*);

    shadow_self._data = shadow_rhs._data;
    return *this;
}

atom<u64>& atom<u64>::operator =(u64 v)
{
    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class<u64>*);

    shadow_self._data = v;
    return *this;
}

u64 atom<u64>::operator ++()
{
    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class<u64>*);

    u64 ret = InterlockedIncrement(&shadow_self._data);
    return ret;
}

u64 atom<u64>::operator ++(int)
{
    u64 ret = operator ++();
    return ret - 1;
}

u64 atom<u64>::operator --()
{
    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class<u64>*);

    u64 ret = InterlockedDecrement(&shadow_self._data);
    return ret;
}

u64 atom<u64>::operator --(int)
{
    u64 ret = operator --();
    return ret + 1;
}

u64 atom<u64>::weak_add(s64 v)
{
    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class<u64>*);

    return shadow_self._data += v;
}

u64 atom<u64>::exchange(u64 replace)
{
    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class<u64>*);

    u64 ret = InterlockedExchange(&shadow_self._data, replace);
    return ret;
}

u64 atom<u64>::compare_exchange(u64 expected, u64 replace)
{
    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class<u64>*);

    u64 ret = InterlockedCompareExchange(&shadow_self._data, replace, expected);
    return ret;
}

atom<void*>::atom()
{
    assert(_mem_sz >= sizeof(shadow_class<void*>));
    new (_mem) volatile void*(nullptr);
}

atom<void*>::atom(const atom<void*>& rhs)
{
    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class<void*>*);
    auto& shadow_rhs = *pointer_convert(rhs._mem, 0, shadow_class<void*>*);

    shadow_self._data = shadow_rhs._data;
}

atom<void*>::atom(void* v)
{
    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class<void*>*);

    shadow_self._data = v;
}

atom<void*>::~atom()
{
}

void* atom<void*>::get() const
{
    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class<void*>*);

    return shadow_self._data;
}

void atom<void*>::set(void* v)
{
    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class<void*>*);

    shadow_self._data = v;
}

atom<void*>& atom<void*>::operator =(const atom<void*>& rhs)
{
    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class<void*>*);
    auto& shadow_rhs = *pointer_convert(rhs._mem, 0, shadow_class<void*>*);

    shadow_self._data = shadow_rhs._data;
    return *this;
}

atom<void*>& atom<void*>::operator =(void* v)
{
    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class<void*>*);

    shadow_self._data = v;
    return *this;
}

void* atom<void*>::exchange(void* replace)
{
    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class<void*>*);

    void* ret = InterlockedExchangePointer(&shadow_self._data, replace);
    return (void*)ret;
}

void* atom<void*>::compare_exchange(void* expected, void* replace)
{
    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class<void*>*);

    void* ret = InterlockedCompareExchangePointer(&shadow_self._data, replace, expected);
    return (void*)ret;
}
