#pragma once

template<typename Ty>
class ref
{
    template<typename All_Ty> friend class ref;

    using Self_Ty = typename ref<Ty>;

public:
    ref() :
        _ptr(nullptr),
        _cnt(nullptr)
    {}

    ref(const Self_Ty& rhs) :
        _ptr(rhs._ptr),
        _cnt(rhs._cnt)
    {
        if (_ptr && _cnt)
        {
            atom_increment(*_cnt);
        }
    }

    template<typename Ty2>
    ref(const ref<Ty2>& rhs) :
        _ptr(rhs._ptr),
        _cnt(rhs._cnt)
    {
        if (_ptr && _cnt)
        {
            atom_increment(*_cnt);
        }
    }

    ~ref()
    {
        if (invalid())
        {
            return;
        }
        if (atom_decrement(*_cnt) == 0)
        {
            delete _ptr;
            delete _cnt;
        }
    }

    Self_Ty& operator =(const Self_Ty& rhs)
    {
        _ptr = rhs._ptr;
        _cnt = rhs._cnt;
        if (_ptr && _cnt)
        {
            atom_increment(*_cnt);
        }
        return *this;
    }

    template<typename Ty2>
    Self_Ty& operator =(const ref<Ty2>& rhs)
    {
        _ptr = rhs._ptr;
        _cnt = rhs._cnt;
        if (_ptr && _cnt)
        {
            atom_increment(*_cnt);
        }
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

    operator boole() const
    {
        boole b = valid();
        return b;
    }

    operator bool() const
    {
        return valid();
    }

    template<typename Ty2>
    ref<Ty2> cast()
    {
        ref<Ty2> r;
        if (invalid())
        {
            return r;
        }
        atom_increment(*_cnt);
        r._ptr = (Ty2*)_ptr;
        r._cnt = _cnt;
        return r;
    }

    boole valid() const
    {
        return _ptr && _cnt;
    }

    boole invalid() const
    {
        return !_ptr || !_cnt;
    }

    void clear()
    {
        if (valid() && atom_decrement(*_cnt) == 0)
        {
            delete _ptr;
            delete _cnt;
            _ptr = nullptr;
            _cnt = nullptr;
        }
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
