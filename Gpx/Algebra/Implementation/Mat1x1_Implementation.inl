#pragma once

namespace gpx
{

mat1x1::mat1x1(f32 v11) :
    _data(v11)
{
}

mat1x1::mat1x1(const mat1x1& rhs) :
    _data(rhs._data)
{
}

mat1x1& mat1x1::operator =(const mat1x1& rhs)
{
    _data = rhs._data;
    return *this;
}

f32 mat1x1::r1() const
{
    return _data._11;
}

f32 mat1x1::c1() const
{
    return _data._11;
}

}
