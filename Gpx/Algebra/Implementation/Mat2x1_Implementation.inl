#pragma once

namespace gpx
{

mat2x1::mat2x1(
    f32 v11,
    f32 v21) :
    _data(
        v11,
        v21)
{
}

mat2x1::mat2x1(const mat2x1& rhs) :
    _data(rhs._data)
{
}

mat2x1& mat2x1::operator =(const mat2x1& rhs)
{
    _data = rhs._data;
    return *this;
}

f32 mat2x1::r1() const
{
    return _data._r1;
}

f32 mat2x1::r2() const
{
    return _data._r2;
}

vec2 mat2x1::c1() const
{
    return vec2(_data._r1, _data._r2);
}

}
