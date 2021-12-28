#pragma once

template<typename PromiseTy>
using CoroTy = std::coroutine_handle<PromiseTy>;

using CoroCtxTy = std::coroutine_handle<>;

#define _Await_   co_await
#define _Return_  co_return

namespace _TaskNs
{

template<typename RetTy = void>
class is_suspend;

template<>
class is_suspend<void>
{
public:
    is_suspend(bool suspend) :
        _suspend(suspend)
    {
    }

    bool await_ready() const noexcept
    {
        return !_suspend;
    }

    void await_suspend(CoroCtxTy)
    {
    }

    void await_resume() const
    {
    }

    bool        _suspend;
};

template<typename RetTy>
class is_suspend
{
public:
    is_suspend(bool suspend, ref<RetTy> r_rst) :
        _suspend(suspend),
        _r_rst(r_rst)
    {
    }

    bool await_ready() const noexcept
    {
        return !_suspend;
    }

    void await_suspend(CoroCtxTy)
    {
    }

    RetTy await_resume() const
    {
        return *_r_rst.raw_ptr();
    }

    bool        _suspend;
    ref<RetTy>  _r_rst;
};

}
