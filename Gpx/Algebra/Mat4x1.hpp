#pragma once

namespace gpx
{

struct mat4x1_data
{
    mat4x1_data(f32 v11, f32 v21, f32 v31, f32 v41) :
        _r1(v11), _r2(v21), _r3(v31), _r4(v41)
    {
    }

    mat4x1_data(const mat4x1_data& m41) :
        _r1(m41._r1), _r2(m41._r2), _r3(m41._r3), _r4(m41._r4)
    {
    }

    f32 _r1;
    f32 _r2;
    f32 _r3;
    f32 _r4;
};

class mat4x1
{
public:
    explicit mat4x1(
        f32 v11 = 0,
        f32 v21 = 0,
        f32 v31 = 0,
        f32 v41 = 0);

    mat4x1(const mat4x1& rhs);

    mat4x1& operator =(const mat4x1& rhs);

public:
    mat4x1_data _data;
};

}
