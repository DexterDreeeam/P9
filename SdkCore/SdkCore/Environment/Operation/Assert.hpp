#pragma once

#define ASSERT_EXCEPTION_CODE (0x09090909L)

struct exception
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
_INLINE_ void assert_func(const JudgeTy& exp, const char* s, const char* file, s64 line);

template<typename JudgeTy>
_INLINE_ void assert_func(const JudgeTy& exp, const char* s, const char* info, const char* file, s64 line);

}

#if DEBUG_LEVEL >= DEBUG_LEVEL_CALIBRATION_CHECK_BASIC

#define assert(exp)           _InternalNs::assert_func(exp, #exp, __FUNCTION__, __LINE__)
#define assert_info(exp,info) _InternalNs::assert_func(exp, #exp, info, __FUNCTION__, __LINE__)

#else

#define assert(exp) ((void)0)
#define assert_info(exp, info) ((void)0)

#endif
