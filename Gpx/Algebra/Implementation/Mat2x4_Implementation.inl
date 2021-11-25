#pragma once

namespace gpx
{

mat2x4::mat2x4(
    f32 v11, f32 v12, f32 v13, f32 v14,
    f32 v21, f32 v22, f32 v23, f32 v24) :
    _data(
        vec4_data(v11, v12, v13, v14),
        vec4_data(v21, v22, v23, v24))
{
}

mat2x4::mat2x4(vec4 vc4_1, vec4 vc4_2) :
    _data(vc4_1._data, vc4_2._data)
{
}

mat2x4::mat2x4(const mat2x4& rhs) :
    _data(rhs._data)
{
}

mat2x4& mat2x4::operator =(const mat2x4& rhs)
{
    _data = rhs._data;
    return *this;
}

vec4 mat2x4::r1() const
{
    return _data._r1;
}

vec4 mat2x4::r2() const
{
    return _data._r2;
}

vec2 mat2x4::c1() const
{
    return vec2(
        _data._r1._x,
        _data._r2._x);
}

vec2 mat2x4::c2() const
{
    return vec2(
        _data._r1._y,
        _data._r2._y);
}

vec2 mat2x4::c3() const
{
    return vec2(
        _data._r1._z,
        _data._r2._z);
}

vec2 mat2x4::c4() const
{
    return vec2(
        _data._r1._w,
        _data._r2._w);
}

}
