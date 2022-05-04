#pragma once

namespace _Internal {
namespace _Task {

_INLINE_ task_base::task_promise_base::task_promise_base() :
    _ctx(ref<runtime_context>::new_instance())
{
}

_INLINE_ is_suspend<false> task_base::task_promise_base::final_suspend() noexcept
{
    auto current_state = _ctx->_state.compare_exchange(
        task_state::Init, task_state::Task_Complete);

    if (current_state == task_state::Init)
    {
        // no waiting outsides, do nothing
    }
    else if (
        current_state == task_state::Task_AwaitEnter ||
        current_state == task_state::Task_AwaitCoroSet ||
        current_state == task_state::Task_AwaitExit)
    {
        // co_await is awaiting this
        while (_ctx->_state.get() != task_state::Task_AwaitExit)
        {
            yield();
        }
        assert(_ctx->_parent_coro);
        _ctx->_parent_coro.resume();
    }
    else if (
        current_state == task_state::WaitEnter ||
        current_state == task_state::WaitExit)
    {
        // there is wait_complete() function waiting this
        while (_ctx->_state.get() != task_state::WaitExit)
        {
            yield();
        }
        _ctx->_event.set();
    }
    else
    {
        // unexpected case
        assert(0);
    }
    return is_suspend<false>();
}

_INLINE_ bool task_base::await_ready()
{
    auto current_state = _ctx->_state.compare_exchange(
        task_state::Init, task_state::Task_AwaitEnter);

    if (current_state == task_state::Init)
    {
        return false;
    }
    else if (current_state == task_state::Task_Complete)
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

_INLINE_ void task_base::await_suspend(CoroTyBase parent_coro)
{
    escape_function ef_exit =
        [=]()
        {
            auto old_state = _ctx->_state.exchange(task_state::Task_AwaitExit);
            assert(old_state == task_state::Task_AwaitCoroSet);
        };

    _ctx->_parent_coro = parent_coro;
    auto old_state = _ctx->_state.exchange(task_state::Task_AwaitCoroSet);
    assert(old_state == task_state::Task_AwaitEnter);
}

_INLINE_ task_base::task_base(ref<runtime_context> ctx) :
    _ctx(ctx)
{
}

_INLINE_ task_base::task_base(const task_base& rhs) :
    _ctx(rhs._ctx)
{
}

_INLINE_ task_base& task_base::operator =(const task_base& rhs)
{
    _ctx = rhs._ctx;
    return *this;
}

_INLINE_ void task_base::wait_complete()
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
    else if (current_state == task_state::Task_Complete)
    {
        // complete already, just return
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

}} // _Internal::_Task

template<typename RetTy>
_INLINE_ task<RetTy>::task_promise::task_promise() :
    _Internal::_Task::task_base::task_promise_base(),
    _r_rst(ref<RetTy>::new_instance())
{
}

template<typename RetTy>
_INLINE_ task<RetTy>::SelfTy task<RetTy>::task_promise::get_return_object()
{
    auto coro = CoroTy<task_promise>::from_promise(*this);
    _ctx->_coro = coro;
    return SelfTy(_ctx, _r_rst);
}

template<typename RetTy>
_INLINE_ void task<RetTy>::task_promise::return_value(const RetTy& rst)
{
    assert(_r_rst.has_value());
    *_r_rst.raw_ptr() = rst;
}

template<typename RetTy>
_INLINE_ void task<RetTy>::task_promise::return_value(RetTy&& rst)
{
    assert(_r_rst.has_value());
    *_r_rst.raw_ptr() = (RetTy&&)rst;
}

template<typename RetTy>
_INLINE_ task<RetTy>::task(ref<_Internal::_Task::runtime_context> ctx, ref<RetTy> r_rst) :
    _Internal::_Task::task_base(ctx),
    _r_rst(r_rst)
{
}

template<typename RetTy>
_INLINE_ task<RetTy>::task(const task<RetTy>& rhs) :
    _Internal::_Task::task_base(rhs),
    _r_rst(rhs._r_rst)
{
}

template<typename RetTy>
_INLINE_ task<RetTy>& task<RetTy>::operator =(const task<RetTy>& rhs)
{
    _Internal::_Task::task_base::operator =(rhs);
    _r_rst = rhs._r_rst;
    return *this;
}

_INLINE_ task<void>::task_promise::task_promise() :
    _Internal::_Task::task_base::task_promise_base()
{
}

_INLINE_ task<void>::SelfTy task<void>::task_promise::get_return_object()
{
    auto coro = CoroTy<task_promise>::from_promise(*this);
    _ctx->_coro = coro;
    return SelfTy(_ctx);
}

_INLINE_ void task<void>::task_promise::return_void()
{
}

_INLINE_ task<void>::task(ref<_Internal::_Task::runtime_context> ctx) :
    _Internal::_Task::task_base(ctx)
{
}

_INLINE_ task<void>::task(const task<void>& rhs) :
    _Internal::_Task::task_base(rhs)
{
}

_INLINE_ task<void>& task<void>::operator =(const task<void>& rhs)
{
    _Internal::_Task::task_base::operator =(rhs);
    return *this;
}
