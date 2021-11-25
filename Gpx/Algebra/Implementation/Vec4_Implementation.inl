#pragma once

namespace gpx
{

_INLINE_ vec4::vec4(f32 x, f32 y, f32 z, f32 w) :
    _data(x, y, z, w)
{
}

_INLINE_ vec4::vec4(const vec2& vc2_1, const vec2& vc2_2) :
    _data(vc2_1._data, vc2_2._data)
{
}

_INLINE_ vec4::vec4(const vec2& vc2, f32 z, f32 w) :
    _data(vc2._data, z, w)
{
}

_INLINE_ vec4::vec4(f32 x, const vec2& vc2, f32 w) :
    _data(x, vc2._data, w)
{
}

_INLINE_ vec4::vec4(f32 x, f32 y, const vec2& vc2) :
    _data(x, y, vc2._data)
{
}

_INLINE_ vec4::vec4(const vec3& vc3, f32 w) :
    _data(vc3._data, w)
{
}

_INLINE_ vec4::vec4(f32 x, const vec3& vc3) :
    _data(x, vc3._data)
{
}

_INLINE_ vec4::vec4(const vec4& vc4) :
    _data(vc4._data)
{
}

_INLINE_ vec4::vec4(const vec4_data& vc4) :
    _data(vc4)
{
}

_INLINE_ vec4& vec4::operator =(const vec4& vc4)
{
    _data = vc4._data;
    return *this;
}

_INLINE_ f32 vec4::x() const
{
    return _data._x;
}

_INLINE_ f32 vec4::y() const
{
    return _data._y;
}

_INLINE_ f32 vec4::z() const
{
    return _data._z;
}

_INLINE_ f32 vec4::w() const
{
    return _data._w;
}

_INLINE_ vec2 vec4::xy() const
{
    return vec2(_data._x, _data._y);
}

_INLINE_ vec2 vec4::xz() const
{
    return vec2(_data._x, _data._z);
}

_INLINE_ vec2 vec4::xw() const
{
    return vec2(_data._x, _data._w);
}

_INLINE_ vec2 vec4::yx() const
{
    return vec2(_data._y, _data._x);
}

_INLINE_ vec2 vec4::yz() const
{
    return vec2(_data._y, _data._z);
}

_INLINE_ vec2 vec4::yw() const
{
    return vec2(_data._y, _data._w);
}

_INLINE_ vec2 vec4::zx() const
{
    return vec2(_data._z, _data._x);
}

_INLINE_ vec2 vec4::zy() const
{
    return vec2(_data._z, _data._y);
}

_INLINE_ vec2 vec4::zw() const
{
    return vec2(_data._z, _data._w);
}

_INLINE_ vec2 vec4::wx() const
{
    return vec2(_data._w, _data._x);
}

_INLINE_ vec2 vec4::wy() const
{
    return vec2(_data._w, _data._y);
}

_INLINE_ vec2 vec4::wz() const
{
    return vec2(_data._w, _data._z);
}

_INLINE_ vec3 vec4::xyz() const
{
    return vec3(_data._x, _data._y, _data._z);
}

_INLINE_ vec3 vec4::xyw() const
{
    return vec3(_data._x, _data._y, _data._w);
}

_INLINE_ vec3 vec4::xzy() const
{
    return vec3(_data._x, _data._z, _data._y);
}

_INLINE_ vec3 vec4::xzw() const
{
    return vec3(_data._x, _data._z, _data._w);
}

_INLINE_ vec3 vec4::xwy() const
{
    return vec3(_data._x, _data._w, _data._y);
}

_INLINE_ vec3 vec4::xwz() const
{
    return vec3(_data._x, _data._w, _data._z);
}

_INLINE_ vec3 vec4::yxz() const
{
    return vec3(_data._y, _data._x, _data._z);
}

_INLINE_ vec3 vec4::yxw() const
{
    return vec3(_data._y, _data._x, _data._w);
}

_INLINE_ vec3 vec4::yzx() const
{
    return vec3(_data._y, _data._z, _data._x);
}

_INLINE_ vec3 vec4::yzw() const
{
    return vec3(_data._y, _data._z, _data._w);
}

_INLINE_ vec3 vec4::ywx() const
{
    return vec3(_data._y, _data._w, _data._x);
}

_INLINE_ vec3 vec4::ywz() const
{
    return vec3(_data._y, _data._w, _data._z);
}

_INLINE_ vec3 vec4::zxy() const
{
    return vec3(_data._z, _data._x, _data._y);
}

_INLINE_ vec3 vec4::zxw() const
{
    return vec3(_data._z, _data._x, _data._w);
}

_INLINE_ vec3 vec4::zyx() const
{
    return vec3(_data._z, _data._y, _data._x);
}

_INLINE_ vec3 vec4::zyw() const
{
    return vec3(_data._z, _data._y, _data._w);
}

_INLINE_ vec3 vec4::zwx() const
{
    return vec3(_data._z, _data._w, _data._x);
}

_INLINE_ vec3 vec4::zwy() const
{
    return vec3(_data._z, _data._w, _data._y);
}

_INLINE_ vec3 vec4::wxy() const
{
    return vec3(_data._w, _data._x, _data._y);
}

_INLINE_ vec3 vec4::wxz() const
{
    return vec3(_data._w, _data._x, _data._z);
}

_INLINE_ vec3 vec4::wyx() const
{
    return vec3(_data._w, _data._y, _data._x);
}

_INLINE_ vec3 vec4::wyz() const
{
    return vec3(_data._w, _data._y, _data._z);
}

_INLINE_ vec3 vec4::wzx() const
{
    return vec3(_data._w, _data._z, _data._x);
}

_INLINE_ vec3 vec4::wzy() const
{
    return vec3(_data._w, _data._z, _data._y);
}

_INLINE_ vec4 vec4::xyzw() const
{
    return vec4(_data._x, _data._y, _data._z, _data._w);
}

_INLINE_ vec4 vec4::xywz() const
{
    return vec4(_data._x, _data._y, _data._w, _data._z);
}

_INLINE_ vec4 vec4::xzyw() const
{
    return vec4(_data._x, _data._z, _data._y, _data._w);
}

_INLINE_ vec4 vec4::xzwy() const
{
    return vec4(_data._x, _data._z, _data._w, _data._y);
}

_INLINE_ vec4 vec4::xwyz() const
{
    return vec4(_data._x, _data._w, _data._y, _data._z);
}

_INLINE_ vec4 vec4::xwzy() const
{
    return vec4(_data._x, _data._w, _data._z, _data._y);
}

_INLINE_ vec4 vec4::yxzw() const
{
    return vec4(_data._y, _data._x, _data._z, _data._w);
}

_INLINE_ vec4 vec4::yxwz() const
{
    return vec4(_data._y, _data._x, _data._w, _data._z);
}

_INLINE_ vec4 vec4::yzxw() const
{
    return vec4(_data._y, _data._z, _data._x, _data._w);
}

_INLINE_ vec4 vec4::yzwx() const
{
    return vec4(_data._y, _data._z, _data._w, _data._x);
}

_INLINE_ vec4 vec4::ywxz() const
{
    return vec4(_data._y, _data._w, _data._x, _data._z);
}

_INLINE_ vec4 vec4::ywzx() const
{
    return vec4(_data._y, _data._w, _data._z, _data._x);
}

_INLINE_ vec4 vec4::zxyw() const
{
    return vec4(_data._z, _data._x, _data._y, _data._w);
}

_INLINE_ vec4 vec4::zxwy() const
{
    return vec4(_data._z, _data._x, _data._w, _data._y);
}

_INLINE_ vec4 vec4::zyxw() const
{
    return vec4(_data._z, _data._y, _data._x, _data._w);
}

_INLINE_ vec4 vec4::zywx() const
{
    return vec4(_data._z, _data._y, _data._w, _data._x);
}

_INLINE_ vec4 vec4::zwxy() const
{
    return vec4(_data._z, _data._w, _data._x, _data._y);
}

_INLINE_ vec4 vec4::zwyx() const
{
    return vec4(_data._z, _data._w, _data._y, _data._x);
}

_INLINE_ vec4 vec4::wxyz() const
{
    return vec4(_data._w, _data._x, _data._y, _data._x);
}

_INLINE_ vec4 vec4::wxzy() const
{
    return vec4(_data._w, _data._x, _data._z, _data._y);
}

_INLINE_ vec4 vec4::wyxz() const
{
    return vec4(_data._w, _data._y, _data._x, _data._z);
}

_INLINE_ vec4 vec4::wyzx() const
{
    return vec4(_data._w, _data._y, _data._z, _data._x);
}

_INLINE_ vec4 vec4::wzxy() const
{
    return vec4(_data._w, _data._z, _data._x, _data._y);
}

_INLINE_ vec4 vec4::wzyx() const
{
    return vec4(_data._w, _data._z, _data._y, _data._x);
}

_INLINE_ f32 vec4::dot(const vec4& vc4) const
{
    f32 rst = 0;
    rst += _data._x * vc4._data._x;
    rst += _data._y * vc4._data._y;
    rst += _data._z * vc4._data._z;
    rst += _data._w * vc4._data._w;
    return rst;
}

_INLINE_ f32 vec4::operator *(const vec4& vc4) const
{
    return dot(vc4);
}

}
