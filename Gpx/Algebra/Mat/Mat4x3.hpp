#pragma once

namespace gpx
{

struct mat4x3_data
{
    mat4x3_data(const vec3& vc3_1, const vec3& vc3_2, const vec3& vc3_3, const vec3& vc3_4) :
        _r1(vc3_1), _r2(vc3_2), _r3(vc3_3), _r4(vc3_4)
    {
    }

    mat4x3_data(const mat4x3_data& m43) :
        _r1(m43._r1), _r2(m43._r2), _r3(m43._r3), _r4(m43._r4)
    {
    }

    mat4x3_data& operator =(const mat4x3_data& rhs)
    {
        _r1 = rhs._r1;
        _r2 = rhs._r2;
        _r3 = rhs._r3;
        _r4 = rhs._r4;
        return *this;
    }

    vec3 _r1;
    vec3 _r2;
    vec3 _r3;
    vec3 _r4;
};

class mat4x3
{
public:
    explicit mat4x3(
        f32 v11 = 0, f32 v12 = 0, f32 v13 = 0,
        f32 v21 = 0, f32 v22 = 0, f32 v23 = 0,
        f32 v31 = 0, f32 v32 = 0, f32 v33 = 0,
        f32 v41 = 0, f32 v42 = 0, f32 v43 = 0);

    explicit mat4x3(vec3 vc3_1, vec3 vc3_2 = vec3(), vec3 vc3_3 = vec3(), vec3 vc3_4 = vec3());

    mat4x3(const mat4x3& rhs);

    mat4x3& operator =(const mat4x3& rhs);

public:
    vec3& r1();

    vec3& r2();

    vec3& r3();

    vec3& r4();

    const vec3& r1() const;

    const vec3& r2() const;

    const vec3& r3() const;

    const vec3& r4() const;

    vec4 c1() const;

    vec4 c2() const;

    vec4 c3() const;

public:
    mat4x3_data _data;
};

}
