#pragma once

template<typename RetTy = void>
class task;

template<>
class task<void>
{
public:
    using TaskTy = task<void>;

public:
    struct PromiseTy
    {
        TaskTy get_return_object()
        {
            auto coro = CoroTy<PromiseTy>::from_promise(*this);
            return TaskTy(coro);
        }

        auto initial_suspend()
        {
            return std::suspend_always();
        }

        auto final_suspend() noexcept
        {
            return std::suspend_never();
        }

        void return_void()
        {
        }

        void unhandled_exception() noexcept
        {
        }
    };

    struct callback_desc
    {
        CoroTy<PromiseTy>    coro;
        ref<void*>           parent_coro_addr;
        ref<atom<s64>>       sync_point;
    };

public:
    struct PromiseTy;
    friend struct PromiseTy;
    using promise_type = PromiseTy;

public:
    task(CoroTy<PromiseTy> coro) :
        _coro(coro),
        _parent_coro_addr(ref<void*>::new_instance(nullptr)),
        _sync_point(ref<atom<s64>>::new_instance(0))
    {
    }

    task(const task& rhs) :
        _coro(rhs._coro),
        _parent_coro_addr(rhs._parent_coro_addr),
        _sync_point(rhs._sync_point)
    {
    }

    task(task&& rhs) :
        _coro(rhs._coro),
        _parent_coro_addr(rhs._parent_coro_addr),
        _sync_point(rhs._sync_point)
    {
    }

    task& operator =(const task&) = delete;

    task& operator =(task&& rhs)
    {
        _coro = rhs._coro;
        _parent_coro_addr = rhs._parent_coro_addr;
        _sync_point = rhs._sync_point;
        return *this;
    }

    ~task()
    {
    }

public:
    bool await_ready() // 4
    {
        auto& l_sync_point = *_sync_point.raw_ptr();
        s64 my_order = ++l_sync_point;
        assert(my_order == 1 || my_order == 2);

        if (my_order == 1)
        {
            // _Await_ this Task, but it doesn't complete
            return false;
        }
        else
        {
            return true;
        }
    }

    bool await_suspend(CoroCtxTy coro)
    {
        *_parent_coro_addr.raw_ptr() = coro.address();

        auto& l_sync_point = *_sync_point.raw_ptr();
        ++l_sync_point;

        return true;
    }

    void await_resume()
    {
    }

public:
    TaskTy& execute_async()
    {
        thread t;
        t.init(execute_entrypoint);

        auto* p = new callback_desc();
        p->coro = _coro;
        p->parent_coro_addr = _parent_coro_addr;
        p->sync_point = _sync_point;

        t.start(p);
        t.uninit();

        return *this;
    }

private:
    static void execute_entrypoint(void* p)
    {
        callback_desc* p_desc = pointer_convert(p, 0, callback_desc*);

        // store desc content
        auto task_coro = p_desc->coro;
        auto parent_coro_addr = p_desc->parent_coro_addr;
        auto sync_point = p_desc->sync_point;

        delete p_desc;

        // execute Task function body
        task_coro.resume();

        auto& l_sync_point = *sync_point.raw_ptr();
        s64 my_order = ++l_sync_point;
        assert(my_order == 1 || my_order == 2 || my_order == 3);

        if (my_order == 1)
        {
            // no _Await_ operator waiting this Task now
            return;
        }
        else if (my_order == 3)
        {
            // resume Caller's execute body
            auto parent_coro = CoroCtxTy::from_address(*parent_coro_addr.raw_ptr());
            parent_coro.resume();
            return;
        }

        while (l_sync_point.get() == 2)
        {
            yield();
        }
    }

private:
    CoroTy<PromiseTy>  _coro;
    ref<void*>         _parent_coro_addr;

    // sync_point will add 1 when:
    // 1. await ready be called
    // 2. await suspend complete
    // 2. Task body complete
    ref<atom<s64>>     _sync_point;
};

template<typename RetTy>
class task
{
public:
    using TaskTy = task<RetTy>;

public:
    struct PromiseTy
    {
        TaskTy get_return_object() // 1
        {
            auto coro = CoroTy<PromiseTy>::from_promise(*this);
            _r_rst = ref<RetTy>::new_instance();
            return TaskTy(coro, _r_rst);
        }

        auto initial_suspend() // 2
        {
            return std::suspend_always();
        }

        auto final_suspend() noexcept
        {
            return std::suspend_never();
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

        void unhandled_exception() noexcept
        {
        }

        ref<RetTy> _r_rst;
    };

    struct callback_desc
    {
        CoroTy<PromiseTy>    coro;
        ref<void*>           parent_coro_addr;
        ref<atom<s64>>       sync_point;
    };

public:
    struct PromiseTy;
    friend struct PromiseTy;
    using promise_type = PromiseTy;

public:
    task(CoroTy<PromiseTy> coro, ref<RetTy> r_rst) :
        _coro(coro),
        _parent_coro_addr(ref<void*>::new_instance(nullptr)),
        _r_rst(r_rst),
        _sync_point(ref<atom<s64>>::new_instance(0))
    {
    }

    task(const task& rhs) :
        _coro(rhs._coro),
        _parent_coro_addr(rhs._parent_coro_addr),
        _r_rst(rhs._r_rst),
        _sync_point(rhs._sync_point)
    {
    }

    task(task&& rhs) :
        _coro(rhs._coro),
        _parent_coro_addr(rhs._parent_coro_addr),
        _r_rst(rhs._r_rst),
        _sync_point(rhs._sync_point)
    {
    }

    task& operator =(const task&) = delete;

    task& operator =(task&& rhs)
    {
        _coro = rhs._coro;
        _parent_coro_addr = rhs._parent_coro_addr;
        _r_rst = rhs._r_rst;
        _sync_point = rhs._sync_point;
        return *this;
    }

    ~task()
    {
    }

public:
    bool await_ready() // 4
    {
        auto& l_sync_point = *_sync_point.raw_ptr();
        s64 my_order = ++l_sync_point;
        assert(my_order == 1 || my_order == 2);

        if (my_order == 1)
        {
            // _Await_ this Task, but it doesn't complete
            return false;
        }
        else
        {
            return true;
        }
    }

    bool await_suspend(CoroCtxTy coro) // 5
    {
        *_parent_coro_addr.raw_ptr() = coro.address();

        auto& l_sync_point = *_sync_point.raw_ptr();
        ++l_sync_point;

        return true;
    }

    auto await_resume() // 6
    {
        return RetTy(*_r_rst.raw_ptr());
    }

public:
    TaskTy& execute_async() // 3
    {
        thread t;
        t.init(execute_entrypoint);

        auto* p = new callback_desc();
        p->coro = _coro;
        p->parent_coro_addr = _parent_coro_addr;
        p->sync_point = _sync_point;

        t.start(p);
        t.uninit();

        return *this;
    }

private:
    static void execute_entrypoint(void* p)
    {
        callback_desc* p_desc = pointer_convert(p, 0, callback_desc*);

        // store desc content
        auto task_coro = p_desc->coro;
        auto parent_coro_addr = p_desc->parent_coro_addr;
        auto sync_point = p_desc->sync_point;

        delete p_desc;

        // execute Task function body
        task_coro.resume();

        auto& l_sync_point = *sync_point.raw_ptr();
        s64 my_order = ++l_sync_point;
        assert(my_order == 1 || my_order == 2 || my_order == 3);

        if (my_order == 1)
        {
            // no _Await_ operator waiting this Task now
            return;
        }
        else if (my_order == 3)
        {
            // resume Caller's execute body
            auto parent_coro = CoroCtxTy::from_address(*parent_coro_addr.raw_ptr());
            parent_coro.resume();
            return;
        }

        while (l_sync_point.get() == 2)
        {
            yield();
        }
    }

private:
    CoroTy<PromiseTy>  _coro;
    ref<void*>         _parent_coro_addr;
    ref<RetTy>         _r_rst;

    // sync_point will add 1 when:
    // 1. await ready be called
    // 2. await suspend complete
    // 2. Task body complete
    ref<atom<s64>>     _sync_point;
};
