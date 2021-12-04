#pragma once

namespace gpx
{

_INLINE_ mat1x4::mat1x4(f32 v11, f32 v12, f32 v13, f32 v14) :
    _data(vec4_data(v11, v12, v13, v14))
{
}

_INLINE_ mat1x4::mat1x4(vec4 vc4) :
    _data(vc4._data)
{
}

_INLINE_ mat1x4::mat1x4(const mat1x4& rhs) :
    _data(rhs._data)
{
}

_INLINE_ mat1x4& mat1x4::operator =(const mat1x4& rhs)
{
    _data = rhs._data;
    return *this;
}

_INLINE_ vec4& mat1x4::r1()
{
    return _data._r1;
}

_INLINE_ const vec4& mat1x4::r1() const
{
    return _data._r1;
}

_INLINE_ f32 mat1x4::c1() const
{
    return _data._r1.x();
}

_INLINE_ f32 mat1x4::c2() const
{
    return _data._r1.y();
}

_INLINE_ f32 mat1x4::c3() const
{
    return _data._r1.z();
}

_INLINE_ f32 mat1x4::c4() const
{
    return _data._r1.w();
}

}
