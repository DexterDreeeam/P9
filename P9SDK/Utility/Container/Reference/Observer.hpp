#pragma once

class observer_base;

template<typename Ty>
class observer;

class observer_base : RefNs::context
{
    template<typename All_Ty> friend class observer;

public:
    observer_base() :
        context(nullptr, nullptr, 0, nullptr)
    {}

protected:
    observer_base(void* ptr, volatile RefNs::counter* cnt, s64 data_size, RefNs::deconstructor* dc) :
        context(ptr, cnt, data_size, dc)
    {}

    observer_base(const observer_base& rhs) :
        context(
            rhs._ptr,
            rhs._counter,
            rhs._data_size,
            rhs._deconstructor)
    {
        if (valid())
        {
            atom_increment(_counter->_ref_cnt);
        }
    }

    observer_base(observer_base&& rhs) :
        context(
            rhs._ptr,
            rhs._counter,
            rhs._data_size,
            rhs._deconstructor)
    {
        rhs._ptr = nullptr;
        rhs._counter = nullptr;
        rhs._data_size = 0;
        rhs._deconstructor = nullptr;
    }

public:
    ~observer_base()
    {
        clear();
    }

public:
    observer_base& operator =(const observer_base& rhs)
    {
        clear();

        _ptr = rhs._ptr;
        _counter = rhs._counter;
        _data_size = rhs._data_size;
        _deconstructor = rhs._deconstructor;

        if (valid())
        {
            atom_increment(_counter->_ref_cnt);
        }
        return *this;
    }

    observer_base& operator =(observer_base&& rhs) noexcept
    {
        clear();

        _ptr = rhs._ptr;
        _counter = rhs._counter;
        _data_size = rhs._data_size;
        _deconstructor = rhs._deconstructor;

        rhs._ptr = nullptr;
        rhs._counter = nullptr;
        rhs._data_size = 0;
        rhs._deconstructor = nullptr;

        return *this;
    }

public:
    operator boole() const
    {
        boole b = valid();
        return b;
    }

    operator bool() const
    {
        return valid();
    }

    boole valid() const
    {
        return _ptr;
    }

    boole invalid() const
    {
        return !_ptr;
    }

    template<typename Ty2>
    observer<Ty2> ref_of()
    {
        if (invalid())
        {
            return observer<Ty2>();
        }
        return observer<Ty2>(*this);
    }

    void clear()
    {
        if (valid() && atom_decrement(_counter->_ref_cnt) == 0)
        {
            _deconstructor(_ptr);
            memory_free(_ptr);
            _ptr = nullptr;

            if (atom_decrement(_counter->_observer_cnt) == 0)
            {
                delete _counter;
            }
            _counter = nullptr;

            _data_size = 0;
            _deconstructor = nullptr;
        }
    }

protected:
    template<typename Ty>
    Ty* pointer_of()
    {
        return reinterpret_cast<Ty*>(_ptr);
    }

    template<typename Ty, typename ...Args>
    static observer_base new_instance(RefNs::deconstructor* dc, Args... args)
    {
        s64 data_size = sizeof(Ty);
        void* mem_data = memory_alloc(data_size);
        new (mem_data) Ty(args...);
        volatile RefNs::counter* mem_cnt = new volatile RefNs::counter(1, 1);
        return observer_base(mem_data, mem_cnt, data_size, dc);
    }

    template<typename Ty>
    static observer_base new_instance(Ty* raw_ptr, RefNs::deconstructor* dc)
    {
        s64 data_size = sizeof(Ty);
        volatile RefNs::counter* mem_cnt = new volatile RefNs::counter(1, 1);
        return observer_base(raw_ptr, mem_cnt, data_size, dc);
    }
};

template<typename Ty>
class observer : public observer_base
{
    friend class observer_base;
    template<typename All_Ty> friend class ref;
    using Self_Ty = typename observer<Ty>;

public:
    observer() :
        observer_base()
    #if DEBUG_LEVEL >= DEBUG_LEVEL_CALIBRATION_ALL
        ,_debug_ptr(pointer_convert(&_ptr, 0, Ty**))
        ,_debug_value(*_debug_ptr)
    #endif
    {}

    observer(const Self_Ty& rhs) :
        observer_base(rhs)
    #if DEBUG_LEVEL >= DEBUG_LEVEL_CALIBRATION_ALL
        ,_debug_ptr(pointer_convert(&_ptr, 0, Ty**))
        ,_debug_value(*_debug_ptr)
    #endif
    {
    }

    observer(Self_Ty&& rhs) :
        observer_base(right_value_type(rhs))
    #if DEBUG_LEVEL >= DEBUG_LEVEL_CALIBRATION_ALL
        ,_debug_ptr(pointer_convert(&_ptr, 0, Ty**))
        ,_debug_value(*_debug_ptr)
    #endif
    {
    }

    template<typename Ty2>
    observer(const observer<Ty2>& rhs) :
        observer_base(rhs)
    #if DEBUG_LEVEL >= DEBUG_LEVEL_CALIBRATION_ALL
        ,_debug_ptr(pointer_convert(&_ptr, 0, Ty**))
        ,_debug_value(*_debug_ptr)
    #endif
    {
    #if DEBUG_LEVEL >= DEBUG_LEVEL_CALIBRATION_ALL
        Ty* check_type_ptr = pointer_convert(rhs._ptr, 0, Ty2*);
    #endif
    }

private:
    observer(const observer_base& rb) :
        observer_base(rb)
    #if DEBUG_LEVEL >= DEBUG_LEVEL_CALIBRATION_ALL
        ,_debug_ptr(pointer_convert(&_ptr, 0, Ty**))
        ,_debug_value(*_debug_ptr)
    #endif
    {
    }

public:
    ~observer() = default;

    Self_Ty& operator =(const Self_Ty& rhs)
    {
        this->observer_base::operator =(rhs);
        return *this;
    }

    template<typename Ty2>
    Self_Ty& operator =(const observer<Ty2>& rhs)
    {
    #if DEBUG_LEVEL >= DEBUG_LEVEL_CALIBRATION_ALL
        Ty* check_type_ptr = pointer_convert(rhs._ptr, 0, Ty2*);
    #endif
        this->observer_base::operator =(rhs);
        return *this;
    }

    Ty* operator ->()
    {
        return pointer_of<Ty>();
    }

    Ty& operator *()
    {
        return *pointer_of<Ty>();
    }

public:
    template<typename ...Args>
    static Self_Ty new_instance(Args ...args)
    {
        Self_Ty rst;
        rst.observer_base::operator =(
            observer_base::new_instance<Ty, Args...>(&Self_Ty::deconstruct, args...));
        return rst;
    }

    static Self_Ty new_instance(Ty* raw_ptr)
    {
        Self_Ty rst;
        rst.observer_base::operator =(
            observer_base::new_instance<Ty>(raw_ptr, &Self_Ty::deconstruct));
        return rst;
    }

    static void deconstruct(void* p)
    {
        pointer_convert(p, 0, Ty*)->~Ty();
    }

#if DEBUG_LEVEL >= DEBUG_LEVEL_CALIBRATION_ALL

private:
    const Ty* const* _debug_ptr;
    const Ty* const& _debug_value;

#endif
};
