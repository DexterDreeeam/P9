#pragma once

namespace gpx
{

mat2x3::mat2x3(
    f32 v11, f32 v12, f32 v13,
    f32 v21, f32 v22, f32 v23) :
    _data(
        vec3_data(v11, v12, v13),
        vec3_data(v21, v22, v23))
{
}

mat2x3::mat2x3(vec3 vc3_1, vec3 vc3_2) :
    _data(vc3_1._data, vc3_2._data)
{
}

mat2x3::mat2x3(const mat2x3& rhs) :
    _data(rhs._data)
{
}

mat2x3& mat2x3::operator =(const mat2x3& rhs)
{
    _data = rhs._data;
    return *this;
}

vec3 mat2x3::r1() const
{
    return _data._r1;
}

vec3 mat2x3::r2() const
{
    return _data._r2;
}

vec2 mat2x3::c1() const
{
    return vec2(
        _data._r1._x,
        _data._r2._x);
}

vec2 mat2x3::c2() const
{
    return vec2(
        _data._r1._y,
        _data._r2._y);
}

vec2 mat2x3::c3() const
{
    return vec2(
        _data._r1._z,
        _data._r2._z);
}

}
