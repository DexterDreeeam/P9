#pragma once

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

/*

namespace _CoroutineNs
{

struct TaskDummyValueTy
{
};

}

template<typename RetTy = _CoroutineNs::TaskDummyValueTy>
class task;

typedef  std::suspend_never   _std_suspend_never;
typedef  std::suspend_always  _std_suspend_always;

namespace _CoroutineNs
{

template<typename TaskTy>
class Promise;

template<>
class Promise<task<TaskDummyValueTy>>
{
    using TaskTy = task<TaskDummyValueTy>;

public:
    TaskTy get_return_object();

    auto initial_suspend() { return _std_suspend_never(); }

    auto final_suspend() { return _std_suspend_never(); }

    void return_void() {}
};

template<typename TaskValueTy>
class Promise<task<TaskValueTy>>
{
    using TaskTy = task<TaskValueTy>;

public:
    TaskTy get_return_object();

    auto initial_suspend() { return _std_suspend_never(); }

    auto final_suspend() { return _std_suspend_never(); }

    TaskValueTy return_value() { return TaskValueTy(); }
};

}

template<typename RetTy>
class task
{
public:
    using SelfTy = task<RetTy>;
    using ValueTy = RetTy;
    using PromiseTy = _CoroutineNs::Promise<SelfTy>;
    using HandleTy = std::coroutine_handle<PromiseTy>;

    using promise_type = PromiseTy;

public:
    task() = default;

    task(const task&) = delete;

    task(task&& rhs) :
        _cr_hndl(rhs._cr_hndl)
    {
        rhs._cr_hndl = nullptr;
    }

    explicit task(HandleTy h)

    Task& operator =(const Task&) = delete;

    task& operator =(task&& rhs)
    {
        if (_cr_hndl)
        {
            _cr_hndl.destroy();
        }
        _cr_hndl = rhs._cr_hndl;
        rhs._cr_hndl = nullptr;
        return *this;
    }

    ~task()
    {
        if (_cr_hndl)
        {
            _cr_hndl.destroy();
        }
    }

private:
    HandleTy _cr_hndl;
};

*/
