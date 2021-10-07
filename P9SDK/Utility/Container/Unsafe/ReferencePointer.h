#pragma once

template<typename Ty>
class ref
{
    using Self_Ty = typename ref<Ty>;

public:
    ref() :
        _ptr(nullptr),
        _cnt(0)
    {}

    ref(const Self_Ty& rhs) :
        _ptr(rhs._ptr),
        _cnt(rhs._cnt)
    {
        atom_increment(*_cnt);
    }

    ~ref()
    {
        if (atom_decrement(*_cnt) == 0)
        {
            delete _ptr;
        }
    }

    Self_Ty& operator =(const Self_Ty& rhs)
    {
        _ptr = rhs._ptr;
        _cnt = rhs._cnt;
        atom_increment(*_cnt);
        return *this;
    }

    Ty* operator ->()
    {
        return _ptr;
    }

    Ty& operator *()
    {
        return *_ptr;
    }

public:
    template<typename ...Args>
    static Self_Ty new_instance(Args ...args)
    {
        Self_Ty r;
        r._cnt = new volatile s64(1);
        r._ptr = new Ty(args...);
        return r;
    }

private:
    Ty*           _ptr;
    volatile s64* _cnt;
};
