#pragma once

class mutex final
{
public:
    mutex();

    mutex(const mutex& rhs);

    mutex& operator =(const mutex& rhs);

    ~mutex() = default;

public:
    boole init();

    boole is_init() { return _ctx != nullptr; }

    boole uninit();

    boole try_acquire();

    boole wait_acquire();

    boole release();

private:
    void* _ctx;
};

class lock final
{
public:
    lock();

    lock(const lock& rhs);

    lock& operator =(const lock& rhs);

    ~lock() = default;

public:
    boole init();

    boole is_init();

    boole uninit();

    boole try_acquire();

    boole wait_acquire();

    boole release();

private:
    mutex      _mtx;
    atom<u64>  _thrd_id;
    atom<u64>  _lock_cnt;
};

class rw_lock final
{
public:
    rw_lock();

    rw_lock(const rw_lock& rhs);

    rw_lock& operator =(const rw_lock& rhs);

    ~rw_lock() = default;

public:
    boole init();

    boole is_init();

    boole uninit();

    boole try_read();

    boole wait_read();

    boole try_write();

    boole wait_write();

    boole read_release();

    boole write_release();

private:
    atom<s64> _reader_count;
    atom<u64> _write_thrd_id;
    atom<u64> _write_thrd_lock_cnt;
};
