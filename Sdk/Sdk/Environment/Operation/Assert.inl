#pragma once

namespace _InternalNs
{

template<typename JudgeTy>
_INLINE_ void assert_func(const JudgeTy& exp, const char* s, const char* file, s64 line)
{
    if (!!!(exp))
    {
        exception ex;
        ex.error_code = ASSERT_EXCEPTION_CODE;
        ex.expect_condition = s;
        ex.information = nullptr;
        ex.file = file;
        ex.line = line;

        print("[ERROR] Assert Failed. Condition '%s' in file: %s at line: %lld\n", s, file, line);
        throw ex;
    }
}

template<typename JudgeTy>
_INLINE_ void assert_func(const JudgeTy& exp, const char* s, const char* info, const char* file, s64 line)
{
    if (!!!(exp))
    {
        exception ex = {};
        ex.error_code = ASSERT_EXCEPTION_CODE;
        ex.expect_condition = s;
        ex.information = info;
        ex.file = file;
        ex.line = line;

        print("[ERROR] Assert Failed. Condition '%s', Info '%s' in file: %s at line: %lld\n", s, info, file, line);
        throw ex;
    }
}

}
