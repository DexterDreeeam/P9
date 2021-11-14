
#include "../EnvironmentHeader.hpp"
#include "../../Interface.hpp"

u64 random::new_u64() noexcept
{
    u64 rst = 0;
    LARGE_INTEGER ticks;
    for (s64 i = 0; i < 16; ++i)
    {
        yield();
        ::QueryPerformanceCounter(&ticks);
        rst = rst << 4;
        rst |= ticks.QuadPart & 0b00001111;
    }
    return rst;
}

date date::now()
{
    SYSTEMTIME sys;
    ::GetLocalTime(&sys);

    date rst;
    rst.year = sys.wYear;
    rst.month = sys.wMonth;
    rst.day = sys.wDay;
    rst.week_day = sys.wDayOfWeek;
    rst.hour = sys.wHour;
    rst.minute = sys.wMinute;
    rst.second = sys.wSecond;
    rst.millisec = sys.wMilliseconds;
    return rst;
}

date date::now_utc()
{
    SYSTEMTIME sys;
    ::GetSystemTime(&sys);

    date rst;
    rst.year = sys.wYear;
    rst.month = sys.wMonth;
    rst.day = sys.wDay;
    rst.week_day = sys.wDayOfWeek;
    rst.hour = sys.wHour;
    rst.minute = sys.wMinute;
    rst.second = sys.wSecond;
    rst.millisec = sys.wMilliseconds;
    return rst;
}

u64 date::timestamp()
{
    time_t time_utc;
    ::time(&time_utc);
    return (u64)time_utc;
}

void tick::sleep(u64 ms)
{
    ::Sleep(ms);
}

u64 tick::count()
{
    return (u64)::GetTickCount64();
}
