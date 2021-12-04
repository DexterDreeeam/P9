#pragma once

namespace gpx
{

struct mat1x1_data
{
    mat1x1_data(f32 v11) :
        _11(v11)
    {
    }

    mat1x1_data(const mat1x1_data& m11) :
        _11(m11._11)
    {
    }

    mat1x1_data& operator =(const mat1x1_data& rhs)
    {
        _11 = rhs._11;
        return *this;
    }

    f32 _11;
};

class mat1x1
{
public:
    explicit mat1x1(f32 v11 = 0);

    mat1x1(const mat1x1& rhs);

    mat1x1& operator =(const mat1x1& rhs);

public:
    static mat1x1 identity();

    void self_transpose();

    mat1x1 transpose() const;

    f32& r1();

    const f32& r1() const;

    f32 c1() const;

public:
    mat1x1_data _data;
};

}
