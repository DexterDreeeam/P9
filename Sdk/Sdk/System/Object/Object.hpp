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

    void report()
    {
        auto allot = _allot_counter.get();
        auto release = _release_counter.get();

        print("\nobject report:\n  allot  \t%llu\n  release\t%llu\n  diff   \t%lld\n", allot._u64_1, release._u64_1, (s64)allot._u64_1 - (s64)release._u64_1);
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

public:
    static void report()
    {
        ObjectNs::__global_object_manager.report();
    }

private:
    u128_counter _counter;
};

#endif
