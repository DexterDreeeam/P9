#pragma once

typedef  hndl  thrd;
typedef  void  Callback_Func(void* param);

_INLINE_ void  yield();
_INLINE_ void  memory_barrier(void);
_INLINE_ thrd  thrd_create(Callback_Func* fn, void* param);
_INLINE_ u64   thrd_myid();
_INLINE_ void  thrd_enforce_exit(thrd td);
_INLINE_ void  thrd_wait_exit(thrd td);
_INLINE_ void  thrd_destroy(thrd td);

_INLINE_ void yield()
{
    WindowsMsvcNs::SwitchToThread();
}

_INLINE_ void memory_barrier(void)
{
    WindowsMsvcNs::MemoryBarrier();
}

struct thrd_windows_cb_input
{
    thrd_windows_cb_input(Callback_Func* f, void* p) : fn(f), param(p) {}
    Callback_Func* fn;
    void* param;
};

_INLINE_ unsigned long __stdcall thrd_windows_cb(void* p)
{
    auto* input = (thrd_windows_cb_input*)p;
    (input->fn)(input->param);
    delete input;
    return 0;
}

_INLINE_ thrd thrd_create(Callback_Func* fn, void* param)
{
    assert(fn);
    auto* input = new thrd_windows_cb_input(fn, param);
    return (thrd)WindowsMsvcNs::CreateThread(NULL, 0, thrd_windows_cb, input, 0, NULL);
}

_INLINE_ u64 thrd_myid()
{
    return static_cast<u64>(WindowsMsvcNs::GetCurrentThreadId());
}

_INLINE_ void thrd_enforce_exit(thrd td)
{
    assert(td);
    WindowsMsvcNs::TerminateThread((WindowsMsvcNs::HANDLE)td, 0);
}

_INLINE_ void thrd_wait_exit(thrd td)
{
    assert(td);
    WindowsMsvcNs::WaitForSingleObject((WindowsMsvcNs::HANDLE)td, INFINITE);
}

_INLINE_ void thrd_destroy(thrd td)
{
    assert(td);
    WindowsMsvcNs::CloseHandle((WindowsMsvcNs::HANDLE)td);
}
