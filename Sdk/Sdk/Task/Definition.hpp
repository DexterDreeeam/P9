#pragma once

// #define Await   co_await
// #define Return  co_return


namespace _TaskNs
{

class task_async_base;

class task_base;

}

template<typename RetTy = void>
class task_async;

template<typename RetTy = void>
class task;

template<typename PromiseTy>
using CoroTy = std::coroutine_handle<PromiseTy>;

using CoroTyBase = std::coroutine_handle<>;


namespace _TaskNs
{

class auto_event
{
public:
    class auto_event_internal
    {
    public:
        auto_event_internal()
        {
            e.init();
        }

        auto_event_internal(const auto_event_internal&) = delete;

        auto_event_internal& operator =(const auto_event_internal&) = delete;

        ~auto_event_internal()
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

        event e;
    };

public:
    auto_event() :
        re(ref<auto_event_internal>::new_instance())
    {
    }

    ~auto_event()
    {
    }

    auto_event(const auto_event& rhs) :
        re(rhs.re)
    {
    }

    auto_event& operator =(const auto_event& rhs)
    {
        re = rhs.re;
        return *this;
    }

    void set()
    {
        re->set();
    }

    void wait()
    {
        re->wait();
    }

private:
    ref<auto_event_internal> re;
};

struct runtime_context
{
    runtime_context() :
        _coro(nullptr),
        _parent_coro(nullptr),
        _event(),
        _state(0)
    {
    }

    ~runtime_context() = default;

    CoroTyBase   _coro;
    CoroTyBase   _parent_coro;
    event        _event;

    /* state definition
           0 -> 16  [task_async][sub_thread]      complete by sub_thread

           0 -> 1   [task_async][co_await]        await, but not complete
           1 -> 2   [task_async][co_await]        parent_coro set done
           2 -> 3   [task_async][co_await]        await process exit

           0 -> 5            [*][wait_complete]   wait_complete() waiting outside
           5 -> 6            [*][wait_complete]   wait_complete() setup event already

           0 -> 18        [task][execute_thread]  complete by any thread

           0 -> 10        [task][co_await]        await, but not complete (another thread is running)
          10 -> 11        [task][co_await]        parent_coro set done
          11 -> 12        [task][co_await]        await process exit
     */
    atom<s64>  _state;
};

template<bool suspend>
class is_suspend
{
public:
    is_suspend() noexcept
    {
    }

    bool await_ready() noexcept
    {
        return !suspend;
    }

    void await_suspend(CoroTyBase coro) noexcept
    {
    }

    void await_resume() noexcept
    {
    }
};

}
