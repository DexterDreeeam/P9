#pragma once

namespace gpx
{

_INLINE_ mat3x1::mat3x1(
    f32 v11,
    f32 v21,
    f32 v31) :
    _data(
        v11,
        v21,
        v31)
{
}

_INLINE_ mat3x1::mat3x1(const mat3x1& rhs) :
    _data(rhs._data)
{
}

_INLINE_ mat3x1& mat3x1::operator =(const mat3x1& rhs)
{
    _data = rhs._data;
    return *this;
}

_INLINE_ f32& mat3x1::r1()
{
    return _data._r1;
}

_INLINE_ f32& mat3x1::r2()
{
    return _data._r2;
}

_INLINE_ f32& mat3x1::r3()
{
    return _data._r3;
}

_INLINE_ const f32& mat3x1::r1() const
{
    return _data._r1;
}

_INLINE_ const f32& mat3x1::r2() const
{
    return _data._r2;
}

_INLINE_ const f32& mat3x1::r3() const
{
    return _data._r3;
}

_INLINE_ vec3 mat3x1::c1() const
{
    return vec3(
        _data._r1,
        _data._r2,
        _data._r3);
}

}
