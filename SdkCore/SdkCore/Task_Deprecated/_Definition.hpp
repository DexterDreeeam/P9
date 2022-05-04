#pragma once

class action_base;

template<typename RetTy = void>
class action;

class task_base;

template<typename RetTy = void>
class task;

template<typename PromiseTy>
using CoroTy = std::coroutine_handle<PromiseTy>;

using CoroCtxTy = std::coroutine_handle<>;

// #define Await   co_await
// #define Return  co_return

namespace _Internal {
namespace _Task {

class sync_event_wrapper
{
public:
    sync_event_wrapper()
    {
        e.init();
    }

    ~sync_event_wrapper()
    {
        e.uninit();
    }

    void set()
    {
        e.set();
    }

    void wait()
    {
        e.wait();
    }

public:
    event e;
};

template<typename RetTy = void>
class suspend_never;

template<>
class suspend_never<void>
{
public:
    suspend_never()
    {
    }

    bool await_ready()
    {
        return true;
    }

    void await_suspend(CoroCtxTy coro)
    {
    }

    void await_resume()
    {
    }
};

template<typename RetTy>
class suspend_never
{
public:
    suspend_never(ref<RetTy> r_rst) :
        _r_rst(r_rst)
    {
    }

    suspend_never(const suspend_never& rhs) :
        _r_rst(rhs._r_rst)
    {
    }

    bool await_ready()
    {
        return true;
    }

    void await_suspend(CoroCtxTy coro)
    {
    }

    RetTy await_resume()
    {
        return RetTy(*_r_rst.raw_ptr());
    }

    ref<RetTy>  _r_rst;
};

}} // _Internal::_Task
