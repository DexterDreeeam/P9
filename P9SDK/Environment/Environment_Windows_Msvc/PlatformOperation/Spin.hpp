#pragma once

typedef atom<s64>* spin;

_INLINE_ spin  spin_create(void);
_INLINE_ boole spin_try_get(spin sp);
_INLINE_ void  spin_wait_get(spin sp);
_INLINE_ void  spin_put(spin sp);
_INLINE_ void  spin_destroy(spin sp);

_INLINE_ spin spin_create(void)
{
    return new atom<s64>(0);
}

_INLINE_ boole spin_try_get(spin sp)
{
    return sp->exchange(1) == 0;
}

_INLINE_ void spin_wait_get(spin sp)
{
    while (!spin_try_get(sp))
    {
        ;
    }
}

_INLINE_ void spin_put(spin sp)
{
    sp->set(0);
}

_INLINE_ void spin_destroy(spin sp)
{
    assert(sp);
    delete sp;
}
