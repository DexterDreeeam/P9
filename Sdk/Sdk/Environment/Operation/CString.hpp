#pragma once

#include "Memory.hpp"

_INLINE_ _NOALIAS_ boole char_equal_case_insensitive(char c1, char c2) noexcept;
_INLINE_ _NOALIAS_ s64   str_len(const char* s) noexcept;
_INLINE_ _NOALIAS_ boole str_equal(const char* s1, const char* s2) noexcept;
_INLINE_ _NOALIAS_ boole str_equal_case_insensitive(const char* s1, const char* s2) noexcept;
_INLINE_ _NOALIAS_ s64   str_compare(const char* s1, const char* s2) noexcept;
_INLINE_ _NOALIAS_ boole is_blank_char(char c) noexcept;
_INLINE_ _NOALIAS_ boole is_alike_blank_char(char c) noexcept;
_INLINE_ _NOALIAS_ void  str_pick_out_blank(_OUT_ char* s) noexcept;
