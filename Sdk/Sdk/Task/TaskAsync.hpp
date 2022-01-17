#pragma once

namespace _TaskNs
{

class task_async_base
{
public:
    struct task_async_promise_base
    {
        task_async_promise_base();

        ~task_async_promise_base() = default;

        is_suspend<true> initial_suspend() noexcept;

        auto final_suspend() noexcept
        {
            return is_suspend<false>();
        }

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
    task_async_base() = delete;

    task_async_base(ref<runtime_context> ctx);

    task_async_base(const task_async_base& rhs);

    task_async_base& operator =(const task_async_base& rhs);

    ~task_async_base() = default;

public:
    static void sub_thread_entrypoint(void* p);

public:
    void wait_complete();

protected:
    ref<runtime_context> _ctx;
};

}

template<typename RetTy>
class task_async : public _TaskNs::task_async_base
{
public:
    using SelfTy = task_async<RetTy>;

    struct task_async_promise : _TaskNs::task_async_base::task_async_promise_base
    {
        task_async_promise();

        ~task_async_promise() = default;

        SelfTy get_return_object();

        void return_value(const RetTy& rst);

        void return_value(RetTy&& rst);

        ref<RetTy> _r_rst;
    };

    using promise_type = task_async_promise;

public:
    task_async() = delete;

    task_async(ref<_TaskNs::runtime_context> ctx, ref<RetTy> r_rst);

    task_async(const task_async& rhs);

    task_async& operator =(const task_async& rhs);

    ~task_async() = default;

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
class task_async<void> : public _TaskNs::task_async_base
{
public:
    using SelfTy = task_async<void>;

    struct task_async_promise : _TaskNs::task_async_base::task_async_promise_base
    {
        task_async_promise();

        ~task_async_promise() = default;

        SelfTy get_return_object();

        void return_void();
    };

    using promise_type = task_async_promise;

public:
    task_async() = delete;

    task_async(ref<_TaskNs::runtime_context> ctx);

    task_async(const task_async& rhs);

    task_async& operator =(const task_async& rhs);

    ~task_async() = default;

public:
    void await_resume()
    {
    }
};
