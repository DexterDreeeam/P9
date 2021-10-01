#pragma once

#include "Print.h"

#define ASSERT_EXCEPTION_CODE (0x09090909L)

#if DEBUG_LEVEL >= DEBUG_LEVEL_CALIBRATION_CHECK_BASIC

namespace WindowsMsvcNs
{

template<typename JudgeTy>
_INLINE_ void dxt_assert(const JudgeTy& exp, const char* s, const char* file, s64 line)
{
    if (!!!(exp))
    {
        print("[ERROR] Assert Failed. Condition '%s' in file: %s at line: %lld\n", s, file, line);
        throw (ASSERT_EXCEPTION_CODE);
    }
}
template<typename JudgeTy>
_INLINE_ void dxt_assert(const JudgeTy& exp, const char* s, const char* info, const char* file, s64 line)
{
    if (!!!(exp))
    {
        print("[ERROR] Assert Failed. Condition '%s', Info '%s' in file: %s at line: %lld\n", s, info, file, line);
        throw (ASSERT_EXCEPTION_CODE);
    }
}

}

//# assert check input parameter
#define assert(exp)           WindowsMsvcNs::dxt_assert(exp, #exp, __FUNCTION__, __LINE__)
#define assert_info(exp,info) WindowsMsvcNs::dxt_assert(exp, #exp, info, __FUNCTION__, __LINE__)

#else

#define assert(exp) ((void)0)
#define assert_info(exp, info) ((void)0)

#endif
