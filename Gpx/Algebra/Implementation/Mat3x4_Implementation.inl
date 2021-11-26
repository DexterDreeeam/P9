#pragma once

namespace gpx
{

_INLINE_ mat3x4::mat3x4(
    f32 v11, f32 v12, f32 v13, f32 v14,
    f32 v21, f32 v22, f32 v23, f32 v24,
    f32 v31, f32 v32, f32 v33, f32 v34) :
    _data(
        vec4_data(v11, v12, v13, v14),
        vec4_data(v21, v22, v23, v24),
        vec4_data(v31, v32, v33, v34))
{
}

_INLINE_ mat3x4::mat3x4(vec4 vc4_1, vec4 vc4_2, vec4 vc4_3) :
    _data(vc4_1._data, vc4_2._data, vc4_3._data)
{
}

_INLINE_ mat3x4::mat3x4(const mat3x4& rhs) :
    _data(rhs._data)
{
}

_INLINE_ mat3x4& mat3x4::operator =(const mat3x4& rhs)
{
    _data = rhs._data;
    return *this;
}

_INLINE_ vec4 mat3x4::r1() const
{
    return _data._r1;
}

_INLINE_ vec4 mat3x4::r2() const
{
    return _data._r2;
}

_INLINE_ vec4 mat3x4::r3() const
{
    return _data._r3;
}

_INLINE_ vec3 mat3x4::c1() const
{
    return vec3(
        _data._r1._x,
        _data._r2._x,
        _data._r3._x);
}

_INLINE_ vec3 mat3x4::c2() const
{
    return vec3(
        _data._r1._y,
        _data._r2._y,
        _data._r3._y);
}

_INLINE_ vec3 mat3x4::c3() const
{
    return vec3(
        _data._r1._z,
        _data._r2._z,
        _data._r3._z);
}

_INLINE_ vec3 mat3x4::c4() const
{
    return vec3(
        _data._r1._w,
        _data._r2._w,
        _data._r3._w);
}

}
