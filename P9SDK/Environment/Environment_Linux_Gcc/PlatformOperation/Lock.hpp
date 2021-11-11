#pragma once

struct _mutex
{
    pthread_mutex_t  _mt;
};

typedef  void* mutex;

_INLINE_ mutex mutex_create(void);
_INLINE_ boole mutex_try_get(mutex x);
_INLINE_ void  mutex_wait_get(mutex x);
_INLINE_ void  mutex_put(mutex x);
_INLINE_ void  mutex_destroy(mutex x);

struct _lock
{
    pthread_mutex_t  _lk;
    atom<u64>        _thrd_id;
    atom<u64>        _lock_cnt;
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

_INLINE_ mutex mutex_create(void)
{
    _mutex* mt = new _mutex();
    pthread_mutex_init(&mt->_mt, nullptr);
    return (mutex)mt;
}

_INLINE_ boole mutex_try_get(mutex x)
{
    _mutex* mt = (_mutex*)x;
    assert(mt);

    return
        pthread_mutex_trylock(&mt->_mt) == 0;
}

_INLINE_ void mutex_wait_get(mutex x)
{
    _mutex* mt = (_mutex*)x;
    assert(mt);

    pthread_mutex_lock(&mt->_mt);
}

_INLINE_ void mutex_put(mutex x)
{
    _mutex* mt = (_mutex*)x;
    assert(mt);

    pthread_mutex_unlock(&mt->_mt);
}

_INLINE_ void mutex_destroy(mutex x)
{
    _mutex* mt = (_mutex*)x;
    assert(mt);

    pthread_mutex_destroy(&mt->_mt);
    delete mt;
}

_INLINE_ lock lock_create(void)
{
    _lock* lk = new _lock();
    pthread_mutex_init(&lk->_lk, nullptr);
    lk->_thrd_id.set(0);
    lk->_lock_cnt.set(0);
    return (lock)lk;
}

_INLINE_ boole lock_try_get(lock x)
{
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
        auto wait_rst = pthread_mutex_trylock(&lk->_lk);
        assert(wait_rst == 0); // try_get should always go through
        lk->_lock_cnt.set(1);
        return boole::True;
    }

    return boole::False;
}

_INLINE_ void lock_wait_get(lock x)
{
    _lock* lk = (_lock*)x;
    assert(lk);

    u64 myid = thrd_myid();

    if (lk->_thrd_id.get() == myid)
    {
        ++lk->_lock_cnt;
    }
    else
    {
        pthread_mutex_lock(&lk->_lk);
        assert(lk->_thrd_id.get() == 0);
        lk->_thrd_id.set(myid);
        lk->_lock_cnt.set(1);
    }
}

_INLINE_ void lock_put(lock x)
{
    _lock* lk = (_lock*)x;
    assert(lk);

    if (--lk->_lock_cnt == 0)
    {
        lk->_thrd_id.set(0);
        pthread_mutex_unlock(&lk->_lk);
    }
}

_INLINE_ void lock_destroy(lock x)
{
    _lock* lk = (_lock*)x;
    assert(lk);
    pthread_mutex_destroy(&lk->_lk);
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
