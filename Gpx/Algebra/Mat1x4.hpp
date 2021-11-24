#pragma once

namespace gpx
{

struct mat1x4_data
{
    mat1x4_data(const vec4_data& vc4) :
        _r1(vc4)
    {
    }

    mat1x4_data(const mat1x4_data& m14) :
        _r1(m14._r1)
    {
    }

    vec4_data _r1;
};

class mat1x4
{
public:
    explicit mat1x4(f32 v11 = 0, f32 v12 = 0, f32 v13 = 0, f32 v14 = 0);

    explicit mat1x4(vec4 vc4 = vec4());

    mat1x4(const mat1x4& rhs);

    mat1x4& operator =(const mat1x4& rhs);

public:
    mat1x4_data _data;
};

}
