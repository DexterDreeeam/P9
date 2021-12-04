#pragma once

namespace gpx
{

_INLINE_ mat1x1::mat1x1(f32 v11) :
    _data(v11)
{
}

_INLINE_ mat1x1::mat1x1(const mat1x1& rhs) :
    _data(rhs._data)
{
}

_INLINE_ mat1x1& mat1x1::operator =(const mat1x1& rhs)
{
    _data = rhs._data;
    return *this;
}

_INLINE_ mat1x1 mat1x1::identity()
{
    return mat1x1(1.0f);
}

_INLINE_ void mat1x1::self_transpose()
{
    return;
}

_INLINE_ mat1x1 mat1x1::transpose() const
{
    return mat1x1(*this);
}

_INLINE_ f32& mat1x1::r1()
{
    return _data._11;
}

_INLINE_ const f32& mat1x1::r1() const
{
    return _data._11;
}

_INLINE_ f32 mat1x1::c1() const
{
    return _data._11;
}

}
