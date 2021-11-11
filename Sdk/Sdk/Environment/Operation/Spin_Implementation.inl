#pragma once

_INLINE_ spin::spin() :
    _v(s64_min)
{
}

_INLINE_ spin::spin(const spin& rhs) :
    _v(rhs._v)
{
}

_INLINE_ spin& spin::operator =(const spin& rhs)
{
    _v = rhs._v;
    return *this;
}

_INLINE_ boole spin::init()
{
    _v.set(0);
}

_INLINE_ boole spin::is_init()
{
    return _v.get() == 0;
}

_INLINE_ boole spin::uninit()
{
    _v.set(s64_min);
}

_INLINE_ boole spin::try_acquire()
{
    return _v.exchange(1) == 0;
}

_INLINE_ boole spin::wait_acquire()
{
    while (!try_acquire())
    {
        yield();
    }
    return boole::True;
}

_INLINE_ boole spin::release()
{
    _v.set(0);
}
