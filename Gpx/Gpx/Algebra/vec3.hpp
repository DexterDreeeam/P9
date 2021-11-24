#pragma once

namespace gpx
{

union vec3_data
{
    vec3_data(f32 x, f32 y, f32 z) :
        _x(x),
        _y(y),
        _z(z)
    {
    }

    vec3_data(const vec2_data& vc2, f32 z) :
        _x(vc2._x),
        _y(vc2._y),
        _z(z)
    {
    }

    vec3_data(f32 x, const vec2_data& vc2) :
        _x(x),
        _y(vc2._x),
        _z(vc2._y)
    {
    }

    vec3_data(const vec3_data& vc3) :
        _x(vc3._x),
        _y(vc3._y),
        _z(vc3._z)
    {
    }

    vec3_data& operator =(const vec3_data& vc3)
    {
        _x = vc3._x;
        _y = vc3._y;
        _z = vc3._z;
        return *this;
    }

    struct
    {
        f32 _x;
        f32 _y;
        f32 _z;
    };

    struct
    {
        f32 _r;
        f32 _g;
        f32 _b;
    };

    struct
    {
        f32 _s;
        f32 _t;
        f32 _p;
    };

    f32 _v[3];
};

class vec3
{
public:
    explicit vec3(f32 x = 0, f32 y = 0, f32 z = 0);

    explicit vec3(const vec2& vc2, f32 z = 0);

    vec3(f32 x, const vec2& vc2);

    vec3(const vec3& vc3);

    vec3& operator =(const vec3& vc3);

    f32 x() const;

    f32 y() const;

    f32 z() const;

    vec2 xy() const;

    vec2 xz() const;

    vec2 yx() const;

    vec2 yz() const;

    vec2 zx() const;

    vec2 zy() const;

    vec3 xyz() const;

    vec3 xzy() const;

    vec3 yxz() const;

    vec3 yzx() const;

    vec3 zxy() const;

    vec3 zyx() const;

public:
    vec3_data _data;
};

}
