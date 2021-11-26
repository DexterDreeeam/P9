#pragma once

namespace gpx
{

struct mat2x2_data
{
    mat2x2_data(const vec2_data& vc2_1, const vec2_data& vc2_2) :
        _r1(vc2_1), _r2(vc2_2)
    {
    }

    mat2x2_data(const mat2x2_data& m22) :
        _r1(m22._r1), _r2(m22._r2)
    {
    }

    mat2x2_data& operator =(const mat2x2_data& rhs)
    {
        _r1 = rhs._r1;
        _r2 = rhs._r2;
        return *this;
    }

    vec2_data _r1;
    vec2_data _r2;
};

class mat2x2
{
public:
    explicit mat2x2(
        f32 v11 = 0, f32 v12 = 0,
        f32 v21 = 0, f32 v22 = 0);

    explicit mat2x2(vec2 vc2_1 = vec2(), vec2 vc2_2 = vec2());

    mat2x2(const mat2x2& rhs);

    mat2x2& operator =(const mat2x2& rhs);

public:
    vec2 r1() const;

    vec2 r2() const;

    vec2 c1() const;

    vec2 c2() const;

public:
    mat2x2_data _data;
};

}
