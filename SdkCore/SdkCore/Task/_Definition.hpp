#pragma once

// #define Await   co_await
// #define Return  co_return

namespace _Internal {
namespace _Task {

class task_async_base;

class task_base;

}} // _Internal::_Task

template<typename RetTy = void>
class task_async;

template<typename RetTy = void>
class task;

template<typename PromiseTy>
using CoroTy = std::coroutine_handle<PromiseTy>;

using CoroTyBase = std::coroutine_handle<>;


namespace _Internal {
namespace _Task {

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
    CoroTyBase   _coro;
    CoroTyBase   _parent_coro;
    event        _event;

    enum class task_state : s64
    {
        Init = 0x10,

        TaskAsync_Complete,
        TaskAsync_AwaitEnter,
        TaskAsync_AwaitCoroSet,
        TaskAsync_AwaitExit,

        Task_Complete,
        Task_AwaitEnter,
        Task_AwaitCoroSet,
        Task_AwaitExit,

        WaitEnter,
        WaitEventSet,
        WaitExit = WaitEventSet,
    };

    /* state definition

           TaskAsync_Complete       [task_async][sub_thread]      complete by sub_thread

           TaskAsync_AwaitEnter     [task_async][co_await]        await, but not complete
           TaskAsync_AwaitCoroSet   [task_async][co_await]        parent_coro set done
           TaskAsync_AwaitExit      [task_async][co_await]        await process exit

           Task_Complete                  [task][execute_thread]  complete by any thread

           Task_AwaitEnter                [task][co_await]        await, but not complete (another thread is running)
           Task_AwaitCoroSet              [task][co_await]        parent_coro set done
           Task_AwaitExit                 [task][co_await]        await process exit

           WaitEnter                         [*][wait_complete]   wait_complete() waiting outside
           WaitEventSet/WaitExit             [*][wait_complete]   wait_complete() setup event already

     */
    atom<s64>  _state;

    runtime_context() :
        _coro(nullptr),
        _parent_coro(nullptr),
        _event(),
        _state(task_state::Init)
    {
    }

    ~runtime_context() = default;
};

using task_state = runtime_context::task_state;

_INLINE_ bool operator ==(const task_state& s, s64 cmp)
{
    return (s64)s == cmp;
}

_INLINE_ bool operator ==(s64 cmp, const task_state& s)
{
    return (s64)s == cmp;
}

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

}} // _Internal::_Task
