#pragma once

namespace gpx
{

_INLINE_ mat4x3::mat4x3(
    f32 v11, f32 v12, f32 v13,
    f32 v21, f32 v22, f32 v23,
    f32 v31, f32 v32, f32 v33,
    f32 v41, f32 v42, f32 v43) :
    _data(
        vec3_data(v11, v12, v13),
        vec3_data(v21, v22, v23),
        vec3_data(v31, v32, v33),
        vec3_data(v41, v42, v43))
{
}

_INLINE_ mat4x3::mat4x3(vec3 vc3_1, vec3 vc3_2, vec3 vc3_3, vec3 vc3_4) :
    _data(vc3_1._data, vc3_2._data, vc3_3._data, vc3_4._data)
{
}

_INLINE_ mat4x3::mat4x3(const mat4x3& rhs) :
    _data(rhs._data)
{
}

_INLINE_ mat4x3& mat4x3::operator =(const mat4x3& rhs)
{
    _data = rhs._data;
    return *this;
}

_INLINE_ vec3 mat4x3::r1() const
{
    return _data._r1;
}

_INLINE_ vec3 mat4x3::r2() const
{
    return _data._r2;
}

_INLINE_ vec3 mat4x3::r3() const
{
    return _data._r3;
}

_INLINE_ vec3 mat4x3::r4() const
{
    return _data._r4;
}

_INLINE_ vec4 mat4x3::c1() const
{
    return vec4(
        _data._r1._x,
        _data._r2._x,
        _data._r3._x,
        _data._r4._x);
}

_INLINE_ vec4 mat4x3::c2() const
{
    return vec4(
        _data._r1._y,
        _data._r2._y,
        _data._r3._y,
        _data._r4._y);
}

_INLINE_ vec4 mat4x3::c3() const
{
    return vec4(
        _data._r1._z,
        _data._r2._z,
        _data._r3._z,
        _data._r4._z);
}

}
