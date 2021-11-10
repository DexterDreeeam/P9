#pragma once

namespace RefNs
{

_INLINE_ observer_base::observer_base() :
    context(nullptr, nullptr, 0, nullptr)
{
}

_INLINE_ observer_base::observer_base(void* ptr, RefNs::counter* cnt, s64 data_size, RefNs::deconstructor* dc) :
    context(ptr, cnt, data_size, dc)
{
}

_INLINE_ observer_base::observer_base(const observer_base& rhs) :
    context(
        rhs._ptr,
        rhs._counter,
        rhs._data_size,
        rhs._deconstructor)
{
    if (valid())
    {
        ++_counter->_observer_cnt;
    }
}

_INLINE_ observer_base::observer_base(observer_base&& rhs) noexcept :
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

_INLINE_ observer_base::~observer_base()
{
    clear();
}

_INLINE_ observer_base& observer_base::operator =(const observer_base& rhs)
{
    clear();

    _ptr = rhs._ptr;
    _counter = rhs._counter;
    _data_size = rhs._data_size;
    _deconstructor = rhs._deconstructor;

    if (valid())
    {
        ++_counter->_observer_cnt;
    }
    return *this;
}

_INLINE_ observer_base& observer_base::operator =(observer_base&& rhs) noexcept
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

_INLINE_ boole observer_base::valid() const
{
    return _counter;
}

_INLINE_ boole observer_base::invalid() const
{
    return !_counter;
}

_INLINE_ void observer_base::clear()
{
    if (valid() && --_counter->_observer_cnt == 0)
    {
        delete _counter;
        _ptr = nullptr;
        _counter = nullptr;
        _data_size = 0;
        _deconstructor = nullptr;
    }
}

}

template<typename Ty>
_INLINE_ observer<Ty>::observer() :
    observer_base()
#if DEBUG_LEVEL >= DEBUG_LEVEL_CALIBRATION_ALL
    , _debug_pptr(pointer_convert(&_ptr, 0, Ty**))
    , _debug_ptr(*_debug_pptr)
#endif
{}

template<typename Ty>
_INLINE_ observer<Ty>::observer(const Self_Ty& rhs) :
    observer_base(rhs)
#if DEBUG_LEVEL >= DEBUG_LEVEL_CALIBRATION_ALL
    , _debug_pptr(pointer_convert(&_ptr, 0, Ty**))
    , _debug_ptr(*_debug_pptr)
#endif
{
}

template<typename Ty>
_INLINE_ observer<Ty>::observer(Self_Ty&& rhs) :
    observer_base(right_value_type(rhs))
#if DEBUG_LEVEL >= DEBUG_LEVEL_CALIBRATION_ALL
    , _debug_pptr(pointer_convert(&_ptr, 0, Ty**))
    , _debug_ptr(*_debug_pptr)
#endif
{
}

template<typename Ty>
_INLINE_ observer<Ty>::observer(const observer_base& rb) :
    observer_base(rb)
#if DEBUG_LEVEL >= DEBUG_LEVEL_CALIBRATION_ALL
    , _debug_pptr(pointer_convert(&_ptr, 0, Ty**))
    , _debug_ptr(*_debug_pptr)
#endif
{
}

template<typename Ty>
_INLINE_ observer<Ty>::~observer()
{
}

template<typename Ty>
_INLINE_ observer<Ty>::Self_Ty& observer<Ty>::operator =(const Self_Ty& rhs)
{
    this->observer_base::operator =(rhs);
    return *this;
}

template<typename Ty>
template<typename Ty2>
_INLINE_ observer<Ty>& observer<Ty>::operator =(const observer<Ty2>& rhs)
{
#if DEBUG_LEVEL >= DEBUG_LEVEL_CALIBRATION_ALL
    Ty* check_type_ptr = pointer_convert(rhs._ptr, 0, Ty2*);
#endif
    this->observer_base::operator =(rhs);
    return *this;
}

template<typename Ty>
_INLINE_ typename::ref<Ty> observer<Ty>::try_ref()
{
    return typename::ref<Ty>(*this);
}
