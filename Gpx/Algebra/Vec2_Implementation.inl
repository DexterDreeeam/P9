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

_INLINE_ vec2& vec2::operator =(const vec2& vc2)
{
    _data = vc2._data;
    return *this;
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

}
