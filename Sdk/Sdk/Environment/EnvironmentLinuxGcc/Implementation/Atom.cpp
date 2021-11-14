
#include "../EnvironmentHeader.hpp"
#include "../../Interface.hpp"

template<typename Ty>
class shadow_class
{
public:
    using DataType = std::atomic<Ty>;

    DataType _data;
};

atom<s64>::atom()
{
    assert(_mem_sz >= sizeof(shadow_class<s64>));
    new (_mem) std::atomic<s64>();
}

atom<s64>::atom(const atom<s64>& rhs) :
    atom()
{
    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class<s64>*);
    auto& shadow_rhs = *pointer_convert(rhs._mem, 0, shadow_class<s64>*);

    shadow_self._data.store(shadow_rhs._data.load());
}

atom<s64>::atom(s64 v) :
    atom()
{
    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class<s64>*);

    shadow_self._data.store(v);
}

atom<s64>::~atom()
{
    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class<s64>*);

    (&shadow_self._data)->~atomic();
}

s64 atom<s64>::get() const
{
    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class<s64>*);

    return shadow_self._data.load();
}

void atom<s64>::set(s64 v)
{
    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class<s64>*);

    shadow_self._data.store(v);
}

atom<s64>& atom<s64>::operator =(const atom<s64>& rhs)
{
    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class<s64>*);
    auto& shadow_rhs = *pointer_convert(rhs._mem, 0, shadow_class<s64>*);

    shadow_self._data.store(shadow_rhs._data.load());
    return *this;
}

atom<s64>& atom<s64>::operator =(s64 v)
{
    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class<s64>*);

    shadow_self._data.store(v);
    return *this;
}

s64 atom<s64>::operator ++()
{
    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class<s64>*);

    s64 ret = ++shadow_self._data;
    return ret;
}

s64 atom<s64>::operator ++(int)
{
    s64 ret = operator ++();
    return ret - 1;
}

s64 atom<s64>::operator --()
{
    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class<s64>*);

    s64 ret = --shadow_self._data;
    return ret;
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

    s64 ret = shadow_self._data.exchange(replace);
    return ret;
}

s64 atom<s64>::compare_exchange(s64 expected, s64 replace)
{
    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class<s64>*);

    shadow_self._data.compare_exchange_strong(expected, replace);
    return expected;
}

atom<u64>::atom()
{
    assert(_mem_sz >= sizeof(shadow_class<u64>));
    new (_mem) std::atomic<u64>();
}

atom<u64>::atom(const atom<u64>& rhs) :
    atom()
{
    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class<u64>*);
    auto& shadow_rhs = *pointer_convert(rhs._mem, 0, shadow_class<u64>*);

    shadow_self._data.store(shadow_rhs._data.load());
}

atom<u64>::atom(u64 v) :
    atom()
{
    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class<u64>*);

    shadow_self._data.store(v);
}

atom<u64>::~atom()
{
    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class<u64>*);

    (&shadow_self._data)->~atomic();
}

u64 atom<u64>::get() const
{
    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class<u64>*);

    return shadow_self._data.load();
}

void atom<u64>::set(u64 v)
{
    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class<u64>*);

    shadow_self._data.store(v);
}

atom<u64>& atom<u64>::operator =(const atom<u64>& rhs)
{
    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class<u64>*);
    auto& shadow_rhs = *pointer_convert(rhs._mem, 0, shadow_class<u64>*);

    shadow_self._data.store(shadow_rhs._data.load());
    return *this;
}

atom<u64>& atom<u64>::operator =(u64 v)
{
    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class<u64>*);

    shadow_self._data.store(v);
    return *this;
}

u64 atom<u64>::operator ++()
{
    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class<u64>*);

    u64 ret = ++shadow_self._data;
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

    u64 ret = --shadow_self._data;
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

    u64 ret = shadow_self._data.exchange(replace);
    return ret;
}

u64 atom<u64>::compare_exchange(u64 expected, u64 replace)
{
    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class<u64>*);

    shadow_self._data.compare_exchange_strong(expected, replace);
    return expected;
}

atom<void*>::atom()
{
    assert(_mem_sz >= sizeof(shadow_class<void*>));
    new (_mem) std::atomic<void*>();
}

atom<void*>::atom(const atom<void*>& rhs) :
    atom()
{
    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class<void*>*);
    auto& shadow_rhs = *pointer_convert(rhs._mem, 0, shadow_class<void*>*);

    shadow_self._data.store(shadow_rhs._data.load());
}

atom<void*>::atom(void* v) :
    atom()
{
    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class<void*>*);

    shadow_self._data.store(v);
}

atom<void*>::~atom()
{
}

void* atom<void*>::get() const
{
    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class<void*>*);

    return shadow_self._data.load();
}

void atom<void*>::set(void* v)
{
    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class<void*>*);

    shadow_self._data.store(v);
}

atom<void*>& atom<void*>::operator =(const atom<void*>& rhs)
{
    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class<void*>*);
    auto& shadow_rhs = *pointer_convert(rhs._mem, 0, shadow_class<void*>*);

    shadow_self._data.store(shadow_rhs._data.load());
    return *this;
}

atom<void*>& atom<void*>::operator =(void* v)
{
    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class<void*>*);

    shadow_self._data.store(v);
    return *this;
}

void* atom<void*>::exchange(void* replace)
{
    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class<void*>*);

    void* ret = shadow_self._data.exchange(replace);
    return ret;
}

void* atom<void*>::compare_exchange(void* expected, void* replace)
{
    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class<void*>*);

    shadow_self._data.compare_exchange_strong(expected, replace);
    return expected;
}
