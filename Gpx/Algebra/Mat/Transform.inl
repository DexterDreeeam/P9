#pragma once

namespace gpx
{

template<mat_major MajorTy>
template<typename ...Args>
_INLINE_ vec4 _TsfNs::transform_base<MajorTy>::act(
    const vec4& v, const MatTy& m, Args ...args)
{
    return act(act(v, m), args...);
}

template<mat_major MajorTy>
_INLINE_ vec4 _TsfNs::transform_base<MajorTy>::act(const vec4& v, const MatTy& m)
{
    return vec4(m.col_at(0) * v, m.row_at(1) * v, m.row_at(2) * v, m.row_at(3) * v);
}

template<mat_major MajorTy>
template<typename ...Args>
_INLINE_ auto _TsfNs::transform_base<MajorTy>::act(
    const MatTy& m1, const MatTy& m2, Args ...args) -> MatTy
{
    return act(act(m1, m2), args...);
}

template<mat_major MajorTy>
_INLINE_ auto _TsfNs::transform_base<MajorTy>::act(const MatTy& m1, const MatTy& m2) -> MatTy
{
    return m2 * m1;
}

template<mat_major MajorTy>
_INLINE_ auto _TsfNs::transform_base<MajorTy>::translate(const vec3& v) -> MatTy
{
    MatTy m;
    m.at(0, 0) = 1;
    m.at(1, 1) = 1;
    m.at(2, 2) = 1;
    m.at(3, 3) = 1;
    m.at(0, 3) = v.x();
    m.at(1, 3) = v.y();
    m.at(2, 3) = v.z();
    return m;
}

template<mat_major MajorTy>
_INLINE_ auto _TsfNs::transform_base<MajorTy>::translate(const vec4& v) -> MatTy
{
    assert(v.w() == 0);
    return translate(v.to_vec3());
}

template<mat_major MajorTy>
_INLINE_ auto _TsfNs::transform_base<MajorTy>::scale(f32 scalar) -> MatTy
{
    MatTy m;
    m.at(0, 0) = scalar;
    m.at(1, 1) = scalar;
    m.at(2, 2) = scalar;
    m.at(3, 3) = 1;
    return m;
}

template<mat_major MajorTy>
_INLINE_ auto _TsfNs::transform_base<MajorTy>::scale(f32 xs, f32 ys, f32 zs) -> MatTy
{
    MatTy m;
    m.at(0, 0) = xs;
    m.at(1, 1) = ys;
    m.at(2, 2) = zs;
    m.at(3, 3) = 1;
    return m;
}

template<mat_major MajorTy>
_INLINE_ auto _TsfNs::transform_base<MajorTy>::rotate_x(f32 radian) -> MatTy
{
    f32 sin = math::sin(radian);
    f32 cos = math::cos(radian);
    MatTy m;
    m.at(0, 0) = 1;
    m.at(1, 1) = cos;
    m.at(1, 2) = -sin;
    m.at(2, 1) = sin;
    m.at(2, 2) = cos;
    m.at(3, 3) = 1;
    return m;
}

template<mat_major MajorTy>
_INLINE_ auto _TsfNs::transform_base<MajorTy>::rotate_y(f32 radian) -> MatTy
{
    f32 sin = math::sin(radian);
    f32 cos = math::cos(radian);
    MatTy m;
    m.at(0, 0) = cos;
    m.at(0, 2) = sin;
    m.at(1, 1) = 1;
    m.at(2, 0) = -sin;
    m.at(2, 2) = cos;
    m.at(3, 3) = 1;
    return m;
}

template<mat_major MajorTy>
_INLINE_ auto _TsfNs::transform_base<MajorTy>::rotate_z(f32 radian) -> MatTy
{
    f32 sin = math::sin(radian);
    f32 cos = math::cos(radian);
    MatTy m;
    m.at(0, 0) = cos;
    m.at(0, 1) = -sin;
    m.at(1, 0) = sin;
    m.at(1, 1) = cos;
    m.at(2, 2) = 1;
    m.at(3, 3) = 1;
    return m;
}

template<mat_major MajorTy>
_INLINE_ auto _TsfNs::transform_base<MajorTy>::rotate(
    const vec3& direction, f32 radian) -> MatTy
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

    //     cos + i_cos*x2, i_cos_xy - sin_z, i_cos_zx + sin_y,  0,
    //   i_cos_xy + sin_z,   cos + i_cos*y2, i_cos_yz - sin_x,  0,
    //   i_cos_zx - sin_y, i_cos_yz + sin_x,   cos + i_cos*z2,  0,
    //                  0,                0,                0,  1

    MatTy m;
    m.at(0, 0) = cos + i_cos * x2;
    m.at(0, 1) = i_cos_xy - sin_z;
    m.at(0, 2) = i_cos_zx + sin_y;
    m.at(1, 0) = i_cos_xy + sin_z;
    m.at(1, 1) = cos + i_cos * y2;
    m.at(1, 2) = i_cos_yz - sin_x;
    m.at(2, 0) = i_cos_zx - sin_y;
    m.at(2, 1) = i_cos_yz + sin_x;
    m.at(2, 2) = cos + i_cos * z2;
    m.at(3, 3) = 1;
    return m;
}

template<mat_major MajorTy>
_INLINE_ auto _TsfNs::transform_base<MajorTy>::rotate(
    const vec4& direction, f32 radian) -> MatTy
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
template<mat_major MajorTy>
_INLINE_ auto _TsfNs::transform_base<MajorTy>::view(
    const vec3& view_pos, const vec3& view_target, const vec3& up) -> MatTy
{
    vec3 n_look_at = (view_target - view_pos).normalize();
    vec3 n_side = n_look_at.cross(up).normalize();
    vec3 n_anti_up = n_look_at.cross(n_side);

//    n_side.x(),    n_side.y(),    n_side.z(), -    view_pos.x()*n_side.x() -    view_pos.y()*n_side.y() -    view_pos.z()*n_side.z(),
// n_anti_up.x(), n_anti_up.y(), n_anti_up.z(), - view_pos.x()*n_anti_up.x() - view_pos.y()*n_anti_up.y() - view_pos.z()*n_anti_up.z(),
// n_look_at.x(), n_look_at.y(), n_look_at.z(), - view_pos.x()*n_look_at.x() - view_pos.y()*n_look_at.y() - view_pos.z()*n_look_at.z(),
//             0,             0,             0,                                                                                      1

    MatTy m;
    m.at(0, 0) = n_side.x();
    m.at(0, 1) = n_side.y();
    m.at(0, 2) = n_side.z();
    m.at(0, 3) = -view_pos.x()*n_side.x() - view_pos.y()*n_side.y() - view_pos.z()*n_side.z();
    m.at(1, 0) = n_anti_up.x();
    m.at(1, 1) = n_anti_up.y();
    m.at(1, 2) = n_anti_up.z();
    m.at(1, 3) = -view_pos.x()*n_anti_up.x() - view_pos.y()*n_anti_up.y() - view_pos.z()*n_anti_up.z();
    m.at(2, 0) = n_look_at.x();
    m.at(2, 1) = n_look_at.y();
    m.at(2, 2) = n_look_at.z();
    m.at(2, 3) = -view_pos.x()*n_look_at.x() - view_pos.y()*n_look_at.y() - view_pos.z()*n_look_at.z();
    m.at(3, 3) = 1;
    return m;
}

template<mat_major MajorTy>
_INLINE_ auto _TsfNs::transform_base<MajorTy>::view(
    const vec4& view_pos, const vec4& view_target, const vec4& up) -> MatTy
{
    return view(view_pos.to_vec3(), view_target.to_vec3(), up.to_vec3());
}

template<mat_major MajorTy>
_INLINE_ auto _TsfNs::transform_base<MajorTy>::ortho_project(
    f32 width, f32 height, f32 sight_distance) -> MatTy
{
    assert(width > math::epsilon());
    assert(height > math::epsilon());
    assert(sight_distance > math::epsilon());

    f32 sx = 2 / width;
    f32 sy = 2 / height;
    f32 sz = 1 / sight_distance;

    // sx,  0,  0, 0,
    //  0, sy,  0, 0,
    //  0,  0, sz, 0,
    //  0,  0,  0, 1
    MatTy m;
    m.at(0, 0) = sx;
    m.at(1, 1) = sy;
    m.at(2, 2) = sz;
    m.at(3, 3) = 1;
    return m;
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
template<mat_major MajorTy>
_INLINE_ auto _TsfNs::transform_base<MajorTy>::persp_project(
    f32 fov, f32 aspect, f32 near, f32 far) -> MatTy
{
    assert(fov > math::epsilon() && fov < math::pi());
    assert(aspect > math::epsilon());
    assert(near > math::epsilon());
    assert(far > near);

    f32 tan_f2 = math::tan(fov / 2);
    f32 height = tan_f2 * near * 2;
    f32 width = height * aspect;
    f32 distance = far - near;

    // 2*near/width,             0,            0,                    0,
    //            0, 2*near/height,            0,                    0,
    //            0,             0, far/distance, -(near*far)/distance,
    //            0,             0,            1,                    0
    MatTy m;
    m.at(0, 0) = 2 * near / width;
    m.at(1, 1) = 2 * near / height;
    m.at(2, 2) = far / distance;
    m.at(2, 3) = -near * far / distance;
    m.at(3, 2) = 1;
    return m;
}

template<mat_major MajorTy>
_INLINE_ auto _TsfNs::transform_base<MajorTy>::perspective(
    f32 fov, f32 aspect, f32 near, f32 far) -> MatTy
{
    return persp_project(fov, aspect, near, far);
}

}
