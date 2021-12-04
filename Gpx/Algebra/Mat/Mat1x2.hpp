#pragma once

namespace gpx
{

struct mat1x2_data
{
    mat1x2_data(const vec2& vc2) :
        _r1(vc2)
    {
    }

    mat1x2_data(const mat1x2_data& m12) :
        _r1(m12._r1)
    {
    }

    mat1x2_data& operator =(const mat1x2_data& rhs)
    {
        _r1 = rhs._r1;
        return *this;
    }

    vec2 _r1;
};

class mat1x2
{
public:
    explicit mat1x2(f32 v11 = 0, f32 v12 = 0);

    explicit mat1x2(vec2 vc2);

    mat1x2(const mat1x2& rhs);

    mat1x2& operator =(const mat1x2& rhs);

public:
    vec2& r1();

    const vec2& r1() const;

    f32 c1() const;

    f32 c2() const;

public:
    mat1x2_data _data;
};

}
