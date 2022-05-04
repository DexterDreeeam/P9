#pragma once

namespace _Internal {
namespace _Task {

class task_base
{
public:
    struct task_promise_base
    {
        task_promise_base();

        ~task_promise_base() = default;

        auto initial_suspend() noexcept
        {
            return is_suspend<false>();
        }

        is_suspend<false> final_suspend() noexcept;

        void unhandled_exception() noexcept
        {
            assert(0);
        }

        void* operator new(size_t sz)
        {
            return memory::alloc(sz);
        }

        void operator delete(void* p) noexcept
        {
            memory::free(p);
        }

        ref<runtime_context> _ctx;
    };

public:
    bool await_ready();

    void await_suspend(CoroTyBase parent_coro);

public:
    task_base() = delete;

    task_base(ref<runtime_context> ctx);

    task_base(const task_base& rhs);

    task_base& operator =(const task_base& rhs);

    ~task_base() = default;

public:
    void wait_complete();

protected:
    ref<runtime_context> _ctx;
};

}} // _Internal::_Task

template<typename RetTy>
class task : public _Internal::_Task::task_base
{
public:
    using SelfTy = task<RetTy>;

    struct task_promise : _Internal::_Task::task_base::task_promise_base
    {
        task_promise();

        ~task_promise() = default;

        SelfTy get_return_object();

        void return_value(const RetTy& rst);

        void return_value(RetTy&& rst);

        ref<RetTy> _r_rst;
    };

    using promise_type = task_promise;

public:
    task() = delete;

    task(ref<_Internal::_Task::runtime_context> ctx, ref<RetTy> r_rst);

    task(const task& rhs);

    task& operator =(const task& rhs);

    ~task() = default;

public:
    RetTy await_resume()
    {
        return RetTy(*_r_rst.raw_ptr());
    }

public:
    ref<RetTy> result_ref()
    {
        return _r_rst;
    }

    RetTy wait_result()
    {
        wait_complete();
        return await_resume();
    }

private:
    ref<RetTy> _r_rst;
};

template<>
class task<void> : public _Internal::_Task::task_base
{
public:
    using SelfTy = task<void>;

    struct task_promise : _Internal::_Task::task_base::task_promise_base
    {
        task_promise();

        ~task_promise() = default;

        SelfTy get_return_object();

        void return_void();
    };

    using promise_type = task_promise;

public:
    task() = delete;

    task(ref<_Internal::_Task::runtime_context> ctx);

    task(const task& rhs);

    task& operator =(const task& rhs);

    ~task() = default;

public:
    void await_resume()
    {
    }
};
