#pragma once

namespace gpx
{

mat3x1::mat3x1(
    f32 v11,
    f32 v21,
    f32 v31) :
    _data(
        v11,
        v21,
        v31)
{
}

mat3x1::mat3x1(const mat3x1& rhs) :
    _data(rhs._data)
{
}

mat3x1& mat3x1::operator =(const mat3x1& rhs)
{
    _data = rhs._data;
    return *this;
}

f32 mat3x1::r1() const
{
    return _data._r1;
}

f32 mat3x1::r2() const
{
    return _data._r2;
}

f32 mat3x1::r3() const
{
    return _data._r3;
}

vec3 mat3x1::c1() const
{
    return vec3(
        _data._r1,
        _data._r2,
        _data._r3);
}

}
