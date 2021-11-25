#pragma once

namespace gpx
{

struct mat3x2_data
{
    mat3x2_data(const vec2_data& vc2_1, const vec2_data& vc2_2, const vec2_data& vc2_3) :
        _r1(vc2_1), _r2(vc2_2), _r3(vc2_3)
    {
    }

    mat3x2_data(const mat3x2_data& m32) :
        _r1(m32._r1), _r2(m32._r2), _r3(m32._r3)
    {
    }

    mat3x2_data& operator =(const mat3x2_data& rhs)
    {
        _r1 = rhs._r1;
        _r2 = rhs._r2;
        _r3 = rhs._r3;
        return *this;
    }

    vec2_data _r1;
    vec2_data _r2;
    vec2_data _r3;
};

class mat3x2
{
public:
    explicit mat3x2(
        f32 v11 = 0, f32 v12 = 0,
        f32 v21 = 0, f32 v22 = 0,
        f32 v31 = 0, f32 v32 = 0);

    explicit mat3x2(vec2 vc2_1 = vec2(), vec2 vc2_2 = vec2(), vec2 vc2_3 = vec2());

    mat3x2(const mat3x2& rhs);

    mat3x2& operator =(const mat3x2& rhs);

public:
    vec2 r1() const;

    vec2 r2() const;

    vec2 r3() const;

    vec3 c1() const;

    vec3 c2() const;

public:
    mat3x2_data _data;
};

}
