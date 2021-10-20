#pragma once

template<typename Fn_Ty>
class escape_function
{
public:
    escape_function(Fn_Ty fn) :
        _fn(fn)
    {}

    ~escape_function()
    {
        _fn();
    }

private:
    Fn_Ty _fn;
};
