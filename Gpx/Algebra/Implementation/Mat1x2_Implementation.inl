#pragma once

namespace gpx
{

mat1x2::mat1x2(f32 v11, f32 v12) :
    _data(vec2_data(v11, v12))
{
}

mat1x2::mat1x2(vec2 vc2) :
    _data(vc2._data)
{
}

mat1x2::mat1x2(const mat1x2& rhs) :
    _data(rhs._data)
{
}

mat1x2& mat1x2::operator =(const mat1x2& rhs)
{
    _data = rhs._data;
    return *this;
}

vec2 mat1x2::r1() const
{
    return vec2(_data._r1);
}

f32 mat1x2::c1() const
{
    return _data._r1._x;
}

f32 mat1x2::c2() const
{
    return _data._r1._y;
}

}
