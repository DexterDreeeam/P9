#pragma once

namespace gpx
{

union vec4_data
{
    vec4_data(f32 x, f32 y, f32 z, f32 w) :
        _x(x),
        _y(y),
        _z(z),
        _w(w)
    {
    }

    vec4_data(const vec2_data& vc2_1, const vec2_data& vc2_2) :
        _x(vc2_1._x),
        _y(vc2_1._y),
        _z(vc2_2._x),
        _w(vc2_2._y)
    {
    }

    vec4_data(const vec2_data& vc2, f32 z, f32 w) :
        _x(vc2._x),
        _y(vc2._y),
        _z(z),
        _w(w)
    {
    }

    vec4_data(f32 x, const vec2_data& vc2, f32 w) :
        _x(x),
        _y(vc2._x),
        _z(vc2._y),
        _w(w)
    {
    }

    vec4_data(f32 x, f32 y, const vec2_data& vc2) :
        _x(x),
        _y(y),
        _z(vc2._x),
        _w(vc2._y)
    {
    }

    vec4_data(const vec3_data& vc3, f32 w) :
        _x(vc3._x),
        _y(vc3._y),
        _z(vc3._z),
        _w(w)
    {
    }

    vec4_data(f32 x, const vec3_data& vc3) :
        _x(x),
        _y(vc3._x),
        _z(vc3._y),
        _w(vc3._z)
    {
    }

    vec4_data(const vec4_data& vc4) :
        _x(vc4._x),
        _y(vc4._y),
        _z(vc4._z),
        _w(vc4._w)
    {
    }

    vec4_data& operator =(const vec4_data& vc4)
    {
        _x = vc4._x;
        _y = vc4._y;
        _z = vc4._z;
        _w = vc4._w;
        return *this;
    }

    struct
    {
        f32 _x;
        f32 _y;
        f32 _z;
        f32 _w;
    };

    struct
    {
        f32 _r;
        f32 _g;
        f32 _b;
        f32 _a;
    };

    struct
    {
        f32 _s;
        f32 _t;
        f32 _p;
        f32 _q;
    };

    f32 _v[4];
};

class vec4
{
public:
    explicit vec4(f32 x = 0, f32 y = 0, f32 z = 0, f32 w = 0);

    vec4(const vec2& vc2_1, const vec2& vc2_2);

    explicit vec4(const vec2& vc2, f32 z = 0, f32 w = 0);

    vec4(f32 x, const vec2& vc2, f32 w = 0);

    explicit vec4(f32 x, f32 y, const vec2& vc2);

    explicit vec4(const vec3& vc3, f32 w = 0);

    vec4(f32 x, const vec3& vc3);

    vec4(const vec4& vc4);

    vec4(const vec4_data& vc4);

    vec4& operator =(const vec4& vc4);

    vec4 operator -() const;

    vec4 operator +(const vec4& vc4) const;

    vec4 operator -(const vec4& vc4) const;

    vec3 to_vec3() const;

    f32& x();

    f32& y();

    f32& z();

    f32& w();

    f32 x() const;

    f32 y() const;

    f32 z() const;

    f32 w() const;

    vec2 xy() const;

    vec2 xz() const;

    vec2 xw() const;

    vec2 yx() const;

    vec2 yz() const;

    vec2 yw() const;

    vec2 zx() const;

    vec2 zy() const;

    vec2 zw() const;

    vec2 wx() const;

    vec2 wy() const;

    vec2 wz() const;

    vec3 xyz() const;

    vec3 xyw() const;

    vec3 xzy() const;

    vec3 xzw() const;

    vec3 xwy() const;

    vec3 xwz() const;

    vec3 yxz() const;

    vec3 yxw() const;

    vec3 yzx() const;

    vec3 yzw() const;

    vec3 ywx() const;

    vec3 ywz() const;

    vec3 zxy() const;

    vec3 zxw() const;

    vec3 zyx() const;

    vec3 zyw() const;

    vec3 zwx() const;

    vec3 zwy() const;

    vec3 wxy() const;

    vec3 wxz() const;

    vec3 wyx() const;

    vec3 wyz() const;

    vec3 wzx() const;

    vec3 wzy() const;

    vec4 xyzw() const;

    vec4 xywz() const;

    vec4 xzyw() const;

    vec4 xzwy() const;

    vec4 xwyz() const;

    vec4 xwzy() const;

    vec4 yxzw() const;

    vec4 yxwz() const;

    vec4 yzxw() const;

    vec4 yzwx() const;

    vec4 ywxz() const;

    vec4 ywzx() const;

    vec4 zxyw() const;

    vec4 zxwy() const;

    vec4 zyxw() const;

    vec4 zywx() const;

    vec4 zwxy() const;

    vec4 zwyx() const;

    vec4 wxyz() const;

    vec4 wxzy() const;

    vec4 wyxz() const;

    vec4 wyzx() const;

    vec4 wzxy() const;

    vec4 wzyx() const;

public:
    f32 dot(const vec4& vc4) const;

    f32 operator *(const vec4& vc4) const;

    vec4 cross(const vec4& vc4) const;

    f32 norm_sq() const;

    f32 norm() const;

    void self_normalize();

    vec4 normalize() const;

public:
    vec4_data _data;
};

}
