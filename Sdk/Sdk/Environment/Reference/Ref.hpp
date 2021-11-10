#pragma once

class ref_base;

template<typename Ty>
class ref;

class ref_base : public RefNs::context
{
    template<typename All_Ty> friend class ref;

public:
    ref_base() :
        context(nullptr, nullptr, 0, nullptr)
    {}

protected:
    ref_base(void* ptr, RefNs::counter* cnt, s64 data_size, RefNs::deconstructor* dc) :
        context(ptr, cnt, data_size, dc)
    {}

    ref_base(const ref_base& rhs) :
        context(
            rhs._ptr,
            rhs._counter,
            rhs._data_size,
            rhs._deconstructor)
    {
        if (valid())
        {
            ++_counter->_ref_cnt;
        }
    }

    ref_base(ref_base&& rhs) :
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

    // try to convert observer to ref
    ref_base(const RefNs::observer_base& ob) :
        context(nullptr, nullptr, 0, nullptr)
    {
        if (ob.valid())
        {
            while (ob._counter->_observer_check_busy.exchange(1) != 0)
            {
                yield();
            }
            if (++ob._counter->_ref_cnt != 1)
            {
                // greater than 1
                // generate new ref of typename::Ty
                ob._counter->_observer_check_busy = 0;
                _ptr = ob._ptr;
                _counter = ob._counter;
                _data_size = ob._data_size;
                _deconstructor = ob._deconstructor;
            }
            else
            {
                // resource has already been released
                // do unsafe decrement to reset _ref_cnt
                --ob._counter->_ref_cnt;
                ob._counter->_observer_check_busy = 0;
            }
        }
    }

public:
    ~ref_base()
    {
        clear();
    }

public:
    ref_base& operator =(const ref_base& rhs)
    {
        clear();

        _ptr = rhs._ptr;
        _counter = rhs._counter;
        _data_size = rhs._data_size;
        _deconstructor = rhs._deconstructor;

        if (valid())
        {
            ++_counter->_ref_cnt;
        }
        return *this;
    }

    ref_base& operator =(ref_base&& rhs) noexcept
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

    ref_base& operator =(const RefNs::observer_base& ob)
    {
        clear();

        if (ob.valid())
        {
            while (ob._counter->_observer_check_busy.exchange(1) != 0)
            {
                yield();
            }
            if (++ob._counter->_ref_cnt != 1)
            {
                // greater than 1
                // generate new ref of typename::Ty
                ob._counter->_observer_check_busy = 0;
                _ptr = ob._ptr;
                _counter = ob._counter;
                _data_size = ob._data_size;
                _deconstructor = ob._deconstructor;
            }
            else
            {
                // resource has already been released
                // do unsafe decrement to reset _ref_cnt
                --ob._counter->_ref_cnt;
                ob._counter->_observer_check_busy = 0;
            }
        }
        return *this;
    }

    bool operator ==(const ref_base& rhs) const noexcept
    {
        return _ptr == rhs._ptr;
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
    ref<Ty2> ref_of()
    {
        if (invalid())
        {
            return ref<Ty2>();
        }
        return ref<Ty2>(*this);
    }

    void clear()
    {
        if (valid() && --_counter->_ref_cnt == 0)
        {
            _deconstructor(_ptr);
            memory::free(_ptr);
            _ptr = nullptr;

            if (--_counter->_observer_cnt == 0)
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

protected:
    template<typename Ty, typename ...Args>
    static ref_base new_instance(RefNs::deconstructor* dc, Args... args)
    {
        s64 data_size = sizeof(Ty);
        void* mem_data = memory::alloc<void>(data_size);
        new (mem_data) Ty(args...);
        RefNs::counter* mem_cnt = new RefNs::counter(1, 1);
        return ref_base(mem_data, mem_cnt, data_size, dc);
    }

    template<typename Ty>
    static ref_base new_instance(Ty* raw_ptr, RefNs::deconstructor* dc)
    {
        s64 data_size = sizeof(Ty);
        RefNs::counter* mem_cnt = new RefNs::counter(1, 1);
        return ref_base(raw_ptr, mem_cnt, data_size, dc);
    }

protected:
    RefNs::observer_base observer()
    {
        if (valid())
        {
            RefNs::observer_base ob(_ptr, _counter, _data_size, _deconstructor);
            ++_counter->_observer_cnt;
            return ob;
        }
        else
        {
            return RefNs::observer_base();
        }
    }
};

template<typename Ty>
class ref : public ref_base
{
    friend class ref_base;
    template<typename All_Ty> friend class ref;

    using Self_Ty = typename::ref<Ty>;
    using Observer_Ty = typename::observer<Ty>;

public:
    ref() :
        ref_base()
    #if DEBUG_LEVEL >= DEBUG_LEVEL_CALIBRATION_ALL
        ,_debug_pptr(pointer_convert(&_ptr, 0, Ty**))
        ,_debug_ptr(*_debug_pptr)
    #endif
    {}

    ref(const Self_Ty& rhs) :
        ref_base(rhs)
    #if DEBUG_LEVEL >= DEBUG_LEVEL_CALIBRATION_ALL
        ,_debug_pptr(pointer_convert(&_ptr, 0, Ty**))
        ,_debug_ptr(*_debug_pptr)
    #endif
    {
    }

    ref(Self_Ty&& rhs) :
        ref_base(right_value_type(rhs))
    #if DEBUG_LEVEL >= DEBUG_LEVEL_CALIBRATION_ALL
        ,_debug_pptr(pointer_convert(&_ptr, 0, Ty**))
        ,_debug_ptr(*_debug_pptr)
    #endif
    {
    }

    template<typename Ty2>
    ref(const ref<Ty2>& rhs) :
        ref_base(rhs)
    #if DEBUG_LEVEL >= DEBUG_LEVEL_CALIBRATION_ALL
        ,_debug_pptr(pointer_convert(&_ptr, 0, Ty**))
        ,_debug_ptr(*_debug_pptr)
    #endif
    {
    #if DEBUG_LEVEL >= DEBUG_LEVEL_CALIBRATION_ALL
        Ty* check_type_ptr = pointer_convert(rhs._ptr, 0, Ty2*);
    #endif
    }

    ref(const Observer_Ty& ob) :
        ref_base(ob)
    #if DEBUG_LEVEL >= DEBUG_LEVEL_CALIBRATION_ALL
        ,_debug_pptr(pointer_convert(&_ptr, 0, Ty**))
        ,_debug_ptr(*_debug_pptr)
    #endif
    {
    }

private:
    ref(const ref_base& rb) :
        ref_base(rb)
    #if DEBUG_LEVEL >= DEBUG_LEVEL_CALIBRATION_ALL
        ,_debug_pptr(pointer_convert(&_ptr, 0, Ty**))
        ,_debug_ptr(*_debug_pptr)
    #endif
    {
    }

public:
    ~ref() = default;

    Self_Ty& operator =(const Self_Ty& rhs)
    {
        this->ref_base::operator =(rhs);
        return *this;
    }

    Self_Ty& operator =(Self_Ty&& rhs)
    {
        this->ref_base::operator =(right_value_type(rhs));
        return *this;
    }

    template<typename Ty2>
    Self_Ty& operator =(const ref<Ty2>& rhs)
    {
    #if DEBUG_LEVEL >= DEBUG_LEVEL_CALIBRATION_ALL
        Ty* check_type_ptr = pointer_convert(rhs._ptr, 0, Ty2*);
    #endif
        this->ref_base::operator =(rhs);
        return *this;
    }

    Self_Ty& operator =(const Observer_Ty& ob)
    {
    #if DEBUG_LEVEL >= DEBUG_LEVEL_CALIBRATION_ALL
        Ty* check_type_ptr1 = (Observer_Ty::Value_Ptr_Ty)(nullptr);
        typename Observer_Ty::Value_Ptr_Ty check_type_ptr2 = (Ty*)(nullptr);
    #endif
        this->ref_base::operator =(ob);
        return *this;
    }

    bool operator ==(const ref<Ty>& rhs) const noexcept
    {
        return this->ref_base::operator ==(rhs);
    }

    bool operator !=(const ref<Ty>& rhs) const noexcept
    {
        return !this->ref_base::operator ==(rhs);
    }

    Ty* operator ->()
    {
        assert_info(valid(), "Try to access invalid reference pointer.");
        return pointer_of<Ty>();
    }

    Ty& operator *()
    {
        assert_info(valid(), "Try to access invalid reference pointer.");
        return *pointer_of<Ty>();
    }

public:
    template<typename ...Args>
    static Self_Ty new_instance(Args ...args)
    {
        Self_Ty rst;
        rst.ref_base::operator =(
            ref_base::new_instance<Ty, Args...>(&Self_Ty::deconstruct, args...));
        return rst;
    }

    static Self_Ty new_instance(Ty* raw_ptr)
    {
        Self_Ty rst;
        rst.ref_base::operator =(
            ref_base::new_instance<Ty>(raw_ptr, &Self_Ty::deconstruct));
        return rst;
    }

    static void deconstruct(void* p)
    {
        pointer_convert(p, 0, Ty*)->~Ty();
    }

public:
    typename::observer<Ty> observer()
    {
        return typename::observer<Ty>(ref_base::observer());
    }

    operator typename::observer<Ty>()
    {
        return observer();
    }

#if DEBUG_LEVEL >= DEBUG_LEVEL_CALIBRATION_ALL

private:
    const Ty* const* _debug_pptr;
    const Ty* const& _debug_ptr;

#endif
};
