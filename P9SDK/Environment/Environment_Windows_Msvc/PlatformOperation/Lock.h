#pragma once

typedef  hndl  lock;

_INLINE_ lock  lock_create(void);
_INLINE_ RET   lock_try_get(lock lk);
_INLINE_ void  lock_wait_get(lock lk);
_INLINE_ void  lock_put(lock lk);
_INLINE_ void  lock_destroy(lock lk);

_INLINE_ lock lock_create(void)
{
    return (lock)WindowsMsvcNs::CreateMutex(NULL, FALSE, NULL);
}

typedef unsigned long DWORD;

_INLINE_ RET lock_try_get(lock lk)
{
    assert(lk);
    return
        WindowsMsvcNs::WaitForSingleObject((WindowsMsvcNs::HANDLE)lk, 0) == WAIT_OBJECT_0 ? RET_SUCCESS : RET_FAILED;
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
