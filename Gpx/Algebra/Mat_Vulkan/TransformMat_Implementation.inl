#pragma once

namespace gpx
{

template<>
template<typename ...Args>
_INLINE_ vec4 transform<algebra_type::Vulkan>::act(const vec4& v, const mat4x4& m, Args ...args)
{
    return act(act(v, m), args...);
}

template<>
_INLINE_ vec4 transform<algebra_type::Vulkan>::act(const vec4& v, const mat4x4& m)
{
    return vec4(m.r1() * v, m.r2() * v, m.r3() * v, m.r4() * v);
}

template<>
template<typename ...Args>
_INLINE_ mat4x4 transform<algebra_type::Vulkan>::act(const mat4x4& m1, const mat4x4& m2, Args ...args)
{
    return act(act(m1, m2), args...);
}

template<>
_INLINE_ mat4x4 transform<algebra_type::Vulkan>::act(const mat4x4& m1, const mat4x4& m2)
{
    return m2 * m1;
}

template<>
_INLINE_ mat4x4 transform<algebra_type::Vulkan>::translate(const vec3& v)
{
    return mat4x4(
        1.0f, 0.0f, 0.0f, v.x(),
        0.0f, 1.0f, 0.0f, v.y(),
        0.0f, 0.0f, 1.0f, v.z(),
        0.0f, 0.0f, 0.0f, 1.0f);
}

template<>
_INLINE_ mat4x4 transform<algebra_type::Vulkan>::translate(const vec4& v)
{
    assert(v.w() == 0);
    return translate(v.to_vec3());
}

template<>
_INLINE_ mat4x4 transform<algebra_type::Vulkan>::scale(f32 scalar)
{
    f32 s = scalar;
    return mat4x4(
           s, 0.0f, 0.0f, 0.0f,
        0.0f,    s, 0.0f, 0.0f,
        0.0f, 0.0f,    s, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);
}

template<>
_INLINE_ mat4x4 transform<algebra_type::Vulkan>::scale(f32 xs, f32 ys, f32 zs)
{
    return mat4x4(
          xs, 0.0f, 0.0f, 0.0f,
        0.0f,   ys, 0.0f, 0.0f,
        0.0f, 0.0f,   zs, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);
}

template<>
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

template<>
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

template<>
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

template<>
_INLINE_ mat4x4 transform<algebra_type::Vulkan>::rotate(const vec3& direction, f32 radian)
{
    // Rodrigues Rotation Formula
    f32 cos = math::cos(radian);
    f32 sin = math::sin(radian);
    f32 i_cos = 1.0f - cos;
    f32 x = direction.x();
    f32 y = direction.y();
    f32 z = direction.z();
    f32 x2 = x * x;
    f32 y2 = y * y;
    f32 z2 = z * z;
    f32 i_cos_xy = i_cos * x * y;
    f32 i_cos_yz = i_cos * y * z;
    f32 i_cos_zx = i_cos * z * x;
    f32 sin_x = sin * x;
    f32 sin_y = sin * y;
    f32 sin_z = sin * z;

    return mat4x4(
          cos + i_cos*x2, i_cos_xy - sin_z, i_cos_zx + sin_y,  0,
        i_cos_xy + sin_z,   cos + i_cos*y2, i_cos_yz - sin_x,  0,
        i_cos_zx - sin_y, i_cos_yz + sin_x,   cos + i_cos*z2,  0,
                       0,                0,                0,  1);
}

template<>
_INLINE_ mat4x4 transform<algebra_type::Vulkan>::rotate(const vec4& direction, f32 radian)
{
    assert(direction.w() == 0);
    return rotate(direction.to_vec3(), radian);
}

//  |(up)
//  |
//  |   / z(look_at)
//  |  /
//  | /
//  |/__________ x(side)
//  |
//  |
//  |
//  | y
//
// M_view = R_view * T_view
//
// R_view_inverse = [
//     Side_x,  AntiUp_x,  LookAt_x,  0,
//     Side_y,  AntiUp_y,  LookAt_y,  0,
//     Side_z,  AntiUp_z,  LookAt_z,  0,
//          0,         0,         0,  1
// ]
//
// R_view = [
//       Side_x,    Side_y,    Side_z,  0,
//     AntiUp_x,  AntiUp_y,  AntiUp_z,  0,
//     LookAt_x,  LookAt_y,  LookAt_z,  0,
//            0,         0,         0,  1
// ]
//
// T_view = [
//     1, 0, 0, -Pos_x,
//     0, 1, 0, -Pos_y,
//     0, 0, 1, -Pos_z,
//     0, 0, 0,      1
// ]
template<>
_INLINE_ mat4x4 transform<algebra_type::Vulkan>::view(const vec3& view_pos, const vec3& view_target, const vec3& up)
{
    vec3 n_look_at = (view_target - view_pos).normalize();
    vec3 n_side = n_look_at.cross(up).normalize();
    vec3 n_anti_up = n_look_at.cross(n_side);

    return mat4x4(
           n_side.x(),    n_side.y(),    n_side.z(), -    view_pos.x()*n_side.x() -    view_pos.y()*n_side.y() -    view_pos.z()*n_side.z(),
        n_anti_up.x(), n_anti_up.y(), n_anti_up.z(), - view_pos.x()*n_anti_up.x() - view_pos.y()*n_anti_up.y() - view_pos.z()*n_anti_up.z(),
        n_look_at.x(), n_look_at.y(), n_look_at.z(), - view_pos.x()*n_look_at.x() - view_pos.y()*n_look_at.y() - view_pos.z()*n_look_at.z(),
                    0,             0,             0,                                                                                      1);
}

template<>
_INLINE_ mat4x4 transform<algebra_type::Vulkan>::view(const vec4& view_pos, const vec4& view_target, const vec4& up)
{
    return view(view_pos.to_vec3(), view_target.to_vec3(), up.to_vec3());
}

template<>
_INLINE_ mat4x4 transform<algebra_type::Vulkan>::ortho_project(f32 width, f32 height, f32 sight_distance)
{
    assert(width > math::epsilon());
    assert(height > math::epsilon());
    assert(sight_distance > math::epsilon());

    f32 sx = 2 / width;
    f32 sy = 2 / height;
    f32 sz = 1 / sight_distance;
    return mat4x4(
        sx,  0,  0, 0,
         0, sy,  0, 0,
         0,  0, sz, 0,
         0,  0,  0, 1);
}

//
// M_persp = M_orth * M_fc
//
// M_orth = [
//     2/width,        0,            0,                0,
//           0, 2/height,            0,                0,
//           0,        0, 1/(far-near), -near/(far-near),
//           0,        0,            0,                1
// ]
//
// frustum -> cuboid
// M_fc = [
//     near,    0,        0,         0,
//        0, near,        0,         0,
//        0,    0, near+far, -near*far,
//        0,    0,        1,         0
// ]
//
// M_fc * [ x, y, z ]
// [   near*x,   near*y, near*z+far*z-near*far, z ]
// [ near/z*x, near/z*y,   near+far-near*far/z, 1 ]
//
// M_persp = [
//     2*near/width,             0,              0,                    0,
//                0, 2*near/height,              0,                    0,
//                0,             0, far/(far-near), -near*far/(far-near),
//                0,             0,              1,                    0
// ]
//
// height/2 = tan(fov/2) * near
// width = height * aspect
//
template<>
_INLINE_ mat4x4 transform<algebra_type::Vulkan>::persp_project(f32 fov, f32 aspect, f32 near, f32 far)
{
    assert(fov > math::epsilon() && fov < math::pi());
    assert(aspect > math::epsilon());
    assert(near > math::epsilon());
    assert(far > near);

    f32 tan_f2 = math::tan(fov / 2);
    f32 height = tan_f2 * near * 2;
    f32 width = height * aspect;
    f32 distance = far - near;

    return mat4x4(
        2*near/width,             0,            0,                    0,
                   0, 2*near/height,            0,                    0,
                   0,             0, far/distance, -(near*far)/distance,
                   0,             0,            1,                    0);
}

template<>
_INLINE_ mat4x4 transform<algebra_type::Vulkan>::perspective(f32 fov, f32 aspect, f32 near, f32 far)
{
    return persp_project(fov, aspect, near, far);
}

}
