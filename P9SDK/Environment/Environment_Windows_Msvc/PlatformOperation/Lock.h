#pragma once

typedef  hndl  lock;

_INLINE_ lock  lock_create(void);
_INLINE_ boole lock_try_get(lock lk);
_INLINE_ void  lock_wait_get(lock lk);
_INLINE_ void  lock_put(lock lk);
_INLINE_ void  lock_destroy(lock lk);

struct rw_lock
{
    lock _write;
    s64  _is_writing;
    s64  _reader_count;
};

_INLINE_ rw_lock rw_lock_create(void);
_INLINE_ boole   rw_lock_try_read(rw_lock lk);
_INLINE_ boole   rw_lock_wait_read(rw_lock lk);
_INLINE_ boole   rw_lock_try_write(rw_lock lk);
_INLINE_ boole   rw_lock_wait_write(rw_lock lk);
_INLINE_ boole   rw_lock_put_read(rw_lock lk);
_INLINE_ boole   rw_lock_put_write(rw_lock lk);

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
    lk._write = lock_create();
    lk._is_writing = 0;
    lk._reader_count = 0;
    return lk;
}

_INLINE_ boole   rw_lock_try_read(rw_lock lk)
{

}

_INLINE_ boole   rw_lock_wait_read(rw_lock lk)
{
}

_INLINE_ boole   rw_lock_try_write(rw_lock lk)
{
}

_INLINE_ boole   rw_lock_wait_write(rw_lock lk)
{
}

_INLINE_ boole   rw_lock_put_read(rw_lock lk)
{
}

_INLINE_ boole   rw_lock_put_write(rw_lock lk)
{
}
