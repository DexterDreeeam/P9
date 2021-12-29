#pragma once

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

        //template<typename AwaiterTy>
        //AwaiterTy await_transform(const AwaiterTy& awaiter)
        //{
        //    return awaiter;
        //}

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
    using promise_type = PromiseTy;
    friend struct PromiseTy;
    template<typename AllTy> friend class task;
    template<typename AllTy> friend class action;

public:
    task(CoroTy<PromiseTy> coro) :
        _coro(coro),
        _parent_coro_addr(ref<void*>::new_instance(nullptr)),
        _sync_point(ref<atom<s64>>::new_instance(0)),
        _p_action_sync_event(nullptr)
    {
    }

    task(const task& rhs) :
        _coro(rhs._coro),
        _parent_coro_addr(rhs._parent_coro_addr),
        _sync_point(rhs._sync_point),
        _p_action_sync_event(rhs._p_action_sync_event)
    {
    }

    task(task&& rhs) :
        _coro(rhs._coro),
        _parent_coro_addr(rhs._parent_coro_addr),
        _sync_point(rhs._sync_point),
        _p_action_sync_event(rhs._p_action_sync_event)
    {
    }

    task& operator =(const task&) = delete;

    task& operator =(task&& rhs)
    {
        _coro = rhs._coro;
        _parent_coro_addr = rhs._parent_coro_addr;
        _sync_point = rhs._sync_point;
        _p_action_sync_event = rhs._p_action_sync_event;
        return *this;
    }

    ~task()
    {
    }

public:
    bool await_ready()
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
    struct callback_desc
    {
        CoroTy<PromiseTy>    coro;
        ref<void*>           parent_coro_addr;
        ref<atom<s64>>       sync_point;
        _TaskNs::action_sync_event_wrapper**
                             pp_action_sync_event;
    };

    TaskTy& execute_async()
    {
        thread t;
        t.init(execute_entrypoint);

        auto* p = new callback_desc();
        p->coro = _coro;
        p->parent_coro_addr = _parent_coro_addr;
        p->sync_point = _sync_point;
        p->pp_action_sync_event = &_p_action_sync_event;

        t.start(p);
        t.uninit();

        return *this;
    }

private:
    void set_action_sync_event(_TaskNs::action_sync_event_wrapper* action_sync_event)
    {
        _p_action_sync_event = action_sync_event;
    }

    static void execute_entrypoint(void* p)
    {
        callback_desc* p_desc = pointer_convert(p, 0, callback_desc*);

        // store desc content
        auto task_coro = p_desc->coro;
        auto parent_coro_addr = p_desc->parent_coro_addr;
        auto sync_point = p_desc->sync_point;
        auto pp_action_sync_event = p_desc->pp_action_sync_event;

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

        if (*pp_action_sync_event)
        {
            // action call task
            // action _Await_ done
            // notify action thread
            (*pp_action_sync_event)->set();
        }

        while (my_order == 2)
        {
            yield();
            my_order = l_sync_point.get();
        }

        assert(my_order == 3);
        // resume Caller's execute body
        auto parent_coro = CoroCtxTy::from_address(*parent_coro_addr.raw_ptr());
        parent_coro.resume();
    }

private:
    CoroTy<PromiseTy>  _coro;
    ref<void*>         _parent_coro_addr;

    // sync_point will add 1 when:
    // 1. await ready be called
    // 2. await suspend complete
    // 2. Task body complete
    ref<atom<s64>>     _sync_point;
    _TaskNs::action_sync_event_wrapper*
                       _p_action_sync_event;
};

template<typename RetTy>
class task
{
public:
    using TaskTy = task<RetTy>;

public:
    struct PromiseTy
    {
        TaskTy get_return_object()
        {
            auto coro = CoroTy<PromiseTy>::from_promise(*this);
            _r_rst = ref<RetTy>::new_instance();
            return TaskTy(coro, _r_rst);
        }

        auto initial_suspend()
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

        //template<typename AwaiterTy>
        //AwaiterTy await_transform(const AwaiterTy& awaiter)
        //{
        //    return awaiter;
        //}

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

        ref<RetTy> _r_rst;
    };

public:
    using promise_type = PromiseTy;
    friend struct PromiseTy;
    template<typename AllTy> friend class task;
    template<typename AllTy> friend class action;

public:
    task(CoroTy<PromiseTy> coro, ref<RetTy> r_rst) :
        _coro(coro),
        _parent_coro_addr(ref<void*>::new_instance(nullptr)),
        _r_rst(r_rst),
        _sync_point(ref<atom<s64>>::new_instance(0)),
        _p_action_sync_event(nullptr)
    {
    }

    task(const task& rhs) :
        _coro(rhs._coro),
        _parent_coro_addr(rhs._parent_coro_addr),
        _r_rst(rhs._r_rst),
        _sync_point(rhs._sync_point),
        _p_action_sync_event(rhs._p_action_sync_event)
    {
    }

    task(task&& rhs) :
        _coro(rhs._coro),
        _parent_coro_addr(rhs._parent_coro_addr),
        _r_rst(rhs._r_rst),
        _sync_point(rhs._sync_point),
        _p_action_sync_event(rhs._p_action_sync_event)
    {
    }

    task& operator =(const task&) = delete;

    task& operator =(task&& rhs)
    {
        _coro = rhs._coro;
        _parent_coro_addr = rhs._parent_coro_addr;
        _r_rst = rhs._r_rst;
        _sync_point = rhs._sync_point;
        _p_action_sync_event = rhs._p_action_sync_event;
        return *this;
    }

    ~task()
    {
    }

public:
    bool await_ready()
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

    auto await_resume()
    {
        return RetTy(*_r_rst.raw_ptr());
    }

public:
    struct callback_desc
    {
        CoroTy<PromiseTy>    coro;
        ref<void*>           parent_coro_addr;
        ref<atom<s64>>       sync_point;
        _TaskNs::action_sync_event_wrapper**
                             pp_action_sync_event;
    };

    TaskTy& execute_async()
    {
        thread t;
        t.init(execute_entrypoint);

        auto* p = new callback_desc();
        p->coro = _coro;
        p->parent_coro_addr = _parent_coro_addr;
        p->sync_point = _sync_point;
        p->pp_action_sync_event = &_p_action_sync_event;

        t.start(p);
        t.uninit();

        return *this;
    }

    ref<RetTy> result_ref()
    {
        return _r_rst;
    }

private:
    void set_action_sync_event(_TaskNs::action_sync_event_wrapper* action_sync_event)
    {
        _p_action_sync_event = action_sync_event;
    }

    static void execute_entrypoint(void* p)
    {
        callback_desc* p_desc = pointer_convert(p, 0, callback_desc*);

        // store desc content
        auto task_coro = p_desc->coro;
        auto parent_coro_addr = p_desc->parent_coro_addr;
        auto sync_point = p_desc->sync_point;
        auto pp_action_sync_event = p_desc->pp_action_sync_event;

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

        if (*pp_action_sync_event)
        {
            // action call task
            // action _Await_ done
            // notify action thread
            (*pp_action_sync_event)->set();
        }

        while (my_order == 2)
        {
            yield();
            my_order = l_sync_point.get();
        }

        assert(my_order == 3);
        // resume Caller's execute body
        auto parent_coro = CoroCtxTy::from_address(*parent_coro_addr.raw_ptr());
        parent_coro.resume();
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
    _TaskNs::action_sync_event_wrapper*
                       _p_action_sync_event;
};
