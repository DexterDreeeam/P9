#pragma once

typedef  hndl  lock;

_INLINE_ lock  lock_create(void);
_INLINE_ boole lock_try_get(lock lk);
_INLINE_ void  lock_wait_get(lock lk);
_INLINE_ void  lock_put(lock lk);
_INLINE_ void  lock_destroy(lock lk);

struct rw_lock
{
    atom<s64> _writing;
    atom<s64> _reader_count;
};

_INLINE_ rw_lock rw_lock_create();
_INLINE_ boole   rw_lock_try_read(rw_lock& lk);
_INLINE_ void    rw_lock_wait_read(rw_lock& lk);
_INLINE_ boole   rw_lock_try_write(rw_lock& lk);
_INLINE_ void    rw_lock_wait_write(rw_lock& lk);
_INLINE_ void    rw_lock_put_read(rw_lock& lk);
_INLINE_ void    rw_lock_put_write(rw_lock& lk);
_INLINE_ void    rw_lock_destroy(rw_lock& lk);

_INLINE_ lock lock_create(void)
{
    return (lock)WindowsMsvcNs::CreateMutex(NULL, FALSE, NULL);
}

typedef unsigned long DWORD;

_INLINE_ boole lock_try_get(lock lk)
{
    assert(lk);
    return
        WindowsMsvcNs::WaitForSingleObject((WindowsMsvcNs::HANDLE)lk, 0) == WAIT_OBJECT_0;
}

_INLINE_ void lock_wait_get(lock lk)
{
    assert(lk);
    WindowsMsvcNs::WaitForSingleObject((WindowsMsvcNs::HANDLE)lk, INFINITE);
}

_INLINE_ void lock_put(lock lk)
{
    assert(lk);
    WindowsMsvcNs::ReleaseMutex((WindowsMsvcNs::HANDLE)lk);
}

_INLINE_ void lock_destroy(lock lk)
{
    assert(lk);
    WindowsMsvcNs::CloseHandle((WindowsMsvcNs::HANDLE)lk);
}

_INLINE_ rw_lock rw_lock_create(void)
{
    rw_lock lk;
    lk._writing = 0;
    lk._reader_count = 0;
    return lk;
}

_INLINE_ boole rw_lock_try_read(rw_lock& lk)
{
    if (lk._writing == 1)
    {
        return boole::False;
    }
    ++lk._reader_count;
    if (lk._writing == 0)
    {
        return boole::True;
    }
    --lk._reader_count;
    return boole::False;
}

_INLINE_ void rw_lock_wait_read(rw_lock& lk)
{
    while (1)
    {
        if (lk._writing == 0)
        {
            ++lk._reader_count;
            if (lk._writing == 0)
            {
                return;
            }
            --lk._reader_count;
        }
        tick_sleep(1);
    }
}

_INLINE_ boole rw_lock_try_write(rw_lock& lk)
{
    if (lk._reader_count != 0)
    {
        return boole::False;
    }
    if (lk._writing.exchange(1) == 1)
    {
        return boole::False;
    }
    if (lk._reader_count != 0)
    {
        lk._writing = 0;
        return boole::False;
    }
    return boole::True;
}

_INLINE_ void rw_lock_wait_write(rw_lock& lk)
{
    while (lk._writing.exchange(1) == 1)
    {
        tick_sleep(2);
    }
    while (lk._reader_count != 0)
    {
        tick_sleep(1);
    }
}

_INLINE_ void rw_lock_put_read(rw_lock& lk)
{
    --lk._reader_count;
}

_INLINE_ void rw_lock_put_write(rw_lock& lk)
{
    lk._writing = 0;

}

_INLINE_ void rw_lock_destroy(rw_lock& lk)
{
    lk._writing = 0;
    lk._reader_count = 0;
}
