#pragma once

#include "Thread.hpp"

struct _lock
{
    WindowsMsvcNs::HANDLE _hdnl;
    u64                   _thrd_id;
    atom<u64>             _lock_cnt;
};

typedef  void* lock;

_INLINE_ lock  lock_create(void);
_INLINE_ boole lock_try_get(lock x);
_INLINE_ void  lock_wait_get(lock x);
_INLINE_ void  lock_put(lock x);
_INLINE_ void  lock_destroy(lock x);

struct _rw_lock
{
    atom<s64> _writing;
    atom<s64> _reader_count; 
};

typedef  void* rw_lock;

_INLINE_ rw_lock rw_lock_create();
_INLINE_ boole   rw_lock_try_read(rw_lock x);
_INLINE_ void    rw_lock_wait_read(rw_lock x);
_INLINE_ boole   rw_lock_try_write(rw_lock x);
_INLINE_ void    rw_lock_wait_write(rw_lock x);
_INLINE_ void    rw_lock_put_read(rw_lock x);
_INLINE_ void    rw_lock_put_write(rw_lock x);
_INLINE_ void    rw_lock_destroy(rw_lock x);

_INLINE_ lock lock_create(void)
{
    using namespace WindowsMsvcNs;

    _lock* lk = new _lock();
    lk->_hdnl = CreateMutex(NULL, FALSE, NULL);
    lk->_thrd_id = 0;
    lk->_lock_cnt.set(0);
    return (lock)lk;
}

_INLINE_ boole lock_try_get(lock x)
{
    using namespace WindowsMsvcNs;

    _lock* lk = (_lock*)x;
    assert(lk);

    u64 myid = thrd_myid();

    if (lk->_thrd_id == myid)
    {
        ++lk->_lock_cnt;
        return boole::True;
    }

    if (lk->_thrd_id == 0 && WaitForSingleObject((HANDLE)lk->_hdnl, 0) == WAIT_OBJECT_0)
    {
        lk->_thrd_id = myid;
        lk->_lock_cnt.set(1);
        return boole::True;
    }

    return boole::False;
}

_INLINE_ void lock_wait_get(lock x)
{
    using namespace WindowsMsvcNs;

    _lock* lk = (_lock*)x;
    assert(lk);

    u64 myid = thrd_myid();

    if (lk->_thrd_id == myid)
    {
        ++lk->_lock_cnt;
    }
    else
    {
        WaitForSingleObject((HANDLE)lk->_hdnl, INFINITE);
        lk->_thrd_id = myid;
        lk->_lock_cnt.set(1);
    }
}

_INLINE_ void lock_put(lock x)
{
    using namespace WindowsMsvcNs;

    _lock* lk = (_lock*)x;
    assert(lk);

    if (--lk->_lock_cnt == 0)
    {
        ReleaseMutex((HANDLE)lk->_hdnl);
    }
}

_INLINE_ void lock_destroy(lock x)
{
    using namespace WindowsMsvcNs;

    _lock* lk = (_lock*)x;
    assert(lk);
    CloseHandle((HANDLE)lk->_hdnl);
    delete lk;
}

_INLINE_ rw_lock rw_lock_create(void)
{
    _rw_lock* lk = new _rw_lock();
    lk->_writing = 0;
    lk->_reader_count = 0;
    return (rw_lock)lk;
}

_INLINE_ boole rw_lock_try_read(rw_lock x)
{
    _rw_lock* lk = (_rw_lock*)x;
    assert(lk);
    if (lk->_writing.get() == 1)
    {
        return boole::False;
    }
    ++lk->_reader_count;
    if (lk->_writing.get() == 0)
    {
        return boole::True;
    }
    --lk->_reader_count;
    return boole::False;
}

_INLINE_ void rw_lock_wait_read(rw_lock x)
{
    _rw_lock* lk = (_rw_lock*)x;
    assert(lk);
    while (1)
    {
        if (lk->_writing.get() == 0)
        {
            ++lk->_reader_count;
            if (lk->_writing.get() == 0)
            {
                return;
            }
            --lk->_reader_count;
        }
        tick_sleep(1);
    }
}

_INLINE_ boole rw_lock_try_write(rw_lock x)
{
    _rw_lock* lk = (_rw_lock*)x;
    assert(lk);
    if (lk->_reader_count.get() != 0)
    {
        return boole::False;
    }
    if (lk->_writing.exchange(1) == 1)
    {
        return boole::False;
    }
    if (lk->_reader_count.get() != 0)
    {
        lk->_writing.set(0);
        return boole::False;
    }
    return boole::True;
}

_INLINE_ void rw_lock_wait_write(rw_lock x)
{
    _rw_lock* lk = (_rw_lock*)x;
    assert(lk);
    while (lk->_writing.exchange(1) == 1)
    {
        tick_sleep(2);
    }
    while (lk->_reader_count.get() != 0)
    {
        tick_sleep(1);
    }
}

_INLINE_ void rw_lock_put_read(rw_lock x)
{
    _rw_lock* lk = (_rw_lock*)x;
    assert(lk);
    --lk->_reader_count;
}

_INLINE_ void rw_lock_put_write(rw_lock x)
{
    _rw_lock* lk = (_rw_lock*)x;
    assert(lk);
    lk->_writing.set(0);

}

_INLINE_ void rw_lock_destroy(rw_lock x)
{
    _rw_lock* lk = (_rw_lock*)x;
    assert(lk);
    delete lk;
}
