#pragma once

namespace _TaskNs
{

_INLINE_ task_base::task_promise_base::task_promise_base() :
    _ctx(ref<runtime_context>::new_instance())
{
}

_INLINE_ is_suspend<false> task_base::task_promise_base::final_suspend() noexcept
{
    auto current_state = _ctx->_state.compare_exchange(0, 18);
    if (current_state == 0)
    {
        // no waiting outsides, do nothing
    }
    else if (current_state >= 10 && current_state <= 12)
    {
        // co_await is awaiting this
        while (_ctx->_state.get() != 12)
        {
            yield();
        }
        assert(_ctx->_parent_coro);
        _ctx->_parent_coro.resume();
    }
    else if (current_state == 5 || current_state == 6)
    {
        // there is wait_complete() function waiting this
        if (current_state == 5)
        {
            while (_ctx->_state.get() != 6)
            {
                yield();
            }
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
    auto current_state = _ctx->_state.compare_exchange(0, 10);
    if (current_state == 0)
    {
        return false;
    }
    else if (current_state == 18)
    {
        return true;
    }
    else if (current_state == 6)
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
            auto old_state = _ctx->_state.exchange(12);
            assert(old_state == 11);
        };

    _ctx->_parent_coro = parent_coro;
    auto old_state = _ctx->_state.exchange(11);
    assert(old_state == 10);
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
    auto current_state = _ctx->_state.compare_exchange(0, 5);
    if (current_state == 0)
    {
        // not complete, setup event to wait
        _ctx->_event.init();
        _ctx->_state.set(6);
        _ctx->_event.wait();
        _ctx->_event.uninit();
        return;
    }
    else if (current_state == 18)
    {
        // complete already, just return
        return;
    }
    else if (current_state == 6)
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
_INLINE_ task<RetTy>::task_promise::task_promise() :
    _TaskNs::task_base::task_promise_base(),
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
_INLINE_ task<RetTy>::task(ref<_TaskNs::runtime_context> ctx, ref<RetTy> r_rst) :
    _TaskNs::task_base(ctx),
    _r_rst(r_rst)
{
}

template<typename RetTy>
_INLINE_ task<RetTy>::task(const task<RetTy>& rhs) :
    _TaskNs::task_base(rhs),
    _r_rst(rhs._r_rst)
{
}

template<typename RetTy>
_INLINE_ task<RetTy>& task<RetTy>::operator =(const task<RetTy>& rhs)
{
    _TaskNs::task_base::operator =(rhs);
    _r_rst = rhs._r_rst;
    return *this;
}

_INLINE_ task<void>::task_promise::task_promise() :
    _TaskNs::task_base::task_promise_base()
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

_INLINE_ task<void>::task(ref<_TaskNs::runtime_context> ctx) :
    _TaskNs::task_base(ctx)
{
}

_INLINE_ task<void>::task(const task<void>& rhs) :
    _TaskNs::task_base(rhs)
{
}

_INLINE_ task<void>& task<void>::operator =(const task<void>& rhs)
{
    _TaskNs::task_base::operator =(rhs);
    return *this;
}
