#pragma once

#include "_Common.hpp"

namespace sdk {

class functor : public obj<functor>
{
public:
    functor() :
        _parameters(),
        _fn(nullptr),
        _return(),
        _is_complete(boole::False)
    {
    }

    virtual ~functor() = default;

    virtual const char* obj_type() const { return "functor"; }

    virtual u128 obj_type_id() const { return u128(0xb42311cc40876b31, 0x70431aeb414b279a); }

public:
    boole set_function()
    {
        return boole::False;
    }

    template<typename ...Args>
    boole set_parameters(const Args&... args)
    {
        _parameters.clear();
        return set_parameters_internal(args...);
    }

    template<typename ...Args>
    boole invoke(const Args&... args)
    {
        return set_parameters(args...) && invoke();
    }

    boole invoke()
    {
        _is_complete = boole::False;
        _return = _fn(_parameters);
        _is_complete = boole::True;
        return boole::True;
    }

    boole is_complete() const
    {
        return _is_complete;
    }

private:
    template<typename Ty, typename ...Args>
    boole set_parameters_internal(const Ty& next, const Args&... args)
    {
        return set_parameters_internal(next) && set_parameters_internal(args...);
    }

    template<typename Ty>
    boole set_parameters_internal(const Ty& next)
    {
        auto r = ref<Ty>::new_instance(next).ref_of<>();
        if (r.has_value())
        {
            _parameters.push_back(r);
            return boole::True;
        }
        else
        {
            return boole::False;
        }
    }

private:
    vector<ref<>>    _parameters;
    ref<>          (*_fn)(vector<ref<>>);
    ref<>            _return;
    boole            _is_complete;
};

} // sdk
