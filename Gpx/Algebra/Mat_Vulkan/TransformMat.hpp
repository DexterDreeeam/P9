#pragma once

namespace gpx
{

template<algebra_type AlgType>
class transform;

template<>
class transform<algebra_type::Vulkan>
{
public:
    template<typename ...Args>
    static vec4 act(const vec4& v, const mat4x4& m, Args ...args);

    static vec4 act(const vec4& v, const mat4x4& m);

    static mat4x4 translate(const vec3& v);

    static mat4x4 translate(const vec4& v);

    static mat4x4 scale(f32 scalar);

    static mat4x4 scale(f32 x_scalar, f32 y_scalar, f32 z_scalar);

    static mat4x4 rotate_x(f32 radian);

    static mat4x4 rotate_y(f32 radian);

    static mat4x4 rotate_z(f32 radian);

    static mat4x4 rotate(const vec3& direction, f32 radian);

    static mat4x4 rotate(const vec4& direction, f32 radian);

    static mat4x4 view(const vec3& view_pos, const vec3& view_target, const vec3& up);

    static mat4x4 view(const vec4& view_pos, const vec4& view_target, const vec4& up);
};

}
