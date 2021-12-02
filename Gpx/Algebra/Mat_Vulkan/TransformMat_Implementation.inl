#pragma once

namespace gpx
{

_INLINE_ mat4x4 transform<algebra_type::Vulkan>::translate(const vec3& v)
{
    return mat4x4(
        1.0f, 0.0f, 0.0f, v.x(),
        0.0f, 1.0f, 0.0f, v.y(),
        0.0f, 0.0f, 1.0f, v.z(),
        0.0f, 0.0f, 0.0f, 1.0f);
}

_INLINE_ mat4x4 transform<algebra_type::Vulkan>::scale(f32 scalar)
{
    f32 s = scalar;
    return mat4x4(
           s, 0.0f, 0.0f, 0.0f,
        0.0f,    s, 0.0f, 0.0f,
        0.0f, 0.0f,    s, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);
}

_INLINE_ mat4x4 transform<algebra_type::Vulkan>::rotate_x(f32 radian)
{
    f32 sin = math::sin(radian);
    f32 cos = math::cos(radian);
    return mat4x4(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f,  cos, -sin, 0.0f,
        0.0f,  sin,  cos, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);
}

_INLINE_ mat4x4 transform<algebra_type::Vulkan>::rotate_y(f32 radian)
{
    f32 sin = math::sin(radian);
    f32 cos = math::cos(radian);
    return mat4x4(
         cos, 0.0f,  sin, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        -sin, 0.0f,  cos, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);
}

_INLINE_ mat4x4 transform<algebra_type::Vulkan>::rotate_z(f32 radian)
{
    f32 sin = math::sin(radian);
    f32 cos = math::cos(radian);
    return mat4x4(
         cos, -sin, 0.0f, 0.0f,
         sin,  cos, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);
}

_INLINE_ mat4x4 transform<algebra_type::Vulkan>::rotate(const vec3& direction, f32 radian)
{
    return mat4x4::identity();
}

}
