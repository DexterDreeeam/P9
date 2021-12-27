#pragma once

#include <coroutine>

// Coroutine Flow
/*
 *
 *     Task<ExpectedValueType> Coroutine_Foo(Args... args)
 *     {
 *         using TaskTy = Task<ExpectedValueType>;
 *         using PromiseTy = TaskTy::promise_type;
 *
 *         promise_object = PromiseTy(args...) or PromiseTy();
 *         __Return__ = promise_object.get_return_object();
 *         if (co_await promise_object.initial_suspend())
 *         {
 *             return __Return__;
 *         }
 *
 *         try
 *         {
 *             // Coroutine Foo Body
 *
 *             // 1. co_return {expr}
 *             promise_object.return_value({expr});
 *             goto final_suspend;
 *
 *
 *             // 2. co_return
 *             promise_object.return_void();
 *             goto final_suspend;
 *
 *
 *             // 3. co_yield {expr}
 *             promise_object.yield_value({expr});
 *
 *
 *             // 4. co_await {awaiter}
 *             #IF promise_object.await_transform() ACCEPT awaiter
 *                 awaiter = promise_object.await_transform(awaiter);
 *             #ENDIF
 *             if (awaiter.await_ready() == false)
 *             {
 *                 MyCoroutineHandle = std::coroutine_handle<PromiseTy>::from_promise(promise_object);
 *                 awaiter.await_suspend(MyCoroutineHandle);
 *             }
 *             await_rst = awaiter.await_resume();
 *
 *         }
 *         catch (...)
 *         {
 *             promise_object.unhandled_exception(std::current_exception());
 *         }
 *
 *     final_suspend:
 *         co_await promise_object.final_suspend();
 *     }
 *
 *
 */

template<typename PromiseTy> using CoroTy = std::coroutine_handle<PromiseTy>;

template<typename RetTy = void>
class task_set;

template<typename RetTy>
class task_set
{
public:
    using TaskTy = task_set<RetTy>;

public:
    struct PromiseTy
    {
        PromiseTy() :
            _thrd()
        {
        }

        ~PromiseTy()
        {
            if (_thrd.is_init())
            {
                _thrd.uninit();
            }
        }

        TaskTy get_return_object()
        {
            auto coro = CoroTy<PromiseTy>::from_promise(*this);
            return TaskTy(_thrd, coro);
        }

        auto initial_suspend()
        {
            auto coro = CoroTy<PromiseTy>::from_promise(*this);
            _thrd.init()
            return std::suspend_always();
        }

        auto final_suspend() noexcept
        {
            return std::suspend_never();
        }

        thread _thrd;
        event  _new_thread_can_run;
    };

public:
    struct PromiseTy;
    friend struct PromiseTy;
    using promise_type = PromiseTy;

public:
    task_set(thread& thrd, CoroTy<PromiseTy> coro) :
        _coro(coro),
        _rst(),
        _done(),
        _thrd(&thrd)
    {
        _done.init();
    }

    task_set(const task_set&) = delete;

    task_set(task_set&& rhs) :
        _coro(rhs._coro)
        _rst((RetTy&&)rhs._rst),
        _done((event&&)rhs._done),
        _thrd(rhs._thrd)
    {
    }

    task_set& operator =(const task_set&) = delete;

    task_set& operator =(task_set&& rhs)
    {
        _coro = rhs._coro;
        _rst = (RetTy&&)rhs._rst;
        _done = (event&&)rhs._done;
        return *this;
    }

    task_set& operator =(const RetTy& rst)
    {
        _rst = rst;
        if (_done.is_init())
        {
            _done.uninit();
        }
        _done.init();
        _done.set();
        return *this;
    }

    task_set& operator =(RetTy&& rst)
    {
        _rst = (RetTy&&)rst;
        if (_done.is_init())
        {
            _done.uninit();
        }
        _done.init();
        _done.set();
        return *this;
    }

    ~task_set()
    {
        if (_done.is_init())
        {
            _done.uninit();
        }
    }

public:
    void await_complete()
    {
        assert(_done.is_init());
        _done.wait();
    }

    static void execute_entrypoint(void* p)
    {
        pointer_convert(p, 0, CoroTy<PromiseTy>)->resume();
    }

private:
    CoroTy<PromiseTy>  _coro;
    RetTy              _rst;
    event              _done;
};
