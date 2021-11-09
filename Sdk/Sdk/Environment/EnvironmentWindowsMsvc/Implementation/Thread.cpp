
#include "../../Interface.hpp"
#include "../EnvironmentHeader.hpp"

namespace _InternalNs
{

struct thread_call_back_context
{
    thread::thread_call_back_function* _fn;
    void* _par;
};

static unsigned long __stdcall thread_windows_call_back_fn(void* p)
{
    auto* cb_ctx = pointer_convert(p, 0, thread_call_back_context*);

    cb_ctx->_fn(cb_ctx->_par);
    delete cb_ctx;
    return 0;
}

}

struct thread_context
{
    thread::thread_call_back_function* _fn;
    HANDLE _hndl;
};

boole thread::init(thread_call_back_function* fn)
{
    auto* ctx = pointer_convert(_ctx, 0, thread_context*);
    assert(ctx == nullptr);
    assert(fn);

    ctx = new thread_context();
    ctx->_fn = fn;
    ctx->_hndl = nullptr;
    return boole::True;
}

boole thread::uninit()
{
    auto* ctx = pointer_convert(_ctx, 0, thread_context*);
    assert(ctx);
    assert(ctx->_fn);

    delete ctx;
    _ctx = nullptr;
    return boole::True;
}

boole thread::start(void* par)
{
    auto* ctx = pointer_convert(_ctx, 0, thread_context*);
    assert(ctx);
    assert(ctx->_fn);

    auto* cb_ctx = new _InternalNs::thread_call_back_context();
    cb_ctx->_fn = ctx->_fn;
    cb_ctx->_par = par;

    HANDLE h = ::CreateThread(NULL, 0, _InternalNs::thread_windows_call_back_fn, cb_ctx, 0, NULL);
    if (h)
    {
        ctx->_hndl = h;
        return boole::True;
    }
    else
    {
        delete cb_ctx;
        return boole::False;
    }
}

boole thread::wait()
{
    auto* ctx = pointer_convert(_ctx, 0, thread_context*);
    assert(ctx);
    assert(ctx->_hndl);

    if (::WaitForSingleObject(ctx->_hndl, INFINITE))
    {
        return boole::True;
    }
    else
    {
        return boole::False;
    }
}

boole thread::force_stop()
{
    auto* ctx = pointer_convert(_ctx, 0, thread_context*);
    assert(ctx);
    assert(ctx->_hndl);

    if (::TerminateThread(ctx->_hndl, 0))
    {
        return boole::True;
    }
    else
    {
        return boole::False;
    }
}
