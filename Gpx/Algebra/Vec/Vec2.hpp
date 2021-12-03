#pragma once

namespace gpx
{

union vec2_data
{
    vec2_data(f32 x, f32 y) :
        _x(x),
        _y(y)
    {
    }

    vec2_data(const vec2_data& vc2) :
        _x(vc2._x),
        _y(vc2._y)
    {
    }

    vec2_data& operator =(const vec2_data& vc2)
    {
        _x = vc2._x;
        _y = vc2._y;
        return *this;
    }

    struct
    {
        f32 _x;
        f32 _y;
    };

    struct
    {
        f32 _r;
        f32 _g;
    };

    struct
    {
        f32 _s;
        f32 _t;
    };

    f32 _v[2];
};

class vec2
{
public:
    explicit vec2(f32 x = 0, f32 y = 0);

    vec2(const vec2& vc2);

    vec2(const vec2_data& vc2);

    vec2& operator =(const vec2& vc2);

    f32& x();

    f32& y();

    f32 x() const;

    f32 y() const;

    vec2 xy() const;

    vec2 yx() const;

public:
    f32 dot(const vec2& vc2) const;

    f32 operator *(const vec2& vc2) const;

    f32 norm_sq() const;

    f32 norm() const;

public:
    vec2_data _data;
};

}
