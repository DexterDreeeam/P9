#pragma once

namespace gpx
{

mat4x2::mat4x2(
    f32 v11, f32 v12,
    f32 v21, f32 v22,
    f32 v31, f32 v32,
    f32 v41, f32 v42) :
    _data(
        vec2_data(v11, v12),
        vec2_data(v21, v22),
        vec2_data(v31, v32),
        vec2_data(v41, v42))
{
}

mat4x2::mat4x2(vec2 vc2_1, vec2 vc2_2, vec2 vc2_3, vec2 vc2_4) :
    _data(vc2_1._data, vc2_2._data, vc2_3._data, vc2_4._data)
{
}

mat4x2::mat4x2(const mat4x2& rhs) :
    _data(rhs._data)
{
}

mat4x2& mat4x2::operator =(const mat4x2& rhs)
{
    _data = rhs._data;
    return *this;
}

vec2 mat4x2::r1() const
{
    return _data._r1;
}

vec2 mat4x2::r2() const
{
    return _data._r2;
}

vec2 mat4x2::r3() const
{
    return _data._r3;
}

vec2 mat4x2::r4() const
{
    return _data._r4;
}

vec4 mat4x2::c1() const
{
    return vec4(
        _data._r1._x,
        _data._r2._x,
        _data._r3._x,
        _data._r4._x);
}

vec4 mat4x2::c2() const
{
    return vec4(
        _data._r1._y,
        _data._r2._y,
        _data._r3._y,
        _data._r4._y);
}

}
