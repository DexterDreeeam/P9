#pragma once

class task_base
{
    friend struct action_base::ActionPromiseBase;

public:
    struct TaskPromiseBase
    {
        constexpr std::suspend_always initial_suspend() noexcept
        {
            return std::suspend_always();
        }

        constexpr std::suspend_never final_suspend() noexcept
        {
            return std::suspend_never();
        }

        void unhandled_exception() noexcept
        {
        }

        void* operator new(size_t sz)
        {
            return memory::alloc(sz);
        }

        void operator delete(void* p)
        {
            return memory::free(p);
        }
    };

public:
    bool await_ready();

    bool await_suspend(CoroCtxTy coro);

public:
    task_base(CoroCtxTy coro);

    task_base(const task_base& rhs);

    task_base& operator =(const task_base& rhs);

    ~task_base() = default;

public:
    struct callback_context
    {
        ref<atom<s64>>                 sync_point;
        CoroCtxTy                      coro;
        ref<void*>                     parent_coro_addr;
        _TaskNs::sync_event_wrapper**  pp_action_sync_event;
    };

protected:
    void execute_async();

private:
    void set_action_sync_event(_TaskNs::sync_event_wrapper* action_sync_event)
    {
        _p_action_sync_event = action_sync_event;
    }

    static void execute_UnitTest(void* p);

protected:
    // sync_point will add 1 when:
    // 1. await ready be called
    // 2. await suspend complete
    // 2. Task body complete
    ref<atom<s64>>                _sync_point;
    CoroCtxTy                     _coro;
    ref<void*>                    _parent_coro_addr;
    _TaskNs::sync_event_wrapper*  _p_action_sync_event;
};

template<>
class task<void> : public task_base
{
public:
    using TaskTy = task<void>;

    friend class task_base;
    template<typename AllTy> friend class task;

    friend class action_base;
    template<typename AllTy> friend class action;

    struct TaskPromiseTy : public task_base::TaskPromiseBase
    {
        TaskTy get_return_object()
        {
            auto coro = CoroTy<TaskPromiseTy>::from_promise(*this);
            return TaskTy(coro);
        }

        void return_void()
        {
        }
    };

    using promise_type = TaskPromiseTy;

public:
    task(CoroCtxTy coro) :
        task_base(coro)
    {
    }

    task(const task& rhs) :
        task_base(rhs)
    {
    }

    task(task&& rhs) :
        task_base(rhs)
    {
    }

    task& operator =(const task&) = delete;

    task& operator =(task&& rhs)
    {
        task_base::operator =(rhs);
        return *this;
    }

    ~task() = default;

public:
    void await_resume()
    {
    }

public:
    TaskTy& execute_async()
    {
        task_base::execute_async();
        return *this;
    }
};

template<typename RetTy>
class task : public task_base
{
public:
    using TaskTy = task<RetTy>;

    friend class task_base;
    template<typename AllTy> friend class task;

    friend class action_base;
    template<typename AllTy> friend class action;

    struct TaskPromiseTy : public task_base::TaskPromiseBase
    {
        TaskTy get_return_object()
        {
            auto coro = CoroTy<TaskPromiseTy>::from_promise(*this);
            _r_rst = ref<RetTy>::new_instance();
            return TaskTy(coro, _r_rst);
        }

        void return_value(const RetTy& rst)
        {
            assert(_r_rst.has_value());
            *_r_rst.raw_ptr() = rst;
        }

        void return_value(RetTy&& rst)
        {
            assert(_r_rst.has_value());
            *_r_rst.raw_ptr() = (RetTy&&)rst;
        }

        ref<RetTy> _r_rst;
    };

    using promise_type = TaskPromiseTy;

public:
    task(CoroTy<TaskPromiseTy> coro, ref<RetTy> r_rst) :
        task_base(coro),
        _r_rst(r_rst)
    {
    }

    task(const task& rhs) :
        task_base(rhs),
        _r_rst(rhs._r_rst)
    {
    }

    task(task&& rhs) :
        task_base(rhs),
        _r_rst(rhs._r_rst)
    {
    }

    task& operator =(const task&) = delete;

    task& operator =(task&& rhs)
    {
        task_base::operator =(rhs);
        _r_rst = rhs._r_rst;
        return *this;
    }

    ~task() = default;

public:
    RetTy await_resume()
    {
        return RetTy(*_r_rst.raw_ptr());
    }

public:
    TaskTy& execute_async()
    {
        task_base::execute_async();
        return *this;
    }

    ref<RetTy> result_ref()
    {
        return _r_rst;
    }

private:
    ref<RetTy>  _r_rst;
};
