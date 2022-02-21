#pragma once

namespace _InternalNs
{

struct timer_context
{
    timer::timer_call_back_function* fn;
};

struct timer_cb_context
{
    timer::timer_call_back_function* fn;
    s64 delay;
    void* par;
};

static void timer_call_back_fn(void* par)
{
    auto* ctx = pointer_convert(par, 0, timer_cb_context*);
    tick::sleep(ctx->delay);
    ctx->fn(ctx->par);
    delete ctx;
}

}

_INLINE_ timer::timer() :
    _ctx(nullptr)
{
}

_INLINE_ timer::timer(const timer& rhs) :
    _ctx(rhs._ctx)
{
}

_INLINE_ timer& timer::operator =(const timer& rhs)
{
    _ctx = rhs._ctx;
    return *this;
}

_INLINE_ boole timer::is_init()
{
    return _ctx != nullptr;
}

_INLINE_ boole timer::init(timer_call_back_function* fn)
{
    assert(fn);
    assert(_ctx == nullptr);

    auto* ctx = new _InternalNs::timer_context();
    ctx->fn = fn;
    _ctx = ctx;
    return boole::True;
}

_INLINE_ boole timer::uninit()
{
    auto* ctx = pointer_convert(_ctx, 0, _InternalNs::timer_context*);
    assert(ctx);

    delete ctx;
    _ctx = nullptr;
    return boole::True;
}

_INLINE_ boole timer::trigger(void* par, u64 ms)
{
    auto* ctx = pointer_convert(_ctx, 0, _InternalNs::timer_context*);
    assert(ctx);

    auto* cb_ctx = new _InternalNs::timer_cb_context();
    cb_ctx->fn = ctx->fn;
    cb_ctx->par = par;
    cb_ctx->delay = ms;

    thread t;
    if (t.init(_InternalNs::timer_call_back_fn) == boole::False)
    {
        delete cb_ctx;
        return boole::False;
    }
    if (t.start(cb_ctx) == boole::False)
    {
        t.uninit();
        delete cb_ctx;
        return boole::False;
    }
    t.uninit();
    return boole::True;
}
