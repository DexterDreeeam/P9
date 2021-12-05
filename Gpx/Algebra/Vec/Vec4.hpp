#pragma once

#include "Vec.hpp"

namespace gpx
{

class vec4 : public vec<4, f32>
{
    using BaseTy = vec<4, f32>;

public:
    vec4(f32 x = 0, f32 y = 0, f32 z = 0, f32 w = 0) :
        vec(x, y, z, w)
    {
    }

    vec4(const vec4& rhs) :
        vec(rhs)
    {
    }

    vec4(const BaseTy& rhs) :
        vec(rhs)
    {
    }

    ~vec4() = default;

public:
    void self_cross(const vec4& rhs)
    {
        f32 new_x = y() * rhs.z() - z() * rhs.y();
        f32 new_y = z() * rhs.x() - x() * rhs.z();
        f32 new_z = x() * rhs.y() - y() * rhs.x();
        x() = new_x;
        y() = new_y;
        z() = new_z;
    }

    vec4 cross(const vec4& rhs) const
    {
        vec4 v(*this);
        v.self_cross(rhs);
        return v;
    }

public:
    vec3 to_vec3() const
    {
        return vec3(elem[0], elem[1], elem[2]);
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

    f32& w()
    {
        return vec::at(3);
    }

    f32 w() const
    {
        return vec::at(3);
    }
};

}
