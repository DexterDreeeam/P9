#pragma once

namespace _TaskNs
{

_INLINE_ task_async_base::task_async_promise_base::task_async_promise_base() :
    _ctx(ref<runtime_context>::new_instance())
{
}

_INLINE_ is_suspend<true> task_async_base::task_async_promise_base::initial_suspend() noexcept
{
    thread t;
    t.init(task_async_base::sub_thread_entrypoint);
    t.start(this);
    t.uninit();
    return is_suspend<true>();
}

_INLINE_ bool task_async_base::await_ready()
{
    auto current_state = _ctx->_state.compare_exchange(
        task_state::Init, task_state::TaskAsync_AwaitEnter);

    if (current_state == task_state::Init)
    {
        return false;
    }
    else if (current_state == task_state::TaskAsync_Complete)
    {
        return true;
    }
    else if (current_state == task_state::WaitExit)
    {
        return true;
    }
    else
    {
        // unexpected
        assert(0);
        return true;
    }
}

_INLINE_ void task_async_base::await_suspend(CoroTyBase parent_coro)
{
    escape_function ef_exit =
        [=]()
        {
            auto old_state = _ctx->_state.exchange(task_state::TaskAsync_AwaitExit);
            assert(old_state == task_state::TaskAsync_AwaitCoroSet);
        };

    _ctx->_parent_coro = parent_coro;
    auto old_state = _ctx->_state.exchange(task_state::TaskAsync_AwaitCoroSet);
    assert(old_state == task_state::TaskAsync_AwaitEnter);
}

_INLINE_ task_async_base::task_async_base(ref<runtime_context> ctx) :
    _ctx(ctx)
{
}

_INLINE_ task_async_base::task_async_base(const task_async_base& rhs) :
    _ctx(rhs._ctx)
{
}

_INLINE_ task_async_base& task_async_base::operator =(const task_async_base& rhs)
{
    _ctx = rhs._ctx;
    return *this;
}

_INLINE_ void task_async_base::sub_thread_entrypoint(void* p)
{
    auto* promise = pointer_convert(p, 0, task_async_promise_base*);
    auto ctx = promise->_ctx;

    ctx->_coro.resume();

    auto current_state = ctx->_state.compare_exchange(
        task_state::Init, task_state::TaskAsync_Complete);

    if (current_state == task_state::Init)
    {
        // there is no one waiting this yet
        return;
    }
    else if (
        current_state == task_state::TaskAsync_AwaitEnter ||
        current_state == task_state::TaskAsync_AwaitCoroSet ||
        current_state == task_state::TaskAsync_AwaitExit)
    {
        // co_await is awaiting this
        while (ctx->_state.get() != task_state::TaskAsync_AwaitExit)
        {
            yield();
        }
        assert(ctx->_parent_coro);
        ctx->_parent_coro.resume();
        return;
    }
    else if (current_state == task_state::WaitEnter || current_state == task_state::WaitExit)
    {
        // there is wait_complete() function waiting this
        while (ctx->_state.get() != task_state::WaitExit)
        {
            yield();
        }
        ctx->_event.set();
        return;
    }
    else
    {
        // unexpected case
        assert(0);
    }
}

_INLINE_ void task_async_base::wait_complete()
{
    auto current_state = _ctx->_state.compare_exchange(
        task_state::Init, task_state::WaitEnter);

    if (current_state == task_state::Init)
    {
        // not complete, setup event to wait
        _ctx->_event.init();
        _ctx->_state.set(task_state::WaitExit);
        _ctx->_event.wait();
        _ctx->_event.uninit();
        return;
    }
    else if (current_state == task_state::TaskAsync_Complete)
    {
        // sub_thread complete already, just return
        return;
    }
    else if (current_state == task_state::WaitExit)
    {
        // there is already wait_complete() ahead this one
        return;
    }
    else
    {
        // unexpected operation, not support
        assert(0);
    }
}

}

template<typename RetTy>
_INLINE_ task_async<RetTy>::task_async_promise::task_async_promise() :
    _TaskNs::task_async_base::task_async_promise_base(),
    _r_rst(ref<RetTy>::new_instance())
{
}

template<typename RetTy>
_INLINE_ task_async<RetTy>::SelfTy task_async<RetTy>::task_async_promise::get_return_object()
{
    auto coro = CoroTy<task_async_promise>::from_promise(*this);
    _ctx->_coro = coro;
    return SelfTy(_ctx, _r_rst);
}

template<typename RetTy>
_INLINE_ void task_async<RetTy>::task_async_promise::return_value(const RetTy& rst)
{
    assert(_r_rst.has_value());
    *_r_rst.raw_ptr() = rst;
}

template<typename RetTy>
_INLINE_ void task_async<RetTy>::task_async_promise::return_value(RetTy&& rst)
{
    assert(_r_rst.has_value());
    *_r_rst.raw_ptr() = (RetTy&&)rst;
}

template<typename RetTy>
_INLINE_ task_async<RetTy>::task_async(ref<_TaskNs::runtime_context> ctx, ref<RetTy> r_rst) :
    _TaskNs::task_async_base(ctx),
    _r_rst(r_rst)
{
}

template<typename RetTy>
_INLINE_ task_async<RetTy>::task_async(const task_async<RetTy>& rhs) :
    _TaskNs::task_async_base(rhs),
    _r_rst(rhs._r_rst)
{
}

template<typename RetTy>
_INLINE_ task_async<RetTy>& task_async<RetTy>::operator =(const task_async<RetTy>& rhs)
{
    _TaskNs::task_async_base::operator =(rhs);
    _r_rst = rhs._r_rst;
    return *this;
}

_INLINE_ task_async<void>::task_async_promise::task_async_promise() :
    _TaskNs::task_async_base::task_async_promise_base()
{
}

_INLINE_ task_async<void>::SelfTy task_async<void>::task_async_promise::get_return_object()
{
    auto coro = CoroTy<task_async_promise>::from_promise(*this);
    _ctx->_coro = coro;
    return SelfTy(_ctx);
}

_INLINE_ void task_async<void>::task_async_promise::return_void()
{
}

_INLINE_ task_async<void>::task_async(ref<_TaskNs::runtime_context> ctx) :
    _TaskNs::task_async_base(ctx)
{
}

_INLINE_ task_async<void>::task_async(const task_async<void>& rhs) :
    _TaskNs::task_async_base(rhs)
{
}

_INLINE_ task_async<void>& task_async<void>::operator =(const task_async<void>& rhs)
{
    _TaskNs::task_async_base::operator =(rhs);
    return *this;
}
