#pragma once

namespace gpx
{

mat1x3::mat1x3(f32 v11, f32 v12, f32 v13) :
    _data(vec3_data(v11, v12, v13))
{
}

mat1x3::mat1x3(vec3 vc3) :
    _data(vc3._data)
{
}

mat1x3::mat1x3(const mat1x3& rhs) :
    _data(rhs._data)
{
}

mat1x3& mat1x3::operator =(const mat1x3& rhs)
{
    _data = rhs._data;
    return *this;
}

vec3 mat1x3::r1() const
{
    return vec3(_data._r1);
}

f32 mat1x3::c1() const
{
    return _data._r1._x;
}

f32 mat1x3::c2() const
{
    return _data._r1._y;
}

f32 mat1x3::c3() const
{
    return _data._r1._z;
}

}
