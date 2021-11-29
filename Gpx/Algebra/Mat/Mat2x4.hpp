#pragma once

namespace gpx
{

struct mat2x4_data
{
    mat2x4_data(const vec4_data& vc4_1, const vec4_data& vc4_2) :
        _r1(vc4_1), _r2(vc4_2)
    {
    }

    mat2x4_data(const mat2x4_data& m24) :
        _r1(m24._r1), _r2(m24._r2)
    {
    }

    mat2x4_data& operator =(const mat2x4_data& rhs)
    {
        _r1 = rhs._r1;
        _r2 = rhs._r2;
        return *this;
    }

    vec4_data _r1;
    vec4_data _r2;
};

class mat2x4
{
public:
    explicit mat2x4(
        f32 v11 = 0, f32 v12 = 0, f32 v13 = 0, f32 v14 = 0,
        f32 v21 = 0, f32 v22 = 0, f32 v23 = 0, f32 v24 = 0);

    explicit mat2x4(vec4 vc4_1, vec4 vc4_2 = vec4());

    mat2x4(const mat2x4& rhs);

    mat2x4& operator =(const mat2x4& rhs);

public:
    vec4 r1() const;

    vec4 r2() const;

    vec2 c1() const;

    vec2 c2() const;

    vec2 c3() const;

    vec2 c4() const;

public:
    mat2x4_data _data;
};

}
