#pragma once

class action_base
{
public:
    struct ActionPromiseBase
    {
        constexpr std::suspend_never initial_suspend() noexcept
        {
            return std::suspend_never();
        }

        constexpr std::suspend_never final_suspend() noexcept
        {
            return std::suspend_never();
        }

        template<typename TaskTy>
        void await_transform_internal(TaskTy& task_awaiter);

        _TaskNs::suspend_never<void> await_transform(task<void>& awaiter);

        _TaskNs::suspend_never<void> await_transform(task<void>&& awaiter);

        template<typename SubTaskRetTy>
        _TaskNs::suspend_never<SubTaskRetTy> await_transform(task<SubTaskRetTy>& awaiter);

        template<typename SubTaskRetTy>
        _TaskNs::suspend_never<SubTaskRetTy> await_transform(task<SubTaskRetTy>&& awaiter);

        template<typename SubActionRetTy>
        constexpr std::suspend_never await_transform(const action<SubActionRetTy>& awaiter)
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
    bool await_ready()
    {
        return true;
    }

    bool await_suspend(CoroCtxTy coro)
    {
        return true;
    }
};

template<>
class action<void> : public action_base
{
public:
    using ActionTy = action<void>;

    friend class task_base;
    template<typename AllTy> friend class task;

    friend class action_base;
    template<typename AllTy> friend class action;

    struct ActionPromiseTy : public action_base::ActionPromiseBase
    {
        ActionTy get_return_object()
        {
            auto coro = CoroTy<ActionPromiseTy>::from_promise(*this);
            return ActionTy(coro);
        }

        void return_void()
        {
        }
    };

    using promise_type = ActionPromiseTy;

public:
    action(CoroTy<ActionPromiseTy> coro) :
        _coro(coro)
    {
    }

    action(const action& rhs) :
        _coro(rhs._coro)
    {
    }

    action(action&& rhs) :
        _coro(rhs._coro)
    {
    }

    action& operator =(const action&) = delete;

    action& operator =(action&& rhs)
    {
        _coro = rhs._coro;
        return *this;
    }

    ~action() = default;

public:
    void await_resume()
    {
    }

private:
    CoroTy<ActionPromiseTy>  _coro;
};

template<typename RetTy>
class action : public action_base
{
public:
    using ActionTy = action<RetTy>;

    friend class task_base;
    template<typename AllTy> friend class task;

    friend class action_base;
    template<typename AllTy> friend class action;

    struct ActionPromiseTy : public action_base::ActionPromiseBase
    {
        ActionTy get_return_object()
        {
            _r_rst = ref<RetTy>::new_instance();
            auto coro = CoroTy<ActionPromiseTy>::from_promise(*this);
            return ActionTy(coro, _r_rst);
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

    using promise_type = ActionPromiseTy;

public:
    action(CoroTy<ActionPromiseTy> coro, ref<RetTy> r_rst) :
        _coro(coro),
        _r_rst(r_rst)
    {
    }

    action(const action& rhs) :
        _coro(rhs._coro),
        _r_rst(rhs._r_rst)
    {
    }

    action(action&& rhs) :
        _coro(rhs._coro),
        _r_rst(rhs._r_rst)
    {
    }

    action& operator =(const action&) = delete;

    action& operator =(action&& rhs)
    {
        _coro = rhs._coro;
        _r_rst = rhs._r_rst;
        return *this;
    }

    ~action() = default;

public:
    auto await_resume()
    {
        return RetTy(*_r_rst.raw_ptr());
    }

private:
    CoroTy<ActionPromiseTy>  _coro;
    ref<RetTy>               _r_rst;
};
