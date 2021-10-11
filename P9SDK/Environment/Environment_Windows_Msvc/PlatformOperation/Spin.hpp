#pragma once

typedef volatile s64* spin;

_INLINE_ spin  spin_create(void);
_INLINE_ RET   spin_try_get(spin sp);
_INLINE_ void  spin_wait_get(spin sp);
_INLINE_ void  spin_put(spin sp);
_INLINE_ void  spin_destroy(spin sp);

_INLINE_ spin spin_create(void)
{
    return (spin)new s64(0);
}

_INLINE_ RET spin_try_get(spin sp)
{
    return atom_exchange(*reinterpret_cast<volatile s64*>(sp), 1LL) == 0 ? RET_SUCCESS : RET_FAILED;
}

_INLINE_ void spin_wait_get(spin sp)
{
    while (spin_try_get(sp) == RET_FAILED)
    {
        ;
    }
}

_INLINE_ void spin_put(spin sp)
{
    *reinterpret_cast<volatile s64*>(sp) = 0;
}

_INLINE_ void spin_destroy(spin sp)
{
    assert(sp);
    delete sp;
}
