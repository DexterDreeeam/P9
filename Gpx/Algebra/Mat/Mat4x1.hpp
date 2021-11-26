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

    mat4x1_data& operator =(const mat4x1_data& rhs)
    {
        _r1 = rhs._r1;
        _r2 = rhs._r2;
        _r3 = rhs._r3;
        _r4 = rhs._r4;
        return *this;
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
    f32 r1() const;

    f32 r2() const;

    f32 r3() const;

    f32 r4() const;

    vec4 c1() const;

public:
    mat4x1_data _data;
};

}
