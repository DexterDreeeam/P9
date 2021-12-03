#pragma once

namespace gpx
{

struct mat4x4_data
{
    mat4x4_data(const vec4_data& vc4_1, const vec4_data& vc4_2, const vec4_data& vc4_3, const vec4_data& vc4_4) :
        _r1(vc4_1), _r2(vc4_2), _r3(vc4_3), _r4(vc4_4)
    {
    }

    mat4x4_data(const mat4x4_data& m44) :
        _r1(m44._r1), _r2(m44._r2), _r3(m44._r3), _r4(m44._r4)
    {
    }

    mat4x4_data& operator =(const mat4x4_data& rhs)
    {
        _r1 = rhs._r1;
        _r2 = rhs._r2;
        _r3 = rhs._r3;
        _r4 = rhs._r4;
        return *this;
    }

    vec4_data _r1;
    vec4_data _r2;
    vec4_data _r3;
    vec4_data _r4;
};

class mat4x4
{
public:
    explicit mat4x4(
        f32 v11 = 0, f32 v12 = 0, f32 v13 = 0, f32 v14 = 0,
        f32 v21 = 0, f32 v22 = 0, f32 v23 = 0, f32 v24 = 0,
        f32 v31 = 0, f32 v32 = 0, f32 v33 = 0, f32 v34 = 0,
        f32 v41 = 0, f32 v42 = 0, f32 v43 = 0, f32 v44 = 0);

    explicit mat4x4(vec4 vc4_1, vec4 vc4_2 = vec4(), vec4 vc4_3 = vec4(), vec4 vc4_4 = vec4());

    mat4x4(const mat4x4& rhs);

    mat4x4& operator =(const mat4x4& rhs);

public:
    static mat4x4 identity();

    void self_transpose();

    mat4x4 transpose() const;

    void self_add(const mat4x4 mat);

    mat4x4 add(const mat4x4 mat) const;

    vec4 r1() const;

    vec4 r2() const;

    vec4 r3() const;

    vec4 r4() const;

    vec4 c1() const;

    vec4 c2() const;

    vec4 c3() const;

    vec4 c4() const;

public:
    mat4x4_data _data;
};

}
