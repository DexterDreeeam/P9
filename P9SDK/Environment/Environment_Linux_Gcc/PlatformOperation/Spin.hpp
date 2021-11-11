#pragma once

typedef atom<s64>  _spin;
typedef void*      spin;

_INLINE_ spin   spin_create(void);
_INLINE_ boole  spin_try_get(spin x);
_INLINE_ void   spin_wait_get(spin x);
_INLINE_ void   spin_put(spin x);
_INLINE_ void   spin_destroy(spin x);

_INLINE_ spin spin_create(void)
{
    return (spin)new atom<s64>(0);
}

_INLINE_ boole spin_try_get(spin x)
{
    _spin* sp = (_spin*)x;
    assert(sp);
    return sp->exchange(1) == 0;
}

_INLINE_ void spin_wait_get(spin x)
{
    _spin* sp = (_spin*)x;
    assert(sp);
    while (!spin_try_get(sp))
    {
        ;
    }
}

_INLINE_ void spin_put(spin x)
{
    _spin* sp = (_spin*)x;
    assert(sp);
    sp->set(0);
}

_INLINE_ void spin_destroy(spin x)
{
    _spin* sp = (_spin*)x;
    assert(sp);
    delete sp;
}
