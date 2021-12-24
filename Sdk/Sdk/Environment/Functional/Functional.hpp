#pragma once

template<typename ...Args>
class functional
{
    class caller_base
    {
    public:
        caller_base() = default;

        virtual ~caller_base() = default;

        virtual void execute(Args... args) = 0;

        virtual caller_base* clone() = 0;
    };

    template<typename Fn_Ty>
    class caller : public caller_base
    {
    public:
        caller() = delete;

        caller(const caller& rhs) = delete;

        caller(Fn_Ty fn) :
            caller_base(),
            _fn(fn)
        {}

        virtual ~caller() override = default;

        virtual void execute(Args... args) override
        {
            _fn(args...);
        }

        virtual caller_base* clone() override
        {
            return new caller(_fn);
        }

    public:
        Fn_Ty _fn;
    };

public:
    functional() :
        _caller(nullptr)
    {
    }

    template<typename Fn_Ty>
    functional(Fn_Ty fn) :
        _caller(new caller<Fn_Ty>(fn))
    {
    }

    functional(const functional& rhs) :
        _caller(nullptr)
    {
        if (rhs._caller)
        {
            _caller = rhs._caller->clone();
        }
    }

    template<typename Fn_Ty>
    functional& operator =(Fn_Ty fn)
    {
        if (_caller)
        {
            delete _caller;
        }
        _caller = new caller<Fn_Ty>(fn);
        return *this;
    }

    ~functional()
    {
        if (_caller)
        {
            delete _caller;
        }
    }

    void operator ()(Args... args)
    {
        _caller->execute(args...);
    }

private:
    caller_base* _caller;
};
