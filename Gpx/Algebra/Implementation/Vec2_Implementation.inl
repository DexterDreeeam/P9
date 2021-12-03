#pragma once

namespace gpx
{

_INLINE_ vec2::vec2(f32 x, f32 y) :
    _data(x, y)
{
}

_INLINE_ vec2::vec2(const vec2& vc2) :
    _data(vc2._data)
{
}

_INLINE_ vec2::vec2(const vec2_data& vc2) :
    _data(vc2)
{
}

_INLINE_ vec2& vec2::operator =(const vec2& vc2)
{
    _data = vc2._data;
    return *this;
}

_INLINE_ f32& vec2::x()
{
    return _data._x;
}

_INLINE_ f32& vec2::y()
{
    return _data._y;
}

_INLINE_ f32 vec2::x() const
{
    return _data._x;
}

_INLINE_ f32 vec2::y() const
{
    return _data._y;
}

_INLINE_ vec2 vec2::xy() const
{
    return vec2(_data._x, _data._y);
}

_INLINE_ vec2 vec2::yx() const
{
    return vec2(_data._y, _data._x);
}

_INLINE_ f32 vec2::dot(const vec2& vc2) const
{
    f32 rst = 0;
    rst += _data._x * vc2._data._x;
    rst += _data._y * vc2._data._y;
    return rst;
}

_INLINE_ f32 vec2::operator *(const vec2& vc2) const
{
    return dot(vc2);
}

_INLINE_ f32 vec2::norm_sq() const
{
    return _data._x * _data._x + _data._y * _data._y;
}

_INLINE_ f32 vec2::norm() const
{
    return math::square_root(norm_sq());
}

}
