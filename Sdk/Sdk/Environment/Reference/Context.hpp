#pragma once

namespace RefNs
{

class counter
{
public:
    counter() :
        _ref_cnt(0),
        _observer_cnt(0),
        _observer_check_busy(0)
    {}

    counter(s64 ref, s64 observer) :
        _ref_cnt(ref),
        _observer_cnt(observer),
        _observer_check_busy(0)
    {}

public:
    atom<s64> _ref_cnt;
    atom<s64> _observer_cnt;
    atom<s64> _observer_check_busy;
};

typedef void deconstructor(void*);

class context
{
public:
    context() = delete;

    context(const context& rhs) = delete;

    context(void* ptr, counter* cnt, s64 data_size, deconstructor* dc) :
        _ptr(ptr),
        _counter(cnt),
        _data_size(data_size),
        _deconstructor(dc)
    {}

    ~context() = default;

public:
    void*              _ptr;
    counter*           _counter;
    s64                _data_size;
    deconstructor*     _deconstructor;
};

}
