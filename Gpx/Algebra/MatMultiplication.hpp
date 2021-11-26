#pragma once

namespace gpx
{

mat1x1 operator *(const mat1x1& lhs, const mat1x1& rhs);

mat1x2 operator *(const mat1x1& lhs, const mat1x2& rhs);

mat1x3 operator *(const mat1x1& lhs, const mat1x3& rhs);

mat1x4 operator *(const mat1x1& lhs, const mat1x4& rhs);

mat1x1 operator *(const mat1x2& lhs, const mat2x1& rhs);

mat1x2 operator *(const mat1x2& lhs, const mat2x2& rhs);

mat1x3 operator *(const mat1x2& lhs, const mat2x3& rhs);

mat1x4 operator *(const mat1x2& lhs, const mat2x4& rhs);

mat1x1 operator *(const mat1x3& lhs, const mat3x1& rhs);

mat1x2 operator *(const mat1x3& lhs, const mat3x2& rhs);

mat1x3 operator *(const mat1x3& lhs, const mat3x3& rhs);

mat1x4 operator *(const mat1x3& lhs, const mat3x4& rhs);

mat1x1 operator *(const mat1x4& lhs, const mat4x1& rhs);

mat1x2 operator *(const mat1x4& lhs, const mat4x2& rhs);

mat1x3 operator *(const mat1x4& lhs, const mat4x3& rhs);

mat1x4 operator *(const mat1x4& lhs, const mat4x4& rhs);

mat2x1 operator *(const mat2x1& lhs, const mat1x1& rhs);

mat2x2 operator *(const mat2x1& lhs, const mat1x2& rhs);

mat2x3 operator *(const mat2x1& lhs, const mat1x3& rhs);

mat2x4 operator *(const mat2x1& lhs, const mat1x4& rhs);

mat2x1 operator *(const mat2x2& lhs, const mat2x1& rhs);

mat2x2 operator *(const mat2x2& lhs, const mat2x2& rhs);

mat2x3 operator *(const mat2x2& lhs, const mat2x3& rhs);

mat2x4 operator *(const mat2x2& lhs, const mat2x4& rhs);

mat2x1 operator *(const mat2x3& lhs, const mat3x1& rhs);

mat2x2 operator *(const mat2x3& lhs, const mat3x2& rhs);

mat2x3 operator *(const mat2x3& lhs, const mat3x3& rhs);

mat2x4 operator *(const mat2x3& lhs, const mat3x4& rhs);

mat2x1 operator *(const mat2x4& lhs, const mat4x1& rhs);

mat2x2 operator *(const mat2x4& lhs, const mat4x2& rhs);

mat2x3 operator *(const mat2x4& lhs, const mat4x3& rhs);

mat2x4 operator *(const mat2x4& lhs, const mat4x4& rhs);

mat3x1 operator *(const mat3x1& lhs, const mat1x1& rhs);

mat3x2 operator *(const mat3x1& lhs, const mat1x2& rhs);

mat3x3 operator *(const mat3x1& lhs, const mat1x3& rhs);

mat3x4 operator *(const mat3x1& lhs, const mat1x4& rhs);

mat3x1 operator *(const mat3x2& lhs, const mat2x1& rhs);

mat3x2 operator *(const mat3x2& lhs, const mat2x2& rhs);

mat3x3 operator *(const mat3x2& lhs, const mat2x3& rhs);

mat3x4 operator *(const mat3x2& lhs, const mat2x4& rhs);

mat3x1 operator *(const mat3x3& lhs, const mat3x1& rhs);

mat3x2 operator *(const mat3x3& lhs, const mat3x2& rhs);

mat3x3 operator *(const mat3x3& lhs, const mat3x3& rhs);

mat3x4 operator *(const mat3x3& lhs, const mat3x4& rhs);

mat3x1 operator *(const mat3x4& lhs, const mat4x1& rhs);

mat3x2 operator *(const mat3x4& lhs, const mat4x2& rhs);

mat3x3 operator *(const mat3x4& lhs, const mat4x3& rhs);

mat3x4 operator *(const mat3x4& lhs, const mat4x4& rhs);

mat4x1 operator *(const mat4x1& lhs, const mat1x1& rhs);

mat4x2 operator *(const mat4x1& lhs, const mat1x2& rhs);

mat4x3 operator *(const mat4x1& lhs, const mat1x3& rhs);

mat4x4 operator *(const mat4x1& lhs, const mat1x4& rhs);

mat4x1 operator *(const mat4x2& lhs, const mat2x1& rhs);

mat4x2 operator *(const mat4x2& lhs, const mat2x2& rhs);

mat4x3 operator *(const mat4x2& lhs, const mat2x3& rhs);

mat4x4 operator *(const mat4x2& lhs, const mat2x4& rhs);

mat4x1 operator *(const mat4x3& lhs, const mat3x1& rhs);

mat4x2 operator *(const mat4x3& lhs, const mat3x2& rhs);

mat4x3 operator *(const mat4x3& lhs, const mat3x3& rhs);

mat4x4 operator *(const mat4x3& lhs, const mat3x4& rhs);

mat4x1 operator *(const mat4x4& lhs, const mat4x1& rhs);

mat4x2 operator *(const mat4x4& lhs, const mat4x2& rhs);

mat4x3 operator *(const mat4x4& lhs, const mat4x3& rhs);

mat4x4 operator *(const mat4x4& lhs, const mat4x4& rhs);

}
