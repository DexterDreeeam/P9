#pragma once

#include "Lock.hpp"

struct _evnt
{
    lock _lk;
};

typedef void* evnt;

_INLINE_ evnt  evnt_create(void);
_INLINE_ void  evnt_set(evnt x);
_INLINE_ void  evnt_wait(evnt x);
_INLINE_ void  evnt_destroy(evnt x);

_INLINE_ evnt evnt_create(void)
{
    _evnt* ev = new _evnt();
    ev->_lk = lock_create();
    lock_wait_get(ev->_lk);
    return (evnt)ev;
}

_INLINE_ void evnt_set(evnt x)
{
    _evnt* ev = (_evnt*)x;
    assert(ev && ev->_lk);
    lock_put(ev->_lk);
}

_INLINE_ void evnt_wait(evnt x)
{
    _evnt* ev = (_evnt*)x;
    assert(ev && ev->_lk);
    lock_wait_get(ev->_lk);
}

_INLINE_ void evnt_destroy(evnt x)
{
    _evnt* ev = (_evnt*)x;
    assert(ev && ev->_lk);
    lock_destroy(ev->_lk);
    delete ev;
}
