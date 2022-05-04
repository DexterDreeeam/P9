#pragma once

namespace _Internal {
namespace _Ref {

_INLINE_ ref_context_base::ref_context_base(deconstructor_cb* dc) :
    _deconstructor(dc),
    _ref_cnt(1),
    _obs_cnt(1),
    _obs_lock(0)
{}

template<typename Ty>
_INLINE_ ref_context<Ty>::ref_context(Ty* ptr) :
    ref_context_base(&ref_context<Ty>::deconstructor),
    _ptr(ptr)
{
}

template<typename Ty>
_INLINE_ ref_context<Ty>::~ref_context()
{
}

template<typename Ty>
_INLINE_ ref_context<Ty>* ref_context<Ty>::new_instance_with_ptr(Ty* raw_ptr)
{
    return new ref_context<Ty>(raw_ptr);
}

template<typename Ty>
_INLINE_ void ref_context<Ty>::add_ref()
{
    assert(_ref_cnt.get() > 0);
    ++_ref_cnt;
}

template<typename Ty>
_INLINE_ boole ref_context<Ty>::remove_ref()
{
    assert(_ref_cnt.get() > 0);
    assert(_ptr);

    if (--_ref_cnt == 0)
    {
        (*_deconstructor)(_ptr);
        memory::free(_ptr);

        assert(_obs_cnt.get() > 0);
        if (--_obs_cnt == 0)
        {
            return boole::True;
        }
        else
        {
            return boole::False;
        }
    }
    return boole::False;
}

template<typename Ty>
_INLINE_ void ref_context<Ty>::add_obs()
{
    assert(_obs_cnt.get() > 0);
    ++_obs_cnt;
}

template<typename Ty>
_INLINE_ boole ref_context<Ty>::remove_obs()
{
    assert(_obs_cnt.get() > 0);
    if (--_obs_cnt == 0)
    {
        assert(_ref_cnt.get() == 0);
        return boole::True;
    }
    else
    {
        return boole::False;
    }
}

template<typename Ty>
_INLINE_ boole ref_context<Ty>::try_ref()
{
    while (_obs_lock.exchange(1) != 0)
    {
        yield();
    }
    barrier();
    if (++_ref_cnt != 1)
    {
        assert(_ref_cnt.get() > 1);
        _obs_lock.set(0);
        return boole::True;
    }
    else
    {
        --_ref_cnt;
        _obs_lock.set(0);
        return boole::False;
    }
}

template<typename Ty>
_INLINE_ void ref_context<Ty>::debug_report() const
{
    print("[_RefNs::context::debug_report] =========\n   ref_cnt = %lld\n   obs_cnt = %lld\n",
        _ref_cnt.get(), _obs_cnt.get());
}

template<typename Ty>
_INLINE_ void ref_context<Ty>::deconstructor(void* p)
{
    pointer_convert(p, 0, Ty*)->~Ty();
}

}} // _Internal::_Ref
