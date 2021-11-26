#pragma once

namespace gpx
{

_INLINE_ mat3x2::mat3x2(
    f32 v11, f32 v12,
    f32 v21, f32 v22,
    f32 v31, f32 v32) :
    _data(
        vec2_data(v11, v12),
        vec2_data(v21, v22),
        vec2_data(v31, v32))
{
}

_INLINE_ mat3x2::mat3x2(vec2 vc2_1, vec2 vc2_2, vec2 vc2_3) :
    _data(vc2_1._data, vc2_2._data, vc2_3._data)
{
}

_INLINE_ mat3x2::mat3x2(const mat3x2& rhs) :
    _data(rhs._data)
{
}

_INLINE_ mat3x2& mat3x2::operator =(const mat3x2& rhs)
{
    _data = rhs._data;
    return *this;
}

_INLINE_ vec2 mat3x2::r1() const
{
    return _data._r1;
}

_INLINE_ vec2 mat3x2::r2() const
{
    return _data._r2;
}

_INLINE_ vec2 mat3x2::r3() const
{
    return _data._r3;
}

_INLINE_ vec3 mat3x2::c1() const
{
    return vec3(
        _data._r1._x,
        _data._r2._x,
        _data._r3._x);
}

_INLINE_ vec3 mat3x2::c2() const
{
    return vec3(
        _data._r1._y,
        _data._r2._y,
        _data._r3._y);
}

}
