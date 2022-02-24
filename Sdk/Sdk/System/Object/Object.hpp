#pragma once

#include "../../Environment/Interface.hpp"
#include "U128Counter.hpp"

#if DEBUG_LEVEL > DEBUG_LEVEL_CALIBRATION_RUNTIME

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

#endif

class object
{
private:
    object(const object&) = delete;

    object& operator =(const object&) = delete;

    void setup_obs(obs<void> self)
    {
        _self_obs = self;
    }

    void setup_object_id(u128 object_id)
    {
        _object_id = object_id;
    }

protected:
    object() :
        _object_id()
    {
    #if DEBUG_LEVEL > DEBUG_LEVEL_CALIBRATION_RUNTIME
        _counter = ObjectNs::__global_object_manager.distribute();
    #endif
    }

public:
    virtual ~object()
    {
    #if DEBUG_LEVEL > DEBUG_LEVEL_CALIBRATION_RUNTIME
        ObjectNs::__global_object_manager.recycle(_counter);
    #endif
    }

    virtual const char* object_type() = 0;

    virtual u128 object_type_id() = 0;

    u128 object_id() const
    {
        return _object_id;
    }

private:
    u128       _object_id;
    obs<void>  _self_obs;

public:
    template<typename Ty, typename ...Args>
        requires is_convertible<Ty, object>
    static ref<Ty> build(Args... args)
    {
        auto r = ref<Ty>::new_instance(args...);
        r->setup_object_id(u128(random::new_u64(), random::new_u64()));
        r->setup_obs(obs<Ty>(r).obs_of<void>);
        return r;
    }

#if DEBUG_LEVEL > DEBUG_LEVEL_CALIBRATION_RUNTIME

public:
    static void report()
    {
        ObjectNs::__global_object_manager.report();
    }

private:
    u128_counter _counter;

#endif
};
