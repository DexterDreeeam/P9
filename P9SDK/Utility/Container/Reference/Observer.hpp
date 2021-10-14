#pragma once

class ref_base;

template<typename Ty>
class ref;

namespace RefNs
{

class observer_base : public RefNs::context
{
    template<typename All_Ty> friend class observer;
    template<typename All_Ty> friend class ref;
    friend class ref_base;

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
            atom_increment(_counter->_observer_cnt);
        }
    }

    observer_base(observer_base&& rhs) noexcept :
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
            atom_increment(_counter->_observer_cnt);
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
    boole valid() const
    {
        return _counter;
    }

    boole invalid() const
    {
        return !_counter;
    }

    void clear()
    {
        if (valid() && atom_decrement(_counter->_observer_cnt) == 0)
        {
            delete _counter;
            _ptr = nullptr;
            _counter = nullptr;
            _data_size = 0;
            _deconstructor = nullptr;
        }
    }
};

}

template<typename Ty>
class observer : public RefNs::observer_base
{
    // friend class observer_base;
    template<typename All_Ty> friend class ref;

    using Self_Ty = typename observer<Ty>;
    using Value_Ty = typename Ty;
    using Value_Ptr_Ty = typename Ty*;

public:
    observer() :
        observer_base()
    #if DEBUG_LEVEL >= DEBUG_LEVEL_CALIBRATION_ALL
        ,_debug_pptr(pointer_convert(&_ptr, 0, Ty**))
        ,_debug_ptr(*_debug_pptr)
    #endif
    {}

    observer(const Self_Ty& rhs) :
        observer_base(rhs)
    #if DEBUG_LEVEL >= DEBUG_LEVEL_CALIBRATION_ALL
        ,_debug_pptr(pointer_convert(&_ptr, 0, Ty**))
        ,_debug_ptr(*_debug_pptr)
    #endif
    {
    }

    observer(Self_Ty&& rhs) :
        observer_base(right_value_type(rhs))
    #if DEBUG_LEVEL >= DEBUG_LEVEL_CALIBRATION_ALL
        ,_debug_pptr(pointer_convert(&_ptr, 0, Ty**))
        ,_debug_ptr(*_debug_pptr)
    #endif
    {
    }

private:
    observer(const observer_base& rb) :
        observer_base(rb)
    #if DEBUG_LEVEL >= DEBUG_LEVEL_CALIBRATION_ALL
        ,_debug_pptr(pointer_convert(&_ptr, 0, Ty**))
        ,_debug_ptr(*_debug_pptr)
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

#if DEBUG_LEVEL >= DEBUG_LEVEL_CALIBRATION_ALL

private:
    const Ty* const* _debug_pptr;
    const Ty* const& _debug_ptr;

#endif
};
