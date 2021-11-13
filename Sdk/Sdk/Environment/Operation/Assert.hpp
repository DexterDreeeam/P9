#pragma once

#define P9_ASSERT_EXCEPTION_CODE (0x09090909L)

struct P9_assert_exception
{
    s64 error_code;
    const char* expect_condition;
    const char* information;
    const char* file;
    s64 line;
};

namespace _InternalNs
{

template<typename JudgeTy>
_INLINE_ void p9_assert(const JudgeTy& exp, const char* s, const char* file, s64 line);

template<typename JudgeTy>
_INLINE_ void p9_assert(const JudgeTy& exp, const char* s, const char* info, const char* file, s64 line);

}

#if DEBUG_LEVEL >= DEBUG_LEVEL_CALIBRATION_CHECK_BASIC

#define assert(exp)           _InternalNs::p9_assert(exp, #exp, __FUNCTION__, __LINE__)
#define assert_info(exp,info) _InternalNs::p9_assert(exp, #exp, info, __FUNCTION__, __LINE__)

#else

#define assert(exp) ((void)0)
#define assert_info(exp, info) ((void)0)

#endif
