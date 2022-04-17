
#include "../EnvironmentHeader.hpp"
#include "../../_Interface.hpp"

namespace _InternalNs
{

struct thread_call_back_context
{
    thread::thread_call_back_function* _fn;
    void* _par;
};

static void* thread_linux_call_back_fn(void* p)
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
    pthread_t _thrd;
};

boole thread::init(thread_call_back_function* fn)
{
    auto* ctx = pointer_convert(_ctx, 0, thread_context*);
    assert(ctx == nullptr);
    assert(fn);

    ctx = new thread_context();
    ctx->_fn = fn;
    // ctx->_thrd = pthread_t();
    _ctx = ctx;
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

    ::pthread_create(
        &ctx->_thrd, nullptr, _InternalNs::thread_linux_call_back_fn, cb_ctx);

    return boole::True;
}

boole thread::wait()
{
    auto* ctx = pointer_convert(_ctx, 0, thread_context*);
    assert(ctx);
    assert(ctx->_thrd);

    ::pthread_join(ctx->_thrd, nullptr);
    return boole::True;
}

boole thread::force_stop()
{
    auto* ctx = pointer_convert(_ctx, 0, thread_context*);
    assert(ctx);
    assert(ctx->_thrd);

    ::pthread_kill(ctx->_thrd, SIGKILL);
    return boole::True;
}
