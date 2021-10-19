#pragma once

#include "Print.hpp"

#if DEBUG_LEVEL >= DEBUG_LEVEL_CALIBRATION_CHECK_BASIC

#define P9_ASSERT_EXCEPTION_CODE (0x09090909L)

struct P9_assert_exception
{
    s64 error_code;
    const char* expect_condition;
    const char* information;
    const char* file;
    s64 line;
};

namespace LinuxGccNs
{

template<typename JudgeTy>
_INLINE_ void p9_assert(const JudgeTy& exp, const char* s, const char* file, s64 line)
{
    if (!!!(exp))
    {
        P9_assert_exception ex;
        ex.error_code = P9_ASSERT_EXCEPTION_CODE;
        ex.expect_condition = s;
        ex.information = nullptr;
        ex.file = file;
        ex.line = line;

        print("[ERROR] Assert Failed. Condition '%s' in file: %s at line: %lld\n", s, file, line);
        throw ex;
    }
}

template<typename JudgeTy>
_INLINE_ void p9_assert(const JudgeTy& exp, const char* s, const char* info, const char* file, s64 line)
{
    if (!!!(exp))
    {
        P9_assert_exception ex = {};
        ex.error_code = P9_ASSERT_EXCEPTION_CODE;
        ex.expect_condition = s;
        ex.information = info;
        ex.file = file;
        ex.line = line;

        print("[ERROR] Assert Failed. Condition '%s', Info '%s' in file: %s at line: %lld\n", s, info, file, line);
        throw ex;
    }
}

}

//# assert check input parameter
#define assert(exp)           LinuxGccNs::p9_assert(exp, #exp, __FUNCTION__, __LINE__)
#define assert_info(exp,info) LinuxGccNs::p9_assert(exp, #exp, info, __FUNCTION__, __LINE__)

#else

#define assert(exp) ((void)0)
#define assert_info(exp, info) ((void)0)

#endif
