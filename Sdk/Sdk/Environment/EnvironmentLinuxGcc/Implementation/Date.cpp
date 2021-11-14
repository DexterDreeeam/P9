
#include "../EnvironmentHeader.hpp"
#include "../../Interface.hpp"

u64 random::new_u64() noexcept
{
    u64 rst = 0;
    timespec ts;
    for (s64 i = 0; i < 16; ++i)
    {
        yield();
        ::clock_gettime(CLOCK_REALTIME, &ts);
        rst = rst << 4;
        rst |= (ts.tv_nsec & (0b00001111 << 4)) >> 4;
    }
    return rst;
}

date date::now()
{
    time_t timer = ::time(0);
    tm* local_time = ::localtime(&timer);
    date rst;
    rst.year = local_time->tm_year;
    rst.month = local_time->tm_mon;
    rst.day = local_time->tm_mday;
    rst.week_day = local_time->tm_wday;
    rst.hour = local_time->tm_hour;
    rst.minute = local_time->tm_min;
    rst.second = local_time->tm_sec;
    rst.millisec = 0;
    return rst;
}

date date::now_utc()
{
    time_t timer = ::time(0);
    tm* utc_time = ::localtime(&timer);
    date rst;
    rst.year = utc_time->tm_year;
    rst.month = utc_time->tm_mon;
    rst.day = utc_time->tm_mday;
    rst.week_day = utc_time->tm_wday;
    rst.hour = utc_time->tm_hour;
    rst.minute = utc_time->tm_min;
    rst.second = utc_time->tm_sec;
    rst.millisec = 0;
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
    ::usleep(ms * 1000);
}

u64 tick::count()
{
    timespec ts;
    ::clock_gettime(CLOCK_MONOTONIC, &ts);
    u64 ms = (u64)(ts.tv_nsec / 1000000) + ((u64)ts.tv_sec * 1000ull);
    return ms;
}
