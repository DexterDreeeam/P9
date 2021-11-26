#pragma once

namespace gpx
{

struct mat2x1_data
{
    mat2x1_data(f32 v11, f32 v21) :
        _r1(v11), _r2(v21)
    {
    }

    mat2x1_data(const mat2x1_data& m21) :
        _r1(m21._r1), _r2(m21._r2)
    {
    }

    mat2x1_data& operator =(const mat2x1_data& rhs)
    {
        _r1 = rhs._r1;
        _r2 = rhs._r2;
        return *this;
    }

    f32 _r1;
    f32 _r2;
};

class mat2x1
{
public:
    explicit mat2x1(
        f32 v11 = 0,
        f32 v21 = 0);

    mat2x1(const mat2x1& rhs);

    mat2x1& operator =(const mat2x1& rhs);

public:
    f32 r1() const;

    f32 r2() const;

    vec2 c1() const;

public:
    mat2x1_data _data;
};

}
