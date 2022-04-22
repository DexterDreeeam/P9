#pragma once

class task_set_base
{
public:
    struct TaskSetPromiseBase
    {
        std::suspend_never initial_suspend() noexcept
        {
            return std::suspend_never();
        }

        std::suspend_never final_suspend() noexcept
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
};

template<typename RetTy = void>
class task_set;

template<>
class task_set<void> : public task_set_base
{
public:
    using TaskSetTy = task_set<void>;

public:
    struct TaskSetPromiseTy : public task_set_base::TaskSetPromiseBase
    {
        TaskSetTy get_return_object()
        {
            _r_done = ref<_TaskNs::sync_event_wrapper>::new_instance();
            return TaskSetTy(_r_done);
        }

        void return_void()
        {
            _r_done->set();
        }

        ref<_TaskNs::sync_event_wrapper> _r_done;
    };

    using promise_type = TaskSetPromiseTy;

public:
    task_set(ref<_TaskNs::sync_event_wrapper> r_done) :
        _r_done(r_done)
    {
    }

    task_set(const task_set&) = delete;

    task_set(task_set&& rhs) :
        _r_done(rhs._r_done)
    {
    }

    task_set& operator =(const task_set&) = delete;

    task_set& operator =(task_set&& rhs)
    {
        _r_done = rhs._r_done;
        return *this;
    }

    ~task_set() = default;

public:
    void await_complete()
    {
        _r_done->wait();
    }

private:
    ref<_TaskNs::sync_event_wrapper> _r_done;
};

template<typename RetTy>
class task_set : public task_set_base
{
public:
    using TaskSetTy = task_set<RetTy>;

public:
    struct TaskSetPromiseTy : public task_set_base::TaskSetPromiseBase
    {
        TaskSetTy get_return_object()
        {
            _r_rst = ref<RetTy>::new_instance();
            _r_done = ref<_TaskNs::sync_event_wrapper>::new_instance();
            return TaskSetTy(_r_rst, _r_done);
        }

        void return_value(const RetTy& rst)
        {
            assert(_r_rst.has_value());
            *_r_rst.raw_ptr() = rst;
            _r_done->set();
        }

        void return_value(RetTy&& rst)
        {
            assert(_r_rst.has_value());
            *_r_rst.raw_ptr() = (RetTy&&)rst;
            _r_done->set();
        }

        ref<RetTy>                       _r_rst;
        ref<_TaskNs::sync_event_wrapper> _r_done;
    };

    using promise_type = TaskSetPromiseTy;

public:
    task_set(ref<RetTy> r_rst, ref<_TaskNs::sync_event_wrapper> r_done) :
        _r_rst(r_rst),
        _r_done(r_done)
    {
    }

    task_set(const task_set&) = delete;

    task_set(task_set&& rhs) :
        _r_rst(rhs._r_rst),
        _r_done(rhs._r_done)
    {
    }

    task_set& operator =(const task_set&) = delete;

    task_set& operator =(task_set&& rhs)
    {
        _r_rst = rhs._r_rst;
        _r_done = rhs._r_done;
        return *this;
    }

    ~task_set() = default;

public:
    RetTy await_complete()
    {
        _r_done->wait();
        return RetTy(*_r_rst.raw_ptr());
    }

private:
    ref<RetTy>                       _r_rst;
    ref<_TaskNs::sync_event_wrapper> _r_done;
};
