#pragma once

namespace gpx
{

_INLINE_ vec3::vec3(f32 x, f32 y, f32 z) :
    _data(x, y, z)
{
}

_INLINE_ vec3::vec3(const vec2& vc2, f32 z) :
    _data(vc2._data, z)
{
}

_INLINE_ vec3::vec3(f32 x, const vec2& vc2) :
    _data(x, vc2._data)
{
}

_INLINE_ vec3::vec3(const vec3& vc3) :
    _data(vc3._data)
{
}

_INLINE_ vec3::vec3(const vec3_data& vc3) :
    _data(vc3)
{
}

_INLINE_ vec3& vec3::operator =(const vec3& vc3)
{
    sizeof(vec3);
    _data = vc3._data;
    return *this;
}

_INLINE_ f32& vec3::x()
{
    return _data._x;
}

_INLINE_ f32& vec3::y()
{
    return _data._y;
}

_INLINE_ f32& vec3::z()
{
    return _data._z;
}

_INLINE_ f32 vec3::x() const
{
    return _data._x;
}

_INLINE_ f32 vec3::y() const
{
    return _data._y;
}

_INLINE_ f32 vec3::z() const
{
    return _data._z;
}

_INLINE_ vec2 vec3::xy() const
{
    return vec2(_data._x, _data._y);
}

_INLINE_ vec2 vec3::xz() const
{
    return vec2(_data._x, _data._z);
}

_INLINE_ vec2 vec3::yx() const
{
    return vec2(_data._y, _data._x);
}

_INLINE_ vec2 vec3::yz() const
{
    return vec2(_data._y, _data._z);
}

_INLINE_ vec2 vec3::zx() const
{
    return vec2(_data._z, _data._x);
}

_INLINE_ vec2 vec3::zy() const
{
    return vec2(_data._z, _data._y);
}

_INLINE_ vec3 vec3::xyz() const
{
    return vec3(_data._x, _data._y, _data._z);
}

_INLINE_ vec3 vec3::xzy() const
{
    return vec3(_data._x, _data._z, _data._y);
}

_INLINE_ vec3 vec3::yxz() const
{
    return vec3(_data._y, _data._x, _data._z);
}

_INLINE_ vec3 vec3::yzx() const
{
    return vec3(_data._y, _data._z, _data._x);
}

_INLINE_ vec3 vec3::zxy() const
{
    return vec3(_data._z, _data._x, _data._y);
}

_INLINE_ vec3 vec3::zyx() const
{
    return vec3(_data._z, _data._y, _data._x);
}

_INLINE_ f32 vec3::dot(const vec3& vc3) const
{
    f32 rst = 0;
    rst += _data._x * vc3._data._x;
    rst += _data._y * vc3._data._y;
    rst += _data._z * vc3._data._z;
    return rst;
}

_INLINE_ f32 vec3::operator *(const vec3& vc3) const
{
    return dot(vc3);
}

_INLINE_ vec3 vec3::cross(const vec3& vc3) const
{
    return vec3(
        _data._y * vc3._data._z - _data._z * vc3._data._y,
        _data._z * vc3._data._x - _data._x * vc3._data._z,
        _data._x * vc3._data._y - _data._y * vc3._data._x);
}

_INLINE_ f32 vec3::norm_sq() const
{
    return _data._x * _data._x + _data._y * _data._y + _data._z * _data._z;
}

_INLINE_ f32 vec3::norm() const
{
    return math::square_root(norm_sq());
}

}
