#pragma once

namespace gpx
{

namespace _TsfNs
{

template<mat_major MajorTy>
class transform_base
{
public:
    using MatTy = mat4x4<MajorTy>;

public:
    template<typename ...Args>
    static vec4 act(const vec4& v, const MatTy& m, Args ...args);

    static vec4 act(const vec4& v, const MatTy& m);

    template<typename ...Args>
    static MatTy act(const MatTy& m1, const MatTy& m2, Args ...args);

    static MatTy act(const MatTy& m1, const MatTy& m2);

    static MatTy translate(const vec3& v);

    static MatTy translate(const vec4& v);

    static MatTy scale(f32 scalar);

    static MatTy scale(f32 x_scalar, f32 y_scalar, f32 z_scalar);

    static MatTy rotate_x(f32 radian);

    static MatTy rotate_y(f32 radian);

    static MatTy rotate_z(f32 radian);

    static MatTy rotate(const vec3& direction, f32 radian);

    static MatTy rotate(const vec4& direction, f32 radian);

    static MatTy view(const vec3& view_pos, const vec3& view_target, const vec3& up);

    static MatTy view(const vec4& view_pos, const vec4& view_target, const vec4& up);

    static MatTy ortho_project(f32 width, f32 height, f32 sight_distance);

    static MatTy persp_project(f32 fov, f32 aspect, f32 near, f32 far);

    static MatTy perspective(f32 fov, f32 aspect, f32 near, f32 far);
};

}

template<graphics_api_type AlgType>
class transform;

template<>
class transform<Vulkan> : public _TsfNs::transform_base<mat_major::Col>
{
};

template<>
class transform<DirectX> : public _TsfNs::transform_base<mat_major::Row>
{
};

}
