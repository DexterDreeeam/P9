#pragma once

namespace gpx
{

struct mat1x3_data
{
    mat1x3_data(const vec3& vc3) :
        _r1(vc3)
    {
    }

    mat1x3_data(const mat1x3_data& m13) :
        _r1(m13._r1)
    {
    }

    mat1x3_data& operator =(const mat1x3_data& rhs)
    {
        _r1 = rhs._r1;
        return *this;
    }

    vec3 _r1;
};

class mat1x3
{
public:
    explicit mat1x3(f32 v11 = 0, f32 v12 = 0, f32 v13 = 0);

    explicit mat1x3(vec3 vc3);

    mat1x3(const mat1x3& rhs);

    mat1x3& operator =(const mat1x3& rhs);

public:
    vec3& r1();

    const vec3& r1() const;

    f32 c1() const;

    f32 c2() const;

    f32 c3() const;

public:
    mat1x3_data _data;
};

}
