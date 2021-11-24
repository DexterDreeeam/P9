#pragma once

namespace gpx
{

struct mat4x2_data
{
    mat4x2_data(const vec2_data& vc2_1, const vec2_data& vc2_2, const vec2_data& vc2_3, const vec2_data& vc2_4) :
        _r1(vc2_1), _r2(vc2_2), _r3(vc2_3), _r4(vc2_4)
    {
    }

    mat4x2_data(const mat4x2_data& m42) :
        _r1(m42._r1), _r2(m42._r2), _r3(m42._r3), _r4(m42._r4)
    {
    }

    vec2_data _r1;
    vec2_data _r2;
    vec2_data _r3;
    vec2_data _r4;
};

class mat4x2
{
public:
    explicit mat4x2(
        f32 v11 = 0, f32 v12 = 0,
        f32 v21 = 0, f32 v22 = 0,
        f32 v31 = 0, f32 v32 = 0,
        f32 v41 = 0, f32 v42 = 0);

    explicit mat4x2(vec2 vc2_1 = vec2(), vec2 vc2_2 = vec2(), vec2 vc2_3 = vec2(), vec2 vc2_4 = vec2());

    mat4x2(const mat4x2& rhs);

    mat4x2& operator =(const mat4x2& rhs);

public:
    mat4x2_data _data;
};

}
