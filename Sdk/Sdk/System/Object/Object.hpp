#pragma once

#if DEBUG_LEVEL <= DEBUG_LEVEL_CALIBRATION_RUNTIME

class object
{
public:
    object() = default;

    ~object() = default;
}

};

#else

#include "U128Counter.hpp"

namespace ObjectNs
{

class _object_manager
{
public:
    _object_manager()
    {}

    ~_object_manager()
    {
        u128_counter record_release = _release_counter.get();
        if (record_release != _allot_counter.get())
        {
            assert_info(0, "There are non-released objects");
        }
    }

    u128_counter distribute()
    {
        return ++_allot_counter;
    }

    void recycle(u128_counter counter)
    {
        counter;
        ++_release_counter;
    }

private:
    atom<u128_counter> _allot_counter;
    atom<u128_counter> _release_counter;
};

_SELECTANY_ _object_manager __global_object_manager;

}

class object
{
public:
    object() :
        _counter(ObjectNs::__global_object_manager.distribute())
    {
    }

    ~object()
    {
        ObjectNs::__global_object_manager.recycle(_counter);
    }

private:
    u128_counter _counter;
};

#endif
