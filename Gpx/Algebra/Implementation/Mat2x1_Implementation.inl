#pragma once

namespace gpx
{

_INLINE_ mat2x1::mat2x1(
    f32 v11,
    f32 v21) :
    _data(
        v11,
        v21)
{
}

_INLINE_ mat2x1::mat2x1(const mat2x1& rhs) :
    _data(rhs._data)
{
}

_INLINE_ mat2x1& mat2x1::operator =(const mat2x1& rhs)
{
    _data = rhs._data;
    return *this;
}

_INLINE_ f32& mat2x1::r1()
{
    return _data._r1;
}

_INLINE_ f32& mat2x1::r2()
{
    return _data._r2;
}

_INLINE_ const f32& mat2x1::r1() const
{
    return _data._r1;
}

_INLINE_ const f32& mat2x1::r2() const
{
    return _data._r2;
}

_INLINE_ vec2 mat2x1::c1() const
{
    return vec2(_data._r1, _data._r2);
}

}
