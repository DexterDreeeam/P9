#pragma once

template<typename RetTy = void>
class action;

template<>
class action<void>
{
public:
    using ActionTy = action<void>;

public:
    struct PromiseTy
    {
        ActionTy get_return_object()
        {
            auto coro = CoroTy<PromiseTy>::from_promise(*this);
            return ActionTy(coro);
        }

        auto initial_suspend()
        {
            return std::suspend_never();
        }

        auto final_suspend() noexcept
        {
            return std::suspend_never();
        }

        void return_void()
        {
            int test = 0;
        }

        _TaskNs::is_suspend<void> await_transform(task<void>& awaiter)
        {
            if (!awaiter.await_ready())
            {
                auto coro = CoroTy<PromiseTy>::from_promise(*this);
                awaiter.await_suspend(coro);
                return _TaskNs::is_suspend<void>(true);
            }
            else
            {
                return _TaskNs::is_suspend<void>(false);
            }
        }

        template<typename SubTaskRetTy>
        _TaskNs::is_suspend<SubTaskRetTy> await_transform(task<SubTaskRetTy>& awaiter)
        {
            if (!awaiter.await_ready())
            {
                auto coro = CoroTy<PromiseTy>::from_promise(*this);
                awaiter.await_suspend(coro);
                return _TaskNs::is_suspend<SubTaskRetTy>(true, awaiter.result_ref());
            }
            else
            {
                return _TaskNs::is_suspend<SubTaskRetTy>(false, awaiter.result_ref());
            }
        }

        template<typename SubActionRetTy>
        auto await_transform(const action<SubActionRetTy>& awaiter)
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
    struct PromiseTy;
    friend struct PromiseTy;
    using promise_type = PromiseTy;

public:
    action(CoroTy<PromiseTy> coro) :
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

    ~action()
    {
    }

public:
    bool await_ready()
    {
        return true;
    }

    bool await_suspend(CoroCtxTy coro)
    {
        return true;
    }

    void await_resume()
    {
        int test = 0;
    }

private:
    CoroTy<PromiseTy>  _coro;
};

template<typename RetTy>
class action
{
public:
    using ActionTy = action<RetTy>;

public:
    struct PromiseTy
    {
        ActionTy get_return_object()
        {
            _r_rst = ref<RetTy>::new_instance();
            auto coro = CoroTy<PromiseTy>::from_promise(*this);
            return ActionTy(coro, _r_rst);
        }

        auto initial_suspend()
        {
            return std::suspend_never();
        }

        auto final_suspend() noexcept
        {
            return std::suspend_never();
        }

        void return_value(const RetTy& rst)
        {
            *_r_rst.raw_ptr() = rst;
        }

        void return_value(RetTy&& rst)
        {
            *_r_rst.raw_ptr() = (RetTy&&)rst;
        }

        _TaskNs::is_suspend<void> await_transform(task<void>& awaiter)
        {
            if (!awaiter.await_ready())
            {
                auto coro = CoroTy<PromiseTy>::from_promise(*this);
                awaiter.await_suspend(coro);
                return _TaskNs::is_suspend<void>(true);
            }
            else
            {
                return _TaskNs::is_suspend<void>(false);
            }
        }

        template<typename SubTaskRetTy>
        _TaskNs::is_suspend<SubTaskRetTy> await_transform(task<SubTaskRetTy>& awaiter)
        {
            if (!awaiter.await_ready())
            {
                auto coro = CoroTy<PromiseTy>::from_promise(*this);
                awaiter.await_suspend(coro);
                return _TaskNs::is_suspend<SubTaskRetTy>(true, awaiter.result_ref());
            }
            else
            {
                return _TaskNs::is_suspend<SubTaskRetTy>(false, awaiter.result_ref());
            }
        }

        template<typename SubActionRetTy>
        auto await_transform(const action<SubActionRetTy>& awaiter)
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

        ref<RetTy> _r_rst;
    };

public:
    struct PromiseTy;
    friend struct PromiseTy;
    using promise_type = PromiseTy;

public:
    action(CoroTy<PromiseTy> coro, ref<RetTy> r_rst) :
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

    ~action()
    {
    }

public:
    bool await_ready()
    {
        return true;
    }

    bool await_suspend(CoroCtxTy coro)
    {
        return true;
    }

    RetTy await_resume()
    {
        return *_r_rst.raw_ptr();
    }

private:
    CoroTy<PromiseTy>  _coro;
    ref<RetTy>         _r_rst;
};
