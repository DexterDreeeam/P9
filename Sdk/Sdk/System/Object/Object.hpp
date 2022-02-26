#pragma once

#include "../../Environment/Interface.hpp"
#include "U128Counter.hpp"

namespace ObjectNs
{

#if DEBUG_LEVEL > DEBUG_LEVEL_CALIBRATION_RUNTIME

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

#endif

template<typename CheckTy, typename = void>
class has_build_function;

}

template<typename Ty = void>
class obj;

template<>
class obj<void>
{
public:
    template<typename Ty, typename> class ObjectNs::has_build_function;

    // every object class should implement 'obj_type' and 'obj_type_id' virtual function
public:
    virtual const char* obj_type() const = 0;

    virtual u128 obj_type_id() const = 0;

protected:
    obj(const obj&) = delete;

    obj& operator =(const obj&) = delete;

protected:
    obj() :
        _obj_id(random::new_u64(), random::new_u64()),
        _self_obs()
    {
    #if DEBUG_LEVEL > DEBUG_LEVEL_CALIBRATION_RUNTIME
        _counter = ObjectNs::__global_object_manager.distribute();
    #endif
    }

public:
    virtual ~obj()
    {
    #if DEBUG_LEVEL > DEBUG_LEVEL_CALIBRATION_RUNTIME
        ObjectNs::__global_object_manager.recycle(_counter);
    #endif
    }

    u128 obj_id() const
    {
        return _obj_id;
    }

protected:
    template<typename Ty>
    obs<Ty> obs_of()
    {
        return _self_obs.obs_of<Ty>();
    }

public:
    u128       _obj_id;
    obs<void>  _self_obs;

#if DEBUG_LEVEL > DEBUG_LEVEL_CALIBRATION_RUNTIME

public:
    static void report()
    {
        ObjectNs::__global_object_manager.report();
    }

protected:
    u128_counter _counter;

#endif
};

namespace ObjectNs
{

template<typename CheckTy, typename>
class has_build_function
{
public:
    template<typename ...Args>
    ref<CheckTy> build(Args... args)
    {
        auto r = ref<CheckTy>::new_instance(args...);
        r->_self_obs = obs<CheckTy>(r).obs_of<void>();
        return r;
    }
};

template<typename CheckTy>
class has_build_function<CheckTy, void_t<typename decltype(&CheckTy::build)>>
{
public:
    template<typename ...Args>
    ref<CheckTy> build(Args... args)
    {
        auto r = CheckTy::build(args...).ref_of<CheckTy>();
        r->_self_obs = obs<CheckTy>(r).obs_of<void>();
        return r;
    }
};

}

template<typename Ty>
class obj : public obj<void>
{
public:
    template<typename ...Args>
        //requires is_convertible<Ty, obj<Ty>>
    static ref<Ty> build(Args... args)
    {
        return ObjectNs::has_build_function<Ty>().build(args...);
    }
};
