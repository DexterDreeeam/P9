#pragma once

template<typename RetTy = void>
class task_set;

template<>
class task_set<void>
{
public:
    using TaskSetTy = task_set<void>;

public:
    struct PromiseTy
    {
        TaskSetTy get_return_object()
        {
            _r_done = ref<event>::new_instance();
            return TaskSetTy(_r_done);
        }

        auto initial_suspend() noexcept
        {
            return std::suspend_never();
        }

        auto final_suspend() noexcept
        {
            return std::suspend_never();
        }

        void return_void()
        {
            assert(_r_done.has_value() && _r_done->is_init());
            _r_done->set();
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

        ref<event>  _r_done;
    };

public:
    struct PromiseTy;
    friend struct PromiseTy;
    using promise_type = PromiseTy;

public:
    task_set(ref<event> r_done) :
        _r_done(r_done)
    {
        assert(_r_done.has_value());
        _r_done->init();
    }

    task_set(const task_set&) = delete;

    task_set(task_set&& rhs) :
        _r_done(rhs._r_done)
    {
        rhs._r_done.clear();
    }

    task_set& operator =(const task_set&) = delete;

    task_set& operator =(task_set&& rhs)
    {
        _r_done = rhs._r_done;
        rhs._r_done.clear();
        return *this;
    }

    ~task_set()
    {
        if (_r_done.has_value() && _r_done->is_init())
        {
            _r_done->uninit();
        }
    }

public:
    void await_complete()
    {
        assert(_r_done.has_value() && _r_done->is_init());
        _r_done->wait();
    }

private:
    ref<event>     _r_done;
};

template<typename RetTy>
class task_set
{
public:
    using TaskSetTy = task_set<RetTy>;

public:
    struct PromiseTy
    {
        TaskSetTy get_return_object()
        {
            _r_rst = ref<RetTy>::new_instance();
            _r_done = ref<event>::new_instance();
            return TaskSetTy(_r_rst, _r_done);
        }

        auto initial_suspend() noexcept
        {
            return std::suspend_never();
        }

        auto final_suspend() noexcept
        {
            return std::suspend_never();
        }

        void return_value(const RetTy& rst)
        {
            assert(_r_rst.has_value());
            *_r_rst.raw_ptr() = rst;

            assert(_r_done.has_value() && _r_done->is_init());
            _r_done->set();
        }

        void return_value(RetTy&& rst)
        {
            assert(_r_rst.has_value());
            *_r_rst.raw_ptr() = (RetTy&&)rst;

            assert(_r_done.has_value() && _r_done->is_init());
            _r_done->set();
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

        ref<RetTy>  _r_rst;
        ref<event>  _r_done;
    };

public:
    struct PromiseTy;
    friend struct PromiseTy;
    using promise_type = PromiseTy;

public:
    task_set(ref<RetTy> r_rst, ref<event> r_done) :
        _r_rst(r_rst),
        _r_done(r_done)
    {
        assert(_r_done.has_value());
        _r_done->init();
    }

    task_set(const task_set&) = delete;

    task_set(task_set&& rhs) :
        _r_rst(rhs._r_rst),
        _r_done(rhs._r_done)
    {
        rhs._r_done.clear();
    }

    task_set& operator =(const task_set&) = delete;

    task_set& operator =(task_set&& rhs)
    {
        _r_rst = rhs._r_rst;
        _r_done = rhs._r_done;
        rhs._r_done.clear();
        return *this;
    }

    ~task_set()
    {
        if (_r_done.has_value() && _r_done->is_init())
        {
            _r_done->uninit();
        }
    }

public:
    RetTy await_complete()
    {
        assert(_r_done.has_value() && _r_done->is_init());
        _r_done->wait();

        return RetTy(*_r_rst.raw_ptr());
    }

private:
    ref<RetTy>     _r_rst;
    ref<event>     _r_done;
};
