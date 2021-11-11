#pragma once

_INLINE_ s64   s32_to_text(s32 v, char* text);
_INLINE_ s64   u32_to_text(u32 v, char* text);
_INLINE_ s64   u32_to_text_padding(u32 v, char* text, s64 holds);
_INLINE_ s64   s64_to_text(s64 v, char* text);
_INLINE_ s64   u64_to_text(u64 v, char* text);
_INLINE_ s64   u64_to_text_padding(u64 v, char* text, s64 holds);
_INLINE_ s64   u64_to_text_hex(u64 v, char* text);
_INLINE_ s64   u64_to_text_hex_padding(u64 v, char* text, s64 holds);
_INLINE_ boole text_to_s64(const char* text, s64 len, _OUT_ s64& rst);
