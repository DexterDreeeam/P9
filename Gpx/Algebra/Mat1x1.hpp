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

    f32 _11;
};

class mat1x1
{
public:
    explicit mat1x1(f32 v11 = 0);

    mat1x1(const mat1x1& rhs);

    mat1x1& operator =(const mat1x1& rhs);

public:
    mat1x1_data _data;
};

}
