#pragma once

typedef struct boole
{
    enum class boole_value_type : s64
    {
        Boole_False = 0,
        Boole_True = 1,
    } v;

    static const boole True;
    static const boole False;

    boole() :
        v(False.v)
    {}

    boole(bool b) :
        v(b ? True.v : False.v)
    {}

    boole(const boole& rhs) :
        v(rhs.v)
    {}

    boole(const boole_value_type& value) :
        v(value)
    {}

    ~boole() = default;

    boole& operator =(const boole& rhs)
    {
        v = rhs.v;
        return *this;
    }

    volatile boole& operator =(const boole& rhs) volatile
    {
        v = rhs.v;
        return *this;
    }

    bool operator ==(const boole& rhs) const
    {
        return v == rhs.v;
    }

    bool operator != (const boole& rhs) const
    {
        return v != rhs.v;
    }

    operator bool() const
    {
        return v == boole::True.v;
    }

} boole;

_SELECTANY_ const boole boole::True  = boole(boole::boole_value_type::Boole_True);
_SELECTANY_ const boole boole::False = boole(boole::boole_value_type::Boole_False);

