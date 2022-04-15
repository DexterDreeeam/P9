#pragma once

#include "Vec.hpp"

namespace gpx
{

class vec3 : public vec<3, f32>
{
    using BaseTy = vec<3, f32>;

public:
    vec3(f32 x = 0, f32 y = 0, f32 z = 0) :
        vec(x, y, z)
    {
    }

    vec3(const vec3& rhs) :
        vec(rhs)
    {
    }

    vec3(const BaseTy& rhs) :
        vec(rhs)
    {
    }

    ~vec3() = default;

public:
    void self_cross(const vec3& rhs)
    {
        f32 new_x = y() * rhs.z() - z() * rhs.y();
        f32 new_y = z() * rhs.x() - x() * rhs.z();
        f32 new_z = x() * rhs.y() - y() * rhs.x();
        x() = new_x;
        y() = new_y;
        z() = new_z;
    }

    vec3 cross(const vec3& rhs) const
    {
        vec3 v(*this);
        v.self_cross(rhs);
        return v;
    }

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

    f32& z()
    {
        return vec::at(2);
    }

    f32 z() const
    {
        return vec::at(2);
    }
};

}
