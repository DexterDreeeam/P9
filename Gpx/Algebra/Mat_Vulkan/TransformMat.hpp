#pragma once

namespace gpx
{

template<algebra_type AlgType>
class transform;

template<>
class transform<algebra_type::Vulkan>
{
public:
    static mat4x4 translate(const vec3& v);

    static mat4x4 scale(f32 scalar);

    static mat4x4 rotate_x(f32 radian);

    static mat4x4 rotate_y(f32 radian);

    static mat4x4 rotate_z(f32 radian);

    static mat4x4 rotate(const vec3& direction, f32 radian);
};

}
