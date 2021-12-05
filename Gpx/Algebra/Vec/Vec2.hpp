#pragma once

#include "Vec.hpp"

namespace gpx
{

class vec2 : public vec<2, f32>
{
    using BaseTy = vec<2, f32>;

public:
    vec2(f32 x = 0, f32 y = 0) :
        vec(x, y)
    {
    }

    vec2(const vec2& rhs) :
        vec(rhs)
    {
    }

    vec2(const BaseTy& rhs) :
        vec(rhs)
    {
    }

    ~vec2() = default;

public:
    f32& x()
    {
        return vec::at(0);
    }

    f32 x() const
    {
        return vec::at(0);
    }

    f32& y()
    {
        return vec::at(1);
    }

    f32 y() const
    {
        return vec::at(1);
    }
};

}
