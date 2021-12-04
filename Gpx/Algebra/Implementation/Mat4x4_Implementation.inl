#pragma once

namespace gpx
{

_INLINE_ mat4x4::mat4x4(
    f32 v11, f32 v12, f32 v13, f32 v14,
    f32 v21, f32 v22, f32 v23, f32 v24,
    f32 v31, f32 v32, f32 v33, f32 v34,
    f32 v41, f32 v42, f32 v43, f32 v44) :
    _data(
        vec4_data(v11, v12, v13, v14),
        vec4_data(v21, v22, v23, v24),
        vec4_data(v31, v32, v33, v34),
        vec4_data(v41, v42, v43, v44))
{
}

_INLINE_ mat4x4::mat4x4(vec4 vc4_1, vec4 vc4_2, vec4 vc4_3, vec4 vc4_4) :
    _data(vc4_1._data, vc4_2._data, vc4_3._data, vc4_4._data)
{
}

_INLINE_ mat4x4::mat4x4(const mat4x4& rhs) :
    _data(rhs._data)
{
}

_INLINE_ mat4x4& mat4x4::operator =(const mat4x4& rhs)
{
    _data = rhs._data;
    return *this;
}

_INLINE_ mat4x4 mat4x4::identity()
{
    return mat4x4(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);
}

_INLINE_ void mat4x4::self_transpose()
{
    swap(_data._r1.y(), _data._r2.x());
    swap(_data._r1.z(), _data._r3.x());
    swap(_data._r1.w(), _data._r4.x());
    swap(_data._r2.z(), _data._r3.y());
    swap(_data._r2.w(), _data._r4.y());
    swap(_data._r3.w(), _data._r4.z());
}

_INLINE_ mat4x4 mat4x4::transpose() const
{
    mat4x4 m(*this);
    m.self_transpose();
    return m;
}

_INLINE_ void mat4x4::self_add(const mat4x4 mat)
{
    _data._r1.x() += mat._data._r1.x();
    _data._r1.y() += mat._data._r1.y();
    _data._r1.z() += mat._data._r1.z();
    _data._r1.w() += mat._data._r1.w();

    _data._r2.x() += mat._data._r2.x();
    _data._r2.y() += mat._data._r2.y();
    _data._r2.z() += mat._data._r2.z();
    _data._r2.w() += mat._data._r2.w();

    _data._r3.x() += mat._data._r3.x();
    _data._r3.y() += mat._data._r3.y();
    _data._r3.z() += mat._data._r3.z();
    _data._r3.w() += mat._data._r3.w();

    _data._r4.x() += mat._data._r4.x();
    _data._r4.y() += mat._data._r4.y();
    _data._r4.z() += mat._data._r4.z();
    _data._r4.w() += mat._data._r4.w();
}

_INLINE_ mat4x4 mat4x4::add(const mat4x4 mat) const
{
    mat4x4 m(*this);
    m.self_add(mat);
    return m;
}

_INLINE_ vec4& mat4x4::r1()
{
    return _data._r1;
}

_INLINE_ vec4& mat4x4::r2()
{
    return _data._r2;
}

_INLINE_ vec4& mat4x4::r3()
{
    return _data._r3;
}

_INLINE_ vec4& mat4x4::r4()
{
    return _data._r4;
}

_INLINE_ const vec4& mat4x4::r1() const
{
    return _data._r1;
}

_INLINE_ const vec4& mat4x4::r2() const
{
    return _data._r2;
}

_INLINE_ const vec4& mat4x4::r3() const
{
    return _data._r3;
}

_INLINE_ const vec4& mat4x4::r4() const
{
    return _data._r4;
}

_INLINE_ vec4 mat4x4::c1() const
{
    return vec4(
        _data._r1.x(),
        _data._r2.x(),
        _data._r3.x(),
        _data._r4.x());
}

_INLINE_ vec4 mat4x4::c2() const
{
    return vec4(
        _data._r1.y(),
        _data._r2.y(),
        _data._r3.y(),
        _data._r4.y());
}

_INLINE_ vec4 mat4x4::c3() const
{
    return vec4(
        _data._r1.z(),
        _data._r2.z(),
        _data._r3.z(),
        _data._r4.z());
}

_INLINE_ vec4 mat4x4::c4() const
{
    return vec4(
        _data._r1.w(),
        _data._r2.w(),
        _data._r3.w(),
        _data._r4.w());
}

}
