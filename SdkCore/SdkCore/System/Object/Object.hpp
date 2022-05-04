#pragma once

#include "../../Environment/_Interface.hpp"
#include "U128Counter.hpp"

namespace _Internal {
namespace _Object {

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

template<typename VoidTy, typename ...OtherTys>
class _obj_derived;

template<typename Ty, typename ...DerivedTys>
class _obj;

template<>
class _obj_derived<void>
{
};

template<typename NextTy>
class _obj_derived<void, NextTy> : public NextTy
{
};

template<typename NextTy, typename ...OthersTys>
class _obj_derived<void, NextTy, OthersTys...> : public NextTy, public _obj_derived<void, OthersTys...>
{
};

template<>
class _obj<void>
{
public:
    virtual ~_obj()
    {
#if DEBUG_LEVEL > DEBUG_LEVEL_CALIBRATION_RUNTIME
        __global_object_manager.recycle(_counter);
#endif
    }

    virtual const char* obj_type() const = 0;

    virtual u128 obj_type_id() const = 0;

protected:
    _obj(const _obj&) = delete;

    _obj& operator =(const _obj&) = delete;

protected:
    _obj()
    {
#if DEBUG_LEVEL > DEBUG_LEVEL_CALIBRATION_RUNTIME
        _counter = __global_object_manager.distribute();
#endif
    }

#if DEBUG_LEVEL > DEBUG_LEVEL_CALIBRATION_RUNTIME

public:
    static void report()
    {
        __global_object_manager.report();
    }

protected:
    u128_counter _counter;

#endif
};

using _obj_base = _obj<void>;

template<typename Ty>
concept is_obj =
    requires(Ty o)
    {
        static_cast<_obj_base*>(&o);
    };

template<typename Ty, typename ...DerivedTys>
class _obj : public _obj_base, public _obj_derived<DerivedTys...>
{
public:
    _obj() :
        _obj_id(random::new_u64(), random::new_u64()),
        _self_obs()
    {
    }

    u128 obj_id() const
    {
        return _obj_id;
    }

    template<typename ...Args>
    static ref<Ty> build(Args ...args)
    {
        auto r = ref<Ty>::new_instance();
        if (r.has_value() && r->Ty::init(args...))
        {
            r->_self_obs = r;
            return r;
        }
        return ref<Ty>();
    }

    template<typename Ty>
    obs<Ty> as_obs()
    {
        return _self_obs.obs_of<Ty>();
    }

protected:
    u128       _obj_id;
    obs<void>  _self_obs;
};

}} // _Internal::_Object

template<typename ...DerivedTys>
class has;

template<>
class has<void>
{
};

template<typename ...DerivedTys>
class has
{
};

template<typename Ty, typename HasTy = has<void>>
class obj;

template<typename Ty>
class obj<Ty, has<void>> : public _Internal::_Object::_obj<Ty, void>
{
};

template<typename Ty, typename ...DerivedTys>
class obj<Ty, has<DerivedTys...>> : public _Internal::_Object::_obj<Ty, void, DerivedTys...>
{
};
