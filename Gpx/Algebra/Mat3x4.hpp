#pragma once

namespace gpx
{

struct mat3x4_data
{
    mat3x4_data(const vec4_data& vc4_1, const vec4_data& vc4_2, const vec4_data& vc4_3) :
        _r1(vc4_1), _r2(vc4_2), _r3(vc4_3)
    {
    }

    mat3x4_data(const mat3x4_data& m34) :
        _r1(m34._r1), _r2(m34._r2), _r3(m34._r3)
    {
    }

    vec4_data _r1;
    vec4_data _r2;
    vec4_data _r3;
};

class mat3x4
{
public:
    explicit mat3x4(
        f32 v11 = 0, f32 v12 = 0, f32 v13 = 0, f32 v14 = 0,
        f32 v21 = 0, f32 v22 = 0, f32 v23 = 0, f32 v24 = 0,
        f32 v31 = 0, f32 v32 = 0, f32 v33 = 0, f32 v34 = 0);

    explicit mat3x4(vec4 vc4_1 = vec4(), vec4 vc4_2 = vec4(), vec4 vc4_3 = vec4());

    mat3x4(const mat3x4& rhs);

    mat3x4& operator =(const mat3x4& rhs);

public:
    mat3x4_data _data;
};

}
