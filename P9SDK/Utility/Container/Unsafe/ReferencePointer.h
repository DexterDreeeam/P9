#pragma once

class ref_base;

template<typename Ty>
class ref;

class ref_base
{
    template<typename All_Ty> friend class ref;

    typedef void ref_deconstructor(void*);

public:
    ref_base() :
        _ptr(nullptr),
        _cnt(nullptr),
        _data_size(0),
        _deconstructor(nullptr)
    {}

protected:
    ref_base(void* ptr, volatile s64* cnt, s64 data_size, ref_deconstructor* dc) :
        _ptr(ptr),
        _cnt(cnt),
        _data_size(data_size),
        _deconstructor(dc)
    {}

    ref_base(const ref_base& rhs) :
        _ptr(rhs._ptr),
        _cnt(rhs._cnt),
        _data_size(rhs._data_size),
        _deconstructor(rhs._deconstructor)
    {
        if (valid())
        {
            atom_increment(*_cnt);
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
        _cnt = rhs._cnt;
        _data_size = rhs._data_size;
        _deconstructor = rhs._deconstructor;
        if (valid())
        {
            atom_increment(*_cnt);
        }
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
        return _ptr && _cnt;
    }

    boole invalid() const
    {
        return !_ptr || !_cnt;
    }

    template<typename Ty2>
    ref<Ty2> ref_of()
    {
        if (invalid())
        {
            return ref<Ty2>();
        }
        ref<Ty2> r;
        r.ref_base::operator =(*this);
        return r;
    }

    void clear()
    {
        if (valid() && atom_decrement(*_cnt) == 0)
        {
            _deconstructor(_ptr);
            memory_free(_ptr);
            delete _cnt;
            _ptr = nullptr;
            _cnt = nullptr;
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
    static ref_base new_instance(ref_deconstructor* dc, Args... args)
    {
        s64 data_size = sizeof(Ty);
        void* mem_data = memory_alloc(data_size);
        new (mem_data) Ty(args...);
        volatile s64* mem_cnt = new volatile s64(1);
        return ref_base(mem_data, mem_cnt, data_size, dc);
    }

    template<typename Ty>
    static ref_base new_instance(Ty* raw_ptr, ref_deconstructor* dc)
    {
        s64 data_size = sizeof(Ty);
        volatile s64* mem_cnt = new volatile s64(1);
        return ref_base(raw_ptr, mem_cnt, data_size, dc);
    }

protected:
    void*              _ptr;
    volatile s64*      _cnt;
    s64                _data_size;
    ref_deconstructor* _deconstructor;
};

template<typename Ty>
class ref : public ref_base
{
    template<typename All_Ty> friend class ref;

    using Self_Ty = typename ref<Ty>;

public:
    ref() :
        ref_base()
    {}

    ref(const Self_Ty& rhs) :
        ref_base(rhs)
    {
    }

    template<typename Ty2>
    ref(const ref<Ty2>& rhs) :
        ref_base(rhs)
    {
        Ty* check_type_ptr = pointer_convert(rhs._ptr, 0, Ty2*);
    }

    ~ref() = default;

    Self_Ty& operator =(const Self_Ty& rhs)
    {
        this->ref_base::operator =(rhs);
        return *this;
    }

    template<typename Ty2>
    Self_Ty& operator =(const ref<Ty2>& rhs)
    {
        this->ref_base::operator =(rhs);
        Ty* check_type_ptr = pointer_convert(rhs._ptr, 0, Ty2*);
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
        auto base = ref_base::new_instance<Ty, Args...>(&Self_Ty::deconstruct, args...);
        Self_Ty rst;
        rst.ref_base::operator =(base);
        return rst;
    }

    static Self_Ty new_instance(Ty* raw_ptr)
    {
        auto base = ref_base::new_instance<Ty>(raw_ptr, &Self_Ty::deconstruct);
        Self_Ty rst;
        rst.ref_base::operator =(base);
        return rst;
    }

    static void deconstruct(void* p)
    {
        pointer_convert(p, 0, Ty*)->~Ty();
    }
};
