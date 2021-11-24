#pragma once

namespace gpx
{

struct mat3x3_data
{
    mat3x3_data(const vec3_data& vc3_1, const vec3_data& vc3_2, const vec3_data& vc3_3) :
        _r1(vc3_1), _r2(vc3_2), _r3(vc3_3)
    {
    }

    mat3x3_data(const mat3x3_data& m33) :
        _r1(m33._r1), _r2(m33._r2), _r3(m33._r3)
    {
    }

    vec3_data _r1;
    vec3_data _r2;
    vec3_data _r3;
};

class mat3x3
{
public:
    explicit mat3x3(
        f32 v11 = 0, f32 v12 = 0, f32 v13 = 0,
        f32 v21 = 0, f32 v22 = 0, f32 v23 = 0,
        f32 v31 = 0, f32 v32 = 0, f32 v33 = 0);

    explicit mat3x3(vec3 vc3_1 = vec3(), vec3 vc3_2 = vec3(), vec3 vc3_3 = vec3());

    mat3x3(const mat3x3& rhs);

    mat3x3& operator =(const mat3x3& rhs);

public:
    mat3x3_data _data;
};

}
