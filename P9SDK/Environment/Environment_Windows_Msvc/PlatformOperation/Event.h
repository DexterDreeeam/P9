#pragma once

typedef  hndl  evnt;

_INLINE_ evnt  evnt_create(void);
_INLINE_ void  evnt_set(evnt ev);
_INLINE_ void  evnt_wait(evnt ev);
_INLINE_ void  evnt_destroy(evnt ev);

_INLINE_ evnt evnt_create(void)
{
    return (evnt)WindowsMsvcNs::CreateEvent(NULL, FALSE, NULL, NULL);
}

_INLINE_ void evnt_set(evnt ev)
{
    assert(ev);
    WindowsMsvcNs::SetEvent((WindowsMsvcNs::HANDLE)ev);
}

_INLINE_ void evnt_wait(evnt ev)
{
    assert(ev);
    WindowsMsvcNs::WaitForSingleObject((WindowsMsvcNs::HANDLE)ev, INFINITE);
}

_INLINE_ void evnt_destroy(evnt ev)
{
    assert(ev);
    WindowsMsvcNs::CloseHandle((WindowsMsvcNs::HANDLE)ev);
}
