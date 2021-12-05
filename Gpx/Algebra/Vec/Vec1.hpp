#pragma once

#include "Vec.hpp"

namespace gpx
{

class vec1 : public vec<1, f32>
{
    using BaseTy = vec<1, f32>;

public:
    vec1(f32 x = 0) :
        vec(x)
    {
    }

    vec1(const vec1& rhs) :
        vec(rhs)
    {
    }

    vec1(const BaseTy& rhs) :
        vec(rhs)
    {
    }

    ~vec1() = default;

public:
    f32& x()
    {
        return vec::at(0);
    }

    f32 x() const
    {
        return vec::at(0);
    }
};

}
