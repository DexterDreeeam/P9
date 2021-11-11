#pragma once

_INLINE_ mutex::mutex() :
    _ctx(nullptr)
{
}

_INLINE_ mutex::mutex(const mutex& rhs) :
    _ctx(rhs._ctx)
{
}

_INLINE_ mutex& mutex::operator =(const mutex& rhs)
{
    _ctx = rhs._ctx;
    return *this;
}

_INLINE_ lock::lock() :
    _mtx()
{
}

_INLINE_ lock::lock(const lock& rhs) :
    _mtx(rhs._mtx),
    _thrd_id(rhs._thrd_id),
    _lock_cnt(rhs._lock_cnt)
{
}

_INLINE_ lock& lock::operator =(const lock& rhs)
{
    _mtx = rhs._mtx;
    _thrd_id = rhs._thrd_id;
    _lock_cnt = rhs._lock_cnt;
    return *this;
}

_INLINE_ boole lock::init()
{
    if (_mtx.init())
    {
        _thrd_id.set(0);
        _lock_cnt.set(0);
        return boole::True;
    }
    else
    {
        return boole::False;
    }
}

_INLINE_ boole lock::is_init()
{
    return _mtx.is_init() && _thrd_id.get() == 0 && _lock_cnt.get() == 0;
}

_INLINE_ boole lock::uninit()
{
    return _mtx.uninit();
}

_INLINE_ boole lock::try_acquire()
{
    boole test_rst;
    u64 myid = current_thread_id();
    if (_thrd_id.get() == myid)
    {
        ++_lock_cnt;
        return boole::True;
    }
    if (_thrd_id.compare_exchange(0, myid) == 0)
    {
        test_rst = _mtx.try_acquire();
        assert(test_rst);
        _lock_cnt.set(1);
        return boole::True;
    }
    return boole::False;
}

_INLINE_ boole lock::wait_acquire()
{
    boole test_rst;
    u64 myid = current_thread_id();
    if (_thrd_id.get() == myid)
    {
        ++_lock_cnt;
        return boole::True;
    }
    else
    {
        test_rst = _mtx.wait_acquire();
        assert(test_rst);
        _thrd_id.set(myid);
        _lock_cnt.set(1);
        return boole::True;
    }
    return boole::False;
}

_INLINE_ boole lock::release()
{
    boole test_rst;
    if (--_lock_cnt == 0)
    {
        _thrd_id.set(0);
        return _mtx.release();
    }
    return boole::True;
}

_INLINE_ rw_lock::rw_lock() :
    _reader_count(s64_min),
    _write_thrd_id(s64_min),
    _write_thrd_lock_cnt(s64_min)
{
}

_INLINE_ rw_lock::rw_lock(const rw_lock& rhs) :
    _reader_count(rhs._reader_count),
    _write_thrd_id(rhs._write_thrd_id),
    _write_thrd_lock_cnt(rhs._write_thrd_lock_cnt)
{
}

_INLINE_ rw_lock& rw_lock::operator =(const rw_lock& rhs)
{
    _reader_count = rhs._reader_count;
    _write_thrd_id = rhs._write_thrd_id;
    _write_thrd_lock_cnt = rhs._write_thrd_lock_cnt;
    return *this;
}

_INLINE_ boole rw_lock::init()
{
    _reader_count.set(0);
    _write_thrd_id.set(0);
    _write_thrd_lock_cnt.set(0);
    return boole::True;
}

_INLINE_ boole rw_lock::is_init()
{
    return _reader_count.get() == 0 && _write_thrd_id.get() == 0 && _write_thrd_lock_cnt.get() == 0;
}

_INLINE_ boole rw_lock::uninit()
{
    _reader_count.set(s64_min);
    _write_thrd_id.set(s64_min);
    _write_thrd_lock_cnt.set(s64_min);
    return boole::True;
}

_INLINE_ boole rw_lock::try_read()
{
    u64 thrd_id = current_thread_id();
    u64 origin_thrd_id = _write_thrd_id.get();

    if (origin_thrd_id == thrd_id)
    {
        assert(_write_thrd_lock_cnt.get() > 0);
        ++_write_thrd_lock_cnt;
        return boole::True;
    }

    if (origin_thrd_id != 0)
    {
        return boole::False;
    }

    ++_reader_count;
    if (_write_thrd_id.get() == 0)
    {
        return boole::True;
    }

    --_reader_count;
    return boole::False;
}

_INLINE_ boole rw_lock::wait_read()
{
    u64 thrd_id = current_thread_id();
    u64 origin_thrd_id = _write_thrd_id.get();

    if (origin_thrd_id == thrd_id)
    {
        assert(_write_thrd_lock_cnt.get() > 0);
        ++_write_thrd_lock_cnt;
        return boole::True;
    }

    auto sleep_tick = tick::new_sleep_cycle();
    while (1)
    {
        if (_write_thrd_id.get() == 0)
        {
            ++_reader_count;
            if (_write_thrd_id.get() == 0)
            {
                break;
            }
            --_reader_count;
        }
        sleep_tick.sleep_cycle();
    }

    return boole::True;
}

_INLINE_ boole rw_lock::try_write()
{
    if (_reader_count.get() != 0)
    {
        return boole::False;
    }

    u64 thrd_id = current_thread_id();
    u64 origin_thrd_id = _write_thrd_id.compare_exchange(0, thrd_id);

    if (origin_thrd_id == thrd_id)
    {
        assert(_write_thrd_lock_cnt.get() > 0);
        ++_write_thrd_lock_cnt;
        return boole::True;
    }
    else if (origin_thrd_id != 0)
    {
        return boole::False;
    }

    assert(origin_thrd_id == 0);
    if (_reader_count.get() != 0)
    {
        _write_thrd_id.set(0);
        return boole::False;
    }

    assert(_write_thrd_lock_cnt.get() == 0);
    _write_thrd_lock_cnt.set(1);

    return boole::True;
}

_INLINE_ boole rw_lock::wait_write()
{
    u64 thrd_id = current_thread_id();
    u64 origin_thrd_id = _write_thrd_id.compare_exchange(0, thrd_id);

    if (origin_thrd_id == thrd_id)
    {
        assert(_write_thrd_lock_cnt.get() > 0);
        ++_write_thrd_lock_cnt;
        return boole::True;
    }

    auto sc = tick::new_sleep_cycle();
    while (origin_thrd_id != 0)
    {
        sc.sleep_cycle();
        origin_thrd_id = _write_thrd_id.compare_exchange(0, thrd_id);
    }

    while (_reader_count.get() != 0)
    {
        sc.sleep_cycle();
    }

    assert(_write_thrd_lock_cnt.get() == 0);
    _write_thrd_lock_cnt.set(1);
    return boole::True;
}

_INLINE_ boole rw_lock::read_release()
{
    while (1)
    {
        u64 origin_cnt = _write_thrd_lock_cnt.get();
        if (origin_cnt == 0)
        {
            break;
        }
        assert(origin_cnt > 1);
        if (_write_thrd_lock_cnt.compare_exchange(origin_cnt, origin_cnt - 1) == origin_cnt)
        {
            if (origin_cnt == 1)
            {
                assert(_write_thrd_id.get() != 0);
                _write_thrd_id.set(0);
            }
            return boole::True;
        }
    }

    assert(_write_thrd_id.get() == 0);
    assert(_reader_count.get() > 0);
    --_reader_count;
    return boole::True;
}

_INLINE_ boole rw_lock::write_release()
{
    assert(_write_thrd_id.get() != 0);
    assert(_write_thrd_lock_cnt.get() > 0);

    if (--_write_thrd_lock_cnt == 0)
    {
        assert(_write_thrd_id.get() != 0);
        _write_thrd_id.set(0);
    }
    return boole::True;
}
