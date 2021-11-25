#pragma once

namespace gpx
{

struct mat2x3_data
{
    mat2x3_data(const vec3_data& vc3_1, const vec3_data& vc3_2) :
        _r1(vc3_1), _r2(vc3_2)
    {
    }

    mat2x3_data(const mat2x3_data& m23) :
        _r1(m23._r1), _r2(m23._r2)
    {
    }

    mat2x3_data& operator =(const mat2x3_data& rhs)
    {
        _r1 = rhs._r1;
        _r2 = rhs._r2;
        return *this;
    }

    vec3_data _r1;
    vec3_data _r2;
};

class mat2x3
{
public:
    explicit mat2x3(
        f32 v11 = 0, f32 v12 = 0, f32 v13 = 0,
        f32 v21 = 0, f32 v22 = 0, f32 v23 = 0);

    explicit mat2x3(vec3 vc3_1 = vec3(), vec3 vc3_2 = vec3());

    mat2x3(const mat2x3& rhs);

    mat2x3& operator =(const mat2x3& rhs);

public:
    vec3 r1() const;

    vec3 r2() const;

    vec2 c1() const;

    vec2 c2() const;

    vec2 c3() const;

public:
    mat2x3_data _data;
};

}
