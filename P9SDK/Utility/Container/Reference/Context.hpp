#pragma once

namespace RefNs
{

class counter
{
public:
    counter() :
        _ref_cnt(0),
        _observer_cnt(0),
        _observer_lk(lock_create())
    {}

    counter(s64 ref, s64 observer) :
        _ref_cnt(ref),
        _observer_cnt(observer),
        _observer_lk(lock_create())
    {}

public:
    volatile s64 _ref_cnt;
    volatile s64 _observer_cnt;
    lock         _observer_lk;
};

typedef void deconstructor(void*);

class context
{
public:
    context() = delete;

    context(const context& rhs) = delete;

    context(void* ptr, volatile counter* cnt, s64 data_size, deconstructor* dc) :
        _ptr(ptr),
        _counter(cnt),
        _data_size(data_size),
        _deconstructor(dc)
    {}

    ~context() = default;

public:
    void*              _ptr;
    volatile counter*  _counter;
    s64                _data_size;
    deconstructor*     _deconstructor;
};

}
