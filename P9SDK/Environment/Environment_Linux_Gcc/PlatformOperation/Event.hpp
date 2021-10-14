#pragma once

typedef  hndl  evnt;

_INLINE_ evnt  evnt_create(void);
_INLINE_ void  evnt_set(evnt ev);
_INLINE_ void  evnt_wait(evnt ev);
_INLINE_ void  evnt_destroy(evnt ev);

_INLINE_ evnt evnt_create(void)
{
    return (evnt)LinuxGccNs::CreateEvent(NULL, FALSE, NULL, NULL);
}

_INLINE_ void evnt_set(evnt ev)
{
    assert(ev);
    LinuxGccNs::SetEvent((LinuxGccNs::HANDLE)ev);
}

_INLINE_ void evnt_wait(evnt ev)
{
    assert(ev);
    LinuxGccNs::WaitForSingleObject((LinuxGccNs::HANDLE)ev, INFINITE);
}

_INLINE_ void evnt_destroy(evnt ev)
{
    assert(ev);
    LinuxGccNs::CloseHandle((LinuxGccNs::HANDLE)ev);
}
