#pragma once

typedef WindowsMsvcNs::__m128  simd4xf32;
typedef WindowsMsvcNs::__m256d simd4xf64;

_INLINE_ void simd_write(const f32* src, simd4xf32* dst);
_INLINE_ void simd_write(const f64* src, simd4xf64* dst);
_INLINE_ void simd_read(const simd4xf32* src, f32* dst);
_INLINE_ void simd_read(const simd4xf64* src, f64* dst);
_INLINE_ void simd_add(simd4xf32* operand1, simd4xf32* operand2, simd4xf32* result);
_INLINE_ void simd_add(simd4xf64* operand1, simd4xf64* operand2, simd4xf64* result);
_INLINE_ void simd_sub(simd4xf32* operand1, simd4xf32* operand2, simd4xf32* result);
_INLINE_ void simd_sub(simd4xf64* operand1, simd4xf64* operand2, simd4xf64* result);
_INLINE_ void simd_mul(simd4xf32* operand1, simd4xf32* operand2, simd4xf32* result);
_INLINE_ void simd_mul(simd4xf64* operand1, simd4xf64* operand2, simd4xf64* result);
_INLINE_ void simd_div(simd4xf32* operand1, simd4xf32* operand2, simd4xf32* result);
_INLINE_ void simd_div(simd4xf64* operand1, simd4xf64* operand2, simd4xf64* result);
_INLINE_ void simd_and(simd4xf32* operand1, simd4xf32* operand2, simd4xf32* result);
_INLINE_ void simd_and(simd4xf64* operand1, simd4xf64* operand2, simd4xf64* result);
_INLINE_ void simd_andnot(simd4xf32* operand1, simd4xf32* operand2, simd4xf32* result);
_INLINE_ void simd_andnot(simd4xf64* operand1, simd4xf64* operand2, simd4xf64* result);
_INLINE_ void simd_or(simd4xf32* operand1, simd4xf32* operand2, simd4xf32* result);
_INLINE_ void simd_or(simd4xf64* operand1, simd4xf64* operand2, simd4xf64* result);
_INLINE_ void simd_xor(simd4xf32* operand1, simd4xf32* operand2, simd4xf32* result);
_INLINE_ void simd_xor(simd4xf64* operand1, simd4xf64* operand2, simd4xf64* result);
_INLINE_ void simd_sqrt(simd4xf32* operand1, simd4xf32* result);
_INLINE_ void simd_sqrt(simd4xf64* operand1, simd4xf64* result);
_INLINE_ void simd_rcp(simd4xf32* operand1, simd4xf32* result);
_INLINE_ void simd_rcp(simd4xf64* operand1, simd4xf64* result);
_INLINE_ void simd_min(simd4xf32* operand1, simd4xf32* operand2, simd4xf32* result);
_INLINE_ void simd_min(simd4xf64* operand1, simd4xf64* operand2, simd4xf64* result);
_INLINE_ void simd_max(simd4xf32* operand1, simd4xf32* operand2, simd4xf32* result);
_INLINE_ void simd_max(simd4xf64* operand1, simd4xf64* operand2, simd4xf64* result);

_INLINE_ void simd_write(const f32* src, simd4xf32* dst)
{
    *dst = WindowsMsvcNs::_mm_load_ps(src);
}

_INLINE_ void simd_write(const f64* src, simd4xf64* dst)
{
    *dst = WindowsMsvcNs::_mm256_load_pd(src);
}

_INLINE_ void simd_read(const simd4xf32* src, f32* dst)
{
    WindowsMsvcNs::_mm_store_ps(dst, *src);
}

_INLINE_ void simd_read(const simd4xf64* src, f64* dst)
{
    WindowsMsvcNs::_mm256_store_pd(dst, *src);
}

_INLINE_ void simd_add(simd4xf32* operand1, simd4xf32* operand2, simd4xf32* result)
{
    *result = WindowsMsvcNs::_mm_add_ps(*operand1, *operand2);
}

_INLINE_ void simd_add(simd4xf64* operand1, simd4xf64* operand2, simd4xf64* result)
{
    *result = WindowsMsvcNs::_mm256_add_pd(*operand1, *operand2);
}

_INLINE_ void simd_sub(simd4xf32* operand1, simd4xf32* operand2, simd4xf32* result)
{
    *result = WindowsMsvcNs::_mm_sub_ps(*operand1, *operand2);
}

_INLINE_ void simd_sub(simd4xf64* operand1, simd4xf64* operand2, simd4xf64* result)
{
    *result = WindowsMsvcNs::_mm256_sub_pd(*operand1, *operand2);
}

_INLINE_ void simd_mul(simd4xf32* operand1, simd4xf32* operand2, simd4xf32* result)
{
    *result = WindowsMsvcNs::_mm_mul_ps(*operand1, *operand2);
}

_INLINE_ void simd_mul(simd4xf64* operand1, simd4xf64* operand2, simd4xf64* result)
{
    *result = WindowsMsvcNs::_mm256_mul_pd(*operand1, *operand2);
}

_INLINE_ void simd_div(simd4xf32* operand1, simd4xf32* operand2, simd4xf32* result)
{
    *result = WindowsMsvcNs::_mm_div_ps(*operand1, *operand2);
}

_INLINE_ void simd_div(simd4xf64* operand1, simd4xf64* operand2, simd4xf64* result)
{
    *result = WindowsMsvcNs::_mm256_div_pd(*operand1, *operand2);
}

_INLINE_ void simd_and(simd4xf32* operand1, simd4xf32* operand2, simd4xf32* result)
{
    *result = WindowsMsvcNs::_mm_and_ps(*operand1, *operand2);
}

_INLINE_ void simd_and(simd4xf64* operand1, simd4xf64* operand2, simd4xf64* result)
{
    *result = WindowsMsvcNs::_mm256_and_pd(*operand1, *operand2);
}

_INLINE_ void simd_andnot(simd4xf32* operand1, simd4xf32* operand2, simd4xf32* result)
{
    *result = WindowsMsvcNs::_mm_andnot_ps(*operand1, *operand2);
}

_INLINE_ void simd_andnot(simd4xf64* operand1, simd4xf64* operand2, simd4xf64* result)
{
    *result = WindowsMsvcNs::_mm256_andnot_pd(*operand1, *operand2);
}

_INLINE_ void simd_or(simd4xf32* operand1, simd4xf32* operand2, simd4xf32* result)
{
    *result = WindowsMsvcNs::_mm_or_ps(*operand1, *operand2);
}

_INLINE_ void simd_or(simd4xf64* operand1, simd4xf64* operand2, simd4xf64* result)
{
    *result = WindowsMsvcNs::_mm256_or_pd(*operand1, *operand2);
}

_INLINE_ void simd_xor(simd4xf32* operand1, simd4xf32* operand2, simd4xf32* result)
{
    *result = WindowsMsvcNs::_mm_xor_ps(*operand1, *operand2);
}

_INLINE_ void simd_xor(simd4xf64* operand1, simd4xf64* operand2, simd4xf64* result)
{
    *result = WindowsMsvcNs::_mm256_xor_pd(*operand1, *operand2);
}

_INLINE_ void simd_sqrt(simd4xf32* operand1, simd4xf32* result)
{
    *result = WindowsMsvcNs::_mm_sqrt_ps(*operand1);
}

_INLINE_ void simd_sqrt(simd4xf64* operand1, simd4xf64* result)
{
    *result = WindowsMsvcNs::_mm256_sqrt_pd(*operand1);
}

_INLINE_ void simd_rcp(simd4xf32* operand1, simd4xf32* result)
{
    *result = WindowsMsvcNs::_mm_rcp_ps(*operand1);
}

_INLINE_ void simd_rcp(simd4xf64* operand1, simd4xf64* result)
{
    WindowsMsvcNs::__m256d td;
    f64 unit[] = { 1.0, 1.0, 1.0, 1.0 };
    simd_write(unit, &td);
    simd_div(&td, operand1, result);
}

_INLINE_ void simd_min(simd4xf32* operand1, simd4xf32* operand2, simd4xf32* result)
{
    *result = WindowsMsvcNs::_mm_min_ps(*operand1, *operand2);
}

_INLINE_ void simd_min(simd4xf64* operand1, simd4xf64* operand2, simd4xf64* result)
{
    *result = WindowsMsvcNs::_mm256_min_pd(*operand1, *operand2);
}

_INLINE_ void simd_max(simd4xf32* operand1, simd4xf32* operand2, simd4xf32* result)
{
    *result = WindowsMsvcNs::_mm_max_ps(*operand1, *operand2);
}

_INLINE_ void simd_max(simd4xf64* operand1, simd4xf64* operand2, simd4xf64* result)
{
    *result = WindowsMsvcNs::_mm256_max_pd(*operand1, *operand2);
}
