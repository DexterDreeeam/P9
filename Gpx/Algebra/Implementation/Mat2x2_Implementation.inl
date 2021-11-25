#pragma once

namespace gpx
{

mat2x2::mat2x2(
    f32 v11, f32 v12,
    f32 v21, f32 v22) :
    _data(
        vec2_data(v11, v12),
        vec2_data(v21, v22))
{
}

mat2x2::mat2x2(vec2 vc2_1, vec2 vc2_2) :
    _data(vc2_1._data, vc2_2._data)
{
}

mat2x2::mat2x2(const mat2x2& rhs) :
    _data(rhs._data)
{
}

mat2x2& mat2x2::operator =(const mat2x2& rhs)
{
    _data = rhs._data;
    return *this;
}

vec2 mat2x2::r1() const
{
    return vec2(_data._r1);
}

vec2 mat2x2::r2() const
{
    return vec2(_data._r2);
}

vec2 mat2x2::c1() const
{
    return vec2(
        _data._r1._x,
        _data._r2._x);
}

vec2 mat2x2::c2() const
{
    return vec2(
        _data._r1._y,
        _data._r2._y);
}

}
