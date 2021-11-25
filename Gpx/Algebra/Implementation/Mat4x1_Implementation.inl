#pragma once

namespace gpx
{

mat4x1::mat4x1(
    f32 v11,
    f32 v21,
    f32 v31,
    f32 v41) :
    _data(
        v11,
        v21,
        v31,
        v41)
{
}

mat4x1::mat4x1(const mat4x1& rhs) :
    _data(rhs._data)
{
}

mat4x1& mat4x1::operator =(const mat4x1& rhs)
{
    _data = rhs._data;
    return *this;
}

f32 mat4x1::r1() const
{
    return _data._r1;
}

f32 mat4x1::r2() const
{
    return _data._r2;
}

f32 mat4x1::r3() const
{
    return _data._r3;
}

f32 mat4x1::r4() const
{
    return _data._r4;
}

vec4 mat4x1::c1() const
{
    return vec4(
        _data._r1,
        _data._r2,
        _data._r3,
        _data._r4);
}

}
