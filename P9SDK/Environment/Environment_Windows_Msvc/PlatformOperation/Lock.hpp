#pragma once

#include "Date.hpp"
#include "Thread.hpp"

struct _lock
{
    WindowsMsvcNs::HANDLE _hdnl;
    atom<u64>             _thrd_id;
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
    atom<s64> _reader_count;

    atom<u64> _write_thrd_id;
    atom<u64> _write_thrd_lock_cnt;
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
    lk->_thrd_id.set(0);
    lk->_lock_cnt.set(0);
    return (lock)lk;
}

_INLINE_ boole lock_try_get(lock x)
{
    using namespace WindowsMsvcNs;

    _lock* lk = (_lock*)x;
    assert(lk);

    u64 myid = thrd_myid();

    if (lk->_thrd_id.get() == myid)
    {
        ++lk->_lock_cnt;
        return boole::True;
    }

    if (lk->_thrd_id.compare_exchange(0, myid) == 0)
    {
        auto wait_rst = WaitForSingleObject((HANDLE)lk->_hdnl, 0);
        assert(wait_rst == WAIT_OBJECT_0);
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

    if (lk->_thrd_id.get() == myid)
    {
        ++lk->_lock_cnt;
    }
    else
    {
        WaitForSingleObject((HANDLE)lk->_hdnl, INFINITE);
        assert(lk->_thrd_id.get() == 0);
        lk->_thrd_id.set(myid);
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
        lk->_thrd_id.set(0);
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
    lk->_reader_count.set(0);
    lk->_write_thrd_id.set(0);
    lk->_write_thrd_lock_cnt.set(0);

    return (rw_lock)lk;
}

_INLINE_ boole rw_lock_try_read(rw_lock x)
{
    _rw_lock* lk = (_rw_lock*)x;
    assert(lk);

    u64 thrd_id = thrd_myid();
    u64 origin_thrd_id = lk->_write_thrd_id.get();

    if (origin_thrd_id == thrd_id)
    {
        assert(lk->_write_thrd_lock_cnt.get() > 0);
        ++lk->_write_thrd_lock_cnt;
        return boole::True;
    }

    if (origin_thrd_id != 0)
    {
        return boole::False;
    }

    ++lk->_reader_count;
    if (lk->_write_thrd_id.get() == 0)
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

    u64 sleep_counter = 0;
    u64 thrd_id = thrd_myid();
    u64 origin_thrd_id = lk->_write_thrd_id.get();

    if (origin_thrd_id == thrd_id)
    {
        assert(lk->_write_thrd_lock_cnt.get() > 0);
        ++lk->_write_thrd_lock_cnt;
        return;
    }

    while (1)
    {
        if (lk->_write_thrd_id.get() == 0)
        {
            ++lk->_reader_count;
            if (lk->_write_thrd_id.get() == 0)
            {
                return;
            }
            --lk->_reader_count;
        }
        tick_sleep(sleep_counter);
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

    u64 thrd_id = thrd_myid();
    u64 origin_thrd_id = lk->_write_thrd_id.compare_exchange(0, thrd_id);

    if (origin_thrd_id == thrd_id)
    {
        assert(lk->_write_thrd_lock_cnt.get() > 0);
        ++lk->_write_thrd_lock_cnt;
        return boole::True;
    }
    else if (origin_thrd_id != 0)
    {
        return boole::False;
    }

    assert(origin_thrd_id == 0);
    if (lk->_reader_count.get() != 0)
    {
        lk->_write_thrd_id.set(0);
        return boole::False;
    }

    assert(lk->_write_thrd_lock_cnt.get() == 0);
    lk->_write_thrd_lock_cnt.set(1);

    return boole::True;
}

_INLINE_ void rw_lock_wait_write(rw_lock x)
{
    _rw_lock* lk = (_rw_lock*)x;
    assert(lk);

    u64 sleep_counter = 0;
    u64 thrd_id = thrd_myid();
    u64 origin_thrd_id = lk->_write_thrd_id.compare_exchange(0, thrd_id);

    if (origin_thrd_id == thrd_id)
    {
        assert(lk->_write_thrd_lock_cnt.get() > 0);
        ++lk->_write_thrd_lock_cnt;
        return;
    }

    while (origin_thrd_id != 0)
    {
        tick_sleep_cycle(sleep_counter);
        origin_thrd_id = lk->_write_thrd_id.compare_exchange(0, thrd_id);
    }

    sleep_counter = 0;
    while (lk->_reader_count.get() != 0)
    {
        tick_sleep_cycle(sleep_counter);
    }

    assert(lk->_write_thrd_lock_cnt.get() == 0);
    lk->_write_thrd_lock_cnt.set(1);
}

_INLINE_ void rw_lock_put_read(rw_lock x)
{
    _rw_lock* lk = (_rw_lock*)x;
    assert(lk);

    while (1)
    {
        u64 origin_cnt = lk->_write_thrd_lock_cnt.get();
        if (origin_cnt == 0)
        {
            break;
        }
        assert(origin_cnt > 1);
        if (lk->_write_thrd_lock_cnt.compare_exchange(origin_cnt, origin_cnt - 1) == origin_cnt)
        {
            if (origin_cnt == 1)
            {
                assert(lk->_write_thrd_id.get() != 0);
                lk->_write_thrd_id.set(0);
            }
            return;
        }
    }

    assert(lk->_write_thrd_id.get() == 0);
    assert(lk->_reader_count.get() > 0);
    --lk->_reader_count;
}

_INLINE_ void rw_lock_put_write(rw_lock x)
{
    _rw_lock* lk = (_rw_lock*)x;
    assert(lk);
    assert(lk->_write_thrd_id.get() != 0);
    assert(lk->_write_thrd_lock_cnt.get() > 0);

    if (--lk->_write_thrd_lock_cnt == 0)
    {
        assert(lk->_write_thrd_id.get() != 0);
        lk->_write_thrd_id.set(0);
    }
}

_INLINE_ void rw_lock_destroy(rw_lock x)
{
    _rw_lock* lk = (_rw_lock*)x;
    assert(lk);
    delete lk;
}
