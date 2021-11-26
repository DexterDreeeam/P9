#pragma once

namespace gpx
{

_INLINE_ mat1x1 operator *(const mat1x1& lhs, const mat1x1& rhs)
{
    auto lr1 = lhs.r1();
    auto rc1 = rhs.c1();
    auto r1 = lr1 * rc1;
    return mat1x1(r1);
}

_INLINE_ mat1x2 operator *(const mat1x1& lhs, const mat1x2& rhs)
{
    auto lr1 = lhs.r1();
    auto rc1 = rhs.c1();
    auto rc2 = rhs.c2();
    auto r1 = vec2(lr1 * rc1, lr1 * rc2);
    return mat1x2(r1);
}

_INLINE_ mat1x3 operator *(const mat1x1& lhs, const mat1x3& rhs)
{
    auto lr1 = lhs.r1();
    auto rc1 = rhs.c1();
    auto rc2 = rhs.c2();
    auto rc3 = rhs.c3();
    auto r1 = vec3(lr1 * rc1, lr1 * rc2, lr1 * rc3);
    return mat1x3(r1);
}

_INLINE_ mat1x4 operator *(const mat1x1& lhs, const mat1x4& rhs)
{
    auto lr1 = lhs.r1();
    auto rc1 = rhs.c1();
    auto rc2 = rhs.c2();
    auto rc3 = rhs.c3();
    auto rc4 = rhs.c4();
    auto r1 = vec4(lr1 * rc1, lr1 * rc2, lr1 * rc3, lr1 * rc4);
    return mat1x4(r1);
}

_INLINE_ mat1x1 operator *(const mat1x2& lhs, const mat2x1& rhs)
{
    auto lr1 = lhs.r1();
    auto rc1 = rhs.c1();
    auto r1 = lr1 * rc1;
    return mat1x1(r1);
}

_INLINE_ mat1x2 operator *(const mat1x2& lhs, const mat2x2& rhs)
{
    auto lr1 = lhs.r1();
    auto rc1 = rhs.c1();
    auto rc2 = rhs.c2();
    auto r1 = vec2(lr1 * rc1, lr1 * rc2);
    return mat1x2(r1);
}

_INLINE_ mat1x3 operator *(const mat1x2& lhs, const mat2x3& rhs)
{
    auto lr1 = lhs.r1();
    auto rc1 = rhs.c1();
    auto rc2 = rhs.c2();
    auto rc3 = rhs.c3();
    auto r1 = vec3(lr1 * rc1, lr1 * rc2, lr1 * rc3);
    return mat1x3(r1);
}

_INLINE_ mat1x4 operator *(const mat1x2& lhs, const mat2x4& rhs)
{
    auto lr1 = lhs.r1();
    auto rc1 = rhs.c1();
    auto rc2 = rhs.c2();
    auto rc3 = rhs.c3();
    auto rc4 = rhs.c4();
    auto r1 = vec4(lr1 * rc1, lr1 * rc2, lr1 * rc3, lr1 * rc4);
    return mat1x4(r1);
}

_INLINE_ mat1x1 operator *(const mat1x3& lhs, const mat3x1& rhs)
{
    auto lr1 = lhs.r1();
    auto rc1 = rhs.c1();
    auto r1 = lr1 * rc1;
    return mat1x1(r1);
}

_INLINE_ mat1x2 operator *(const mat1x3& lhs, const mat3x2& rhs)
{
    auto lr1 = lhs.r1();
    auto rc1 = rhs.c1();
    auto rc2 = rhs.c2();
    auto r1 = vec2(lr1 * rc1, lr1 * rc2);
    return mat1x2(r1);
}

_INLINE_ mat1x3 operator *(const mat1x3& lhs, const mat3x3& rhs)
{
    auto lr1 = lhs.r1();
    auto rc1 = rhs.c1();
    auto rc2 = rhs.c2();
    auto rc3 = rhs.c3();
    auto r1 = vec3(lr1 * rc1, lr1 * rc2, lr1 * rc3);
    return mat1x3(r1);
}

_INLINE_ mat1x4 operator *(const mat1x3& lhs, const mat3x4& rhs)
{
    auto lr1 = lhs.r1();
    auto rc1 = rhs.c1();
    auto rc2 = rhs.c2();
    auto rc3 = rhs.c3();
    auto rc4 = rhs.c4();
    auto r1 = vec4(lr1 * rc1, lr1 * rc2, lr1 * rc3, lr1 * rc4);
    return mat1x4(r1);
}

_INLINE_ mat1x1 operator *(const mat1x4& lhs, const mat4x1& rhs)
{
    auto lr1 = lhs.r1();
    auto rc1 = rhs.c1();
    auto r1 = lr1 * rc1;
    return mat1x1(r1);
}

_INLINE_ mat1x2 operator *(const mat1x4& lhs, const mat4x2& rhs)
{
    auto lr1 = lhs.r1();
    auto rc1 = rhs.c1();
    auto rc2 = rhs.c2();
    auto r1 = vec2(lr1 * rc1, lr1 * rc2);
    return mat1x2(r1);
}

_INLINE_ mat1x3 operator *(const mat1x4& lhs, const mat4x3& rhs)
{
    auto lr1 = lhs.r1();
    auto rc1 = rhs.c1();
    auto rc2 = rhs.c2();
    auto rc3 = rhs.c3();
    auto r1 = vec3(lr1 * rc1, lr1 * rc2, lr1 * rc3);
    return mat1x3(r1);
}

_INLINE_ mat1x4 operator *(const mat1x4& lhs, const mat4x4& rhs)
{
    auto lr1 = lhs.r1();
    auto rc1 = rhs.c1();
    auto rc2 = rhs.c2();
    auto rc3 = rhs.c3();
    auto rc4 = rhs.c4();
    auto r1 = vec4(lr1 * rc1, lr1 * rc2, lr1 * rc3, lr1 * rc4);
    return mat1x4(r1);
}

//==================================================

_INLINE_ mat2x1 operator *(const mat2x1& lhs, const mat1x1& rhs)
{
    auto lr1 = lhs.r1();
    auto lr2 = lhs.r2();
    auto rc1 = rhs.c1();
    auto r1 = lr1 * rc1;
    auto r2 = lr2 * rc1;
    return mat2x1(r1, r2);
}

_INLINE_ mat2x2 operator *(const mat2x1& lhs, const mat1x2& rhs)
{
    auto lr1 = lhs.r1();
    auto lr2 = lhs.r2();
    auto rc1 = rhs.c1();
    auto rc2 = rhs.c2();
    auto r1 = vec2(lr1 * rc1, lr1 * rc2);
    auto r2 = vec2(lr2 * rc1, lr2 * rc2);
    return mat2x2(r1, r2);
}

_INLINE_ mat2x3 operator *(const mat2x1& lhs, const mat1x3& rhs)
{
    auto lr1 = lhs.r1();
    auto lr2 = lhs.r2();
    auto rc1 = rhs.c1();
    auto rc2 = rhs.c2();
    auto rc3 = rhs.c3();
    auto r1 = vec3(lr1 * rc1, lr1 * rc2, lr1 * rc3);
    auto r2 = vec3(lr2 * rc1, lr2 * rc2, lr2 * rc3);
    return mat2x3(r1, r2);
}

_INLINE_ mat2x4 operator *(const mat2x1& lhs, const mat1x4& rhs)
{
    auto lr1 = lhs.r1();
    auto lr2 = lhs.r2();
    auto rc1 = rhs.c1();
    auto rc2 = rhs.c2();
    auto rc3 = rhs.c3();
    auto rc4 = rhs.c4();
    auto r1 = vec4(lr1 * rc1, lr1 * rc2, lr1 * rc3, lr1 * rc4);
    auto r2 = vec4(lr2 * rc1, lr2 * rc2, lr2 * rc3, lr2 * rc4);
    return mat2x4(r1, r2);
}

_INLINE_ mat2x1 operator *(const mat2x2& lhs, const mat2x1& rhs)
{
    auto lr1 = lhs.r1();
    auto lr2 = lhs.r2();
    auto rc1 = rhs.c1();
    auto r1 = lr1 * rc1;
    auto r2 = lr2 * rc1;
    return mat2x1(r1, r2);
}

_INLINE_ mat2x2 operator *(const mat2x2& lhs, const mat2x2& rhs)
{
    auto lr1 = lhs.r1();
    auto lr2 = lhs.r2();
    auto rc1 = rhs.c1();
    auto rc2 = rhs.c2();
    auto r1 = vec2(lr1 * rc1, lr1 * rc2);
    auto r2 = vec2(lr2 * rc1, lr2 * rc2);
    return mat2x2(r1, r2);
}

_INLINE_ mat2x3 operator *(const mat2x2& lhs, const mat2x3& rhs)
{
    auto lr1 = lhs.r1();
    auto lr2 = lhs.r2();
    auto rc1 = rhs.c1();
    auto rc2 = rhs.c2();
    auto rc3 = rhs.c3();
    auto r1 = vec3(lr1 * rc1, lr1 * rc2, lr1 * rc3);
    auto r2 = vec3(lr2 * rc1, lr2 * rc2, lr2 * rc3);
    return mat2x3(r1, r2);
}

_INLINE_ mat2x4 operator *(const mat2x2& lhs, const mat2x4& rhs)
{
    auto lr1 = lhs.r1();
    auto lr2 = lhs.r2();
    auto rc1 = rhs.c1();
    auto rc2 = rhs.c2();
    auto rc3 = rhs.c3();
    auto rc4 = rhs.c4();
    auto r1 = vec4(lr1 * rc1, lr1 * rc2, lr1 * rc3, lr1 * rc4);
    auto r2 = vec4(lr2 * rc1, lr2 * rc2, lr2 * rc3, lr2 * rc4);
    return mat2x4(r1, r2);
}

_INLINE_ mat2x1 operator *(const mat2x3& lhs, const mat3x1& rhs)
{
    auto lr1 = lhs.r1();
    auto lr2 = lhs.r2();
    auto rc1 = rhs.c1();
    auto r1 = lr1 * rc1;
    auto r2 = lr2 * rc1;
    return mat2x1(r1, r2);
}

_INLINE_ mat2x2 operator *(const mat2x3& lhs, const mat3x2& rhs)
{
    auto lr1 = lhs.r1();
    auto lr2 = lhs.r2();
    auto rc1 = rhs.c1();
    auto rc2 = rhs.c2();
    auto r1 = vec2(lr1 * rc1, lr1 * rc2);
    auto r2 = vec2(lr2 * rc1, lr2 * rc2);
    return mat2x2(r1, r2);
}

_INLINE_ mat2x3 operator *(const mat2x3& lhs, const mat3x3& rhs)
{
    auto lr1 = lhs.r1();
    auto lr2 = lhs.r2();
    auto rc1 = rhs.c1();
    auto rc2 = rhs.c2();
    auto rc3 = rhs.c3();
    auto r1 = vec3(lr1 * rc1, lr1 * rc2, lr1 * rc3);
    auto r2 = vec3(lr2 * rc1, lr2 * rc2, lr2 * rc3);
    return mat2x3(r1, r2);
}

_INLINE_ mat2x4 operator *(const mat2x3& lhs, const mat3x4& rhs)
{
    auto lr1 = lhs.r1();
    auto lr2 = lhs.r2();
    auto rc1 = rhs.c1();
    auto rc2 = rhs.c2();
    auto rc3 = rhs.c3();
    auto rc4 = rhs.c4();
    auto r1 = vec4(lr1 * rc1, lr1 * rc2, lr1 * rc3, lr1 * rc4);
    auto r2 = vec4(lr2 * rc1, lr2 * rc2, lr2 * rc3, lr2 * rc4);
    return mat2x4(r1, r2);
}

_INLINE_ mat2x1 operator *(const mat2x4& lhs, const mat4x1& rhs)
{
    auto lr1 = lhs.r1();
    auto lr2 = lhs.r2();
    auto rc1 = rhs.c1();
    auto r1 = lr1 * rc1;
    auto r2 = lr2 * rc1;
    return mat2x1(r1, r2);
}

_INLINE_ mat2x2 operator *(const mat2x4& lhs, const mat4x2& rhs)
{
    auto lr1 = lhs.r1();
    auto lr2 = lhs.r2();
    auto rc1 = rhs.c1();
    auto rc2 = rhs.c2();
    auto r1 = vec2(lr1 * rc1, lr1 * rc2);
    auto r2 = vec2(lr2 * rc1, lr2 * rc2);
    return mat2x2(r1, r2);
}

_INLINE_ mat2x3 operator *(const mat2x4& lhs, const mat4x3& rhs)
{
    auto lr1 = lhs.r1();
    auto lr2 = lhs.r2();
    auto rc1 = rhs.c1();
    auto rc2 = rhs.c2();
    auto rc3 = rhs.c3();
    auto r1 = vec3(lr1 * rc1, lr1 * rc2, lr1 * rc3);
    auto r2 = vec3(lr2 * rc1, lr2 * rc2, lr2 * rc3);
    return mat2x3(r1, r2);
}

_INLINE_ mat2x4 operator *(const mat2x4& lhs, const mat4x4& rhs)
{
    auto lr1 = lhs.r1();
    auto lr2 = lhs.r2();
    auto rc1 = rhs.c1();
    auto rc2 = rhs.c2();
    auto rc3 = rhs.c3();
    auto rc4 = rhs.c4();
    auto r1 = vec4(lr1 * rc1, lr1 * rc2, lr1 * rc3, lr1 * rc4);
    auto r2 = vec4(lr2 * rc1, lr2 * rc2, lr2 * rc3, lr2 * rc4);
    return mat2x4(r1, r2);
}

//==================================================

_INLINE_ mat3x1 operator *(const mat3x1& lhs, const mat1x1& rhs)
{
    auto lr1 = lhs.r1();
    auto lr2 = lhs.r2();
    auto lr3 = lhs.r3();
    auto rc1 = rhs.c1();
    auto r1 = lr1 * rc1;
    auto r2 = lr2 * rc1;
    auto r3 = lr3 * rc1;
    return mat3x1(r1, r2, r3);
}

_INLINE_ mat3x2 operator *(const mat3x1& lhs, const mat1x2& rhs)
{
    auto lr1 = lhs.r1();
    auto lr2 = lhs.r2();
    auto lr3 = lhs.r3();
    auto rc1 = rhs.c1();
    auto rc2 = rhs.c2();
    auto r1 = vec2(lr1 * rc1, lr1 * rc2);
    auto r2 = vec2(lr2 * rc1, lr2 * rc2);
    auto r3 = vec2(lr3 * rc1, lr3 * rc2);
    return mat3x2(r1, r2, r3);
}

_INLINE_ mat3x3 operator *(const mat3x1& lhs, const mat1x3& rhs)
{
    auto lr1 = lhs.r1();
    auto lr2 = lhs.r2();
    auto lr3 = lhs.r3();
    auto rc1 = rhs.c1();
    auto rc2 = rhs.c2();
    auto rc3 = rhs.c3();
    auto r1 = vec3(lr1 * rc1, lr1 * rc2, lr1 * rc3);
    auto r2 = vec3(lr2 * rc1, lr2 * rc2, lr2 * rc3);
    auto r3 = vec3(lr3 * rc1, lr3 * rc2, lr3 * rc3);
    return mat3x3(r1, r2, r3);
}

_INLINE_ mat3x4 operator *(const mat3x1& lhs, const mat1x4& rhs)
{
    auto lr1 = lhs.r1();
    auto lr2 = lhs.r2();
    auto lr3 = lhs.r3();
    auto rc1 = rhs.c1();
    auto rc2 = rhs.c2();
    auto rc3 = rhs.c3();
    auto rc4 = rhs.c4();
    auto r1 = vec4(lr1 * rc1, lr1 * rc2, lr1 * rc3, lr1 * rc4);
    auto r2 = vec4(lr2 * rc1, lr2 * rc2, lr2 * rc3, lr2 * rc4);
    auto r3 = vec4(lr3 * rc1, lr3 * rc2, lr3 * rc3, lr3 * rc4);
    return mat3x4(r1, r2, r3);
}

_INLINE_ mat3x1 operator *(const mat3x2& lhs, const mat2x1& rhs)
{
    auto lr1 = lhs.r1();
    auto lr2 = lhs.r2();
    auto lr3 = lhs.r3();
    auto rc1 = rhs.c1();
    auto r1 = lr1 * rc1;
    auto r2 = lr2 * rc1;
    auto r3 = lr3 * rc1;
    return mat3x1(r1, r2, r3);
}

_INLINE_ mat3x2 operator *(const mat3x2& lhs, const mat2x2& rhs)
{
    auto lr1 = lhs.r1();
    auto lr2 = lhs.r2();
    auto lr3 = lhs.r3();
    auto rc1 = rhs.c1();
    auto rc2 = rhs.c2();
    auto r1 = vec2(lr1 * rc1, lr1 * rc2);
    auto r2 = vec2(lr2 * rc1, lr2 * rc2);
    auto r3 = vec2(lr3 * rc1, lr3 * rc2);
    return mat3x2(r1, r2, r3);
}

_INLINE_ mat3x3 operator *(const mat3x2& lhs, const mat2x3& rhs)
{
    auto lr1 = lhs.r1();
    auto lr2 = lhs.r2();
    auto lr3 = lhs.r3();
    auto rc1 = rhs.c1();
    auto rc2 = rhs.c2();
    auto rc3 = rhs.c3();
    auto r1 = vec3(lr1 * rc1, lr1 * rc2, lr1 * rc3);
    auto r2 = vec3(lr2 * rc1, lr2 * rc2, lr2 * rc3);
    auto r3 = vec3(lr3 * rc1, lr3 * rc2, lr3 * rc3);
    return mat3x3(r1, r2, r3);
}

_INLINE_ mat3x4 operator *(const mat3x2& lhs, const mat2x4& rhs)
{
    auto lr1 = lhs.r1();
    auto lr2 = lhs.r2();
    auto lr3 = lhs.r3();
    auto rc1 = rhs.c1();
    auto rc2 = rhs.c2();
    auto rc3 = rhs.c3();
    auto rc4 = rhs.c4();
    auto r1 = vec4(lr1 * rc1, lr1 * rc2, lr1 * rc3, lr1 * rc4);
    auto r2 = vec4(lr2 * rc1, lr2 * rc2, lr2 * rc3, lr2 * rc4);
    auto r3 = vec4(lr3 * rc1, lr3 * rc2, lr3 * rc3, lr3 * rc4);
    return mat3x4(r1, r2, r3);
}

_INLINE_ mat3x1 operator *(const mat3x3& lhs, const mat3x1& rhs)
{
    auto lr1 = lhs.r1();
    auto lr2 = lhs.r2();
    auto lr3 = lhs.r3();
    auto rc1 = rhs.c1();
    auto r1 = lr1 * rc1;
    auto r2 = lr2 * rc1;
    auto r3 = lr3 * rc1;
    return mat3x1(r1, r2, r3);
}

_INLINE_ mat3x2 operator *(const mat3x3& lhs, const mat3x2& rhs)
{
    auto lr1 = lhs.r1();
    auto lr2 = lhs.r2();
    auto lr3 = lhs.r3();
    auto rc1 = rhs.c1();
    auto rc2 = rhs.c2();
    auto r1 = vec2(lr1 * rc1, lr1 * rc2);
    auto r2 = vec2(lr2 * rc1, lr2 * rc2);
    auto r3 = vec2(lr3 * rc1, lr3 * rc2);
    return mat3x2(r1, r2, r3);
}

_INLINE_ mat3x3 operator *(const mat3x3& lhs, const mat3x3& rhs)
{
    auto lr1 = lhs.r1();
    auto lr2 = lhs.r2();
    auto lr3 = lhs.r3();
    auto rc1 = rhs.c1();
    auto rc2 = rhs.c2();
    auto rc3 = rhs.c3();
    auto r1 = vec3(lr1 * rc1, lr1 * rc2, lr1 * rc3);
    auto r2 = vec3(lr2 * rc1, lr2 * rc2, lr2 * rc3);
    auto r3 = vec3(lr3 * rc1, lr3 * rc2, lr3 * rc3);
    return mat3x3(r1, r2, r3);
}

_INLINE_ mat3x4 operator *(const mat3x3& lhs, const mat3x4& rhs)
{
    auto lr1 = lhs.r1();
    auto lr2 = lhs.r2();
    auto lr3 = lhs.r3();
    auto rc1 = rhs.c1();
    auto rc2 = rhs.c2();
    auto rc3 = rhs.c3();
    auto rc4 = rhs.c4();
    auto r1 = vec4(lr1 * rc1, lr1 * rc2, lr1 * rc3, lr1 * rc4);
    auto r2 = vec4(lr2 * rc1, lr2 * rc2, lr2 * rc3, lr2 * rc4);
    auto r3 = vec4(lr3 * rc1, lr3 * rc2, lr3 * rc3, lr3 * rc4);
    return mat3x4(r1, r2, r3);
}

_INLINE_ mat3x1 operator *(const mat3x4& lhs, const mat4x1& rhs)
{
    auto lr1 = lhs.r1();
    auto lr2 = lhs.r2();
    auto lr3 = lhs.r3();
    auto rc1 = rhs.c1();
    auto r1 = lr1 * rc1;
    auto r2 = lr2 * rc1;
    auto r3 = lr3 * rc1;
    return mat3x1(r1, r2, r3);
}

_INLINE_ mat3x2 operator *(const mat3x4& lhs, const mat4x2& rhs)
{
    auto lr1 = lhs.r1();
    auto lr2 = lhs.r2();
    auto lr3 = lhs.r3();
    auto rc1 = rhs.c1();
    auto rc2 = rhs.c2();
    auto r1 = vec2(lr1 * rc1, lr1 * rc2);
    auto r2 = vec2(lr2 * rc1, lr2 * rc2);
    auto r3 = vec2(lr3 * rc1, lr3 * rc2);
    return mat3x2(r1, r2, r3);
}

_INLINE_ mat3x3 operator *(const mat3x4& lhs, const mat4x3& rhs)
{
    auto lr1 = lhs.r1();
    auto lr2 = lhs.r2();
    auto lr3 = lhs.r3();
    auto rc1 = rhs.c1();
    auto rc2 = rhs.c2();
    auto rc3 = rhs.c3();
    auto r1 = vec3(lr1 * rc1, lr1 * rc2, lr1 * rc3);
    auto r2 = vec3(lr2 * rc1, lr2 * rc2, lr2 * rc3);
    auto r3 = vec3(lr3 * rc1, lr3 * rc2, lr3 * rc3);
    return mat3x3(r1, r2, r3);
}

_INLINE_ mat3x4 operator *(const mat3x4& lhs, const mat4x4& rhs)
{
    auto lr1 = lhs.r1();
    auto lr2 = lhs.r2();
    auto lr3 = lhs.r3();
    auto rc1 = rhs.c1();
    auto rc2 = rhs.c2();
    auto rc3 = rhs.c3();
    auto rc4 = rhs.c4();
    auto r1 = vec4(lr1 * rc1, lr1 * rc2, lr1 * rc3, lr1 * rc4);
    auto r2 = vec4(lr2 * rc1, lr2 * rc2, lr2 * rc3, lr2 * rc4);
    auto r3 = vec4(lr3 * rc1, lr3 * rc2, lr3 * rc3, lr3 * rc4);
    return mat3x4(r1, r2, r3);
}

//==================================================

_INLINE_ mat4x1 operator *(const mat4x1& lhs, const mat1x1& rhs)
{
    auto lr1 = lhs.r1();
    auto lr2 = lhs.r2();
    auto lr3 = lhs.r3();
    auto lr4 = lhs.r4();
    auto rc1 = rhs.c1();
    auto r1 = lr1 * rc1;
    auto r2 = lr2 * rc1;
    auto r3 = lr3 * rc1;
    auto r4 = lr4 * rc1;
    return mat4x1(r1, r2, r3, r4);
}

_INLINE_ mat4x2 operator *(const mat4x1& lhs, const mat1x2& rhs)
{
    auto lr1 = lhs.r1();
    auto lr2 = lhs.r2();
    auto lr3 = lhs.r3();
    auto lr4 = lhs.r4();
    auto rc1 = rhs.c1();
    auto rc2 = rhs.c2();
    auto r1 = vec2(lr1 * rc1, lr1 * rc2);
    auto r2 = vec2(lr2 * rc1, lr2 * rc2);
    auto r3 = vec2(lr3 * rc1, lr3 * rc2);
    auto r4 = vec2(lr4 * rc1, lr4 * rc2);
    return mat4x2(r1, r2, r3, r4);
}

_INLINE_ mat4x3 operator *(const mat4x1& lhs, const mat1x3& rhs)
{
    auto lr1 = lhs.r1();
    auto lr2 = lhs.r2();
    auto lr3 = lhs.r3();
    auto lr4 = lhs.r4();
    auto rc1 = rhs.c1();
    auto rc2 = rhs.c2();
    auto rc3 = rhs.c3();
    auto r1 = vec3(lr1 * rc1, lr1 * rc2, lr1 * rc3);
    auto r2 = vec3(lr2 * rc1, lr2 * rc2, lr2 * rc3);
    auto r3 = vec3(lr3 * rc1, lr3 * rc2, lr3 * rc3);
    auto r4 = vec3(lr4 * rc1, lr4 * rc2, lr4 * rc3);
    return mat4x3(r1, r2, r3, r4);
}

_INLINE_ mat4x4 operator *(const mat4x1& lhs, const mat1x4& rhs)
{
    auto lr1 = lhs.r1();
    auto lr2 = lhs.r2();
    auto lr3 = lhs.r3();
    auto lr4 = lhs.r4();
    auto rc1 = rhs.c1();
    auto rc2 = rhs.c2();
    auto rc3 = rhs.c3();
    auto rc4 = rhs.c4();
    auto r1 = vec4(lr1 * rc1, lr1 * rc2, lr1 * rc3, lr1 * rc4);
    auto r2 = vec4(lr2 * rc1, lr2 * rc2, lr2 * rc3, lr2 * rc4);
    auto r3 = vec4(lr3 * rc1, lr3 * rc2, lr3 * rc3, lr3 * rc4);
    auto r4 = vec4(lr4 * rc1, lr4 * rc2, lr4 * rc3, lr4 * rc4);
    return mat4x4(r1, r2, r3, r4);
}

_INLINE_ mat4x1 operator *(const mat4x2& lhs, const mat2x1& rhs)
{
    auto lr1 = lhs.r1();
    auto lr2 = lhs.r2();
    auto lr3 = lhs.r3();
    auto lr4 = lhs.r4();
    auto rc1 = rhs.c1();
    auto r1 = lr1 * rc1;
    auto r2 = lr2 * rc1;
    auto r3 = lr3 * rc1;
    auto r4 = lr4 * rc1;
    return mat4x1(r1, r2, r3, r4);
}

_INLINE_ mat4x2 operator *(const mat4x2& lhs, const mat2x2& rhs)
{
    auto lr1 = lhs.r1();
    auto lr2 = lhs.r2();
    auto lr3 = lhs.r3();
    auto lr4 = lhs.r4();
    auto rc1 = rhs.c1();
    auto rc2 = rhs.c2();
    auto r1 = vec2(lr1 * rc1, lr1 * rc2);
    auto r2 = vec2(lr2 * rc1, lr2 * rc2);
    auto r3 = vec2(lr3 * rc1, lr3 * rc2);
    auto r4 = vec2(lr4 * rc1, lr4 * rc2);
    return mat4x2(r1, r2, r3, r4);
}

_INLINE_ mat4x3 operator *(const mat4x2& lhs, const mat2x3& rhs)
{
    auto lr1 = lhs.r1();
    auto lr2 = lhs.r2();
    auto lr3 = lhs.r3();
    auto lr4 = lhs.r4();
    auto rc1 = rhs.c1();
    auto rc2 = rhs.c2();
    auto rc3 = rhs.c3();
    auto r1 = vec3(lr1 * rc1, lr1 * rc2, lr1 * rc3);
    auto r2 = vec3(lr2 * rc1, lr2 * rc2, lr2 * rc3);
    auto r3 = vec3(lr3 * rc1, lr3 * rc2, lr3 * rc3);
    auto r4 = vec3(lr4 * rc1, lr4 * rc2, lr4 * rc3);
    return mat4x3(r1, r2, r3, r4);
}

_INLINE_ mat4x4 operator *(const mat4x2& lhs, const mat2x4& rhs)
{
    auto lr1 = lhs.r1();
    auto lr2 = lhs.r2();
    auto lr3 = lhs.r3();
    auto lr4 = lhs.r4();
    auto rc1 = rhs.c1();
    auto rc2 = rhs.c2();
    auto rc3 = rhs.c3();
    auto rc4 = rhs.c4();
    auto r1 = vec4(lr1 * rc1, lr1 * rc2, lr1 * rc3, lr1 * rc4);
    auto r2 = vec4(lr2 * rc1, lr2 * rc2, lr2 * rc3, lr2 * rc4);
    auto r3 = vec4(lr3 * rc1, lr3 * rc2, lr3 * rc3, lr3 * rc4);
    auto r4 = vec4(lr4 * rc1, lr4 * rc2, lr4 * rc3, lr4 * rc4);
    return mat4x4(r1, r2, r3, r4);
}

_INLINE_ mat4x1 operator *(const mat4x3& lhs, const mat3x1& rhs)
{
    auto lr1 = lhs.r1();
    auto lr2 = lhs.r2();
    auto lr3 = lhs.r3();
    auto lr4 = lhs.r4();
    auto rc1 = rhs.c1();
    auto r1 = lr1 * rc1;
    auto r2 = lr2 * rc1;
    auto r3 = lr3 * rc1;
    auto r4 = lr4 * rc1;
    return mat4x1(r1, r2, r3, r4);
}

_INLINE_ mat4x2 operator *(const mat4x3& lhs, const mat3x2& rhs)
{
    auto lr1 = lhs.r1();
    auto lr2 = lhs.r2();
    auto lr3 = lhs.r3();
    auto lr4 = lhs.r4();
    auto rc1 = rhs.c1();
    auto rc2 = rhs.c2();
    auto r1 = vec2(lr1 * rc1, lr1 * rc2);
    auto r2 = vec2(lr2 * rc1, lr2 * rc2);
    auto r3 = vec2(lr3 * rc1, lr3 * rc2);
    auto r4 = vec2(lr4 * rc1, lr4 * rc2);
    return mat4x2(r1, r2, r3, r4);
}

_INLINE_ mat4x3 operator *(const mat4x3& lhs, const mat3x3& rhs)
{
    auto lr1 = lhs.r1();
    auto lr2 = lhs.r2();
    auto lr3 = lhs.r3();
    auto lr4 = lhs.r4();
    auto rc1 = rhs.c1();
    auto rc2 = rhs.c2();
    auto rc3 = rhs.c3();
    auto r1 = vec3(lr1 * rc1, lr1 * rc2, lr1 * rc3);
    auto r2 = vec3(lr2 * rc1, lr2 * rc2, lr2 * rc3);
    auto r3 = vec3(lr3 * rc1, lr3 * rc2, lr3 * rc3);
    auto r4 = vec3(lr4 * rc1, lr4 * rc2, lr4 * rc3);
    return mat4x3(r1, r2, r3, r4);
}

_INLINE_ mat4x4 operator *(const mat4x3& lhs, const mat3x4& rhs)
{
    auto lr1 = lhs.r1();
    auto lr2 = lhs.r2();
    auto lr3 = lhs.r3();
    auto lr4 = lhs.r4();
    auto rc1 = rhs.c1();
    auto rc2 = rhs.c2();
    auto rc3 = rhs.c3();
    auto rc4 = rhs.c4();
    auto r1 = vec4(lr1 * rc1, lr1 * rc2, lr1 * rc3, lr1 * rc4);
    auto r2 = vec4(lr2 * rc1, lr2 * rc2, lr2 * rc3, lr2 * rc4);
    auto r3 = vec4(lr3 * rc1, lr3 * rc2, lr3 * rc3, lr3 * rc4);
    auto r4 = vec4(lr4 * rc1, lr4 * rc2, lr4 * rc3, lr4 * rc4);
    return mat4x4(r1, r2, r3, r4);
}

_INLINE_ mat4x1 operator *(const mat4x4& lhs, const mat4x1& rhs)
{
    auto lr1 = lhs.r1();
    auto lr2 = lhs.r2();
    auto lr3 = lhs.r3();
    auto lr4 = lhs.r4();
    auto rc1 = rhs.c1();
    auto r1 = lr1 * rc1;
    auto r2 = lr2 * rc1;
    auto r3 = lr3 * rc1;
    auto r4 = lr4 * rc1;
    return mat4x1(r1, r2, r3, r4);
}

_INLINE_ mat4x2 operator *(const mat4x4& lhs, const mat4x2& rhs)
{
    auto lr1 = lhs.r1();
    auto lr2 = lhs.r2();
    auto lr3 = lhs.r3();
    auto lr4 = lhs.r4();
    auto rc1 = rhs.c1();
    auto rc2 = rhs.c2();
    auto r1 = vec2(lr1 * rc1, lr1 * rc2);
    auto r2 = vec2(lr2 * rc1, lr2 * rc2);
    auto r3 = vec2(lr3 * rc1, lr3 * rc2);
    auto r4 = vec2(lr4 * rc1, lr4 * rc2);
    return mat4x2(r1, r2, r3, r4);
}

_INLINE_ mat4x3 operator *(const mat4x4& lhs, const mat4x3& rhs)
{
    auto lr1 = lhs.r1();
    auto lr2 = lhs.r2();
    auto lr3 = lhs.r3();
    auto lr4 = lhs.r4();
    auto rc1 = rhs.c1();
    auto rc2 = rhs.c2();
    auto rc3 = rhs.c3();
    auto r1 = vec3(lr1 * rc1, lr1 * rc2, lr1 * rc3);
    auto r2 = vec3(lr2 * rc1, lr2 * rc2, lr2 * rc3);
    auto r3 = vec3(lr3 * rc1, lr3 * rc2, lr3 * rc3);
    auto r4 = vec3(lr4 * rc1, lr4 * rc2, lr4 * rc3);
    return mat4x3(r1, r2, r3, r4);
}

_INLINE_ mat4x4 operator *(const mat4x4& lhs, const mat4x4& rhs)
{
    auto lr1 = lhs.r1();
    auto lr2 = lhs.r2();
    auto lr3 = lhs.r3();
    auto lr4 = lhs.r4();
    auto rc1 = rhs.c1();
    auto rc2 = rhs.c2();
    auto rc3 = rhs.c3();
    auto rc4 = rhs.c4();
    auto r1 = vec4(lr1 * rc1, lr1 * rc2, lr1 * rc3, lr1 * rc4);
    auto r2 = vec4(lr2 * rc1, lr2 * rc2, lr2 * rc3, lr2 * rc4);
    auto r3 = vec4(lr3 * rc1, lr3 * rc2, lr3 * rc3, lr3 * rc4);
    auto r4 = vec4(lr4 * rc1, lr4 * rc2, lr4 * rc3, lr4 * rc4);
    return mat4x4(r1, r2, r3, r4);
}

}
